<?php

namespace Vaxia\Tests\Rpg\SceneRecord;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Scene records: a scene collects chat posts (comments) for evaluation and shows
 * them as a table with each post's character, text, dice outcome and running
 * KXP/XP totals. Only the scene's author, characters in it, and staff may look.
 *
 * @group scene_record
 */
class SceneRecordTest extends VaxiaTestCase {

  /**
   * A scene with two posts by one character (1000 and 200 characters long),
   * listed out of order.
   */
  private function sceneWithPosts($author, $player = NULL) {
    $player = $player ?: $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Hero')));
    $room = $this->site->createRoom();
    $first = $this->site->postComment($room, $player, str_repeat('a', 1000), $character);
    $second = $this->site->postComment($room, $player, str_repeat('b', 200), $character);
    $scene = $this->site->createScene($author, array($second, $first));
    return array($scene, $character);
  }

  private function column($n) {
    $cells = array();
    foreach ($this->xpath()->query('//table//tbody/tr/td[' . $n . ']') as $td) {
      $cells[] = trim(preg_replace('/\s+/', ' ', $td->textContent));
    }
    return $cells;
  }

  private function viewAs($account, $scene) {
    $this->login($account);
    $this->get('node/' . $scene->nid);
  }

  // ---------------------------------------------------------------------------
  // Who may look.
  // ---------------------------------------------------------------------------

  public function testAuthorWithViewOwnSeesScene() {
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    list($scene, $character) = $this->sceneWithPosts($author);
    $this->viewAs($author, $scene);
    $this->assertStatus(200);
    $this->assertText($character->title);
  }

  public function testPlayerWithCharacterInSceneSeesIt() {
    $player = $this->site->createUser(array('player'));
    list($scene) = $this->sceneWithPosts($this->site->createUser(array('player')), $player);
    $this->viewAs($player, $scene);
    $this->assertSame(1, $this->xpath()->query('//table')->length);
  }

  public function testStaffWithViewAllSeeAnyScene() {
    list($scene) = $this->sceneWithPosts($this->site->createUser(array('player')));
    $this->viewAs($this->site->createUser(array('storyteller'), array('view all scenes')), $scene);
    $this->assertSame(1, $this->xpath()->query('//table')->length);
  }

  public function testOutsidersAreSentToTheFrontPage() {
    list($scene) = $this->sceneWithPosts($this->site->createUser(array('player')));
    $this->viewAs($this->site->createUser(array('player')), $scene);
    $this->assertStringNotContainsString('/node/' . $scene->nid, $this->url);
    $this->assertText("This is not your scene or you are not able to see other's scenes.");
  }

  public function testAuthorWithoutViewOwnIsSentAway() {
    $this->withoutPermission('view own scenes', array('anonymous user', 'authenticated user', 'player'), function () {
      $author = $this->site->createUser(array('player'));
      list($scene) = $this->sceneWithPosts($author);
      $this->viewAs($author, $scene);
      $this->assertText("This is not your scene or you are not able to see other's scenes.");
    });
  }

  // ---------------------------------------------------------------------------
  // The results table.
  // ---------------------------------------------------------------------------

  public function testPostsAreShownInPostOrder() {
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    list($scene) = $this->sceneWithPosts($author);
    $this->viewAs($author, $scene);
    $posts = $this->column(2);
    $this->assertCount(2, $posts);
    $this->assertStringStartsWith('aaaa', $posts[0]);
    $this->assertStringStartsWith('bbbb', $posts[1]);
  }

  public function testRowsShowCharacterAndResults() {
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    list($scene, $character) = $this->sceneWithPosts($author);
    $this->viewAs($author, $scene);
    $this->assertSame(array($character->title, $character->title), $this->column(1));
    $results = $this->column(3);
    $this->assertStringContainsString('No roll found to evaluate. Skipped!', $results[0]);
    $this->assertStringContainsString('Link to post', $results[0]);
  }

