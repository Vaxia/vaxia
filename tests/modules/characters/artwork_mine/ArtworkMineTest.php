<?php

namespace Vaxia\Tests\Characters\ArtworkMine;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The "My art" tab (D7 view artwork, display artwork_mine): your own
 * published art in creation or approved.
 *
 * The D7 test site doesn't carry this view, so this runs on D11 only.
 *
 * @group artwork_mine
 */
class ArtworkMineTest extends VaxiaTestCase {

  protected function setUp(): void {
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site lacks the artwork view.');
    }
    parent::setUp();
  }

  private function art($owner, $title, $state = NULL, array $values = array()) {
    $art = $this->site->createNode('artwork', $values + array('uid' => $owner->uid, 'title' => $title));
    if ($state) {
      $this->site->forceWorkflowState($art, $state);
    }
    return $art;
  }

  public function testListsYourLiveArtByTitle() {
    $artist = $this->site->createUser(array('player'));
    $prefix = $this->site->uniqueName('Art');
    $b = $this->art($artist, $prefix . ' B', 'approved');
    $a = $this->art($artist, $prefix . ' A');
    $removed = $this->art($artist, $prefix . ' C', 'removed from site');
    $unpublished = $this->art($artist, $prefix . ' D', 'approved', array('status' => 0));
    $theirs = $this->art($this->site->createUser(array('player')), $this->site->uniqueName('Theirs'), 'approved');

    $this->login($artist);
    $this->get('artwork/mine');
    $this->assertStatus(200);
    $this->assertText('My artwork');
    $text = $this->pageText();
    $this->assertNotFalse(strpos($text, $a->title));
    $this->assertNotFalse(strpos($text, $b->title));
    $this->assertLessThan(strpos($text, $b->title), strpos($text, $a->title));
    $this->assertNoText($removed->title);
    $this->assertNoText($unpublished->title);
    $this->assertNoText($theirs->title);
    // Tabs: the gallery and My art.
    $this->assertRaw('href="/artwork/all"');
    $this->assertText('My art');
  }

  public function testPagesAtThirtyFive() {
    $artist = $this->site->createUser(array('player'));
    $prefix = $this->site->uniqueName('Many');
    for ($i = 10; $i < 46; $i++) {
      $this->art($artist, $prefix . ' ' . $i, 'approved');
    }
    $this->login($artist);
    $this->get('artwork/mine');
    $this->assertText($prefix . ' 44');
    $this->assertNoText($prefix . ' 45');
    $this->get('artwork/mine', array('page' => 1));
    $this->assertText($prefix . ' 45');
    $this->assertNoText($prefix . ' 44');
  }

  public function testIsForPlayers() {
    $account = $this->site->createUser(array('lead'));
    $this->site->removeRole($account, 'new account');
    $this->login($account);
    $this->get('artwork/mine');
    $this->assertStatus(403);
    $this->anonymous();
    $this->get('artwork/mine');
    $this->assertStatus(403);
  }

}
