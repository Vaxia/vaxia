<?php

namespace Vaxia\Tests\Chatrooms\RoomCleanup;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Room cleanup: staff turn wiki articles into chat rooms, and a cleanup job
 * deletes old rooms nobody ever posted in (keeping parents of rooms in use).
 *
 * @group room_cleanup
 */
class RoomCleanupTest extends VaxiaTestCase {

  /** @var bool Whether this test left Apache stuck in a request. */
  private $apacheStuck = FALSE;

  protected function tearDown(): void {
    if ($this->apacheStuck) {
      // The abandoned request keeps looping server-side (set_time_limit() is
      // reset every pass). Kill Apache's worker processes; the parent (PID 1 in
      // the container) starts fresh ones. Not "apachectl restart": it signals
      // this process group too.
      exec('pkill -9 -P 1 apache2 2>&1');
      sleep(2);
    }
    parent::tearDown();
  }

  private function cleanupAdmin() {
    return $this->site->createUser(array(), array('administer room cleanup'));
  }

  private function oldRoom(array $values = array()) {
    $room = $this->site->createRoom($values);
    $this->site->backdateNodeCreated($room, strtotime('-2 years'));
    return $room;
  }

  private function nodeExists($node) {
    return $this->inSession($this->superuser(), function () use ($node) {
      $this->get('node/' . $node->nid);
      return $this->status === 200;
    });
  }

  // ---------------------------------------------------------------------------
  // Settings.
  // ---------------------------------------------------------------------------

  public function testSettingsPage() {
    $this->login($this->cleanupAdmin());
    $this->submitForm('admin/vaxia/room_cleanup', array('room_cleanup_date' => '18 months', 'room_cleanup_cron' => TRUE), 'Save configuration');
    $this->assertContains('The configuration options have been saved.', $this->messages('status'));
    $this->get('admin/vaxia/room_cleanup');
    $this->assertSame('18 months', $this->xpath()->query('//input[@name="room_cleanup_date"]')->item(0)->getAttribute('value'));
    $this->assertSame(1, $this->xpath()->query('//input[@name="room_cleanup_cron" and @checked]')->length);
    $this->assertSame(1, $this->xpath()->query('//input[@value="Cleanup Rooms"]')->length);
  }

  public function testSettingsNeedPermission() {
    $this->login($this->site->createUser(array('lead')));
    $this->get('admin/vaxia/room_cleanup');
    $this->assertStatus(403);
  }

  /**
   * @group knownbug
   * "Room cleanup last run" formats the on/off setting (room_cleanup_cron) as a
   * date: with cron switched on it reads "01/01/1970 - 00:00". The real last run
   * is saved under another name and never shown.
   */
  public function testLastRunIsNotTheEpoch() {
    $this->login($this->cleanupAdmin());
    $this->submitForm('admin/vaxia/room_cleanup', array('room_cleanup_cron' => TRUE), 'Save configuration');
    $this->get('admin/vaxia/room_cleanup');
    $this->assertNoText('01/01/1970');
  }

  // ---------------------------------------------------------------------------
  // Cleanup.
  // ---------------------------------------------------------------------------

  public function testCleanupWithNothingToDoKeepsRoomsInUse() {
    $used = $this->oldRoom();
    $this->site->postComment($used, $this->site->createUser(array('player')), 'Still here');
    $recent = $this->site->createRoom();

    $this->login($this->cleanupAdmin());
    $this->submitForm('admin/vaxia/room_cleanup', array(), 'Cleanup Rooms');

    $this->assertContains('Identified 0 rooms that may be cleaned up.', $this->messages('status'));
    $this->assertContains('Cleaned up 0 rooms.', $this->messages('status'));
    $this->assertTrue($this->nodeExists($used), 'Room with posts kept');
    $this->assertTrue($this->nodeExists($recent), 'New room kept');
  }

