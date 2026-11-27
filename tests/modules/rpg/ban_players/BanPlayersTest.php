<?php

namespace Vaxia\Tests\Rpg\BanPlayers;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Ban players: banned users who log in get their IP recorded and blocked; players'
 * last IP and location are tracked; staff list banned and suspended players.
 *
 * @group ban_players
 */
class BanPlayersTest extends VaxiaTestCase {

  protected function tearDown(): void {
    // Banning blocks the test client's own IP (127.0.0.1); lift it for later tests.
    $this->site->unblockAllIps();
    parent::tearDown();
  }

  /**
   * Submits the login form in a fresh browser (no "logged in" assertion: a
   * banned login ends on the IP-blocked page).
   */
  private function attemptLogin($account) {
    $this->anonymous();
    $this->submitForm('user/login', array('name' => $account->name, 'pass' => 'pass'), 'Log in');
  }

  // ---------------------------------------------------------------------------
  // Banned user logs in.
  // ---------------------------------------------------------------------------

  public function testBannedLoginBlocksTheirIp() {
    $this->attemptLogin($this->site->createUser(array('banned user')));
    $this->anonymous();
    $this->get('user/login');
    $this->assertStatus(403);
    $this->assertStringContainsString('127.0.0.1 has been banned', $this->content);
  }

  public function testBannedLoginRecordsIpOnProfile() {
    $banned = $this->site->createUser(array('banned user'));
    $this->attemptLogin($banned);
    $this->site->unblockAllIps();
    $this->assertStringContainsString('Banned IP: 127.0.0.1', $this->profileText($banned));
  }

  public function testBannedIpIsRecordedOnce() {
    $banned = $this->site->createUser(array('banned user'));
    $this->attemptLogin($banned);
    $this->site->unblockAllIps();
    $this->attemptLogin($banned);
    $this->site->unblockAllIps();
    $profile = $this->profileText($banned);
    $this->assertSame(1, substr_count($profile, '127.0.0.1', strpos($profile, 'Banned IP')));
  }

  public function testOrdinaryLoginBlocksNothing() {
    $this->login($this->site->createUser(array('player')));
    $this->anonymous();
    $this->get('user/login');
    $this->assertStatus(200);
  }

  /**
   * @group knownbug
   * The rule's redirect to user/logout loses to the login form's own redirect:
   * the banned user keeps a live session, stopped only by the IP block.
   */
  public function testBannedUserIsLoggedOut() {
    $this->attemptLogin($this->site->createUser(array('banned user')));
    $this->site->unblockAllIps();
    $this->get('user');
    $this->assertSame(0, $this->xpath()->query('//a[contains(@href, "user/logout")]')->length, 'Session ended');
  }

  // ---------------------------------------------------------------------------
  // Last IP tracking.
  // ---------------------------------------------------------------------------

  public function testPlayerLoginRecordsLastIpAndLocation() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $profile = $this->profileText($player);
    $this->assertStringContainsString('Last IP: 127.0.0.1', $profile);
    // smart_ip has no data for localhost, so the rule stores 0.
    $this->assertStringContainsString('Latitude: 0', $profile);
    $this->assertStringContainsString('Longitude: 0', $profile);
  }

  public function testNewAccountsAreTrackedToo() {
    $account = $this->site->createUser();
    $this->assertSame(array('new account'), $this->rolesOf($account), 'New accounts get "new account"');
    $this->login($account);
    $this->assertStringContainsString('Last IP: 127.0.0.1', $this->profileText($account));
  }

  public function testStaffWithoutPlayerRoleAreNotTracked() {
    $staff = $this->site->createUser(array('lead'));
    $this->site->removeRole($staff, 'new account');
    $this->login($staff);
    $this->assertStringNotContainsString('Last IP', $this->profileText($staff));
  }

  // ---------------------------------------------------------------------------
  // Banned/suspended list.
  // ---------------------------------------------------------------------------

  public function testLeadsSeeBannedAndSuspendedPlayers() {
    $banned = $this->site->createUser(array('banned user'), array(), array(
      'field_reason_for_ban' => 'Harassment in chat',
      'field_banned_ip' => '10.9.9.9',
    ));
    $suspended = $this->site->createUser(array('suspended user'));
    $player = $this->site->createUser(array('player'));

    $this->login($this->site->createUser(array('lead')));
    $this->get('players/banned');

    $this->assertStatus(200);
    $this->assertText($banned->name);
    $this->assertText('Harassment in chat');
    $this->assertText('10.9.9.9');
    $this->assertText('PERMANENT');
    $this->assertText($suspended->name);
    $this->assertNoText($player->name);
  }

  public function testBannedListFiltersByName() {
    $match = $this->site->createUser(array('banned user'), array(), array('name' => $this->site->uniqueName('otter')));
    $other = $this->site->createUser(array('banned user'), array(), array('name' => $this->site->uniqueName('badger')));
    $this->login($this->site->createUser(array('administrator')));
    $this->get('players/banned', array('name' => 'otter'));
    $this->assertText($match->name);
    $this->assertNoText($other->name);
  }

  public function testBannedListHiddenFromOthers() {
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('players/banned');
    $this->assertStatus(403);
  }

}
