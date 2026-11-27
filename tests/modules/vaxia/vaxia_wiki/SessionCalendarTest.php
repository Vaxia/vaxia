<?php

namespace Vaxia\Tests\Vaxia\VaxiaWiki;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Session calendars: sessions/calendar/<realm> (the realm's name in lower
 * case with dashes: fantasy, science-fiction) are iCalendar feeds of one
 * realm's published session reports, one event per
 * report on its "session held" date, latest session first. Anyone who may
 * view content may fetch them.
 *
 * D11 only: the D7 test site doesn't have the wiki_display_sessions feeds.
 *
 * @group vaxia_wiki
 */
class SessionCalendarTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The session calendar feeds were ported to the D11 site only.');
    }
  }

  private function report($realm, $held, array $values = array()) {
    return $this->site->createNode('session_report', $values + array(
      'title' => $this->site->uniqueName('Session'),
      'field_tag_realm' => $this->site->term('realm', $realm),
      'field_session_held' => $held,
    ));
  }

  /**
   * Fetches a feed anonymously; returns its SUMMARY lines.
   */
  private function summaries($realm) {
    $this->anonymous();
    $this->get('sessions/calendar/' . $realm);
    $this->assertStatus(200);
    preg_match_all('/^SUMMARY:(.*)$/m', str_replace("\r", '', $this->content), $m);
    return $m[1];
  }

  public function testFeedIsAnICalendarNamedForTheRealm() {
    $this->site->term('realm', 'Fantasy');
    $this->site->term('realm', 'Science Fiction');
    $this->summaries('fantasy');
    $this->assertStringStartsWith('BEGIN:VCALENDAR', $this->content);
    $this->assertStringContainsString('X-WR-CALNAME:Fantasy Sessions', $this->content);
    $this->assertStringContainsString('END:VCALENDAR', $this->content);
    $this->summaries('science-fiction');
    $this->assertStringContainsString('X-WR-CALNAME:Science Fiction Sessions', $this->content);
  }

  public function testEachRealmFeedListsOnlyItsOwnSessions() {
    $vaxia = $this->report('Fantasy', '2026-03-04 20:30:00');
    $sirian = $this->report('Science Fiction', '2026-03-05 20:30:00');
    $vaxiaFeed = $this->summaries('fantasy');
    $this->assertContains($vaxia->title, $vaxiaFeed);
    $this->assertNotContains($sirian->title, $vaxiaFeed);
    $sirianFeed = $this->summaries('science-fiction');
    $this->assertContains($sirian->title, $sirianFeed);
    $this->assertNotContains($vaxia->title, $sirianFeed);
  }

  public function testEventCarriesTheSessionDateAndLink() {
    $report = $this->report('Fantasy', '2026-03-04 20:30:00');
    $this->summaries('fantasy');
    $event = substr($this->content, strpos($this->content, 'SUMMARY:' . $report->title));
    $event = substr($event, 0, strpos($event, 'END:VEVENT'));
    $this->assertStringContainsString('DTSTART;VALUE=DATE:20260304', $event);
    $this->assertMatchesRegularExpression('#URL:http\S*/node/' . $report->nid . '\s#', $event);
  }

  public function testUnpublishedUndatedAndOtherContentAreLeftOut() {
    $unpublished = $this->report('Fantasy', '2026-03-04 20:30:00', array('status' => 0));
    $undated = $this->report('Fantasy', '', array('field_session_held' => array()));
    $other = $this->site->createNode('wiki_page', array('title' => $this->site->uniqueName('Lore'), 'field_tag_realm' => $this->site->term('realm', 'Vaxia')));
    $feed = $this->summaries('fantasy');
    $this->assertNotContains($unpublished->title, $feed);
    $this->assertNotContains($undated->title, $feed);
    $this->assertNotContains($other->title, $feed);
  }

  public function testLatestSessionComesFirst() {
    $older = $this->report('Fantasy', '2026-01-01 10:00:00');
    $newer = $this->report('Fantasy', '2026-02-01 10:00:00');
    $oldest = $this->report('Fantasy', '2025-12-01 10:00:00');
    $mine = array_values(array_intersect($this->summaries('fantasy'), array($older->title, $newer->title, $oldest->title)));
    $this->assertSame(array($newer->title, $older->title, $oldest->title), $mine);
  }

  public function testNoFeedForARealmThatDoesNotExist() {
    $this->anonymous();
    $this->get('sessions/calendar/no-such-realm');
    $this->assertStatus(404);
  }

  public function testNeedsPermissionToViewContent() {
    $this->withoutPermission('access content', array('anonymous user'), function () {
      $this->anonymous();
      $this->get('sessions/calendar/fantasy');
      $this->assertStatus(403);
    });
  }

  public function testEachRealmHasASessionList() {
    // wiki/<realm>/session-list for every realm (D7 had one each for its two
    // realms; user, 2026-11-26).
    $fantasy = $this->report('Fantasy', '2026-03-04 20:30:00');
    $horror = $this->report('Horror', '2026-03-05 20:30:00');
    $this->login($this->site->createUser(array('player')));
    $this->get('wiki/fantasy/session-list');
    $this->assertStatus(200);
    $this->assertText('Fantasy Sessions');
    $this->assertText($fantasy->title);
    $this->assertNoText($horror->title);
    $this->get('wiki/no-such-realm/session-list');
    $this->assertStatus(404);
  }

}
