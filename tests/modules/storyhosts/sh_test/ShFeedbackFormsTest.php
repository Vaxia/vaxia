<?php

namespace Vaxia\Tests\Storyhosts\ShTest;

use Vaxia\Tests\VaxiaTestCase;

/**
 * SH test feedback forms: two webforms at production nodes. "Storyhost
 * Feedback" (9832), filled in by storyhosts, ASHs, candidates and leads, is
 * mailed to the storyhost it names and to the leads. "Session Feedback"
 * (9907), filled in by players, goes to the storyhosts only (not to the ASH).
 *
 * The forms are open to every account here, so the rules' own role checks
 * decide.
 *
 * @group sh_test
 */
class ShFeedbackFormsTest extends VaxiaTestCase {

  const SH_FEEDBACK = 9832;
  const SESSION_FEEDBACK = 9907;

  protected function setUp(): void {
    parent::setUp();
    // Components as on production (form keys, component ids).
    $this->site->createWebformAt(self::SH_FEEDBACK, 'Storyhost Feedback', array(2), array(
      1 => array('cid' => 1, 'pid' => 0, 'form_key' => 'feedback', 'name' => 'Feedback', 'type' => 'textarea', 'value' => '', 'extra' => array(), 'mandatory' => 0, 'weight' => 1),
      2 => array('cid' => 2, 'pid' => 0, 'form_key' => 'storyhost', 'name' => 'Storyhost', 'type' => 'textfield', 'value' => '', 'extra' => array(), 'mandatory' => 0, 'weight' => 0),
    ));
    $this->site->createWebformAt(self::SESSION_FEEDBACK, 'Session Feedback', array(2), array(
      1 => array('cid' => 1, 'pid' => 0, 'form_key' => 'storyhost', 'name' => 'Storyhost', 'type' => 'textfield', 'value' => '', 'extra' => array(), 'mandatory' => 0, 'weight' => 0),
      2 => array('cid' => 2, 'pid' => 0, 'form_key' => 'feedback', 'name' => 'Feedback', 'type' => 'textarea', 'value' => '', 'extra' => array(), 'mandatory' => 0, 'weight' => 1),
    ));
  }

  private function send($account, $nid, $storyhost, $feedback) {
    $this->login($account);
    $this->submitForm('node/' . $nid, array(
      'submitted[storyhost]' => $storyhost,
      'submitted[feedback]' => $feedback,
    ), 'Submit');
  }

  /**
   * Mails to $account with $subject, bodies with line wrapping undone.
   */
  private function mailsTo($account, $subject) {
    return array_map(function ($m) {
      $m['body'] = preg_replace('/\s+/', ' ', $m['body']);
      return $m;
    }, array_values(array_filter($this->sentMail(), function ($m) use ($account, $subject) {
      return $m['to'] === $account->mail && $m['subject'] === $subject;
    })));
  }

  private function mailsWithSubject($subject) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($subject) {
      return $m['subject'] === $subject;
    }));
  }

  // ---------------------------------------------------------------------------
  // Storyhost feedback.
  // ---------------------------------------------------------------------------

  public function testStaffFeedbackGoesToTheStoryhostAndLeads() {
    $lead = $this->site->createUser(array('lead'));
    $ash = $this->site->createUser(array('player', 'storyteller-assistant'));
    $bystander = $this->site->createUser(array('player'));

    $this->send($this->site->createUser(array('storyteller')), self::SH_FEEDBACK, $ash->name, 'Clear rulings and good pacing.');

    $mails = $this->mailsTo($ash, '[Vaxia] Storyhost feedback');
    $this->assertCount(1, $mails, 'The storyhost named');
    $this->assertStringContainsString('The following feedback was received for Storyhost ' . $ash->name, $mails[0]['body']);
    $this->assertStringContainsString('Clear rulings and good pacing.', $mails[0]['body']);
    $this->assertCount(1, $this->mailsTo($lead, '[Vaxia] Storyhost feedback'), 'Leads');
    $this->assertSame(array(), $this->mailsTo($bystander, '[Vaxia] Storyhost feedback'));
  }

  public function testCandidatesAndLeadsMaySendStoryhostFeedback() {
    $ash = $this->site->createUser(array('player', 'storyteller-newbie'));
    $this->send($this->site->createUser(array('player', 'storyteller-candidate')), self::SH_FEEDBACK, $ash->name, 'From a candidate.');
    $this->send($this->site->createUser(array('lead')), self::SH_FEEDBACK, $ash->name, 'From a lead.');
    $this->assertCount(2, $this->mailsTo($ash, '[Vaxia] Storyhost feedback'));
  }

  public function testPlayersStoryhostFeedbackIsNotMailed() {
    $this->site->createUser(array('lead'));
    $ash = $this->site->createUser(array('player', 'storyteller-assistant'));
    $this->send($this->site->createUser(array('player')), self::SH_FEEDBACK, $ash->name, 'Not staff.');
    $this->assertSame(array(), $this->mailsWithSubject('[Vaxia] Storyhost feedback'));
  }

  // ---------------------------------------------------------------------------
  // Session feedback.
  // ---------------------------------------------------------------------------

  public function testPlayersSessionFeedbackGoesToStorytellers() {
    $sh = $this->site->createUser(array('storyteller'));
    $ash = $this->site->createUser(array('player', 'storyteller-newbie'));

    $this->send($this->site->createUser(array('player')), self::SESSION_FEEDBACK, $ash->name, 'More combat next time.');

    $mails = $this->mailsTo($sh, '[Vaxia] Storyhost session feedback');
    $this->assertCount(1, $mails);
    $this->assertStringContainsString('The following feedback was received for Storyhost ' . $ash->name . ' based on their latest session report.', $mails[0]['body']);
    $this->assertStringContainsString('More combat next time.', $mails[0]['body']);
    $this->assertSame(array(), $this->mailsTo($ash, '[Vaxia] Storyhost session feedback'), 'Not forwarded to the ASH');
  }

  /**
   * D7 also looked the storyhost up from the Storyhost Feedback form's latest
   * submission (never used): the mail names the storyhost from this form.
   */
  public function testSessionFeedbackNamesItsOwnStoryhost() {
    $sh = $this->site->createUser(array('storyteller'));
    $other = $this->site->createUser(array('player', 'storyteller-assistant'));
    $ash = $this->site->createUser(array('player', 'storyteller-newbie'));
    $this->send($this->site->createUser(array('lead')), self::SH_FEEDBACK, $other->name, 'About someone else.');

    $this->send($this->site->createUser(array('player')), self::SESSION_FEEDBACK, $ash->name, 'About this session.');

    $mails = $this->mailsTo($sh, '[Vaxia] Storyhost session feedback');
    $this->assertCount(1, $mails);
    $this->assertStringContainsString('for Storyhost ' . $ash->name . ' based on', $mails[0]['body']);
    $this->assertStringNotContainsString($other->name, $mails[0]['body']);
  }

  public function testNonPlayersSessionFeedbackIsNotMailed() {
    $this->site->createUser(array('storyteller'));
    $ash = $this->site->createUser(array('player', 'storyteller-newbie'));
    $this->send($this->site->createUser(array('lead')), self::SESSION_FEEDBACK, $ash->name, 'Not a player.');
    $this->assertSame(array(), $this->mailsWithSubject('[Vaxia] Storyhost session feedback'));
  }

}
