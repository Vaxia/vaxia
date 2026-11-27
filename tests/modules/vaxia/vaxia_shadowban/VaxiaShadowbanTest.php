<?php

namespace Vaxia\Tests\Vaxia\VaxiaShadowban;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia shadowban: ships the "shadowbanned user" role and rules (all shipped
 * disabled) that would auto-approve a shadowbanned player's characters, items
 * and scenes so they look accepted. With the rules off, a shadowbanned
 * player's submissions go through normal evaluation.
 *
 * @group vaxia_shadowban
 */
class VaxiaShadowbanTest extends VaxiaTestCase {

  public function testRoleExists() {
    $this->login($this->site->createUser(array(), array('administer permissions', 'administer users')));
    $this->get('admin/people/permissions/roles');
    $this->assertText('shadowbanned user');
  }

  public function testShadowbannedCharactersAreNotAutoApproved() {
    $player = $this->site->createUser(array('player', 'shadowbanned user'));
    $character = $this->site->createCharacter($player);
    $this->transition($character, 'draft', $player);
    $this->transition($character, 'submitted', $player);
    $this->assertSame('submitted', $this->workflowState($character));
  }

}
