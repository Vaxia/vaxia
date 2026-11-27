<?php

namespace Vaxia\Tests\Vaxia\Vaxia;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia character sheet system and storyhost dice.
 *
 * - The Sheet tab lists a character's XP (available, spent, banked,
 *   roleplayed, posted, flavours).
 * - Players can move 75% of a retired character's spent XP ("HXP") to another
 *   of their characters; the old sheet becomes xp_transferred.
 * - Storyhost and NPC sheets can pass their roleplayed XP (RXP) to a
 *   character.
 * - Draft sheets without XP must spend exactly the 300 starting points.
 * - Storyhost characters roll with preset ability levels and free dice.
 *
 * @group vaxia
 */
class VaxiaSheetTest extends VaxiaTestCase {

  use DiceTrait;

  private function sheetTab($account, $character) {
    $this->login($account);
    $this->get('node/' . $character->nid . '/character_sheet');
    $this->assertStatus(200);
    return $this->pageText();
  }

  private function label($character, $owner) {
    return $character->title . ' (' . $owner->name . ') [nid:' . $character->nid . ']';
  }

  // ---------------------------------------------------------------------------
  // XP display.
  // ---------------------------------------------------------------------------

  public function testSheetTabListsXp() {
    $owner = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($owner);
    $this->approveCharacter($character);
    $this->site->addCharacterXp($character, 'xp', 12);
    $this->site->addCharacterXp($character, 'spent_xp', 5);
    $this->site->addCharacterXp($character, 'bxp', 3);
    $this->site->addCharacterXp($character, 'rxp', 2);
    $this->site->addCharacterXp($character, 'kxp', 40);

    $text = $this->sheetTab($owner, $character);

    $this->assertStringContainsString('You have 7 xp available to spend.', $text);
    $this->assertStringContainsString('Spent 5 out of 12 xp.', $text);
    $this->assertStringContainsString('Banked 3 bxp.', $text);
    $this->assertStringContainsString('Roleplayed 2 rxp.', $text);
    $this->assertStringContainsString('Posted 40 kxp.', $text);
  }

  public function testFlavorXpIsListed() {
    $owner = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($owner);
    $this->approveCharacter($character);
    $this->inSession($this->superuser(), function () use ($character) {
      $this->submitForm('node/' . $character->nid . '/character_sheet', array('xp' => '4', 'type' => 'warrior'), 'Add XP');
    });
    $this->assertStringContainsString('Earned 4 warrior XP.', $this->sheetTab($owner, $character));
  }

  /**
   * @group knownbug
   * "Earned N historical xp." prints the spent XP, not the XP earned.
   */
  public function testHistoricalXpShowsXpEarned() {
    $owner = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($owner);
    $this->approveCharacter($character);
    $this->site->addCharacterXp($character, 'xp', 12);
    $this->site->addCharacterXp($character, 'spent_xp', 5);
    $this->assertStringContainsString('Earned 12 historical xp.', $this->sheetTab($owner, $character));
  }

  // ---------------------------------------------------------------------------
  // Transfers.
  // ---------------------------------------------------------------------------

  public function testAutocompleteOffersOwnPlayableCharacters() {
    $owner = $this->site->createUser(array('player'));
    $hero = $this->site->createCharacter($owner, array('title' => 'Zed' . $this->site->uniqueName('hero')));
    $this->approveCharacter($hero);
    $draft = $this->site->createCharacter($owner, array('title' => 'Zed' . $this->site->uniqueName('draft')));
    $theirs = $this->site->createCharacter($this->site->createUser(array('player')), array('title' => 'Zed' . $this->site->uniqueName('theirs')));
    $this->approveCharacter($theirs);

    $this->login($owner);
    $this->get('vaxia/transfer_xp/Zed');
    $matches = json_decode($this->content, TRUE);

    $this->assertContains($this->label($hero, $owner), array_keys($matches));
    $this->assertCount(1, $matches, 'Not drafts or other players\' characters');
  }

