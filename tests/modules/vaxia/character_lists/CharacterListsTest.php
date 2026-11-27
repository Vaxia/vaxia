<?php

namespace Vaxia\Tests\Vaxia\CharacterLists;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Character sheet lists (D7 views vaxia_character_sheets and
 * administration_characters): character-sheets(/all), character-sheets/mine,
 * user/%/characters, character-sheets/evaluate and admin/content/characters.
 *
 * The D7 test site doesn't carry these views, so this runs on D11 only.
 *
 * @group character_lists
 */
class CharacterListsTest extends VaxiaTestCase {

  protected function setUp(): void {
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site lacks the character sheet list views.');
    }
    parent::setUp();
  }

  private function player() {
    return $this->site->createUser(array('player'));
  }

  /**
   * A character in a workflow state (a state label, or NULL for creation).
   */
  private function character($owner, $state, array $values = array()) {
    $character = $this->site->createCharacter($owner, $values + array('title' => $this->site->uniqueName('Char')));
    if ($state) {
      $this->site->forceWorkflowState($character, $state);
    }
    return $character;
  }

  /**
   * First-column text of each table body row on the last page.
   */
  private function names() {
    $names = array();
    foreach ($this->xpath()->query('//table//tbody/tr[td]') as $row) {
      $names[] = trim($row->getElementsByTagName('td')->item(0)->textContent);
    }
    return $names;
  }

  private function headers() {
    $headers = array();
    foreach ($this->xpath()->query('//table//thead//th') as $th) {
      // The text before a click-sort marker ("Sort ascending").
      $headers[] = trim(preg_split('/\s*\n/', trim($th->textContent))[0]);
    }
    return $headers;
  }

  private function staff($role) {
    $account = $this->site->createUser(array($role));
    $this->site->removeRole($account, 'new account');
    return $account;
  }

  // ---------------------------------------------------------------------------
  // character-sheets, character-sheets/all.
  // ---------------------------------------------------------------------------

  public function testAllListsApprovedPublishedPlayerCharacters() {
    $owner = $this->player();
    $approved = $this->character($owner, 'approved');
    $submitted = $this->character($owner, 'submitted');
    $npc = $this->character($owner, 'approved', array('field_is_npc' => 1));
    $unpublished = $this->character($owner, 'approved', array('status' => 0));
    $blocked = $this->site->createUser(array('player'), array(), array('status' => 0));
    $blockedChar = $this->character($blocked, 'approved');

    $this->login($this->player());
    foreach (array('character-sheets', 'character-sheets/all') as $path) {
      $this->get($path, array('uid' => $owner->name . ',' . $blocked->name));
      $this->assertStatus(200);
      $this->assertText('Character Sheets');
      $this->assertSame(array($approved->title), $this->names(), $path);
      $this->assertSame(array('Name', 'Player', 'Posted', 'Last updated', 'Current state'), $this->headers());
      $this->assertText('approved');
      $this->assertText($owner->name);
      $this->assertRaw('/node/' . $approved->nid . '"');
    }
  }

  public function testAllIsOldestFirstThirtyFiveAPageAndClickSortable() {
    $owner = $this->player();
    $titles = array();
    for ($i = 0; $i < 36; $i++) {
      $character = $this->character($owner, 'approved', array('title' => $this->site->uniqueName('Pager' . chr(90 - ($i % 26)))));
      $this->site->backdateNodeCreated($character, 1500000000 + $i * 60);
      $titles[] = $character->title;
    }
    $this->site->expireViewsCache();

    $this->login($this->player());
    $this->get('character-sheets/all', array('uid' => $owner->name));
    $this->assertSame(array_slice($titles, 0, 35), $this->names());
    $this->get('character-sheets/all', array('uid' => $owner->name, 'page' => 1));
    $this->assertSame(array($titles[35]), $this->names());

    $sorted = $titles;
    sort($sorted);
    $this->get('character-sheets/all', array('uid' => $owner->name, 'order' => 'Name', 'sort' => 'asc'));
    $this->assertSame(array_slice($sorted, 0, 35), $this->names());
  }

  public function testAllIsOpenToVisitorsAndPlayersOnly() {
    $owner = $this->player();
    $approved = $this->character($owner, 'approved');

    $this->anonymous();
    $this->get('character-sheets/all', array('uid' => $owner->name));
    $this->assertStatus(200);
    $this->assertSame(array($approved->title), $this->names());

    $this->login($this->staff('storyteller'));
    $this->get('character-sheets/all');
    $this->assertStatus(403);
  }

  public function testUnknownPlayerNameListsNothing() {
    $this->character($this->player(), 'approved');
    $this->login($this->player());
    $this->get('character-sheets/all', array('uid' => 'no_such_player_xyz'));
    $this->assertStatus(200);
    $this->assertText('Unable to find user: no_such_player_xyz');
    $this->assertSame(array(), $this->names());
  }

  public function testTabs() {
    $this->login($this->player());
    $this->get('character-sheets/all');
    $this->assertRaw('href="/character-sheets/mine"');
    $this->assertText('My characters');
  }

  // ---------------------------------------------------------------------------
  // character-sheets/mine.
  // ---------------------------------------------------------------------------

  public function testMineListsYourPublishedPlayerCharactersByName() {
    $owner = $this->player();
    $realm = $this->site->term('realm', 'Vaxia');
    $b = $this->character($owner, 'approved', array('title' => $this->site->uniqueName('Bmine')));
    $a = $this->character($owner, 'draft', array('title' => $this->site->uniqueName('Amine')));
    $c = $this->character($owner, 'submitted', array('title' => $this->site->uniqueName('Cmine')));
    $d = $this->character($owner, 'inactive', array('title' => $this->site->uniqueName('Dmine')));
    $hidden = array(
      $this->character($owner, 'disapproved'),
      $this->character($owner, NULL),
      $this->character($owner, 'approved', array('field_is_npc' => 1)),
      $this->character($owner, 'approved', array('status' => 0)),
      $this->character($this->player(), 'approved'),
    );

    $this->login($owner);
    $this->get('character-sheets/mine');
    $this->assertStatus(200);
    $this->assertText('My character sheets');
    $this->assertSame(array($a->title, $b->title, $c->title, $d->title), $this->names());
    $this->assertSame(array('Name', 'Player', 'Realm', 'Posted', 'Last updated', 'Current state'), $this->headers());
    $this->assertText('Vaxia');
    $this->assertTrue($realm > 0);
    foreach ($hidden as $character) {
      $this->assertNoText($character->title);
    }
  }

  public function testMineVariesByViewer() {
    $first = $this->player();
    $second = $this->player();
    $mine = $this->character($first, 'approved');
    $theirs = $this->character($second, 'approved');

    $this->login($first);
    $this->get('character-sheets/mine');
    $this->assertText($mine->title);
    $this->login($second);
    $this->get('character-sheets/mine');
    $this->assertText($theirs->title);
    $this->assertNoText($mine->title);

    $this->anonymous();
    $this->get('character-sheets/mine');
    $this->assertStatus(200);
    $this->assertSame(array(), $this->names());
  }

  // ---------------------------------------------------------------------------
  // user/%/characters.
  // ---------------------------------------------------------------------------

  public function testUserTabShowsApprovedCharactersToOthers() {
    $owner = $this->player();
    $approved = $this->character($owner, 'approved', array('title' => $this->site->uniqueName('Bapp')));
    $unpublished = $this->character($owner, 'approved', array('title' => $this->site->uniqueName('Aunpub'), 'status' => 0));
    $draft = $this->character($owner, 'draft');
    $npc = $this->character($owner, 'approved', array('field_is_npc' => 1));

    $this->login($this->player());
    $this->get('user/' . $owner->uid . '/characters');
    $this->assertStatus(200);
    $this->assertText('These are the characters for this user that have been approved for play.');
    $this->assertRaw('href="/character-sheets/mine"');
    $this->assertText('Click here to see all of your own character sheets.');
    // Security fix (D7 had no published filter): other players don't see
    // unpublished sheets.
    $this->assertSame(array($approved->title), $this->names());
    $this->assertNoText($draft->title);
    $this->assertNoText($npc->title);

    // Staff (and the owner) still do.
    foreach (array('administrator', 'lead', 'storyteller') as $role) {
      $this->login($this->site->createUser(array($role)));
      $this->get('user/' . $owner->uid . '/characters');
      $this->assertSame(array($unpublished->title, $approved->title), $this->names(), $role);
    }
    $this->login($owner);
    $this->get('user/' . $owner->uid . '/characters');
    $this->assertContains($unpublished->title, $this->names());
  }

  public function testUserTabShowsYourOwnWorkInProgress() {
    $owner = $this->player();
    $approved = $this->character($owner, 'approved', array('title' => $this->site->uniqueName('Aown')));
    $draft = $this->character($owner, 'draft', array('title' => $this->site->uniqueName('Bown')));
    $submitted = $this->character($owner, 'submitted', array('title' => $this->site->uniqueName('Cown')));
    $inactive = $this->character($owner, 'inactive', array('title' => $this->site->uniqueName('Down')));
    $disapproved = $this->character($owner, 'disapproved');

    $this->login($owner);
    $this->get('user/' . $owner->uid . '/characters');
    $this->assertSame(array($approved->title, $draft->title, $submitted->title, $inactive->title), $this->names());
    $this->assertNoText($disapproved->title);
    $this->assertRaw('href="/user/' . $owner->uid . '/characters"');
  }

  public function testUserTabForUnknownUserIsNotFound() {
    $this->login($this->player());
    $this->get('user/987654321/characters');
    $this->assertStatus(404);
  }

  // ---------------------------------------------------------------------------
  // character-sheets/evaluate.
  // ---------------------------------------------------------------------------

  public function testEvaluateIsForLeadsStorytellersAndAdmins() {
    foreach (array('lead', 'storyteller', 'administrator') as $role) {
      $this->login($this->staff($role));
      $this->get('character-sheets/evaluate');
      $this->assertStatus(200);
      $this->assertText('Evaluation');
    }
    $this->login($this->player());
    $this->get('character-sheets/evaluate');
    $this->assertStatus(403);
  }

  public function testEvaluateListsSubmittedAndInactiveMostRecentlyUpdatedFirst() {
    $owner = $this->player();
    $old = $this->character($owner, 'submitted');
    $inactive = $this->character($owner, 'inactive');
    $commented = $this->character($owner, 'submitted');
    $approved = $this->character($owner, 'approved');
    $this->site->backdateNode($old, 1500000000);
    $this->site->backdateNode($inactive, 1500000100);
    $this->site->backdateNode($commented, 1500000000);
    // Comment statistics start at the node's creation; a comment newer than
    // the edit counts as the last update.
    $this->site->setLastCommentTime($old, 1500000000);
    $this->site->setLastCommentTime($inactive, 1500000000);
    $this->site->setLastCommentTime($commented, 1500000200);
    $this->site->expireViewsCache();

    $this->login($this->staff('lead'));
    $this->get('character-sheets/evaluate', array('uid' => $owner->name));
    $this->assertSame(array($commented->title, $inactive->title, $old->title), $this->names());
    $this->assertNoText($approved->title);
    $this->assertSame(array('Name', 'Player', 'Posted', 'Last updated', 'Current state', 'edit'), $this->headers());
    $this->assertRaw('href="/node/' . $old->nid . '/edit"');
    $this->assertText('Characters marked as Other require approval from the Setting department.');

    $this->get('character-sheets/evaluate', array('uid' => $owner->name, 'sid' => $this->optionValue('sid', 'inactive')));
    $this->assertSame(array($inactive->title), $this->names());
  }

  public function testEvaluateLeavesOutBlockedAccounts() {
    $blocked = $this->site->createUser(array('player'), array(), array('status' => 0));
    $character = $this->character($blocked, 'submitted');
    $this->login($this->staff('lead'));
    $this->get('character-sheets/evaluate');
    $this->assertNoText($character->title);
  }

  // ---------------------------------------------------------------------------
  // admin/content/characters.
  // ---------------------------------------------------------------------------

  public function testAdminListsEveryCharacterNewestFirst() {
    $owner = $this->player();
    $older = $this->character($owner, 'approved');
    $unpublished = $this->character($owner, 'draft', array('status' => 0));
    $npc = $this->character($owner, NULL, array('field_is_npc' => 1));
    $this->site->backdateNodeCreated($older, 1500000000);
    $this->site->backdateNodeCreated($unpublished, 1500000100);
    $this->site->backdateNodeCreated($npc, 1500000200);
    $this->site->expireViewsCache();

    $this->login($this->staff('administrator'));
    $this->get('admin/content/characters', array('uid' => $owner->name));
    $this->assertStatus(200);
    $this->assertText('Administration: Characters');
    $this->assertSame(array($npc->title, $unpublished->title, $older->title), $this->names());
    $this->assertSame(array('Title', 'Author', 'Published', 'Post date', 'Revised by', 'Updated date', 'Operations', 'Delete link', 'Current state'), $this->headers());
    $this->assertRaw('href="/node/' . $older->nid . '/delete"');

    $this->get('admin/content/characters', array('uid' => $owner->name, 'field_is_npc_value' => '1'));
    $this->assertSame(array($npc->title), $this->names());
    $this->get('admin/content/characters', array('uid' => $owner->name, 'sid' => 'character_evaluation_draft'));
    $this->assertSame(array($unpublished->title), $this->names());
    $this->get('admin/content/characters', array('title' => $older->title));
    $this->assertSame(array($older->title), $this->names());
  }

  public function testAdminListNeedsContentOverviewAccess() {
    // D7 let anyone with "access content" (visitors too) see this list.
    $this->login($this->player());
    $this->get('admin/content/characters');
    $this->assertStatus(403);
    $this->anonymous();
    $this->get('admin/content/characters');
    $this->assertStatus(403);
  }

}
