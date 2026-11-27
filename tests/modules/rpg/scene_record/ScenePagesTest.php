<?php

namespace Vaxia\Tests\Rpg\SceneRecord;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Scene lists (D7 view "scenes"): "My Scenes" at character-scenes/mine (and
 * its parent character-scenes), the storyhosts' "Scene Evaluation" at
 * character-scenes/evaluate, and the "Scenes" tab of a character sheet.
 * Published scenes only, newest first, 35 a page; a "Current state" filter
 * starts on the listed states (draft, submitted and ruled for your own;
 * submitted and ruled otherwise).
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group scene_record
 */
class ScenePagesTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
  }

  private function scene($author, $state, array $values = array()) {
    $scene = $this->site->createNode('scene', $values + array('uid' => $author->uid, 'title' => $this->site->uniqueName('Scene')));
    if ($state) {
      $this->site->forceWorkflowState($scene, $state);
    }
    return $scene;
  }

  /**
   * Rows of the scene table on the last page: arrays of cell texts.
   */
  private function rows() {
    $xpath = $this->xpath();
    $rows = array();
    foreach ($xpath->query('//table[.//th[contains(., "Attention To")]]/tbody/tr') as $tr) {
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

  /**
   * Titles listed on the last page (the Title column).
   */
  private function titles($column = 1) {
    return array_map(function ($row) use ($column) {
      return $row[$column];
    }, $this->rows());
  }

  public function testMineListsOwnSubmittableScenesNewestFirst() {
    $player = $this->site->createUser(array('player'));
    $other = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Hero')));
    $draft = $this->scene($player, 'draft', array('field_scene_character' => $character->nid, 'field_sh_attention_to' => 'Saga X'));
    $submitted = $this->scene($player, 'submitted');
    $ruled = $this->scene($player, 'ruled');
    $this->site->backdateNodeCreated($draft, time() - 300);
    $this->site->backdateNodeCreated($submitted, time() - 200);
    // Explicit: in-process nodes default to the PHPUnit process's request
    // time, which a long suite run leaves minutes behind.
    $this->site->backdateNodeCreated($ruled, time() - 100);
    $not_for_submission = $this->scene($player, 'not for submission');
    $unpublished = $this->scene($player, 'draft', array('status' => 0));
    $others = $this->scene($other, 'submitted');

    $this->login($player);
    foreach (array('character-scenes/mine', 'character-scenes') as $path) {
      $this->get($path);
      $this->assertStatus(200);
      $this->assertText('Scenes');
      $this->assertSame(array($ruled->title, $submitted->title, $draft->title), $this->titles(), $path);
    }
    $this->assertNoText($not_for_submission->title);
    $this->assertNoText($unpublished->title);
    $this->assertNoText($others->title);

    $row = $this->rows()[2];
    $this->assertSame($character->title, $row[0]);
    $this->assertSame('Saga X', $row[2]);
    $this->assertSame('draft', $row[4]);
    $this->assertMatchesRegularExpression('/^\d\d:\d\d[ap]m \S+$/', $row[3], 'Last updated shows the time of day');
    $this->assertRaw('href="' . '/node/' . $draft->nid . '"');
  }

  public function testMineStateAndCharacterFilters() {
    $player = $this->site->createUser(array('player'));
    $hero = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Hero')));
    $draft = $this->scene($player, 'draft', array('field_scene_character' => $hero->nid));
    $ruled = $this->scene($player, 'ruled');
    $this->login($player);

    $this->get('character-scenes/mine', array('sid' => array('scene_evaluation_ruled')));
    $this->assertSame(array($ruled->title), $this->titles());

    $this->get('character-scenes/mine', array('field_scene_character_nid' => $hero->nid));
    $this->assertSame(array($draft->title), $this->titles());

    // The filter offers only the listed states.
    $options = array();
    foreach ($this->xpath()->query('//select[@name="sid[]"]/option') as $option) {
      $options[] = trim($option->textContent);
    }
    $this->assertSame(array('draft', 'submitted', 'ruled'), $options);
  }

  public function testMineSortsByClickedColumn() {
    $player = $this->site->createUser(array('player'));
    $b = $this->scene($player, 'draft', array('title' => $this->site->uniqueName('B')));
    $a = $this->scene($player, 'draft', array('title' => $this->site->uniqueName('A')));
    $c = $this->scene($player, 'draft', array('title' => $this->site->uniqueName('C')));
    $this->login($player);
    $this->get('character-scenes/mine', array('order' => 'Title', 'sort' => 'asc'));
    $this->assertSame(array($a->title, $b->title, $c->title), $this->titles());
    $this->get('character-scenes/mine', array('order' => 'Title', 'sort' => 'desc'));
    $this->assertSame(array($c->title, $b->title, $a->title), $this->titles());
  }

  public function testMineEmptyText() {
    $this->login($this->site->createUser(array('player')));
    $this->get('character-scenes/mine');
    $this->assertStatus(200);
    $this->assertText('No scenes found.');
  }

  public function testMinePagerShows35() {
    $player = $this->site->createUser(array('player'));
    for ($i = 0; $i < 36; $i++) {
      $this->scene($player, 'draft');
    }
    $this->login($player);
    $this->get('character-scenes/mine');
    $this->assertCount(35, $this->rows());
    $this->get('character-scenes/mine', array('page' => 1));
    $this->assertCount(1, $this->rows());
  }

  public function testEvaluationIsForStorytellers() {
    $author = $this->site->createUser(array('player'));
    $submitted = $this->scene($author, 'submitted', array('field_sh_attention_to' => 'any A/SH'));
    $draft = $this->scene($author, 'draft');

    $this->login($this->site->createUser(array('player')));
    $this->get('character-scenes/evaluate');
    $this->assertStatus(403);

    $this->login($this->site->createUser(array('lead')));
    $this->get('character-scenes/evaluate');
    $this->assertStatus(403);

    $this->login($this->site->createUser(array('storyteller')));
    $this->get('character-scenes/evaluate');
    $this->assertStatus(200);
    $this->assertText('Evaluation');
    $this->assertText($submitted->title);
    $this->assertNoText($draft->title);
    $rows = array_values(array_filter($this->rows(), function ($row) use ($submitted) {
      return $row[1] === $submitted->title;
    }));
    $this->assertSame($author->name, $rows[0][2], 'The author is shown');
    $this->assertSame('submitted', $rows[0][5]);
  }

  public function testEvaluationTabShownOnMyScenesToStorytellers() {
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('character-scenes');
    $this->assertRaw('href="/character-scenes/evaluate"');
    $this->login($this->site->createUser(array('player')));
    $this->get('character-scenes');
    $this->assertNoRaw('href="/character-scenes/evaluate"');
  }

  public function testCharacterTabListsItsScenes() {
    $player = $this->site->createUser(array('player'));
    $hero = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Hero')));
    $other_author = $this->site->createUser(array('player'));
    $about = $this->scene($other_author, 'submitted', array('field_scene_character' => $hero->nid));
    $elsewhere = $this->scene($other_author, 'submitted');
    $draft = $this->scene($other_author, 'draft', array('field_scene_character' => $hero->nid));

    $this->login($player);
    $this->get('node/' . $hero->nid);
    $this->assertRaw('href="/node/' . $hero->nid . '/scenes"');
    $this->get('node/' . $hero->nid . '/scenes');
    $this->assertStatus(200);
    $this->assertSame(array($about->title), $this->titles());
    $this->assertNoText($elsewhere->title);
    $this->assertNoText($draft->title);
  }

  public function testCharacterTabAccess() {
    $owner = $this->site->createUser(array('player'));
    $hero = $this->site->createCharacter($owner);

    // Another player can't edit the sheet.
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $hero->nid . '/scenes');
    $this->assertStatus(403);

    // Not a character sheet.
    $this->login($owner);
    $page = $this->site->createNode('page', array('uid' => $owner->uid));
    $this->get('node/' . $page->nid . '/scenes');
    $this->assertStatus(403);

    $this->anonymous();
    $this->get('node/' . $hero->nid . '/scenes');
    $this->assertStatus(403);
  }

}
