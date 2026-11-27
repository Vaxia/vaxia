<?php

namespace Vaxia\Tests\Vaxia\VaxiaEvaluation;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia evaluation: character evaluation mail. Submitting a character mails the
 * evaluators and tells the player; approval mails the player and makes them a
 * "player" (no longer a "new account"); sending back to draft or commenting
 * mails the player. Two checks return a submission to draft: players with
 * under 41 XP over time can't submit "Other" races or conditions, and players
 * whose submissions are mostly (over half) min-max flagged are sent back when
 * this one is flagged too.
 *
 * @group vaxia_evaluation
 */
class VaxiaEvaluationTest extends VaxiaTestCase {

  private function mailsTo($account, $subject) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($account, $subject) {
      return $m['to'] === $account->mail && $m['subject'] === $subject;
    }));
  }

  private function submit($character, $owner) {
    $this->transition($character, 'draft', $owner);
    return $this->transition($character, 'submitted', $owner);
  }

  public function testSubmittingNotifiesEvaluatorsAndPlayer() {
    $admin = $this->site->createUser(array('administrator'));
    $owner = $this->site->createUser(array('new account'), array(), array('field_xp_over_time' => 50));
    $character = $this->site->createCharacter($owner, array('title' => $this->site->uniqueName('Hero')));

    $messages = $this->submit($character, $owner);

    $this->assertStringContainsString('Your character ' . $character->title . ' has been submitted!', implode(' ', $messages));
    $this->assertCount(1, $this->mailsTo($admin, ' [Vaxia] A character is ready to be reviewed'));
    $this->assertSame('submitted', $this->workflowState($character));
  }

  public function testApprovalMakesAPlayerAndMailsThem() {
    $owner = $this->site->createUser(array('new account'), array(), array('field_xp_over_time' => 50));
    $character = $this->site->createCharacter($owner);
    $this->submit($character, $owner);

    $this->transition($character, 'approved');

    $roles = $this->rolesOf($owner);
    $this->assertContains('player', $roles);
    $this->assertNotContains('new account', $roles);
    $this->assertCount(1, $this->mailsTo($owner, '[Vaxia] Your character has been approved'));
  }

  public function testReturningToDraftMailsThePlayer() {
    $owner = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 50));
    $character = $this->site->createCharacter($owner);
    $this->submit($character, $owner);
    $this->transition($character, 'draft');
    $this->assertCount(1, $this->mailsTo($owner, '[Vaxia] Your new character needs changes made'));
  }

  public function testWorkflowCommentMailsThePlayer() {
    $owner = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 50));
    $character = $this->site->createCharacter($owner);
    $this->submit($character, $owner);
    // A comment without a move: "submitted" again, with a comment.
    $this->transition($character, 'submitted', NULL, 'Please fix your skills.');
    $this->assertCount(1, $this->mailsTo($owner, '[Vaxia] Your character has received a workflow comment'));
  }

  public function testOtherRaceUnder41XpIsSentBack() {
    $owner = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 10));
    $character = $this->site->createCharacter($owner, array('field_fantasy_species' => 'other'));
    $messages = $this->submit($character, $owner);
    $this->assertSame('draft', $this->workflowState($character));
    $this->assertStringContainsString('This character has a race set to Other or a Condition.', implode(' ', $messages));
  }

  public function testOtherRaceWithEnoughXpStaysSubmitted() {
    $owner = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 41));
    $character = $this->site->createCharacter($owner, array('field_fantasy_species' => 'other'));
    $this->submit($character, $owner);
    $this->assertSame('submitted', $this->workflowState($character));
  }

  public function testMostlyMinMaxedPlayersAreSentBack() {
    $owner = $this->site->createUser(array('player'), array(), array(
      'field_xp_over_time' => 50, 'field_characters_submitted' => 4, 'field_characters_minmax' => 3,
    ));
    // All stats at zero: flagged as min-maxed.
    $character = $this->site->createCharacter($owner);
    $messages = $this->submit($character, $owner);
    $this->assertSame('draft', $this->workflowState($character));
    $this->assertStringContainsString('More than half of your character submissions to date have been flagged under the min-max rules.', implode(' ', $messages));
  }

  public function testFewSubmissionsAreNotCheckedForMinMax() {
    $owner = $this->site->createUser(array('player'), array(), array(
      'field_xp_over_time' => 50, 'field_characters_submitted' => 1, 'field_characters_minmax' => 1,
    ));
    $character = $this->site->createCharacter($owner);
    $this->submit($character, $owner);
    $this->assertSame('submitted', $this->workflowState($character));
  }

}
