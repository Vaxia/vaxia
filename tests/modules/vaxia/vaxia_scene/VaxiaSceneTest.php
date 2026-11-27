<?php

namespace Vaxia\Tests\Vaxia\VaxiaScene;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia scene: a new scene sends its author to the Workflow tab with a reminder
 * to submit it. Submitting mails the scene rulers and confirms to the author;
 * ruling mails the author.
 *
 * @group vaxia_scene
 */
class VaxiaSceneTest extends VaxiaTestCase {

  private function mailsTo($account, $subject) {
    return array_values(array_filter($this->sentMail(), function ($m) use ($account, $subject) {
      return $m['to'] === $account->mail && $m['subject'] === $subject;
    }));
  }

  /**
   * Creates a scene through the form; leaves the browser where it lands.
   */
  private function createSceneByForm() {
    $author = $this->site->createUser(array('player'), array('create scene content'));
    $character = $this->site->createCharacter($author, array('title' => $this->site->uniqueName('Hero')));
    $this->login($author);
    $this->get('node/add/scene');
    $forum = $this->xpath()->query('//select[@name="' . $this->fieldName('taxonomy_forums[und]') . '"]/option[@value!="_none"]')->item(0)->getAttribute('value');
    $this->submitForm(NULL, array(
      'title' => $this->site->uniqueName('Scene'),
      'taxonomy_forums[und]' => $forum,
      'field_scene_character[und][0][nid]' => $this->referenceValue($character->title, $character->nid),
    ), 'Save');
  }

  public function testNewSceneRemindsToSubmit() {
    $this->createSceneByForm();
    $this->assertStringContainsString("Don't forget to set the scene to submitted", implode(' ', $this->messages()));
  }

  /**
   * @group knownbug
   * "Creating scene move to workflow" redirects to the Workflow tab, but
   * vaxia_players' "Node redirect after creation" also lists scenes and
   * redirects to node/N afterwards, so the author lands on the scene page.
   */
  public function testNewSceneGoesToTheWorkflowTab() {
    $this->createSceneByForm();
    $this->assertMatchesRegularExpression('#/node/\d+/workflow$#', $this->url);
  }

  public function testSubmittingMailsTheRulers() {
    $admin = $this->site->createUser(array('administrator'));
    $author = $this->site->createUser(array('player'));
    $scene = $this->site->createScene($author, array());

    $messages = $this->transition($scene, 'submitted', $author);

    $this->assertStringContainsString('Your scene has been submitted', implode(' ', $messages));
    $this->assertCount(1, $this->mailsTo($admin, ' [Vaxia] A scene is ready to be ruled'));
  }

  public function testRulingMailsTheAuthor() {
    $author = $this->site->createUser(array('player'));
    $scene = $this->site->createScene($author, array());
    $this->transition($scene, 'submitted', $author);
    $this->transition($scene, 'ruled');
    $mails = $this->mailsTo($author, '[Vaxia] Your scene has been ruled');
    $this->assertCount(1, $mails);
    $this->assertStringContainsString($scene->title ?? '', $mails[0]['body']);
  }

}
