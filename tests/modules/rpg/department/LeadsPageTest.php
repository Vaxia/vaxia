<?php

namespace Vaxia\Tests\Rpg\Department;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The Leads tab, players/leads (D7 view leads): active leads, newest account
 * first, 30 a page, with when they joined and last logged in ("time ago"),
 * their badges, a name filter, and the latest election underneath (for
 * players). Needs "access user profiles".
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group department
 * @group views_pages
 */
class LeadsPageTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
  }

  /**
   * Names listed, in page order.
   */
  private function names(array $query = array()) {
    $this->get('players/leads', $query);
    $this->assertStatus(200);
    $names = array();
    foreach ($this->xpath()->query('//table//tbody/tr/td[1]') as $td) {
      $names[] = trim($td->textContent);
    }
    return $names;
  }

  public function testNeedsAccessUserProfiles() {
    $player = $this->site->createUser(array('player'));
    $this->withoutPermission('access user profiles', array('anonymous user', 'authenticated user', 'player', 'new account'), function () use ($player) {
      $this->login($player);
      $this->get('players/leads');
      $this->assertStatus(403);
    });
    $this->login($this->site->createUser(array(), array('access user profiles')));
    $this->get('players/leads');
    $this->assertStatus(200);
    $this->assertText('Leads');
  }

  public function testActiveLeadsNewestFirst() {
    $prefix = $this->site->uniqueName('lead');
    $old = $this->site->createUser(array('lead', 'player'), array(), array('name' => $prefix . 'old'));
    $this->site->backdateAccountCreated($old, strtotime('-3 days'));
    $new = $this->site->createUser(array('lead', 'storyteller'), array(), array('name' => $prefix . 'new'));
    $this->site->createUser(array('lead'), array(), array('name' => $prefix . 'blocked', 'status' => 0));
    $this->site->createUser(array('player'), array(), array('name' => $prefix . 'player'));

    $this->login($this->site->createUser(array('administrator')));
    $this->assertSame(array($prefix . 'new', $prefix . 'old'), $this->names(array('name' => $prefix)));
    $this->assertRaw('href="/user/' . $new->uid . '"');
    $this->assertText('3 days ago');
    $row = trim($this->xpath()->query('//table//tbody/tr[1]/td[4]')->item(0)->textContent);
    $this->assertStringContainsString('lead', $row);
    $this->assertStringContainsString('storyteller', $row);
    $this->assertSame(array('Name', 'Created date', 'Last login', 'Badges'), array_map(function ($th) {
      return trim($th->textContent);
    }, iterator_to_array($this->xpath()->query('//table//thead//th'))));
  }

  public function testThirtyAPage() {
    $prefix = $this->site->uniqueName('many');
    for ($i = 1; $i <= 31; $i++) {
      $account = $this->site->createUser(array('lead'), array(), array('name' => $prefix . '_' . $i));
      $this->site->backdateAccountCreated($account, time() - 1000 + $i);
    }
    $this->login($this->site->createUser(array('administrator')));
    $names = $this->names(array('name' => $prefix));
    $this->assertCount(30, $names);
    $this->assertSame($prefix . '_31', $names[0]);
    $this->assertSame(array($prefix . '_1'), $this->names(array('name' => $prefix, 'page' => 1)));
  }

  public function testLatestElectionShowsForPlayers() {
    $title = $this->site->uniqueName('Election');
    $this->site->createProposal(array('player'), array('title' => $title, 'field_proposal_type' => 'election'));
    $this->login($this->site->createUser(array('player')));
    $this->get('players/leads');
    $this->assertText($title);
    $this->login($this->site->createUser(array('lead')));
    $this->get('players/leads');
    $this->assertNoText($title);
  }

}
