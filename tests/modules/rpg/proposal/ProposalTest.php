<?php

namespace Vaxia\Tests\Rpg\Proposal;

use Vaxia\Tests\ProposalTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Proposals: site votes run as webforms. The author builds the ballot, launches
 * it, and it stays open for two weeks from its last edit. Only voters eligible
 * when the proposal was set up may vote; results show once it closes.
 *
 * @group proposal
 */
class ProposalTest extends VaxiaTestCase {

  use ProposalTrait;

  protected function setUp(): void {
    parent::setUp();
    foreach (array('proposal_node_help', 'proposal_open_timeframe', 'proposal_active_timeframe', 'proposal_voter_timeframe') as $name) {
      $this->site->deleteConfig($name);
    }
  }

  private function isOpen($proposal) {
    return strpos($this->nodeText($proposal), 'This proposal will stay open') !== FALSE;
  }

  /**
   * array(votes, eligible voters) from the participation line, as staff see it.
   */
  private function participation($proposal) {
    preg_match('/(\d+) out of (\d+) potential voters \(/', $this->nodeText($proposal), $m);
    return array((int) $m[1], (int) $m[2]);
  }

  private function ballotEnabledFor($account, $proposal) {
    $this->login($account);
    $this->get('node/' . $proposal->nid);
    return $this->xpath()->query('//select[@name="submitted[question]" and not(@disabled)]')->length === 1;
  }

  // ---------------------------------------------------------------------------
  // Settings.
  // ---------------------------------------------------------------------------

  public function testAdminSettings() {
    $this->login($this->site->createUser(array(), array('administer proposal')));
    $this->submitForm('admin/rpg/proposal', array(
      'proposal_node_help' => 'Read the rules first.',
      'proposal_open_timeframe' => '10 days',
    ), 'Save configuration');
    $this->assertContains('The configuration options have been saved.', $this->messages('status'));

    $this->login($this->author());
    $this->get('node/add/proposal');
    $this->assertText('Read the rules first.');
  }

  public function testSettingsNeedPermission() {
    $this->login($this->author());
    $this->get('admin/rpg/proposal');
    $this->assertStatus(403);
  }

  // ---------------------------------------------------------------------------
  // Creating and launching.
  // ---------------------------------------------------------------------------

  public function testNewProposalGoesToBallotBuilderUnlaunched() {
    $proposal = $this->createProposalViaForm($this->author());
    $this->assertStringEndsWith('/node/' . $proposal->nid . '/webform/components', $this->url, 'Sent to build the ballot');
    $this->assertText('Add a new form element');
    $this->assertSame(1, $this->xpath()->query('//input[@value="Click here to launch the proposal"]')->length);
    $this->assertFalse($this->isOpen($proposal));
    $this->assertStringContainsString('Additional Comments', $this->nodeText($proposal));
  }

  public function testLaunchingOpensVoting() {
    $proposal = $this->createProposalViaForm($this->author());
    $this->launch($proposal);
    $this->assertText('This proposal has been opened for voting.');
    $this->assertTrue($this->isOpen($proposal));
  }

  public function testGrievanceOpensAtOnceAnonymouslyWithAQuestionPerMember() {
    // Only players can be named (field_grievance_members references role player).
    $accused = $this->site->createUser(array('player', 'lead'));
    $author = $this->author();
    $proposal = $this->createProposalViaForm($author, 'grievance', array(
      'field_grievance_members[und][0][uid]' => $accused->name,
    ));

    $this->assertTrue($this->isOpen($proposal), 'Open immediately');
    $text = $this->nodeText($proposal);
    $this->assertStringNotContainsString($author->name, $text, 'Author hidden');
    $this->assertStringContainsString('Should ' . $accused->name . ' be removed from their position(s) on the site for misuse of power?', $text);
  }

  // ---------------------------------------------------------------------------
  // Voting.
  // ---------------------------------------------------------------------------

  public function testOpenProposalShowsCloseDateAndHidesResults() {
    $voter = $this->site->createUser(array('player'));
    $proposal = $this->openProposal(array($voter));
    $this->login($voter);
    $this->get('node/' . $proposal->nid);
    $this->assertText('This proposal will stay open for two weeks since it was last updated. It is set to close on ' . date('l, F j, Y', strtotime('+2 weeks')));
    $this->assertSame(0, $this->xpath()->query('//div[@class="proposal_result"]//table')->length, 'No results while open');
  }

  public function testEligibleVoterCanVote() {
    $voter = $this->site->createUser(array('player'));
    $proposal = $this->openProposal(array($voter));
    list($before) = $this->participation($proposal);
    $this->vote($voter, $proposal);
    list($after) = $this->participation($proposal);
    $this->assertSame($before + 1, $after);
  }

  public function testVoterAddedLaterCannotVote() {
    $proposal = $this->openProposal(array($this->site->createUser(array('player'))));
    $late = $this->site->createUser(array('player', 'voter'));
    $this->assertFalse($this->ballotEnabledFor($late, $proposal));
    $this->assertStringContainsString('You are not eligible to vote on this issue.', implode(' ', $this->messages()));
  }

  public function testParticipationLine() {
    $a = $this->site->createUser(array('player'));
    $proposal = $this->openProposal(array($a, $this->site->createUser(array('player'))));
    $this->vote($a, $proposal);
    list($votes, $eligible) = $this->participation($proposal);
    $this->assertSame(1, $votes);
    $this->assertStringContainsString('1 out of ' . $eligible . ' potential voters (' . number_format(100 / $eligible) . ' %) responsed to this proposal.', $this->nodeText($proposal));
  }

