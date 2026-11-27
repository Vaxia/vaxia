<?php

namespace Vaxia\Tests\Vaxia\VaxiaElections;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia elections: ships the lead and starvation-mode-ash-approver roles. An
 * ASH 1 (eligible for SH and storyteller-newbie: the rule needs both) who
 * files a session report becomes a "starvation-mode-ash-approver" (allowed to
 * approve ASH 3 when storyhosts are scarce).
 *
 * @group vaxia_elections
 */
class VaxiaElectionsTest extends VaxiaTestCase {

  private function session($author) {
    return $this->site->createNode('session_report', array('uid' => $author->uid, 'title' => $this->site->uniqueName('Session')));
  }

  public function testAsh1SessionMakesAStarvationApprover() {
    $ash = $this->site->createUser(array('player', 'eligible for SH', 'storyteller-newbie'));
    $this->session($ash);
    $this->assertContains('starvation-mode-ash-approver', $this->rolesOf($ash));
  }

  public function testEligibleOnlyDoesNotQualify() {
    $player = $this->site->createUser(array('player', 'eligible for SH'));
    $this->session($player);
    $this->assertNotContains('starvation-mode-ash-approver', $this->rolesOf($player));
  }

  public function testOthersDoNotBecomeApprovers() {
    $player = $this->site->createUser(array('player'));
    $this->session($player);
    $this->assertNotContains('starvation-mode-ash-approver', $this->rolesOf($player));
  }

}
