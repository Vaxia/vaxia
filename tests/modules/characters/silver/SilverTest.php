<?php

namespace Vaxia\Tests\Characters\Silver;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Silver: each character has an economy level that pays daily silver; staff
 * add or remove silver (removing more than the purse sells economy levels);
 * owners invest silver to raise their economy. Shown on the character's
 * Sheet tab (node/N/character_sheet).
 *
 * Economy math (from silver.module), level 3: worth 9.90, monthly income 0.40,
 * daily 0.013; cost to reach level 4: 6.93; selling level 3 back: 5.42.
 *
 * @group silver
 */
class SilverTest extends VaxiaTestCase {

  private function staff() {
    return $this->site->createUser(array('storyteller'), array('add xp to character sheet'));
  }

  private function character($economy = 3, array $values = array()) {
    $owner = $this->site->createUser(array('player'));
    return array($owner, $this->site->createCharacter($owner, $values + array('field_economy' => $economy)));
  }

  private function sheet($account, $character) {
    $this->login($account);
    $this->get('node/' . $character->nid . '/character_sheet');
    return $this->pageText();
  }

  private function addSilver($character, $amount) {
    $this->login($this->staff());
    $this->submitForm('node/' . $character->nid . '/character_sheet', array('add_silver' => (string) $amount), 'Add silver');
    return $this->messages();
  }

  // ---------------------------------------------------------------------------
  // Economy display.
  // ---------------------------------------------------------------------------

  public function testSheetShowsEconomyDetails() {
    list($owner, $character) = $this->character(3);
    $text = $this->sheet($owner, $character);
    $this->assertStringContainsString('You have 0.00 silver.', $text);
    $this->assertStringContainsString('Your current economy level is 3.', $text);
    $this->assertStringContainsString('Your monthly disposable income is 0.40.', $text);
    $this->assertStringContainsString('You get paid 0.01 a day.', $text);
    $this->assertStringContainsString('Your total worth is 9.90 if you cashed in all of your economy levels.', $text);
  }

  public function testOwnerWithoutSilverCannotInvest() {
    list($owner, $character) = $this->character(3);
    $this->sheet($owner, $character);
    $this->assertSame(0, $this->xpath()->query('//input[@value="Invest silver"]')->length);
  }

  // ---------------------------------------------------------------------------
  // Staff add and remove silver.
  // ---------------------------------------------------------------------------

  public function testStaffAddSilver() {
    list($owner, $character) = $this->character(3);
    $this->assertContains('10 silver has been added to this sheet, total 10.', $this->addSilver($character, 10));
    $this->assertStringContainsString('You have 10.00 silver.', $this->sheet($owner, $character));
  }

  public function testStaffRemoveSilverFromPurse() {
    list($owner, $character) = $this->character(3);
    $this->addSilver($character, 10);
    $this->addSilver($character, -4);
    $this->assertStringContainsString('You have 6.00 silver.', $this->sheet($owner, $character));
  }

  public function testRemovingMoreThanThePurseSellsEconomy() {
    list($owner, $character) = $this->character(3);
    $this->addSilver($character, 2);
    $messages = $this->addSilver($character, -4);
    $this->assertContains('Converted economy from level 3 to 2, for 5 silver.', $messages);
    $text = $this->sheet($owner, $character);
    $this->assertStringContainsString('Your current economy level is 2.', $text);
    // 2 - 2 = 0, then +5.42 from the level, -2 still owed: 3.42 left.
    $this->assertStringContainsString('You have 3.42 silver.', $text);
  }

  public function testSilverAmountMustBeANumber() {
    list(, $character) = $this->character();
    $this->addSilver($character, 'lots');
    $this->assertContains('Silver given is not a numeric value', $this->messages('error'));
  }

  public function testNoSilverFormForNpcOrStorytellerCharacters() {
    list(, $npc) = $this->character(3, array('field_is_npc' => 1));
    $this->sheet($this->staff(), $npc);
    $this->assertSame(0, $this->xpath()->query('//input[@value="Add silver"]')->length);
  }

