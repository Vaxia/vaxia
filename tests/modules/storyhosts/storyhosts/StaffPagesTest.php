<?php

namespace Vaxia\Tests\Storyhosts\Storyhosts;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Two staff pages from D7 views:
 * - admin/content/storyhost-characters ("SH characters" tab, view
 *   storyhost_characters): published storyhost characters, newest first, with
 *   their player, the player's listed SH character and roles, filterable by
 *   the player's roles. For those who may administer content.
 * - character-sheets/moderation_log (view vaxia_moderation_log): every
 *   character sheet log line, newest first, 35 a page, filterable; daily
 *   silver is left out to start with. For admins, leads, players and
 *   storyhosts.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group storyhosts
 */
class StaffPagesTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
  }

  private function rows($column) {
    $xpath = $this->xpath();
    $rows = array();
    foreach ($xpath->query('//table[.//th[contains(., "' . $column . '")]]/tbody/tr') as $tr) {
      $cells = array();
      foreach ($xpath->query('./td', $tr) as $td) {
        $cells[] = trim(preg_replace('/\s+/', ' ', $td->textContent));
      }
      if (count($cells) > 1) {
        $rows[] = $cells;
      }
    }
    return $rows;
  }

  public function testStoryhostCharactersAccess() {
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('admin/content/storyhost-characters');
    $this->assertStatus(403);
    $this->login($this->site->createUser(array(), array('administer nodes', 'access administration pages')));
    $this->get('admin/content/storyhost-characters');
    $this->assertStatus(200);
    $this->assertText('Storyhost characters');
  }

  public function testStoryhostCharactersList() {
    $sh = $this->site->createUser(array('storyteller'));
    $old = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Shade'), 'field_is_storyteller' => 1));
    $this->site->backdateNodeCreated($old, time() - 600);
    $new = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Shade'), 'field_is_storyteller' => 1));
    // Explicit: in-process nodes default to the PHPUnit process's request
    // time, which a long suite run leaves minutes behind.
    $this->site->backdateNodeCreated($new, time() - 100);
    $pc = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Hero')));
    $unpublished = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Shade'), 'field_is_storyteller' => 1, 'status' => 0));
    // (Creating a storyhost character lists it on its player; list the old one.)
    $this->site->setUserFields($sh, array('field_storyhost_character' => $old->nid));

    $this->login($this->site->createUser(array('administrator')));
    $this->get('admin/content');
    $this->assertRaw('href="/admin/content/storyhost-characters"');
    $this->get('admin/content/storyhost-characters', array('rid' => array('storyteller'), 'rid_op' => 'or'));
    $this->assertStatus(200);
    $mine = array_values(array_filter($this->rows('Listed SH Character'), function ($row) use ($sh) {
      return $row[0] === $sh->name;
    }));
    // "[titles] [title]" (the site gives storyhost characters their titles).
    $this->assertCount(2, $mine);
    $this->assertStringEndsWith(' ' . $new->title, $mine[0][3]);
    $this->assertStringEndsWith(' ' . $old->title, $mine[1][3]);
    $this->assertSame($old->title, $mine[0][1], 'The player\'s listed SH character');
    $this->assertStringContainsString('storyteller', strtolower($mine[0][2]));
    $this->assertSame('edit', $mine[0][4]);
    $this->assertSame('delete', $mine[0][5]);
    $this->assertNoText($pc->title);
    $this->assertNoText($unpublished->title);
  }

  public function testStoryhostCharactersRoleFilter() {
    $sh = $this->site->createUser(array('storyteller'));
    $lead = $this->site->createUser(array('lead'));
    $sh_char = $this->site->createCharacter($sh, array('field_is_storyteller' => 1));
    $lead_char = $this->site->createCharacter($lead, array('field_is_storyteller' => 1));
    $this->login($this->site->createUser(array('administrator')));

    $this->get('admin/content/storyhost-characters', array('rid' => array('lead'), 'rid_op' => 'or'));
    $this->assertText($lead_char->title);
    $this->assertNoText($sh_char->title);

    $this->get('admin/content/storyhost-characters', array('rid' => array('lead', 'storyteller'), 'rid_op' => 'and'));
    $this->assertNoText($lead_char->title);
    $this->assertNoText($sh_char->title);

    $this->get('admin/content/storyhost-characters', array('rid' => array('lead'), 'rid_op' => 'not'));
    $this->assertNoText($lead_char->title);
  }

  public function testModerationLogAccess() {
    foreach (array('administrator', 'lead', 'player', 'storyteller') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('character-sheets/moderation_log');
      $this->assertStatus(200);
      $this->assertText('Character sheet moderation log');
    }
    $this->login($this->site->createUser(array('storyteller_assistant')));
    $this->get('character-sheets/moderation_log');
    $this->assertStatus(403);
    $this->anonymous();
    $this->get('character-sheets/moderation_log');
    $this->assertStatus(403);
  }

  public function testModerationLogShowsHostnameToAdministratorsAndLeads() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Hero')));
    $tag = $this->site->uniqueName('msg');
    $this->site->addSheetLog($character, $player, 'edit', $tag, time() + 86400 * 400);
    foreach (array('administrator', 'lead') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('character-sheets/moderation_log');
      $this->assertText('Hostname');
      $rows = $this->rows('Logged user');
      $this->assertSame('127.0.0.1', $rows[0][7], $role);
    }
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('character-sheets/moderation_log');
    $this->assertNoText('Hostname');
  }

  public function testModerationLogNewestFirstWithoutSilver() {
    $player = $this->site->createUser(array('player'));
    $moderator = $this->site->createUser(array('storyteller'));
    $character = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Hero')));
    $future = time() + 86400 * 400;
    $tag = $this->site->uniqueName('msg');
    $this->site->addSheetLog($character, $moderator, 'xp', $tag . ' older', $future);
    $this->site->addSheetLog($character, $moderator, 'edit', $tag . ' newer', $future + 60);
    $this->site->addSheetLog($character, NULL, 'silver', $tag . ' silver', $future + 120);

    $this->login($player);
    $this->get('character-sheets/moderation_log');
    $rows = $this->rows('Logged user');
    $this->assertSame($tag . ' newer', $rows[0][4]);
    $this->assertSame($tag . ' older', $rows[1][4]);
    $this->assertSame(array('edit', $character->title, $player->name, $moderator->name), array($rows[0][1], $rows[0][2], $rows[0][3], $rows[0][5]));
    // Security fix (D7 showed IP addresses to all players): no Hostname
    // column for players.
    $this->assertCount(7, $rows[0]);
    $this->assertNoText('Hostname');
    $this->assertNoText('127.0.0.1');
    $this->assertNoText($tag . ' silver');
    $this->assertRaw('href="/node/' . $character->nid . '"');

    // The log type filter can be cleared to show silver too.
    $this->get('character-sheets/moderation_log', array('type' => '', 'message' => $tag, 'message_op' => 'starts'));
    $this->assertSame(array($tag . ' silver', $tag . ' newer', $tag . ' older'), array_column($this->rows('Logged user'), 4));
  }

  public function testModerationLogFilters() {
    $player = $this->site->createUser(array('player'));
    $other = $this->site->createUser(array('player'));
    $moderator = $this->site->createUser(array('storyteller'));
    $mine = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Hero')));
    $theirs = $this->site->createCharacter($other, array('title' => $this->site->uniqueName('Hero')));
    $tag = $this->site->uniqueName('msg');
    $this->site->addSheetLog($mine, $moderator, 'xp', $tag . ' a', time());
    $this->site->addSheetLog($theirs, $player, 'xp', $tag . ' b', time());

    $this->login($this->site->createUser(array('lead')));
    $this->get('character-sheets/moderation_log', array('message' => $tag, 'message_op' => 'starts', 'uid' => $player->name));
    $this->assertSame(array($tag . ' a'), array_column($this->rows('Logged user'), 4));
    $this->get('character-sheets/moderation_log', array('message' => $tag, 'message_op' => 'starts', 'uid_1' => $player->name));
    $this->assertSame(array($tag . ' b'), array_column($this->rows('Logged user'), 4));
    // The site logs the sheet's creation too.
    $this->get('character-sheets/moderation_log', array('title' => $theirs->title, 'title_op' => '='));
    $messages = array_column($this->rows('Logged user'), 4);
    $this->assertContains($tag . ' b', $messages);
    $this->assertNotContains($tag . ' a', $messages);
  }

  public function testModerationLogPager() {
    $character = $this->site->createCharacter($this->site->createUser(array('player')));
    $tag = $this->site->uniqueName('msg');
    for ($i = 0; $i < 36; $i++) {
      $this->site->addSheetLog($character, NULL, 'xp', $tag . ' ' . $i, time());
    }
    $this->login($this->site->createUser(array('lead')));
    $this->get('character-sheets/moderation_log', array('message' => $tag, 'message_op' => 'starts'));
    $this->assertCount(35, $this->rows('Logged user'));
    $this->get('character-sheets/moderation_log', array('message' => $tag, 'message_op' => 'starts', 'page' => 1));
    $this->assertCount(1, $this->rows('Logged user'));
  }

}
