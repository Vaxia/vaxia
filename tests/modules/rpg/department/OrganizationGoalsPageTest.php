<?php

namespace Vaxia\Tests\Rpg\Department;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Organization goals, setting/organization-goals (D7 view organization_goals,
 * an editable table): for administrators and leads, every published goal by
 * organization then priority, edited in place and saved with one button, and
 * a form underneath to add one.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group department
 * @group views_pages
 */
class OrganizationGoalsPageTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->deleteNodesOfType('organization_goal');
  }

  private function goal($method, $organization, $priority, array $values = array()) {
    $target = $this->site->createNode('wiki_page', array('title' => $this->site->uniqueName('Target')));
    return $this->site->createNode('organization_goal', $values + array(
      'title' => $method,
      'field_organization' => $organization->nid,
      'field_priority' => (string) $priority,
      'field_first_stat' => 'members',
      'field_second_stat' => 'soft',
      'field_org_goal' => 'stat_up',
      'field_target' => $target->nid,
      'field_target_government' => $target->nid,
      'field_target_stat' => 'hard',
      'field_current_pp' => 7,
    ));
  }

  /**
   * IC methods (the title boxes) in page order.
   */
  private function methods() {
    $methods = array();
    foreach ($this->xpath()->query('//table[contains(@class, "organization-goals")]//tbody/tr//input[contains(@name, "[title]")]') as $input) {
      $methods[] = $input->getAttribute('value');
    }
    return $methods;
  }

  public function testAdministratorsAndLeadsOnly() {
    foreach (array('administrator', 'lead') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('setting/organization-goals');
      $this->assertStatus(200);
      $this->assertText('Organization Goals');
      $this->assertText('Add a new Organization');
    }
    foreach (array('player', 'storyteller') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('setting/organization-goals');
      $this->assertStatus(403);
    }
  }

  public function testGoalsByOrganizationThenPriority() {
    $first = $this->site->createNode('wiki_page', array('title' => $this->site->uniqueName('Guild')));
    $second = $this->site->createNode('wiki_page', array('title' => $this->site->uniqueName('Order')));
    $this->goal('second org', $second, 1);
    $this->goal('first org low', $first, 3);
    $this->goal('first org high', $first, 1);
    $this->goal('unpublished', $first, 1, array('status' => 0));

    $this->login($this->site->createUser(array('lead')));
    $this->get('setting/organization-goals');
    $this->assertSame(array('first org high', 'first org low', 'second org'), $this->methods());
    $this->assertSame('7', trim($this->xpath()->query('//table[contains(@class, "organization-goals")]//tbody/tr[1]/td[10]')->item(0)->textContent));
    $headers = array_map(function ($th) {
      return trim($th->textContent);
    }, iterator_to_array($this->xpath()->query('//table[contains(@class, "organization-goals")]//thead//th')));
    $this->assertSame(array('Organization', 'Priority', 'Using Stats', '', 'Goal', 'Target Organization', 'Target Government', '', 'IC Methods', 'Current PP', 'Actions'), $headers);
  }

  public function testRowsAreEditedInPlace() {
    $org = $this->site->createNode('wiki_page', array('title' => $this->site->uniqueName('Guild')));
    $goal = $this->goal('bribe officials', $org, 2);
    $this->login($this->site->createUser(array('administrator')));
    $this->submitForm('setting/organization-goals', array(
      'goals[' . $goal->nid . '][title][0][value]' => 'blackmail officials',
      'goals[' . $goal->nid . '][field_priority]' => '5',
    ), 'Save');
    $this->assertText('Saved blackmail officials.');
    $this->get('setting/organization-goals');
    $this->assertSame(array('blackmail officials'), $this->methods());
    $this->assertSame('5', $this->xpath()->query('//select[@name="goals[' . $goal->nid . '][field_priority]"]/option[@selected]')->item(0)->getAttribute('value'));
  }

  public function testDeleteLink() {
    $org = $this->site->createNode('wiki_page', array('title' => $this->site->uniqueName('Guild')));
    $goal = $this->goal('spy', $org, 1);
    $this->login($this->site->createUser(array('administrator')));
    $this->get('setting/organization-goals');
    $this->assertRaw('/node/' . $goal->nid . '/delete?destination=/setting/organization-goals');
  }

}
