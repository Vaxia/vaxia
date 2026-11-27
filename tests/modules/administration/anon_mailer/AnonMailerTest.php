<?php

namespace Vaxia\Tests\Administration\AnonMailer;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Anonymous mailer: a webform (production node 9368, open to everyone) whose
 * submissions are mailed to leads and admins without identifying the sender,
 * and announced on the forums. Marked as a grievance (with a violated principle
 * and named members) it's posted to the forums as a grievance; marked but
 * incomplete, the sender is told what's missing.
 *
 * The rules decide by pasting submitted values into PHP code ([data:...-value]
 * tokens inside php_eval): see the security knownbug.
 *
 * @group anon_mailer
 */
class AnonMailerTest extends VaxiaTestCase {

  const MAILER = 9368;

  protected function setUp(): void {
    parent::setUp();
    $this->site->createWebformAt(self::MAILER, 'Anonymous mailer', array(1, 2), array(
      1 => array('cid' => 1, 'pid' => 0, 'form_key' => 'message', 'name' => 'Your message', 'type' => 'textarea', 'value' => '', 'extra' => array(), 'mandatory' => 0, 'weight' => 0),
      4 => array('cid' => 4, 'pid' => 0, 'form_key' => 'grievance_violation', 'name' => 'What sort of grievance is it?', 'type' => 'select', 'value' => '',
        'extra' => array('items' => "3F_fun|Fun\n3F_free|Freedom\n3F_fair|Fairness\ntransparency|Transparency", 'multiple' => 0, 'aslist' => 1), 'mandatory' => 0, 'weight' => 1),
      5 => array('cid' => 5, 'pid' => 0, 'form_key' => 'grievance_request', 'name' => 'Is this a grievance request?', 'type' => 'select', 'value' => 'no',
        'extra' => array('items' => "no|This is NOT a grievance request.\nyes|This is a grievance request.", 'multiple' => 0, 'aslist' => 1), 'mandatory' => 0, 'weight' => 2),
      7 => array('cid' => 7, 'pid' => 0, 'form_key' => 'grievance_names', 'name' => 'Who are you naming in this grievance?', 'type' => 'textfield', 'value' => '', 'extra' => array(), 'mandatory' => 0, 'weight' => 3),
    ));
  }

  private function send(array $values) {
    $edit = array();
    foreach ($values as $key => $value) {
      $edit['submitted[' . $key . ']'] = $value;
    }
    $this->anonymous();
    $this->submitForm('node/' . self::MAILER, $edit, 'Submit');
    return implode(' ', $this->messages());
  }

  private function forumTitles() {
    return $this->inSession($this->superuser(), function () {
      $this->get('admin/content', array('type' => 'forum'));
      $titles = array();
      foreach ($this->xpath()->query('//table//td/a') as $a) {
        $titles[] = trim($a->textContent);
      }
      return $titles;
    });
  }

  public function testMailIsForwardedToLeadsAndAdmins() {
    $lead = $this->site->createUser(array('lead'));
    $player = $this->site->createUser(array('player'));

    $messages = $this->send(array('message' => 'The tavern rules are unclear.'));

    $this->assertStringContainsString('Your mailing has been forwarded to the current Leads and Technical Admin', $messages);
    $mails = array_values(array_filter($this->sentMail(), function ($m) use ($lead) {
      return $m['to'] === $lead->mail && $m['subject'] === '[Vaxia] Anonymous Mailer Submission';
    }));
    $this->assertCount(1, $mails);
    $this->assertStringContainsString('The tavern rules are unclear.', $mails[0]['body']);
    $this->assertNotContains($player->mail, $this->mailRecipients());
  }

  public function testNonGrievanceIsAnnouncedOnTheForums() {
    $messages = $this->send(array('message' => 'Hello', 'grievance_request' => 'no'));
    $this->assertStringContainsString('a notice of your non-grievance mailing has been posted to the forums', $messages);
    $this->assertNotEmpty(preg_grep('/^Anonymous Mail Posting /', $this->forumTitles()));
  }

  public function testSettingsNeedPermission() {
    $this->login($this->site->createUser(array('lead')));
    $this->get('admin/vaxia/anon_mailer');
    $this->assertStatus(403);
    $this->login($this->site->createUser(array(), array('administrate anon mailer')));
    $this->get('admin/vaxia/anon_mailer');
    $this->assertStatus(200);
    $this->assertSame(1, $this->xpath()->query('//select[@name="anon_mailer_webform"]/option[@value="' . self::MAILER . '"]')->length);
  }

  public function testCompleteGrievanceIsPosted() {
    $messages = $this->send(array(
      'message' => 'Abuse of power.',
      'grievance_request' => 'yes',
      'grievance_violation' => 'transparency',
      'grievance_names' => 'SomeLead',
    ));
    $this->assertStringContainsString('your grievance has been posted to the forums', $messages);
    $this->assertNotEmpty(preg_grep('/^Grievance Posting /', $this->forumTitles()));
  }

  public function testIncompleteGrievanceIsRejected() {
    $messages = $this->send(array('message' => 'Abuse!', 'grievance_request' => 'yes'));
    $this->assertStringContainsString('Your grievance posting is incomplete.', $messages);
  }

  /**
   * @group knownbug
   * SECURITY (critical): remote code execution by anonymous visitors. The
   * grievance rules ("Grievance Posting", "... - Incomplete") paste
   * [data:grievance_names-value], a free-text field, into php_eval code inside
   * double quotes. A name like  x"); <any PHP>; ("  runs on the server.
   */
  public function testSubmittedTextIsNeverRunAsPhp() {
    $messages = $this->send(array(
      'message' => 'probe',
      'grievance_request' => 'yes',
      'grievance_violation' => 'transparency',
      'grievance_names' => 'x"); drupal_set_message("INJ" . "ECTED"); ("',
    ));
    $this->assertStringNotContainsString('INJECTED', $messages);
  }

}
