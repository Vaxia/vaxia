<?php

namespace Vaxia\Tests\Harness;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Smoke tests for the setup driver (Site.php): what it arranges shows up over
 * HTTP the same way on every target (D7 and the D11 port).
 */
class SiteDriverTest extends VaxiaTestCase {

  public function testRoomShowsAPostedComment() {
    $room = $this->site->createRoom(array('title' => 'Harness tavern'));
    $player = $this->site->createUser(array('player'));
    $comment = $this->site->postComment($room, $player, 'Hello from the harness');
    $this->assertGreaterThan(0, $comment->cid);
    $text = $this->nodeText($room);
    $this->assertStringContainsString('Harness tavern', $text);
    $this->assertStringContainsString('Hello from the harness', $text);
  }

  public function testCharacterShowsItsSkill() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player, array('title' => 'Harness hero'));
    $skillId = $this->site->addSkill($character, 'Harness juggling', 3, array('craft'));
    $this->assertGreaterThan(0, $skillId);
    $this->assertStringContainsString('Harness juggling', $this->nodeText($character));
  }

  public function testTermsAreCreatedOnceAndReused() {
    $first = $this->site->term('realm', 'Harness realm');
    $this->assertSame($first, $this->site->term('realm', 'Harness realm'));
    $this->assertNotSame($first, $this->site->term('realm', 'Another harness realm'));
  }

  public function testUserFieldsAndRolesCanChangeLater() {
    $account = $this->site->createUser();
    // Only the role this test touches: on D7 a rule also gives every new
    // account "new account" (KNOWN-BUGS.md).
    $this->site->addRole($account, 'voter');
    $this->assertTrue($this->hasRole($account, 'voter'));
    $this->site->removeRole($account, 'voter');
    $this->assertFalse($this->hasRole($account, 'voter'));
  }

  public function testPermissionsCanBeGrantedAndRevoked() {
    $role = $this->site->createRole();
    $this->site->grantPermissions($role, array('access content'));
    $this->assertTrue($this->site->roleHas($role, 'access content'));
    $this->assertTrue($this->roleHasPermission($role, 'access content'));
    $this->site->revokePermissions($role, array('access content'));
    $this->assertFalse($this->site->roleHas($role, 'access content'));
  }

}
