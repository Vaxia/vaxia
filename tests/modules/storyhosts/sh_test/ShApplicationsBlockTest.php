<?php

namespace Vaxia\Tests\Storyhosts\ShTest;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "SH Applications" sidebar block (D7 view sh_applications): for those who may
 * view SH test results, the newest five published storyhost applications
 * that are open or were changed in the last day, by applicant name.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group sh_test
 * @group views_blocks
 */
class ShApplicationsBlockTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'sh_applications-sh_applications', 'sidebar_first');
  }

  /**
   * The block's rows on the front page, or NULL when it isn't shown.
   */
  private function rows() {
    $this->get('node');
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-sh_applications ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query('.//li', $view->item(0)) as $li) {
      $rows[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
    }
    return $rows;
  }

  private function application($applicant, array $values = array()) {
    return $this->site->createWebformNode('storyhost_application', $values + array('uid' => $applicant->uid, 'title' => $this->site->uniqueName('ShApp')));
  }

  private function viewer() {
    return $this->site->createUser(array('player'), array('view sh test results'));
  }

  public function testListsApplicantsWithStatus() {
    $applicant = $this->site->createUser(array('player'));
    $app = $this->application($applicant);
    $this->login($this->viewer());
    $this->assertContains($applicant->name . ' (Open)', $this->rows());
    // The applicant's name links to the application.
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-sh_applications")]//a[@href="/node/' . $app->nid . '" and text()="' . $applicant->name . '"]')->length);
    $this->assertStringNotContainsString($app->title, implode("\n", $this->rows()));
  }

  public function testStorytellersSeeIt() {
    $applicant = $this->site->createUser(array('player'));
    $this->application($applicant);
    $this->login($this->site->createUser(array('storyteller')));
    $this->assertContains($applicant->name . ' (Open)', $this->rows());
  }

  public function testHiddenWithoutPermission() {
    $this->application($this->site->createUser(array('player')));
    $this->withoutPermission('view sh test results', array('anonymous user', 'authenticated user', 'player'), function () {
      $this->login($this->site->createUser(array('player')));
      $this->assertNull($this->rows());
      $this->anonymous();
      $this->assertNull($this->rows());
    });
  }

  public function testClosedApplicationsShowForOneDay() {
    $recent = $this->site->createUser(array('player'));
    $this->site->setWebformOpen($this->application($recent), FALSE);
    $old = $this->site->createUser(array('player'));
    $oldApp = $this->application($old);
    $this->site->setWebformOpen($oldApp, FALSE);
    $this->site->backdateNode($oldApp, time() - 2 * 86400);
    $unpublished = $this->site->createUser(array('player'));
    $this->application($unpublished, array('status' => 0));
    $roleApp = $this->site->createUser(array('player'));
    $this->site->createWebformNode('application', array('uid' => $roleApp->uid));

    $this->login($this->viewer());
    $rows = $this->rows();
    $this->assertContains($recent->name . ' (Closed)', $rows);
    $text = implode("\n", $rows);
    $this->assertStringNotContainsString($old->name, $text);
    $this->assertStringNotContainsString($unpublished->name, $text);
    $this->assertStringNotContainsString($roleApp->name, $text);
  }

  public function testNewestFiveFirst() {
    $names = array();
    for ($i = 1; $i <= 6; $i++) {
      $applicant = $this->site->createUser(array('player'));
      $this->site->backdateNodeCreated($this->application($applicant), time() + 1000 + $i);
      $names[$i] = $applicant->name . ' (Open)';
    }
    $this->login($this->viewer());
    $this->assertSame(array($names[6], $names[5], $names[4], $names[3], $names[2]), $this->rows());
  }

}
