<?php

namespace Vaxia\Tests\Storyhosts\Storyhosts;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Session report lists (D7 view "sessions"): every session at sessions and
 * sessions/all, a character's sessions (node/%/sessions tab), the sessions a
 * user ran (user/%/sessions tab, with whether each counts toward session
 * requirements), and the storyhost roll calls (sessions/storytellers-and-
 * apprentices, with the retired ones below and on their own page).
 *
 * Lists show published session reports, newest first, 35 a page, filtered by
 * Saga, A/SH Player and Realm (Vaxia, Sirian and Storyhost to start with).
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group storyhosts
 */
class SessionPagesTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
  }

  private function session($author, array $values = array()) {
    return $this->site->createNode('session_report', $values + array(
      'uid' => $author->uid,
      'title' => $this->site->uniqueName('Session'),
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ));
  }

  /**
   * Rows of the first table on the last page whose header has $column.
   */
  private function rows($column = 'Session held') {
    $xpath = $this->xpath();
    $rows = array();
    $table = $xpath->query('//table[.//th[normalize-space()="' . $column . '" or .//a[normalize-space()="' . $column . '"]]]')->item(0);
    if (!$table) {
      return $rows;
    }
    foreach ($xpath->query('./tbody/tr', $table) as $tr) {
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

  private function column($index, $column = 'Session held') {
    return array_map(function ($row) use ($index) {
      return $row[$index];
    }, $this->rows($column));
  }

  public function testAllSessionsArePublicNewestFirst() {
    $sh = $this->site->createUser(array('storyteller'));
    $hero = $this->site->createCharacter($this->site->createUser(array('player')), array('title' => $this->site->uniqueName('Hero')));
    $saga = $this->site->uniqueName('Saga');
    $old = $this->session($sh, array(
      'field_saga' => $this->site->term('sagas', $saga),
      'field_session_held' => '2026-03-04',
      'field_characters_involved' => $hero->nid,
      'body' => array('value' => 'We fought a dragon.', 'format' => 'plain_text'),
      'field_rate_session' => 4,
    ));
    $this->site->backdateNodeCreated($old, time() - 600);
    $new = $this->session($sh);
    // Explicit: in-process nodes default to the PHPUnit process's request
    // time, which a long suite run leaves minutes behind.
    $this->site->backdateNodeCreated($new, time() - 100);
    $unpublished = $this->session($sh, array('status' => 0));

    $this->anonymous();
    foreach (array('sessions', 'sessions/all') as $path) {
      $this->get($path, array('user' => $sh->name));
      $this->assertStatus(200);
      $this->assertText('Sessions');
      $this->assertSame(array($new->title, $old->title), $this->column(0), $path);
    }
    $this->assertNoText($unpublished->title);
    $row = $this->rows()[1];
    $this->assertSame(array($old->title, '(' . $saga . ')', 'Vaxia', 'Wednesday, March 4, 2026', $hero->title, 'We fought a dragon.', '4'), $row);
    $this->assertRaw('href="/node/' . $hero->nid . '"');
  }

  public function testSessionsTabs() {
    $this->anonymous();
    $this->get('sessions');
    $this->assertRaw('href="/sessions/storytellers-and-apprentices"');
  }

  public function testRealmFilterStartsOnEveryRealm() {
    // The original site started on its three named realms; the release
    // starts on every realm, so a new realm's sessions show without setup.
    $sh = $this->site->createUser(array('storyteller'));
    $fantasy = $this->session($sh, array('field_tag_realm' => $this->site->term('realm', 'Fantasy')));
    $new_realm = $this->site->uniqueName('Realm');
    $elsewhere = $this->session($sh, array('field_tag_realm' => $this->site->term('realm', $new_realm)));
    $this->anonymous();
    $this->get('sessions');
    $this->assertText($fantasy->title);
    $this->assertText($elsewhere->title);
    $selected = array();
    foreach ($this->xpath()->query('//select[@name="field_tag_realm_tid[]"]/option[@selected]') as $option) {
      $selected[] = trim($option->textContent, " -");
    }
    $this->assertContains('Fantasy', $selected);
    $this->assertContains($new_realm, $selected);

    // As with D7's exposed filters: once the form is used, no realm selected
    // means any realm, and a picked realm narrows the list.
    $this->get('sessions', array('user' => $sh->name));
    $this->assertText($elsewhere->title);
    $this->get('sessions', array('user' => $sh->name, 'field_tag_realm_tid' => array($this->site->term('realm', $new_realm))));
    $this->assertSame(array($elsewhere->title), $this->column(0));
    $this->get('sessions', array('user' => $sh->name, 'field_tag_realm_tid' => array($this->site->term('realm', 'Fantasy'))));
    $this->assertSame(array($fantasy->title), $this->column(0));
  }


  public function testSagaFilter() {
    $sh = $this->site->createUser(array('storyteller'));
    $saga = $this->site->uniqueName('Saga');
    $in = $this->session($sh, array('field_saga' => $this->site->term('sagas', $saga)));
    $out = $this->session($sh);
    $this->anonymous();
    $this->get('sessions', array('field_saga_tid' => $saga));
    $this->assertSame(array($in->title), $this->column(0));
    $this->assertNoText($out->title);
  }

  public function testEmptyTextAndPager() {
    $this->anonymous();
    $this->get('sessions', array('user' => $this->site->uniqueName('nobody')));
    $this->assertText('No sessions found.');

    $sh = $this->site->createUser(array('storyteller'));
    for ($i = 0; $i < 36; $i++) {
      $this->session($sh);
    }
    $this->get('sessions', array('user' => $sh->name));
    $this->assertCount(35, $this->rows());
    $this->get('sessions', array('user' => $sh->name, 'page' => 1));
    $this->assertCount(1, $this->rows());
  }

  public function testCharacterSessionsTab() {
    $owner = $this->site->createUser(array('player'));
    $hero = $this->site->createCharacter($owner, array('title' => $this->site->uniqueName('Hero')));
    $sh = $this->site->createUser(array('storyteller'));
    $with = $this->session($sh, array('field_characters_involved' => $hero->nid));
    $without = $this->session($sh);

    $this->login($owner);
    $this->get('node/' . $hero->nid);
    $this->assertRaw('href="/node/' . $hero->nid . '/sessions"');
    $this->get('node/' . $hero->nid . '/sessions');
    $this->assertStatus(200);
    $this->assertText('Character sessions');
    $this->assertSame(array($with->title), $this->column(1));
    $this->assertSame($sh->name, $this->rows()[0][0], 'SH column');
    $this->assertNoText($without->title);

    // Someone who can't edit the sheet; and a node that isn't one.
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $hero->nid . '/sessions');
    $this->assertStatus(403);
    $this->login($this->site->createUser(array('administrator')));
    $this->get('node/' . $with->nid . '/sessions');
    $this->assertStatus(403);
  }

  public function testUserSessionsTabShowsRequirements() {
    $sh = $this->site->createUser(array('storyteller'));
    $other = $this->site->createUser(array('storyteller'));
    $ran = $this->session($sh);
    $not_theirs = $this->session($other);

    $this->anonymous();
    $this->get('user/' . $sh->uid . '/sessions');
    $this->assertStatus(200);
    $this->assertText('SH Sessions');
    $this->assertText('Session requirements are used to determine if a session qualifies');
    $this->assertSame(array($ran->title), $this->column(1));
    $this->assertNoText($not_theirs->title);
    $this->assertContains($this->rows()[0][6], array('yes', 'no'));

    $this->login($sh);
    $this->get('user/' . $sh->uid);
    $this->assertRaw('href="/user/' . $sh->uid . '/sessions"');
  }

  public function testUserSessionsRequirementColumn() {
    // Production's requirement: two PCs of other players.
    $sh = $this->site->createUser(array('storyteller'));
    $one = $this->site->createCharacter($this->site->createUser(array('player')));
    $two = $this->site->createCharacter($this->site->createUser(array('player')));
    $counts = $this->session($sh, array('field_characters_involved' => array(array('nid' => $one->nid), array('nid' => $two->nid))));
    $short = $this->session($sh, array('field_characters_involved' => $one->nid));
    $this->anonymous();
    $this->get('user/' . $sh->uid . '/sessions');
    $requirements = array();
    foreach ($this->rows() as $row) {
      $requirements[$row[1]] = $row[6];
    }
    $this->assertSame('yes', $requirements[$counts->title]);
    $this->assertSame('no', $requirements[$short->title]);
  }

  public function testStoryhostRollCall() {
    $sh = $this->site->createUser(array('storyteller'), array(), array('field_became_sh' => '2025-02-03'));
    $character = $this->site->createCharacter($sh, array('title' => $this->site->uniqueName('Shade'), 'field_is_storyteller' => 1));
    $this->site->setUserFields($sh, array('field_storyhost_character' => $character->nid));
    $this->session($sh, array('field_rate_session' => 5));
    $this->session($sh, array('field_rate_session' => 3));
    $apprentice = $this->site->createUser(array('storyteller_candidate'));
    $this->session($apprentice);
    $retired = $this->site->createUser(array('player'));
    $this->session($retired);

    $this->anonymous();
    $this->get('sessions/storytellers-and-apprentices');
    $this->assertStatus(200);
    $this->assertText('Storyhosts and Apprentices');
    $this->assertText('This is a list of our Storyhosts and Apprentices over time.');
    $this->assertText('Our retired storyhosts and apprentices are no longer running here');

    $active = $this->xpath()->query('//table[contains(@class, "vaxia-storyhosts")]')->item(0);
    $this->assertNotNull($active);
    $text = preg_replace('/\s+/', ' ', $active->textContent);
    $this->assertStringContainsString($sh->name, $text);
    // "[titles] [title]" of the listed storyhost character (the site sets the titles).
    $this->assertMatchesRegularExpression('/\\S+ ' . preg_quote($character->title, '/') . '/', $text);
    $this->assertStringContainsString('Number of sessions: 2', $text);
    $this->assertStringContainsString('Average approval: 80.00%', $text);
    $this->assertStringContainsString('Became Storyhost: 02/03/2025', $text);
    $this->assertStringContainsString($apprentice->name, $text);
    $this->assertStringContainsString('Apprentice Storyhost', $text);
    $this->assertStringContainsString('No votes.', $text);
    $this->assertStringNotContainsString($retired->name, $text);

    $old = $this->xpath()->query('//table[contains(@class, "vaxia-retired-storyhosts")]')->item(0);
    $this->assertNotNull($old);
    $text = preg_replace('/\s+/', ' ', $old->textContent);
    $this->assertStringContainsString($retired->name, $text);
    $this->assertStringContainsString('Retired', $text);
    $this->assertStringNotContainsString($sh->name, $text);

    $this->get('sessions/storytellers-and-apprentices/retired');
    $this->assertStatus(200);
    $this->assertText('Retired Storyhosts and Apprentices');
    $this->assertText($retired->name);
    $this->assertNoText($sh->name);
  }

  public function testRollCallMostSessionsFirst() {
    $busy = $this->site->createUser(array('storyteller'));
    $quiet = $this->site->createUser(array('storyteller'));
    $this->session($quiet);
    for ($i = 0; $i < 3; $i++) {
      $this->session($busy);
    }
    $this->anonymous();
    $this->get('sessions/storytellers-and-apprentices');
    $text = preg_replace('/\s+/', ' ', $this->xpath()->query('//table[contains(@class, "vaxia-storyhosts")]')->item(0)->textContent);
    $this->assertLessThan(strpos($text, $quiet->name), strpos($text, $busy->name));
  }

}
