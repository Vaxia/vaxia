<?php

namespace Vaxia\Tests\Items\ItemUses;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Item uses: single-use and per-day items spend a charge each time they're
 * rolled with. When the charges run out the item moves to a "used up" state
 * (single use) or a "used for the day" state that cron recharges daily.
 *
 * Today no charge is ever spent through the site: item_uses only tracks rolls
 * in rooms _vaxia_is_ic_room() accepts, which (despite the name) are the limbo
 * (OOC) rooms, and limbo rooms have no dice roller. So every test that expects
 * a charge to be spent is a knownbug; they post in normal (IC) rooms.
 *
 * @group item_uses
 */
class ItemUsesTest extends VaxiaTestCase {

  use DiceTrait;

  protected function setUp(): void {
    parent::setUp();
    $this->inSession($this->superuser(), function () {
      $this->get('admin/vaxia/item_uses');
      $this->submitForm(NULL, array(
        'item_uses_active' => TRUE,
        'item_uses_once_workflow' => $this->optionValue('item_uses_once_workflow', 'Item evaluation: needs recharge'),
        'item_uses_daily_workflow' => $this->optionValue('item_uses_daily_workflow', 'Item evaluation: needs daily refresh'),
      ), 'Save configuration');
    });
  }

  private function chargedItem($character, $frequency, $charges, array $values = array()) {
    return $this->characterItem($character, $values + array(
      'field_item_strength' => 1,
      'field_item_use_day' => $frequency,
      'field_item_uses' => $charges,
    ));
  }

  private function rollWith($player, $room, $character, $item) {
    $this->postRoll($player, $room, $character, array(array('stat' => 'strength')), array($item));
    return $this->messages('status');
  }

  // ---------------------------------------------------------------------------
  // Settings and labels.
  // ---------------------------------------------------------------------------

  public function testSettingsPageNeedsPermission() {
    $this->login($this->site->createUser(array('lead')));
    $this->get('admin/vaxia/item_uses');
    $this->assertStatus(403);
  }

  public function testSettingsPageSavesStates() {
    $this->login($this->site->createUser(array(), array('administer item uses')));
    $this->get('admin/vaxia/item_uses');
    $this->assertStatus(200);
    $selected = $this->xpath()->query('//select[@name="item_uses_daily_workflow"]/option[@selected]');
    $this->assertSame('Item evaluation: needs daily refresh', trim($selected->item(0)->textContent));
  }

  public function testChoiceLabelShowsUsesAndPp() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $once = $this->chargedItem($character, 'once', 3, array('title' => 'Flare', 'field_pp_created' => 5));
    $daily = $this->chargedItem($character, 'per_day', 2, array('title' => 'Lamp'));
    $atWill = $this->chargedItem($character, 'at_will', 2, array('title' => 'Stick'));

    $choices = $this->itemChoices($player, $this->site->createRoom(), $character);

