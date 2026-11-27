<?php

namespace Vaxia\Tests\Players\OnlineTime;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Online time (players/online-time): for storytellers and administrators, a
 * table of active accounts, most recent access first, 35 a page: name,
 * active, online or offline (a session within the last 30 minutes), and how
 * long ago the account was created, last logged in and last accessed the
 * site. Filters: online (any/yes/no) and player names.
 *
 * D11 only: the D7 test site doesn't have the player_online_time view.
 *
 * @group online_time
 */
class OnlineTimeTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The online time page was ported to the D11 site only.');
    }
  }

  /**
   * Table rows (cell texts) shown to a storyteller, keyed by account name.
   */
  private function rows(array $query = array()) {
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('players/online-time', $query);
    $this->assertStatus(200);
    $rows = array();
    $xpath = $this->xpath();
    foreach ($xpath->query('//table//tbody/tr[not(td[@colspan])]') as $tr) {
      $cells = array();
      foreach ($xpath->query('td', $tr) as $td) {
        $cells[] = trim($td->textContent);
      }
      $rows[$cells[0]] = $cells;
    }
    return $rows;
  }

  public function testAccess() {
    foreach (array('storyteller', 'administrator') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('players/online-time');
      $this->assertStatus(200);
      $this->assertText('Online time');
    }
    foreach (array('player', 'lead', 'storyteller_assistant') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('players/online-time');
      $this->assertStatus(403);
    }
    $this->anonymous();
    $this->get('players/online-time');
    $this->assertStatus(403);
  }

  public function testColumnsAndOnlineStatus() {
    $online = $this->site->createUser(array('player'));
    $this->login($online);
    $offline = $this->site->createUser(array('player'));
    $rows = $this->rows(array('uid' => $online->name . ', ' . $offline->name));
    foreach (array('Name', 'Active', 'Is online', 'Created date', 'Last login', 'Last access') as $label) {
      $this->assertNotNull($this->xpath()->query('//table//th[normalize-space()="' . $label . '"]')->item(0), "'$label' column");
    }
    $this->assertSame(array($online->name, $offline->name), array_keys($rows), 'Most recent access first');
    $this->assertSame('Yes', $rows[$online->name][1]);
    $this->assertSame('Online', $rows[$online->name][2]);
    $this->assertMatchesRegularExpression('/^\d+ sec/', $rows[$online->name][4], 'Last login, time ago');
    $this->assertSame('Offline', $rows[$offline->name][2]);
    $this->assertSame('', $rows[$offline->name][4], 'Never logged in');
    $this->assertSame('', $rows[$offline->name][5], 'Never accessed');
    $this->assertNotNull($this->xpath()->query('//table//a[contains(@href, "/user/' . $online->uid . '")]')->item(0), 'Name links to the account');
  }

  public function testIsATabBesideThePlayersList() {
    $this->rows();
    $this->assertNotNull($this->xpath()->query('//a[contains(@href, "/players/all") and normalize-space()="Players"]')->item(0), 'Players tab');
    $this->assertNotNull($this->xpath()->query('//a[contains(@href, "/players/online-time") and contains(normalize-space(), "Online")]')->item(0), 'Online tab');
  }

  public function testBlockedAccountsAreLeftOut() {
    $blocked = $this->site->createUser(array('player'), array(), array('status' => 0));
    $this->assertSame(array(), $this->rows(array('uid' => $blocked->name)));
  }

  public function testOnlineFilter() {
    $online = $this->site->createUser(array('player'));
    $this->login($online);
    $offline = $this->site->createUser(array('player'));
    $names = $online->name . ',' . $offline->name;
    $this->assertSame(array($online->name), array_keys($this->rows(array('uid' => $names, 'is_online' => '1'))));
    $this->assertSame(array($offline->name), array_keys($this->rows(array('uid' => $names, 'is_online' => '0'))));
  }

  public function testThirtyFivePerPage() {
    for ($i = 0; $i < 36; $i++) {
      $this->site->createUser(array('player'));
    }
    $this->assertCount(35, $this->rows());
    $this->assertNotNull($this->xpath()->query('//nav[contains(@class, "pager")]')->item(0), 'Pager');
  }

}
