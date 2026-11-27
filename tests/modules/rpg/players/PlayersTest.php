<?php

namespace Vaxia\Tests\Rpg\Players;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Players feature: the "player" role and the staff-only player list at /players/all.
 *
 * @group players
 */
class PlayersTest extends VaxiaTestCase {

  public function testPlayerRoleExists() {
    $this->login($this->superuser());
    $this->get('admin/people/permissions/roles');
    $this->assertText('player');
  }

  public function staffRoles() {
    return array(
      'administrator' => array('administrator'),
      'lead' => array('lead'),
      'storyteller' => array('storyteller'),
      'storyteller-assistant' => array('storyteller-assistant'),
      'storyteller-candidate' => array('storyteller-candidate'),
    );
  }

  /**
   * @dataProvider staffRoles
   */
  public function testStaffCanSeePlayerList($role) {
    $this->login($this->site->createUser(array($role)));
    $this->get('players/all');
    $this->assertStatus(200);
    $this->assertText('Players');
  }

  public function nonStaffRoles() {
    return array(
      'player' => array(array('player')),
      'authenticated only' => array(array()),
      'storyteller-newbie' => array(array('storyteller-newbie')),
    );
  }

  /**
   * @dataProvider nonStaffRoles
   */
  public function testOthersCannotSeePlayerList(array $roles) {
    $this->login($this->site->createUser($roles));
    $this->get('players/all');
    $this->assertStatus(403);
  }

  public function testAnonymousCannotSeePlayerList() {
    $this->get('players/all');
    $this->assertStatus(403);
  }

  public function testListsOnlyActivePlayers() {
    $active = $this->site->createUser(array('player'));
    $blocked = $this->site->createUser(array('player'), array(), array('status' => 0));
    $notPlayer = $this->site->createUser(array('voter'));

    $this->login($this->site->createUser(array('lead')));
    $this->get('players/all');

    $this->assertText($active->name);
    $this->assertNoText($blocked->name);
    $this->assertNoText($notPlayer->name);
  }

  public function testNewestPlayersFirst() {
    $older = $this->site->createUser(array('player'), array(), array('created' => time() - 86400));
    $newer = $this->site->createUser(array('player'), array(), array('created' => time()));

    $this->login($this->site->createUser(array('lead')));
    $this->get('players/all');

    $text = $this->pageText();
    $this->assertLessThan(strpos($text, $older->name), strpos($text, $newer->name));
  }

  public function testNameFilterMatchesPartOfName() {
    $match = $this->site->createUser(array('player'), array(), array('name' => $this->site->uniqueName('zebra')));
    $other = $this->site->createUser(array('player'), array(), array('name' => $this->site->uniqueName('yak')));

    $this->login($this->site->createUser(array('lead')));
    $this->get('players/all', array('name' => 'zebra', 'name_op' => 'contains'));

    $this->assertText($match->name);
    $this->assertNoText($other->name);
  }

  public function testShowsColumns() {
    $this->site->createUser(array('player'));
    $this->login($this->site->createUser(array('lead')));
    $this->get('players/all');
    $headers = array();
    foreach ($this->xpath()->query('//table//th') as $th) {
      $headers[] = trim($th->textContent);
    }
    $this->assertSame(array('Name', 'Created date', 'Last login', 'Badges'), $headers);
  }

  public function testListIsCachedPerRoleSet() {
    $lead = $this->site->createUser(array('lead'));
    $this->login($lead);
    $this->get('players/all');
    $late = $this->site->createUser(array('player'));

    $this->get('players/all');
    $this->assertNoText($late->name, 'Cached list does not show the new player yet');

    $this->site->expireViewsCache();
    $this->get('players/all');
    $this->assertText($late->name);
  }

  public function testPagesThirtyPerPage() {
    for ($i = 0; $i < 31; $i++) {
      $this->site->createUser(array('player'));
    }
    // The list is cached (see testListIsCachedPerRoleSet).
    $this->site->expireViewsCache();
    $this->login($this->site->createUser(array('lead')));
    $this->get('players/all');
    $this->assertSame(30, $this->xpath()->query('//table/tbody/tr')->length);
    $this->assertGreaterThan(0, $this->xpath()->query('//ul[contains(@class, "pager")]')->length);
  }

}
