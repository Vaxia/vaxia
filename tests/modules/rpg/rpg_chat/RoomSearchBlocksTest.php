<?php

namespace Vaxia\Tests\Rpg\RpgChat;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The two room search blocks of D7 view rpg_chats, for logged-in users. Both
 * list published, unlocked, non-mature, public ("show in Power Search")
 * rooms whose name contains the words searched, the five most recently
 * active first, as "Room (Realm)".
 * - "Search RPG Chats" (search_rpg_chats): name and realms; shows nothing
 *   until a search is made.
 * - "In-Room Search RPG Chats" (inroom_rpg_chats): name only (required),
 *   with a link to Power Search.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group rpg_chat
 * @group views_blocks
 */
class RoomSearchBlocksTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'rpg_chats-search_rpg_chats', 'sidebar_first');
    $this->site->placeBlock('views', 'rpg_chats-inroom_rpg_chats', 'content');
  }

  /**
   * One block on the last page: rows, the empty text and the whole text;
   * NULL when it isn't shown.
   */
  private function block($display) {
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-' . $display . ' ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query('.//div[contains(@class, "view-content")]//li', $view->item(0)) as $li) {
      $rows[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
    }
    $empty = $xpath->query('.//div[contains(@class, "view-empty")]', $view->item(0))->item(0);
    return array(
      'rows' => $rows,
      'empty' => $empty ? trim($empty->textContent) : NULL,
      'text' => trim(preg_replace('/\s+/', ' ', $view->item(0)->textContent)),
    );
  }

  private function room($name, array $values = array()) {
    return $this->site->createRoom($values + array(
      'title' => $name,
      'field_locked_to_users' => 0,
      'field_mature_posting' => 0,
      'field_private_room' => 1,
    ));
  }

  public function testNothingUntilSearched() {
    $this->room($this->site->uniqueName('Tavern'));
    $this->login($this->site->createUser(array('player')));
    $this->get('node');
    $block = $this->block('search_rpg_chats');
    $this->assertSame(array(), $block['rows']);
    $this->assertNull($block['empty']);
    $this->assertStringContainsString('Search for a room by name.', $block['text']);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-search_rpg_chats")]//form[@method="get"]//input[@name="title"]')->length);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-search_rpg_chats")]//select[@name="field_tag_realm_tid[]" and @multiple]')->length);
  }

  public function testSearchByName() {
    $key = $this->site->uniqueName('Inn');
    $vaxia = $this->site->term('realm', 'Vaxia');
    $match = $this->room($key . ' Common Room', array('field_tag_realm' => $vaxia));
    $noRealm = $this->room('The ' . $key, array('field_tag_realm' => array()));
    $locked = $this->room($key . ' locked', array('field_locked_to_users' => 1));
    $mature = $this->room($key . ' mature', array('field_mature_posting' => 1));
    $private = $this->room($key . ' private', array('field_private_room' => 0));
    $unpublished = $this->room($key . ' unpublished', array('status' => 0));
    $this->room($this->site->uniqueName('Elsewhere'));
    $this->login($this->site->createUser(array('player')));
    $this->get('node', array('title' => strtolower($key)));
    $rows = $this->block('search_rpg_chats')['rows'];
    sort($rows);
    $this->assertSame(array($match->title . ' (Vaxia)', $noRealm->title), $rows);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-search_rpg_chats")]//a[@href="/node/' . $match->nid . '"]')->length);
    // The search box keeps what was searched.
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-search_rpg_chats")]//input[@name="title" and @value="' . strtolower($key) . '"]')->length);
  }

  public function testSearchByRealm() {
    $key = $this->site->uniqueName('Hall');
    $sirian = $this->site->term('realm', 'Sirian');
    $vaxia = $this->site->term('realm', 'Vaxia');
    $inSirian = $this->room($key . ' one', array('field_tag_realm' => $sirian));
    $this->room($key . ' two', array('field_tag_realm' => $vaxia));
    $this->login($this->site->createUser(array('player')));
    $this->get('node', array('title' => $key, 'field_tag_realm_tid' => array($sirian)));
    $this->assertSame(array($inSirian->title . ' (Sirian)'), $this->block('search_rpg_chats')['rows']);
    $this->assertSame(1, $this->xpath()->query('//select[@name="field_tag_realm_tid[]"]/option[@value="' . $sirian . '" and @selected]')->length);
  }

  public function testNoMatches() {
    $this->login($this->site->createUser(array('player')));
    $this->get('node', array('title' => $this->site->uniqueName('Nowhere')));
    $block = $this->block('search_rpg_chats');
    $this->assertSame(array(), $block['rows']);
    $this->assertSame('No rooms found matching this name, or no search performed.', $block['empty']);
  }

  public function testFiveMostRecentlyActive() {
    $key = $this->site->uniqueName('Keep');
    // Minutes since each room's last post.
    $idle = array(1 => 50, 2 => 10, 3 => 40, 4 => 20, 5 => 60, 6 => 30);
    $rooms = array();
    foreach ($idle as $i => $minutes) {
      $rooms[$i] = $this->room($key . ' ' . $i);
      $this->site->setLastCommentTime($rooms[$i], time() - 60 * $minutes);
    }
    $this->login($this->site->createUser(array('player')));
    $this->get('node', array('title' => $key));
    $this->assertSame(array($rooms[2]->title, $rooms[4]->title, $rooms[6]->title, $rooms[3]->title, $rooms[1]->title), $this->block('search_rpg_chats')['rows']);
  }

  public function testInRoomSearch() {
    $key = $this->site->uniqueName('Cellar');
    $room = $this->room($key . ' below', array('field_tag_realm' => $this->site->term('realm', 'Vaxia')));
    $this->login($this->site->createUser(array('player')));
    $this->get('node');
    $block = $this->block('inroom_rpg_chats');
    // The name is required: nothing until one is given.
    $this->assertSame(array(), $block['rows']);
    $this->assertSame('No rooms found matching this name, or no search performed.', $block['empty']);
    $this->assertStringContainsString('Search for a room by name.', $block['text']);
    $this->assertStringContainsString('Or use the Power Search', $block['text']);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-inroom_rpg_chats")]//a[@href="/search/search" and text()="Power Search"]')->length);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-inroom_rpg_chats")]//input[@name="inroom"]')->length);
    $this->assertSame(0, $this->xpath()->query('//div[contains(@class, "view-display-id-inroom_rpg_chats")]//select')->length);

    $this->get('node', array('inroom' => $key));
    $this->assertSame(array($room->title . ' (Vaxia)'), $this->block('inroom_rpg_chats')['rows']);
    // Each block reads its own box.
    $this->assertSame(array(), $this->block('search_rpg_chats')['rows']);
  }

  public function testOnlyForLoggedInUsers() {
    $this->anonymous();
    $this->get('user/login');
    $this->assertNull($this->block('search_rpg_chats'));
    $this->assertNull($this->block('inroom_rpg_chats'));
  }

}
