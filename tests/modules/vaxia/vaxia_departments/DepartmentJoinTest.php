<?php

namespace Vaxia\Tests\Vaxia\VaxiaDepartments;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Joining a department: players submit the "Join Social / Setting / System
 * Department" webforms (production nodes 12927, 12928, 12929). Those who took
 * the department's walkthrough in the last two weeks are welcomed and get its
 * role (knows-the-social / -setting / -system) for a year.
 *
 * On production these were Rules kept only in the database (not exported to
 * code), so the D7 test site doesn't have them.
 *
 * @group vaxia_departments
 */
class DepartmentJoinTest extends VaxiaTestCase {

  const WELCOME = 'Welcome to the department!';

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The join rules lived only in the production database; the D7 test site has none.');
    }
    foreach (array(12927 => 'Join Social Department', 12928 => 'Join Setting Department', 12929 => 'Join System Department') as $nid => $title) {
      $this->site->createWebformAt($nid, $title, array('player'), array(
        1 => array('cid' => 1, 'pid' => 0, 'form_key' => 'uid', 'name' => 'uid', 'type' => 'hidden', 'value' => '', 'extra' => array(), 'mandatory' => 0, 'weight' => 0),
      ));
    }
  }

  public function departments() {
    return array(
      'social' => array(12927, 'social', 'knows-the-social'),
      'setting' => array(12928, 'setting', 'knows-the-setting'),
      'system' => array(12929, 'system', 'knows-the-system'),
    );
  }

  private function join($player, $nid) {
    $this->login($player);
    $this->submitForm('node/' . $nid, array(), 'Submit');
    return implode(' ', $this->messages());
  }

  /**
   * @dataProvider departments
   */
  public function testRecentWalkthroughJoinsTheDepartment($nid, $key, $role) {
    $player = $this->site->createUser(array('player'));
    $this->site->setUserFields($player, array('field_took_' . $key . '_walkthrough' => date('Y-m-d', strtotime('-3 days'))));

    $messages = $this->join($player, $nid);

    $this->assertStringContainsString(self::WELCOME, $messages);
    $this->assertTrue($this->hasRole($player, $role));
  }

  public function testWithoutTheWalkthroughNothingHappens() {
    $player = $this->site->createUser(array('player'));
    $this->assertStringNotContainsString(self::WELCOME, $this->join($player, 12927));
    $this->assertFalse($this->hasRole($player, 'knows-the-social'));
  }

  public function testAnOldWalkthroughIsNotEnough() {
    $player = $this->site->createUser(array('player'));
    $this->site->setUserFields($player, array('field_took_setting_walkthrough' => date('Y-m-d', strtotime('-3 weeks'))));
    $this->join($player, 12928);
    $this->assertFalse($this->hasRole($player, 'knows-the-setting'));
  }

  public function testAnotherDepartmentsWalkthroughIsNotEnough() {
    $player = $this->site->createUser(array('player'));
    $this->site->setUserFields($player, array('field_took_social_walkthrough' => date('Y-m-d')));
    $this->join($player, 12929);
    $this->assertFalse($this->hasRole($player, 'knows-the-system'));
  }

  public function testMembersAreNotWelcomedAgain() {
    $player = $this->site->createUser(array('player', 'knows-the-system'));
    $this->site->setUserFields($player, array('field_took_system_walkthrough' => date('Y-m-d')));
    $this->assertStringNotContainsString(self::WELCOME, $this->join($player, 12929));
  }

}
