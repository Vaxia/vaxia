<?php

namespace Vaxia\Tests\Vaxia\VaxiaCharacters;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia characters: how character sheets show and edit.
 *
 * - Sheets whose XP was transferred are closed to all but staff.
 * - Owners of new characters get min-max warnings and pick stats from lists
 *   (5-40); "Other" races and conditions need 40 XP over time. Submitted
 *   sheets are locked to the owner.
 * - Approved characters under 20 XP note that evaluators can help revise.
 * - Others see only the public fields; SH notes show to owner and staff,
 *   private SH notes to staff only (not the owner).
 *
 * @group vaxia_characters
 */
class VaxiaCharactersTest extends VaxiaTestCase {

  private function owner(array $values = array()) {
    return $this->site->createUser(array('player'), array('create character_sheet content', 'edit own character_sheet content'), $values);
  }

  private function viewSheet($account, $character) {
    $this->login($account);
    $this->get('node/' . $character->nid);
    // The sheet itself, not blocks around it (the dice ruler lists stats).
    $article = $this->xpath()->query('//article[contains(concat(" ", normalize-space(@class), " "), " node ")]')->item(0);
    $text = $article ? trim(preg_replace('/[\s\x{00A0}]+/u', ' ', $article->textContent)) : $this->pageText();
    return array($text, implode(' ', $this->messages()));
  }

  // ---------------------------------------------------------------------------
  // Transferred sheets.
  // ---------------------------------------------------------------------------

  public function testTransferredSheetsAreClosedToPlayers() {
    $character = $this->site->createCharacter($this->owner());
    $this->approveCharacter($character);
    $this->transition($character, 'xp_transferred');
    list(, $messages) = $this->viewSheet($this->site->createUser(array('player')), $character);
    $this->assertStringContainsString('You are not allowed to view this sheet. It has been removed from play.', $messages);
    $this->assertDoesNotMatchRegularExpression('#/node/' . $character->nid . '$#', $this->url);
  }

  public function testStaffSeeTransferredSheetsWithAWarning() {
    $character = $this->site->createCharacter($this->owner());
    $this->approveCharacter($character);
    $this->transition($character, 'xp_transferred');
    list(, $messages) = $this->viewSheet($this->site->createUser(array(), array('view inactive sheets')), $character);
    $this->assertStringContainsString('This character is currently listed as transferred XP.', $messages);
  }

  // ---------------------------------------------------------------------------
  // New characters.
  // ---------------------------------------------------------------------------

  public function testOwnersSeeMinMaxWarnings() {
    $owner = $this->owner();
    // All stats at zero is flagged.
    $character = $this->site->createCharacter($owner, array(
      'field_life' => 0, 'field_endurance' => 0, 'field_strength' => 0, 'field_dexterity' => 0,
      'field_intelligence' => 0, 'field_spirituality' => 0, 'field_charisma' => 0,
    ));
    $this->transition($character, 'draft', $owner);
    list(, $messages) = $this->viewSheet($owner, $character);
    $this->assertStringContainsString('likely to be reviewed by our evaluation staff as a potential min-maxed build', $messages);
  }

  public function testNewCharacterStatsAreChosenFromARange() {
    $owner = $this->owner(array('field_xp_over_time' => 0));
    $character = $this->site->createCharacter($owner);
    $this->login($owner);
    $this->get('node/' . $character->nid . '/edit');
    $this->assertContains('This character is currently not playable.', $this->messages());
    $values = array();
    foreach ($this->xpath()->query('//select[@name="' . $this->fieldName('field_strength[und][0][value]') . '"]/option') as $o) {
      $values[] = (int) $o->getAttribute('value');
    }
    $this->assertSame(range(5, 40), $values);
  }

  public function testOtherRaceNeedsFortyXpOverTime() {
    $novice = $this->owner(array('field_xp_over_time' => 39));
    $veteran = $this->owner(array('field_xp_over_time' => 40));
    foreach (array(array($novice, 0), array($veteran, 1)) as $case) {
      list($owner, $expected) = $case;
      $character = $this->site->createCharacter($owner);
      $this->login($owner);
      $this->get('node/' . $character->nid . '/edit');
      $this->assertSame($expected, $this->xpath()->query('//select[@name="' . $this->fieldName('field_fantasy_species[und]') . '"]/option[@value="other"]')->length, $owner->name);
    }
  }

