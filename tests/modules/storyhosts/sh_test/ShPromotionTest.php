<?php

namespace Vaxia\Tests\Storyhosts\ShTest;

use Vaxia\Tests\VaxiaTestCase;

/**
 * SH test: promotion to storyhost. A storyhost candidate's storyteller
 * application (role_app) is voted on; once more than 75% of the storyhost
 * approvers (holders of roles with "approve sh test") and at least 75% of the
 * application's approvers approve, the candidate becomes a storyhost: the
 * ballot closes, their storyhost character is titled "SH", the course dates
 * are set, powers expire in three months, and storyhosts are told.
 *
 * Setup: a fresh role per test both approves storyteller applications and
 * holds "approve sh test" (revoked from every other role), so other accounts
 * don't count as approvers.
 *
 * @group sh_test
 */
class ShPromotionTest extends VaxiaTestCase {

  /** Roles that hold the course's approving permissions on production. */
  const APPROVING = array('storyteller', 'administrator', 'lead', 'starvation-mode-ash-approver');

  /** @var string Approving role (fresh per test). */
  private $approverRole;

  /** @var object The storyteller application quiz. */
  private $quiz;

  protected function setUp(): void {
    parent::setUp();
    $this->approverRole = $this->site->createRole();
    $this->quiz = $this->site->createNode('quiz', array('title' => $this->site->uniqueName('ASH Test Three')));
    $this->inSession($this->superuser(), function () {
      $this->get('admin/rpg/role_app');
      $applied = $this->optionValue('role_app_roles[]', 'storyteller');
      $this->submitForm(NULL, array('role_app_roles[]' => array($applied)), 'Save configuration');
      $this->get('admin/rpg/role_app');
      $this->submitForm(NULL, array(
        'role_app_quiz_' . $applied . '[]' => array($this->quiz->nid),
        'role_app_eligible_' . $applied => $this->optionValue('role_app_eligible_' . $applied, 'storyteller-candidate'),
        'role_app_approve_' . $applied . '[]' => array($this->optionValue('role_app_approve_' . $applied . '[]', $this->approverRole)),
        'role_app_view_' . $applied . '[]' => array($this->optionValue('role_app_view_' . $applied . '[]', 'lead')),
      ), 'Save configuration');
      // The boundary tests assume 75 (D7's default; production used 74).
      $this->submitForm('admin/vaxia/sh_test', array('sh_test_approval_min' => '75'), 'Save configuration');
    });
  }

  /**
   * Runs $fn with "approve sh test" only on the fresh role (or, with
   * $approverRoleApproves FALSE, on no role) and "approve sh starvation" on
   * no role unless $fn grants it.
   */
  private function withApprovers($fn, $approverRoleApproves = TRUE) {
    return $this->withoutPermission('approve sh test', self::APPROVING, function () use ($fn, $approverRoleApproves) {
      return $this->withoutPermission('approve sh starvation', self::APPROVING, function () use ($fn, $approverRoleApproves) {
        if ($approverRoleApproves) {
          $this->site->grantPermissions($this->approverRole, array('approve sh test'));
        }
        try {
          return $fn();
        }
        finally {
          // Earlier tests' approvers would count in later tests.
          $this->site->revokePermissions($this->approverRole, array('approve sh test', 'approve sh starvation'));
        }
      });
    });
  }

  /**
   * A storyhost candidate with a storyhost character, who has applied.
   */
  private function candidate() {
    $candidate = $this->site->createUser(array('player', 'eligible for SH', 'storyteller-candidate'));
    $candidate->character = $this->site->createCharacter($candidate);
    $this->site->setUserFields($candidate, array('field_storyhost_character' => $candidate->character->nid));
    $this->site->passQuiz($candidate, $this->quiz);
    $candidate->application = $this->applicationOf($candidate);
    $this->assertNotNull($candidate->application, 'Application created');
    return $candidate;
  }

  private function applicationOf($applicant) {
    return $this->inSession($this->superuser(), function () use ($applicant) {
      $this->get('admin/content', array('type' => 'application'));
      foreach ($this->xpath()->query('//a[contains(text(), "Application (' . $applicant->name . ')")]') as $a) {
        if (preg_match('#/node/(\d+)$#', $a->getAttribute('href'), $m)) {
          return (object) array('nid' => (int) $m[1]);
        }
      }
      return NULL;
    });
  }

  private function approvers($count) {
    $out = array();
    for ($i = 0; $i < $count; $i++) {
      $out[] = $this->site->createUser(array($this->approverRole));
    }
    return $out;
  }

  private function vote($approver, $application, $choice = 'approve') {
    $this->login($approver);
    $this->submitForm('node/' . $application->nid, array(
      'submitted[approval]' => $choice,
      'submitted[comments]' => 'Looks ' . $choice,
    ), 'Submit');
    return implode(' ', $this->messages());
  }