  public function testKxpAndXpRunningTotals() {
    $this->site->setConfig('vaxia_kxp_ratio', 5);
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    list($scene, $character) = $this->sceneWithPosts($author);
    $this->viewAs($author, $scene);
    $this->site->deleteConfig('vaxia_kxp_ratio');

    $results = $this->column(3);
    // 1000 characters: 81.6 * (e^0.08 - 1) = 6.797..., truncated to "6.79".
    $this->assertStringContainsString('6.79 KXP in this post.', $results[0]);
    $this->assertStringContainsString('6.79 KXP for ' . $character->title . '.', $results[0]);
    $this->assertStringContainsString('1 XP for ' . $character->title, $results[0]);
    // 200 characters: 81.6 * (e^0.016 - 1) = 1.316..., "1.31"; running total 8.1.
    $this->assertStringContainsString('1.31 KXP in this post.', $results[1]);
    $this->assertStringContainsString('8.1 KXP for ' . $character->title . '.', $results[1]);
  }

  public function testKxpPerPostIsCapped() {
    $this->site->setConfig('vaxia_max_kxp_post', 3);
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    list($scene) = $this->sceneWithPosts($author);
    $this->viewAs($author, $scene);
    $this->site->deleteConfig('vaxia_max_kxp_post');
    $this->assertStringContainsString('3 KXP in this post.', $this->column(3)[0]);
  }

  public function testTotalsForAllWhenSeveralCharactersPost() {
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    $room = $this->site->createRoom();
    $pa = $this->site->createUser(array('player'));
    $pb = $this->site->createUser(array('player'));
    $a = $this->site->createCharacter($pa);
    $b = $this->site->createCharacter($pb);
    $scene = $this->site->createScene($author, array(
      $this->site->postComment($room, $pa, str_repeat('a', 1000), $a),
      $this->site->postComment($room, $pb, str_repeat('b', 1000), $b),
    ));
    $this->viewAs($author, $scene);
    $this->assertStringContainsString('Total 13.58 KXP for all.', $this->column(3)[1]);
  }

  // ---------------------------------------------------------------------------
  // Scene form.
  // ---------------------------------------------------------------------------

  public function testEditFormShowsSceneAssistant() {
    $author = $this->site->createUser(array('player'), array('view own scenes', 'edit own scene content'));
    list($scene, $character) = $this->sceneWithPosts($author);
    $this->login($author);
    $this->get('node/' . $scene->nid . '/edit');
    $this->assertStatus(200);
    $this->assertText('Scene assistant');
    $this->assertSame(2, $this->xpath()->query('//input[@value="Refresh scene"]')->length);
    $this->assertText($character->title);
  }

  // ---------------------------------------------------------------------------
  // Known bugs.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * _scene_record_view_table() passes 'headers' to theme('table') instead of
   * 'header', so the Character / Scene posts / Results headings never show.
   */
  public function testTableHasColumnHeadings() {
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    list($scene) = $this->sceneWithPosts($author);
    $this->viewAs($author, $scene);
    $headers = array();
    foreach ($this->xpath()->query('//table//th') as $th) {
      $headers[] = trim($th->textContent);
    }
    $this->assertSame(array('Character', 'Scene posts', 'Results'), $headers);
  }

  /**
   * @group knownbug
   * The scene form alter reads $scene before setting it when the scene has no
   * posts yet: a PHP notice on every new-scene form.
   */
  public function testNewSceneFormHasNoPhpErrors() {
    $this->login($this->site->createUser(array('player'), array('create scene content')));
    $this->get('node/add/scene');
    $this->assertSame(array(), $this->pageErrorsFrom('scene_record.module'));
  }

  /**
   * @group knownbug
   * Posts made without a character (OOC) break the results table:
   * _scene_record_calculate_results() reads ->title/->nid of a FALSE character.
   */
  public function testPostWithoutCharacterHasNoPhpErrors() {
    $author = $this->site->createUser(array('player'), array('view own scenes'));
    $room = $this->site->createRoom();
    $scene = $this->site->createScene($author, array($this->site->postComment($room, $author, 'brb')));
    $this->viewAs($author, $scene);
    $this->assertSame(array(), $this->pageErrorsFrom('scene_record.module'));
  }

}