  public function testSubmittedSheetsAreLockedToTheOwner() {
    $owner = $this->owner(array('field_xp_over_time' => 0));
    $character = $this->site->createCharacter($owner);
    $this->transition($character, 'draft', $owner);
    $this->transition($character, 'submitted', $owner);
    $this->login($owner);
    $this->get('node/' . $character->nid . '/edit');
    $this->assertContains('This character is currently in submit and cannot be edited by the owner.', $this->messages());
    $this->assertSame(1, $this->xpath()->query('//input[(@name="title" or @name="title[0][value]") and @disabled]')->length);
  }

  /**
   * @group knownbug
   * The edit form reads the owner's field_xp_over_time['und'][0] without
   * checking it's set: a player with no XP total yet gets PHP notices on
   * every new character form.
   */
  public function testNewCharacterFormHasNoPhpErrorsForNewPlayers() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner);
    $this->login($owner);
    $this->get('node/' . $character->nid . '/edit');
    $this->assertSame(array(), $this->pageErrorsFrom('vaxia_characters.module'));
  }

  /**
   * @group knownbug
   * _vaxia_characters_is_removed() reads $node->workflow_state_name, which a
   * sheet still in "(creation)" doesn't have: its owner gets PHP notices.
   */
  public function testNewSheetViewHasNoPhpErrors() {
    $owner = $this->owner();
    $this->viewSheet($owner, $this->site->createCharacter($owner));
    $this->assertSame(array(), $this->pageErrorsFrom('vaxia_characters.module'));
  }

  // ---------------------------------------------------------------------------
  // Approved characters.
  // ---------------------------------------------------------------------------

  public function testLowXpCharactersMentionRevisions() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner);
    $this->approveCharacter($character);
    list(, $messages) = $this->viewSheet($owner, $character);
    $this->assertStringContainsString('This character is currently listed below 20 XP.', $messages);
  }

  public function testLowXpNpcsDoNotMentionRevisions() {
    $owner = $this->owner();
    $npc = $this->site->createCharacter($owner, array('field_is_npc' => 1));
    $this->approveCharacter($npc);
    list(, $messages) = $this->viewSheet($owner, $npc);
    $this->assertStringNotContainsString('below 20 XP', $messages);
  }

  // ---------------------------------------------------------------------------
  // Who sees what.
  // ---------------------------------------------------------------------------

  public function testOthersSeeOnlyPublicFields() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner, array(
      'field_strength' => 33,
      'field_public_information' => 'Known as the Red Fox.',
    ));
    $this->approveCharacter($character);
    list($text) = $this->viewSheet($this->site->createUser(array('player')), $character);
    $this->assertStringContainsString('Known as the Red Fox.', $text);
    $this->assertStringNotContainsString('Strength', $text);
    list($text) = $this->viewSheet($owner, $character);
    $this->assertStringContainsString('Strength', $text);
  }

  public function testShNotesShowToOwnerAndStaffOnly() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner, array('field_sh_notes' => 'Owes the guild a favour.'));
    $this->approveCharacter($character);
    $this->assertStringContainsString('Owes the guild a favour.', $this->viewSheet($owner, $character)[0]);
    $this->assertStringContainsString('Owes the guild a favour.', $this->viewSheet($this->site->createUser(array('storyteller')), $character)[0]);
    $this->assertStringNotContainsString('Owes the guild a favour.', $this->viewSheet($this->site->createUser(array('player')), $character)[0]);
  }

  public function testPrivateShNotesAreHiddenFromTheOwner() {
    $owner = $this->owner();
    $character = $this->site->createCharacter($owner, array('field_sh_private_notes' => 'Secretly a spy.'));
    $this->approveCharacter($character);
    $this->assertStringNotContainsString('Secretly a spy.', $this->viewSheet($owner, $character)[0]);
    $this->assertStringContainsString('Secretly a spy.', $this->viewSheet($this->site->createUser(array('storyteller')), $character)[0]);
  }

}
