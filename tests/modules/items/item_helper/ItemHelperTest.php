<?php

namespace Vaxia\Tests\Items\ItemHelper;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Item helper: on scenes with the "item" goal, the scene results column keeps a
 * running total for item crafting. Only posts rolling two skills that are both
 * craft/repair/persist skills count.
 *
 * @group item_helper
 */
class ItemHelperTest extends VaxiaTestCase {

  use DiceTrait;

  private function itemScene($author, array $comments, array $values = array()) {
    $refs = array();
    foreach ($comments as $cid) {
      $refs[] = array('target_id' => $cid);
    }
    return $this->site->createNode('scene', $values + array('uid' => $author->uid, 'field_rp_posts' => $refs, 'field_goal' => 'item'));
  }

  /**
   * Posts a roll and returns its comment id (from the post's anchor).
   */
  private function rollPost($player, $room, $character, array $rows) {
    $text = $this->site->uniqueName('post');
    $this->postRoll($player, $room, $character, $rows, array(), $text);
    // The post's marker: D7 puts <a id="comment-N"> before the post, D11 an
    // <article id="comment-N"> around it.
    $cid = 0;
    foreach ($this->xpath()->query('//*[starts-with(@id, "comment-")][(following::text() | .//text())[contains(., "' . $text . '")]]') as $marker) {
      if (preg_match('/^comment-(\d+)$/', $marker->getAttribute('id'), $m)) {
        $cid = (int) $m[1];
      }
    }
    $this->assertGreaterThan(0, $cid, 'Post found');
    return $cid;
  }

  private function resultsFor($author, $scene) {
    $this->login($author);
    $this->get('node/' . $scene->nid);
    $cells = array();
    foreach ($this->xpath()->query('//table//tbody/tr/td[3]') as $td) {
      $cells[] = trim(preg_replace('/\s+/', ' ', $td->textContent));
    }
    return $cells;
  }

  public function testSingleRollIsNotACraftRoll() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_dexterity' => 5));
    $smithing = $this->site->addSkill($character, 'Smithing', 10, array('craft'));
    $cid = $this->rollPost($player, $this->site->createRoom(), $character, array(array('stat' => 'dexterity', 'skill' => $smithing)));
    $author = $this->site->createUser(array('player'), array('view own scenes'));

    $results = $this->resultsFor($author, $this->itemScene($author, array($cid)));

    $this->assertStringContainsString('Item craft/persist 2-stat roll NOT found.', $results[0]);
  }

  public function testTwoNonCraftSkillsAreNotACraftRoll() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_dexterity' => 5, 'field_strength' => 5));
    $sword = $this->site->addSkill($character, 'Swords', 10, array('attack'));
    $smithing = $this->site->addSkill($character, 'Smithing', 10, array('craft'));
    $cid = $this->rollPost($player, $this->site->createRoom(), $character, array(
      array('stat' => 'strength', 'skill' => $sword),
      array('stat' => 'dexterity', 'skill' => $smithing),
    ));
    $author = $this->site->createUser(array('player'), array('view own scenes'));

    $results = $this->resultsFor($author, $this->itemScene($author, array($cid)));

    $this->assertStringContainsString('Item craft/persist 2-stat roll NOT found.', $results[0]);
  }

  /**
   * A post rolling two skills with the given aspects; returns its results cell.
   */
  private function twoSkillResults($aspectOne, $aspectTwo, array $sceneValues = array(), $level = 10) {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_dexterity' => $level, 'field_intelligence' => $level));
    $one = $this->site->addSkill($character, 'Skill one', $level, array($aspectOne));
    $two = $this->site->addSkill($character, 'Skill two', $level, array($aspectTwo));
    $cid = $this->rollPost($player, $this->site->createRoom(), $character, array(
      array('stat' => 'dexterity', 'skill' => $one),
      array('stat' => 'intelligence', 'skill' => $two),
    ));
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    return $this->resultsFor($author, $this->itemScene($author, array($cid), $sceneValues))[0];
  }

  public function testTwoCraftSkillsCountTowardMaking() {
    $results = $this->twoSkillResults('craft', 'craft');
    $this->assertMatchesRegularExpression('/Item craft 2-stat roll found\.|A critical FAIL wiped out items progress\./', $results);
    if (preg_match('/gained ([1-9][\d,]*) PP/', $results, $m) && strpos($results, 'critical FAIL') === FALSE) {
      $this->assertStringContainsString('PP generated for effect: ' . number_format((float) str_replace(',', '', $m[1]), 2) . '.', $results);
    }
  }

  public function testTwoPersistSkillsCountTowardEnchanting() {
    $results = $this->twoSkillResults('persist', 'persist');
    $this->assertMatchesRegularExpression('/Item persist 2-stat roll found\.|A critical FAIL wiped out items progress\./', $results);
    if (preg_match('/gained ([1-9][\d,]*) PP/', $results, $m) && strpos($results, 'critical FAIL') === FALSE) {
      $this->assertStringContainsString('PP persisted for 1 use: ' . number_format((float) str_replace(',', '', $m[1]), 2) . '.', $results);
    }
  }

  /**
   * @group knownbug
   * A craft + persist (or repair) roll passes the "2-stat craft/persist roll"
   * check, then matches neither the craft nor the persist branch: no message,
   * nothing counted, and no hint why.
   */
  public function testMixedCraftAndPersistRollIsReported() {
    $results = $this->twoSkillResults('craft', 'persist');
    $this->assertMatchesRegularExpression('/Item (craft|persist) 2-stat roll (found|NOT found)/', $results);
  }

  /**
   * @group knownbug
   * PP per point is floor((1 + (difficulty - 40) / 10) * 25), which is 0 at
   * difficulty 30: every craft post on such a scene divides by zero (a fatal
   * DivisionByZeroError on PHP 8).
   */
  public function testDifficultyThirtyHasNoPhpErrors() {
    $this->twoSkillResults('craft', 'craft', array('field_difficulty' => 30));
    $this->assertSame(array(), $this->pageErrorsFrom('item_helper.module'));
  }

  /**
   * @group knownbug
   * Same formula: below difficulty 30 (including the default 25) the limits and
   * points come out negative ("-3 / -2 / -4", "Points for primary stats: -23").
   */
  public function testLimitsAreNotNegativeAtDefaultDifficulty() {
    // Stats and skills of 300 always succeed, so PP is gained and limits shown.
    $results = $this->twoSkillResults('craft', 'craft', array(), 300);
    $this->assertMatchesRegularExpression('/Max primary \/ secondary \/ skill : \d+ \/ \d+ \/ \d+\./', $results);
  }

  public function testOtherGoalsShowNoItemTotals() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player, array('field_dexterity' => 5));
    $cid = $this->rollPost($player, $this->site->createRoom(), $character, array(array('stat' => 'dexterity')));
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    $scene = $this->site->createNode('scene', array('uid' => $author->uid, 'field_rp_posts' => array(array('target_id' => $cid))));

    $results = $this->resultsFor($author, $scene);

    $this->assertStringNotContainsString('Item craft', $results[0]);
  }

}
