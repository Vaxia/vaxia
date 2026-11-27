<?php

namespace Vaxia\Tests\Vaxia\VaxiaRpg;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Message of the Day" block (D7 view message_of_the_day): the body of the
 * published "Message of the Day" wiki page (production node 11918), for
 * everyone who may access content.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group vaxia_rpg
 * @group views_blocks
 */
class MessageOfTheDayBlockTest extends VaxiaTestCase {

  const NID = 11918;

  /** @var object|null The message page, made once per class. */
  private static $page;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'message_of_the_day-motd_block', 'sidebar_first');
    if (!self::$page) {
      self::$page = $this->site->createNode('wiki_page', array('nid' => self::NID, 'title' => 'Message of the Day'));
    }
  }

  public static function tearDownAfterClass(): void {
    self::$page = NULL;
    parent::tearDownAfterClass();
  }

  private function setMessage($text, $status = 1) {
    $this->site->saveNode(self::$page, array('status' => $status, 'body' => array(array('value' => $text, 'format' => 'filtered_html'))), $this->superuser());
  }

  /**
   * The block's text on the front page, or NULL when it isn't shown.
   */
  private function message() {
    $this->get('node');
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-motd_block ")]');
    return $view->length ? trim(preg_replace('/\s+/', ' ', $view->item(0)->textContent)) : NULL;
  }

  public function testShowsTheMessageToEveryone() {
    $message = $this->site->uniqueName('Welcome back');
    $this->setMessage('<p><strong>' . $message . '</strong></p>');
    $this->anonymous();
    $this->assertSame($message, $this->message());
    $this->assertRaw('<strong>' . $message . '</strong>');
    $this->login($this->site->createUser(array('player')));
    $this->assertSame($message, $this->message());
  }

  public function testUpdatesWhenThePageChanges() {
    $this->setMessage('First ' . $this->site->uniqueName());
    $this->anonymous();
    $this->message();
    $second = 'Second ' . $this->site->uniqueName();
    $this->setMessage($second);
    $this->assertSame($second, $this->message());
  }

  public function testHiddenWhenUnpublished() {
    $this->setMessage('Hidden ' . $this->site->uniqueName(), 0);
    $this->anonymous();
    $this->assertNull($this->message());
    $this->login($this->site->createUser(array('player')));
    $this->assertNull($this->message());
  }

  public function testOtherWikiPagesDoNotShow() {
    $this->setMessage('Today ' . $this->site->uniqueName());
    $other = $this->site->createNode('wiki_page', array('body' => array(array('value' => 'Other page text', 'format' => 'filtered_html'))));
    $this->anonymous();
    $this->assertStringNotContainsString('Other page text', $this->message());
  }

  public function testHiddenWithoutAccessContent() {
    $this->setMessage('Members only ' . $this->site->uniqueName());
    $this->withoutPermission('access content', array('anonymous user'), function () {
      $this->anonymous();
      $this->get('user/login');
      $this->assertSame(0, $this->xpath()->query('//div[contains(@class, "view-display-id-motd_block")]')->length);
    });
  }

}
