<?php

namespace Vaxia\Tests\Storyhosts\StoryhostDuties;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Storyhost duties: a "Storyhost Activity" table (user-activity) shows, per
 * storyhost with a storyhost character, their past year's work: prompt character/item/scene evaluations
 * (within 3 days of creation), wiki pages created and revised, replies in the
 * "SH Requests" forums, and unread private messages. Evaluating characters and
 * items pushes the storyhost's powers expiry out to six months; cron is meant
 * to retire storyhosts whose powers lapse.
 *
 * @group storyhost_duties
 */
class StoryhostDutiesTest extends VaxiaTestCase {

  /**
   * The activity row for $account: column label => cell text.
   */
  private function activityOf($account) {
    return $this->inSession($this->site->createUser(array('lead')), function () use ($account) {
      $this->get('user-activity');
      $this->assertStatus(200);
      $xpath = $this->xpath();
      $labels = array();
      foreach ($xpath->query('//table//thead//th') as $th) {
        $labels[] = trim(preg_replace('/\s+/', ' ', $th->textContent));
      }
      foreach ($xpath->query('//table//tbody/tr') as $tr) {
        if (strpos($tr->textContent, $account->name) === FALSE) {
          continue;
        }
        $row = array();
        foreach ($xpath->query('./td', $tr) as $i => $td) {
          $row[$labels[$i] ?? $i] = trim(preg_replace('/\s+/', ' ', $td->textContent));
        }
        return $row;
      }
      return NULL;
    });
  }

  /**
   * A storyhost with a storyhost character (the table only lists those).
   */
  private function storyhost(array $values = array()) {
    $sh = $this->site->createUser(array('player', 'storyteller'), array('write privatemsg', 'read privatemsg'), $values);
    $character = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Sh')));
    $this->site->setUserFields($sh, array('field_storyhost_character' => $character->nid));
    return $sh;
  }

  /**
   * A player's character, submitted for evaluation.
   */
  private function submittedCharacter() {
    $owner = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($owner);
    $this->transition($character, 'draft', $owner);
    $this->transition($character, 'submitted', $owner);
    return $character;
  }

  private function powersExpireOn($account) {
    preg_match('/Powers expire:?\s*(\w+, \w+ \d+, \d{4})/', $this->profileText($account), $m);
    return $m[1] ?? NULL;
  }

  // ---------------------------------------------------------------------------
  // The activity table.
  // ---------------------------------------------------------------------------

  public function testStoryhostsAreListedPlayersAreNot() {
    $sh = $this->storyhost();
    $player = $this->site->createUser(array('player'));
    $this->assertNotNull($this->activityOf($sh));
    $this->assertNull($this->activityOf($player));
  }

  public function testCharacterEvaluationsAreCounted() {
    $sh = $this->storyhost();
    $this->transition($this->submittedCharacter(), 'approved', $sh);
    $this->transition($this->submittedCharacter(), 'approved', $sh);
    $this->assertSame('2', $this->activityOf($sh)['Characters Eval']);
  }

  public function testItemEvaluationsAreCounted() {
    $sh = $this->storyhost();
    $item = $this->site->createNode('items', array('uid' => $this->site->createUser(array('player'))->uid));
    $this->transition($item, 'draft');
    $this->transition($item, 'submitted');
    $this->transition($item, 'approved', $sh);
    $this->assertSame('1', $this->activityOf($sh)['Items Eval']);
  }

  public function testWikiPagesCreatedAndRevisedAreCounted() {
    $sh = $this->storyhost();
    $page = $this->site->createNode('wiki_page', array('uid' => $sh->uid, 'title' => $this->site->uniqueName('Wiki')));
    $this->site->saveNode($page, array('revision' => TRUE, 'log' => 'edit'), $sh);
    $activity = $this->activityOf($sh);
    $this->assertSame('1', $activity['Created Wiki']);
    $this->assertGreaterThanOrEqual(1, (int) $activity['Revised Wiki']);
  }