    $this->assertSame('Flare (str:1, Once, 3 uses, 5 PP)', $choices[$once->nid]);
    $this->assertSame('Lamp (str:1, Per Day, 2 uses)', $choices[$daily->nid]);
    $this->assertSame('Stick (str:1)', $choices[$atWill->nid]);
  }

  // ---------------------------------------------------------------------------
  // Spending charges.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * No charge is ever spent (see the class comment): _vaxia_is_ic_room() is
   * TRUE only for limbo rooms, which have no dice roller.
   */
  public function testRollingSpendsACharge() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'once', 3, array('title' => 'Flare'));

    $this->assertContains('Flare charge used, 2 charges remaining.', $this->rollWith($player, $this->site->createRoom(), $character, $item));
    $this->assertSame('approved', $this->workflowState($item));
    $this->assertSame('Flare (str:1, Once, 2 uses)', $this->itemChoices($player, $this->site->createRoom(), $character)[$item->nid]);
  }

  /**
   * @group knownbug
   * No charge is ever spent (see the class comment): _vaxia_is_ic_room() is
   * TRUE only for limbo rooms, which have no dice roller.
   */
  public function testSingleUseItemRunsOutAndNeedsRecharge() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'once', 1, array('title' => 'Flare'));

    $messages = $this->rollWith($player, $this->site->createRoom(), $character, $item);

    $this->assertContains('Flare is out of uses cannot be used again until recharged.', $messages);
    $this->assertSame('needs recharge', $this->workflowState($item));
    $this->assertArrayNotHasKey($item->nid, $this->itemChoices($player, $this->site->createRoom(), $character), 'No longer offered');
  }

  /**
   * @group knownbug
   * No charge is ever spent (see the class comment): _vaxia_is_ic_room() is
   * TRUE only for limbo rooms, which have no dice roller.
   */
  public function testPerDayItemRunsOutForTheDay() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'per_day', 1, array('title' => 'Lamp'));

    $messages = $this->rollWith($player, $this->site->createRoom(), $character, $item);

    $this->assertContains('Lamp is out of uses cannot be used again until tomorrow.', $messages);
    $this->assertSame('needs daily refresh', $this->workflowState($item));
  }

  public function testAtWillItemsSpendNothing() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'at_will', 1, array('title' => 'Stick'));
    $this->assertNotContains('Stick charge used, 0 charges remaining.', $this->rollWith($player, $this->site->createRoom(), $character, $item));
    $this->assertSame('approved', $this->workflowState($item));
  }

  public function testUnlimitedPermissionSpendsNothing() {
    $player = $this->roller(array('use items indefinately'));
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'once', 1, array('title' => 'Flare'));
    $this->rollWith($player, $this->site->createRoom(), $character, $item);
    $this->assertSame('approved', $this->workflowState($item));
  }

  public function testTrackingCanBeTurnedOff() {
    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/vaxia/item_uses', array('item_uses_active' => FALSE), 'Save configuration');
    });
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'once', 1, array('title' => 'Flare'));
    $this->rollWith($player, $this->site->createRoom(), $character, $item);
    $this->assertSame('approved', $this->workflowState($item));
  }

  public function testPostingWithoutTheItemSpendsNothing() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'once', 1);
    $this->postRoll($player, $this->site->createRoom(), $character, array(array('stat' => 'strength')));
    $this->assertSame('approved', $this->workflowState($item));
  }

  // ---------------------------------------------------------------------------
  // Known bugs.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * The daily recharge calls _vaxia_get_in_workflows($dailyState) with the state
   * in the node-types argument, so it finds no items: per-day items stay used
   * up forever. (The item is moved to the used-up state directly, since rolling
   * spends no charges today.)
   */
  public function testCronRechargesPerDayItems() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'per_day', 1, array('title' => 'Lamp', 'field_item_spent_uses' => 1));
    $this->site->forceWorkflowState($item, 'needs daily refresh');
    $this->site->resetDailyCron('item_uses_daily_recharge');

    $this->runCron();

    $this->assertSame('approved', $this->workflowState($item));
    $this->assertSame('Lamp (str:1, Per Day, 1 uses)', $this->itemChoices($player, $this->site->createRoom(), $character)[$item->nid]);
  }


  public function testEditingAPostSpendsNoCharge() {
    // Charges are spent once, by the new post (review, 2026-11-26).
    $player = $this->roller(array('edit own comments'));
    $character = $this->rollingCharacter($player);
    $item = $this->chargedItem($character, 'once', 3, array('title' => 'Flare'));
    $room = $this->site->createRoom();
    $this->rollWith($player, $room, $character, $item);
    $this->get('node/' . $room->nid);
    $cid = $this->firstCommentId();
    $this->submitForm('comment/' . $cid . '/edit', array('vaxia_rolls[items][]' => array((string) $item->nid)), 'Post');
    $this->assertSame('Flare (str:1, Once, 2 uses)', $this->itemChoices($player, $this->site->createRoom(), $character)[$item->nid]);
  }

}