  private function canVote($account, $application) {
    $this->login($account);
    $this->get('node/' . $application->nid);
    return $this->xpath()->query('//select[@name="' . $this->fieldName('submitted[approval]') . '" and not(@disabled)]')->length === 1;
  }

  private function mailsTo($account, $subject) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($account, $subject) {
      return $m['to'] === $account->mail && $m['subject'] === $subject;
    }));
  }

  public function testAllStoryhostApprovalsPromoteTheCandidate() {
    $this->withApprovers(function () {
      $candidate = $this->candidate();
      $approvers = $this->approvers(4);
      $storyhost = $this->site->createUser(array('storyteller'));

      foreach (array_slice($approvers, 0, 3) as $approver) {
        $this->vote($approver, $candidate->application);
      }
      $this->assertFalse($this->hasRole($candidate, 'storyteller'), '75% is not more than 75%');

      $messages = $this->vote($approvers[3], $candidate->application);

      $this->assertStringContainsString('SH Candidate ' . $candidate->name . ' has received three approvals and has been promoted to a full Storyhost.', $messages);
      $roles = $this->rolesOf($candidate);
      $this->assertContains('storyteller', $roles);
      $this->assertNotContains('storyteller-candidate', $roles);
      $this->assertCount(1, $this->mailsTo($candidate, '[Vaxia] SH privileges granted'));
      $this->assertCount(1, $this->mailsTo($storyhost, '[Vaxia] New SH approved'));
      $this->assertFalse($this->canVote($this->site->createUser(array($this->approverRole)), $candidate->application), 'Ballot closed');
      $profile = $this->profileText($candidate);
      foreach (array('Took ASH 1', 'Took ASH 2', 'Took ASH 3', 'Became storyhost') as $label) {
        $this->assertStringContainsString($label, $profile);
      }
      $this->assertStringContainsString(date('l, F j, Y', strtotime('+3 months')), $profile, 'Powers expire in three months');
      $this->assertMatchesRegularExpression('/Titles:?\s*SH\b/', $this->nodeText($candidate->character));
    });
  }

  public function testDisapprovalsKeepTheCandidate() {
    $this->withApprovers(function () {
      $candidate = $this->candidate();
      $approvers = $this->approvers(4);
      foreach ($approvers as $i => $approver) {
        $this->vote($approver, $candidate->application, $i === 0 ? 'disapprove' : 'approve');
      }
      $this->assertFalse($this->hasRole($candidate, 'storyteller'));
      $this->assertTrue($this->hasRole($candidate, 'storyteller-candidate'));
    });
  }

  public function testCandidatesWithoutAStoryhostCharacterAreNotPromoted() {
    $this->withApprovers(function () {
      $candidate = $this->site->createUser(array('player', 'eligible for SH', 'storyteller-candidate'));
      $this->site->passQuiz($candidate, $this->quiz);
      $application = $this->applicationOf($candidate);
      foreach ($this->approvers(4) as $approver) {
        $this->vote($approver, $application);
      }
      $this->assertFalse($this->hasRole($candidate, 'storyteller'));
    });
  }

  /**
   * Starvation mode: with fewer than four holders of "approve sh test" (here
   * three), the holders of "approve sh starvation" decide, and more than half
   * is enough.
   */
  public function testStarvationModeNeedsHalfOfTheStarvationApprovers() {
    $this->withApprovers(function () {
      $this->site->grantPermissions($this->approverRole, array('approve sh starvation'));
      $candidate = $this->candidate();
      $approvers = $this->approvers(3);
      $this->vote($approvers[0], $candidate->application);
      $this->assertFalse($this->hasRole($candidate, 'storyteller'), 'A third is not enough');
      $this->vote($approvers[1], $candidate->application);
      // Two of three: over half of the starvation approvers, and over 75% of
      // the application's approvers is not yet reached (66%).
      $this->assertFalse($this->hasRole($candidate, 'storyteller'), "The application's own bar still applies");
      $this->vote($approvers[2], $candidate->application);
      $this->assertTrue($this->hasRole($candidate, 'storyteller'));
    });
  }

  /**
   * @group knownbug
   * sh_test_check_ash_3_approvals() divides the approvals by the number of
   * possible storyhost approvers. In starvation mode (here one holder of
   * "approve sh test") with nobody holding "approve sh starvation" that's a
   * division by zero (a warning today, fatal on PHP 8), and the resulting INF
   * passes the check: the candidate is promoted without any storyhost's
   * approval.
   */
  public function testNoStoryhostApproversPromotesNobody() {
    $this->withApprovers(function () {
      $candidate = $this->candidate();
      list($approver) = $this->approvers(1);
      $this->vote($approver, $candidate->application);
      $this->assertSame(array(), $this->pageErrorsFrom('sh_test.module'));
      $this->assertFalse($this->hasRole($candidate, 'storyteller'));
    });
  }

}
