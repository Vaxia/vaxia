<?php

namespace Vaxia\Tests\Vaxia\VaxiaRpg;

use Vaxia\Tests\ProposalTrait;
use Vaxia\Tests\StrikeFormTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia RPG: the site's notification rules for the rpg modules: cold storage
 * (deactivation / reactivation), proposals (receipts, closing), notes and
 * strikes, vacations, and logins from a banned player's IP. Plus leads' pages
 * of player locations.
 *
 * @group vaxia_rpg
 */
class VaxiaRpgTest extends VaxiaTestCase {

  use ProposalTrait;
  use StrikeFormTrait;

  private function mailsTo($account, $subject) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($account, $subject) {
      return $m['to'] === $account->mail && $m['subject'] === $subject;
    }));
  }

  private function setBlocked($account, $blocked) {
    $this->inSession($this->superuser(), function () use ($account, $blocked) {
      $this->submitForm('user/' . $account->uid . '/edit', array('status' => $blocked ? '0' : '1'), 'Save');
    });
  }

  // ---------------------------------------------------------------------------
  // Cold storage.
  // ---------------------------------------------------------------------------

  /**
   * Blocking a player moves them to cold storage (cold_storage module), and
   * this module tells them.
   */
  public function testBlockedPlayersAreToldTheyAreInColdStorage() {
    $player = $this->site->createUser(array('player'));
    $this->setBlocked($player, TRUE);
    $this->assertContains('cold storage', $this->rolesOf($player));
    $this->assertNotEmpty($this->mailsTo($player, '[Vaxia] Account ' . $player->name . ' deactivated due to inactivity'));
  }

  public function testBannedPlayersAreNotToldAboutColdStorage() {
    $player = $this->site->createUser(array('player', 'banned user'));
    $this->setBlocked($player, TRUE);
    $this->assertSame(array(), $this->mailsTo($player, '[Vaxia] Account ' . $player->name . ' deactivated due to inactivity'));
  }

  /**
   * @group knownbug
   * The "deactivated due to inactivity" mail is sent twice when a player is
   * blocked (the cold storage rule saves the account again, firing "user was
   * blocked" once more).
   */
  public function testColdStorageMailIsSentOnce() {
    $player = $this->site->createUser(array('player'));
    $this->setBlocked($player, TRUE);
    $this->assertCount(1, $this->mailsTo($player, '[Vaxia] Account ' . $player->name . ' deactivated due to inactivity'));
  }

  public function testReactivatedPlayersAndLeadsAreTold() {
    $lead = $this->site->createUser(array('lead'));
    $player = $this->site->createUser(array('player'), array(), array('status' => 0));
    $this->setBlocked($player, FALSE);
    $mails = $this->mailsTo($player, '[Vaxia] Account ' . $player->name . ' Re-activated');
    $this->assertCount(1, $mails);
    $this->assertCount(1, $this->mailsTo($lead, '[Vaxia] Account Reactivated From Cold Storage'));
    if (getenv('VAXIA_TARGET') === 'd11') {
      // Added for the relaunch: D7 passwords that no longer work.
      // Mail bodies are wrapped: compare with whitespace collapsed.
      $body = preg_replace('/\s+/', ' ', reset($mails)['body']);
      $this->assertStringContainsString('use "Forgot password"', $body);
      $this->assertMatchesRegularExpression('#\(https?://[^)]+/user/password\)#', $body);
    }
  }

  // ---------------------------------------------------------------------------
  // Proposals.
  // ---------------------------------------------------------------------------

  public function testVotersGetAReceipt() {
    $voter = $this->site->createUser(array('player'));
    $proposal = $this->openProposal(array($voter));
    $this->vote($voter, $proposal);
    $this->assertCount(1, $this->mailsTo($voter, '[Vaxia] Proposal receipt for ' . $proposal->title));
  }

  public function testAuthorIsToldWhenTheProposalCloses() {
    $proposal = $this->openProposal(array($this->site->createUser(array('player'))));
    $this->closeByCron($proposal);
    $mails = array_filter($this->sentMail(), function ($m) use ($proposal) {
      return $m['to'] === $proposal->author->mail && strpos($m['subject'], '[Vaxia] Proposal ' . $proposal->title . ' closed on ') === 0;
    });
    $this->assertCount(1, $mails);
  }

  // ---------------------------------------------------------------------------
  // Notes and strikes.
  // ---------------------------------------------------------------------------

  public function testNotesAreMailedToLeadsAndMediators() {
    $lead = $this->site->createUser(array('lead'));
    $mediator = $this->site->createUser(array('mediator'));
    $player = $this->site->createUser(array('player'));
    $this->site->createNotation($player);
    $this->assertCount(1, $this->mailsTo($lead, '[Vaxia] A note has added for ' . $player->name));
    $this->assertCount(1, $this->mailsTo($mediator, '[Vaxia] A note has added for ' . $player->name));
  }

  public function testRedStrikeIsMailedToPlayerAndStaff() {
    $storyteller = $this->site->createUser(array('storyteller'));
    $player = $this->site->createUser(array('player'));
    $this->site->createCharacter($player);
    $this->fileStrike($player, 'Red');
    $this->assertCount(1, $this->mailsTo($player, '[Vaxia] A red strike has been issued on your account'));
    $this->assertCount(1, $this->mailsTo($storyteller, '[Vaxia] A red strike has been issued to ' . $player->name));
    $this->assertCount(1, $this->mailsTo($player, '[Vaxia] Player Characters for ' . $player->name), 'Sent a copy of their characters');
  }

  public function testYellowStrikeIsMailedToPlayerAndLeads() {
    $lead = $this->site->createUser(array('lead'));
    $player = $this->site->createUser(array('player'));
    $this->fileStrike($player, 'Yellow');
    $this->assertCount(1, $this->mailsTo($player, '[Vaxia] A yellow strike has been issued to your account'));
    $this->assertCount(1, $this->mailsTo($lead, '[Vaxia] A yellow strike against ' . $player->name));
  }

  // ---------------------------------------------------------------------------
  // Vacations.
  // ---------------------------------------------------------------------------

  public function testStorytellersHearAboutVacations() {
    $colleague = $this->site->createUser(array('storyteller'));
    $staff = $this->site->createUser(array('storyteller'), array('can go on vacation'));
    $this->login($staff);
    $this->submitForm('user/' . $staff->uid . '/edit', array('vacation_end' => '1 week'), 'Go on vacation');
    $this->assertCount(1, $this->mailsTo($colleague, '[Vaxia] Storyhost or ASH has gone on vacation'));
  }

  // ---------------------------------------------------------------------------
  // Banned IPs.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * The rule hands the view loop the IP as PHP code ("<?php echo ip_address();
   * ?>"), which isn't evaluated there: the lookup searches for that literal
   * text and the alert never goes out. (The view itself finds the banned
   * player when given 127.0.0.1.)
   */
  public function testLoginFromABannedPlayersIpAlertsLeads() {
    $lead = $this->site->createUser(array('lead'));
    // The test client connects from 127.0.0.1.
    $banned = $this->site->createUser(array('banned user'), array(), array('status' => 0, 'field_banned_ip' => '127.0.0.1'));
    try {
      $this->login($this->site->createUser(array('player')));
      $this->assertCount(1, $this->mailsTo($lead, '[Vaxia] User detected sharing IP with banned user(s)'));
    }
    finally {
      // The class shares one database: later logins in it would match this IP.
      $this->site->setUserFields($banned, array('field_banned_ip' => NULL));
    }
  }

  public function testOrdinaryLoginsAlertNobody() {
    $lead = $this->site->createUser(array('lead'));
    $this->site->createUser(array('banned user'), array(), array('status' => 0, 'field_banned_ip' => '10.9.9.9'));
    $this->login($this->site->createUser(array('player')));
    $this->assertSame(array(), $this->mailsTo($lead, '[Vaxia] User detected sharing IP with banned user(s)'));
  }

  // ---------------------------------------------------------------------------
  // Pages.
  // ---------------------------------------------------------------------------

  public function testLocationsPagesAreForLeads() {
    $this->login($this->site->createUser(array('player')));
    $this->get('players/locations');
    $this->assertStatus(403);
    $this->login($this->site->createUser(array('lead')));
    foreach (array('players/locations', 'players/locations/map') as $path) {
      $this->get($path);
      $this->assertStatus(200, $path);
    }
  }

}
