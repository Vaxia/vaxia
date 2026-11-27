<?php

namespace Vaxia\Tests\Vaxia\PowerSearchUnfiltered;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Leads' unfiltered power search (D7 view tolex_power_search, display
 * search_unfiltered): posts in every chat room, hidden ones included.
 *
 * The D7 test site doesn't carry this view, so this runs on D11 only.
 *
 * @group power_search_unfiltered
 */
class PowerSearchUnfilteredTest extends VaxiaTestCase {

  protected function setUp(): void {
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site lacks the power search view.');
    }
    parent::setUp();
  }

  private function lead() {
    $account = $this->site->createUser(array('lead'));
    $this->site->removeRole($account, 'new account');
    return $account;
  }

  private function rooms() {
    $rooms = array();
    foreach ($this->xpath()->query('//table//tbody/tr[td]') as $row) {
      $rooms[] = trim($row->getElementsByTagName('td')->item(0)->textContent);
    }
    return $rooms;
  }

  public function testIsForLeads() {
    $this->login($this->lead());
    $this->get('search/unfiltered');
    $this->assertStatus(200);
    $this->assertText('Lead Search (unfiltered)');
    $this->assertText('This search page is the UNFILTERED lead only search so we can monitor all posts.');
    $this->assertRaw('href="/search/search"');
    $this->login($this->site->createUser(array('administrator')));
    $this->get('search/unfiltered');
    $this->assertStatus(200);
    foreach (array('storyteller', 'player') as $role) {
      $account = $this->site->createUser(array($role));
      $this->login($account);
      $this->get('search/unfiltered');
      $this->assertStatus(403);
    }
  }

  public function testFindsPostsInHiddenRoomsNewestFirst() {
    $author = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($author, array('title' => $this->site->uniqueName('Hero'), 'field_visibility' => 0));
    $word = $this->site->uniqueName('platypus');
    $rooms = array(
      $this->site->createRoom(array('title' => $this->site->uniqueName('Private'), 'field_private_room' => 0)),
      $this->site->createRoom(array('title' => $this->site->uniqueName('Mature'), 'field_private_room' => 1, 'field_mature_posting' => 1)),
      $this->site->createRoom(array('title' => $this->site->uniqueName('Locked'), 'field_private_room' => 1, 'field_locked_to_users' => 1)),
    );
    $time = 1500000000;
    foreach ($rooms as $room) {
      $comment = $this->site->postComment($room, $author, 'A ' . $word . ' was here', $character);
      $this->site->backdateComment($comment, $time += 60);
    }
    $limbo = $this->site->createRoom(array('title' => 'Limbo'));
    $this->site->postComment($limbo, $author, 'Limbo ' . $word, $character);
    $this->site->postComment($rooms[0], $author, 'Nothing to see');
    $this->site->expireViewsCache();

    $this->login($this->lead());
    $this->get('search/unfiltered', array('comment_body_value' => $word));
    $this->assertSame(array($rooms[2]->title, $rooms[1]->title, $rooms[0]->title), $this->rooms());
    $this->assertText($character->title);
    $this->assertNoText('Nothing to see');
  }

  public function testPagesAtThirtyFive() {
    $author = $this->site->createUser(array('player'));
    $room = $this->site->createRoom(array('title' => $this->site->uniqueName('Busy')));
    $word = $this->site->uniqueName('echidna');
    for ($i = 0; $i < 36; $i++) {
      $this->site->postComment($room, $author, $word . ' ' . $i);
    }
    $this->login($this->lead());
    $this->get('search/unfiltered', array('comment_body_value' => $word));
    $this->assertCount(35, $this->rooms());
    $this->get('search/unfiltered', array('comment_body_value' => $word, 'page' => 1));
    $this->assertCount(1, $this->rooms());
  }

  public function testNeedsWords() {
    $this->login($this->lead());
    $this->get('search/unfiltered');
    $this->assertText('Enter words to search for.');
  }

}