  public function testHxpTransferMovesSpentXpAndRetiresTheSheet() {
    $owner = $this->site->createUser(array('player'));
    $old = $this->site->createCharacter($owner, array('title' => $this->site->uniqueName('Old')));
    $this->approveCharacter($old);
    $this->site->addCharacterXp($old, 'xp', 40);
    $this->site->addCharacterXp($old, 'spent_xp', 40);
    $new = $this->site->createCharacter($owner, array('title' => $this->site->uniqueName('New')));
    $this->approveCharacter($new);

    $this->sheetTab($owner, $old);
    $this->assertText('When transferred, 75% of the XP (30 out of 40) will be placed on the selected character sheet.');
    $this->submitForm(NULL, array('target' => $this->label($new, $owner), 'confirm' => 'TRANSFER'), 'Transfer ALL character HXP');

    $this->assertStringContainsString('Transferring 30 XP out of 40 from #' . $old->nid, implode(' ', $this->messages('status')));
    $this->assertSame('xp_transferred', $this->workflowState($old));
    // A new sheet (nothing spent yet) gets spendable XP.
    $this->assertStringContainsString('You have 30 xp available to spend.', $this->sheetTab($owner, $new));
  }

  public function testHxpTransferNeedsConfirmation() {
    $owner = $this->site->createUser(array('player'));
    $old = $this->site->createCharacter($owner);
    $this->approveCharacter($old);
    $new = $this->site->createCharacter($owner);
    $this->approveCharacter($new);
    $this->sheetTab($owner, $old);
    $this->submitForm(NULL, array('target' => $this->label($new, $owner), 'confirm' => 'yes'), 'Transfer ALL character HXP');
    $this->assertContains('Transfer was not confirmed.', $this->messages('error'));
    $this->assertSame('approved', $this->workflowState($old));
  }

  public function testHxpTransferToAnUnknownCharacterFails() {
    $owner = $this->site->createUser(array('player'));
    $old = $this->site->createCharacter($owner);
    $this->approveCharacter($old);
    $this->sheetTab($owner, $old);
    $this->submitForm(NULL, array('target' => 'Nobody', 'confirm' => 'TRANSFER'), 'Transfer ALL character HXP');
    $this->assertContains('Unable to locate selected character. Transfer not completed.', $this->messages('error'));
  }

  public function testStoryhostsPassRxpToACharacter() {
    $sh = $this->site->createUser(array('player', 'storyteller'));
    $sh_character = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Sh'), 'field_is_storyteller' => 1));
    $this->site->addCharacterXp($sh_character, 'rxp', 5);
    $this->site->addCharacterXp($sh_character, 'xp', 5);
    $hero = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Hero')));
    $this->approveCharacter($hero);

    $this->sheetTab($sh, $sh_character);
    $this->submitForm(NULL, array('rxp_target' => $this->label($hero, $sh), 'rxp_amount' => '3'), 'Transfer character RXP');

    $messages = implode(' ', $this->messages('status'));
    $this->assertStringContainsString('Transfering RXP (3) from #' . $sh_character->nid, $messages);
    $text = $this->sheetTab($sh, $hero);
    $this->assertStringContainsString('Roleplayed 3 rxp.', $text);
    $this->assertStringContainsString('Roleplayed 2 rxp.', $this->sheetTab($sh, $sh_character));
  }

  public function testPlayerCharactersHaveNoRxpTransfer() {
    $owner = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($owner);
    $this->approveCharacter($character);
    $this->site->addCharacterXp($character, 'rxp', 5);
    $this->sheetTab($owner, $character);
    $this->assertSame(0, $this->xpath()->query('//input[@value="Transfer character RXP"]')->length);
  }

  /**
   * @group knownbug
   * The RXP transfer log message uses $user->name without global $user:
   * a PHP notice, and the log reads "by ."
   */
  public function testRxpTransferHasNoPhpErrors() {
    $sh = $this->site->createUser(array('player', 'storyteller'));
    $sh_character = $this->site->createCharacter($sh, array('field_is_storyteller' => 1));
    $this->site->addCharacterXp($sh_character, 'rxp', 5);
    $hero = $this->site->createCharacter($sh);
    $this->approveCharacter($hero);
    $this->sheetTab($sh, $sh_character);
    $this->submitForm(NULL, array('rxp_target' => $this->label($hero, $sh), 'rxp_amount' => '1'), 'Transfer character RXP');
    $this->assertSame(array(), $this->pageErrorsFrom('vaxia.character_sheet.inc'));
  }

  // ---------------------------------------------------------------------------
  // New sheet points.
  // ---------------------------------------------------------------------------

