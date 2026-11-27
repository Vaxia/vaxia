<?php

namespace Vaxia\Tests\Vaxia\VaxiaWiki;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia wiki: wiki flags (delete, rewrite, expand) and lists. Players may flag
 * a page for deletion (on D7 that unpublishes it at once; unflagging publishes
 * it again). Admins, leads and storytellers review flagged pages; everyone gets
 * the recently edited,
 * NPC, organization and governance lists.
 *
 * @group vaxia_wiki
 */
class VaxiaWikiTest extends VaxiaTestCase {

  private function wiki($title = NULL) {
    return $this->site->createNode('wiki_page', array(
      'title' => $title ?: $this->site->uniqueName('Lore'),
      'body' => array(array('value' => 'Some lore.', 'format' => 'wiki')),
    ));
  }

  /**
   * Follows the flag or unflag link for $flag on the node's page.
   */
  private function toggleFlag($node, $action, $flag = 'delete') {
    $this->get('node/' . $node->nid);
    $link = $this->xpath()->query('//a[contains(@href, "flag/' . $action . '/' . $flag . '/' . $node->nid . '")]')->item(0);
    $this->assertNotNull($link, "'$action $flag' link on node {$node->nid}");
    $this->get(html_entity_decode($link->getAttribute('href')));
  }

  private function isPublic($node) {
    return $this->inSession($this->site->createUser(array('player')), function () use ($node) {
      $this->get('node/' . $node->nid);
      return $this->status === 200;
    });
  }

  /**
   * @group knownbug
   * Flagging for deletion unpublishes the page at once: one player can hide
   * any wiki page. Decided for the port (Phase 1 #90): the flag queues the
   * page for staff review and it stays published.
   */
  public function testFlaggingForDeletionKeepsPagePublished() {
    $page = $this->wiki();
    $this->login($this->site->createUser(array('player')));
    $this->toggleFlag($page, 'flag');
    $this->assertTrue($this->isPublic($page));
  }

  public function testUnflaggingPublishesAgain() {
    $page = $this->wiki();
    $this->login($this->superuser());
    $this->toggleFlag($page, 'flag');
    $this->toggleFlag($page, 'unflag');
    $this->assertTrue($this->isPublic($page));
  }

  public function testStaffSeePagesFlaggedForRewrite() {
    $page = $this->wiki();
    $this->login($this->site->createUser(array('player'), array('flag rewrite', 'unflag rewrite')));
    $this->toggleFlag($page, 'flag', 'rewrite');
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('wiki/flagged-rewrite');
    $this->assertStatus(200);
    $this->assertText($page->title);
  }

  /**
   * @group knownbug
   * Flagging for deletion unpublishes the page, and the "Flagged delete" list
   * only shows published pages: pages flagged for deletion never appear on the
   * list meant for reviewing them.
   */
  public function testStaffSeePagesFlaggedForDeletion() {
    $page = $this->wiki();
    $this->login($this->site->createUser(array('player')));
    $this->toggleFlag($page, 'flag');
    $this->login($this->site->createUser(array('storyteller')));
    $this->get('wiki/flagged-delete');
    $this->assertStatus(200);
    $this->assertText($page->title);
  }

  public function testPlayersCannotSeeTheFlaggedLists() {
    $this->login($this->site->createUser(array('player')));
    foreach (array('wiki/flagged-delete', 'wiki/flagged-rewrite', 'wiki/flagged-expand') as $path) {
      $this->get($path);
      $this->assertStatus(403, $path);
    }
  }

  public function testRecentlyEditedListsPages() {
    $page = $this->wiki();
    $this->login($this->site->createUser(array('player')));
    $this->get('wiki/recently-edited');
    $this->assertStatus(200);
    $this->assertText($page->title);
  }

  public function publicLists() {
    return array(array('wiki/npcs-list'), array('wiki/organizations-list'), array('wiki/site-governance'), array('wiki/fantasy/session-list'), array('search/wiki'));
  }

  /**
   * @dataProvider publicLists
   */
  public function testPublicListsLoad($path) {
    $this->site->term('realm', 'Fantasy');
    $this->login($this->site->createUser(array('player')));
    $this->get($path);
    $this->assertStatus(200);
  }

}
