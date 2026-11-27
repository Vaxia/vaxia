<?php

namespace Vaxia\Tests\Ui\Favorites;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "My Favorites" (D7 favorites 7.x-2.1, block favorites/0): each user's saved
 * links in the sidebar, "add this page" (titled with the page title by
 * default, keeping the query string), an "x" to remove one, and drag to
 * reorder. Only for "manage own favorites" (authenticated users, players and
 * administrators in production; never anonymous).
 *
 * The D7 test site doesn't have the favorites module, so these run on the
 * port only.
 *
 * @group favorites
 */
class FavoritesTest extends VaxiaTestCase {

  const FORM = 'favorites-add-favorite-form';

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have the favorites module.');
    }
    $this->site->placeBlock('favorites', '0', 'sidebar_first');
  }

  private function member() {
    return $this->site->createUser(array('player'), array('manage own favorites'));
  }

  /**
   * The favorites listed on the last page: title => href, in order.
   */
  private function favorites() {
    $list = array();
    $xpath = $this->xpath();
    foreach ($xpath->query('//div[@id="favorites-list"]//li') as $li) {
      $link = $xpath->query('.//a[not(contains(@class, "favorites-remove"))]', $li)->item(0);
      $list[trim($link->textContent)] = $link->getAttribute('href');
    }
    return $list;
  }

  private function removeLink($title) {
    $li = $this->xpath()->query('//div[@id="favorites-list"]//li[a[normalize-space(.)="' . $title . '"]]/a[contains(@class, "favorites-remove")]');
    $this->assertSame(1, $li->length, 'Remove link for ' . $title);
    return html_entity_decode($li->item(0)->getAttribute('href'));
  }

  private function page($title) {
    return $this->site->createNode('page', array('title' => $title));
  }

  public function testOnlyForUsersWithThePermission() {
    $this->anonymous();
    $this->get('user/login');
    $this->assertSame(0, $this->xpath()->query('//form[@id="' . self::FORM . '"]')->length);

    // Production grants it to authenticated users and players (as D7 did).
    $this->withoutPermission('manage own favorites', array('authenticated user', 'player'), function () {
      $this->login($this->site->createUser(array('player')));
      $this->get('node');
      $this->assertSame(0, $this->xpath()->query('//form[@id="' . self::FORM . '"]')->length);
    });

    $this->login($this->member());
    $this->get('node');
    $this->assertSame(1, $this->xpath()->query('//form[@id="' . self::FORM . '"]')->length);
    $this->assertText('add this page');
    $this->assertNoPhpErrors();
  }

  public function testTitleDefaultsToThePageTitle() {
    $node = $this->page($this->site->uniqueName('Tavern'));
    $this->login($this->member());
    $this->get('node/' . $node->nid);
    $field = $this->xpath()->query('//form[@id="' . self::FORM . '"]//input[@name="title"]')->item(0);
    $this->assertSame($node->title, $field->getAttribute('value'));
  }

  public function testAddingAPageListsIt() {
    $node = $this->page($this->site->uniqueName('Market'));
    $this->login($this->member());
    $this->submitForm('node/' . $node->nid, array('title' => 'My market'), 'Add', self::FORM);
    $list = $this->favorites();
    $this->assertSame(array('My market'), array_keys($list));
    $this->assertMatchesRegularExpression('#/node/' . $node->nid . '$#', $list['My market']);
    $this->assertNoPhpErrors();
  }

  public function testQueryStringIsKept() {
    $node = $this->page($this->site->uniqueName('Archive'));
    $this->login($this->member());
    $this->submitForm('node/' . $node->nid . '?page=2&sort=asc', array('title' => 'Page two'), 'Add', self::FORM);
    $this->get('node');
    $href = html_entity_decode($this->favorites()['Page two']);
    $this->assertMatchesRegularExpression('#/node/' . $node->nid . '\?page=2&sort=asc$#', $href);
  }

  public function testAddingTheSamePageAgainMovesItToTheEnd() {
    $first = $this->page($this->site->uniqueName('First'));
    $second = $this->page($this->site->uniqueName('Second'));
    $this->login($this->member());
    $this->submitForm('node/' . $first->nid, array('title' => 'One'), 'Add', self::FORM);
    $this->submitForm('node/' . $second->nid, array('title' => 'Two'), 'Add', self::FORM);
    $this->submitForm('node/' . $first->nid, array('title' => 'One again'), 'Add', self::FORM);
    $this->assertSame(array('Two', 'One again'), array_keys($this->favorites()));
  }

  public function testRemovingAFavorite() {
    $node = $this->page($this->site->uniqueName('Docks'));
    $other = $this->page($this->site->uniqueName('Gate'));
    $this->login($this->member());
    $this->submitForm('node/' . $node->nid, array('title' => 'Docks'), 'Add', self::FORM);
    $this->submitForm('node/' . $other->nid, array('title' => 'Gate'), 'Add', self::FORM);
    $this->get($this->removeLink('Docks'));
    // Back on the page it was removed from.
    $this->assertMatchesRegularExpression('#/node/' . $other->nid . '$#', $this->url);
    $this->assertSame(array('Gate'), array_keys($this->favorites()));
  }

  public function testCannotRemoveWithoutTheToken() {
    $node = $this->page($this->site->uniqueName('Keep'));
    $this->login($this->member());
    $this->submitForm('node/' . $node->nid, array('title' => 'Keep me'), 'Add', self::FORM);
    $this->get(preg_replace('/token=[^&]+/', 'token=forged', $this->removeLink('Keep me')));
    $this->assertText('You do not have permission to remove this favorite.');
    $this->assertSame(array('Keep me'), array_keys($this->favorites()));
  }

  public function testCannotRemoveSomeoneElsesFavorite() {
    $node = $this->page($this->site->uniqueName('Mine'));
    $owner = $this->member();
    $this->login($owner);
    $this->submitForm('node/' . $node->nid, array('title' => 'Mine'), 'Add', self::FORM);
    $link = $this->removeLink('Mine');

    $this->login($this->member());
    $this->get($link);
    $this->assertText('You do not have permission to remove this favorite.');

    $this->login($owner);
    $this->get('node');
    $this->assertSame(array('Mine'), array_keys($this->favorites()));
  }

  public function testFavoritesAreYourOwn() {
    $node = $this->page($this->site->uniqueName('Private'));
    $this->login($this->member());
    $this->submitForm('node/' . $node->nid, array('title' => 'Only mine'), 'Add', self::FORM);
    $this->login($this->member());
    $this->get('node/' . $node->nid);
    $this->assertSame(array(), $this->favorites());
  }

  /**
   * The reorder callback path (with its token) from drupalSettings.
   */
  private function orderPath() {
    $json = $this->xpath()->query('//script[@data-drupal-selector="drupal-settings-json"]')->item(0);
    return json_decode($json->textContent, TRUE)['favorites']['orderPath'];
  }

  /**
   * fids in list order.
   */
  private function fids() {
    $fids = array();
    foreach ($this->xpath()->query('//div[@id="favorites-list"]//a[contains(@class, "favorites-remove")]') as $a) {
      $fids[] = $a->getAttribute('fid');
    }
    return $fids;
  }

  public function testDraggingReordersTheList() {
    $this->login($this->member());
    foreach (array('A', 'B', 'C') as $title) {
      $node = $this->page($this->site->uniqueName($title));
      $this->submitForm('node/' . $node->nid, array('title' => $title), 'Add', self::FORM);
    }
    $this->assertSame(array('A', 'B', 'C'), array_keys($this->favorites()));
    $this->assertRaw('/js/favorites.js');
    $fids = $this->fids();
    $this->request('POST', $this->orderPath(), http_build_query(array('fids' => array($fids[2], $fids[0], $fids[1]))), array(), array('Content-Type: application/x-www-form-urlencoded'));
    $this->assertStatus(200);
    $this->assertSame(array('success' => TRUE), json_decode($this->content, TRUE));
    $this->get('node');
    $this->assertSame(array('C', 'A', 'B'), array_keys($this->favorites()));
  }

  public function testReorderNeedsTheTokenAndOnlyTouchesYourOwn() {
    $owner = $this->member();
    $this->login($owner);
    foreach (array('A', 'B') as $title) {
      $node = $this->page($this->site->uniqueName($title));
      $this->submitForm('node/' . $node->nid, array('title' => $title), 'Add', self::FORM);
    }
    $fids = $this->fids();
    $this->request('POST', preg_replace('/token=[^&]+/', 'token=forged', $this->orderPath()), http_build_query(array('fids' => array($fids[1], $fids[0]))), array(), array('Content-Type: application/x-www-form-urlencoded'));
    $this->assertStatus(403);

    $this->login($this->member());
    $this->get('node');
    $this->request('POST', $this->orderPath(), http_build_query(array('fids' => array($fids[1], $fids[0]))), array(), array('Content-Type: application/x-www-form-urlencoded'));

    $this->login($owner);
    $this->get('node');
    $this->assertSame(array('A', 'B'), array_keys($this->favorites()));
  }

}
