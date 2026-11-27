<?php

namespace Vaxia\Tests;

/**
 * Proposal flows: create through the node form, add a ballot question, launch,
 * vote, and close the way the site does (cron, two weeks untouched).
 */
trait ProposalTrait {

  protected function author() {
    return $this->site->createUser(array('player'), array('create proposal content', 'edit own proposal content'));
  }

  /**
   * Creates a proposal through the node form; returns it with nid and title.
   */
  protected function createProposalViaForm($author, $type = 'proposal', array $extra = array()) {
    // The forum must exist before the form is built, or its choice is "illegal".
    $forum = $this->site->term('forums', 'Proposals');
    $this->login($author);
    $title = $this->site->uniqueName('Proposal');
    $this->get('node/add/proposal');
    $this->submitForm(NULL, $extra + array(
      'title' => $title,
      'field_proposal_type[und]' => $type,
      'taxonomy_forums[und]' => $forum,
    ), 'Save');
    $this->assertMatchesRegularExpression('#/node/\d+#', $this->url, 'Proposal saved: ' . implode(' | ', $this->messages()));
    preg_match('#/node/(\d+)#', $this->url, $m);
    return (object) array('nid' => (int) $m[1], 'title' => $title, 'author' => $author);
  }

  /**
   * A launched proposal with one Yes/No question; $voters are eligible.
   */
  protected function openProposal(array $voters) {
    foreach ($voters as $v) {
      $this->site->addRole($v, 'voter');
    }
    $proposal = $this->createProposalViaForm($this->author());
    $this->site->addWebformQuestion($proposal, 'question', 'Do you agree?', array('yes' => 'Yes', 'no' => 'No'));
    $this->launch($proposal);
    return $proposal;
  }

  protected function launch($proposal) {
    $this->login($proposal->author);
    $this->submitForm('node/' . $proposal->nid . '/webform', array(), 'Click here to launch the proposal');
  }

  protected function vote($voter, $proposal, $answer = 'yes') {
    $this->login($voter);
    $this->submitForm('node/' . $proposal->nid, array('submitted[question]' => $answer), 'Submit');
  }

  /**
   * Closes it the way the site does: two weeks untouched, then cron.
   */
  protected function closeByCron($proposal) {
    $this->site->backdateNode($proposal, strtotime('-15 days'));
    $this->site->resetDailyCron('proposal_cron');
    $this->runCron();
  }

}
