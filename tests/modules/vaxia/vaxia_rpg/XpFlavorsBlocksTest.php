<?php

namespace Vaxia\Tests\Vaxia\VaxiaRpg;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The two blocks of D7 view character_xp_flavors, on an approved character's
 * page:
 * - "Character XP Flavors": each flavour XP type (physical, magical, ...;
 *   by name, at most 9) as a share of the character's flavour XP;
 * - "Character XP Breakdown": the XP pools in one row, shown only when the
 *   character has flavour XP.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group vaxia_rpg
 * @group views_blocks
 */
class XpFlavorsBlocksTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'character_xp_flavors-block', 'content');
    $this->site->placeBlock('views', 'character_xp_flavors-block_1', 'content');
  }

  /**
   * Table rows (cell texts) of one of the blocks on the last page; NULL when
   * it isn't shown.
   */
  private function table($display, $header = FALSE) {
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-' . $display . ' ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query($header ? './/thead/tr' : './/tbody/tr', $view->item(0)) as $tr) {
      $cells = array();
      foreach ($xpath->query('./td|./th', $tr) as $cell) {
        $cells[] = trim($cell->textContent);
      }
      $rows[] = $cells;
    }
    return $rows;
  }

  private function approvedCharacter($owner) {
    $character = $this->site->createCharacter($owner);
    $this->site->forceWorkflowState($character, 'approved');
    return $character;
  }

  public function testFlavourShares() {
    $player = $this->site->createUser(array('player'));
    $character = $this->approvedCharacter($player);
    $this->site->addFlavourXp($character, 'physical', 30);
    $this->site->addFlavourXp($character, 'magical', 10);
    // Counted in the flavour total, but not flavour rows.
    $this->site->addFlavourXp($character, 'misc', 10);
    // Neither.
    $this->site->addFlavourXp($character, 'transfer', 50);
    $this->site->addFlavourXp($character, 'site-award', 5);
    $this->login($player);
    $this->get('node/' . $character->nid);
    $this->assertSame(array(array('20.00%', 'magical'), array('60.00%', 'physical')), $this->table('block'));
  }

  public function testAtMostNineFlavours() {
    $player = $this->site->createUser(array('player'));
    $character = $this->approvedCharacter($player);
    // Types containing a flavour word count too ("good" in "goodness").
    foreach (array('artist', 'charismatic', 'evil', 'goodness', 'intelligent', 'magical', 'physical', 'rogue', 'support', 'supportive') as $type) {
      $this->site->addFlavourXp($character, $type, 10);
    }
    $this->login($player);
    $this->get('node/' . $character->nid);
    $rows = $this->table('block');
    $this->assertSame(array('artist', 'charismatic', 'evil', 'goodness', 'intelligent', 'magical', 'physical', 'rogue', 'support'), array_column($rows, 1));
    $this->assertSame('10.00%', $rows[0][0]);
  }

  public function testBreakdown() {
    $player = $this->site->createUser(array('player'));
    $character = $this->approvedCharacter($player);
    $this->site->addCharacterXp($character, 'bxp', 7.5);
    $this->site->addCharacterXp($character, 'rxp', 12);
    $this->site->addCharacterXp($character, 'spent_xp', -3);
    $this->site->addFlavourXp($character, 'physical', 20.5);
    $this->site->addFlavourXp($character, 'transfer', 40);
    $this->site->addFlavourXp($character, 'site-award', 9);
    $this->login($player);
    $this->get('node/' . $character->nid);
    $this->assertSame(array(array('Banked XP', 'Roleplayed XP', 'Spent XP', 'Transfer XP', 'Site Award XP', 'Flavor XP')), $this->table('block_1', TRUE));
    // Rounded down; negative pools show 0. "Site Award XP" repeats the
    // transfer total, as D7's view did.
    $this->assertSame(array(array('7', '12', '0', '40', '40', '20')), $this->table('block_1'));
  }

  public function testHiddenWithoutFlavourXp() {
    $player = $this->site->createUser(array('player'));
    $character = $this->approvedCharacter($player);
    $this->site->addCharacterXp($character, 'bxp', 5);
    $this->site->addFlavourXp($character, 'transfer', 5);
    $this->login($player);
    $this->get('node/' . $character->nid);
    $this->assertStatus(200);
    $this->assertNull($this->table('block'));
    $this->assertNull($this->table('block_1'));
  }

  public function testOnlyOnApprovedCharacters() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player);
    $this->site->forceWorkflowState($character, 'draft');
    $this->site->addFlavourXp($character, 'physical', 5);
    $this->login($player);
    $this->get('node/' . $character->nid);
    $this->assertStatus(200);
    $this->assertNull($this->table('block'));
    $this->assertNull($this->table('block_1'));
    // Nor on other pages.
    $this->get('node');
    $this->assertNull($this->table('block'));
  }

  public function testFollowsTheCharacterShown() {
    $player = $this->site->createUser(array('player'));
    $first = $this->approvedCharacter($player);
    $second = $this->approvedCharacter($player);
    $this->site->addFlavourXp($first, 'physical', 5);
    $this->site->addFlavourXp($second, 'magical', 5);
    $this->login($player);
    $this->get('node/' . $first->nid);
    $this->assertSame(array(array('100.00%', 'physical')), $this->table('block'));
    $this->get('node/' . $second->nid);
    $this->assertSame(array(array('100.00%', 'magical')), $this->table('block'));
    // Shown live: XP added later appears at once.
    $this->site->addFlavourXp($second, 'magical', 15);
    $this->site->addFlavourXp($second, 'evil', 20);
    $this->get('node/' . $second->nid);
    $this->assertSame(array(array('50.00%', 'evil'), array('50.00%', 'magical')), $this->table('block'));
  }

}
