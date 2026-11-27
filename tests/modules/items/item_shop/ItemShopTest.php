<?php

namespace Vaxia\Tests\Items\ItemShop;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Item shop: approved characters buy shop items (copies, charged in silver) from
 * a Shop tab on the character; owners sell items back for a share of their
 * value; approving a player's item marked "in shop" puts a copy in the shop.
 *
 * @group item_shop
 */
class ItemShopTest extends VaxiaTestCase {

  private function buyer() {
    return $this->site->createUser(array('player'), array('use item shop', 'sellback item shop'));
  }

  private function approvedCharacter($owner, array $values = array()) {
    $character = $this->site->createCharacter($owner, $values + array('field_economy' => 1));
    $this->approveCharacter($character);
    return $character;
  }

  private function giveSilver($character, $amount) {
    $this->inSession($this->site->createUser(array('storyteller'), array('add xp to character sheet')), function () use ($character, $amount) {
      $this->submitForm('node/' . $character->nid . '/character_sheet', array('add_silver' => (string) $amount), 'Add silver');
    });
  }

  private function silverOf($owner, $character) {
    return $this->inSession($owner, function () use ($character) {
      $this->get('node/' . $character->nid . '/character_sheet');
      preg_match('/You have ([\d,.]+) silver\./', $this->pageText(), $m);
      return $m[1] ?? NULL;
    });
  }

  private function shopItem($value, array $values = array()) {
    // Shop stock is written by staff and approved by someone else.
    $item = $this->site->createNode('items', $values + array(
      'uid' => $this->site->createUser(array('storyteller'))->uid,
      'title' => $this->site->uniqueName('Lantern'),
      'field_in_shop' => 1,
      'field_value' => (string) $value,
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ));
    foreach (array('draft', 'submitted', 'approved') as $state) {
      $this->transition($item, $state);
    }
    return $item;
  }

  private function shopTitles($owner, $character) {
    $this->login($owner);
    $this->get('node/' . $character->nid . '/item_shop');
    $titles = array();
    foreach ($this->xpath()->query('//table//tbody//td[contains(@class, "views-field-title")]') as $td) {
      $titles[] = trim($td->textContent);
    }
    return $titles;
  }

  private function buy($owner, $character, $item) {
    $this->login($owner);
    $this->get('node/' . $character->nid . '/item_shop');
    $box = $this->xpath()->query('//tr[.//td[contains(@class, "views-field-title") and contains(., "' . $item->title . '")]]//input[@type="checkbox"]')->item(0);
    $this->assertNotNull($box, "{$item->title} offered");
    $this->submitForm(NULL, array($box->getAttribute('name') => TRUE), 'Purchase items');
    return $this->messages();
  }

  // ---------------------------------------------------------------------------
  // Shopping.
  // ---------------------------------------------------------------------------

  public function testShopListsAffordableItemsInTheCharactersRealm() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $this->giveSilver($character, 20);
    $cheap = $this->shopItem(10);
    $pricey = $this->shopItem(500);
    $foreign = $this->shopItem(5, array('field_tag_realm' => $this->site->term('realm', 'Sirian')));

    $titles = $this->shopTitles($owner, $character);