  /**
   * @group knownbug
   * The cleanup loop runs "while (no new parents found || rooms left to clean)":
   * once any room qualifies it never ends (set_time_limit(600) is reset every
   * pass), so the button and cron hang and nothing is deleted.
   */
  public function testCleanupDeletesOldEmptyRooms() {
    $empty = $this->oldRoom();
    $this->login($this->cleanupAdmin());
    $this->get('admin/vaxia/room_cleanup');
    $this->httpTimeout = 20;
    try {
      $this->submitForm(NULL, array(), 'Cleanup Rooms');
    }
    catch (\RuntimeException $e) {
      $this->apacheStuck = TRUE;
      $this->fail('Cleanup did not finish within 20 seconds: ' . $e->getMessage());
    }
    finally {
      $this->httpTimeout = 120;
    }
    $this->assertFalse($this->nodeExists($empty));
  }

  // ---------------------------------------------------------------------------
  // Making a room from a wiki article.
  // ---------------------------------------------------------------------------

  private function roomMaker() {
    return $this->site->createUser(array(), array('make room from wiki', 'create wiki_page content', 'create rpg_chatroom content', 'edit own rpg_chatroom content', 'view rpg chat', 'view vaxia ic chat'));
  }

  public function testWikiArticleOffersMakeRoomButton() {
    $wiki = $this->site->createNode('wiki_page', array('title' => 'Places/Harbor'));
    $this->login($this->roomMaker());
    $this->get('node/' . $wiki->nid);
    $this->assertSame(1, $this->xpath()->query('//input[@value="Make a Room from this article"]')->length);

    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $wiki->nid);
    $this->assertSame(0, $this->xpath()->query('//input[@value="Make a Room from this article"]')->length);
  }

  public function testMakeRoomUsesLastTitlePartAndFindsParentRoom() {
    $parentName = $this->site->uniqueName('Docks');
    $parent = $this->site->createRoom(array('title' => $parentName));
    $wiki = $this->site->createNode('wiki_page', array('title' => $parentName . '/Fish Market'));

    $this->login($this->roomMaker());
    $this->submitForm('node/' . $wiki->nid, array(), 'Make a Room from this article');

    $this->assertContains('Found suggested parent room ' . $parentName . '.', $this->messages('status'));
    // The handler sends you to the room's edit form, but you land on the room.
    $this->assertMatchesRegularExpression('#/node/(\d+)#', $this->url);
    preg_match('#/node/(\d+)#', $this->url, $m);
    $this->assertNotEquals($wiki->nid, (int) $m[1], 'A new room, not the article');

    $this->login($this->superuser());
    $this->get('node/' . $m[1] . '/edit');
    $xpath = $this->xpath();
    $this->assertSame('Fish Market', $xpath->query('//input[starts-with(@name, "title")]')->item(0)->getAttribute('value'));
    // Reference boxes name the node as "[nid:N]" (D7) or "(N)" (D11).
    $refers = '/\[nid:%d\]|\(%d\)$/';
    $this->assertMatchesRegularExpression(str_replace('%d', $parent->nid, $refers), $xpath->query('//input[starts-with(@name, "field_parent")]')->item(0)->getAttribute('value'));
    $this->assertMatchesRegularExpression(str_replace('%d', $wiki->nid, $refers), $xpath->query('//input[starts-with(@name, "field_wiki_source")]')->item(0)->getAttribute('value'));
  }

  /**
   * @group knownbug
   * The handler writes to an undefined $room ("Creating default object", fatal
   * "Attempt to assign property on null" on PHP 8).
   */
  public function testMakeRoomHasNoPhpErrors() {
    $wiki = $this->site->createNode('wiki_page', array('title' => 'Somewhere'));
    $this->login($this->roomMaker());
    $this->submitForm('node/' . $wiki->nid, array(), 'Make a Room from this article');
    $this->assertSame(array(), $this->pageErrorsFrom('room_cleanup.module'));
  }

}
