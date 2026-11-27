<?php

namespace Vaxia\Tests\Items\ItemWeapons;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Item weapons: items of type "Weapon" are labelled on the dice roller with
 * their weapon class ("Sword Weapon" from "Sword (10% / 2)"), and a fixed
 * strength replaces the character's strength on strength rolls.
 *
 * @group item_weapons
 */
class ItemWeaponsTest extends VaxiaTestCase {

  use DiceTrait;

  private function weapon($character, $class, $fixed = NULL, array $values = array()) {
    $values += array(
      'title' => $this->site->uniqueName('Blade'),
      'field_item_type' => $this->site->term('item_type', 'Weapon'),
      'field_weapon_class' => $this->site->term('weapon_category', $class),
    );
    if ($fixed !== NULL) {
      $values['field_fixed_strength'] = $fixed;
    }
    return $this->characterItem($character, $values);
  }

  public function testWeaponIsOfferedWithClassAndFixedStrength() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $blade = $this->weapon($character, 'Sword (10% / 2)', 30, array('title' => 'Longsword'));

    $choices = $this->itemChoices($player, $this->site->createRoom(), $character);

    $this->assertSame('Longsword (Sword Weapon fixed 30)', $choices[$blade->nid]);
  }

  public function testUnarmedClassHasNoLabel() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $fists = $this->weapon($character, 'Unarmed (0% / 0)');
    $this->assertArrayNotHasKey($fists->nid, $this->itemChoices($player, $this->site->createRoom(), $character));
  }

  public function testNonWeaponItemTypeGetsNoWeaponLabel() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $item = $this->characterItem($character, array(
      'field_item_type' => $this->site->term('item_type', 'Clothing'),
      'field_weapon_class' => $this->site->term('weapon_category', 'Sword (10% / 2)'),
      'field_fixed_strength' => 30,
    ));
    $this->assertArrayNotHasKey($item->nid, $this->itemChoices($player, $this->site->createRoom(), $character));
  }

  public function testFixedStrengthReplacesStrength() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_strength' => 5));
    $blade = $this->weapon($character, 'Sword (10% / 2)', 30, array('title' => 'Longsword'));

    $lines = $this->postRoll($player, $this->site->createRoom(), $character, array(array('stat' => 'strength')), array($blade));

    $this->assertCount(1, $lines);
    $this->assertMatchesRegularExpression('/Might: 30 /', $lines[0]);
  }

  public function testFixedStrengthDoesNotAffectOtherStats() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_dexterity' => 4));
    $blade = $this->weapon($character, 'Sword (10% / 2)', 30);

    $lines = $this->postRoll($player, $this->site->createRoom(), $character, array(array('stat' => 'dexterity')), array($blade));

    $this->assertMatchesRegularExpression('/Might: 4 /', $lines[0]);
  }

}