  /**
   * Saves the owner's new sheet through its edit form with every required
   * field filled, stats at $stat and one skill at $skill; returns the errors.
   */
  private function saveNewSheet($stat, $skill) {
    // The skill aspects are required and come from this vocabulary.
    $aspect = $this->site->term('skill_verbs', 'craft');
    $owner = $this->site->createUser(array('player'), array('create character_sheet content', 'edit own character_sheet content'), array('field_xp_over_time' => 0));
    $character = $this->site->createCharacter($owner);
    // Points are checked while the sheet is a draft.
    $this->transition($character, 'draft', $owner);
    $this->login($owner);
    $this->get('node/' . $character->nid . '/edit');
    $other_species = $this->xpath()->query('//select[@name="' . $this->fieldName('field_other_species[und]') . '"]/option[@value!="_none"]')->item(0)->getAttribute('value');
    $edit = array(
      'field_background[und][0][value]' => 'Raised by wolves.',
      'field_personality[und][0][value]' => 'Gruff.',
      'field_occupation[und][0][value]' => 'Smith.',
      'field_description[und][0][value]' => 'Tall.',
      'field_other_species[und]' => $other_species,
      'field_real_age[und][0][value]' => '30',
      'field_birth_month[und]' => '03',
      'field_birth_day[und][0][value]' => '12',
      // Languages of the realm and another realm (only the chosen realm's are
      // required).
      'field_fantasy_languages[und][common_nat]' => TRUE,
      'field_scifi_languages[und][standard_nat]' => TRUE,
      'field_skill[und][0][field_skill_name][und][0][value]' => 'Smithing',
      'field_skill[und][0][field_skill_value][und][0][value]' => (string) $skill,
      'field_skill[und][0][field_aspect_one][und]' => (string) $aspect,
      'field_skill[und][0][field_aspect_two][und]' => (string) $aspect,
      'field_skill[und][0][field_aspect_three][und]' => (string) $aspect,
    );
    foreach (array('life', 'endurance', 'strength', 'dexterity', 'intelligence', 'spirituality', 'charisma') as $name) {
      $edit['field_' . $name . '[und][0][value]'] = (string) $stat;
    }
    $this->submitForm(NULL, $edit, 'Save');
    return implode(' ', $this->messages('error'));
  }

  public function testNewSheetsMustSpendAllTheStartingPoints() {
    $this->assertMatchesRegularExpression('/You have spent \d+ points out of 300 available points\. You have more points to spend\./', $this->saveNewSheet(10, 5));
  }

  public function testNewSheetsCannotOverspend() {
    $this->assertMatchesRegularExpression('/You have spent \d+ points out of 300 available points\. Please revise your sheet\./', $this->saveNewSheet(40, 40));
  }

  // ---------------------------------------------------------------------------
  // Storyhost dice.
  // ---------------------------------------------------------------------------

  private function storyhostRoller() {
    $sh = $this->roller(array('use SH dice options'));
    $character = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Sh'), 'field_is_storyteller' => 1));
    return array($sh, $character);
  }

  public function testStoryhostCharactersRollPresetLevelsAndFreeDice() {
    list($sh, $character) = $this->storyhostRoller();
    $room = $this->site->createRoom();
    $this->login($sh);
    $this->get('node/' . $room->nid);
    $this->triggerAjax('Post', array('field_comment_character[und]' => $character->nid), 'field_comment_character[und]');
    $levels = array();
    foreach ($this->xpath()->query('//select[@name="vaxia_rolls[dice][0][stat]"]/option') as $o) {
      $levels[] = trim($o->textContent);
    }
    $this->assertContains('Trained (45)', $levels);
    $this->assertContains('Mastery (70) (NPC cap)', $levels);
    $sizes = array();
    foreach ($this->xpath()->query('//select[@name="vaxia_rolls[dice][5][size]"]/option') as $o) {
      $sizes[] = $o->getAttribute('value');
    }
    $this->assertSame(array('100', '20', '12', '10', '8', '6', '4', '2'), $sizes);
  }

  public function testStoryhostRollsUseThePresetLevels() {
    list($sh, $character) = $this->storyhostRoller();
    $room = $this->site->createRoom();
    $text = $this->site->uniqueName('storm');
    $this->login($sh);
    $this->get('node/' . $room->nid);
    $this->triggerAjax('Post', array('field_comment_character[und]' => $character->nid), 'field_comment_character[und]');
    $this->submitForm(NULL, array(
      'comment_body[und][0][value]' => $text,
      'field_comment_character[und]' => $character->nid,
      'vaxia_rolls[dice][0][number]' => '1',
      'vaxia_rolls[dice][0][stat]' => '25',
      'vaxia_rolls[dice][0][skill]' => '45',
    ), 'Post');
    $lines = $this->diceLinesOf($text);
    $this->assertCount(1, $lines);
    // Stat and skill levels add up to the might.
    $this->assertMatchesRegularExpression('/^stat \(25\) \+ skill \(45\) => Might: 70 Roll: \d+$/', $lines[0]);
  }

}
