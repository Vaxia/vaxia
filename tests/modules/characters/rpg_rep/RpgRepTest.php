<?php

namespace Vaxia\Tests\Characters\RpgRep;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Reputation: groups (wiki articles flagged "provides reputation" in a realm)
 * track a character's standing. Staff add or remove reputation on the Sheet
 * tab; it gives a roll modifier capped at 25, shown on the sheet and on the
 * character's chat posts. It drifts one point toward zero on the 1st of each
 * month.
 *
 * @group rpg_rep
 */
class RpgRepTest extends VaxiaTestCase {

  private function group($realm = 'Vaxia') {
    return $this->site->createNode('wiki_page', array(
      'title' => $this->site->uniqueName('Guild'),
      'field_provides_reputation' => 1,
      'field_tag_realm' => $this->site->term('realm', $realm),
    ));
  }

  private function staff() {
    return $this->site->createUser(array('storyteller'), array('add xp to character sheet', 'give reputation xp', 'view reputation xp'));
  }

  private function character(array $values = array()) {
    $owner = $this->site->createUser(array('player'));
    return array($owner, $this->site->createCharacter($owner, $values));
  }

  private function giveRep($character, $group, $amount, $staff = NULL) {
    $this->login($staff ?: $this->staff());
    $this->get('node/' . $character->nid . '/character_sheet');
    $this->submitForm(NULL, array(
      'rpg_rep_amount' => (string) $amount,
      'rpg_rep_group' => $this->optionValue('rpg_rep_group', $group->title),
    ), 'Add reputation');
    return $this->messages();
  }

  private function sheetText($viewer, $character) {
    $this->login($viewer);
    $this->get('node/' . $character->nid . '/character_sheet');
    return $this->pageText();
  }

  // ---------------------------------------------------------------------------
  // Giving reputation.
  // ---------------------------------------------------------------------------

  public function testStaffChooseFromTheCharactersRealmGroups() {
    $ours = $this->group('Vaxia');
    $theirs = $this->group('Sirian');
    list(, $character) = $this->character();
    $this->login($this->staff());
    $this->get('node/' . $character->nid . '/character_sheet');
    $options = array();
    foreach ($this->xpath()->query('//select[@name="rpg_rep_group"]/option') as $opt) {
      $options[] = trim($opt->textContent);
    }
    $this->assertContains($ours->title, $options);
    $this->assertNotContains($theirs->title, $options);
  }

  public function testGivingReputationShowsOnTheSheet() {
    $group = $this->group();
    list(, $character) = $this->character();
    $this->assertContains('5 reputation for ' . $group->title . ' has been added to this sheet.', $this->giveRep($character, $group, 5));
    $this->assertStringContainsString($group->title . ': 5 rep for a modifier of 5 to rolls.', $this->sheetText($this->staff(), $character));
  }

  public function testModifierIsCappedAt25() {
    $group = $this->group();
    list(, $character) = $this->character();
    $this->giveRep($character, $group, 30);
    $this->assertStringContainsString($group->title . ': 30 rep for a modifier of 25 to rolls.', $this->sheetText($this->staff(), $character));
  }

  public function testNegativeAmountRemovesReputation() {
    $group = $this->group();
    list(, $character) = $this->character();
    $this->giveRep($character, $group, 5);
    $messages = $this->giveRep($character, $group, -8);
    $this->assertContains('-8 reputation for ' . $group->title . ' has been removed from this sheet.', $messages);
    $this->assertStringContainsString($group->title . ': -3 rep for a modifier of -3 to rolls.', $this->sheetText($this->staff(), $character));
  }

  public function testAmountMustBeANumber() {
    $group = $this->group();
    list(, $character) = $this->character();
    $this->giveRep($character, $group, 'heaps');
    $this->assertContains('Reputation given is not a numeric value', $this->messages('error'));
  }

  public function testNoReputationFormForOwnNpcOrStorytellerCharacters() {
    $this->group();
    $staff = $this->staff();
    $own = $this->site->createCharacter($staff);
    list(, $npc) = $this->character(array('field_is_npc' => 1));
    list(, $sh) = $this->character(array('field_is_storyteller' => 1));
    foreach (array('own' => $own, 'npc' => $npc, 'storyteller' => $sh) as $label => $character) {
      $this->sheetText($staff, $character);
      $this->assertSame(0, $this->xpath()->query('//input[@value="Add reputation"]')->length, "No form for $label character");
    }
  }

  // ---------------------------------------------------------------------------
  // Who sees reputation.
  // ---------------------------------------------------------------------------

  public function testOwnerNeedsViewOwnPermission() {
    $this->withoutPermission('view own reputation xp', array('anonymous user', 'authenticated user', 'player'), function () {
      $group = $this->group();
      $owner = $this->site->createUser(array('player'), array('add xp to own character sheet'));
      $character = $this->site->createCharacter($owner);
      $this->giveRep($character, $group, 4);
      $this->assertStringNotContainsString('Reputation details', $this->sheetText($owner, $character));

      $this->site->grantPermissions('player', array('view own reputation xp'));
      $this->assertStringContainsString($group->title . ': 4 rep', $this->sheetText($owner, $character));
    });
  }

  public function testReputationShowsOnChatPosts() {
    $group = $this->group();
    list($owner, $character) = $this->character();
    $this->giveRep($character, $group, 6);
    $room = $this->site->createRoom();
    $this->site->postComment($room, $owner, 'I bow to the guildmaster.', $character);

    $this->login($this->site->createUser(array('storyteller'), array('view reputation xp', 'view rpg chat', 'view vaxia ic chat')));
    $this->get('node/' . $room->nid);
    $this->assertSame(1, $this->xpath()->query('//*[self::fieldset or self::details][contains(@class, "rpg-reputation")]')->length);
    $this->assertText($group->title . ': 6 rep for a modifier of 6 to rolls.');

    $this->login($this->site->createUser(array('player'), array('view rpg chat', 'view vaxia ic chat')));
    $this->get('node/' . $room->nid);
    $this->assertSame(0, $this->xpath()->query('//*[self::fieldset or self::details][contains(@class, "rpg-reputation")]')->length, 'Hidden without permission');
  }

  // ---------------------------------------------------------------------------
  // Monthly drift.
  // ---------------------------------------------------------------------------

  public function testReputationDriftsTowardZeroOnTheFirst() {
    if (date('d') !== '01') {
      $this->markTestSkipped('The monthly drift only runs on the 1st of the month.');
    }
    $group = $this->group();
    list(, $character) = $this->character();
    $this->approveCharacter($character);
    $this->giveRep($character, $group, 5);
    $this->site->resetDailyCron('rpg_rep_sheet_cron');
    $this->runCron();
    $this->assertStringContainsString($group->title . ': 4 rep', $this->sheetText($this->staff(), $character));
  }

  // ---------------------------------------------------------------------------
  // Known bugs.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * _rpg_rep_add_submit() logs "...added to @title" with an undefined
   * $character: a PHP notice every time, and the log line has no title.
   */
  public function testGivingReputationHasNoPhpErrors() {
    list(, $character) = $this->character();
    $this->giveRep($character, $this->group(), 2);
    $this->assertSame(array(), $this->pageErrorsFrom('rpg_rep.module'));
  }

}
