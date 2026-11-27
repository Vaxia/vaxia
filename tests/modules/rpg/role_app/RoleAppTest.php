<?php

namespace Vaxia\Tests\Rpg\RoleApp;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Role applications: passing a role's quiz creates an application webform;
 * holders of the approving roles vote on it, and once enough approve, the
 * applicant gets the role.
 *
 * Setup: applying for "storyteller-candidate"; "eligible for SH" may apply; a
 * fresh role per test approves (so other tests' users don't count); leads view.
 *
 * @group role_app
 */
class RoleAppTest extends VaxiaTestCase {

  /** @var string Approving role (fresh per test). */
  private $approverRole;

  /** @var object The application quiz. */
  private $quiz;

  protected function setUp(): void {
    parent::setUp();
    $this->approverRole = $this->site->createRole();
    $this->quiz = $this->site->createNode('quiz', array('title' => $this->site->uniqueName('Candidate quiz')));
    $this->configure('storyteller-candidate', $this->quiz, 'eligible for SH', $this->approverRole);
  }

  /**
   * Settings page, in two saves: the per-role section appears once the role is
   * chosen as an application role.
   */
  private function configure($appliedRole, $quiz, $eligibleRole, $approverRole) {
    $this->inSession($this->superuser(), function () use ($appliedRole, $quiz, $eligibleRole, $approverRole) {
      $this->get('admin/rpg/role_app');
      $applied = $this->optionValue('role_app_roles[]', $appliedRole);
      $this->submitForm(NULL, array('role_app_roles[]' => array($applied)), 'Save configuration');
      $this->get('admin/rpg/role_app');
      $this->submitForm(NULL, array(
        'role_app_quiz_' . $applied . '[]' => array($quiz->nid),
        'role_app_eligible_' . $applied => $this->optionValue('role_app_eligible_' . $applied, $eligibleRole),
        'role_app_approve_' . $applied . '[]' => array($this->optionValue('role_app_approve_' . $applied . '[]', $approverRole)),
        'role_app_view_' . $applied . '[]' => array($this->optionValue('role_app_view_' . $applied . '[]', 'lead')),
      ), 'Save configuration');
    });
  }

  private function approvers($count) {
    $out = array();
    for ($i = 0; $i < $count; $i++) {
      $out[] = $this->site->createUser(array($this->approverRole));
    }
    return $out;
  }

  /**
   * The applicant's application, from the admin content list.
   */
  private function applicationOf($applicant) {
    return $this->inSession($this->superuser(), function () use ($applicant) {
      $this->get('admin/content', array('type' => 'application'));
      foreach ($this->xpath()->query('//a[contains(text(), "Application (' . $applicant->name . ')")]') as $a) {
        if (preg_match('#/node/(\d+)$#', $a->getAttribute('href'), $m)) {
          return (object) array('nid' => (int) $m[1], 'title' => trim($a->textContent));
        }
      }
      return NULL;
    });
  }

  private function vote($approver, $application, $choice) {
    $this->login($approver);
    $this->submitForm('node/' . $application->nid, array(
      'submitted[approval]' => $choice,
      'submitted[comments]' => 'Looks ' . $choice,
    ), 'Submit');
  }

  private function canVote($account, $application) {
    $this->login($account);
    $this->get('node/' . $application->nid);
    return $this->xpath()->query('//select[@name="' . $this->fieldName('submitted[approval]') . '" and not(@disabled)]')->length === 1;
  }

  // ---------------------------------------------------------------------------
  // Settings page.
  // ---------------------------------------------------------------------------

  public function testSettingsPageShowsPerRoleSettings() {
    $this->login($this->site->createUser(array(), array('administrate role application')));
    $this->get('admin/rpg/role_app');
    $this->assertStatus(200);
    $this->assertText('Settings for storyteller-candidate');
    foreach (array('Application quiz', 'Application eligable role', 'Application viewing role', 'Application approving role') as $label) {
      $this->assertText($label);
    }
    $this->assertGreaterThan(0, $this->xpath()->query('//option[text()="' . $this->quiz->title . '"]')->length, 'Quizzes offered');
  }

  public function testSettingsPageNeedsPermission() {
    $this->withoutPermission('administrate role application', array('lead'), function () {
      $this->login($this->site->createUser(array('lead')));
      $this->get('admin/rpg/role_app');
      $this->assertStatus(403);
    });
  }

  /**
   * @group knownbug
   * "Approval percentage" is declared with '#type' => 'text', which isn't a
   * Form API element: it never renders, so the 75% default can't be changed.
   */
  public function testApprovalPercentageIsConfigurable() {
    $this->login($this->site->createUser(array(), array('administrate role application')));
    $this->get('admin/rpg/role_app');
    $this->assertSame(1, $this->xpath()->query('//input[@name="application_approval_min"]')->length);
  }

  // ---------------------------------------------------------------------------
  // Creating applications.
  // ---------------------------------------------------------------------------

  public function testPassingQuizCreatesApplicationForApprovers() {
    $applicant = $this->site->createUser(array('eligible for SH'));
    list($approver) = $this->approvers(1);

    $this->site->passQuiz($applicant, $this->quiz);

    $app = $this->applicationOf($applicant);
    $this->assertNotNull($app, 'Application created');
    $this->assertSame('Storyteller-candidate Application (' . $applicant->name . ')', $app->title);
    $this->assertTrue($this->canVote($approver, $app), 'Approvers get the ballot');
    $this->assertSame(1, $this->xpath()->query('//textarea[@name="' . $this->fieldName('submitted[comments]') . '"]')->length, 'With a comments box');
    $this->assertFalse($this->canVote($this->site->createUser(array('player')), $app), 'Others do not');
  }

