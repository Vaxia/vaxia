<?php

namespace Vaxia\Tests\Events\TimelineEvents;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Timeline events: a wiki timeline (wiki/timelines, TimelineJS) of published
 * timeline events and session reports of one realm (Vaxia or Sirian, Vaxia
 * first), placed by their "session held" date.
 *
 * @group timeline_events
 */
class TimelineEventsTest extends VaxiaTestCase {

  private function timelineEvent(array $values = array()) {
    return $this->site->createNode('timeline_event', $values + array(
      'title' => $this->site->uniqueName('Battle'),
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
      'field_session_held' => array(array('value' => '2015-06-01 12:00:00', 'value2' => '2015-06-01 14:00:00')),
    ));
  }

  public function testTimelineShowsEventsAndSessions() {
    $event = $this->timelineEvent();
    $session = $this->site->createNode('session_report', array(
      'title' => $this->site->uniqueName('Session'),
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
      'field_session_held' => array(array('value' => '2016-02-01 12:00:00', 'value2' => '2016-02-01 14:00:00')),
    ));
    $this->login($this->site->createUser(array('player')));
    $this->get('wiki/timelines');
    $this->assertStatus(200);
    $this->assertRaw($event->title);
    $this->assertRaw($session->title);
  }

  public function testOneRealmAtATime() {
    $vaxia = $this->timelineEvent();
    $sirian = $this->timelineEvent(array('field_tag_realm' => $this->site->term('realm', 'Sirian')));
    $this->login($this->site->createUser(array('player')));
    $this->get('wiki/timelines', array('field_tag_realm_tid' => $this->site->term('realm', 'Sirian')));
    $this->assertRaw($sirian->title);
    $this->assertNoRaw($vaxia->title);
  }

  public function testOtherRealmsAndUnpublishedAreLeftOut() {
    $storyhost = $this->timelineEvent(array('field_tag_realm' => $this->site->term('realm', 'Storyhost')));
    $draft = $this->timelineEvent(array('status' => 0));
    $this->login($this->site->createUser(array('player')));
    $this->get('wiki/timelines');
    $this->assertNoRaw($storyhost->title);
    $this->assertNoRaw($draft->title);
  }

}
