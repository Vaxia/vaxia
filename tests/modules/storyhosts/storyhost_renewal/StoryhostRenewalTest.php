<?php

namespace Vaxia\Tests\Storyhosts\StoryhostRenewal;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Storyhost renewal: storyhost powers last a year from passing ASH tests 1 and
 * 2. A storyhost (or expired storyhost) who has passed both within the last
 * month and then passes a renewal quiz (production nodes 7037 / 7781) is
 * renewed: storyteller role back, character titled "SH", powers extended, mails
 * sent. Once a year has passed, cron is meant to expire them.
 *
 * @group storyhost_renewal
 */
class StoryhostRenewalTest extends VaxiaTestCase {

  const RENEWAL_QUIZZES = array(7037, 7781);

  /** @var bool Quizzes exist (the database is shared by the class). */
  private static $quizzes = FALSE;

  protected function setUp(): void {
    parent::setUp();
    if (!self::$quizzes) {
      foreach (self::RENEWAL_QUIZZES as $nid) {
        $this->site->createNode('quiz', array('nid' => $nid, 'is_new' => TRUE, 'title' => 'ASH renewal ' . $nid));
      }
      self::$quizzes = TRUE;
    }
  }

  public static function tearDownAfterClass(): void {
    self::$quizzes = FALSE;
    parent::tearDownAfterClass();
  }

  /**
   * A storyhost with a storyhost character, who took ASH 1 and 2 on $took.
   */
  private function storyhost($role, $took) {
    $account = $this->site->createUser(array('player', $role));
    $character = $this->site->createCharacter($account, array('title' => $this->site->uniqueName('Sh'), 'field_titles' => 'SH (retired)'));
    $this->site->setUserFields($account, array(
      'field_storyhost_character' => $character->nid,
      'field_took_ash1' => date('Y-m-d 00:00:00', strtotime($took)),
      'field_took_ash2' => date('Y-m-d 00:00:00', strtotime($took)),
    ));
    return array($account, $character);
  }

  private function mailsTo($account, $subject) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($account, $subject) {
      return $m['to'] === $account->mail && $m['subject'] === $subject;
    }));
  }

  public function renewalQuizzes() {
    return array(array(7037), array(7781));
  }

  /**
   * @dataProvider renewalQuizzes
   */
  public function testExpiredStoryhostIsRenewed($quiz) {
    $watcher = $this->site->createUser(array('storyteller'));
    list($account, $character) = $this->storyhost('storyteller-expired', '-1 week');

    $this->site->passQuiz($account, (object) array('nid' => $quiz));

    $roles = $this->rolesOf($account);
    $this->assertContains('storyteller', $roles);
    $this->assertNotContains('storyteller-expired', $roles);
    $this->assertStringNotContainsString('SH (retired)', $this->nodeText($character));
    $this->assertCount(1, $this->mailsTo($account, '[Vaxia] You have renewed your SH permissions'));
    $this->assertCount(1, $this->mailsTo($watcher, '[Vaxia] A SH has renewed permissions'));
  }

  public function testOtherQuizzesDoNotRenew() {
    list($account) = $this->storyhost('storyteller-expired', '-1 week');
    $this->site->passQuiz($account, $this->site->createNode('quiz'));
    $this->assertContains('storyteller-expired', $this->rolesOf($account));
  }

  public function testAshTestsOlderThanAMonthDoNotRenew() {
    list($account) = $this->storyhost('storyteller-expired', '-2 months');
    $this->site->passQuiz($account, (object) array('nid' => 7037));
    $this->assertContains('storyteller-expired', $this->rolesOf($account));
    $this->assertSame(array(), $this->mailsTo($account, '[Vaxia] You have renewed your SH permissions'));
  }

  public function testPlayersAreNotRenewed() {
    list($account) = $this->storyhost('player', '-1 week');
    $this->site->passQuiz($account, (object) array('nid' => 7037));
    $this->assertNotContains('storyteller', $this->rolesOf($account));
  }

  /**
   * @group knownbug
   * "SH Renewal - Expire Storyhost" listens for user_stats_day_older, which
   * can't deliver a user under Rules 2 (see suspend_players): storyhost powers
   * never expire.
   */
  public function testCronExpiresStoryhostsAfterAYear() {
    list($account, $character) = $this->storyhost('storyteller', '-13 months');
    $this->site->backdateAccountCreated($account, strtotime('-3 days'));
    $this->site->setLastCron(strtotime('-1 day'));

    $this->runCron();

    $this->assertContains('storyteller-expired', $this->rolesOf($account));
    $this->assertStringContainsString('SH (retired)', $this->nodeText($character));
    $this->assertCount(1, $this->mailsTo($account, '[Vaxia] Your SH privileges have expired'));
  }

}
