<?php

namespace Vaxia\Tests\Items\Items;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Items: character equipment, evaluated through "Item evaluation" (draft ->
 * submitted -> approved). Approved items show on their character's sheet;
 * editing an approved item sends it back for evaluation; an item's silver value
 * can be calculated from its bonuses.
 *
 * @group items
 */
class ItemsTest extends VaxiaTestCase {

  private function owner() {
    return $this->site->createUser(array('player'), array('create items content', 'edit own items content'));
  }

  private function item($owner, $character = NULL, array $values = array()) {
    return $this->site->createNode('items', $values + array(
      'uid' => $owner->uid,
      'title' => $this->site->uniqueName('Sword'),
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ) + ($character ? array('field_character_owner' => $character->nid) : array()));
  }

  private function submitted($item) {
    $this->transition($item, 'draft');
    $this->transition($item, 'submitted');
  }

  private function approved($item) {
    $this->submitted($item);
    $this->transition($item, 'approved');
  }

  private function workflowChoicesFor($account, $item) {
    $this->login($account);
    $this->get('node/' . $item->nid . '/workflow');
    return $this->workflowChoices();
  }

  // ---------------------------------------------------------------------------
  // Ownership.
  // ---------------------------------------------------------------------------

  public function testItemShowsOnItsCharacter() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner);
    $item = $this->item($owner, $character);
    $this->approved($item);
    $this->assertStringContainsString($item->title, $this->nodeText($character));
  }

  public function testUnapprovedItemsAreHiddenOnTheSheet() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner);
    $draft = $this->item($owner, $character);
    $this->transition($draft, 'draft');
    $approved = $this->item($owner, $character);
    $this->approved($approved);
    $text = $this->nodeText($character);
    $this->assertStringContainsString($approved->title, $text);
    $this->assertStringNotContainsString($draft->title, $text);
  }

  public function testRemovedFromGameLeavesTheCharacter() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner);
    $item = $this->item($owner, $character);
    $this->approved($item);
    $this->transition($item, 'removed from game');
    $this->assertStringNotContainsString($item->title, $this->nodeText($character));
    $this->login($this->superuser());
    $this->get('node/' . $item->nid . '/edit');
    $this->assertSame('', $this->xpath()->query('//input[starts-with(@name, "field_character_owner")]')->item(0)->getAttribute('value'), 'Owner cleared');
  }

  // ---------------------------------------------------------------------------
  // Evaluation.
  // ---------------------------------------------------------------------------

  public function testSubmittingNotifiesEvaluators() {
    $owner = $this->owner();
    $item = $this->item($owner, $this->site->createCharacter($owner));
    $this->transition($item, 'draft', $owner);
    $messages = $this->transition($item, 'submitted', $owner);
    $this->assertContains('Your item has been submitted, and email sent to the item evaluators.', $messages);
    $this->assertNotEmpty($this->mailRecipients(), 'Evaluators mailed');
  }

  public function testApprovalNotifiesTheOwner() {
    $owner = $this->owner();
    $item = $this->item($owner, $this->site->createCharacter($owner));
    $this->submitted($item);
    $this->transition($item, 'approved', $this->site->createUser(array('storyteller')));
    $this->assertContains($owner->mail, $this->mailRecipients());
  }

  public function testStorytellersApproveSubmittedItems() {
    $owner = $this->owner();
    $item = $this->item($owner, $this->site->createCharacter($owner));
    $this->submitted($item);
    $this->assertContains('approved', $this->workflowChoicesFor($this->site->createUser(array('storyteller')), $item));
  }

  public function testEditingAnApprovedItemSendsItBackToEvaluation() {
    $owner = $this->owner();
    $item = $this->item($owner, $this->site->createCharacter($owner));
    $this->approved($item);
    $this->site->ageWorkflowHistory($item);
    $this->site->saveNode($item, array('title' => $item->title . ' +1'));
    $this->assertSame('submitted', $this->workflowState($item));
  }

  public function testEditFormWarnsAboutApprovedItems() {
    $owner = $this->owner();
    $item = $this->item($owner, $this->site->createCharacter($owner));
    $this->approved($item);
    $this->login($owner);
    $this->get('node/' . $item->nid . '/edit');
    $this->assertStringContainsString('You are editing an Item that has already been approved for play.', implode(' ', $this->messages('warning')));
  }

  public function testOwnersCannotApproveTheirOwnItems() {
    $storyteller = $this->site->createUser(array('storyteller'));
    $item = $this->item($storyteller, $this->site->createCharacter($storyteller));
    $this->submitted($item);
    $this->assertNotContains('approved', $this->workflowChoicesFor($storyteller, $item));
    $this->assertStringContainsString('You own this node, you cannot set it to approved.', implode(' ', $this->messages()));
  }

  /**
   * @group knownbug
   * items.module also means to stop the owner of the item's CHARACTER from
   * approving it, but its workflow-tab guard reads undefined $node and $user and
   * never runs. (The general "you own this node" guard elsewhere only checks
   * the item's own author.)
   */
  public function testCharacterOwnersCannotApproveTheirCharactersItems() {
    $storyteller = $this->site->createUser(array('storyteller'));
    $item = $this->item($this->owner(), $this->site->createCharacter($storyteller));
    $this->submitted($item);
    $this->assertNotContains('approved', $this->workflowChoicesFor($storyteller, $item));
  }

  // ---------------------------------------------------------------------------
  // Value.
  // ---------------------------------------------------------------------------

  private function valueOf($values) {
    $owner = $this->owner();
    $item = $this->item($owner, NULL, $values + array('field_calc_value' => 1));
    $this->login($this->superuser());
    $this->get('node/' . $item->nid . '/edit');
    return $this->xpath()->query('//input[@name="' . $this->fieldName('field_value[und][0][value]') . '"]')->item(0)->getAttribute('value');
  }

  public function testValueFromStatBonuses() {
    // 3 points: 50 silver each.
    $this->assertSame('150', $this->valueOf(array('field_item_strength' => 3)));
  }

  public function testBonusPriceDoublesEveryFivePoints() {
    // 6 points: floor((6-1)/5) = 1 doubling, so 100 each.
    $this->assertSame('600', $this->valueOf(array('field_item_strength' => 6)));
  }

  public function testSkillBonusesCountThreeQuarters() {
    // 4 skill points * 0.75 = 3 points: 150.
    $this->assertSame('150', $this->valueOf(array('field_item_skill' => 4)));
  }

  public function testOverlapsDefenseAndPowers() {
    $this->assertSame('4', $this->valueOf(array('field_item_over_strength' => 2)), '2 silver per overlap');
    $this->assertSame('100', $this->valueOf(array('field_armor_absorbs' => 2)), 'Armor 50 per point');
    $this->assertSame('50', $this->valueOf(array('field_armor_absorbs' => 2, 'field_absorb_type' => 'shield')), 'Shield 25 per point');
    $this->assertSame('10', $this->valueOf(array('field_build_points' => 5)), 'Companion 2 per build point');
    $this->assertSame('1500', $this->valueOf(array('field_pp_created' => 2, 'field_item_use_day' => 'at_will')), 'At-will power 750 per PP');
    $this->assertSame('500', $this->valueOf(array('field_pp_created' => 2, 'field_item_use_day' => 'per_day')), 'Daily power 250 per PP');
    $this->assertSame('30', $this->valueOf(array('field_pp_created' => 2, 'field_item_use_day' => 'once', 'field_item_uses' => 3)), 'Single-use 5 per PP per charge');
  }

  public function testTheEvaluationExamItemAnswers() {
    // The storyhost course's evaluation exam asks for these worths
    // (scripts/starter-content/sh-course.yml; user, 2026-11-26).
    $this->assertSame('750', $this->valueOf(array('field_item_skill' => 10)), '+10 weapon skill');
    $this->assertSame('575', $this->valueOf(array('field_pp_created' => 115, 'field_item_use_day' => 'once', 'field_item_uses' => 1, 'field_item_difficulty' => 25)), 'Paralysis poison, 115 PP');
    $this->assertSame('200', $this->valueOf(array('field_build_points' => 100)), 'Drone, 100 build points');
    $this->assertSame('392', $this->valueOf(array('field_pp_created' => 49, 'field_item_use_day' => 'once', 'field_item_uses' => 1, 'field_item_difficulty' => 40)), 'Poison, difficulty 40, 49 PP');
  }

  public function testDeclaredValueIsKept() {
    $owner = $this->owner();
    $item = $this->item($owner, NULL, array('field_calc_value' => 1, 'field_item_strength' => 3, 'field_value' => '999'));
    $this->login($this->superuser());
    $this->get('node/' . $item->nid . '/edit');
    $this->assertSame('999', $this->xpath()->query('//input[@name="' . $this->fieldName('field_declared_value[und][0][value]') . '"]')->item(0)->getAttribute('value'));
  }

  /**
   * @group knownbug
   * The intelligence overlap field is spelled field_item_over_intellegence, but
   * the value code reads field_item_over_intelligence, so it never counts.
   */
  public function testIntelligenceOverlapCounts() {
    $this->assertSame('4', $this->valueOf(array('field_item_over_intellegence' => 2)));
  }

  // ---------------------------------------------------------------------------
  // Settings page.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * _items_admin_settings() has "'#collapsed' => TRUE,s" twice: an undefined
   * constant becomes a string form element and the page dies with HTTP 500
   * ("Unsupported operand types"). On PHP 8 the constant itself is fatal.
   */
  public function testSettingsPageLoads() {
    $this->login($this->site->createUser(array(), array('administer item settings')));
    $this->get('admin/vaxia/items');
    $this->assertStatus(200);
  }

}
