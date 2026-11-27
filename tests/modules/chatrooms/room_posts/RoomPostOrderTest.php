<?php

namespace Vaxia\Tests\Chatrooms\RoomPosts;

use Vaxia\Tests\VaxiaTestCase;

/**
 * A chat room lists its posts newest first, 30 a page (D7 comment_goodness,
 * newer first for rpg_chatroom), and a post's permalink, comment/N, opens the
 * page that shows it (D7 comment_goodness_permalink(): core assumed oldest
 * first, so a recent post's permalink opened the oldest page).
 *
 * The D7 test site has comment_goodness disabled (production had it on, with
 * newer first and 30 a page for rpg_chatroom), so these run on the port only.
 *
 * @group chatrooms
 */
class RoomPostOrderTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site has comment_goodness disabled.');
    }
  }

  /**
   * A room with 32 posts, oldest first: more than one page.
   */
  private function busyRoom() {
    $room = $this->site->createRoom();
    $player = $this->site->createUser(array('player'));
    $posts = array();
    for ($i = 1; $i <= 32; $i++) {
      $posts[] = $this->site->postComment($room, $player, 'room-post-' . $i);
    }
    return array($room, $posts, $player);
  }

  /**
   * The ids of the posts on the current page, in page order.
   */
  private function shownPosts() {
    $ids = array();
    foreach ($this->xpath()->query('//*[starts-with(@id, "comment-")]') as $el) {
      if (preg_match('/^comment-(\d+)$/', $el->getAttribute('id'), $m)) {
        $ids[] = (int) $m[1];
      }
    }
    return array_values(array_unique($ids));
  }

  public function testRoomShowsNewestFirst() {
    list($room, $posts, $player) = $this->busyRoom();
    $this->login($player);
    $this->get('node/' . $room->nid);
    $this->assertStatus(200);
    $shown = $this->shownPosts();
    $this->assertSame($posts[31]->cid, $shown[0] ?? NULL, 'Newest post first');
    $this->assertNotContains($posts[0]->cid, $shown, 'Oldest post is on a later page');
  }

  public function testPermalinkOpensThePageWithThePost() {
    list($room, $posts, $player) = $this->busyRoom();
    $this->login($player);
    // The newest post is on the first page, the oldest on the second.
    foreach (array($posts[31], $posts[0]) as $post) {
      $this->get('comment/' . $post->cid);
      $this->assertStatus(200);
      $this->assertContains($post->cid, $this->shownPosts(), 'Permalink to post ' . $post->cid . ' shows it');
    }
  }

}
