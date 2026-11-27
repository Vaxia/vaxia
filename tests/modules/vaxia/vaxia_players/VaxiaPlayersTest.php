<?php

namespace Vaxia\Tests\Vaxia\VaxiaPlayers;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia players: accounts and profiles.
 *
 * - New accounts get "new account"; registering without meeting the age
 *   requirement blocks the account and says so by mail.
 * - Profiles: a player can mail themselves a copy of all their characters
 *   (staff can do it for anyone). People without "view others accounts" only
 *   see the public profile fields; owners can't edit the staff-managed fields.
 * - Spendable XP a character gains adds to the player's "XP over time".
 * - A power search over chat posts (not in private or mature rooms).
 *
 * @group vaxia_players
 */
class VaxiaPlayersTest extends VaxiaTestCase {

  private function register($name, $adult) {
    $this->anonymous();
    $this->get('user/register');
    $edit = array('name' => $name, 'mail' => $name . '@example.com', 'field_terms_and_conditions[und]' => TRUE);
    if ($adult) {
      $edit['field_age_requirement[und]'] = TRUE;
    }
    $this->submitForm(NULL, $edit, 'Create new account');
    return $this->site->findUser($name);
  }

  private function mailsTo($account, $subject) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($account, $subject) {
      return $m['to'] === $account->mail && $m['subject'] === $subject;
    }));
  }

  // ---------------------------------------------------------------------------
  // Registration.
  // ---------------------------------------------------------------------------

  public function testNewAccountsGetTheNewAccountRole() {
    $account = $this->register($this->site->uniqueName('newbie'), TRUE);
    $this->assertNotNull($account, 'Registered');
    $this->assertContains('new account', $this->rolesOf($account));
    $this->assertFalse($this->isBlocked($account));
  }

  /**
   * @group knownbug
   * "Set new account role on new accounts" checks NOT (has "new account" AND
   * has "player"); it means OR. Accounts created as players (by staff) are
   * also given "new account".
   */
  public function testAccountsCreatedAsPlayersAreNotNewAccounts() {
    $player = $this->site->createUser(array('player'));
    $this->assertNotContains('new account', $this->rolesOf($player));
  }

  /**
   * The age box is required, so registering without ticking it is refused
   * (the "User is under minimal age" rule, which blocks such accounts, never
   * gets a chance to run).
   */
  public function testUnderageRegistrationsAreRefused() {
    $this->assertNull($this->register($this->site->uniqueName('kid'), FALSE));
    $this->assertStringContainsString('field is required', implode(' ', $this->messages('error')));
  }

  public function testTermsMustBeAccepted() {
    $this->anonymous();
    $this->get('user/register');
    $name = $this->site->uniqueName('nope');
    $this->submitForm(NULL, array('name' => $name, 'mail' => $name . '@example.com', 'field_age_requirement[und]' => TRUE), 'Create new account');
    $this->assertNull($this->site->findUser($name));
  }

  public function testAccountsMadeUnderageAreBlocked() {
    $account = $this->site->createUser(array(), array(), array('field_age_requirement' => 0));
    $this->assertTrue($this->isBlocked($account));
    $this->assertNotEmpty(preg_grep('/\(NOT approved\)$/', array_column(array_filter($this->sentMail(), function ($m) use ($account) {
      return $m['to'] === $account->mail;
    }), 'subject')));
  }

  /**
   * @group knownbug
   * The underage mail's subject uses [user:name], which that rule (on account
   * creation) doesn't have: "Account details for  at Vaxia (NOT approved)".
   */
  public function testUnderageMailNamesTheAccount() {
    $account = $this->site->createUser(array(), array(), array('field_age_requirement' => 0));
    $this->assertCount(1, $this->mailsTo($account, 'Account details for ' . $account->name . ' at VAXIA-test (NOT approved)'));
  }

  // ---------------------------------------------------------------------------
  // Mailing characters.
  // ---------------------------------------------------------------------------

  public function testPlayersCanMailThemselvesTheirCharacters() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Hero')));
    $this->login($player);
    $this->submitForm('user/' . $player->uid, array(), 'Send Me My Characters');

    $this->assertContains('Characters have been sent to the email associated with this player account.', $this->messages());
    $mails = $this->mailsTo($player, '[Vaxia] Player Characters for ' . $player->name);
    $this->assertCount(1, $mails);
    $this->assertStringContainsString(strtoupper($character->title), $mails[0]['body'], 'Character included (titles are upper-cased in text mail)');
  }

  public function testStaffCanMailPlayersTheirCharacters() {
    $player = $this->site->createUser(array('player'));
    $this->site->createCharacter($player);
    $this->login($this->site->createUser(array('lead'), array('email others characters')));
    $this->submitForm('user/' . $player->uid, array(), 'Send ' . $player->name . ' Their Characters');
    $this->assertCount(1, $this->mailsTo($player, '[Vaxia] Player Characters for ' . $player->name));
  }

  public function testOthersCannotMailSomeonesCharacters() {
    $player = $this->site->createUser(array('player'));
    $this->site->createCharacter($player);
    $this->login($this->site->createUser(array('player')));
    $this->get('user/' . $player->uid);
    $this->assertSame(0, $this->xpath()->query('//input[contains(@value, "Their Characters")]')->length);
  }

  public function testNoButtonWithoutCharacters() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('user/' . $player->uid);
    $this->assertSame(0, $this->xpath()->query('//input[@value="Send Me My Characters"]')->length);
  }

  // ---------------------------------------------------------------------------
  // Profile fields.
  // ---------------------------------------------------------------------------

  public function testPrivateFieldsAreHiddenFromOthers() {
    $player = $this->site->createUser(array('player'));
    $this->site->setUserFields($player, array('field_took_ash1' => '2025-01-01 00:00:00'));
    $this->withoutPermission('view others accounts', array('authenticated user', 'player', 'new account'), function () use ($player) {
      $this->login($this->site->createUser(array('new account')));
      $this->get('user/' . $player->uid);
      $this->assertNoText('Took ASH 1');
      $this->login($player);
      $this->get('user/' . $player->uid);
      $this->assertText('Took ASH 1');
    });
  }

  public function testOwnersCannotEditStaffFields() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('user/' . $player->uid . '/edit');
    $this->assertStatus(200);
    foreach (array('field_xp_over_time', 'field_powers_expire', 'field_took_ash1') as $field) {
      $this->assertSame(0, $this->xpath()->query('//*[starts-with(@name, "' . $field . '[")]')->length, $field);
    }
  }

  // ---------------------------------------------------------------------------
  // XP over time.
  // ---------------------------------------------------------------------------

  /**
   * Observed through the storyhosts rule that makes players with over 39 XP
   * over time eligible for SH when their account is saved.
   */
  public function testSpendableXpAddsToXpOverTime() {
    $player = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 1));
    $character = $this->site->createCharacter($player);
    $this->approveCharacter($character);
    $this->site->addCharacterXp($character, 'rxp', 40);
    $this->inSession($this->superuser(), function () use ($character) {
      $this->submitForm('node/' . $character->nid . '/character_sheet', array('xp' => '40', 'type' => 'xp'), 'Add XP');
    });
    $this->login($player);
    $this->submitForm('user/' . $player->uid . '/edit', array(), 'Save');
    $this->assertContains('eligible for SH', $this->rolesOf($player));
  }

  /**
   * @group knownbug
   * "Add XP to total User XP" adds to the player's current total with
   * data_calc, which refuses an empty input ("The variable or parameter
   * input_1 is empty"): a player with no total yet (every new player) never
   * starts counting, until an admin runs "Recalculate Earned XP".
   */
  public function testFirstXpStartsTheTotal() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player);
    $this->approveCharacter($character);
    $this->site->addCharacterXp($character, 'rxp', 40);
    $this->inSession($this->superuser(), function () use ($character) {
      $this->submitForm('node/' . $character->nid . '/character_sheet', array('xp' => '40', 'type' => 'xp'), 'Add XP');
    });
    $this->login($player);
    $this->submitForm('user/' . $player->uid . '/edit', array(), 'Save');
    $this->assertContains('eligible for SH', $this->rolesOf($player));
  }

  public function testBankedXpDoesNotCount() {
    $player = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 1));
    $character = $this->site->createCharacter($player);
    $this->approveCharacter($character);
    // No RXP: added XP is banked, not spendable.
    $this->inSession($this->superuser(), function () use ($character) {
      $this->submitForm('node/' . $character->nid . '/character_sheet', array('xp' => '40', 'type' => 'xp'), 'Add XP');
    });
    $this->login($player);
    $this->submitForm('user/' . $player->uid . '/edit', array(), 'Save');
    $this->assertNotContains('eligible for SH', $this->rolesOf($player));
  }

  // ---------------------------------------------------------------------------
  // Power search.
  // ---------------------------------------------------------------------------

  public function testPowerSearchFindsPostsByText() {
    // Public room (field_private_room 1 = "show in Power Search"), public character.
    $room = $this->site->createRoom(array('title' => $this->site->uniqueName('Tavern'), 'field_private_room' => 1, 'field_mature_posting' => 0, 'field_locked_to_users' => 0));
    $author = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($author, array('title' => $this->site->uniqueName('Hero'), 'field_visibility' => 1));
    $word = $this->site->uniqueName('quokka');
    $this->site->postComment($room, $author, 'I saw a ' . $word . ' here', $character);
    $this->login($this->site->createUser(array('player')));
    $this->get('search/search', array('comment_body_value' => $word));
    $this->assertStatus(200);
    // Results show the room, character, author and the post's subject.
    $this->assertText($room->title ?? '');
    $this->assertText($character->title ?? '');
  }

  public function testPowerSearchSkipsHiddenRooms() {
    $room = $this->site->createRoom(array('field_private_room' => 0, 'field_mature_posting' => 0, 'field_locked_to_users' => 0));
    $author = $this->site->createUser(array('player'));
    $word = $this->site->uniqueName('wombat');
    $this->site->postComment($room, $author, 'A ' . $word, $this->site->createCharacter($author, array('field_visibility' => 1)));
    $this->login($this->site->createUser(array('player')));
    $this->get('search/search', array('comment_body_value' => $word));
    $this->assertNoText($author->name);
  }

  /**
   * @group knownbug
   * "Retire inactive user account" listens for user_stats_day_older (broken
   * under Rules 2, see suspend_players): inactive players are never moved to
   * cold storage.
   */
  public function testInactivePlayersAreRetired() {
    $player = $this->site->createUser(array('player'));
    $this->site->backdateAccountCreated($player, strtotime('-1 year'));
    $this->site->setLastCron(strtotime('-1 day'));
    $this->runCron();
    $this->assertContains('cold storage', $this->rolesOf($player));
  }

}
