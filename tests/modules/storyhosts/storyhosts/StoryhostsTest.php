<?php

namespace Vaxia\Tests\Storyhosts\Storyhosts;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Storyhosts: players with over 39 XP earned over time become "eligible for SH"
 * (and are told so) the next time their account is saved. Storyhost characters
 * are set as the author's storyhost character, put in the Storyhost realm,
 * titled "ASH" and approved on creation. Session report notes are only shown
 * (an attached file) to those allowed to see them. Leads and storyhosts get goal and skill lists.
 *
 * @group storyhosts
 */
class StoryhostsTest extends VaxiaTestCase {

  private function saveOwnProfile($account) {
    $this->login($account);
    $this->submitForm('user/' . $account->uid . '/edit', array(), 'Save');
  }

  private function eligibilityMailsTo($account) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($account) {
      return $m['to'] === $account->mail && $m['subject'] === '[Vaxia] You are eligible to become a Storyhost';
    }));
  }

  // ---------------------------------------------------------------------------
  // Eligibility.
  // ---------------------------------------------------------------------------

  public function testPlayerOver39XpBecomesEligible() {
    $player = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 40));
    $this->saveOwnProfile($player);
    $this->assertTrue($this->hasRole($player, 'eligible for SH'));
    $this->assertCount(1, $this->eligibilityMailsTo($player));
  }

  public function testPlayerAt39XpIsNotEligible() {
    $player = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 39));
    $this->saveOwnProfile($player);
    $this->assertFalse($this->hasRole($player, 'eligible for SH'));
    $this->assertSame(array(), $this->eligibilityMailsTo($player));
  }

  public function testAlreadyEligiblePlayersAreNotToldAgain() {
    $player = $this->site->createUser(array('player', 'eligible for SH'), array(), array('field_xp_over_time' => 80));
    $this->saveOwnProfile($player);
    $this->assertSame(array(), $this->eligibilityMailsTo($player));
  }

  public function testNonPlayersAreNotMadeEligible() {
    $account = $this->site->createUser(array(), array(), array('field_xp_over_time' => 80));
    $this->saveOwnProfile($account);
    $this->assertFalse($this->hasRole($account, 'eligible for SH'));
  }

  // ---------------------------------------------------------------------------
  // Storyhost characters.
  // ---------------------------------------------------------------------------

  public function testStoryhostCharacterIsSetUpAndApproved() {
    $sh = $this->site->createUser(array('storyteller'));
    $character = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Sh'), 'field_is_storyteller' => 1));

    $this->assertSame('approved', $this->workflowState($character));
    $page = $this->nodeText($character);
    $this->assertStringContainsString('Storyhost', $page, 'Realm');
    $this->assertStringContainsString('ASH', $page, 'Title');
    $this->assertStringContainsString($character->title, $this->profileText($sh), 'Set as the storyhost character');
  }

  public function testPlayerCharactersAreNotAutoApproved() {
    $character = $this->site->createCharacter($this->site->createUser(array('player')));
    $this->assertNotSame('approved', $this->workflowState($character));
  }

  // ---------------------------------------------------------------------------
  // Session reports.
  // ---------------------------------------------------------------------------

  private function sessionReport() {
    return $this->site->createNode('session_report', array(
      'uid' => $this->site->createUser(array('storyteller'))->uid,
      'title' => $this->site->uniqueName('Session'),
      'field_additional_notes' => $this->site->createFile('secret-staff-notes.txt'),
    ));
  }

  public function testSessionNotesShownToThoseAllowed() {
    $report = $this->sessionReport();
    $this->login($this->site->createUser(array('player'), array('view session notes')));
    $this->get('node/' . $report->nid);
    $this->assertSame(1, $this->xpath()->query('//a[contains(@href, "secret-staff-notes")]')->length);
  }

  public function testSessionNotesHiddenFromOthers() {
    $report = $this->sessionReport();
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $report->nid);
    $this->assertStatus(200);
    $this->assertSame(0, $this->xpath()->query('//a[contains(@href, "secret-staff-notes")]')->length);
  }

  // ---------------------------------------------------------------------------
  // Lists.
  // ---------------------------------------------------------------------------

  public function testLeadsAndStorytellersSeeCharacterGoals() {
    foreach (array('lead', 'storyteller') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('character-goals');
      $this->assertStatus(200, $role);
    }
  }

  public function testStaffSeeSkillInventory() {
    foreach (array('lead', 'storyteller', 'administrator') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('skill-inventory');
      $this->assertStatus(200, $role);
    }
  }

  public function testPlayersCannotSeeStaffLists() {
    $this->login($this->site->createUser(array('player')));
    foreach (array('character-goals', 'skill-inventory') as $path) {
      $this->get($path);
      $this->assertStatus(403, $path);
    }
  }

}
