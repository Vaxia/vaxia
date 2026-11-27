<?php

namespace Vaxia\Tests\Rpg\RpgChat;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The Locations page, rpg-chats (D7 view rpg_chats): for logged-in users,
 * published rooms that are public (field_private_room = 1, "shows in Power
 * Search"), not mature and not locked, by title, 50 a page, filtered by
 * title and realm.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group rpg_chat
 * @group views_pages
 */
class RpgChatsPageTest extends VaxiaTestCase {

  /** @var string Prefix shared by this test's rooms. */
  private $prefix;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->prefix = $this->site->uniqueName('Loc');
  }

  private function room($name, array $values = array()) {
    return $this->site->createRoom($values + array(
      'title' => $this->prefix . ' ' . $name,
      'field_private_room' => 1,
      'field_mature_posting' => 0,
      'field_locked_to_users' => 0,
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ));
  }

  /**
   * Room titles listed for this test's prefix, in page order.
   */
  private function listed(array $query = array()) {
    $this->get('rpg-chats', $query + array('title' => $this->prefix));
    $this->assertStatus(200);
    $titles = array();
    foreach ($this->xpath()->query('//table//tbody/tr/td/a') as $a) {
      $titles[] = str_replace($this->prefix . ' ', '', trim($a->textContent));
    }
    return $titles;
  }

  public function testLoggedInUsersOnly() {
    $this->anonymous();
    $this->get('rpg-chats');
    $this->assertStatus(403);
    $this->login($this->site->createUser(array()));
    $this->get('rpg-chats');
    $this->assertStatus(200);
    $this->assertText('RPG Chats');
    $this->assertText('Chatroom');
  }

  public function testOnlyPublicOpenRoomsByTitle() {
    $this->room('Tavern');
    $this->room('Armory');
    $this->room('Hidden', array('field_private_room' => 0));
    $this->room('Adults', array('field_mature_posting' => 1));
    $this->room('Locked', array('field_locked_to_users' => 1));
    $this->room('Draft', array('status' => 0));
    $this->login($this->site->createUser(array('player')));
    $this->assertSame(array('Armory', 'Tavern'), $this->listed());
    $this->assertSame(array('Tavern', 'Armory'), $this->listed(array('order' => 'Chatroom', 'sort' => 'desc')));
  }

  public function testRoomsLinkToThemselves() {
    $room = $this->room('Docks');
    $this->login($this->site->createUser(array('player')));
    $this->listed();
    $this->assertRaw('href="/node/' . $room->nid . '"');
  }

  public function testTitleAndRealmFilters() {
    $sirian = $this->site->term('realm', 'Sirian');
    $this->room('Harbor');
    $this->room('Bazaar', array('field_tag_realm' => $sirian));
    $this->login($this->site->createUser(array('player')));
    $this->assertSame(array('Bazaar'), $this->listed(array('field_tag_realm_tid' => array($sirian))));
    $this->assertSame(array('Harbor'), $this->listed(array('title' => $this->prefix . ' Harb')));
    $this->assertSame(array(), $this->listed(array('title' => $this->prefix . ' Harb', 'field_tag_realm_tid' => array($sirian))));
  }

  public function testFiftyAPage() {
    for ($i = 1; $i <= 51; $i++) {
      $this->room(str_pad((string) $i, 2, '0', STR_PAD_LEFT));
    }
    $this->login($this->site->createUser(array('player')));
    $first = $this->listed();
    $this->assertCount(50, $first);
    $this->assertSame('01', $first[0]);
    $this->assertSame(array('51'), $this->listed(array('page' => 1)));
  }

}
