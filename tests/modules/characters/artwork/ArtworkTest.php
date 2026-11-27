<?php

namespace Vaxia\Tests\Characters\Artwork;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Artwork: character art goes through "Art evaluation" (approved, or removed
 * from site). Players browse approved art; each user has a page of their own.
 *
 * @group artwork
 */
class ArtworkTest extends VaxiaTestCase {

  private function art($owner, $state = NULL) {
    $art = $this->site->createNode('artwork', array('uid' => $owner->uid, 'title' => $this->site->uniqueName('Portrait')));
    if ($state) {
      $this->transition($art, 'approved');
      if ($state === 'removed from site') {
        $this->transition($art, 'removed from site');
      }
    }
    return $art;
  }

  public function testArtIsEvaluated() {
    $art = $this->art($this->site->createUser(array('player')));
    $this->assertSame('(creation)', $this->workflowState($art));
    $this->transition($art, 'approved');
    $this->assertSame('approved', $this->workflowState($art));
  }

  // ---------------------------------------------------------------------------
  // Gallery of approved art.
  // ---------------------------------------------------------------------------

  public function testGalleryShowsOnlyApprovedArt() {
    $artist = $this->site->createUser(array('player'));
    $approved = $this->art($artist, 'approved');
    $removed = $this->art($artist, 'removed from site');

    $this->login($this->site->createUser(array('player')));
    $this->get('artwork/all');

    $this->assertStatus(200);
    $this->assertText($approved->title);
    $this->assertNoText($removed->title);
  }

  public function testGalleryIsForPlayers() {
    $account = $this->site->createUser(array('lead'));
    $this->site->removeRole($account, 'new account');
    $this->login($account);
    $this->get('artwork/all');
    $this->assertStatus(403);
  }

  // ---------------------------------------------------------------------------
  // Your own art.
  // ---------------------------------------------------------------------------

  public function testOwnArtPageListsYourLiveArt() {
    $artist = $this->site->createUser(array('player'));
    $approved = $this->art($artist, 'approved');
    $removed = $this->art($artist, 'removed from site');
    $someoneElses = $this->art($this->site->createUser(array('player')), 'approved');

    $this->login($artist);
    $this->get('user/' . $artist->uid . '/artwork');

    $this->assertStatus(200);
    $this->assertText('My artwork');
    $this->assertText($approved->title);
    $this->assertNoText($removed->title);
    $this->assertNoText($someoneElses->title);
  }

}
