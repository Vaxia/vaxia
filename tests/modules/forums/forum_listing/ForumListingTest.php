<?php

namespace Vaxia\Tests\Forums\ForumListing;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Forum listings cover every node type filed into a forum, as D7's did:
 * events, scenes, session reports and proposals with a forum show in its
 * topic list, its topic and reply counts and its last post, with their own
 * replies counted (contrib forum only counted forum topics' comment_forum).
 *
 * The D7 test site's forum is core forum; this checks the D11 port only.
 *
 * @group forums
 */
class ForumListingTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('Checks the D11 forum listing override.');
    }
  }

  /**
   * The forum's own row in the /forum index: [topics, posts] as text.
   */
  private function forumCounts($tid) {
    $this->get('forum');
    $xpath = $this->xpath();
    $row = $xpath->query('//tr[.//a[contains(@href, "/forum/' . $tid . '")]]')->item(0);
    $this->assertNotNull($row, 'Forum ' . $tid . ' listed');
    // Forum, Topics, Posts, Last post (the cells carry no classes).
    $cells = $xpath->query('./td', $row);
    // The Topics cell also carries an "N new posts" link after the count.
    return array(trim($cells->item(1)->firstChild->textContent), trim($cells->item(2)->textContent));
  }

  /**
   * Topic list rows of a forum page: title => replies.
   */
  private function topics($tid) {
    $this->get('forum/' . $tid);
    $xpath = $this->xpath();
    $topics = array();
    foreach ($xpath->query('//table//tbody/tr') as $tr) {
      $title = NULL;
      foreach ($xpath->query('.//td[contains(@class, "forum__topic")]//a', $tr) as $a) {
        if (preg_match('#/node/\d+$#', $a->getAttribute('href'))) {
          $title = $a;
          break;
        }
      }
      $replies = $xpath->query('.//td[contains(@class, "forum__replies")]', $tr)->item(0);
      if ($title) {
        // The count, then contrib's "N new posts" link when there is one.
        $topics[trim($title->textContent)] = $replies && $replies->firstChild ? trim($replies->firstChild->textContent) : NULL;
      }
    }
    return $topics;
  }

  public function testEventsAndScenesInAForumAreListedWithTheirReplies() {
    $tid = $this->site->createTermAt('forums', 90001, $this->site->uniqueName('Board'));
    $author = $this->site->createUser(array('player'));
    $topic = $this->site->createNode('forum', array('title' => $this->site->uniqueName('Topic'), 'uid' => $author->uid, 'taxonomy_forums' => $tid));
    $event = $this->site->createNode('event', array('title' => $this->site->uniqueName('Event'), 'uid' => $author->uid, 'taxonomy_forums' => $tid));
    $scene = $this->site->createNode('scene', array('title' => $this->site->uniqueName('Scene'), 'uid' => $author->uid, 'taxonomy_forums' => $tid));
    $this->site->postComment($topic, $author, 'One');
    $this->site->postComment($event, $author, 'Two');
    $this->site->postComment($event, $author, 'Three');

    $this->login($this->site->createUser(array('administrator')));
    $topics = $this->topics($tid);
    $this->assertSame('1', $topics[$topic->title] ?? NULL, 'Forum topic replies');
    $this->assertSame('2', $topics[$event->title] ?? NULL, 'Event replies');
    $this->assertSame('0', $topics[$scene->title] ?? NULL, 'Scene listed, no replies');

    // Three topics; posts = topics + replies (forum's own sum).
    $this->assertSame(array('3', '6'), $this->forumCounts($tid));
  }

}
