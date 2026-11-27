<?php

namespace Vaxia\Tests\Chatrooms\Archives;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Archives: old chat-room and forum posts are written out to flat HTML files
 * (one page per room per day, plus character/location/player indexes) and
 * removed from the site; players search the flat files.
 *
 * @group archives
 */
class ArchivesTest extends VaxiaTestCase {

  const DIR = 'archive_test/';
  const PROBE = 'vaxia_injection_probe';

  protected function tearDown(): void {
    $this->site->deletePublicPath('archive_test');
    $this->site->deletePublicPath(self::PROBE);
    parent::tearDown();
  }

  private function archiveRoot() {
    return $this->site->publicFilesPath() . '/' . self::DIR;
  }

  private function configure($keep = '1 day') {
    $this->inSession($this->superuser(), function () use ($keep) {
      $this->submitForm('admin/vaxia/archives', array(
        'archives_flat_file_path' => $this->archiveRoot(),
        'archives_date' => $keep,
      ), 'Save configuration');
    });
  }

  private function archiveAll() {
    $this->login($this->site->createUser(array(), array('administer old archives')));
    $this->submitForm('admin/vaxia/archives', array(), 'Archive All Posts');
    return $this->messages('status');
  }

  private function search($text) {
    $this->login($this->site->createUser(array('player'), array('search old archives')));
    $this->submitForm('archives', array('archives_text' => $text), 'Search archives');
    $links = array();
    foreach ($this->xpath()->query('//div[@id="archives_results"]//a') as $a) {
      $links[] = $a->getAttribute('href');
    }
    return $links;
  }

  /**
   * A chat room with one post from three days ago.
   */
  private function oldRoomPost($text) {
    $player = $this->site->createUser(array('player'));
    $room = $this->site->createRoom(array('title' => $this->site->uniqueName('Tavern')));
    $comment = $this->site->postComment($room, $player, $text);
    $this->site->backdateComment($comment, strtotime('-3 days'));
    return array($room, $player);
  }

  // ---------------------------------------------------------------------------
  // Access and settings.
  // ---------------------------------------------------------------------------

  public function testSearchPageNeedsPermission() {
    $this->withoutPermission('search old archives', array('anonymous user', 'authenticated user', 'player'), function () {
      $this->login($this->site->createUser(array('player')));
      $this->get('archives');
      $this->assertStatus(403);
      $this->login($this->site->createUser(array('player'), array('search old archives')));
      $this->get('archives');
      $this->assertStatus(200);
      $this->assertText('No search performed.');
    });
  }

  public function testSettingsPage() {
    $this->login($this->site->createUser(array(), array('administer old archives')));
    $this->get('admin/vaxia/archives');
    $this->assertStatus(200);
    $this->assertText('Archives last run: never');
    $this->login($this->site->createUser(array('player'), array('search old archives')));
    $this->get('admin/vaxia/archives');
    $this->assertStatus(403);
  }

  // ---------------------------------------------------------------------------
  // Archiving.
  // ---------------------------------------------------------------------------

  public function testOldRoomPostsMoveToFlatFilesAndOffTheSite() {
    $this->configure();
    list($room, $player) = $this->oldRoomPost('The bard sang of dragons');

    $messages = $this->archiveAll();

    $this->assertContains('Archived 1 comments.', $messages);
    $this->assertContains('Archive complete.', $messages);
    $this->assertStringNotContainsString('The bard sang of dragons', $this->nodeText($room), 'Removed from the room');

    $links = $this->search('bard sang');
    $this->assertCount(1, $links, 'Found in the archive');
    $this->assertStringContainsString('/room/', $links[0]);
    $this->anonymous();
    $this->get($links[0]);
    $this->assertStatus(200);
    $this->assertStringContainsString('The bard sang of dragons', $this->content);
    $this->assertStringContainsString($player->name, $this->content);
  }

  public function testRecentPostsStay() {
    $this->configure('1 week');
    list($room) = $this->oldRoomPost('Fresh gossip');
    $this->archiveAll();
    $this->assertStringContainsString('Fresh gossip', $this->nodeText($room));
  }

  public function testPostsOutsideChatAndForumsStay() {
    $this->configure();
    $page = $this->site->createNode('page');
    $comment = $this->site->postComment($page, $this->site->createUser(array('player')), 'Session note');
    $this->site->backdateComment($comment, strtotime('-3 days'));
    $this->archiveAll();
    $this->assertStringContainsString('Session note', $this->nodeText($page));
  }

  public function testArchiveRecordsLastRun() {
    $this->configure();
    $this->archiveAll();
    $this->get('admin/vaxia/archives');
    $this->assertText('Archives last run: ' . date('m/d/Y'));
  }

  public function testSearchWithoutMatchesSaysSo() {
    $this->configure();
    $this->search('nothing like this anywhere');
    $this->assertText('No results found.');
  }

  // ---------------------------------------------------------------------------
  // Known bugs.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * SECURITY: the search runs `grep ... "<text>"` through the shell with only
   * addslashes() applied. $(...) inside double quotes still runs, so anyone who
   * may search the archives can run commands on the server. This probe only
   * creates an empty file in the public files directory.
   */
  public function testSearchTextIsNotRunAsACommand() {
    $this->configure();
    // Guard: the probe only proves anything if the web server can write files
    // there (archiving writes to the same directory).
    $this->oldRoomPost('Guard post');
    $this->assertContains('Archived 1 comments.', $this->archiveAll(), 'Web server can write to the files directory');
    $probe = DRUPAL_ROOT . '/' . $this->site->publicFilesPath() . '/' . self::PROBE;
    $this->search('$(touch ' . $probe . ')');
    $this->anonymous();
    $this->get($this->site->publicFilesPath() . '/' . self::PROBE);
    $this->assertStatus(404, 'The search text must not run');
  }

}
