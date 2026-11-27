<?php

namespace Vaxia\Tests\Rpg\RoleApp;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Applications" sidebar block (D7 view applications): for leads and
 * mediators, the newest five published role applications that are open or
 * were changed in the last day, each with its webform status.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group role_app
 * @group views_blocks
 */
class ApplicationsBlockTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'applications-applications', 'sidebar_first');
  }

  /**
   * The block's rows on the front page ("Title (Open)"), or NULL when it isn't
   * shown.
   */
  private function rows() {
    $this->get('node');
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-applications ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query('.//li', $view->item(0)) as $li) {
      $rows[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
    }
    return $rows;
  }

  private function application(array $values = array()) {
    return $this->site->createWebformNode('application', $values + array('title' => $this->site->uniqueName('Application')));
  }

  public function testLeadsAndMediatorsSeeOpenApplications() {
    $app = $this->application();
    $this->login($this->site->createUser(array('lead')));
    $this->assertContains($app->title . ' (Open)', $this->rows());
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-applications")]//a[@href="/node/' . $app->nid . '" and text()="' . $app->title . '"]')->length, 'Title links to the application');

    $this->login($this->site->createUser(array('mediator')));
    $this->assertContains($app->title . ' (Open)', $this->rows());
  }

  public function testHiddenFromOthers() {
    $this->application();
    $this->login($this->site->createUser(array('player', 'storyteller')));
    $this->assertNull($this->rows());
    $this->anonymous();
    $this->assertNull($this->rows());
  }

  public function testClosedApplicationsShowForOneDay() {
    $recent = $this->application();
    $this->site->setWebformOpen($recent, FALSE);
    $old = $this->application();
    $this->site->setWebformOpen($old, FALSE);
    $this->site->backdateNode($old, time() - 2 * 86400);
    $this->login($this->site->createUser(array('lead')));
    $rows = $this->rows();
    $this->assertContains($recent->title . ' (Closed)', $rows);
    $this->assertNotContains($old->title . ' (Closed)', $rows);
  }

  public function testOnlyPublishedApplications() {
    $unpublished = $this->application(array('status' => 0));
    $other = $this->site->createWebformNode('storyhost_application', array('title' => $this->site->uniqueName('ShApp')));
    $this->login($this->site->createUser(array('lead')));
    $text = implode("\n", (array) $this->rows());
    $this->assertStringNotContainsString($unpublished->title, $text);
    $this->assertStringNotContainsString($other->title, $text);
  }

  public function testNewestFiveFirst() {
    $apps = array();
    for ($i = 1; $i <= 6; $i++) {
      $apps[$i] = $this->application();
      // Newer than anything else on the site.
      $this->site->backdateNodeCreated($apps[$i], time() + 1000 + $i);
    }
    $this->login($this->site->createUser(array('lead')));
    $this->assertSame(array(
      $apps[6]->title . ' (Open)',
      $apps[5]->title . ' (Open)',
      $apps[4]->title . ' (Open)',
      $apps[3]->title . ' (Open)',
      $apps[2]->title . ' (Open)',
    ), $this->rows());
  }

  public function testEmptyListHidesBlock() {
    $this->site->deleteNodesOfType('application');
    $this->login($this->site->createUser(array('lead')));
    $this->assertNull($this->rows());
  }

}
