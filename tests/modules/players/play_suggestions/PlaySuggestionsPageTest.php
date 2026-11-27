<?php

namespace Vaxia\Tests\Players\PlaySuggestions;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The play-suggestions page (D7 view play_suggestions): staff (administrator,
 * lead, storyteller) see every published suggestion, newest first, 35 a page,
 * with its realm, and can delete suggestions in bulk.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group play_suggestions
 * @group views_pages
 */
class PlaySuggestionsPageTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->deleteNodesOfType('play_suggestions');
  }

  private function suggestion($title, $realm = 'Vaxia', $status = 1) {
    return $this->site->createNode('play_suggestions', array('title' => $title, 'status' => $status, 'field_tag_realm' => $this->site->term('realm', $realm)));
  }

  /**
   * Titles in the table, in page order.
   */
  private function titles() {
    $titles = array();
    foreach ($this->xpath()->query('//table//tbody/tr/td[3]') as $td) {
      $titles[] = trim($td->textContent);
    }
    return $titles;
  }

  public function testOnlyStaffGetIn() {
    foreach (array('administrator', 'lead', 'storyteller') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('play-suggestions');
      $this->assertStatus(200);
      $this->assertText('Play suggestions');
    }
    foreach (array('player', 'storyteller-assistant') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('play-suggestions');
      $this->assertStatus(403);
    }
    $this->anonymous();
    $this->get('play-suggestions');
    $this->assertStatus(403);
  }

  public function testPublishedSuggestionsNewestFirstWithRealm() {
    $old = $this->suggestion('go fishing', 'Sirian');
    $this->site->backdateNodeCreated($old, time() - 3600);
    $this->suggestion('climb a tree');
    $this->suggestion('hidden idea', 'Vaxia', 0);

    $this->login($this->site->createUser(array('lead')));
    $this->get('play-suggestions');
    $this->assertSame(array('climb a tree', 'go fishing'), $this->titles());
    $this->assertSame('Sirian', trim($this->xpath()->query('//table//tbody/tr[2]/td[2]')->item(0)->textContent));
    $this->assertRaw('href="/node/' . $old->nid . '"');
    $this->assertSame(array('Realm', 'Title'), array_values(array_filter(array_map('trim', array_map(function ($th) {
      return $th->textContent;
    }, iterator_to_array($this->xpath()->query('//table//thead//th')))))));
  }

  public function testThirtyFiveAPage() {
    for ($i = 1; $i <= 36; $i++) {
      $node = $this->suggestion('idea ' . $i);
      $this->site->backdateNodeCreated($node, time() - 100 + $i);
    }
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('play-suggestions');
    $this->assertCount(35, $this->titles());
    $this->assertSame('idea 36', $this->titles()[0]);
    $this->get('play-suggestions', array('page' => 1));
    $this->assertSame(array('idea 1'), $this->titles());
  }

  public function testBulkDelete() {
    $gone = $this->suggestion('delete me');
    $kept = $this->suggestion('keep me');
    $this->login($this->site->createUser(array('administrator')));
    $this->get('play-suggestions');
    $this->submitForm(NULL, array('nodes[' . $gone->nid . ']' => TRUE), 'Delete item');
    $this->assertText('delete me');
    $this->submitForm(NULL, array(), 'Delete');
    $this->get('play-suggestions');
    $this->assertSame(array('keep me'), $this->titles());
  }

  public function testBulkDeleteNeedsASelection() {
    $this->suggestion('lonely');
    $this->login($this->site->createUser(array('administrator')));
    $this->submitForm('play-suggestions', array(), 'Delete item');
    $this->assertText('Please select at least one item.');
  }

}