  public function testResultsTabOnlyForAdminsWhileOpen() {
    $proposal = $this->openProposal(array($this->site->createUser(array('player'))));
    $this->login($proposal->author);
    $this->get('node/' . $proposal->nid . '/webform-results');
    $this->assertStatus(403);
    $this->login($this->site->createUser(array(), array('administer proposal')));
    $this->get('node/' . $proposal->nid . '/webform-results');
    $this->assertStatus(200);
  }

  public function testConfigureAndEmailPagesAreClosedForProposals() {
    $proposal = $this->createProposalViaForm($this->author());
    $this->login($this->superuser());
    $this->get('node/' . $proposal->nid . '/webform/configure');
    $this->assertStatus(403);
    $this->get('node/' . $proposal->nid . '/webform/emails');
    $this->assertStatus(403);
  }

  public function testOrdinaryWebformsKeepConfigurePage() {
    $webform = $this->site->createWebform(array(2), array());
    $this->login($this->superuser());
    $this->get('node/' . $webform->nid . '/webform/configure');
    $this->assertStatus(200);
  }

  // ---------------------------------------------------------------------------
  // Closing on schedule.
  // ---------------------------------------------------------------------------

  public function testCronClosesProposalsUntouchedForTwoWeeks() {
    $voter = $this->site->createUser(array('player'));
    $stale = $this->openProposal(array($voter));
    $fresh = $this->openProposal(array($this->site->createUser(array('player'))));
    $this->vote($voter, $stale, 'no');

    $this->closeByCron($stale);

    $this->assertFalse($this->isOpen($stale), 'Stale closed');
    $this->assertTrue($this->isOpen($fresh), 'Recent stays open');
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $stale->nid);
    $this->assertText('This proposal is closed.');
    $this->assertText('This webform is no longer accepting submissions.');
    $this->assertGreaterThan(0, $this->xpath()->query('//div[@class="proposal_result"]')->length, 'Results shown once closed');
  }

  public function testCronClosesOncePerDay() {
    $stale = $this->openProposal(array($this->site->createUser(array('player'))));
    $this->site->backdateNode($stale, strtotime('-15 days'));
    $this->site->markDailyCronRan('proposal_cron', time() - 3600);
    $this->runCron();
    $this->assertTrue($this->isOpen($stale));
  }

  // ---------------------------------------------------------------------------
  // Becoming a voter.
  // ---------------------------------------------------------------------------

  public function testPlayerPostingContentBecomesVoter() {
    $player = $this->site->createUser(array('player'), array('create page content'));
    $this->login($player);
    $this->submitForm('node/add/page', array('title' => $this->site->uniqueName('Post')), 'Save');
    $this->assertTrue($this->hasRole($player, 'voter'));
  }

  public function testPlayerCommentingBecomesVoter() {
    $player = $this->site->createUser(array('player'));
    $page = $this->site->createNode('page');
    $this->login($player);
    $this->submitForm('node/' . $page->nid, array('comment_body[und][0][value]' => 'Hello'), 'Save');
    $this->assertTrue($this->hasRole($player, 'voter'));
  }

  public function testNonPlayersDontBecomeVoters() {
    $account = $this->site->createUser(array('lead'), array('create page content'));
    $this->site->removeRole($account, 'new account');
    $this->login($account);
    $this->submitForm('node/add/page', array('title' => $this->site->uniqueName('Post')), 'Save');
    $this->assertFalse($this->hasRole($account, 'voter'));
  }

  // ---------------------------------------------------------------------------
  // Lists.
  // ---------------------------------------------------------------------------

  public function testPlayersSeeProposalList() {
    $proposal = $this->openProposal(array($this->site->createUser(array('player'))));
    $this->login($this->site->createUser(array('player')));
    $this->get('proposals');
    $this->assertStatus(200);
    $this->assertText($proposal->title);
  }

  public function testProposalListHiddenFromNonPlayers() {
    $account = $this->site->createUser(array('lead'));
    $this->site->removeRole($account, 'new account');
    $this->login($account);
    $this->get('proposals');
    $this->assertStatus(403);
  }

  // ---------------------------------------------------------------------------
  // Known bugs.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * Shadowbanned users can vote. Every shadowban check calls
   * shadowban_is_banned_user(), which no module defines; proposal_node_view()
   * also only compares ("== FALSE") instead of closing the ballot.
   */
  public function testShadowbannedUserCannotVote() {
    $banned = $this->site->createUser(array('player', 'shadowbanned user'));
    $proposal = $this->openProposal(array($banned));
    $this->assertFalse($this->ballotEnabledFor($banned, $proposal));
  }

  /**
   * @group knownbug
   * Launching invokes open_proposal with an undefined $node: a PHP notice on the
   * page and the event fires with nothing.
   */
  public function testLaunchHasNoPhpErrors() {
    $this->launch($this->createProposalViaForm($this->author()));
    $this->assertSame(array(), $this->pageErrorsFrom('proposal.module'));
  }

  /**
   * @group knownbug
   * When none of the eligible voters commented in the window, the "active
   * voters" percentage divides by zero (warning; fatal on PHP 8).
   */
  public function testNoActiveVotersHasNoPhpErrors() {
    $proposal = $this->openProposal(array($this->site->createUser(array('player'))));
    $this->login($this->superuser());
    $this->get('node/' . $proposal->nid);
    $this->assertSame(array(), $this->pageErrorsFrom('proposal.module'));
  }

  /**
   * @group knownbug
   * The Proposals and Elections page displays both claim /proposals, so the
   * Elections page (latest election, one item) is unreachable.
   */
  public function testElectionsPageIsReachable() {
    $this->openProposal(array($this->site->createUser(array('player'))));
    $this->login($this->site->createUser(array('player')));
    $this->get('proposals');
    $this->assertText('Elections');
  }

}