  public function testIneligibleUserGetsNoApplication() {
    $applicant = $this->site->createUser(array('player'));
    $messages = $this->site->passQuiz($applicant, $this->quiz);
    $this->assertNull($this->applicationOf($applicant));
    $this->assertContains('You are not eligable for storyteller-candidate, the application was not created.', $messages);
  }

  public function testOtherQuizzesCreateNothing() {
    $applicant = $this->site->createUser(array('eligible for SH'));
    $this->site->passQuiz($applicant, $this->site->createNode('quiz'));
    $this->assertNull($this->applicationOf($applicant));
  }

  // ---------------------------------------------------------------------------
  // Viewing and voting.
  // ---------------------------------------------------------------------------

  public function testApplicantCannotVoteOnOwnApplication() {
    $applicant = $this->site->createUser(array('eligible for SH', $this->approverRole));
    $this->approvers(1);
    $this->site->passQuiz($applicant, $this->quiz);
    $this->assertFalse($this->canVote($applicant, $this->applicationOf($applicant)));
  }

  public function testApplicantSeesResponseTally() {
    $applicant = $this->site->createUser(array('eligible for SH'));
    $approvers = $this->approvers(4);
    $this->site->passQuiz($applicant, $this->quiz);
    $app = $this->applicationOf($applicant);
    $this->vote($approvers[0], $app, 'approve');

    $this->login($applicant);
    $this->get('node/' . $app->nid);
    $this->assertText('1 out of 4 (25 %) have responded to this application.');
  }

  public function testOutsidersDontSeeResults() {
    $applicant = $this->site->createUser(array('eligible for SH'));
    $this->approvers(1);
    $this->site->passQuiz($applicant, $this->quiz);
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $this->applicationOf($applicant)->nid);
    $this->assertNoText('have responded to this application');
  }

  public function testEnoughApprovalsGrantRoleAndCloseApplication() {
    $applicant = $this->site->createUser(array('eligible for SH'));
    $approvers = $this->approvers(4);
    $this->site->passQuiz($applicant, $this->quiz);
    $app = $this->applicationOf($applicant);

    $this->vote($approvers[0], $app, 'approve');
    $this->vote($approvers[1], $app, 'approve');
    $this->assertFalse($this->hasRole($applicant, 'storyteller-candidate'), '50% is not enough');

    $this->vote($approvers[2], $app, 'approve');
    $this->assertTrue($this->hasRole($applicant, 'storyteller-candidate'), '75% approves');
    $this->assertFalse($this->canVote($approvers[3], $app), 'Application closed');
  }

  public function testApprovalMailsTheApplicant() {
    // D7 rule "Vaxia Role App - Check webform submission" lived only in the
    // production database, so the D7 test site doesn't run it.
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('Production-only D7 rule.');
    }
    $applicant = $this->site->createUser(array('eligible for SH'));
    $approvers = $this->approvers(4);
    $this->site->passQuiz($applicant, $this->quiz);
    $app = $this->applicationOf($applicant);
    $this->vote($approvers[0], $app, 'approve');
    $this->vote($approvers[1], $app, 'approve');
    $this->clearMail();
    $this->vote($approvers[2], $app, 'approve');
    $this->assertText('Approval for ' . $applicant->name . ' has been granted! Thanks!');
    $mails = array_values(array_filter($this->sentMail(), function ($mail) {
      return $mail['subject'] === '[Vaxia] Application for Role Evaluated';
    }));
    $this->assertCount(1, $mails);
    $this->assertSame($applicant->mail, $mails[0]['to']);
    $this->assertStringContainsString('Url:  ', $mails[0]['body']);
    $this->assertStringContainsString('/node/' . $app->nid, $mails[0]['body']);
  }

  public function testDisapprovalsDontCount() {
    $applicant = $this->site->createUser(array('eligible for SH'));
    $approvers = $this->approvers(2);
    $this->site->passQuiz($applicant, $this->quiz);
    $app = $this->applicationOf($applicant);
    $this->vote($approvers[0], $app, 'approve');
    $this->vote($approvers[1], $app, 'disapprove');
    $this->assertFalse($this->hasRole($applicant, 'storyteller-candidate'));
  }

  public function testStorytellerRoleIsNeverGrantedAutomatically() {
    $quiz = $this->site->createNode('quiz');
    $this->configure('storyteller', $quiz, 'eligible for SH', $this->approverRole);
    $applicant = $this->site->createUser(array('eligible for SH'));
    list($approver) = $this->approvers(1);
    $this->site->passQuiz($applicant, $quiz);

    $this->vote($approver, $this->applicationOf($applicant), 'approve');

    $this->assertFalse($this->hasRole($applicant, 'storyteller'));
  }

  /**
   * @group knownbug
   * With no approvers (approving role empty), the tally and the approval check
   * divide by zero: a PHP warning today, a fatal DivisionByZeroError on PHP 8.
   */
  public function testApplicationWithoutApproversHasNoPhpErrors() {
    $applicant = $this->site->createUser(array('eligible for SH'));
    $this->site->passQuiz($applicant, $this->quiz);
    $this->login($applicant);
    $this->get('node/' . $this->applicationOf($applicant)->nid);
    $this->assertSame(array(), $this->pageErrorsFrom('role_app.module'));
  }

}
