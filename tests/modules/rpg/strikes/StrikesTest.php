<?php

namespace Vaxia\Tests\Rpg\Strikes;

use Vaxia\Tests\StrikeFormTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Strikes: social staff record notations (notes on player conduct). Approving a
 * notation whose results recommend a Yellow or Red strike issues that strike to
 * the player. Staff and players get pages listing notes and strikes.
 *
 * @group strikes
 */
class StrikesTest extends VaxiaTestCase {

  use StrikeFormTrait;

  private function notationWithResults($player, array $results, $body = 'Was rude in the tavern.') {
    return $this->site->createNotation($player, array(
      'field_results' => $results,
      'field_date_of_incident' => '2026-11-01 12:00:00',
      'body' => array(array('value' => $body, 'format' => 'plain_text')),
    ));
  }

  /**
   * Submits then approves the strike on a notation; returns the approval messages.
   */
  private function approveStrike($notation) {
    $this->transition($notation, 'strike submitted');
    return $this->transition($notation, 'strike approved');
  }

  /**
   * Strike titles listed for a player on the staff strike list.
   */
  private function strikeTitlesFor($player) {
    return $this->inSession($this->strikeFiler(), function () use ($player) {
      $this->get('notations/strikes');
      $titles = array();
      foreach ($this->xpath()->query('//a[contains(text(), "' . $player->name . '")]') as $a) {
        $titles[] = trim($a->textContent);
      }
      return $titles;
    });
  }

  private function strikePageFor($player, $severity) {
    return $this->inSession($this->strikeFiler(), function () use ($player, $severity) {
      $this->get('notations/strikes');
      $link = $this->xpath()->query('//a[contains(text(), "' . $severity . ' Strike - ' . $player->name . '")]')->item(0);
      $this->assertNotNull($link, "$severity strike listed");
      $this->get($link->getAttribute('href'));
      return $this->pageText();
    });
  }

  // ---------------------------------------------------------------------------
  // Strikes from notations.
  // ---------------------------------------------------------------------------

  public function testApprovedRedRecommendationIssuesRedStrike() {
    $player = $this->site->createUser(array('player'));
    $messages = $this->approveStrike($this->notationWithResults($player, array('red')));

    $this->assertContains('Based on your approval of this notation: A red strike has been issued to ' . $player->name . '.', $messages);
    $page = $this->strikePageFor($player, 'Red');
    $this->assertStringContainsString('Red (suspension)', $page);
    $this->assertStringContainsString('2026', $page, 'Date of incident copied');
  }

  public function testRedStrikeSuspendsThePlayer() {
    $player = $this->site->createUser(array('player'));
    $this->approveStrike($this->notationWithResults($player, array('red')));
    $this->assertTrue($this->isBlocked($player), 'Suspended by the Red strike rules');
  }

  public function testApprovedYellowRecommendationIssuesYellowStrike() {
    $player = $this->site->createUser(array('player'));
    $this->approveStrike($this->notationWithResults($player, array('training', 'yellow'), 'Spammed OOC.'));
    $page = $this->strikePageFor($player, 'Yellow');
    $this->assertStringContainsString('Details from Notation:', $page);
    $this->assertStringContainsString('Spammed OOC.', $page);
  }

  public function testNoStrikeWithoutStrikeRecommendation() {
    $player = $this->site->createUser(array('player'));
    $this->approveStrike($this->notationWithResults($player, array('training', 'apology')));
    $this->assertSame(array(), $this->strikeTitlesFor($player));
  }

  public function testGreenieDoesNotIssueStrike() {
    $player = $this->site->createUser(array('player'));
    $this->transition($this->notationWithResults($player, array('red')), 'greenie / no strike');
    $this->assertSame(array(), $this->strikeTitlesFor($player));
  }

  public function testRejectedStrikeIssuesNothing() {
    $player = $this->site->createUser(array('player'));
    $notation = $this->notationWithResults($player, array('red'));
    $this->transition($notation, 'strike submitted');
    $this->transition($notation, 'strike NOT approved');
    $this->assertSame(array(), $this->strikeTitlesFor($player));
  }

  /**
   * @group knownbug
   * The Yellow rule's message uses [strike-user:name], a variable only the Red
   * rule defines, so the player's name is missing from the confirmation.
   */
  public function testYellowMessageNamesThePlayer() {
    $player = $this->site->createUser(array('player'));
    $messages = $this->approveStrike($this->notationWithResults($player, array('yellow')));
    $this->assertContains('Based on your approval of this notation: A yellow strike has been issued to ' . $player->name . '.', $messages);
  }

  // ---------------------------------------------------------------------------
  // Pages.
  // ---------------------------------------------------------------------------

  public function testPlayerSeesOwnNotesPage() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('user/' . $player->uid . '/notations');
    $this->assertStatus(200);
  }

  public function testPlayerCannotSeeSomeoneElsesNotes() {
    $other = $this->site->createUser(array('player'));
    $this->login($this->site->createUser(array('player')));
    $this->get('user/' . $other->uid . '/notations');
    $this->assertStatus(403);
  }

  public function testNotationEditorsSeeAnyonesNotes() {
    $player = $this->site->createUser(array('player'));
    $this->login($this->site->createUser(array(), array('edit any notation content')));
    $this->get('user/' . $player->uid . '/notations');
    $this->assertStatus(200);
  }

  public function staffPages() {
    return array(array('notations'), array('notations/strikes'), array('notations/all'));
  }

  /**
   * @dataProvider staffPages
   */
  public function testLeadsSeeStaffPages($path) {
    $this->login($this->site->createUser(array('lead')));
    $this->get($path);
    $this->assertStatus(200);
  }

  /**
   * @dataProvider staffPages
   */
  public function testPlayersCannotSeeStaffPages($path) {
    $this->login($this->site->createUser(array('player')));
    $this->get($path);
    $this->assertStatus(403);
  }

  public function testStrikeListShowsFiledStrikes() {
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Yellow');
    $this->assertContains('Yellow strike - ' . $player->name, $this->strikeTitlesFor($player));
  }

  public function testNotesLogOpenToPlayers() {
    $this->login($this->site->createUser(array('player')));
    $this->get('notation-log');
    $this->assertStatus(200);
  }

}
