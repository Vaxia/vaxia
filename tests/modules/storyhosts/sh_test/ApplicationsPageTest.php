<?php

namespace Vaxia\Tests\Storyhosts\ShTest;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Applications" tab on user profiles (D7 view sh_applications, page_1): for
 * leads, mediators and storyhosts, the user's published role and storyhost
 * applications, newest first, 35 a page: name, (Open)/(Closed), post date.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group sh_test
 */
class ApplicationsPageTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
  }

  private function application($applicant, $type = 'storyhost_application', array $values = array()) {
    return $this->site->createWebformNode($type, $values + array('uid' => $applicant->uid, 'title' => $this->site->uniqueName('Application')));
  }

  private function rows() {
    $xpath = $this->xpath();
    $rows = array();
    foreach ($xpath->query('//table[.//th[contains(., "Application Status")]]/tbody/tr') as $tr) {
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

  public function testAccessByRole() {
    $applicant = $this->site->createUser(array('player'));
    foreach (array('lead', 'mediator', 'storyteller') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('user/' . $applicant->uid . '/applications');
      $this->assertStatus(200);
      $this->assertText('Applications');
    }
    foreach (array('player', 'storyteller_assistant') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('user/' . $applicant->uid . '/applications');
      $this->assertStatus(403);
    }
    $this->anonymous();
    $this->get('user/' . $applicant->uid . '/applications');
    $this->assertStatus(403);
  }

  public function testListsTheUsersApplicationsNewestFirst() {
    $applicant = $this->site->createUser(array('player'));
    $old = $this->application($applicant, 'application');
    $this->site->backdateNodeCreated($old, strtotime('2025-01-02 10:00'));
    $this->site->setWebformOpen($old, FALSE);
    $new = $this->application($applicant);
    $unpublished = $this->application($applicant, 'storyhost_application', array('status' => 0));
    $someone_elses = $this->application($this->site->createUser(array('player')));

    $this->login($this->site->createUser(array('lead')));
    $this->get('user/' . $applicant->uid);
    $this->assertRaw('href="/user/' . $applicant->uid . '/applications"');
    $this->get('user/' . $applicant->uid . '/applications');
    $rows = $this->rows();
    $this->assertSame(array($new->title, $old->title), array_column($rows, 0));
    $this->assertSame('(Open)', $rows[0][1]);
    $this->assertSame('(Closed)', $rows[1][1]);
    $this->assertNotSame('', $rows[1][2]);
    $this->assertRaw('href="/node/' . $old->nid . '"');
    $this->assertNoText($unpublished->title);
    $this->assertNoText($someone_elses->title);
  }

  public function testPagerShows35() {
    $applicant = $this->site->createUser(array('player'));
    for ($i = 0; $i < 36; $i++) {
      $this->application($applicant);
    }
    $this->login($this->site->createUser(array('lead')));
    $this->get('user/' . $applicant->uid . '/applications');
    $this->assertCount(35, $this->rows());
    $this->get('user/' . $applicant->uid . '/applications', array('page' => 1));
    $this->assertCount(1, $this->rows());
  }

  public function testUnknownUserIsNotFound() {
    $this->login($this->site->createUser(array('lead')));
    $this->get('user/999999999/applications');
    $this->assertStatus(404);
  }

}
