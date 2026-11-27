<?php

namespace Vaxia\Tests\Vaxia\VaxiaDice;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * The dice rows on a room's post form: only for characters the poster may
 * post as, and rolled once, when the post is new (review, 2026-11-26).
 *
 * @group vaxia_dice
 */
class DicePostFormTest extends VaxiaTestCase {

  use DiceTrait;

  public function testAForgedCharacterPickShowsNoOtherSheet() {
    // A player sends another player's character in the post form's AJAX
    // rebuild: the dice rows must not show that sheet's stats.
    $room = $this->site->createRoom();
    $player = $this->roller();
    $own = $this->rollingCharacter($player, array('field_dexterity' => 21));
    $victim = $this->rollingCharacter($this->roller(), array('title' => $this->site->uniqueName('Victim'), 'field_dexterity' => 37));
    $this->login($player);
    $this->get('node/' . $room->nid);
    $this->triggerAjax('Post', array('field_comment_character[und]' => $victim->nid), 'field_comment_character[und]', 'comment-form');
    $this->assertStringNotContainsString('Dexterity (37)', $this->content);
    $this->assertStringNotContainsString($victim->title, $this->content);
    // Their own character still works.
    $this->get('node/' . $room->nid);
    $this->triggerAjax('Post', array('field_comment_character[und]' => $own->nid), 'field_comment_character[und]', 'comment-form');
    $this->assertStringContainsString('Dexterity (21)', $this->content);
  }

  public function testEditingAPostDoesNotRollAgain() {
    $room = $this->site->createRoom();
    $player = $this->roller(array('edit own comments'));
    $character = $this->rollingCharacter($player);
    $text = $this->site->uniqueName('post');
    $lines = $this->postRoll($player, $room, $character, array(array('stat' => 'strength')), array(), $text);
    $this->assertCount(1, $lines);
    $this->get('node/' . $room->nid);
    $cid = $this->firstCommentId();
    $this->assertNotNull($cid);
    $this->submitForm('comment/' . $cid . '/edit', array(
      'comment_body[und][0][value]' => $text . ' (edited)',
      'vaxia_rolls[dice][0][number]' => '1',
      'vaxia_rolls[dice][0][stat]' => 'dexterity',
    ), 'Post');
    $this->get('node/' . $room->nid);
    $this->assertCount(1, $this->diceLinesOf($text), 'Still one roll after the edit');
  }

  public function testDerivedStatsCanBeRolled() {
    // The derived stats are in the roller, rounded down (review, 2026-11-26;
    // D7's roller had them).
    $room = $this->site->createRoom();
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_dexterity' => 32, 'field_intelligence' => 21));
    $this->login($player);
    $this->get('node/' . $room->nid);
    $this->assertSame(1, $this->xpath()->query('//select[@name="vaxia_rolls[dice][0][stat]"]/option[@value="reflexes"][normalize-space()="Finesse (26)"]')->length);
    $lines = $this->postRoll($player, $room, $character, array(array('stat' => 'reflexes')));
    $this->assertCount(1, $lines);
    $this->assertStringContainsString('fin (26)', $lines[0]);
  }

}
