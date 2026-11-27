<?php

namespace Vaxia\Tests\Players\TotalTopTen;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Total top ten (total-top-ten): for players and administrators, the ten
 * approved, published player characters (no NPCs, open NPCs or storyhost
 * characters) that have spent the most XP, under an "hxp" heading: amount,
 * artwork, character.
 *
 * Characters here spend amounts far above anything else the suite sets up, so
 * they make the top ten.
 *
 * D11 only: the D7 test site doesn't have the total_top_ten view.
 *
 * @group total_top_ten
 */
class TotalTopTenTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The total top ten page was ported to the D11 site only.');
    }
  }

  private function character($spent, array $values = array(), $approve = TRUE) {
    $character = $this->site->createCharacter($this->site->createUser(array('player')), $values + array('title' => $this->site->uniqueName('Hero')));
    if ($approve) {
      $this->site->forceWorkflowState($character, 'approved');
    }
    $this->site->addCharacterXp($character, 'spent_xp', $spent);
    return $character;
  }

  /**
   * [title => amount] in page order.
   */
  private function listed() {
    $this->login($this->site->createUser(array('player')));
    $this->get('total-top-ten');
    $this->assertStatus(200);
    $listed = array();
    $xpath = $this->xpath();
    foreach ($xpath->query('//table//tbody/tr[not(td[@colspan])]') as $tr) {
      $listed[trim($xpath->query('td[contains(@class, "views-field-title")]', $tr)->item(0)->textContent)] = trim($xpath->query('td[contains(@class, "views-field-xp-1")]', $tr)->item(0)->textContent);
    }
    return $listed;
  }

  public function testAccess() {
    foreach (array('player', 'administrator') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('total-top-ten');
      $this->assertStatus(200);
      $this->assertText('Total Top Ten');
    }
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('total-top-ten');
    $this->assertStatus(403);
    $this->anonymous();
    $this->get('total-top-ten');
    $this->assertStatus(403);
  }

  public function testTopTenBySpentXp() {
    $characters = array();
    for ($i = 1; $i <= 11; $i++) {
      $characters[$i] = $this->character(99000 + $i);
    }
    $listed = $this->listed();
    $this->assertText('hxp');
    $this->assertText('Amount');
    $this->assertText('Character');
    $expected = array();
    for ($i = 11; $i >= 2; $i--) {
      $expected[$characters[$i]->title] = (string) (99000 + $i);
    }
    $this->assertSame($expected, $listed);
    $this->assertNotNull($this->xpath()->query('//td[contains(@class, "views-field-title")]/a[contains(@href, "/node/' . $characters[11]->nid . '")]')->item(0), 'Character links to its sheet');
  }

  public function testOnlyApprovedPublishedPlayerCharactersCount() {
    $hero = $this->character(990000);
    $draft = $this->character(990005, array(), FALSE);
    $npc = $this->character(990004, array('field_is_npc' => 1));
    $openNpc = $this->character(990003, array('field_is_open_npc' => 1));
    $storyhost = $this->character(990002, array('field_is_storyteller' => 1));
    $unpublished = $this->character(990001);
    $this->site->saveNode($unpublished, array('status' => 0), (object) array('uid' => 1));
    $listed = $this->listed();
    $this->assertArrayHasKey($hero->title, $listed);
    foreach (array($draft, $npc, $openNpc, $storyhost, $unpublished) as $left) {
      $this->assertArrayNotHasKey($left->title, $listed);
    }
  }

}
