<?php

namespace Vaxia\Tests\Rpg\Proposal;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The two blocks of D7 view proposals:
 * - "Proposals" (proposal_block), for players: the newest 15 published
 *   proposals that are open or changed in the last three days, with their
 *   status, and a link to the proposals page;
 * - "Proposals closing" (proposals_closing), for voters: open proposals last
 *   revised eleven or more days ago.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group proposal
 * @group views_blocks
 */
class ProposalBlocksTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'proposals-proposal_block', 'sidebar_first');
    $this->site->placeBlock('views', 'proposals-proposals_closing', 'content');
  }

  /**
   * Rows of one of the blocks on the front page, or NULL when it isn't shown.
   */
  private function rows($display) {
    $this->get('node');
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-' . $display . ' ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query('.//li', $view->item(0)) as $li) {
      $rows[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
    }
    return $rows;
  }

  private function proposal(array $values = array()) {
    return $this->site->createProposal(array('voter'), $values + array('title' => $this->site->uniqueName('Proposal')));
  }

  private function closed($age) {
    $proposal = $this->proposal();
    $this->site->setWebformOpen($proposal, FALSE);
    $this->site->backdateNode($proposal, time() - $age);
    return $proposal;
  }

  public function testPlayersSeeOpenAndRecentlyClosed() {
    $open = $this->proposal();
    $recent = $this->closed(2 * 86400);
    $old = $this->closed(4 * 86400);
    $unpublished = $this->proposal(array('status' => 0));
    $this->login($this->site->createUser(array('player')));
    $rows = $this->rows('proposal_block');
    $this->assertContains($open->title . ' (Open)', $rows);
    $this->assertContains($recent->title . ' (Closed)', $rows);
    $text = implode("\n", $rows);
    $this->assertStringNotContainsString($old->title, $text);
    $this->assertStringNotContainsString($unpublished->title, $text);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-proposal_block")]//li//a[@href="/node/' . $open->nid . '"]')->length);
    $this->assertText('These proposals are currently under site consideration or have recently come to a final vote within the last month. See the proposals page for all proposals.');
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-proposal_block")]//div[contains(@class, "view-footer")]//a[@href="/proposals"]')->length);
  }

  public function testProposalBlockOnlyForPlayers() {
    $this->proposal();
    $this->login($this->site->createUser(array('voter', 'storyteller')));
    $this->assertNull($this->rows('proposal_block'));
    $this->anonymous();
    $this->assertNull($this->rows('proposal_block'));
  }

  public function testNewestFifteenFirst() {
    $titles = array();
    for ($i = 1; $i <= 16; $i++) {
      $proposal = $this->proposal();
      $this->site->backdateNodeCreated($proposal, time() + 1000 + $i);
      $titles[$i] = $proposal->title . ' (Open)';
    }
    $this->login($this->site->createUser(array('player')));
    $this->assertSame(array_reverse(array_slice($titles, 1, 15)), $this->rows('proposal_block'));
  }

  public function testVotersSeeProposalsClosingSoon() {
    $closing = $this->proposal();
    $this->site->backdateNode($closing, time() - 12 * 86400);
    $young = $this->proposal();
    $this->site->backdateNode($young, time() - 5 * 86400);
    $closedOld = $this->closed(12 * 86400);
    $unpublished = $this->proposal(array('status' => 0));
    $this->site->backdateNode($unpublished, time() - 12 * 86400);
    $this->login($this->site->createUser(array('voter')));
    $rows = $this->rows('proposals_closing');
    $this->assertContains($closing->title, $rows);
    $text = implode("\n", $rows);
    foreach (array($young, $closedOld, $unpublished) as $node) {
      $this->assertStringNotContainsString($node->title, $text);
    }
    $this->assertText("These proposals are nearing their close date. If you haven't voted in them yet, please do! We need your input! Closing proposals:");
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-proposals_closing")]//a[@href="/node/' . $closing->nid . '"]')->length);
  }

  public function testClosingOnlyForVoters() {
    $closing = $this->proposal();
    $this->site->backdateNode($closing, time() - 12 * 86400);
    $this->login($this->site->createUser(array('player')));
    $this->assertNull($this->rows('proposals_closing'));
  }

  public function testEmptyBlocksAreHidden() {
    $this->site->deleteNodesOfType('proposal');
    $this->login($this->site->createUser(array('player', 'voter')));
    $this->assertNull($this->rows('proposal_block'));
    $this->assertNull($this->rows('proposals_closing'));
    $this->assertNoText('Closing proposals:');
  }

}
