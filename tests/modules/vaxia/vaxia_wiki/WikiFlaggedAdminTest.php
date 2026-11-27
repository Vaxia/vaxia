<?php

namespace Vaxia\Tests\Vaxia\VaxiaWiki;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Site wiki flagged (admin/content/wiki-flagged): an administrators' table of
 * wiki pages, 35 a page, most recently updated first, with filters on
 * publishing (published by default), title, and each wiki flag (expand,
 * rewrite, delete; "- Any -" by default, so every wiki page is listed).
 * A "Site wiki flagged" tab on the content admin page.
 *
 * D11 only: the D7 test site doesn't have the site_wiki_admin view.
 *
 * @group vaxia_wiki
 */
class WikiFlaggedAdminTest extends VaxiaTestCase {

  /** @var string Title prefix of this test's pages. */
  private $prefix;

  /** @var object An administrator. */
  private $admin;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The wiki flagged admin page was ported to the D11 site only.');
    }
    $this->prefix = $this->site->uniqueName('Lore');
    $this->admin = $this->site->createUser(array('administrator'));
  }

  private function wiki(array $values = array()) {
    return $this->site->createNode('wiki_page', $values + array(
      'title' => $this->prefix . ' ' . $this->site->uniqueName('page'),
      'body' => array(array('value' => 'Some lore.', 'format' => 'wiki')),
    ));
  }

  private function flag($node, $flag) {
    $this->inSession($this->admin, function () use ($node, $flag) {
      $this->get('node/' . $node->nid);
      $link = $this->xpath()->query('//a[contains(@href, "flag/flag/' . $flag . '/' . $node->nid . '")]')->item(0);
      $this->assertNotNull($link, "flag $flag link on node {$node->nid}");
      $this->get(html_entity_decode($link->getAttribute('href')));
    });
  }

  /**
   * Titles listed for this test's pages, as an administrator, in page order.
   */
  private function listed(array $query = array()) {
    $this->login($this->admin);
    $this->get('admin/content/wiki-flagged', $query + array('title' => $this->prefix));
    $this->assertStatus(200);
    $titles = array();
    foreach ($this->xpath()->query('//table//tbody/tr/td[1][not(@colspan)]') as $td) {
      $titles[] = trim($td->textContent);
    }
    return $titles;
  }

  public function testOnlyAdministratorsMayOpenIt() {
    $this->login($this->site->createUser(array('administrator')));
    $this->get('admin/content/wiki-flagged');
    $this->assertStatus(200);
    $this->assertText('Site wiki flagged');
    foreach (array('lead', 'storyteller', 'player') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('admin/content/wiki-flagged');
      $this->assertStatus(403);
    }
    $this->anonymous();
    $this->get('admin/content/wiki-flagged');
    $this->assertStatus(403);
  }

  public function testIsATabOfTheContentAdminPage() {
    $this->login($this->admin);
    $this->get('admin/content');
    $this->assertNotNull($this->xpath()->query('//a[contains(@href, "/admin/content/wiki-flagged")]')->item(0));
  }

  public function testListsPublishedWikiPagesWithEditAndDeleteLinks() {
    $page = $this->wiki();
    $unpublished = $this->wiki(array('status' => 0));
    $other = $this->site->createNode('session_report', array('title' => $this->prefix . ' session'));
    $this->assertSame(array($page->title), $this->listed());
    $this->assertText('Last updated');
    $this->assertNotNull($this->xpath()->query('//a[contains(@href, "/node/' . $page->nid . '/edit")]')->item(0));
    $this->assertNotNull($this->xpath()->query('//a[contains(@href, "/node/' . $page->nid . '/delete")]')->item(0));
    $this->assertNotContains($unpublished->title, $this->listed());
    $this->assertNotContains($other->title, $this->listed());
  }

  public function testPublishedFilter() {
    $page = $this->wiki();
    $unpublished = $this->wiki(array('status' => 0));
    $this->assertSame(array($unpublished->title), $this->listed(array('status' => '0')));
    $this->assertEqualsCanonicalizing(array($page->title, $unpublished->title), $this->listed(array('status' => 'All')));
  }

  public function testFlagFilters() {
    $plain = $this->wiki();
    $expand = $this->wiki();
    $rewrite = $this->wiki();
    $delete = $this->wiki();
    $this->flag($expand, 'expand');
    $this->flag($rewrite, 'rewrite');
    $this->flag($delete, 'delete');
    $this->assertEqualsCanonicalizing(array($plain->title, $expand->title, $rewrite->title, $delete->title), $this->listed());
    $this->assertSame(array($expand->title), $this->listed(array('flagged_expand' => '1')));
    $this->assertSame(array($rewrite->title), $this->listed(array('flagged_rewrite' => '1')));
    $this->assertSame(array($delete->title), $this->listed(array('flagged_delete' => '1')));
    $this->assertEqualsCanonicalizing(array($plain->title, $expand->title, $rewrite->title), $this->listed(array('flagged_delete' => '0')));
    $this->assertSame(array($plain->title), $this->listed(array('flagged_expand' => '0', 'flagged_rewrite' => '0', 'flagged_delete' => '0')));
  }

  public function testMostRecentlyUpdatedFirst() {
    $old = $this->wiki();
    $new = $this->wiki();
    $older = $this->wiki();
    $this->site->backdateNode($old, time() - 2000);
    $this->site->backdateNode($new, time() - 1000);
    $this->site->backdateNode($older, time() - 3000);
    $this->assertSame(array($new->title, $old->title, $older->title), $this->listed());
  }

  public function testThirtyFivePerPage() {
    for ($i = 0; $i < 36; $i++) {
      $this->wiki();
    }
    $this->assertCount(35, $this->listed());
    $this->assertCount(1, $this->listed(array('page' => 1)));
  }

  public function testEmptyText() {
    $this->assertSame(array(), $this->listed(array('title' => $this->prefix . ' nothing')));
    $this->assertText('No articles flagged for deletion.');
  }

}
