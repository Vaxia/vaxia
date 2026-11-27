<?php

namespace Vaxia\Tests\Items\ItemDice;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Item dice: a character's approved items can be used on chat rolls. Bonuses add
 * to the rolled stat (the best item per stat, no stacking); "flat" overlaps
 * replace the stat.
 *
 * @group item_dice
 */
class ItemDiceTest extends VaxiaTestCase {

  use DiceTrait;

  public function testApprovedItemsAreOfferedWithTheirBonuses() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $sword = $this->characterItem($character, array('title' => 'Iron Sword', 'field_item_strength' => 2));
    $cloak = $this->characterItem($character, array('title' => 'Cloak', 'field_item_over_dexterity' => 40));

    $choices = $this->itemChoices($player, $this->site->createRoom(), $character);

    $this->assertSame('Iron Sword (str:2)', $choices[$sword->nid]);
    $this->assertSame('Cloak (dex:40 flat)', $choices[$cloak->nid]);
  }

  public function testUnapprovedItemsAreNotOffered() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $draft = $this->characterItem($character, array('field_item_strength' => 2), 'draft');
    $this->assertArrayNotHasKey($draft->nid, $this->itemChoices($player, $this->site->createRoom(), $character));
  }

  public function testItemsWithoutEffectsAreNotOffered() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $plain = $this->characterItem($character);
    $this->assertArrayNotHasKey($plain->nid, $this->itemChoices($player, $this->site->createRoom(), $character));
  }

  public function testItemBonusAddsToTheRoll() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_strength' => 5));
    $sword = $this->characterItem($character, array('title' => 'Iron Sword', 'field_item_strength' => 2));

    $lines = $this->postRoll($player, $this->site->createRoom(), $character, array(array('stat' => 'strength')), array($sword));

    $this->assertCount(1, $lines);
    $this->assertMatchesRegularExpression('/^str \(5\) \+ Items \(2\) => Might: 7 Roll: \d+$/', $lines[0]);
  }

  public function testBestItemWinsBonusesDontStack() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_strength' => 5));
    $small = $this->characterItem($character, array('field_item_strength' => 2));
    $big = $this->characterItem($character, array('field_item_strength' => 3));

    $lines = $this->postRoll($player, $this->site->createRoom(), $character, array(array('stat' => 'strength')), array($small, $big));

    $this->assertMatchesRegularExpression('/^str \(5\) \+ Items \(3\) => Might: 8 /', $lines[0]);
  }

  public function testFlatOverlapReplacesTheStat() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_dexterity' => 5));
    $cloak = $this->characterItem($character, array('field_item_over_dexterity' => 40));

    $lines = $this->postRoll($player, $this->site->createRoom(), $character, array(array('stat' => 'dexterity')), array($cloak));

    $this->assertMatchesRegularExpression('/Might: 40 /', $lines[0]);
  }

  public function testBonusOnlyAppliesToItsStat() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_strength' => 5, 'field_dexterity' => 4));
    $sword = $this->characterItem($character, array('field_item_strength' => 2));

    $lines = $this->postRoll($player, $this->site->createRoom(), $character, array(array('stat' => 'dexterity')), array($sword));

    $this->assertMatchesRegularExpression('/Might: 4 /', $lines[0]);
  }

}
