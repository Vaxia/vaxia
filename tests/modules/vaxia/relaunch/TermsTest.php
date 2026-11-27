<?php

namespace Vaxia\Tests\Vaxia\Relaunch;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Relaunch: accepting the current terms (D11 only). D7 only had the
 * "Terms and Conditions" box on the sign-up form. Staff set a terms version
 * (admin/vaxia/relaunch); a logged-in user who hasn't accepted that version
 * is sent to terms/accept from every page until they do, the site admin
 * included. Logging out, the terms page and the one-time login / new
 * password flow stay open. Signing up with the box ticked accepts the
 * current version. No version set: nobody is asked.
 *
 * @group relaunch
 */
class TermsTest extends VaxiaTestCase {

  private $termsPage;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The relaunch features exist on the Drupal 11 port only.');
    }
    $this->termsPage = $this->site->createNode('page', array('title' => 'Our terms', 'body' => 'Be excellent to each other.'));
    $this->site->setConfig('vaxia_terms_path', '/node/' . $this->termsPage->nid);
    $this->site->setConfig('vaxia_terms_version', '2026-12-01');
  }

  protected function tearDown(): void {
    if (getenv('VAXIA_TARGET') === 'd11') {
      $this->site->deleteConfig('vaxia_terms_version');
      $this->site->deleteConfig('vaxia_terms_path');
    }
    parent::tearDown();
  }

  private function onAcceptPage() {
    return strpos($this->url, '/terms/accept') !== FALSE && $this->xpath()->query('//input[@name="agree"]')->length === 1;
  }

  private function accept() {
    $this->submitForm('terms/accept', array('agree' => TRUE), 'Accept');
  }

  public function testReturningPlayerIsSentToTheTermsAfterLogin() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->assertTrue($this->onAcceptPage(), 'Login lands on the terms');
    $this->assertStringContainsString('href="/node/' . $this->termsPage->nid . '"', $this->content);

    foreach (array('node', 'user/' . $player->uid, 'character-sheets/mine') as $path) {
      $this->get($path);
      $this->assertTrue($this->onAcceptPage(), $path . ' sends to the terms');
    }
  }

  public function testAcceptingReturnsToThePageAndIsRemembered() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('character-sheets/mine');
    $this->assertTrue($this->onAcceptPage());
    $this->submitForm(NULL, array('agree' => TRUE), 'Accept');
    $this->assertContains('Thank you for accepting the Terms and Conditions.', $this->messages('status'));
    $this->assertStringContainsString('/character-sheets/mine', $this->url);

    $this->get('node');
    $this->assertFalse($this->onAcceptPage());
    $this->get('terms/accept');
    $this->assertText('You have accepted the current Terms and Conditions.');
  }

  public function testTheBoxMustBeTicked() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->submitForm('terms/accept', array(), 'Accept');
    $this->assertNotEmpty($this->messages('error'));
    $this->get('node');
    $this->assertTrue($this->onAcceptPage());
  }

  public function testNewTermsVersionMustBeAcceptedAgain() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->accept();
    $this->get('node');
    $this->assertFalse($this->onAcceptPage());

    $this->site->setConfig('vaxia_terms_version', '2027-01-15');
    $this->get('node');
    $this->assertTrue($this->onAcceptPage());
    $this->accept();
    $this->get('node');
    $this->assertFalse($this->onAcceptPage());
  }

  public function testStaffBumpTheVersionFromTheSettings() {
    $this->login($this->superuser());
    // The site admin is asked too.
    $this->get('admin/vaxia/relaunch');
    $this->assertTrue($this->onAcceptPage());
    $this->accept();
    $this->get('admin/vaxia/relaunch');
    $this->assertSame('2026-12-01', $this->xpath()->query('//input[@name="terms_version"]/@value')->item(0)->nodeValue);

    $this->submitForm(NULL, array('terms_version' => '3'), 'Save configuration');
    $this->assertSame('3', $this->site->config('vaxia_terms_version'));
    $this->get('node');
    $this->assertTrue($this->onAcceptPage());
    $this->accept();

    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->assertTrue($this->onAcceptPage());
  }

  public function testNoVersionAsksNobody() {
    $this->site->setConfig('vaxia_terms_version', '');
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->assertFalse($this->onAcceptPage());
    $this->get('character-sheets/mine');
    $this->assertFalse($this->onAcceptPage());
  }

  public function testTermsPageAndLogoutStayOpen() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('node/' . $this->termsPage->nid);
    $this->assertFalse($this->onAcceptPage());
    $this->assertText('Be excellent to each other.');

    $this->logout();
    $this->get('node');
    $this->assertStringNotContainsString('user/logout', $this->content);
    $this->assertFalse($this->onAcceptPage());
  }

  public function testAnonymousVisitorsAreNotAsked() {
    $this->anonymous();
    $this->get('node');
    $this->assertFalse($this->onAcceptPage());
    $this->get('terms/accept');
    $this->assertStatus(403);
  }

  public function testOneTimeLoginAndNewPasswordStayOpen() {
    $player = $this->site->createUser(array('player'));
    $this->anonymous();
    $this->clearMail();
    $this->submitForm('user/password', array('name' => $player->mail), 'Submit');
    $mails = array_values(array_filter($this->sentMail(), function ($m) use ($player) {
      return $m['to'] === $player->mail;
    }));
    $this->assertCount(1, $mails);
    $this->assertSame(1, preg_match('#/(user/reset/\d+/\d+/[\w-]+)#', $mails[0]['body'], $m));

    $this->get($m[1]);
    $this->submitForm(NULL, array(), 'Log in');
    $this->assertFalse($this->onAcceptPage(), 'The one-time login reaches the account form');
    $this->assertStringContainsString('/user/' . $player->uid . '/edit', $this->url);
    $this->submitForm(NULL, array('pass[pass1]' => 'newpass', 'pass[pass2]' => 'newpass'), 'Save');
    $this->assertContains('The changes have been saved.', $this->messages('status'));

    $this->get('node');
    $this->assertTrue($this->onAcceptPage());
  }

  public function testSigningUpAcceptsTheCurrentTerms() {
    $name = $this->site->uniqueName('newbie');
    $this->anonymous();
    $this->clearMail();
    $this->submitForm('user/register', array('name' => $name, 'mail' => $name . '@example.com', 'field_terms_and_conditions[und]' => TRUE, 'field_age_requirement[und]' => TRUE), 'Create new account');
    $account = $this->site->findUser($name);
    $this->assertNotNull($account);
    $mails = array_values(array_filter($this->sentMail(), function ($m) use ($account) {
      return $m['to'] === $account->mail;
    }));
    $this->assertNotEmpty($mails);
    $this->assertSame(1, preg_match('#/(user/reset/\d+/\d+/[\w-]+)#', $mails[0]['body'], $m));
    $this->get($m[1]);
    $this->submitForm(NULL, array(), 'Log in');
    $this->get('node');
    $this->assertStringContainsString('user/logout', $this->content);
    $this->assertFalse($this->onAcceptPage());
  }

}