  public function testRepliesInShRequestForumsAreCounted() {
    // Production's "SH Requests" forums are terms 10 and 11.
    $forum = $this->site->createTermAt('forums', 10, 'SH Requests');
    $topic = $this->site->createNode('forum', array('title' => $this->site->uniqueName('Request'), 'taxonomy_forums' => $forum));
    $sh = $this->storyhost();
    $this->site->postComment($topic, $sh, 'On it!');
    $this->assertSame('1', $this->activityOf($sh)['Replies on SH Requests']);
  }

  public function testUnreadPrivateMessagesAreCounted() {
    $sh = $this->storyhost();
    $this->login($this->site->createUser(array('player'), array('write privatemsg')));
    $this->submitForm('messages/new', array('recipient' => $this->referenceValue($sh->name, $sh->uid, 'user'), 'subject' => 'Help', 'body[value]' => 'Please run a session'), 'Send message');

    $activity = $this->activityOf($sh);

    $this->assertSame('1', $activity['Count Unread PM Threads']);
    $this->assertNotSame('all read', $activity['Oldest Unread PM']);
  }

  public function testNoUnreadMessagesReadsAllRead() {
    $this->assertSame('all read', $this->activityOf($this->storyhost())['Oldest Unread PM']);
  }

  /**
   * @group knownbug
   * "Prompt" evaluation compares workflow_node_history.stamp (a Unix time) with
   * DATE_ADD(FROM_UNIXTIME(created), +3 DAY) (a DATETIME, which MySQL compares
   * as 20261123...), so it's always true: late evaluations count as prompt.
   */
  public function testLateEvaluationsAreNotCounted() {
    $sh = $this->storyhost();
    $character = $this->submittedCharacter();
    $this->site->backdateNodeCreated($character, strtotime('-10 days'));
    $this->transition($character, 'approved', $sh);
    $this->assertSame('0', $this->activityOf($sh)['Characters Eval']);
  }

  // ---------------------------------------------------------------------------
  // Powers expiry.
  // ---------------------------------------------------------------------------

  public function testEvaluatingACharacterExtendsPowersToSixMonths() {
    $sh = $this->storyhost(array('field_powers_expire' => date('Y-m-d 00:00:00', strtotime('+1 month'))));
    $this->transition($this->submittedCharacter(), 'approved', $sh);
    $this->assertSame(date('l, F j, Y', strtotime('+6 months')), $this->powersExpireOn($sh));
  }

  public function testEvaluatingAnItemExtendsPowersToSixMonths() {
    $sh = $this->storyhost(array('field_powers_expire' => date('Y-m-d 00:00:00', strtotime('+1 month'))));
    $item = $this->site->createNode('items', array('uid' => $this->site->createUser(array('player'))->uid));
    $this->transition($item, 'draft');
    $this->transition($item, 'submitted');
    $this->transition($item, 'approved', $sh);
    $this->assertSame(date('l, F j, Y', strtotime('+6 months')), $this->powersExpireOn($sh));
  }

  public function testLongerExpiryIsKept() {
    $later = strtotime('+9 months');
    $sh = $this->storyhost(array('field_powers_expire' => date('Y-m-d 00:00:00', $later)));
    $this->transition($this->submittedCharacter(), 'approved', $sh);
    $this->assertSame(date('l, F j, Y', $later), $this->powersExpireOn($sh));
  }

  /**
   * @group knownbug
   * "Remove SH roles if Neglect Duties" listens for user_stats_day_older, which
   * can't deliver a user under Rules 2 (see suspend_players): lapsed storyhosts
   * keep their roles.
   */
  public function testCronRetiresStoryhostsWhosePowersLapsed() {
    $sh = $this->storyhost(array('field_powers_expire' => date('Y-m-d 00:00:00', strtotime('-1 week'))));
    $this->site->backdateAccountCreated($sh, strtotime('-3 days'));
    $this->site->setLastCron(strtotime('-1 day'));
    $this->runCron();
    $this->assertNotContains('storyteller', $this->rolesOf($sh));
  }

}
