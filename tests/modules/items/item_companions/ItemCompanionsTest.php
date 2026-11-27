<?php

namespace Vaxia\Tests\Items\ItemCompanions;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Item companions: a character's "Companion/Drone" items leave the "Using items"
 * list and go into a "Using companion" select. Picking one (AJAX) adds a dice
 * roller for the companion, using the companion item's own stats; its rolls are
 * posted alongside the character's, marked "Companion".
 *
 * @group item_companions
 */
class ItemCompanionsTest extends VaxiaTestCase {

  use DiceTrait;

  private function companion($character, array $values = array()) {
    return $this->characterItem($character, $values + array(
      'title' => $this->site->uniqueName('Drone'),
      'field_item_type' => $this->site->term('item_type', 'Companion/Drone'),
      'field_item_strength' => 3,
      'field_strength' => 7,
    ));
  }

  private function companionChoices() {
    $options = array();
    foreach ($this->xpath()->query('//select[@name="vaxia_rolls[as_companion]"]/option') as $opt) {
      $options[$opt->getAttribute('value')] = trim($opt->textContent);
    }
    return $options;
  }

  public function testCompanionsMoveToTheirOwnSelect() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $drone = $this->companion($character, array('title' => 'Scout Drone'));
    $sword = $this->characterItem($character, array('field_item_strength' => 2));

    $items = $this->itemChoices($player, $this->site->createRoom(), $character);

    $this->assertArrayNotHasKey($drone->nid, $items, 'Not among items');
    $this->assertArrayHasKey($sword->nid, $items);
    $this->assertSame(array('_none' => '- None -', (string) $drone->nid => 'Scout Drone'), $this->companionChoices());
  }

  public function testNoCompanionSelectWithoutCompanions() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $this->characterItem($character, array('field_item_strength' => 2));
    $this->itemChoices($player, $this->site->createRoom(), $character);
    $this->assertSame(array(), $this->companionChoices());
  }

  public function testPickingACompanionAddsItsDiceRoller() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $drone = $this->companion($character);
    $this->itemChoices($player, $this->site->createRoom(), $character);

    $this->triggerAjax('Post', array(
      'field_comment_character[und]' => $character->nid,
      'vaxia_rolls[as_companion]' => $drone->nid,
    ), 'vaxia_rolls[as_companion]');

    $this->assertSame(1, $this->xpath()->query('//select[@name="vaxia_rolls[companions][dice][0][stat]"]')->length);
  }

  public function testCompanionRollIsPostedAndMarked() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_strength' => 5));
    $drone = $this->companion($character, array('title' => 'Scout Drone'));
    $room = $this->site->createRoom();
    $this->itemChoices($player, $room, $character);
    $text = $this->site->uniqueName('post');

    $this->triggerAjax('Post', array(
      'field_comment_character[und]' => $character->nid,
      'vaxia_rolls[as_companion]' => $drone->nid,
    ), 'vaxia_rolls[as_companion]');
    $this->submitForm(NULL, array(
      'comment_body[und][0][value]' => $text,
      'field_comment_character[und]' => $character->nid,
      'vaxia_rolls[dice][0][number]' => '1',
      'vaxia_rolls[dice][0][stat]' => 'strength',
      'vaxia_rolls[as_companion]' => $drone->nid,
      'vaxia_rolls[companions][dice][0][number]' => '1',
      'vaxia_rolls[companions][dice][0][stat]' => 'strength',
    ), 'Post');

    $lines = $this->diceLinesOf($text);
    $this->assertCount(2, $lines, implode(' | ', $lines));
    $this->assertMatchesRegularExpression('/^str \(5\) => Might: 5 /', $lines[0]);
    $this->assertMatchesRegularExpression('/^str \(7\) \+ Companion => Might: 7 Roll: \d+$/', $lines[1], 'Rolled with the companion\'s own stats');
  }

}
