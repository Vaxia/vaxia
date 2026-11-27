<?php

namespace Vaxia\Tests\Vaxia\VaxiaEvaluation;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Evaluation notices" block (D7 view storyhost_evaluation): for admins, leads
 * and storytellers, the ten newest published character sheets, items and
 * scenes waiting in "submitted", grouped under "Evaluate <type>s", each as
 * "Title (for <attention to>)" ("any A/SH" when nobody is named).
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group vaxia_evaluation
 * @group views_blocks
 */
class EvaluationNoticesBlockTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'storyhost_evaluation-sh_evaluation', 'sidebar_first');
  }

  /**
   * Group heading => rows, on the front page; NULL when the block isn't shown.
   */
  private function groups() {
    $this->get('node');
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-sh_evaluation ")]');
    if (!$view->length) {
      return NULL;
    }
    $groups = array();
    foreach ($xpath->query('.//div[contains(@class, "item-list")]', $view->item(0)) as $list) {
      $heading = $xpath->query('.//h3', $list)->item(0);
      $rows = array();
      foreach ($xpath->query('.//li', $list) as $li) {
        $rows[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
      }
      $groups[$heading ? trim($heading->textContent) : ''] = $rows;
    }
    return $groups;
  }

  private function submitted($type, array $values = array()) {
    $owner = $this->site->createUser(array('player'));
    if ($type === 'character_sheet') {
      $node = $this->site->createCharacter($owner, $values);
    }
    elseif ($type === 'scene') {
      $node = $this->site->createNode('scene', $values + array('uid' => $owner->uid));
    }
    else {
      $node = $this->site->createNode($type, $values + array('uid' => $owner->uid));
    }
    $this->site->forceWorkflowState($node, 'submitted');
    return $node;
  }

  public function testGroupsByTypeWithAttention() {
    $sheet = $this->submitted('character_sheet');
    $item = $this->submitted('items');
    $scene = $this->submitted('scene', array('field_sh_attention_to' => 'Storyhost Bob'));
    $anyScene = $this->submitted('scene');
    $this->login($this->site->createUser(array('storyteller')));
    $groups = $this->groups();
    $this->assertContains($sheet->title . ' (for any A/SH)', $groups['Evaluate Character sheets']);
    $this->assertContains($item->title . ' (for any A/SH)', $groups['Evaluate Items']);
    $this->assertContains($scene->title . ' (for Storyhost Bob)', $groups['Evaluate Scenes']);
    $this->assertContains($anyScene->title . ' (for any A/SH)', $groups['Evaluate Scenes']);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-sh_evaluation")]//a[@href="/node/' . $scene->nid . '"]')->length, 'Rows link to the node');
  }

  public function testOnlySubmittedAndPublished() {
    $draft = $this->site->createCharacter($this->site->createUser(array('player')));
    $this->site->forceWorkflowState($draft, 'draft');
    $approved = $this->submitted('character_sheet');
    $this->site->forceWorkflowState($approved, 'approved');
    $unpublished = $this->submitted('items', array('status' => 0));
    $page = $this->site->createNode('page');
    $this->login($this->site->createUser(array('lead')));
    $text = json_encode($this->groups());
    foreach (array($draft, $approved, $unpublished, $page) as $node) {
      $this->assertStringNotContainsString($node->title, $text);
    }
  }

  public function testWhoSeesIt() {
    $sheet = $this->submitted('character_sheet');
    foreach (array('administrator', 'lead', 'storyteller') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->assertContains($sheet->title . ' (for any A/SH)', $this->groups()['Evaluate Character sheets'], $role);
    }
    $this->login($this->site->createUser(array('player', 'storyteller-assistant')));
    $this->assertNull($this->groups());
    $this->anonymous();
    $this->assertNull($this->groups());
  }

  public function testNewestTenFirst() {
    $sheets = array();
    for ($i = 1; $i <= 11; $i++) {
      $sheets[$i] = $this->submitted('character_sheet');
      $this->site->backdateNodeCreated($sheets[$i], time() + 1000 + $i);
    }
    $this->login($this->site->createUser(array('storyteller')));
    $groups = $this->groups();
    $this->assertSame(array('Evaluate Character sheets'), array_keys($groups));
    $expected = array();
    for ($i = 11; $i >= 2; $i--) {
      $expected[] = $sheets[$i]->title . ' (for any A/SH)';
    }
    $this->assertSame($expected, $groups['Evaluate Character sheets']);
  }

}
