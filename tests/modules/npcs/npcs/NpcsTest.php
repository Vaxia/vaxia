<?php

namespace Vaxia\Tests\Npcs\Npcs;

use Vaxia\Tests\VaxiaTestCase;

/**
 * NPCs: when an NPC (or open NPC) is submitted, it's approved straight away if
 * every stat and skill is under the NPC stat cap (default 70). Otherwise it
 * stays submitted for "oversight": the submitter is warned and storytellers and
 * admins are mailed. Approved NPCs' numbers are locked on the edit form.
 *
 * @group npcs
 */
class NpcsTest extends VaxiaTestCase {

  private function owner() {
    return $this->site->createUser(array('player'), array('create character_sheet content', 'edit own character_sheet content'));
  }

  private function npc($owner, array $values = array()) {
    return $this->site->createCharacter($owner, $values + array(
      'title' => $this->site->uniqueName('Npc'),
      'field_is_npc' => 1,
      'field_strength' => 20,
    ));
  }

  /**
   * Owner moves the NPC to draft, then submits it; returns the submit messages.
   */
  private function submit($npc, $owner) {
    $this->transition($npc, 'draft', $owner);
    return $this->transition($npc, 'submitted', $owner);
  }

  private function oversightTitles($viewer = NULL) {
    return $this->inSession($viewer ?: $this->site->createUser(array('storyteller')), function () {
      $this->get('oversight/npcs');
      $this->assertStatus(200);
      return $this->pageText();
    });
  }

  // ---------------------------------------------------------------------------
  // Settings.
  // ---------------------------------------------------------------------------

  public function testSettingsPageNeedsPermission() {
    $this->login($this->site->createUser(array('player')));
    $this->get('admin/vaxia/npcs');
    $this->assertStatus(403);
  }

  public function testStatCapDefaultsTo70() {
    $this->login($this->site->createUser(array(), array('administer NPC handler')));
    $this->get('admin/vaxia/npcs');
    $this->assertSame('70', $this->xpath()->query('//input[@name="npcs_stat_cap"]')->item(0)->getAttribute('value'));
  }

  // ---------------------------------------------------------------------------
  // Submitting NPCs.
  // ---------------------------------------------------------------------------

  public function testNpcUnderTheCapIsApprovedOnSubmit() {
    $owner = $this->owner();
    $npc = $this->npc($owner, array('field_strength' => 69));

    $messages = $this->submit($npc, $owner);

    $this->assertContains('This character has all stat and skills under 70 points. Therefore the character does not require oversight and has been passed along directly to approval.', $messages);
    $this->assertSame('approved', $this->workflowState($npc));
  }

  public function testOpenNpcIsAlsoApprovedOnSubmit() {
    $owner = $this->owner();
    $npc = $this->npc($owner, array('field_is_npc' => 0, 'field_is_open_npc' => 1));
    $this->submit($npc, $owner);
    $this->assertSame('approved', $this->workflowState($npc));
  }

  public function testPlayerCharactersAreNotAutoApproved() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner, array('field_strength' => 20));
    $this->submit($character, $owner);
    $this->assertSame('submitted', $this->workflowState($character));
  }

  public function testStatAtTheCapNeedsOversight() {
    $owner = $this->owner();
    $npc = $this->npc($owner, array('field_strength' => 70));

    $messages = $this->submit($npc, $owner);

    $this->assertStringContainsString('This character falls under OVERSIGHT as it has a stat or skill at or over 70 points.', implode(' ', $messages));
    $this->assertSame('submitted', $this->workflowState($npc));
  }

  public function testOversightMailsStorytellersAndAdmins() {
    $storyteller = $this->site->createUser(array('storyteller'));
    $admin = $this->site->createUser(array('administrator'));
    $player = $this->site->createUser(array('player'));
    $owner = $this->owner();
    $npc = $this->npc($owner, array('field_charisma' => 90));

    $this->submit($npc, $owner);

    $to = $this->mailRecipients();
    $this->assertContains($storyteller->mail, $to);
    $this->assertContains($admin->mail, $to);
    $this->assertNotContains($player->mail, $to);
  }

  public function testSkillAtTheCapNeedsOversight() {
    $owner = $this->owner();
    $npc = $this->npc($owner);
    $this->site->addSkill($npc, 'Swordsmanship', 75);

    $this->submit($npc, $owner);

    $this->assertSame('submitted', $this->workflowState($npc));
  }

  public function testCapIsConfigurable() {
    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/vaxia/npcs', array('npcs_stat_cap' => '50'), 'Save configuration');
    });
    $owner = $this->owner();
    $npc = $this->npc($owner, array('field_strength' => 55));

    $this->submit($npc, $owner);

    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/vaxia/npcs', array('npcs_stat_cap' => '70'), 'Save configuration');
    });
    $this->assertSame('submitted', $this->workflowState($npc));
  }

  // ---------------------------------------------------------------------------
  // Lists.
  // ---------------------------------------------------------------------------

  public function testOversightListShowsOnlyNpcsOverTheCap() {
    $owner = $this->owner();
    $big = $this->npc($owner, array('field_strength' => 80));
    $small = $this->npc($owner, array('field_strength' => 20));
    $this->submit($big, $owner);
    $this->submit($small, $owner);

    $page = $this->oversightTitles();

    $this->assertStringContainsString($big->title, $page);
    $this->assertStringNotContainsString($small->title, $page);
  }

  public function testNpcListShowsNpcsNotPlayerCharacters() {
    $owner = $this->owner();
    $npc = $this->npc($owner);
    $pc = $this->site->createCharacter($owner, array('title' => $this->site->uniqueName('Pc')));
    $this->submit($npc, $owner);
    $this->submit($pc, $owner);

    $this->login($this->site->createUser(array('storyteller')));
    $this->get('character-sheets/npcs');
    $this->assertStatus(200);
    $this->assertText($npc->title);
    $this->assertNoText($pc->title);
  }

  public function testPlayersCannotSeeNpcLists() {
    $this->login($this->site->createUser(array('player')));
    foreach (array('character-sheets/npcs', 'oversight/npcs') as $path) {
      $this->get($path);
      $this->assertStatus(403, $path);
    }
  }

  // ---------------------------------------------------------------------------
  // Editing approved NPCs.
  // ---------------------------------------------------------------------------

  public function testApprovedNpcNumbersAreLocked() {
    $owner = $this->owner();
    $npc = $this->npc($owner);
    $this->submit($npc, $owner);

    $this->login($owner);
    $this->get('node/' . $npc->nid . '/edit');

    $this->assertStringContainsString('You are attempting to edit an NPC that has already been approved for play.', implode(' ', $this->messages()));
    $this->assertSame(1, $this->xpath()->query('//*[@name="' . $this->fieldName('field_strength[und][0][value]') . '" and @disabled]')->length, 'Stats locked');
    $this->assertSame(0, $this->xpath()->query('//textarea[starts-with(@name, "field_personality") and @disabled]')->length, 'Personality still editable');
  }

  public function testDraftNpcNumbersAreEditable() {
    $owner = $this->owner();
    $npc = $this->npc($owner);
    $this->login($owner);
    $this->get('node/' . $npc->nid . '/edit');
    $this->assertSame(0, $this->xpath()->query('//*[@name="' . $this->fieldName('field_strength[und][0][value]') . '" and @disabled]')->length);
  }

}
