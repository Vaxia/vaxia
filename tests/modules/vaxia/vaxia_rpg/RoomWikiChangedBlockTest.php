<?php

namespace Vaxia\Tests\Vaxia\VaxiaRpg;

use Vaxia\Tests\VaxiaTestCase;

/**
 * "Vaxia RPG Wiki View" block (D7 view vaxia_rpg_wiki_view): in a chat room
 * whose description lives on wiki pages, a warning when a page changed since
 * the user last read it ("has new content": never read and changed in the
 * last 30 days, or changed after the last read).
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group vaxia_rpg
 * @group views_blocks
 */
class RoomWikiChangedBlockTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->placeBlock('views', 'vaxia_rpg_wiki_view-block', 'content');
  }

  /**
   * The warning on a page, or NULL when the block isn't shown.
   */
  private function warning($path) {
    $this->get($path);
    $xpath = $this->xpath();
    $view = $xpath->query('//div[contains(concat(" ", @class, " "), " view-display-id-block ") and contains(@class, "view-id-vaxia_rpg_wiki_view")]');
    return $view->length ? trim(preg_replace('/\s+/', ' ', $view->item(0)->textContent)) : NULL;
  }

  private function roomWithWiki() {
    $wiki = $this->site->createNode('wiki_page', array('title' => $this->site->uniqueName('Room lore')));
    $room = $this->site->createRoom(array('field_wiki_source' => array(array('target_id' => $wiki->nid))));
    return array($room, $wiki);
  }

  public function testWarnsUntilRead() {
    list($room, $wiki) = $this->roomWithWiki();
    $this->login($this->site->createUser(array('player')));
    $warning = $this->warning('node/' . $room->nid);
    $this->assertMatchesRegularExpression('/^This room description was updated on .+, please review description changes\.$/', $warning);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-id-vaxia_rpg_wiki_view")]//a[@href="/node/' . $wiki->nid . '" and text()="review description changes"]')->length);
    $this->assertSame(1, $this->xpath()->query('//div[contains(@class, "view-id-vaxia_rpg_wiki_view") and contains(@class, "messages--warning")]')->length);

    // Reading the page (the browser reports it) clears the warning.
    $this->request('POST', 'history/' . $wiki->nid . '/read');
    $this->assertNull($this->warning('node/' . $room->nid));
  }

  public function testWarnsAgainAfterAChange() {
    list($room, $wiki) = $this->roomWithWiki();
    $player = $this->site->createUser(array('player'));
    $this->site->setNodeRead($player, $wiki, time() - 3600);
    $this->login($player);
    $this->assertNotNull($this->warning('node/' . $room->nid));
    $this->site->setNodeRead($player, $wiki, time() + 60);
    $this->assertNull($this->warning('node/' . $room->nid));
  }

  public function testOldUnreadPagesDoNotWarn() {
    list($room, $wiki) = $this->roomWithWiki();
    $this->site->backdateNode($wiki, time() - 40 * 86400);
    // Comment statistics start at creation; a recent one counts as new too.
    $this->site->setLastCommentTime($wiki, time() - 40 * 86400);
    $this->login($this->site->createUser(array('player')));
    $this->assertNull($this->warning('node/' . $room->nid));
  }

  public function testReadsArePerUser() {
    list($room, $wiki) = $this->roomWithWiki();
    $reader = $this->site->createUser(array('player'));
    $this->site->setNodeRead($reader, $wiki, time() + 60);
    $this->login($reader);
    $this->assertNull($this->warning('node/' . $room->nid));
    $this->login($this->site->createUser(array('player')));
    $this->assertNotNull($this->warning('node/' . $room->nid));
  }

  public function testOnlyInRoomsWithWikiPages() {
    $plain = $this->site->createRoom();
    list(, $wiki) = $this->roomWithWiki();
    $this->login($this->site->createUser(array('player')));
    $this->assertNull($this->warning('node/' . $plain->nid));
    $this->assertNull($this->warning('node/' . $wiki->nid));
    $this->assertNull($this->warning('node'));
  }

}
