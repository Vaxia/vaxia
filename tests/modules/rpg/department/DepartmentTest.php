<?php

namespace Vaxia\Tests\Rpg\Department;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Departments: a configured list of site departments, each tied to a role that may
 * vote on proposals; elected leads must name a temporary replacement before a
 * vacation.
 *
 * @group department
 */
class DepartmentTest extends VaxiaTestCase {

  private function adminLogin() {
    $this->login($this->site->createUser(array(), array('administrate departments')));
  }

  /**
   * Department "Setting" -> role knows-the-setting, "Social" -> knows-the-social,
   * "System" -> newbie-helper, through the settings page.
   */
  private function configureDepartments() {
    $this->inSession($this->superuser(), function () {
      // Save the list first: a department's role picker appears once it's saved.
      $this->submitForm('admin/rpg/departments', array('departments' => "Setting\nSystem\nSocial"), 'Save configuration');
      $this->get('admin/rpg/departments');
      $this->submitForm(NULL, array(
        'department_roles_setting' => $this->optionValue('department_roles_setting', 'knows-the-setting'),
        'department_roles_social' => $this->optionValue('department_roles_social', 'knows-the-social'),
        'department_roles_system' => $this->optionValue('department_roles_system', 'newbie-helper'),
      ), 'Save configuration');
    });
  }

  /**
   * Elected role "lead"; candidates (run roles) from $runRole, via the election settings.
   */
  private function configureElection($runRole = 'voter') {
    $this->inSession($this->superuser(), function () use ($runRole) {
      $this->get('admin/rpg/rpg_election');
      $this->submitForm(NULL, array(
        'election_role' => $this->optionValue('election_role', 'lead'),
        'election_run_roles[]' => array($this->optionValue('election_run_roles[]', $runRole)),
      ), 'Save configuration');
    });
  }

  // ---------------------------------------------------------------------------
  // Settings page.
  // ---------------------------------------------------------------------------

  public function testSettingsPageListsDefaultDepartmentsWithRolePickers() {
    $this->adminLogin();
    $this->get('admin/rpg/departments');
    $this->assertStatus(200);
    $textarea = $this->xpath()->query('//textarea[@name="departments"]')->item(0);
    $this->assertSame("Setting\nSystem\nSocial", trim(str_replace("\r", '', $textarea->textContent)));
    foreach (array('setting', 'system', 'social') as $name) {
      $this->assertSame(1, $this->xpath()->query('//select[@name="department_roles_' . $name . '"]')->length, "Role picker for $name");
    }
    $this->assertSame(1, $this->xpath()->query('//textarea[@name="department_vacation_cutoff"]')->length);
  }

  public function testSettingsPageNeedsPermission() {
    $this->login($this->site->createUser(array('lead')));
    $this->get('admin/rpg/departments');
    $this->assertStatus(403);
  }

  public function testSavingGrantsVotingToDepartmentRoles() {
    $this->withoutPermission('vote on proposal', array('knows-the-setting'), function () {
      $this->assertFalse($this->roleHasPermission('knows-the-setting', 'vote on proposal'));
      $this->adminLogin();
      $this->get('admin/rpg/departments');
      $this->submitForm(NULL, array(
        'departments' => "Setting\nCrafting Guild",
        'department_roles_setting' => $this->optionValue('department_roles_setting', 'knows-the-setting'),
      ), 'Save configuration');
      $this->assertContains('The configuration options have been saved.', $this->messages('status'));
      $this->assertTrue($this->roleHasPermission('knows-the-setting', 'vote on proposal'));
      // The new department gets its own role picker.
      $this->get('admin/rpg/departments');
      $this->assertSame(1, $this->xpath()->query('//select[@name="department_roles_crafting_guild"]')->length);
      $this->assertSame('knows-the-setting', trim($this->xpath()->query('//select[@name="department_roles_setting"]/option[@selected]')->item(0)->textContent));
    });
  }

  /**
   * @group knownbug
   * department_form__election_admin_settings_alter() targets the old form id
   * "_election_admin_settings"; the election form is now
   * "_rpg_election_admin_settings", so positions are never locked to departments.
   */
  public function testElectionPositionsLockedToDepartmentLeads() {
    $this->configureDepartments();
    $this->login($this->site->createUser(array(), array('administrate rpg_elections')));
    $this->get('admin/rpg/rpg_election');
    $textarea = $this->xpath()->query('//textarea[@name="election_positions"]')->item(0);
    $this->assertNotNull($textarea);
    $this->assertTrue($textarea->hasAttribute('disabled'));
    $this->assertSame("Setting Lead\nSystem Lead\nSocial Lead", trim(str_replace("\r", '', $textarea->textContent)));
  }

  // ---------------------------------------------------------------------------
  // Proposals.
  // ---------------------------------------------------------------------------

  public function testProposalShowsWhichDepartmentsMayVote() {
    $this->configureDepartments();
    $proposal = $this->site->createProposal(array('knows-the-social', 'voter'));
    $this->login($this->site->createUser(array('lead')));
    $this->get('node/' . $proposal->nid);
    $this->assertText('Departments that can vote on this are: Social');
    $this->assertNoText('Departments that can vote on this are: Setting');
  }

