<?php

namespace Vaxia\Tests\Storyhosts\ShTest;

use Vaxia\Tests\VaxiaTestCase;

/**
 * SH test: the storyhost course. Quizzes at production nodes 7037 (ASH Test
 * One), 7050 (ASH Test Two), 9359 and 7181 (ASH Test Three) move players
 * through storyteller-newbie (ASH 1) and storyteller-assistant (ASH 2); ASH 2
 * needs three qualifying sessions. Session reports say whether they qualify
 * (enough other players' characters, recent enough), and ASHs' sessions ask the
 * players for feedback.
 *
 * @group sh_test
 */
class ShTestTest extends VaxiaTestCase {

  const ASH1 = 7037;
  const ASH2 = 7050;
  const ASH3 = 7181;

  /** @var bool Quizzes exist (the database is shared by the class). */
  private static $quizzes = FALSE;

  protected function setUp(): void {
    parent::setUp();
    if (!self::$quizzes) {
      foreach (array(self::ASH1 => 'ASH Test One', self::ASH2 => 'ASH Test Two', 9359 => 'ASH Test Three', self::ASH3 => 'ASH Test Three essay') as $nid => $title) {
        $this->site->createNode('quiz', array('nid' => $nid, 'is_new' => TRUE, 'title' => $title));
      }
      self::$quizzes = TRUE;
    }
  }

  public static function tearDownAfterClass(): void {
    self::$quizzes = FALSE;
    parent::tearDownAfterClass();
  }

  private function quiz($nid) {
    return (object) array('nid' => $nid);
  }

