<?php

namespace Vaxia\Tests\Vaxia\Relaunch;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Relaunch: staff account recovery (D11 only). Staff with "recover user
 * accounts" move a player's account to a new email address from the
 * player's "Recover account" tab, saying how they confirmed who the player
 * is. They can send a one-time login link to the new address. Each change is
 * listed on the tab (who, when, old and new address, the note).
 *
 * @group relaunch
 */
class AccountRecoveryTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The relaunch features exist on the Drupal 11 port only.');
    }
    $this->clearMail();
  }

  private function staff() {
    return $this->site->createUser(array('lead'), array('recover user accounts'));
  }

  private function mailsTo($address) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($address) {
      return $m['to'] === $address;
    }));
  }

  /**
   * The account's address as staff see it on its edit form.
   */
  private function mailOf($account) {
    return $this->inSession($this->superuser(), function () use ($account) {
      $this->get('user/' . $account->uid . '/edit');
      $node = $this->xpath()->query('//input[@name="mail"]/@value')->item(0);
      return $node ? $node->nodeValue : NULL;
    });
  }

  public function testOnlyStaffWithThePermissionGetTheTab() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('user/' . $player->uid . '/recover');
    $this->assertStatus(403);

    $this->login($this->site->createUser(array('lead')));
    $this->get('user/' . $player->uid . '/recover');
    $this->assertStatus(403);

    $this->login($this->staff());
    $this->get('user/' . $player->uid);
    $this->assertNotEmpty($this->xpath()->query('//a[contains(@href, "/user/' . $player->uid . '/recover")]'));
    $this->get('user/' . $player->uid . '/recover');
    $this->assertStatus(200);
    $this->assertText($player->mail);
    $this->assertText('No changes yet.');
  }

  public function testChangingTheAddressSendsALoginLinkThereAndIsLogged() {
    $player = $this->site->createUser(array('player'));
    $staff = $this->staff();
    $new = $this->site->uniqueName('new') . '@example.com';
    $this->login($staff);
    $this->submitForm('user/' . $player->uid . '/recover', array('mail' => $new, 'note' => 'Matched their 2016 character names over Discord.'), 'Change email address');
    $this->assertContains('The email address was changed and a one-time login link was sent to the new address.', $this->messages('status'), implode(' / ', $this->messages()));
    $this->assertSame($new, $this->mailOf($player));

    $this->assertSame(array(), $this->mailsTo($player->mail));
    $mails = $this->mailsTo($new);
    $this->assertCount(1, $mails);
    $this->assertSame(1, preg_match('#/(user/reset/' . $player->uid . '/\d+/[\w-]+)#', $mails[0]['body'], $m));

    $this->get('user/' . $player->uid . '/recover');
    $rows = $this->xpath()->query('//table[contains(@class, "vaxia-recovery-log")]/tbody/tr');
    $this->assertSame(1, $rows->length);
    $row = preg_replace('/\s+/', ' ', $rows->item(0)->textContent);
    foreach (array($staff->name, $player->mail, $new, 'Matched their 2016 character names over Discord.', 'Yes') as $text) {
      $this->assertStringContainsString($text, $row);
    }

    // The link works for the player at the new address.
    $this->anonymous();
    $this->get($m[1]);
    $this->submitForm(NULL, array(), 'Log in');
    $this->assertStringContainsString('user/logout', $this->content);
  }

  public function testWithoutTheLink() {
    $player = $this->site->createUser(array('player'));
    $new = $this->site->uniqueName('new') . '@example.com';
    $this->login($this->staff());
    $this->submitForm('user/' . $player->uid . '/recover', array('mail' => $new, 'note' => 'Video call.', 'send_link' => FALSE), 'Change email address');
    $this->assertContains('The email address was changed.', $this->messages('status'));
    $this->assertSame($new, $this->mailOf($player));
    $this->assertSame(array(), $this->mailsTo($new));
    $row = preg_replace('/\s+/', ' ', $this->xpath()->query('//table[contains(@class, "vaxia-recovery-log")]/tbody/tr')->item(0)->textContent);
    $this->assertStringContainsString('No', $row);
  }

  public function testTheNoteIsRequired() {
    $player = $this->site->createUser(array('player'));
    $new = $this->site->uniqueName('new') . '@example.com';
    $this->login($this->staff());
    $this->submitForm('user/' . $player->uid . '/recover', array('mail' => $new, 'note' => ''), 'Change email address');
    $this->assertNotEmpty($this->messages('error'));
    $this->assertSame($player->mail, $this->mailOf($player));
    $this->assertSame(array(), $this->mailsTo($new));
  }

  public function testAnAddressAnotherAccountUsesIsRefused() {
    $player = $this->site->createUser(array('player'));
    $other = $this->site->createUser(array('player'));
    $this->login($this->staff());
    $this->submitForm('user/' . $player->uid . '/recover', array('mail' => $other->mail, 'note' => 'Asked in chat.'), 'Change email address');
    $this->assertContains('Another account already uses that email address.', $this->messages('error'));
    $this->assertSame($player->mail, $this->mailOf($player));

    $this->submitForm('user/' . $player->uid . '/recover', array('mail' => $player->mail, 'note' => 'Asked in chat.'), 'Change email address');
    $this->assertContains('That is already the account\'s email address.', $this->messages('error'));
  }

  public function testBlockedAccountGetsNoLink() {
    $player = $this->site->createUser(array('player'));
    $this->site->setUserFields($player, array('status' => 0));
    $new = $this->site->uniqueName('new') . '@example.com';
    $this->login($this->staff());
    $this->submitForm('user/' . $player->uid . '/recover', array('mail' => $new, 'note' => 'Phone call.'), 'Change email address');
    $this->assertSame($new, $this->mailOf($player));
    $this->assertNotEmpty(preg_grep('/account is blocked, so no login link was sent/', $this->messages('warning')));
    $this->assertSame(array(), $this->mailsTo($new));
  }

  public function testChangesAccumulateNewestFirst() {
    $player = $this->site->createUser(array('player'));
    $first = $this->site->uniqueName('first') . '@example.com';
    $second = $this->site->uniqueName('second') . '@example.com';
    $this->login($this->staff());
    $this->submitForm('user/' . $player->uid . '/recover', array('mail' => $first, 'note' => 'First note.', 'send_link' => FALSE), 'Change email address');
    $this->submitForm('user/' . $player->uid . '/recover', array('mail' => $second, 'note' => 'Second note.', 'send_link' => FALSE), 'Change email address');
    $rows = $this->xpath()->query('//table[contains(@class, "vaxia-recovery-log")]/tbody/tr');
    $this->assertSame(2, $rows->length);
    $this->assertStringContainsString('Second note.', $rows->item(0)->textContent);
    $this->assertStringContainsString('First note.', $rows->item(1)->textContent);
  }

}
