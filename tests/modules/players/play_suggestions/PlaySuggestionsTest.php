<?php

namespace Vaxia\Tests\Players\PlaySuggestions;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Play suggestions: an "I'm bored." button (a block production shows in the
 * limbo rooms) makes the site post a random suggestion into the room: "In
 * <realm>, <suggestion> at <room>." The room is picked from that realm's active
 * rooms that are public, not mature and not locked. (field_private_room = 1
 * means "posts show in Power Search", i.e. public, despite its name.)
 *
 * Picks are random, so tests click several times and check every resulting post.
 *
 * @group play_suggestions
 */
class PlaySuggestionsTest extends VaxiaTestCase {

  /** @var object The room with the button. */
  private $limbo;

  /** @var string A realm of this test's own. */
  private $realm;

  protected function setUp(): void {
    parent::setUp();
    // Suggestions are picked among all of them; keep only this test's.
    $this->site->deleteNodesOfType('play_suggestions');
    $this->limbo = $this->site->createRoom(array('title' => $this->site->uniqueName('Limbo')));
    $this->site->placeBlock('play_suggestions', 'play_suggestions', 'content', 'node/' . $this->limbo->nid);
    $this->realm = $this->site->uniqueName('Realm');
    $this->site->createNode('play_suggestions', array(
      'title' => 'go fishing',
      'field_tag_realm' => $this->site->term('realm', $this->realm),
    ));
  }

  /**
   * An active room (one with a post) in this test's realm.
   */
  private function activeRoom(array $values = array()) {
    $room = $this->site->createRoom($values + array(
      'title' => $this->site->uniqueName('Room'),
      'field_tag_realm' => $this->site->term('realm', $this->realm),
      // 1 = public ("posts in this room show in Power Search").
      'field_private_room' => 1,
      'field_mature_posting' => 0,
      'field_locked_to_users' => 0,
    ));
    $this->site->postComment($room, $this->site->createUser(array('player')), 'Hello');
    return $room;
  }

  private function bored($times) {
    $this->login($this->site->createUser(array('player'), array('view rpg chat', 'view vaxia ic chat')));
    for ($i = 0; $i < $times; $i++) {
      $this->submitForm('node/' . $this->limbo->nid, array(), "I'm bored.");
    }
  }

  /**
   * Suggestion lines posted in limbo.
   */
  private function suggestions() {
    $this->get('node/' . $this->limbo->nid);
    preg_match_all('/In ' . preg_quote($this->realm, '/') . ', go fishing at ([^.]+)\./', $this->pageText(), $m);
    return $m[1];
  }

  public function testButtonShowsWhereTheBlockIs() {
    $this->login($this->site->createUser(array('player'), array('view rpg chat', 'view vaxia ic chat')));
    $this->get('node/' . $this->limbo->nid);
    $this->assertSame(1, $this->xpath()->query('//input[@value="I\'m bored."]')->length);
    $this->get('node/' . $this->site->createRoom()->nid);
    $this->assertSame(0, $this->xpath()->query('//input[@value="I\'m bored."]')->length);
  }

  public function testSuggestionNamesRealmSuggestionAndARoom() {
    $public = $this->activeRoom();
    $private = $this->activeRoom(array('field_private_room' => 0));

    $this->bored(4);

    $posted = $this->suggestions();
    $this->assertNotEmpty($posted, 'A suggestion was posted');
    foreach ($posted as $room) {
      $this->assertContains($room, array($public->title, $private->title));
    }
  }

  public function testMatureAndLockedRoomsAreNeverSuggested() {
    $this->activeRoom();
    $this->activeRoom(array('field_private_room' => 0));
    $mature = $this->activeRoom(array('field_mature_posting' => 1));
    $locked = $this->activeRoom(array('field_locked_to_users' => 1));

    $this->bored(4);

    $this->assertNotContains($mature->title, $this->suggestions());
    $this->assertNotContains($locked->title, $this->suggestions());
  }

  public function testNothingIsPostedWithoutARoomToSuggest() {
    $this->bored(2);
    $this->assertSame(array(), $this->suggestions());
  }

  public function testPublicRoomsAreSuggestedAndPrivateOnesNot() {
    $public = $this->activeRoom();
    $private = $this->activeRoom(array('field_private_room' => 0));

    $this->bored(4);

    $posted = $this->suggestions();
    $this->assertContains($public->title, $posted);
    $this->assertNotContains($private->title, $posted);
  }

  /**
   * @group knownbug
   * Each click should post one suggestion. The validate handler calls the submit
   * handler ("HACK"), then Drupal runs the submit handler too, so a click posts
   * two. And the room pick is rand(0, count) over count rooms, so some picks
   * miss and post nothing.
   */
  public function testEachClickPostsOneSuggestion() {
    $this->activeRoom();
    $this->activeRoom(array('field_private_room' => 0));

    $this->bored(5);

    $this->assertCount(5, $this->suggestions());
  }

}
