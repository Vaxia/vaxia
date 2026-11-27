<?php

namespace Vaxia\Tests\Rpg\RpgElection;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Elections: yearly (or off-cycle) staff elections run as proposal webforms.
 * Candidates are users in the "run" roles who haven't opted out per position;
 * winners get the elected role and are recorded per cycle for term limits.
 *
 * Setup: positions "Setting Lead" (2 seats) and "Social Lead" (1 seat);
 * storytellers may run; the elected role is "lead"; players vote.
 *
 * @group rpg_election
 */
class RpgElectionTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    $forum = $this->site->term('forums', 'Elections');
    $this->site->grantPermissions('player', array('can vote on rpg_elections'));
    $this->inSession($this->superuser(), function () use ($forum) {
      // Positions first: saving creates their opt-in fields and seat settings.
      $this->submitForm('admin/rpg/rpg_election', array('election_positions' => "Setting Lead\nSocial Lead"), 'Save configuration');
      $this->get('admin/rpg/rpg_election');
      $this->submitForm(NULL, array(
        'election_position_count_setting_lead' => '2',
        'election_position_count_social_lead' => '1',
        'election_role' => $this->optionValue('election_role', 'lead'),
        'election_run_roles[]' => array($this->optionValue('election_run_roles[]', 'storyteller')),
        'election_default_forum' => (string) $forum,
      ), 'Save configuration');
    });
    // Each test starts with no open ballots, no sitting leads and no candidates.
    $this->site->closeAllProposals();
    $this->site->removeRoleFromEveryone('lead');
    $this->site->removeRoleFromEveryone('storyteller');
  }

  private function candidate(array $optIn = array('setting_lead' => 1, 'social_lead' => 1)) {
    $values = array();
    foreach ($optIn as $key => $on) {
      $values['field_staff_' . $key] = $on;
    }
    return $this->site->createUser(array('storyteller'), array(), $values);
  }

  private function adminLogin() {
    $this->login($this->site->createUser(array(), array('administrate rpg_elections')));
  }

  /**
   * Election proposals, newest first, from the admin content list.
   */
  private function elections() {
    return $this->inSession($this->superuser(), function () {
      $this->get('admin/content', array('type' => 'proposal', 'order' => 'changed', 'sort' => 'desc'));
      $out = array();
      foreach ($this->xpath()->query('//a[contains(text(), "Site Election")]') as $a) {
        if (preg_match('#/node/(\d+)$#', $a->getAttribute('href'), $m)) {
          $out[] = (object) array('nid' => (int) $m[1], 'title' => trim($a->textContent));
        }
      }
      usort($out, function ($a, $b) {
        return $b->nid - $a->nid;
      });
      return $out;
    });
  }

  private function holdElection() {
    $this->adminLogin();
    $this->submitForm('admin/rpg/rpg_election', array(), 'Hold an election off-cycle');
    $messages = $this->messages();
    $elections = $this->elections();
    $this->assertNotEmpty($elections, 'Election created: ' . implode(' | ', $messages));
    return $elections[0];
  }

  /**
   * Ballot choices as a voter sees them: form key => candidate names.
   */
  private function ballot($election) {
    return $this->inSession($this->site->createUser(array('player')), function () use ($election) {
      $this->get('node/' . $election->nid);
      $out = array();
      $xpath = $this->xpath();
      // D7 names ballot boxes submitted[KEY]; D11 just KEY.
      foreach ($xpath->query($this->webformFormXpath() . '//select') as $select) {
        $m = array(1 => preg_replace('/^submitted\[(.+)\]$/', '$1', $select->getAttribute('name')));
        $names = array();
        foreach ($xpath->query('.//option', $select) as $opt) {
          if ($opt->getAttribute('value') !== '') {
            $names[] = trim($opt->textContent);
          }
        }
        $out[$m[1]] = $names;
      }
      return $out;
    });
  }

  private function vote($voter, $election, array $ballot) {
    $this->login($voter);
    $edit = array();
    foreach ($ballot as $key => $name) {
      $edit['submitted[' . $key . ']'] = $name;
    }
    $this->submitForm('node/' . $election->nid, $edit, 'Submit');
  }

  private function closeElections() {
    $this->adminLogin();
    $this->submitForm('admin/rpg/rpg_election', array(), 'Close open elections');
  }

  // ---------------------------------------------------------------------------
  // Settings.
  // ---------------------------------------------------------------------------

  public function testSettingsPageListsPositionsWithSeatsAndDescriptions() {
    $this->adminLogin();
    $this->get('admin/rpg/rpg_election');
    $this->assertStatus(200);
    $xpath = $this->xpath();
    $this->assertSame("Setting Lead\nSocial Lead", trim(str_replace("\r", '', $xpath->query('//textarea[@name="election_positions"]')->item(0)->textContent)));
    $this->assertSame('2', $xpath->query('//select[@name="election_position_count_setting_lead"]/option[@selected]')->item(0)->getAttribute('value'));
    $this->assertSame(1, $xpath->query('//textarea[@name="election_position_desc_social_lead"]')->length);
  }

  public function testSettingsNeedPermission() {
    $this->login($this->site->createUser(array('lead')));
    $this->get('admin/rpg/rpg_election');
    $this->assertStatus(403);
  }

  public function testNewPositionAddsOptInToActiveCandidates() {
    $existing = $this->candidate();
    $this->adminLogin();
    $this->submitForm('admin/rpg/rpg_election', array('election_positions' => "Setting Lead\nSocial Lead\nSystem Lead"), 'Save configuration');
    $this->login($existing);
    $this->get('user/' . $existing->uid . '/edit');
    $this->assertSame(1, $this->xpath()->query('//input[starts-with(@name, "field_staff_system_lead") and @checked]')->length, 'Opted in by default');
  }

  public function testOnlyCandidatesSeeTheOptInFields() {
    $candidate = $this->candidate();
    $this->login($candidate);
    $this->get('user/' . $candidate->uid . '/edit');
    $this->assertGreaterThan(0, $this->xpath()->query('//*[starts-with(@name, "field_staff_setting_lead")]')->length);

    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('user/' . $player->uid . '/edit');
    $this->assertSame(0, $this->xpath()->query('//*[starts-with(@name, "field_staff_")]')->length);
  }

  // ---------------------------------------------------------------------------
  // Opening an election.
  // ---------------------------------------------------------------------------

  public function testOffCycleElectionCreatesBallot() {
    $a = $this->candidate();
    $b = $this->candidate();
    $this->adminLogin();
    $this->submitForm('admin/rpg/rpg_election', array('election_position_desc_social_lead' => 'Runs the social team.'), 'Save configuration');

    $election = $this->holdElection();

    $this->assertSame(date('m/d/Y') . ' Site Election: ' . date('Y'), $election->title);
    $ballot = $this->ballot($election);
    $this->assertSame(array('setting_lead_a', 'setting_lead_b', 'social_lead_a'), array_keys($ballot), 'Two Setting seats, one Social seat');
    $this->assertContains($a->name, $ballot['setting_lead_a']);
    $this->assertContains($b->name, $ballot['setting_lead_a']);
    $text = $this->nodeText($election);
    $this->assertStringContainsString('Runs the social team.', $text);
    $this->assertStringContainsString('Additional Comments', $text);
  }

  public function testOffCycleElectionVacatesSeatsToAdmin() {
    $sitting = $this->site->createUser(array('lead'));
    $this->candidate();
    $this->holdElection();
    $this->assertFalse($this->hasRole($sitting, 'lead'), 'Sitting leads lose the seat');
    $this->assertTrue($this->hasRole($this->superuser(), 'lead'), 'Admin holds it meanwhile');
  }

  public function testCandidatesExcludeOptedOutAndNonRunners() {
    $in = $this->candidate();
    $optedOut = $this->candidate(array('setting_lead' => 0, 'social_lead' => 1));
    $player = $this->site->createUser(array('player'));

    $ballot = $this->ballot($this->holdElection());

    $this->assertContains($in->name, $ballot['setting_lead_a']);
    $this->assertNotContains($optedOut->name, $ballot['setting_lead_a'], 'Opted out of Setting Lead');
    $this->assertContains($optedOut->name, $ballot['social_lead_a'], 'Still runs for Social Lead');
    $this->assertNotContains($player->name, $ballot['setting_lead_a']);
  }

  public function testTermLimitedCandidatesAreLeftOff() {
    $veteran = $this->candidate();
    $fresh = $this->candidate();
    // Held Setting Lead for the last two cycles (limit 2).
    $this->site->recordElectionWin($veteran, 'setting_lead', 1);
    $this->site->recordElectionWin($veteran, 'setting_lead', 2);

    $ballot = $this->ballot($this->holdElection());

    $this->assertNotContains($veteran->name, $ballot['setting_lead_a']);
    $this->assertContains($veteran->name, $ballot['social_lead_a'], 'Limit is per position');
    $this->assertContains($fresh->name, $ballot['setting_lead_a']);
  }

  public function testNoCandidatesCancelsElection() {
    $this->adminLogin();
    $this->submitForm('admin/rpg/rpg_election', array(), 'Hold an election off-cycle');
    $this->assertContains('This rpg_election has no candidates for ANY position. It has been automatically closed without runoff.', $this->messages('error'));
    $this->assertStringEndsWith('(Cancelled)', $this->elections()[0]->title);
  }

  // ---------------------------------------------------------------------------
  // Counting and closing.
  // ---------------------------------------------------------------------------

  /**
   * Setting Lead: s1 3 votes, s2 3, s3 1, s4 1. Social Lead: s3 3, s2 1.
   */
  private function runElection() {
    $s = array();
    for ($i = 1; $i <= 4; $i++) {
      $s[$i] = $this->candidate();
    }
    $voters = array();
    for ($i = 0; $i < 4; $i++) {
      $voters[] = $this->site->createUser(array('player'));
    }
    $election = $this->holdElection();
    $this->vote($voters[0], $election, array('setting_lead_a' => $s[1]->name, 'setting_lead_b' => $s[2]->name, 'social_lead_a' => $s[3]->name));
    $this->vote($voters[1], $election, array('setting_lead_a' => $s[1]->name, 'setting_lead_b' => $s[2]->name, 'social_lead_a' => $s[3]->name));
    $this->vote($voters[2], $election, array('setting_lead_a' => $s[1]->name, 'setting_lead_b' => $s[3]->name, 'social_lead_a' => $s[3]->name));
    $this->vote($voters[3], $election, array('setting_lead_a' => $s[2]->name, 'setting_lead_b' => $s[4]->name, 'social_lead_a' => $s[2]->name));
    return array($election, $s);
  }

  public function testCheckVotesReportsTallies() {
    list(, $s) = $this->runElection();
    $this->adminLogin();
    $this->submitForm('admin/rpg/rpg_election', array(), 'Check open election votes');
    $messages = $this->messages('status');
    $this->assertContains('Votes for position setting_lead.', $messages);
    $this->assertContains('3 votes for ' . $s[1]->name . '.', $messages);
    $this->assertContains('3 votes for ' . $s[2]->name . '.', $messages);
  }

  public function testClosingElectsWinners() {
    list($election, $s) = $this->runElection();

    $this->closeElections();

    $this->assertStringContainsString('This proposal is closed.', $this->nodeText($election));
    $this->assertTrue($this->hasRole($s[1], 'lead'));
    $this->assertTrue($this->hasRole($s[2], 'lead'));
    $this->assertTrue($this->hasRole($s[3], 'lead'), 'Social Lead');
    $this->assertFalse($this->hasRole($s[4], 'lead'), 'One vote is under the minimum of 2');
    $this->assertFalse($this->hasRole($this->superuser(), 'lead'), 'Admin steps down once there are winners');
  }

  public function testTwoTermsInARowReachTheLimit() {
    $veteran = $this->candidate();
    $other = $this->candidate();
    $voters = array($this->site->createUser(array('player')), $this->site->createUser(array('player')));
    // Two elections in a row that the veteran wins as Setting Lead.
    for ($term = 1; $term <= 2; $term++) {
      $this->site->removeRoleFromEveryone('lead');
      $this->site->closeAllProposals();
      $election = $this->holdElection();
      foreach ($voters as $voter) {
        $this->vote($voter, $election, array('setting_lead_a' => $veteran->name, 'setting_lead_b' => $other->name, 'social_lead_a' => $other->name));
      }
      $this->closeElections();
      $this->assertTrue($this->hasRole($veteran, 'lead'), "Won term $term");
    }
    $this->site->removeRoleFromEveryone('lead');
    $this->site->closeAllProposals();
    // Someone new, so the Setting Lead race still has a candidate.
    $fresh = $this->candidate();

    $ballot = $this->ballot($this->holdElection());

    $this->assertContains($fresh->name, $ballot['setting_lead_a']);
    $this->assertNotContains($veteran->name, $ballot['setting_lead_a'], 'Term limit (2) reached');
    $this->assertContains($veteran->name, $ballot['social_lead_a'], 'Limit is per position');
  }

  // ---------------------------------------------------------------------------
  // Election day (cron).
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * On election day _rpg_election_cron_election() runs
   * "DELETE ... WHERE rid = :rid" with an extra :uid argument (from an undefined
   * $winner): PDOException, no election. election_day has already been moved to
   * today, so the yearly election is silently skipped until next year.
   */
  public function testCronOpensElectionOnElectionDay() {
    $this->candidate();
    $this->site->setConfig('election_day', date('m/d/Y', strtotime('-1 year')));
    $this->site->setConfig('election_rate', '1 year');
    $this->site->resetDailyCron('election_cron');
    $before = count($this->elections());
    $this->runCron();
    $this->assertSame($before + 1, count($this->elections()));
  }

  public function testANewSiteStartsTheElectionClockAtItsFirstCron() {
    // The release ships no last election day (the original's 2013 date would
    // never come round again; review, 2026-11-26).
    $this->candidate();
    $this->site->setConfig('election_day', '');
    $this->site->setConfig('election_rate', '1 year');
    $this->site->resetDailyCron('election_cron');
    $before = count($this->elections());
    $this->runCron();
    $this->assertSame($before, count($this->elections()), 'No election yet');
    $this->assertSame(date('m/d/Y'), $this->site->stateValue('vaxia_governance.election_day'));
  }

  public function testAMissedElectionDayOpensItAtTheNextCron() {
    // No cron on the day itself (no visitors): the next cron opens it
    // (review, 2026-11-26).
    $this->candidate();
    $this->site->setConfig('election_day', date('m/d/Y', strtotime('-1 year -3 days')));
    $this->site->setConfig('election_rate', '1 year');
    $this->site->resetDailyCron('election_cron');
    $before = count($this->elections());
    $this->runCron();
    $this->assertSame($before + 1, count($this->elections()));
    // The day it was due, not the day it ran: later elections keep their day.
    $this->assertSame(date('m/d/Y', strtotime('+1 year', strtotime(date('m/d/Y', strtotime('-1 year -3 days'))))), $this->site->stateValue('vaxia_governance.election_day'));
  }

  public function testCronDoesNothingOnOtherDays() {
    $this->candidate();
    $this->site->setConfig('election_day', date('m/d/Y', strtotime('-200 days')));
    $this->site->setConfig('election_rate', '1 year');
    $this->site->resetDailyCron('election_cron');
    $before = count($this->elections());
    $this->runCron();
    $this->assertSame($before, count($this->elections()));
  }

  // ---------------------------------------------------------------------------
  // Known bugs.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * With no election forum configured the election proposal is saved with
   * taxonomy_forums tid '' and node_save fails (SQL strict mode): no election.
   */
  public function testElectionWithoutForumConfigured() {
    $this->site->deleteConfig('election_default_forum');
    $this->candidate();
    $before = count($this->elections());
    $this->adminLogin();
    $this->submitForm('admin/rpg/rpg_election', array(), 'Hold an election off-cycle');
    $this->assertSame($before + 1, count($this->elections()));
  }

  /**
   * @group knownbug
   * Profile edit warns for every user while "Election run role" is unset
   * (foreach over ''), e.g. on a fresh site before elections are configured.
   */
  public function testProfileEditWithoutElectionSettingsHasNoWarnings() {
    $this->site->deleteConfig('election_run_roles');
    $account = $this->site->createUser();
    $this->login($account);
    $this->get('user/' . $account->uid . '/edit');
    $this->assertSame(array(), $this->pageErrorsFrom('rpg_election.module'));
  }

  /**
   * @group knownbug
   * Opening and closing print PHP notices: _rpg_election_trigger_election()
   * writes to an undefined $node (fatal "Attempt to assign property on null" on
   * PHP 8), results use undefined indexes, and the close_election event is
   * invoked with an undefined $wins.
   */
  public function testElectionRunsWithoutPhpErrors() {
    $this->runElection();
    $this->closeElections();
    $mine = array_filter($this->pageErrorsFrom('rpg_election.module'), function ($e) {
      return strpos($e, 'user_profile_form_alter') === FALSE;
    });
    $this->assertSame(array(), array_values($mine));
  }

}