  private function mailsTo($account, $subject) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($account, $subject) {
      return $m['to'] === $account->mail && $m['subject'] === $subject;
    }));
  }

  /**
   * A session report by $sh with characters of $players (new players by default).
   */
  private function session($sh, $players = 2, array $values = array()) {
    $characters = array();
    for ($i = 0; $i < $players; $i++) {
      $characters[] = array('nid' => $this->site->createCharacter($this->site->createUser(array('player')))->nid);
    }
    return $this->site->createNode('session_report', $values + array(
      'uid' => $sh->uid,
      'title' => $this->site->uniqueName('Session'),
      'field_characters_involved' => $characters,
    ));
  }

  private function qualifyMessage($report) {
    $this->login($this->site->createUser(array('player'), array('view session qualify')));
    $this->get('node/' . $report->nid);
    preg_match('/This session report (does NOT|does) count towards session requirements on site\./', $this->pageText(), $m);
    return $m[1] ?? NULL;
  }

  // ---------------------------------------------------------------------------
  // Settings.
  // ---------------------------------------------------------------------------

  public function testSettingsNeedPermission() {
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('admin/vaxia/sh_test');
    $this->assertStatus(403);
  }

  public function testSettingsPageShowsDefaults() {
    $this->login($this->site->createUser(array(), array('administrate sh test')));
    $this->get('admin/vaxia/sh_test');
    $this->assertSame('4', $this->xpath()->query('//input[@name="sh_test_starvation_mode"]')->item(0)->getAttribute('value'));
    $this->assertSame('2', $this->xpath()->query('//input[@name="sh_test_session_party_size_min"]')->item(0)->getAttribute('value'));
    $this->assertSame('3 years', $this->xpath()->query('//input[@name="sh_test_session_expires_timeframe"]')->item(0)->getAttribute('value'));
  }

  public function testRecalculateEarnedXp() {
    $this->login($this->site->createUser(array(), array('administrate sh test')));
    $this->submitForm('admin/vaxia/sh_test', array(), 'Recalculate Earned XP');
    $this->assertContains('SH totals for all active players have been reset.', $this->messages('status'));
  }

  /**
   * @group knownbug
   * '#collpased' (sic) on the Session requirements fieldset: it's meant to
   * start collapsed but doesn't.
   */
  public function testSessionRequirementsStartCollapsed() {
    $this->login($this->site->createUser(array(), array('administrate sh test')));
    $this->get('admin/vaxia/sh_test');
    $this->assertSame(1, $this->xpath()->query($this->collapsedXpath() . '[.//input[@name="sh_test_session_party_size_min"]]')->length);
  }

  // ---------------------------------------------------------------------------
  // Quiz pages.
  // ---------------------------------------------------------------------------

  public function testIneligiblePlayersAreWarnedOnShTests() {
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . self::ASH1);
    $this->assertStringContainsString('You are not yet eligible to become a Storyhost.', implode(' ', $this->messages()));
  }

  public function testEligiblePlayersAreNotWarned() {
    $this->login($this->site->createUser(array('player', 'eligible for SH')));
    $this->get('node/' . self::ASH1);
    $this->assertStringNotContainsString('You are not yet eligible', implode(' ', $this->messages()));
  }

  public function testOnlyAsh1sMayPassAsh2() {
    $this->login($this->site->createUser(array('player', 'eligible for SH')));
    $this->get('node/' . self::ASH2);
    $this->assertStringContainsString('You are not eligible to pass ASH Test 2.', implode(' ', $this->messages()));
  }

  /**
   * @group knownbug
   * The ASH 2 warning checks "not ASH 1 OR has run 3 sessions": an ASH 1 who has
   * run their sessions is told they're not eligible, and one who hasn't isn't
   * warned. (ASH 3 has the same inverted check.)
   */
  public function testAsh1WithSessionsIsNotWarned() {
    $ash = $this->site->createUser(array('player', 'eligible for SH', 'storyteller-newbie'));
    for ($i = 0; $i < 3; $i++) {
      $this->session($ash);
    }
    $this->login($ash);
    $this->get('node/' . self::ASH2);
    $this->assertStringNotContainsString('You are not eligible to pass ASH Test 2.', implode(' ', $this->messages()));
  }

  /**
   * @group knownbug
   * Same inverted check: an ASH 1 with no sessions isn't warned.
   */
  public function testAsh1WithoutSessionsIsWarned() {
    $this->login($this->site->createUser(array('player', 'eligible for SH', 'storyteller-newbie')));
    $this->get('node/' . self::ASH2);
    $this->assertStringContainsString('You are not eligible to pass ASH Test 2.', implode(' ', $this->messages()));
  }

  // ---------------------------------------------------------------------------
  // Passing the tests.
  // ---------------------------------------------------------------------------

  public function testPassingAsh1MakesAnAsh1() {
    $staff = $this->site->createUser(array('storyteller'));
    $player = $this->site->createUser(array('player', 'eligible for SH'));
    $character = $this->site->createCharacter($player);
    $this->site->setUserFields($player, array('field_storyhost_character' => $character->nid));

    $messages = $this->site->passQuiz($player, $this->quiz(self::ASH1));

    $this->assertContains('Congratulations, you have passed the ASH Test One.', $messages);
    $this->assertContains('storyteller-newbie', $this->rolesOf($player));
    $this->assertCount(1, $this->mailsTo($player, '[Vaxia] You have passed ASH Test One'));
    $this->assertCount(1, $this->mailsTo($staff, '[Vaxia] A new SH applicant has completed ASH Test One'));
    $this->assertSame('approved', $this->workflowState($character), 'Storyhost character approved');
  }

  public function testIneligiblePlayersPassingAsh1StayPlayers() {
    $player = $this->site->createUser(array('player'));
    $this->site->passQuiz($player, $this->quiz(self::ASH1));
    $this->assertNotContains('storyteller-newbie', $this->rolesOf($player));
  }

  public function testPassingAsh2WithThreeSessionsMakesAnAsh2() {
    $ash = $this->site->createUser(array('player', 'eligible for SH', 'storyteller-newbie'));
    for ($i = 0; $i < 3; $i++) {
      $this->session($ash);
    }

    $messages = $this->site->passQuiz($ash, $this->quiz(self::ASH2));

    $this->assertContains('Congratulations, you have passed the ASH Test Two.', $messages);
    $roles = $this->rolesOf($ash);
    $this->assertContains('storyteller-assistant', $roles);
    $this->assertNotContains('storyteller-newbie', $roles);
    $this->assertCount(1, $this->mailsTo($ash, '[Vaxia] You have passed ASH Test Two'));
  }

  public function testPassingAsh2WithoutSessionsChangesNothing() {
    $ash = $this->site->createUser(array('player', 'eligible for SH', 'storyteller-newbie'));
    $this->session($ash);
    $this->site->passQuiz($ash, $this->quiz(self::ASH2));
    $this->assertContains('storyteller-newbie', $this->rolesOf($ash));
  }

  // ---------------------------------------------------------------------------
  // Sessions.
  // ---------------------------------------------------------------------------

  public function testSessionWithTwoPlayersQualifies() {
    $this->assertSame('does', $this->qualifyMessage($this->session($this->site->createUser(array('storyteller')))));
  }

  public function testSessionWithOnePlayerDoesNotQualify() {
    $this->assertSame('does NOT', $this->qualifyMessage($this->session($this->site->createUser(array('storyteller')), 1)));
  }

  public function testNpcsAndOwnCharactersDoNotCount() {
    $sh = $this->site->createUser(array('storyteller'));
    $npc = $this->site->createCharacter($this->site->createUser(array('player')), array('field_is_npc' => 1));
    $own = $this->site->createCharacter($sh);
    $player = $this->site->createCharacter($this->site->createUser(array('player')));
    $report = $this->site->createNode('session_report', array(
      'uid' => $sh->uid,
      'field_characters_involved' => array(array('nid' => $npc->nid), array('nid' => $own->nid), array('nid' => $player->nid)),
    ));
    $this->assertSame('does NOT', $this->qualifyMessage($report));
  }

  public function testOldSessionsDoNotQualify() {
    $report = $this->session($this->site->createUser(array('storyteller')));
    $this->site->backdateNodeCreated($report, strtotime('-4 years'));
    $this->assertSame('does NOT', $this->qualifyMessage($report));
  }

  public function testQualifyNoteNeedsPermission() {
    $this->withoutPermission('view session qualify', array('anonymous user', 'authenticated user', 'player'), function () {
      $report = $this->session($this->site->createUser(array('storyteller')));
      $this->login($this->site->createUser(array('player')));
      $this->get('node/' . $report->nid);
      $this->assertNoText('count towards session requirements');
    });
  }

  public function testAshSessionsAskPlayersForFeedback() {
    $ash = $this->site->createUser(array('player', 'storyteller-newbie'));
    $player = $this->site->createUser(array('player'));
    $this->site->createNode('session_report', array(
      'uid' => $ash->uid,
      'field_characters_involved' => array(array('nid' => $this->site->createCharacter($player)->nid)),
    ));
    $this->assertCount(1, $this->mailsTo($player, '[Vaxia] ' . $ash->name . ' feedback requested!'));
  }

  public function testFullStoryhostSessionsDoNotAskForFeedback() {
    $sh = $this->site->createUser(array('storyteller'));
    $player = $this->site->createUser(array('player'));
    $this->site->createNode('session_report', array(
      'uid' => $sh->uid,
      'field_characters_involved' => array(array('nid' => $this->site->createCharacter($player)->nid)),
    ));
    $this->assertSame(array(), $this->mailsTo($player, '[Vaxia] ' . $sh->name . ' feedback requested!'));
  }

  /**
   * @group knownbug
   * sh_test_session_counts() loops over field_characters_involved without
   * checking it's set: a session report with no characters prints a warning
   * to anyone who may see whether it qualifies.
   */
  public function testSessionWithoutCharactersHasNoPhpErrors() {
    $report = $this->site->createNode('session_report', array('uid' => $this->site->createUser(array('storyteller'))->uid));
    $this->qualifyMessage($report);
    $this->assertSame(array(), $this->pageErrorsFrom('sh_test.module'));
  }

  // ---------------------------------------------------------------------------
  // Applications.
  // ---------------------------------------------------------------------------

  private function application($applicant) {
    return $this->site->createWebformNode('storyhost_application', array('uid' => $applicant->uid));
  }

  public function testApplicantCannotVoteOnOwnApplication() {
    $applicant = $this->site->createUser(array('player', 'storyteller-candidate'));
    $application = $this->application($applicant);
    $this->login($applicant);
    $this->get('node/' . $application->nid);
    $this->assertSame(0, $this->xpath()->query($this->webformFormXpath())->length);
  }

  /**
   * The reminder rule ("ADJUST: SH Test - Candidate Reminder") ships disabled,
   * so cron's overdue check sends nothing.
   */
  public function testWeekOldApplicationRemindersAreSwitchedOff() {
    $staff = $this->site->createUser(array('storyteller'));
    $application = $this->application($this->site->createUser(array('player')));
    $this->site->backdateNode($application, strtotime('-8 days'));
    $this->site->setConfig('sh_test_cron', 0);

    $this->runCron();

    $this->assertSame(array(), $this->mailsTo($staff, '[Vaxia] An ASH application has been waiting for a week'));
  }

}