  /**
   * @group knownbug
   * The removal message uses placeholder %add_silver but passes @add_silver, so
   * it reads "%add_silver silver has been removed from this sheet".
   */
  public function testRemovalMessageStatesAmount() {
    list(, $character) = $this->character(3);
    $this->addSilver($character, 10);
    $this->assertContains('-4 silver has been removed from this sheet, total 6.', $this->addSilver($character, -4));
  }

  // ---------------------------------------------------------------------------
  // Owners invest.
  // ---------------------------------------------------------------------------

  public function testInvestingEnoughRaisesEconomy() {
    list($owner, $character) = $this->character(3);
    $this->addSilver($character, 10);
    $this->login($owner);
    $this->submitForm('node/' . $character->nid . '/character_sheet', array('invest_silver' => '7'), 'Invest silver');
    $text = $this->sheet($owner, $character);
    $this->assertStringContainsString('Your current economy level is 4.', $text);
    $this->assertStringContainsString('You have 3.00 silver.', $text);
  }

  public function testPartialInvestmentIsKept() {
    list($owner, $character) = $this->character(3);
    $this->addSilver($character, 10);
    $this->login($owner);
    $this->submitForm('node/' . $character->nid . '/character_sheet', array('invest_silver' => '3'), 'Invest silver');
    $text = $this->sheet($owner, $character);
    $this->assertStringContainsString('Your current economy level is 3.', $text);
    $this->assertStringContainsString('Invested so far 3', $text);
    $this->assertStringContainsString('You have 7.00 silver.', $text);
  }

  public function testOnlyTheOwnerMayInvest() {
    list(, $character) = $this->character(3);
    $this->addSilver($character, 10);
    $this->sheet($this->staff(), $character);
    $this->assertSame(0, $this->xpath()->query('//input[@value="Invest silver"]')->length);
  }

  /**
   * @group knownbug
   * The refusal message uses placeholder @invest but passes '$invest', so it
   * reads "Unable to invest @invest".
   */
  public function testRefusedInvestmentStatesAmount() {
    list($owner, $character) = $this->character(3);
    $this->addSilver($character, 2);
    $this->login($owner);
    $this->submitForm('node/' . $character->nid . '/character_sheet', array('invest_silver' => '50'), 'Invest silver');
    $this->assertContains('Unable to invest 50, not enough silver available or not a number.', $this->messages());
  }

  // ---------------------------------------------------------------------------
  // Daily pay (cron).
  // ---------------------------------------------------------------------------

  private function payDay() {
    $this->site->resetDailyCron('vaxia_character_sheet_cron');
    $this->runCron();
  }

  public function testCharactersBelowMonthlyIncomeGetDailyPay() {
    list($owner, $character) = $this->character(3);
    $this->approveCharacter($character);
    $this->payDay();
    // 0.013 rounds to 0.01.
    $this->assertStringContainsString('You have 0.01 silver.', $this->sheet($owner, $character));
  }

  public function testCharactersAboveMonthlyIncomeSpendDailyPay() {
    list($owner, $character) = $this->character(3);
    $this->approveCharacter($character);
    $this->addSilver($character, 10);
    $this->payDay();
    // 10 - 0.013 = 9.987.
    $this->assertStringContainsString('You have 9.99 silver.', $this->sheet($owner, $character));
  }

  public function testNpcsAndStorytellersAreNotPaid() {
    list($owner, $npc) = $this->character(3, array('field_is_npc' => 1));
    $this->approveCharacter($npc);
    $this->payDay();
    $this->assertStringContainsString('You have 0.00 silver.', $this->sheet($this->site->createUser(array(), array('add xp to character sheet')), $npc));
  }

  public function testPayRunsOncePerDay() {
    list($owner, $character) = $this->character(3);
    $this->approveCharacter($character);
    $this->site->markDailyCronRan('vaxia_character_sheet_cron', time() - 3600);
    $this->runCron();
    $this->assertStringContainsString('You have 0.00 silver.', $this->sheet($owner, $character));
  }

  /**
   * Only "playable" (approved) characters are paid.
   */
  public function testDraftCharactersAreNotPaid() {
    list($owner, $character) = $this->character(3);
    $this->transition($character, 'draft');
    $this->payDay();
    $this->assertStringContainsString('You have 0.00 silver.', $this->sheet($owner, $character));
  }

}
