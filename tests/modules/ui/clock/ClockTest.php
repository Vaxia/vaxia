<?php

namespace Vaxia\Tests\Ui\Clock;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The clock block (D7 contrib clock, block clock/clock) as production set it
 * up: the "long" date type, in the visitor's own time zone ("User time
 * zone"), ticking every second in the browser (clock.js gets the format and
 * the zone's offset).
 *
 * The D7 test site doesn't have the clock module, so these run on the port
 * only.
 *
 * @group clock
 */
class ClockTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have the clock module.');
    }
    $this->site->placeBlock('clock', 'clock', 'sidebar_first');
  }

  /**
   * The clock settings passed to clock.js on the last page.
   */
  private function clockSettings() {
    $json = $this->xpath()->query('//script[@data-drupal-selector="drupal-settings-json"]')->item(0);
    $this->assertNotNull($json, 'drupalSettings on the page');
    $settings = json_decode($json->textContent, TRUE);
    $this->assertArrayHasKey('clock', $settings);
    return $settings['clock'];
  }

  private function clockText() {
    $nodes = $this->xpath()->query('//div[@class="clock"]');
    $this->assertSame(1, $nodes->length, 'One clock on the page');
    return trim($nodes->item(0)->textContent);
  }

  /**
   * The clock text for "now" in $zone, allowing for a minute boundary between
   * the request and the check.
   */
  private function assertShowsNowIn($zone, $pattern, $text, $before) {
    $expected = array();
    foreach (array($before, time()) as $moment) {
      $date = new \DateTime('@' . $moment);
      $date->setTimezone(new \DateTimeZone($zone));
      $expected[] = $date->format($pattern);
    }
    $this->assertContains($text, $expected);
  }

  public function testShowsTheTimeInTheUsersOwnZone() {
    $player = $this->site->createUser(array('player'));
    $this->site->setUserFields($player, array('timezone' => 'Pacific/Auckland'));
    $this->login($player);
    $before = time();
    $this->get('user/' . $player->uid);
    $settings = $this->clockSettings();
    $this->assertSame('Pacific/Auckland', $settings['time_zone']);
    $date = new \DateTime('@' . $before);
    $date->setTimezone(new \DateTimeZone('Pacific/Auckland'));
    $this->assertSame($date->format('Z'), (string) $settings['offset_seconds']);
    $this->assertSame($date->format('T'), $settings['offset_name']);
    $this->assertSame($date->format('I'), (string) $settings['daylight_savings_time']);
    $this->assertShowsNowIn('Pacific/Auckland', $settings['date_format'], $this->clockText(), $before);
    $this->assertNoPhpErrors();
  }

  public function testUsesTheLongDateTypeAndTicks() {
    $this->login($this->site->createUser(array('player')));
    $this->get('node');
    $settings = $this->clockSettings();
    // The long date type: weekday, day, month name, year and the time.
    $this->assertMatchesRegularExpression('/l/', $settings['date_format']);
    $this->assertMatchesRegularExpression('/Y/', $settings['date_format']);
    $this->assertMatchesRegularExpression('/H:i|g:ia/', $settings['date_format']);
    $this->assertSame(1, (int) $settings['update']);
    $this->assertSame(0, (int) $settings['local']);
    $this->assertRaw('/js/clock.js');
  }

  public function testOtherUsersSeeTheirOwnZone() {
    // Not one cached clock for everyone.
    $east = $this->site->createUser(array('player'));
    $this->site->setUserFields($east, array('timezone' => 'Asia/Tokyo'));
    $west = $this->site->createUser(array('player'));
    $this->site->setUserFields($west, array('timezone' => 'America/Los_Angeles'));
    $this->login($east);
    $this->get('node');
    $this->assertSame('Asia/Tokyo', $this->clockSettings()['time_zone']);
    $this->login($west);
    $this->get('node');
    $this->assertSame('America/Los_Angeles', $this->clockSettings()['time_zone']);
  }

  public function testAnonymousSeesTheSiteZone() {
    $this->anonymous();
    $before = time();
    $this->get('user/login');
    $settings = $this->clockSettings();
    $this->assertShowsNowIn($settings['time_zone'], $settings['date_format'], $this->clockText(), $before);
  }

}
