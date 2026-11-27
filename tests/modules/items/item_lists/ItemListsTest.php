<?php

namespace Vaxia\Tests\Items\ItemLists;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Staff item lists (D7 views character_items, item_shop display
 * item_shop_admin and oversight_items): character-items(/all),
 * character-items/evaluate, item_shop, oversight/items and
 * oversight/expensive-items.
 *
 * The D7 test site doesn't carry these views, so this runs on D11 only.
 *
 * @group item_lists
 */
class ItemListsTest extends VaxiaTestCase {

  protected function setUp(): void {
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site lacks the item list views.');
    }
    parent::setUp();
  }

  private function staff($role) {
    $account = $this->site->createUser(array($role));
    $this->site->removeRole($account, 'new account');
    return $account;
  }

  private function item($creator, $state, array $values = array()) {
    $item = $this->site->createNode('items', $values + array(
      'uid' => $creator->uid,
      'title' => $this->site->uniqueName('Item'),
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ));
    if ($state) {
      $this->site->forceWorkflowState($item, $state);
    }
    return $item;
  }

  /**
   * Text of column $column (0-based) of each table body row.
   */
  private function column($column = 0) {
    $cells = array();
    foreach ($this->xpath()->query('//table//tbody/tr[td]') as $row) {
      $cells[] = trim($row->getElementsByTagName('td')->item($column)->textContent);
    }
    return $cells;
  }

  private function headers() {
    $headers = array();
    foreach ($this->xpath()->query('//table//thead//th') as $th) {
      // The text before a click-sort marker ("Sort ascending").
      $headers[] = trim(preg_split('/\s*\n/', trim($th->textContent))[0]);
    }
    return $headers;
  }

  // ---------------------------------------------------------------------------
  // character-items, character-items/all.
  // ---------------------------------------------------------------------------

  public function testCharacterItemsAreForStaff() {
    foreach (array('administrator', 'lead', 'storyteller') as $role) {
      $this->login($this->staff($role));
      foreach (array('character-items', 'character-items/all', 'character-items/evaluate') as $path) {
        $this->get($path);
        $this->assertStatus(200);
      }
    }
    $this->login($this->site->createUser(array('player')));
    foreach (array('character-items', 'character-items/all', 'character-items/evaluate') as $path) {
      $this->get($path);
      $this->assertStatus(403);
    }
  }

  public function testCharacterItemsListApprovedItemsNewestFirst() {
    $creator = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($creator);
    $older = $this->item($creator, 'approved', array('field_character_owner' => $character->nid));
    $newer = $this->item($creator, 'approved');
    $submitted = $this->item($creator, 'submitted');
    $unpublished = $this->item($creator, 'approved', array('status' => 0));
    $this->site->backdateNodeCreated($older, 1500000000);
    $this->site->backdateNodeCreated($newer, 1500000100);
    $this->site->expireViewsCache();

    $this->login($this->staff('lead'));
    foreach (array('character-items', 'character-items/all') as $path) {
      $this->get($path, array('uid' => $creator->name));
      $this->assertText('Character items');
      $this->assertSame(array($newer->title, $older->title), $this->column(0));
      $this->assertSame(array('Item', 'Character', 'Creator', 'Created', 'Updated', 'Current state'), $this->headers());
      $this->assertSame(array('', $character->title), $this->column(1));
      $this->assertSame(array('approved', 'approved'), $this->column(5));
      $this->assertNoText($submitted->title);
      $this->assertNoText($unpublished->title);
    }
    $this->get('character-items/all', array('title' => substr($older->title, -8)));
    $this->assertSame(array($older->title), $this->column(0));
    $this->assertRaw('href="/character-items/evaluate"');
    $this->assertText('Evaluate items');
  }

  public function testEvaluateItemsListsSubmittedItemsWithLinks() {
    $creator = $this->site->createUser(array('player'));
    $submitted = $this->item($creator, 'submitted');
    $approved = $this->item($creator, 'approved');

    $this->login($this->staff('storyteller'));
    $this->get('character-items/evaluate', array('uid' => $creator->name));
    $this->assertText('Evaluate Items');
    $this->assertSame(array($submitted->title), $this->column(0));
    $this->assertSame(array('Item', 'Character', 'Creator', 'Created', 'Updated', 'Workflow', 'Operations', 'Delete'), $this->headers());
    $this->assertRaw('href="/node/' . $submitted->nid . '/edit"');
    $this->assertRaw('href="/node/' . $submitted->nid . '/delete"');
    $this->assertNoText($approved->title);
  }

  public function testCharacterItemsPageAtThirtyFive() {
    $creator = $this->site->createUser(array('player'));
    for ($i = 0; $i < 36; $i++) {
      $this->item($creator, 'approved');
    }
    $this->login($this->staff('lead'));
    $this->get('character-items/all', array('uid' => $creator->name));
    $this->assertCount(35, $this->column(0));
    $this->get('character-items/all', array('uid' => $creator->name, 'page' => 1));
    $this->assertCount(1, $this->column(0));
  }

  // ---------------------------------------------------------------------------
  // item_shop.
  // ---------------------------------------------------------------------------

  public function testShopListIsForStorytellers() {
    $this->login($this->staff('storyteller'));
    $this->get('item_shop');
    $this->assertStatus(200);
    $this->login($this->staff('administrator'));
    $this->get('item_shop');
    $this->assertStatus(200);
    foreach (array('lead', 'player') as $role) {
      $this->login($this->staff($role));
      $this->get('item_shop');
      $this->assertStatus(403);
    }
  }

  public function testShopListShowsApprovedShopItemsCheapestFirst() {
    $creator = $this->staff('storyteller');
    $type = $this->site->term('item_type', $this->site->uniqueName('Type'));
    $prefix = $this->site->uniqueName('Shop');
    $dear = $this->item($creator, 'approved', array('title' => $prefix . ' A', 'field_in_shop' => 1, 'field_value' => '9', 'field_item_type' => $type));
    $cheap = $this->item($creator, 'approved', array('title' => $prefix . ' C', 'field_in_shop' => 1, 'field_value' => '1', 'field_item_type' => $type));
    $tie = $this->item($creator, 'approved', array('title' => $prefix . ' B', 'field_in_shop' => 1, 'field_value' => '1', 'field_item_type' => $type));
    $free = $this->item($creator, 'approved', array('title' => $prefix . ' D', 'field_in_shop' => 1, 'field_item_type' => $type));
    $notInShop = $this->item($creator, 'approved', array('title' => $prefix . ' E', 'field_in_shop' => 0, 'field_item_type' => $type));
    $submitted = $this->item($creator, 'submitted', array('title' => $prefix . ' F', 'field_in_shop' => 1, 'field_item_type' => $type));

    $this->login($creator);
    $this->get('item_shop', array('title' => $prefix));
    $this->assertText('Item shop');
    $this->assertSame(array('Items', 'Type', 'Cost', 'Realm'), $this->headers());
    $this->assertSame(array($free->title, $tie->title, $cheap->title, $dear->title), $this->column(0));
    $this->assertSame(array('Free (Lifestyle)', '1', '1', '9'), $this->column(2));
    $this->assertSame('Vaxia', $this->column(3)[0]);

    $this->get('item_shop', array('title' => $prefix, 'field_item_type_tid' => (string) $type));
    $this->assertCount(4, $this->column(0));
    $other = $this->site->term('item_type', $this->site->uniqueName('Other'));
    $this->get('item_shop', array('title' => $prefix, 'field_item_type_tid' => (string) $other));
    $this->assertNoText($free->title);
    $this->assertText('No items found or this character may not be allowed access to the shop at this time.');
  }

  public function testShopListRealmFilter() {
    $creator = $this->staff('storyteller');
    $prefix = $this->site->uniqueName('Realm');
    $vaxia = $this->item($creator, 'approved', array('title' => $prefix . ' V', 'field_in_shop' => 1, 'field_value' => '2'));
    $sirian = $this->item($creator, 'approved', array('title' => $prefix . ' S', 'field_in_shop' => 1, 'field_value' => '2', 'field_tag_realm' => $this->site->term('realm', 'Sirian')));

    $this->login($creator);
    $this->get('item_shop', array('title' => $prefix, 'field_tag_realm_tid' => (string) $this->site->term('realm', 'Sirian')));
    $this->assertSame(array($sirian->title), $this->column(0));
  }

  // ---------------------------------------------------------------------------
  // oversight/items, oversight/expensive-items.
  // ---------------------------------------------------------------------------

  public function testOversightIsForStorytellers() {
    foreach (array('oversight/items', 'oversight/expensive-items') as $path) {
      $this->login($this->staff('storyteller'));
      $this->get($path);
      $this->assertStatus(200);
      foreach (array('lead', 'player') as $role) {
        $this->login($this->staff($role));
        $this->get($path);
        $this->assertStatus(403);
      }
    }
  }

  public function testOversightItemsListsItemsWithAStatAtTheLimit() {
    $player = $this->site->createUser(array('player'));
    $b = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Bowner')));
    $a = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Aowner')));
    $gone = $this->site->createCharacter($player, array('status' => 0));
    $strong = $this->item($player, 'approved', array('field_character_owner' => $b->nid, 'field_item_strength' => 7, 'field_value' => '10', 'field_declared_value' => '12'));
    $charming = $this->item($player, 'needs recharge', array('field_character_owner' => $a->nid, 'field_item_charisma' => 8));
    $weak = $this->item($player, 'approved', array('field_character_owner' => $a->nid, 'field_item_strength' => 6));
    $submitted = $this->item($player, 'submitted', array('field_character_owner' => $a->nid, 'field_item_strength' => 9));
    $unowned = $this->item($player, 'approved', array('field_item_strength' => 9));
    $ownerGone = $this->item($player, 'approved', array('field_character_owner' => $gone->nid, 'field_item_strength' => 9));

    $this->login($this->staff('storyteller'));
    $this->get('oversight/items', array('uid' => $player->name));
    $this->assertText('Oversight Items');
    $this->assertText('These are Items that require additional OVERSIGHT. These Items either have a stat over 7');
    $this->assertSame(array('Title', 'Declared value', 'Value', 'Player', 'Character', 'Updated date', 'Workflow', 'Edit link'), $this->headers());
    // By owning character's name.
    $this->assertSame(array($charming->title, $strong->title), $this->column(0));
    $this->assertSame(array($a->title, $b->title), $this->column(4));
    $this->assertSame(array('needs recharge', 'approved'), $this->column(6));
    // Item saves fill an empty declared value with 0.
    $this->assertSame(array('0', '12'), $this->column(1));
    $this->assertRaw('href="/node/' . $strong->nid . '/edit"');
    foreach (array($weak, $submitted, $unowned, $ownerGone) as $hidden) {
      $this->assertNoText($hidden->title);
    }

    $this->get('oversight/items', array('title' => $b->title));
    $this->assertSame(array($strong->title), $this->column(0));
    $this->assertRaw('href="/oversight/expensive-items"');
  }

  public function testExpensiveItemsListsItemsWorthOver5000() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player);
    $valued = $this->item($player, 'approved', array('field_character_owner' => $character->nid, 'field_value' => '6000'));
    $declared = $this->item($player, 'approved', array('field_character_owner' => $character->nid, 'field_declared_value' => '5001'));
    $cheap = $this->item($player, 'approved', array('field_character_owner' => $character->nid, 'field_value' => '5000'));
    // D7 read "12,000" as 12.
    $comma = $this->item($player, 'approved', array('field_character_owner' => $character->nid, 'field_value' => '12,000'));

    $this->login($this->staff('storyteller'));
    $this->get('oversight/expensive-items', array('uid' => $player->name));
    $this->assertText('Expensive Items');
    $this->assertText('These Items are extremely valuable and risk economic damage in the game world.');
    $titles = $this->column(0);
    sort($titles);
    $expected = array($valued->title, $declared->title);
    sort($expected);
    $this->assertSame($expected, $titles);
    $this->assertNoText($cheap->title);
    $this->assertNoText($comma->title);
  }

}
