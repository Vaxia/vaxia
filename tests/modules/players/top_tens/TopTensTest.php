<?php

namespace Vaxia\Tests\Players\TopTens;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Top tens: a public page listing, per flavor XP type ("warrior", "mage"...),
 * the ten approved player characters with the most XP of that type. NPCs and
 * storyhost characters are left out, as are transfer and site-award XP.
 *
 * Each test uses an XP type of its own (the types are a setting).
 *
 * @group top_tens
 */
class TopTensTest extends VaxiaTestCase {

  /** @var string */
  private $type;

  protected function setUp(): void {
    parent::setUp();
    // The type column holds 15 characters.
    $this->type = 'brave' . substr(md5(uniqid('', TRUE)), 0, 8);
    $this->site->setConfig('vaxia_xp_types', "good\nevil\nwarrior\nsite-award\ntransfer\n" . $this->type);
  }

  private function character(array $values = array(), $approve = TRUE) {
    $character = $this->site->createCharacter($this->site->createUser(array('player')), $values + array('title' => $this->site->uniqueName('Hero')));
    if ($approve) {
      $this->approveCharacter($character);
    }
    return $character;
  }

  /**
   * Awards XP of a type from the character's Sheet tab.
   */
  private function award($character, $xp, $type = NULL) {
    $this->inSession($this->superuser(), function () use ($character, $xp, $type) {
      $this->submitForm('node/' . $character->nid . '/character_sheet', array('xp' => (string) $xp, 'type' => $type ?: $this->type), 'Add XP');
    });
  }

  /**
   * Character titles listed under $type, in page order.
   */
  private function listed($type = NULL, array $query = array()) {
    $this->get('top-ten-displays', $query);
    $this->assertStatus(200);
    $titles = array();
    $type = $type ?: $this->type;
    foreach ($this->xpath()->query('//table[.//h2[normalize-space()="' . $type . '"] or preceding-sibling::*[1][normalize-space()="' . $type . '"] or caption[normalize-space()="' . $type . '"]]//td[contains(@class, "views-field-title")]') as $td) {
      $titles[] = trim($td->textContent);
    }
    return $titles;
  }

  public function testPageIsPublic() {
    $this->anonymous();
    $this->get('top-ten-displays');
    $this->assertStatus(200);
    $this->assertText('Top Tens');
  }

  public function testCharactersAreListedUnderTheirXpType() {
    $hero = $this->character();
    $this->award($hero, 5);
    $this->anonymous();
    $this->assertSame(array($hero->title), $this->listed());
  }

  public function testOnlyTheTopTenAreListed() {
    $characters = array();
    for ($xp = 1; $xp <= 11; $xp++) {
      $characters[$xp] = $this->character();
      $this->award($characters[$xp], $xp);
    }
    $this->anonymous();

    $listed = $this->listed();

    $this->assertCount(10, $listed);
    $this->assertNotContains($characters[1]->title, $listed, 'Lowest left out');
    $this->assertContains($characters[11]->title, $listed);
  }

  public function testUnapprovedCharactersAreLeftOut() {
    $draft = $this->character(array(), FALSE);
    $hero = $this->character();
    $this->award($draft, 3);
    $this->award($hero, 3);
    $this->anonymous();
    $this->assertSame(array($hero->title), $this->listed());
  }

  public function testNpcAndStorytellerSheetsCannotGetFlavorXp() {
    $this->login($this->superuser());
    foreach (array(array('field_is_npc' => 1), array('field_is_storyteller' => 1)) as $values) {
      $this->get('node/' . $this->character($values, FALSE)->nid . '/character_sheet');
      $this->assertSame(0, $this->xpath()->query('//input[@value="Add XP"]')->length);
    }
  }

  public function testTransferAndSiteAwardXpAreNotListed() {
    $hero = $this->character();
    $this->award($hero, 3, 'transfer');
    $this->award($hero, 3, 'site-award');
    $this->anonymous();
    $this->assertSame(array(), $this->listed('transfer'));
    $this->assertSame(array(), $this->listed('site-award'));
  }

  public function testRealmFilter() {
    $vaxian = $this->character();
    $sirian = $this->character(array('field_tag_realm' => $this->site->term('realm', 'Sirian')));
    $this->award($vaxian, 3);
    $this->award($sirian, 3);
    $this->anonymous();
    $this->assertSame(array($sirian->title), $this->listed(NULL, array('field_tag_realm_tid[]' => $this->site->term('realm', 'Sirian'))));
  }

  /**
   * @group knownbug
   * XP types are free text in the settings, but the totals table's type column
   * is varchar(15): awarding a longer type is an SQL error (PDOException) and
   * the XP is lost.
   */
  public function testLongXpTypesCanBeAwarded() {
    $type = 'extraordinarily_brave';
    $this->site->setConfig('vaxia_xp_types', "good\n" . $type);
    $hero = $this->character();
    $this->award($hero, 3, $type);
    $this->anonymous();
    $this->assertSame(array($hero->title), $this->listed($type));
  }

}
