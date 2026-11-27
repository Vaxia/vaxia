<?php

namespace Vaxia\Tests\Wiki\SiteWiki;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Site wiki search" block (D7 view site_wiki_admin, site_wiki_search): wiki
 * pages by published status (default: published), title and category, most
 * recently updated first, each with the start of its text. Nothing is listed
 * until a search is made.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group site_wiki
 * @group views_blocks
 */
class WikiSearchBlockTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'site_wiki_admin-site_wiki_search', 'content');
  }

  /**
   * The block on the front page searched with $query: rows, empty text and
   * whole text; NULL when it isn't shown.
   */
  private function search(array $query = array()) {
    $this->get('node', $query);
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-site_wiki_search ")]');
    if (!$view->length) {
      return NULL;
    }
    $rows = array();
    foreach ($xpath->query('.//div[contains(@class, "view-content")]//li', $view->item(0)) as $li) {
      $rows[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
    }
    $empty = $xpath->query('.//div[contains(@class, "view-empty")]', $view->item(0))->item(0);
    return array(
      'rows' => $rows,
      'empty' => $empty ? trim($empty->textContent) : NULL,
      'text' => trim(preg_replace('/\s+/', ' ', $view->item(0)->textContent)),
    );
  }

  private function page($title, $body = 'Some lore.', array $values = array()) {
    return $this->site->createNode('wiki_page', $values + array(
      'title' => $title,
      'body' => array(array('value' => $body, 'format' => 'filtered_html')),
    ));
  }

  public function testNothingUntilSearched() {
    $this->page($this->site->uniqueName('Lore'));
    $this->anonymous();
    $block = $this->search();
    $this->assertSame(array(), $block['rows']);
    $this->assertNull($block['empty']);
    $this->assertStringContainsString('Search for a wiki article by tag(s).', $block['text']);
    $xpath = $this->xpath();
    foreach (array('title', 'field_additional_tagging_tid', 'field_additional_tagging_tid_op') as $name) {
      $this->assertSame(1, $xpath->query('//div[contains(@class, "view-display-id-site_wiki_search")]//form[@method="get"]//*[@name="' . $name . '"]')->length, $name);
    }
    // Security fix: only administrators and leads get the Published filter.
    $this->assertSame(0, $xpath->query('//select[@name="status"]')->length, 'No Published filter for visitors');
    $this->login($this->site->createUser(array('lead')));
    $this->search();
    $this->assertSame(1, $this->xpath()->query('//select[@name="status"]/option[@value="1" and @selected]')->length, 'Published by default');
  }

  public function testSearchByTitle() {
    $key = $this->site->uniqueName('Dragon');
    $older = $this->page($key . ' lairs', '<p>Where <em>dragons</em> sleep.</p>');
    $this->site->backdateNode($older, time() - 3600);
    $newer = $this->page('Big ' . $key);
    $unpublished = $this->page($key . ' draft', 'x', array('status' => 0));
    $notWiki = $this->site->createNode('page', array('title' => $key . ' page'));
    $this->anonymous();
    $block = $this->search(array('title' => $key));
    $this->assertSame(array($newer->title . ' - Type Some lore.', $older->title . ' - Type Where dragons sleep.'), $block['rows']);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-display-id-site_wiki_search")]//a[@href="/node/' . $older->nid . '"]')->length);
    $this->assertStringNotContainsString($unpublished->title, $block['text']);
    $this->assertStringNotContainsString($notWiki->title, $block['text']);
  }

  public function testPublishedFilter() {
    $key = $this->site->uniqueName('Ruins');
    $published = $this->page($key . ' standing');
    $unpublished = $this->page($key . ' fallen', 'Gone.', array('status' => 0));
    $titles = function ($rows) {
      return array_map(function ($row) {
        return explode(' - ', $row)[0];
      }, $rows);
    };
    // Security fix (D7 let anyone list unpublished pages): a player asking
    // for unpublished pages gets published ones only.
    $this->login($this->site->createUser(array('player')));
    $this->assertSame(array($published->title), $titles($this->search(array('title' => $key, 'status' => '0'))['rows']));
    $this->assertSame(array($published->title), $titles($this->search(array('title' => $key, 'status' => 'All'))['rows']));

    $this->login($this->site->createUser(array('administrator')));
    $this->assertSame(array($unpublished->title), $titles($this->search(array('title' => $key, 'status' => '0'))['rows']));
    $all = $titles($this->search(array('title' => $key, 'status' => 'All'))['rows']);
    sort($all);
    $this->assertSame(array($unpublished->title, $published->title), $all);
  }

  public function testLeadsSearchAndOpenUnpublishedPages() {
    // D7 view_unpublished gave leads 'view any unpublished wiki_page content'.
    $key = $this->site->uniqueName('Vault');
    $unpublished = $this->page($key . ' sealed', 'Hidden.', array('status' => 0));
    $this->login($this->site->createUser(array('lead')));
    $rows = $this->search(array('title' => $key, 'status' => '0'))['rows'];
    $this->assertCount(1, $rows);
    $this->assertStringStartsWith($unpublished->title, $rows[0]);
    $this->get('node/' . $unpublished->nid);
    $this->assertStatus(200);

    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $unpublished->nid);
    $this->assertStatus(403);
  }

  public function testExcerptIsTrimmed() {
    $key = $this->site->uniqueName('Saga');
    $words = str_repeat('alpha beta gamma delta ', 10);
    $this->page($key, $words);
    $this->anonymous();
    $row = $this->search(array('title' => $key))['rows'][0];
    $excerpt = substr($row, strlen($key . ' - Type '));
    $this->assertStringEndsWith('...', $excerpt);
    $this->assertLessThanOrEqual(143, strlen($excerpt));
    $this->assertStringStartsWith('alpha beta gamma delta alpha', $excerpt);
    // Cut at a word boundary.
    $this->assertMatchesRegularExpression('/(alpha|beta|gamma|delta)\.\.\.$/', $excerpt);
  }

  public function testSearchByCategory() {
    $key = $this->site->uniqueName('Guild');
    $trade = $this->site->term('wiki_tags', 'Trade ' . $key);
    $magic = $this->site->term('wiki_tags', 'Magic ' . $key);
    $both = $this->page($key . ' both', 'x', array('field_additional_tagging' => array(array('target_id' => $trade), array('target_id' => $magic))));
    $tradeOnly = $this->page($key . ' trade', 'x', array('field_additional_tagging' => array(array('target_id' => $trade))));
    $none = $this->page($key . ' none', 'x');
    $this->anonymous();
    $titles = function ($rows) {
      $titles = array_map(function ($row) {
        return explode(' - ', $row)[0];
      }, $rows);
      sort($titles);
      return $titles;
    };
    $this->assertSame(array($both->title, $tradeOnly->title), $titles($this->search(array('title' => $key, 'field_additional_tagging_tid' => 'Trade ' . $key))['rows']));
    $this->assertSame(array($both->title), $titles($this->search(array('title' => $key, 'field_additional_tagging_tid_op' => 'and', 'field_additional_tagging_tid' => 'Trade ' . $key . ', Magic ' . $key))['rows']));
    $this->assertSame(array($none->title), $titles($this->search(array('title' => $key, 'field_additional_tagging_tid_op' => 'not', 'field_additional_tagging_tid' => 'Trade ' . $key))['rows']));
  }

  public function testNoMatches() {
    $this->anonymous();
    $block = $this->search(array('title' => $this->site->uniqueName('Nothing')));
    $this->assertSame(array(), $block['rows']);
    $this->assertSame('No wiki pages found matching, or no search performed.', $block['empty']);
  }

  public function testHiddenWithoutAccessContent() {
    $this->withoutPermission('access content', array('anonymous user'), function () {
      $this->anonymous();
      $this->get('user/login');
      $this->assertSame(0, $this->xpath()->query('//div[contains(@class, "view-display-id-site_wiki_search")]')->length);
    });
  }

}
