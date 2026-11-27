<?php

namespace Vaxia\Tests\Vaxia\Vaxia;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "My Characters" block (D7 view vaxia_character_sheets, my_chars_block): a
 * jump menu of the user's own published, approved character sheets, plus
 * published, approved NPCs for those who may use NPCs, grouped as
 * Characters / NPCs / Generic NPCs and sorted by name. Its footer links to
 * the character form, or for visitors to sign up.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group vaxia
 * @group views_blocks
 */
class MyCharactersBlockTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'vaxia_character_sheets-my_chars_block', 'sidebar_first');
  }

  /**
   * The jump menu on the front page: group => [title, ...]; NULL when the
   * block isn't shown, a string (the block's text) when it has no menu.
   */
  private function menu() {
    $this->get('node');
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-my_chars_block ")]');
    if (!$view->length) {
      return NULL;
    }
    $select = $xpath->query('.//select[@name="jump"]', $view->item(0));
    if (!$select->length) {
      // The "empty" text (the footer's link to make a character aside).
      $empty = $xpath->query('.//div[contains(concat(" ", @class, " "), " view-empty ")]', $view->item(0));
      return trim(preg_replace('/\s+/', ' ', ($empty->length ? $empty->item(0) : $view->item(0))->textContent));
    }
    $menu = array();
    foreach ($xpath->query('.//optgroup', $select->item(0)) as $group) {
      foreach ($xpath->query('.//option', $group) as $option) {
        $menu[$group->getAttribute('label')][] = trim($option->textContent);
      }
    }
    return $menu;
  }

  private function character($owner, $state, array $values = array()) {
    $node = $this->site->createCharacter($owner, $values);
    $this->site->forceWorkflowState($node, $state);
    return $node;
  }

  public function testOwnApprovedCharactersByName() {
    $player = $this->site->createUser(array('player'));
    $zed = $this->character($player, 'approved', array('title' => 'Zed ' . $this->site->uniqueName()));
    $abe = $this->character($player, 'approved', array('title' => 'Abe ' . $this->site->uniqueName()));
    $draft = $this->character($player, 'draft');
    $unpublished = $this->character($player, 'approved', array('status' => 0));
    $others = $this->character($this->site->createUser(array('player')), 'approved');
    $this->login($player);
    $menu = $this->menu();
    $this->assertSame(array('Characters' => array($abe->title, $zed->title)), $menu);
    $this->assertText('Select a character to view.');
    $this->assertNoText($draft->title);
    $this->assertNoText($unpublished->title);
    $this->assertNoText($others->title);
  }

  public function testNpcsForThoseWhoMayUseThem() {
    $owner = $this->site->createUser(array('storyteller'));
    $npc = $this->character($owner, 'approved', array('field_is_npc' => 1));
    $generic = $this->character($owner, 'approved', array('field_is_npc' => 1, 'field_is_generic_npc' => 1));
    $unapproved = $this->character($owner, 'draft', array('field_is_npc' => 1));

    $user = $this->site->createUser(array('player'), array('use NPCs'));
    $own = $this->character($user, 'approved');
    $this->login($user);
    $menu = $this->menu();
    $this->assertSame(array($own->title), $menu['Characters']);
    $this->assertContains($npc->title, $menu['NPCs']);
    $this->assertContains($generic->title, $menu['Generic NPCs']);
    $this->assertNotContains($generic->title, $menu['NPCs']);
    $this->assertNoText($unapproved->title);

    // Without the permission, only one's own NPCs.
    $this->login($this->site->createUser(array('player')));
    $this->assertSame('No characters available.', $this->menu());
    $this->site->removeRole($owner, 'new account');
    $this->login($owner);
    $this->assertNull($this->menu(), 'Storytellers without the player role get no block');
    $this->site->addRole($owner, 'player');
    $this->withoutPermission('use NPCs', array('storyteller', 'player', 'authenticated user'), function () use ($owner, $npc, $generic) {
      $this->login($owner);
      $menu = $this->menu();
      $this->assertContains($npc->title, $menu['NPCs']);
      $this->assertContains($generic->title, $menu['Generic NPCs']);
    });
  }

  public function testEmptyText() {
    $this->login($this->site->createUser(array('player')));
    $this->assertSame('No characters available.', $this->menu());
    $this->assertNoText('Select a character to view.');
  }

  public function testWhoSeesIt() {
    $this->login($this->site->createUser(array('new account')));
    $this->assertSame('No characters available.', $this->menu());
    // Not for visitors (user, 2026-11-26).
    $this->anonymous();
    $this->assertNull($this->menu());
    // New accounts start with "new account"; without it a lead has no block.
    $lead = $this->site->createUser(array('lead'));
    $this->site->removeRole($lead, 'new account');
    $this->login($lead);
    $this->assertNull($this->menu());
  }

  public function testJumpsToTheCharacter() {
    $player = $this->site->createUser(array('player'));
    $character = $this->character($player, 'approved');
    $this->login($player);
    $this->get('node');
    $this->submitForm(NULL, array('jump' => $character->nid), 'Go');
    $this->assertStringEndsWith('/node/' . $character->nid, $this->url);
  }

  public function testListsStayWithTheirOwner() {
    $a = $this->site->createUser(array('player'));
    $b = $this->site->createUser(array('player'));
    $aChar = $this->character($a, 'approved');
    $bChar = $this->character($b, 'approved');
    $this->login($a);
    $this->assertSame(array('Characters' => array($aChar->title)), $this->menu());
    $this->login($b);
    $this->assertSame(array('Characters' => array($bChar->title)), $this->menu());
  }


  /**
   * The footer link of the block on the front page: [text, href], or NULL.
   */
  private function footerLink() {
    $this->get('node');
    $link = $this->xpath()->query('//div[contains(concat(" ", @class, " "), " view-display-id-my_chars_block ")]//div[contains(concat(" ", @class, " "), " view-footer ")]//a')->item(0);
    return $link ? array(trim($link->textContent), $link->getAttribute('href')) : NULL;
  }

  public function testPlayersGetALinkToMakeACharacter() {
    $this->login($this->site->createUser(array('player'), array('create character_sheet content')));
    $this->assertSame(array('Create a new character', '/node/add/character_sheet'), $this->footerLink());
  }

  public function testTheLinkStaysOnceTheyHaveCharacters() {
    $player = $this->site->createUser(array('player'), array('create character_sheet content'));
    $this->character($player, 'approved');
    $this->login($player);
    $this->assertSame('Create a new character', $this->footerLink()[0] ?? NULL);
  }

  public function testNoLinkWithoutPermissionToCreate() {
    $this->withoutPermission('create character_sheet content', array('player', 'authenticated user'), function () {
      $this->login($this->site->createUser(array('player')));
      $this->assertNull($this->footerLink());
    });
  }

  public function testVisitorsDoNotSeeTheBlock() {
    $this->anonymous();
    $this->assertNull($this->footerLink());
    $this->assertNoText('My Characters');
  }


  /**
   * The item shop links in the block's footer: [text => href].
   */
  private function shopLinks() {
    $this->get('node');
    $links = array();
    foreach ($this->xpath()->query('//div[contains(concat(" ", @class, " "), " view-display-id-my_chars_block ")]//*[contains(concat(" ", @class, " "), " vaxia-item-shop-links ")]//a') as $a) {
      $links[trim($a->textContent)] = $a->getAttribute('href');
    }
    return $links;
  }

  public function testPlayersGetTheirCharactersItemShop() {
    $player = $this->site->createUser(array('player'), array('use item shop'));
    $character = $this->character($player, 'approved');
    $this->login($player);
    $this->assertSame(array('Item shop for ' . $character->title => '/node/' . $character->nid . '/item_shop'), $this->shopLinks());
  }

  public function testNoItemShopLinkWithoutPermission() {
    $this->withoutPermission('use item shop', array('player', 'authenticated user'), function () {
      $player = $this->site->createUser(array('player'));
      $this->character($player, 'approved');
      $this->login($player);
      $this->assertSame(array(), $this->shopLinks());
    });
  }

}
