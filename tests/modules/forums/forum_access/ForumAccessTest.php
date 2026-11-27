<?php

namespace Vaxia\Tests\Forums\ForumAccess;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Per-forum role grants (D7 forum_access, vaxia_forum_access on D11): View,
 * Post (new topics and replies), Edit posts, Delete posts per forum, set on
 * the forum / container edit form. A forum without grants behaves like a new
 * forum in D7: everyone views, logged-in users post. Admins (bypass content
 * access) see and do everything.
 *
 * D11 only: the D7 test site doesn't run forum_access.
 *
 * @group forum_access
 */
class ForumAccessTest extends VaxiaTestCase {

  /** @var object Forum administrator (administrator role: bypasses content access). */
  private $admin;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('Forum access was ported to the D11 site only (the D7 test site has no forum_access).');
    }
    $this->admin = $this->site->createUser(array('administrator'), array('rebuild node access permissions'));
  }

  /**
   * Adds a forum (or container) through the admin UI; returns its tid.
   */
  private function addForum($kind = 'forum', $parent = 0) {
    $name = $this->site->uniqueName($kind === 'forum' ? 'Forum' : 'Container');
    $this->login($this->admin);
    $this->submitForm('admin/structure/forum/add/' . $kind, array(
      'name[0][value]' => $name,
      'parent[0]' => (string) $parent,
    ), 'Save');
    $this->assertNoPhpErrors();
    return $this->site->term('forums', $name);
  }

  /**
   * Sets a forum's grid through its edit form: role => ops, every other box
   * unticked.
   */
  private function setGrants($tid, array $grants, $kind = 'forum') {
    $this->login($this->admin);
    $this->get('admin/structure/forum/edit/' . $kind . '/' . $tid);
    $this->assertStatus(200);
    $edit = array();
    foreach ($this->xpath()->query('//input[@type="checkbox" and starts-with(@name, "vaxia_forum_access[grants]")]') as $box) {
      preg_match('/^vaxia_forum_access\[grants\]\[([^\]]+)\]\[([^\]]+)\]$/', $box->getAttribute('name'), $m);
      $edit[$box->getAttribute('name')] = in_array($m[2], $grants[$m[1]] ?? array(), TRUE);
    }
    $this->assertNotEmpty($edit, 'The forum form has the access grid');
    $this->submitForm(NULL, $edit, 'Save');
    $this->assertNoPhpErrors();
  }

  /**
   * Posts a topic through node/add/forum, as $options['author'] (default the
   * forum admin); 'owner' and 'status' use the admin's authoring options.
   */
  private function topic($tid, array $options = array()) {
    $title = $this->site->uniqueName('Topic');
    $edit = array('title[0][value]' => $title, 'taxonomy_forums' => (string) $tid);
    if (isset($options['owner'])) {
      $edit['uid[0][target_id]'] = $options['owner']->name . ' (' . $options['owner']->uid . ')';
    }
    if (isset($options['status'])) {
      $edit['status[value]'] = (bool) $options['status'];
    }
    $url = $this->inSession($options['author'] ?? $this->admin, function () use ($edit) {
      $this->submitForm('node/add/forum', $edit, 'Save');
      $this->assertSame(array(), $this->messages('error'));
      return $this->url;
    });
    $this->assertMatchesRegularExpression('#/node/\d+$#', $url, 'The topic was saved');
    preg_match('#/node/(\d+)$#', $url, $m);
    return (object) array('nid' => (int) $m[1], 'title' => $title);
  }

  private function forumName($tid) {
    return $this->inSession($this->admin, function () use ($tid) {
      $this->get('forum/' . $tid);
      return trim($this->xpath()->query('//h1')->item(0)->textContent);
    });
  }

  /**
   * Rebuilds content access through the status report (its no-JS batch).
   *
   * The site's baseline carries the "everyone views everything" grant it had
   * before any module granted node access, which listings honour until the
   * node access table is rebuilt.
   */
  private function rebuildContentAccess() {
    $this->login($this->admin);
    $this->submitForm('admin/reports/status/rebuild', array(), 'Rebuild permissions');
    for ($i = 0; $i < 50 && preg_match('#http-equiv="Refresh" content="0; URL=([^"]+)"#', $this->content, $m); $i++) {
      $this->get(html_entity_decode($m[1]));
    }
    $this->assertStatus(200);
    $this->assertNoText('need to be rebuilt');
  }

  /**
   * Values of the forum select on the open node form.
   */
  private function forumOptions() {
    $values = array();
    foreach ($this->xpath()->query('//select[@name="taxonomy_forums"]/option') as $option) {
      $values[] = $option->getAttribute('value');
    }
    return $values;
  }

  public function testRestrictedForumIsHiddenAndDeniedToRolesWithoutView() {
    $forum = $this->addForum();
    $this->setGrants($forum, array('lead' => array('view', 'create'), 'administrator' => array('view', 'create')));
    $name = $this->forumName($forum);
    // The forum module's "New forum topics" block lists topics through a
    // node access query.
    $this->site->placeBlock('forum', 'forum_new_block', 'sidebar_first');
    $topic = $this->topic($forum);
    $this->rebuildContentAccess();
    $player = $this->site->createUser(array('player'));
    $lead = $this->site->createUser(array('lead'));

    foreach (array(NULL, $player) as $account) {
      $who = $account ? 'player' : 'anonymous';
      $account ? $this->login($account) : $this->anonymous();
      $this->get('forum');
      $this->assertStatus(200);
      $this->assertStringNotContainsString($name, $this->pageText(), "$who doesn't see the forum on the index");
      $this->get('forum/' . $forum);
      $this->assertStatus(403);
      $this->get('node/' . $topic->nid);
      $this->assertStatus(403);
      $this->get('forum');
      $this->assertStringNotContainsString($topic->title, $this->pageText(), "$who doesn't see the topic in the new topics block");
    }

    $this->login($lead);
    $this->get('forum');
    $this->assertText($name);
    $this->get('forum/' . $forum);
    $this->assertStatus(200);
    $this->assertText($topic->title);
    $this->get('node/' . $topic->nid);
    $this->assertStatus(200);
    $this->assertText($topic->title);
    $this->get('user/' . $lead->uid);
    $this->assertText($topic->title);
  }

  public function testForumWithoutGrantsIsOpenToAllAndLoggedInUsersPost() {
    $forum = $this->site->term('forums', $this->site->uniqueName('Open'));
    $topic = $this->topic($forum);
    $this->anonymous();
    $this->get('forum');
    $this->assertText($this->forumName($forum));
    $this->get('forum/' . $forum);
    $this->assertStatus(200);
    $this->assertText($topic->title);
    $this->get('node/' . $topic->nid);
    $this->assertStatus(200);
    $this->get('node/add/forum', array('forum_id' => $forum));
    $this->assertStatus(403);

    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('forum/' . $forum);
    $this->assertText('Add new Forum topic');
    $this->get('node/add/forum', array('forum_id' => $forum));
    $this->assertStatus(200);
    $this->assertContains((string) $forum, $this->forumOptions());
  }

  public function testHiddenContainerHidesTheForumsInIt() {
    $container = $this->addForum('container');
    $forum = $this->addForum('forum', $container);
    $this->setGrants($container, array('lead' => array('view', 'create')), 'container');
    $forumName = $this->forumName($forum);
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('forum');
    $this->assertStringNotContainsString($forumName, $this->pageText());
    $this->get('forum/' . $container);
    $this->assertStatus(403);
    $this->get('forum/' . $forum);
    $this->assertStatus(403);

    $this->login($this->site->createUser(array('lead')));
    $this->get('forum');
    $this->assertText($forumName);
    $this->get('forum/' . $forum);
    $this->assertStatus(200);
  }

  public function testUnpublishedTopicIsForItsAuthorOnly() {
    $forum = $this->addForum();
    $author = $this->site->createUser(array('player'));
    $topic = $this->topic($forum, array('owner' => $author, 'status' => FALSE));
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $topic->nid);
    $this->assertStatus(403);
    $this->get('forum/' . $forum);
    $this->assertStringNotContainsString($topic->title, $this->pageText());
    $this->site->grantPermissions('player', array('view own unpublished content'));
    try {
      $this->login($author);
      $this->get('node/' . $topic->nid);
      $this->assertStatus(200);
    }
    finally {
      $this->site->revokePermissions('player', array('view own unpublished content'));
    }
  }

  public function testPostGrantLimitsTheForumSelectAndIsCheckedOnSubmit() {
    $open = $this->addForum();
    $readOnly = $this->addForum();
    $this->setGrants($open, array('player' => array('view', 'create')));
    $this->setGrants($readOnly, array('player' => array('view')));
    $player = $this->site->createUser(array('player'));
    $this->login($player);

    $this->get('forum/' . $open);
    $this->assertText('Add new Forum topic');
    $this->get('forum/' . $readOnly);
    $this->assertStatus(200);
    $this->assertNoText('Add new Forum topic');

    $this->get('node/add/forum', array('forum_id' => $readOnly));
    $this->assertStatus(403);
    $this->get('node/add/forum');
    $this->assertStatus(200);
    $options = $this->forumOptions();
    $this->assertContains((string) $open, $options);
    $this->assertNotContains((string) $readOnly, $options);

    // A tampered select is refused.
    $sneaky = $this->site->uniqueName('Sneaky');
    $this->submitForm(NULL, array('title[0][value]' => $sneaky, 'taxonomy_forums' => (string) $readOnly), 'Save');
    $this->assertNotEmpty($this->messages('error'), 'Posting in a forum without Post is refused');
    $this->login($this->admin);
    $this->get('forum/' . $readOnly);
    $this->assertNoText($sneaky);

    $this->login($player);
    $fine = $this->site->uniqueName('Fine');
    $this->submitForm('node/add/forum', array('title[0][value]' => $fine, 'taxonomy_forums' => (string) $open), 'Save');
    $this->assertSame(array(), $this->messages('error'));
    $this->get('forum/' . $open);
    $this->assertText($fine);
  }

  public function testRepliesNeedThePostGrant() {
    $readOnly = $this->addForum();
    $open = $this->addForum();
    $this->setGrants($readOnly, array('player' => array('view')));
    $this->setGrants($open, array('player' => array('view', 'create')));
    $closedTopic = $this->topic($readOnly);
    $openTopic = $this->topic($open);
    $reply = $this->site->postComment($closedTopic, $this->superuser(), 'An admin reply');
    $this->login($this->site->createUser(array('player')));

    $this->get('node/' . $closedTopic->nid);
    $this->assertStatus(200);
    $this->assertSame(0, $this->xpath()->query('//form[contains(@class, "comment-form")]')->length, 'No reply form without Post');
    $this->assertSame(0, $this->xpath()->query('//a[contains(@href, "/comment/reply/")]')->length, 'No reply links without Post');
    $this->get('comment/reply/node/' . $closedTopic->nid . '/comment_forum');
    $this->assertStatus(403);
    $this->get('comment/reply/node/' . $closedTopic->nid . '/comment_forum/' . $reply->cid);
    $this->assertStatus(403);

    $this->get('node/' . $openTopic->nid);
    $this->assertSame(1, $this->xpath()->query('//form[contains(@class, "comment-form")]')->length, 'Reply form with Post');
    $text = $this->site->uniqueName('Reply');
    $this->submitForm(NULL, array('comment_body[0][value]' => $text), 'Save');
    $this->get('node/' . $openTopic->nid);
    $this->assertText($text);
  }

  public function testEditAndDeleteGrants() {
    $forum = $this->addForum();
    $this->setGrants($forum, array(
      'player' => array('view', 'create'),
      'lead' => array('view', 'update', 'delete'),
    ));
    $player = $this->site->createUser(array('player'));
    $lead = $this->site->createUser(array('lead'));
    $topic = $this->topic($forum);
    $own = $this->topic($forum, array('author' => $player));
    $reply = $this->site->postComment($own, $player, 'A player reply');

    $this->login($player);
    $this->get('node/' . $topic->nid . '/edit');
    $this->assertStatus(403);
    $this->get('node/' . $topic->nid . '/delete');
    $this->assertStatus(403);
    // Own topics: "edit own forum content", which players hold; no delete.
    $this->get('node/' . $own->nid . '/edit');
    $this->assertStatus(200);
    $this->get('node/' . $own->nid . '/delete');
    $this->assertStatus(403);

    $this->login($this->site->createUser(array('player')));
    $this->get('comment/' . $reply->cid . '/edit');
    $this->assertStatus(403);

    $this->login($lead);
    $this->get('node/' . $topic->nid . '/edit');
    $this->assertStatus(200);
    $this->get('node/' . $topic->nid . '/delete');
    $this->assertStatus(200);
    $this->get('comment/' . $reply->cid . '/edit');
    $this->assertStatus(200);
    $this->get('comment/' . $reply->cid . '/delete');
    $this->assertStatus(200);
  }

  public function testAdminsBypassTheGrants() {
    $forum = $this->addForum();
    $this->setGrants($forum, array('lead' => array('view')));
    $topic = $this->topic($forum);
    $admin = $this->site->createUser(array('administrator'));
    $this->login($admin);
    $this->get('forum');
    $this->assertText($this->forumName($forum));
    $this->get('forum/' . $forum);
    $this->assertStatus(200);
    $this->assertText($topic->title);
    $this->get('node/' . $topic->nid . '/edit');
    $this->assertStatus(200);
    $this->get('node/add/forum', array('forum_id' => $forum));
    $this->assertStatus(200);
    $this->assertContains((string) $forum, $this->forumOptions());
  }

  public function testGridDefaultsSavesAndLoads() {
    $this->login($this->admin);
    $this->get('admin/structure/forum/add/forum');
    $checked = function ($role, $op) {
      return $this->xpath()->query('//input[@name="vaxia_forum_access[grants][' . $role . '][' . $op . ']" and @checked]')->length === 1;
    };
    // New forums: everyone views, logged-in users post (D7's defaults).
    $this->assertTrue($checked('anonymous', 'view'));
    $this->assertTrue($checked('authenticated', 'view'));
    $this->assertTrue($checked('authenticated', 'create'));
    $this->assertFalse($checked('anonymous', 'create'));
    $this->assertFalse($checked('player', 'view'));

    $forum = $this->addForum();
    $this->setGrants($forum, array('mediator' => array('view', 'create', 'update'), 'lead' => array('delete')));
    $this->get('admin/structure/forum/edit/forum/' . $forum);
    $this->assertTrue($checked('mediator', 'view'));
    $this->assertTrue($checked('mediator', 'create'));
    $this->assertTrue($checked('mediator', 'update'));
    $this->assertFalse($checked('mediator', 'delete'));
    $this->assertTrue($checked('lead', 'delete'));
    $this->assertFalse($checked('anonymous', 'view'));
    $this->assertFalse($checked('authenticated', 'view'));

    // Containers: View and "See in the forum selection list" only.
    $container = $this->addForum('container');
    $this->get('admin/structure/forum/edit/container/' . $container);
    $this->assertSame(1, $this->xpath()->query('//input[@name="vaxia_forum_access[grants][player][create]"]')->length);
    $this->assertSame(0, $this->xpath()->query('//input[@name="vaxia_forum_access[grants][player][update]"]')->length);
  }

}
