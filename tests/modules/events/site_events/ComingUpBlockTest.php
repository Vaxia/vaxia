<?php

namespace Vaxia\Tests\Events\SiteEvents;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Calendar: Coming up" block (D7 view calendar_block, coming_up): the next
 * five published, scheduled events in the "Session announcements" forum
 * (production term 879) starting today or later, soonest first, each with its
 * start ("m/d/y h:ia").
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group site_events
 * @group views_blocks
 */
class ComingUpBlockTest extends VaxiaTestCase {

  const FORUM = 879;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->createTermAt('forums', self::FORUM, 'Session announcements');
    $this->site->placeBlock('views', 'calendar_block-coming_up', 'sidebar_first');
  }

  /**
   * The block's rows on the front page, or NULL when it isn't shown.
   */
  private function rows() {
    $this->get('node');
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-coming_up ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query('.//li', $view->item(0)) as $li) {
      $rows[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
    }
    return $rows;
  }

  /**
   * Titles of the rows (the date cut off).
   */
  private function titles() {
    return array_map(function ($row) {
      return preg_replace('# \d\d/\d\d/\d\d \d\d:\d\d[ap]m$#', '', $row);
    }, (array) $this->rows());
  }

  private function event($start, $state = 'scheduled', array $values = array()) {
    $event = $this->site->createNode('event', $values + array(
      'uid' => $this->site->createUser(array('player'))->uid,
      'title' => $this->site->uniqueName('Session'),
      'taxonomy_forums' => self::FORUM,
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
      'field_when' => array(array('value' => gmdate('Y-m-d H:i:s', $start), 'value2' => gmdate('Y-m-d H:i:s', $start + 7200))),
    ));
    if ($state) {
      $this->site->forceWorkflowState($event, $state);
    }
    return $event;
  }

  public function testUpcomingSessions() {
    $event = $this->event(time() + 2 * 86400);
    $this->anonymous();
    $rows = $this->rows();
    $this->assertContains($event->title, $this->titles());
    $row = $rows[array_search($event->title, $this->titles())];
    $this->assertMatchesRegularExpression('#^' . preg_quote($event->title, '#') . ' \d\d/\d\d/\d\d \d\d:\d\d[ap]m$#', $row);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-coming_up")]//a[@href="/node/' . $event->nid . '"]')->length);
    $this->login($this->site->createUser(array('player')));
    $this->assertContains($event->title, $this->titles());
  }

  public function testOnlyScheduledSessionAnnouncementsAhead() {
    $past = $this->event(time() - 2 * 86400);
    $cancelled = $this->event(time() + 86400);
    $this->site->forceWorkflowState($cancelled, 'cancelled');
    $unscheduled = $this->event(time() + 86400, NULL);
    $otherForum = $this->event(time() + 86400, 'scheduled', array('taxonomy_forums' => $this->site->term('forums', $this->site->uniqueName('Forum'))));
    $unpublished = $this->event(time() + 86400, 'scheduled', array('status' => 0));
    $shown = $this->event(time() + 86400);
    $this->anonymous();
    $titles = $this->titles();
    $this->assertContains($shown->title, $titles);
    foreach (array($past, $cancelled, $unscheduled, $otherForum, $unpublished) as $event) {
      $this->assertNotContains($event->title, $titles);
    }
  }

  public function testNextFiveSoonestFirst() {
    $this->site->deleteNodesOfType('event');
    $events = array();
    foreach (array(5, 2, 6, 1, 4, 3) as $days) {
      $events[$days] = $this->event(time() + $days * 86400);
    }
    $this->anonymous();
    $this->assertSame(array($events[1]->title, $events[2]->title, $events[3]->title, $events[4]->title, $events[5]->title), $this->titles());
  }

  public function testHiddenWhenNothingComingUp() {
    $this->site->deleteNodesOfType('event');
    $this->anonymous();
    $this->assertNull($this->rows());
  }

  public function testHiddenWithoutAccessContent() {
    $this->event(time() + 86400);
    $this->withoutPermission('access content', array('anonymous user'), function () {
      $this->anonymous();
      $this->get('user/login');
      $this->assertSame(0, $this->xpath()->query('//div[contains(@class, "view-display-id-coming_up")]')->length);
    });
  }

}
