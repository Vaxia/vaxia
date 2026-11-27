<?php

namespace Vaxia\Tests\Storyhosts\Storyhosts;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Moderation log" block (D7 view vaxia_moderation_log,
 * moderation_log_block): on a character sheet's page, for staff (admin, lead,
 * storyteller, assistant, candidate) who may edit that sheet, its ten latest
 * log lines except daily silver: type, who, when, message.
 *
 * The site logs on its own too (sheet created, ...), so test lines are dated
 * in the future to be the newest.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group storyhosts
 * @group views_blocks
 */
class ModerationLogBlockTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'vaxia_moderation_log-moderation_log_block', 'content');
  }

  /**
   * Table rows of the block on a page, or NULL when it isn't shown.
   */
  private function rows($path) {
    $this->get($path);
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-moderation_log_block ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query('.//tbody/tr', $view->item(0)) as $tr) {
      $cells = array();
      foreach ($xpath->query('./td', $tr) as $td) {
        $cells[] = trim($td->textContent);
      }
      $rows[] = $cells;
    }
    return $rows;
  }

  /** @var object Owner of the last character made. */
  private $owner;

  private function character() {
    $this->owner = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($this->owner);
    $this->site->forceWorkflowState($character, 'approved');
    return $character;
  }

  public function testLatestLinesForStaff() {
    $character = $this->character();
    $moderator = $this->site->createUser(array('storyteller'));
    $base = time() + 1000;
    $this->site->addSheetLog($character, $moderator, 'xp', 'Older <b>line</b>', $base);
    $this->site->addSheetLog($character, $moderator, 'daily_silver', 'Daily pay', $base + 10);
    $this->site->addSheetLog($character, NULL, 'silver', 'Newer line', $base + 20);
    $this->login($this->site->createUser(array('storyteller')));
    $rows = $this->rows('node/' . $character->nid);
    $this->assertNotContains('Daily pay', array_column($rows, 3));
    $this->assertSame(array('silver', 'Anonymous', 'Newer line'), array($rows[0][0], $rows[0][1], $rows[0][3]));
    $this->assertSame(array('xp', $moderator->name, 'Older <b>line</b>'), array($rows[1][0], $rows[1][1], $rows[1][3]));
    $this->assertNotSame('', $rows[1][2], 'Has a timestamp');
    $this->assertSame(array('Log type', 'Logged user', 'Timestamp', 'Message'), array_map('trim', array_map(function ($th) {
      return $th->textContent;
    }, iterator_to_array($this->xpath()->query('//div[contains(@class, "view-display-id-moderation_log_block")]//thead//th')))));
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-moderation_log_block")]//a[@href="/user/' . $moderator->uid . '"]')->length, 'Logged user links to the account');
  }

  public function testTenNewestOnly() {
    $character = $this->character();
    for ($i = 1; $i <= 11; $i++) {
      $this->site->addSheetLog($character, NULL, 'xp', 'Line ' . $i, time() + 1000 + $i);
    }
    $this->login($this->site->createUser(array('storyteller-assistant')));
    $messages = array_column($this->rows('node/' . $character->nid), 3);
    $expected = array();
    for ($i = 11; $i >= 2; $i--) {
      $expected[] = 'Line ' . $i;
    }
    $this->assertSame($expected, $messages);
  }

  public function testOnlyStaffWhoMayEditTheSheet() {
    $character = $this->character();
    $this->site->addSheetLog($character, NULL, 'xp', 'A line', time() + 1000);
    $this->login($this->site->createUser(array('storyteller-candidate')));
    $this->assertSame('A line', $this->rows('node/' . $character->nid)[0][3]);
    $this->login($this->site->createUser(array('administrator')));
    $this->assertNotNull($this->rows('node/' . $character->nid));
    // The owner may edit it but isn't staff.
    $this->login($this->owner);
    $this->assertNull($this->rows('node/' . $character->nid));
    $this->login($this->site->createUser(array('player', 'storyteller-assistant')));
    $this->assertNotNull($this->rows('node/' . $character->nid));
  }

  public function testStaffWhoMayNotEditSeeNothing() {
    $character = $this->character();
    $this->site->addSheetLog($character, NULL, 'xp', 'A line', time());
    $this->withoutPermission('edit any character_sheet content', array('storyteller', 'authenticated user', 'player'), function () use ($character) {
      $this->login($this->site->createUser(array('storyteller')));
      $this->assertNull($this->rows('node/' . $character->nid));
    });
  }

  public function testOnlyOnCharacterSheets() {
    $this->login($this->site->createUser(array('storyteller')));
    $page = $this->site->createNode('page');
    $this->assertNull($this->rows('node/' . $page->nid));
    $this->assertNull($this->rows('node'));
  }

}
