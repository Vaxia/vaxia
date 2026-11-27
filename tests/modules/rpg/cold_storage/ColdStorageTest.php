<?php

namespace Vaxia\Tests\Rpg\ColdStorage;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Cold storage: blocking an inactive player parks them (role "cold storage",
 * characters back to draft); unblocking brings them back as a player; returning
 * players reactivate themselves through a webform by email.
 *
 * @group cold_storage
 */
class ColdStorageTest extends VaxiaTestCase {

  /** Production nid of the "returning player" webform the rule is tied to. */
  const RETURN_FORM_NID = 11863;

  /**
   * Blocks or unblocks through the admin account form, as staff do.
   */
  private function setBlocked($account, $blocked) {
    $this->inSession($this->superuser(), function () use ($account, $blocked) {
      $this->submitForm('user/' . $account->uid . '/edit', array('status' => $blocked ? '0' : '1'), 'Save');
    });
  }

  private function frozenPlayer(array $values = array()) {
    return $this->site->createUser(array('cold storage'), array(), $values + array('status' => 0));
  }

  private function submittedCharacter($player) {
    $character = $this->site->createCharacter($player);
    $this->transition($character, 'draft');
    $this->transition($character, 'submitted');
    return $character;
  }

  // ---------------------------------------------------------------------------
  // Blocking.
  // ---------------------------------------------------------------------------

  public function testBlockingPlayerMovesThemToColdStorage() {
    $player = $this->site->createUser(array('player', 'storyteller', 'voter', 'lead'));
    $this->setBlocked($player, TRUE);
    // D7 also gave players "new account" at signup (KNOWN-BUGS #85); it stays.
    $this->assertSame(array('cold storage'), array_values(array_diff($this->rolesOf($player), array('new account'))));
  }

  public function testBlockingMovesApprovedCharactersToDraft() {
    $player = $this->site->createUser(array('player'));
    $approved = $this->site->createCharacter($player);
    $this->approveCharacter($approved);
    $submitted = $this->submittedCharacter($player);
    $this->site->ageWorkflowHistory($approved);
    $this->site->ageWorkflowHistory($submitted);

    $this->setBlocked($player, TRUE);

    $this->assertSame('draft', $this->workflowState($approved));
    $this->assertSame('submitted', $this->workflowState($submitted));
  }

  public function testBannedOrSuspendedUsersAreNotFrozen() {
    foreach (array('banned user', 'suspended user') as $role) {
      $account = $this->site->createUser(array('player', $role));
      $this->setBlocked($account, TRUE);
      $this->assertNotContains('cold storage', $this->rolesOf($account), "$role not frozen");
    }
  }

  public function testNonPlayersAreNotFrozen() {
    $account = $this->site->createUser(array('lead'));
    $this->setBlocked($account, TRUE);
    $this->assertSame(array('lead', 'new account'), $this->rolesOf($account));
  }

  // ---------------------------------------------------------------------------
  // Unblocking.
  // ---------------------------------------------------------------------------

  public function testUnblockingBringsPlayerBack() {
    $account = $this->frozenPlayer();
    $this->setBlocked($account, FALSE);
    $roles = $this->rolesOf($account);
    $this->assertContains('player', $roles);
    $this->assertNotContains('cold storage', $roles);
  }