    $this->assertContains($cheap->title, $titles);
    $this->assertNotContains($pricey->title, $titles, 'Too expensive');
    $this->assertNotContains($foreign->title, $titles, 'Other realm');
    $this->assertText('You have 20.00 available to spend.');
  }

  public function testUnapprovedCharactersSeeNoItems() {
    $owner = $this->buyer();
    $character = $this->site->createCharacter($owner);
    $this->transition($character, 'draft');
    $this->shopItem(1);
    $this->assertSame(array(), $this->shopTitles($owner, $character));
  }

  public function testShopTabOnlyOnCharacters() {
    $owner = $this->buyer();
    $page = $this->site->createNode('page', array('uid' => $owner->uid));
    $this->login($owner);
    $this->get('node/' . $page->nid . '/item_shop');
    $this->assertStatus(403);
  }

  public function testShopNeedsPermission() {
    $this->withoutPermission('use item shop', array('anonymous user', 'authenticated user', 'player'), function () {
      $owner = $this->site->createUser(array('player'));
      $character = $this->approvedCharacter($owner);
      $this->login($owner);
      $this->get('node/' . $character->nid . '/item_shop');
      $this->assertStatus(403);
    });
  }

  public function testBuyingAddsAnApprovedCopyAndChargesSilver() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $this->giveSilver($character, 30);
    $item = $this->shopItem(12);

    $messages = $this->buy($owner, $character, $item);

    $this->assertContains($item->title . ' has been added to this character. 12 silver charged.', $messages);
    $this->assertContains('Purchase complete.', $messages);
    $this->assertSame('18.00', $this->silverOf($owner, $character));
    $this->assertStringContainsString($item->title, $this->nodeText($character), 'On the sheet');
    $this->assertContains($item->title, $this->shopTitles($owner, $character), 'The shop keeps its own copy');
  }

  public function testOthersCannotBuyForYourCharacter() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $this->giveSilver($character, 20);
    $this->shopItem(1);
    $this->login($this->buyer());
    $this->get('node/' . $character->nid . '/item_shop');
    $this->assertSame(1, $this->xpath()->query('//input[@value="This is not your character sheet" and @disabled]')->length);
  }

  /**
   * @group knownbug
   * Prices have "." stripped before comparing ("12.50" becomes 1250), so items
   * with a decimal value are hidden as unaffordable or charged 100x.
   */
  public function testDecimalPricesAreCharged() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $this->giveSilver($character, 20);
    $item = $this->shopItem('12.50');
    $this->assertContains($item->title, $this->shopTitles($owner, $character));
  }

  /**
   * The shop counts a character's whole economy worth, but selling every
   * level back raises about half a silver less (worth(0)). A price in that
   * last half silver sells the final level and then asks what economy 0 is
   * worth over -1, a division by zero: a warning on D7's PHP 7, an error that
   * stopped the purchase on PHP 8. Economy 1 with no silver is worth about
   * 1.28: a 1 silver item reaches it.
   */
  public function testBuyingWithTheLastOfTheEconomyCompletes() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $item = $this->shopItem(1);

    $messages = $this->buy($owner, $character, $item);

    $this->assertSame(200, $this->status);
    $this->assertContains($item->title . ' has been added to this character. 1 silver charged.', $messages);
    $this->assertContains('Converted economy from level 1 to 0, for 0 silver.', $messages);
    $this->assertStringContainsString($item->title, $this->nodeText($character), 'On the sheet');
  }

  // ---------------------------------------------------------------------------
  // Selling back.
  // ---------------------------------------------------------------------------

  private function sellableItem($owner, $character, $value = 40) {
    $item = $this->site->createNode('items', array(
      'uid' => $owner->uid,
      'title' => $this->site->uniqueName('Shield'),
      'field_value' => (string) $value,
      'field_sellback' => 1,
      'field_character_owner' => $character->nid,
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ));
    foreach (array('draft', 'submitted', 'approved') as $state) {
      $this->transition($item, $state);
    }
    return $item;
  }

  public function testOwnerSeesSellbackOffer() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $item = $this->sellableItem($owner, $character, 40);
    $this->login($owner);
    $this->get('node/' . $item->nid);
    $this->assertText($item->title . ' may be automatically sold for 30 silver at 75% of it\'s value.');
  }

  public function testSellingBackPaysAndRemovesTheItem() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $item = $this->sellableItem($owner, $character, 40);
    $this->login($owner);
    $this->get('node/' . $item->nid);
    $button = $this->xpath()->query('//div[@class="sellback_shop"]//input[@type="submit"]')->item(0);
    $this->submitForm(NULL, array(), $button->getAttribute('value'));

    $this->assertContains('30 silver added to ' . $character->title . '.', $this->messages('status'));
    $this->assertSame('30.00', $this->silverOf($owner, $character));
    $this->assertSame('removed from game', $this->workflowState($item));
    $this->assertStringNotContainsString($item->title, $this->nodeText($character));
  }

  public function testNoSellbackForOthersOrUnmarkedItems() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $item = $this->sellableItem($owner, $character);
    $this->login($this->buyer());
    $this->get('node/' . $item->nid);
    $this->assertNoText('may be automatically sold');
  }

  public function testSellbackCanBeTurnedOff() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $item = $this->sellableItem($owner, $character);
    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/vaxia/item_shop', array('item_shop_sellback' => FALSE), 'Save configuration');
    });
    try {
      $this->login($owner);
      $this->get('node/' . $item->nid);
      $this->assertNoText('may be automatically sold');
    }
    finally {
      $this->inSession($this->superuser(), function () {
        $this->submitForm('admin/vaxia/item_shop', array('item_shop_sellback' => TRUE), 'Save configuration');
      });
    }
  }

  // ---------------------------------------------------------------------------
  // Player items into the shop.
  // ---------------------------------------------------------------------------

  public function testApprovingAnInShopItemPutsACopyInTheShop() {
    $owner = $this->buyer();
    $character = $this->approvedCharacter($owner);
    $item = $this->site->createNode('items', array(
      'uid' => $owner->uid, 'title' => $this->site->uniqueName('Charm'), 'field_in_shop' => 1, 'field_value' => '3',
      'field_character_owner' => $character->nid, 'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ));
    $this->transition($item, 'draft');
    $this->transition($item, 'submitted');

    $messages = $this->transition($item, 'approved', $this->site->createUser(array('storyteller')));

    $this->assertNotEmpty(preg_grep('/^A new item ' . preg_quote($item->title, '/') . ' with item ID #\d+ has been created from your item ID #' . $item->nid . ' and added to the item shop\.$/', $messages));
    $buyer = $this->buyer();
    $other = $this->approvedCharacter($buyer);
    $this->giveSilver($other, 10);
    $this->assertContains($item->title, $this->shopTitles($buyer, $other));
  }

}
