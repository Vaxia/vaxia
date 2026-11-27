<?php

namespace Vaxia\Tests\Vaxia\Vaxia;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Notify Returned Chars" block (D7 view vaxia_character_sheets,
 * block_notify_ret_chars): tells a player which of their published character
 * sheets were sent back to draft in the last three days after having been
 * submitted.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group vaxia
 * @group views_blocks
 */
class ReturnedCharactersBlockTest extends VaxiaTestCase {

  /** @var object Staff account that moves sheets on their Workflow tab. */
  private $staff;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'vaxia_character_sheets-block_notify_ret_chars', 'content');
    $this->staff = $this->site->createUser(array('administrator'));
  }

  /**
   * The block's rows on the front page, or NULL when it isn't shown.
   */
  private function rows() {
    $this->get('node');
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-block_notify_ret_chars ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query('.//li', $view->item(0)) as $li) {
      $rows[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
    }
    return $rows;
  }

  private function message($character) {
    return 'Your character "' . $character->title . '" has been returned to draft for further revision. Please check the "Workflow" tab on the character for details.';
  }

  /**
   * A character moved draft -> submitted -> draft on its Workflow tab (moves
   * there are recorded in the workflow history; setting the state directly
   * is not).
   */
  private function returned($owner, array $values = array()) {
    $node = $this->site->createCharacter($owner, $values);
    foreach (array('draft', 'submitted', 'draft') as $state) {
      $this->transition($node, $state, $this->staff);
    }
    return $node;
  }

  public function testReturnedCharacterNotice() {
    $player = $this->site->createUser(array('player'));
    $character = $this->returned($player);
    $this->login($player);
    $this->assertSame(array($this->message($character)), $this->rows());
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-block_notify_ret_chars")]//a[@href="/node/' . $character->nid . '"]')->length, 'The notice links to the character');
  }

  public function testOnlyReturnedOwnPublishedCharacters() {
    $player = $this->site->createUser(array('player'));
    $neverSubmitted = $this->site->createCharacter($player);
    $this->transition($neverSubmitted, 'draft', $this->staff);
    $reApproved = $this->returned($player);
    $this->transition($reApproved, 'submitted', $this->staff);
    $this->transition($reApproved, 'approved', $this->staff);
    $unpublished = $this->returned($player, array('status' => 0));
    $others = $this->returned($this->site->createUser(array('player')));
    $mine = $this->returned($player);
    $this->login($player);
    $rows = $this->rows();
    $this->assertSame(array($this->message($mine)), $rows);
    foreach (array($neverSubmitted, $reApproved, $unpublished, $others) as $node) {
      $this->assertStringNotContainsString('"' . $node->title . '"', implode("\n", $rows));
    }
  }

  public function testNoticeLastsThreeDays() {
    $player = $this->site->createUser(array('player'));
    $old = $this->returned($player);
    $this->site->backdateWorkflowHistory($old, time() - 3 * 86400 - 60);
    $recent = $this->returned($player);
    $this->site->backdateWorkflowHistory($recent, time() - 3 * 86400 + 3600);
    $this->login($player);
    $this->assertSame(array($this->message($recent)), $this->rows());
  }

  public function testSubmittedTwiceListedOnce() {
    // D7 listed it once per "submitted" history entry.
    $player = $this->site->createUser(array('player'));
    $character = $this->returned($player);
    $this->transition($character, 'submitted', $this->staff);
    $this->transition($character, 'draft', $this->staff);
    $this->login($player);
    $this->assertSame(array($this->message($character)), $this->rows());
  }

  public function testHiddenWhenNothingReturned() {
    $this->login($this->site->createUser(array('player')));
    $this->assertNull($this->rows());
  }

  public function testWhoSeesIt() {
    $owner = $this->site->createUser(array('storyteller'));
    $this->site->removeRole($owner, 'new account');
    $this->returned($owner);
    $this->login($owner);
    $this->assertNull($this->rows(), 'Not for storytellers without the player role');
    $this->site->addRole($owner, 'new account');
    $this->assertCount(1, $this->rows());
  }

}
