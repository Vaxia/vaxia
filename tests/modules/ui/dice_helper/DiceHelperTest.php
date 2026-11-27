<?php

namespace Vaxia\Tests\Ui\DiceHelper;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Dice helper: adds a helper script to chat rooms for users allowed to use it,
 * and a "Diceroller proof" page charting how often each d100 result came up in
 * the last 1000 rolls, with their average.
 *
 * @group dice_helper
 */
class DiceHelperTest extends VaxiaTestCase {

  use DiceTrait;

  public function testHelperScriptLoadsInRooms() {
    $room = $this->site->createRoom();
    $this->login($this->site->createUser(array('player'), array('use dice helper', 'view rpg chat')));
    $this->get('node/' . $room->nid);
    $this->assertRaw('dice_helper/js/dice_helper.js');
  }

  public function testNoHelperScriptWithoutPermission() {
    $room = $this->site->createRoom();
    $this->withoutPermission('use dice helper', array('anonymous user', 'authenticated user', 'player'), function () use ($room) {
      $this->login($this->site->createUser(array('player'), array('view rpg chat')));
      $this->get('node/' . $room->nid);
      $this->assertNoRaw('dice_helper.js');
    });
  }

  public function testProofPageCountsRolls() {
    $this->site->deleteDiceRolls();
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $room = $this->site->createRoom();
    $this->postRoll($player, $room, $character, array(array('stat' => 'strength', 'number' => 1)));
    $this->postRoll($player, $room, $character, array(array('stat' => 'strength', 'number' => 1)));

    $this->get('dice_helper/dice_proof');

    $this->assertStatus(200);
    $this->assertText('Across 2 Rolls:');
    $this->assertText('Expected mathematical average of all rolls: 50 / 51');
    $this->assertMatchesRegularExpression('/Actual mathematical average of all rolls: \d+(\.\d+)?/', $this->pageText());
  }

  public function testProofPageNeedsViewDiceRolls() {
    $this->withoutPermission('view dice rolls', array('anonymous user', 'authenticated user', 'player'), function () {
      $this->login($this->site->createUser(array('player')));
      $this->get('dice_helper/dice_proof');
      $this->assertStatus(403);
    });
  }

  /**
   * @group knownbug
   * With no d100 rolls yet, the average divides by a zero count (a warning
   * today, a fatal DivisionByZeroError on PHP 8).
   */
  public function testProofPageWithNoRollsHasNoPhpErrors() {
    $this->site->deleteDiceRolls();
    $this->login($this->site->createUser(array('player'), array('view dice rolls')));
    $this->get('dice_helper/dice_proof');
    $this->assertSame(array(), $this->pageErrorsFrom('dice_helper.module'));
  }

}
