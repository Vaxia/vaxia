<?php

namespace Vaxia\Tests\Rpg\SuspendPlayers;

use Vaxia\Tests\StrikeFormTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Suspensions: a Red strike suspends and blocks the player for a period that
 * grows with each Red in the last year; enough Yellows in a year become a Red;
 * suspensions should lift once the ban date passes.
 *
 * @group suspend_players
 */
class SuspendPlayersTest extends VaxiaTestCase {

  use StrikeFormTrait;

  private function statusMessages() {
    return implode("\n", $this->messages('status'));
  }

  // ---------------------------------------------------------------------------
  // Red strikes.
  // ---------------------------------------------------------------------------

  public function testRedStrikeSuspendsAndBlocks() {
    $player = $this->site->createUser(array('player', 'storyteller', 'voter'));
    $this->fileStrike($player, 'Red');
    $this->assertTrue($this->isBlocked($player));
    $this->assertSame(array('suspended user'), $this->rolesOf($player), 'Every other role removed');
  }

  public function testSuspensionMovesApprovedCharactersBackToDraft() {
    $player = $this->site->createUser(array('player'));
    $approved = $this->site->createCharacter($player);
    $this->approveCharacter($approved);
    $submitted = $this->site->createCharacter($player);
    $this->transition($submitted, 'draft');
    $this->transition($submitted, 'submitted');
    // Strikes come long after approval; keep workflow history in order.
    $this->site->ageWorkflowHistory($approved);
    $this->site->ageWorkflowHistory($submitted);

    $this->fileStrike($player, 'Red');

    $this->assertSame('draft', $this->workflowState($approved));
    $this->assertSame('submitted', $this->workflowState($submitted), 'Only approved characters move');
  }

  public function testMessageCountsRedsInTheLastYear() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Red', '-14 months');
    $this->fileStrike($player, 'Red', '-3 months');
    $this->fileStrike($player, 'Red');
    $this->assertStringContainsString('This is Red strike #2 in the last year. ' . $player->name . ' has been suspended', $this->statusMessages());
  }

  /**
   * @group knownbug
   * check_reds returns the new ban as a formatted string ("12/22/2026"), which
   * Entity API rejects for field_banned_until (it wants a timestamp): no ban
   * date is ever saved, so the suspension has no end.
   */
  public function testFirstRedSetsBanDateOneMonthOut() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Red');
    $this->assertStringContainsString('Banned until: ' . date('l, F j, Y', strtotime('+1 month')), $this->profileText($player));
  }

  /**
   * @group knownbug
   * Same cause: the second Red in a year should ban for 3 months.
   */
  public function testSecondRedInAYearBansForThreeMonths() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Red', '-2 months');
    $this->fileStrike($player, 'Red');
    $this->assertStringContainsString('Banned until: ' . date('l, F j, Y', strtotime('+3 months')), $this->profileText($player));
  }

  /**
   * @group knownbug
   * The message's [new-ban-months] and [new-ban] tokens render empty
   * ("suspended for  months thru ."); the Red count works.
   */
  public function testSuspensionMessageStatesLengthAndDate() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Red', '-2 months');
    $this->fileStrike($player, 'Red');
    $this->assertStringContainsString('has been suspended for 3 months thru ' . date('m/d/Y', strtotime('+3 months')), $this->statusMessages());
  }

  // ---------------------------------------------------------------------------
  // Yellow strikes.
  // ---------------------------------------------------------------------------

  public function testFirstYellowDoesNotSuspend() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Yellow', 'now', $this->site->createNotation($player));
    $this->assertFalse($this->isBlocked($player));
  }

  public function testThirdYellowInAYearBecomesRedAndSuspends() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Yellow', '-5 months', $this->site->createNotation($player));
    $this->fileStrike($player, 'Yellow', '-2 months', $this->site->createNotation($player));
    $notation = $this->site->createNotation($player);

    $this->fileStrike($player, 'Yellow', 'now', $notation);

    $this->assertTrue($this->isBlocked($player), 'Player suspended');
    $this->assertStringContainsString('Yellow upgraded to Red, 3rd Yellow w/in year', $this->nodeText($notation));
  }

  public function testYellowsOlderThanAYearDontCount() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Yellow', '-20 months', $this->site->createNotation($player));
    $this->fileStrike($player, 'Yellow', '-14 months', $this->site->createNotation($player));
    $this->fileStrike($player, 'Yellow', 'now', $this->site->createNotation($player));
    $this->assertFalse($this->isBlocked($player));
  }

  /**
   * @group knownbug
   * check_yellows drops counts below 2 and the count includes the strike just
   * saved, so the 2nd Yellow is upgraded, not the 3rd.
   */
  public function testSecondYellowIsNotUpgraded() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Yellow', '-2 months', $this->site->createNotation($player));
    $this->fileStrike($player, 'Yellow', 'now', $this->site->createNotation($player));
    $this->assertFalse($this->isBlocked($player));
  }

  // ---------------------------------------------------------------------------
  // Suspension ends.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * "Suspend - Unblock a suspended user" listens for user_stats_day_older,
   * fired by cron. Under Rules 2 that event can't deliver a user (variables
   * typed 'number', Rules-1-style handlers), so suspensions never lift.
   */
  public function testCronLiftsSuspensionAfterBanDate() {
    $account = $this->site->createUser(array('suspended user'), array(), array(
      'status' => 0,
      'field_banned_until' => date('Y-m-d 00:00:00', strtotime('-1 day')),
    ));
    // A day has passed for this account since cron last ran.
    $this->site->backdateAccountCreated($account, strtotime('-3 days'));
    $this->site->setLastCron(strtotime('-1 day'));

    $this->runCron();

    $this->assertFalse($this->isBlocked($account), 'Unblocked');
    $this->assertContains('player', $this->rolesOf($account));
  }

}