  public function testProposalWithoutDepartmentRolesShowsNoDepartmentLine() {
    $this->configureDepartments();
    $proposal = $this->site->createProposal(array('voter'));
    $this->login($this->site->createUser(array('lead')));
    $this->get('node/' . $proposal->nid);
    $this->assertNoText('Departments that can vote on this are');
  }

  public function testProposalBallotRolesAreNamedByDepartment() {
    // Saving departments gives their roles the vote; voters and leads have it too.
    $this->configureDepartments();
    $this->site->grantPermissions('voter', array('vote on proposal'));
    $this->site->grantPermissions('lead', array('vote on proposal'));
    $proposal = $this->site->createProposal(array('voter'));
    $this->login($this->superuser());
    $this->get('node/' . $proposal->nid . '/webform');
    $this->assertStatus(200);
    $this->assertText('Departments that can vote on this proposal');
    $labels = array();
    foreach ($this->xpath()->query('//input[starts-with(@name, "role_control[roles][")]/following-sibling::label') as $label) {
      $labels[] = trim($label->textContent);
    }
    $labels = array_values(array_unique($labels));
    sort($labels);
    // "lead" may vote but isn't a department role, so it is dropped.
    $this->assertSame(array('Setting Department', 'Social Department', 'System Department', 'Voters'), $labels);
  }

  /**
   * @group knownbug
   * Department role pickers have no "none" choice and default to the first role,
   * "anonymous user". Saving the page without picking a role for every
   * department grants anonymous visitors "vote on proposal".
   */
  public function testSavingDefaultsDoesNotGiveAnonymousTheVote() {
    $this->login($this->superuser());
    $this->submitForm('admin/rpg/departments', array(), 'Save configuration');
    $this->assertFalse($this->roleHasPermission('anonymous user', 'vote on proposal'));
  }

  // ---------------------------------------------------------------------------
  // Vacation: elected leads name a temporary replacement.
  // ---------------------------------------------------------------------------

  private function electedLeadWithCandidate() {
    $this->configureElection('voter');
    $lead = $this->site->createUser(array('lead'), array('can go on vacation'));
    $candidate = $this->site->createUser(array('voter'));
    return array($lead, $candidate);
  }

  private function goOnVacation($lead, $candidate, $duration = '1 month') {
    $this->login($lead);
    $this->submitForm('user/' . $lead->uid . '/edit', array(
      'vacation_end' => $duration,
      'temp_replacement' => $candidate->uid,
    ), 'Go on vacation');
  }

  public function testElectedLeadPicksReplacementFromCandidates() {
    list($lead, $candidate) = $this->electedLeadWithCandidate();
    $otherLead = $this->site->createUser(array('lead', 'voter'));

    $this->login($lead);
    $this->get('user/' . $lead->uid . '/edit');

    $options = array();
    foreach ($this->xpath()->query('//select[@name="temp_replacement"]/option') as $opt) {
      $options[(int) $opt->getAttribute('value')] = trim($opt->textContent);
    }
    $this->assertArrayHasKey($candidate->uid, $options);
    $this->assertArrayNotHasKey($lead->uid, $options, 'Not yourself');
    $this->assertArrayNotHasKey($otherLead->uid, $options, 'Not someone already elected');
  }

  public function testNoCandidatesMeansNoVacation() {
    // A candidate role nobody holds.
    $this->configureElection($this->site->createRole());
    $lead = $this->site->createUser(array('lead'), array('can go on vacation'));
    $this->login($lead);
    $this->get('user/' . $lead->uid . '/edit');
    $this->assertText('There are no available temporary replacements for your position. You cannot go on vacation at this time.');
    $this->assertSame(0, $this->xpath()->query('//input[@value="Go on vacation"]')->length);
  }

  public function testNonElectedStaffNeedNoReplacement() {
    $this->configureElection('voter');
    $staff = $this->site->createUser(array('storyteller'), array('can go on vacation'));
    $this->login($staff);
    $this->get('user/' . $staff->uid . '/edit');
    $this->assertSame(1, $this->xpath()->query('//input[@value="Go on vacation"]')->length);
    $this->assertSame(0, $this->xpath()->query('//select[@name="temp_replacement"]')->length);
  }

  public function testReplacementIsPromotedWhileLeadIsAway() {
    list($lead, $candidate) = $this->electedLeadWithCandidate();
    $this->goOnVacation($lead, $candidate);
    $this->assertTrue($this->hasRole($candidate, 'lead'), 'Replacement promoted to lead');
  }

  public function testReplacementDemotedWhenVacationEnds() {
    list($lead, $candidate) = $this->electedLeadWithCandidate();
    $this->goOnVacation($lead, $candidate);

    $this->site->setVacationEnd($lead, strtotime(date('m/d/Y') . ' 12:00:00'));
    $this->site->resetDailyCron('vacation_cron');
    $this->runCron();

    $this->assertFalse($this->hasRole($candidate, 'lead'), 'Replacement demoted');
    $this->assertTrue($this->hasRole($lead, 'lead'), 'Lead has the role back');
  }

  /**
   * @group knownbug
   * '#default_value' => array_unshift(array_keys(...)) returns a count, not a
   * uid, and prints "Only variables should be passed by reference".
   */
  public function testReplacementPickerHasNoPhpErrors() {
    list($lead) = $this->electedLeadWithCandidate();
    $this->login($lead);
    $this->get('user/' . $lead->uid . '/edit');
    $this->assertSame(array(), $this->pageErrorsFrom('department.module'));
  }

}