  /**
   * Current behaviour, intent unconfirmed: the unblock rule's action is a copy of
   * the block rule's ("approved -> draft"), so characters stay in draft and
   * must be re-submitted.
   */
  public function testCharactersStayInDraftAfterUnblocking() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player);
    $this->approveCharacter($character);
    // Blocks happen long after approval; keep workflow history in order.
    $this->site->ageWorkflowHistory($character);
    $this->setBlocked($player, TRUE);
    $this->site->ageWorkflowHistory($character);
    $this->setBlocked($player, FALSE);
    $this->assertSame('draft', $this->workflowState($character));
  }

  public function testUnblockingBannedUserKeepsThemFrozen() {
    $account = $this->site->createUser(array('cold storage', 'banned user'), array(), array('status' => 0));
    $this->setBlocked($account, FALSE);
    $roles = $this->rolesOf($account);
    $this->assertContains('cold storage', $roles);
    $this->assertNotContains('player', $roles);
  }

  // ---------------------------------------------------------------------------
  // Returning players reactivate themselves by email.
  // ---------------------------------------------------------------------------

  private function emailComponent() {
    return array(1 => array(
      'cid' => 1, 'pid' => 0, 'form_key' => 'account_email', 'name' => 'Account email',
      'type' => 'email', 'value' => '', 'extra' => array(), 'mandatory' => 1, 'weight' => 0,
    ));
  }

  private function requestReturn($email) {
    // Anonymous (1) and authenticated (2) may submit.
    $this->site->createWebformAt(self::RETURN_FORM_NID, 'Returning players', array(1, 2), $this->emailComponent());
    $this->anonymous();
    $this->submitForm('node/' . self::RETURN_FORM_NID, array('submitted[account_email]' => $email), 'Submit');
  }

  public function testReturningPlayerReactivatesByEmail() {
    $account = $this->frozenPlayer();

    $this->requestReturn($account->mail);

    $text = $this->pageText();
    $this->assertMatchesRegularExpression('/Looking up accounts for the email address "\s*' . preg_quote($account->mail, '/') . '\s*"/', $text);
    $this->assertStringContainsString('Found account "' . $account->name . '"', $text);
    $this->assertStringContainsString('Done searching.', $text);
    $this->assertFalse($this->isBlocked($account), 'Unblocked');
    $this->assertContains('player', $this->rolesOf($account));
  }

  public function testReactivationEmailsThePlayer() {
    $account = $this->frozenPlayer();
    $this->requestReturn($account->mail);
    $this->assertContains($account->mail, $this->mailRecipients(), 'Account activated mail sent');
  }

  public function testEmailWithSurroundingSpacesStillMatches() {
    $account = $this->frozenPlayer();
    $this->requestReturn('  ' . $account->mail . '  ');
    $this->assertFalse($this->isBlocked($account));
  }

  public function testUnknownEmailOnlySaysDoneSearching() {
    $this->requestReturn('nobody@example.test');
    $this->assertText('Done searching.');
    $this->assertNoText('Found account');
  }

  /**
   * @group knownbug
   * The "not found" message lives in the view's "No results" area, which
   * views_rules never renders.
   */
  public function testUnknownEmailReportsNothingFound() {
    $this->requestReturn('nobody@example.test');
    $this->assertText('No account matching this email was found in cold storage.');
  }

  public function testReturnFormDoesNotReactivateBannedOrSuspended() {
    foreach (array('banned user', 'suspended user') as $role) {
      $account = $this->site->createUser(array('cold storage', $role), array(), array('status' => 0));
      $this->requestReturn($account->mail);
      $this->assertTrue($this->isBlocked($account), "$role stays blocked");
    }
  }

  public function testReturnFormIgnoresActiveAccounts() {
    $active = $this->site->createUser(array('player'));
    $this->requestReturn($active->mail);
    $this->assertNoText('Found account');
  }

  public function testOtherWebformsDontTriggerLookup() {
    $account = $this->frozenPlayer();
    $other = $this->site->createWebform(array(1), $this->emailComponent());
    $this->anonymous();
    $this->submitForm('node/' . $other->nid, array('submitted[account_email]' => $account->mail), 'Submit');
    $this->assertTrue($this->isBlocked($account));
  }

  // ---------------------------------------------------------------------------
  // Staff list.
  // ---------------------------------------------------------------------------

  public function testLeadsSeeColdStoragePlayers() {
    $frozen = $this->frozenPlayer();
    $active = $this->site->createUser(array('player'));
    $this->login($this->site->createUser(array('lead')));
    $this->get('players/cold_storage');
    $this->assertStatus(200);
    $this->assertText($frozen->name);
    $this->assertNoText($active->name);
  }

  public function testColdStorageListFiltersByMail() {
    $match = $this->frozenPlayer(array('mail' => $this->site->uniqueName('walrus') . '@example.test'));
    $other = $this->frozenPlayer();
    $this->login($this->site->createUser(array('administrator')));
    $this->get('players/cold_storage', array('mail' => 'walrus'));
    $this->assertText($match->name);
    $this->assertNoText($other->name);
  }

  public function testColdStorageListHiddenFromPlayers() {
    $this->login($this->site->createUser(array('player')));
    $this->get('players/cold_storage');
    $this->assertStatus(403);
  }

}
