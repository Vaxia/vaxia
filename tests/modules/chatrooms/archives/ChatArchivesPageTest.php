<?php

namespace Vaxia\Tests\Chatrooms\Archives;

use Vaxia\Tests\VaxiaTestCase;

/**
 * A room's "Earlier Posts" tab, node/N/chat-archives (D7 view chat_archives):
 * the room's posts, newest first, 35 a page, optionally between two dates.
 * Only on chat rooms. Unlike D7, mature and locked rooms keep their posts
 * from those who may not open the room.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group archives
 * @group views_pages
 */
class ChatArchivesPageTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
  }

  /**
   * Post texts on the last page, in order.
   */
  private function posts() {
    preg_match_all('/post-text-(\d+)/', $this->pageText(), $m);
    // Each post shows its text as subject and body.
    return array_values(array_unique(array_map('intval', $m[1])));
  }

  public function testTabOnRoomsOnly() {
    $room = $this->site->createRoom();
    $page = $this->site->createNode('page');
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $room->nid);
    $this->assertRaw('/node/' . $room->nid . '/chat-archives');
    $this->assertText('Earlier Posts');
    $this->get('node/' . $room->nid . '/chat-archives');
    $this->assertStatus(200);
    $this->assertText('Chat Archives');
    $this->get('node/' . $page->nid . '/chat-archives');
    $this->assertStatus(403);
  }

  public function testPostsNewestFirstThirtyFiveAPage() {
    $room = $this->site->createRoom();
    $author = $this->site->createUser(array('player'));
    for ($i = 1; $i <= 36; $i++) {
      $this->site->postComment($room, $author, 'post-text-' . $i);
    }
    $other = $this->site->createRoom();
    $this->site->postComment($other, $author, 'post-text-99');

    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $room->nid . '/chat-archives');
    $this->assertSame(range(36, 2), $this->posts());
    $this->get('node/' . $room->nid . '/chat-archives', array('page' => 1));
    $this->assertSame(array(1), $this->posts());
  }

  public function testDateFilter() {
    $room = $this->site->createRoom();
    $author = $this->site->createUser(array('player'));
    $old = $this->site->postComment($room, $author, 'post-text-1');
    $this->site->backdateComment($old, strtotime('2020-03-10 12:00'));
    $this->site->postComment($room, $author, 'post-text-2');

    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $room->nid . '/chat-archives', array('created' => array('min' => '2020-03-01', 'max' => '2020-03-31')));
    $this->assertSame(array(1), $this->posts());
    $this->assertText('Posted date');
    $this->get('node/' . $room->nid . '/chat-archives', array('created' => array('min' => 'not a date', 'max' => '')));
    $this->assertText('Invalid date format.');
    $this->assertSame(array(), $this->posts());
  }

  public function testLockedRoomsStayClosed() {
    $listed = $this->site->createUser(array('player'));
    $room = $this->site->createRoom(array('field_locked_to_users' => 1, 'field_users' => array(array('uid' => $listed->uid))));
    $this->site->postComment($room, $listed, 'post-text-5');

    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $room->nid . '/chat-archives');
    $this->assertStatus(403);
    $this->login($listed);
    $this->get('node/' . $room->nid . '/chat-archives');
    $this->assertSame(array(5), $this->posts());
  }

  public function testMatureRoomsNeedAdults() {
    $room = $this->site->createRoom(array('field_mature_posting' => 1));
    $this->login($this->site->createUser(array('player'), array(), array('field_mature' => 0)));
    $this->get('node/' . $room->nid . '/chat-archives');
    $this->assertStatus(403);
    $this->login($this->site->createUser(array('player'), array(), array('field_mature' => 1)));
    $this->get('node/' . $room->nid . '/chat-archives');
    $this->assertStatus(200);
  }

}
