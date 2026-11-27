<?php

namespace Vaxia\Tests\Rpg\RpgChat;

use Vaxia\Tests\VaxiaTestCase;

/**
 * RPG chat: chat-enabled content types (RPG Chatroom) show their comments as a
 * live chat that refreshes over AJAX, with a list of who's in the room.
 *
 * @group rpg_chat
 */
class RpgChatTest extends VaxiaTestCase {

  /**
   * A player allowed into rooms. vaxia_chatrooms additionally requires
   * "view vaxia ic chat" for in-character rooms.
   */
  private function chatter(array $roles = array('player')) {
    return $this->site->createUser($roles, array('view rpg chat', 'view vaxia ic chat'));
  }

  private function userList() {
    $names = array();
    foreach ($this->xpath()->query('//div[@id="rpg-chat-userlist"]//li') as $li) {
      $names[] = trim($li->textContent);
    }
    return $names;
  }

  private function refresh($room) {
    $this->get('system/rpg_chat/' . $room->nid);
    return json_decode($this->content, TRUE);
  }

  private function setPageChat($on) {
    $this->inSession($this->superuser(), function () use ($on) {
      $this->submitForm('admin/structure/types/manage/page', array('rpg_chat' => $on), 'Save content type');
    });
  }

  // ---------------------------------------------------------------------------
  // Setup and access.
  // ---------------------------------------------------------------------------

  public function testChatSwitchOnContentTypeGatesAccess() {
    $this->withoutPermission('view rpg chat', array('anonymous user', 'authenticated user', 'player'), function () {
      $page = $this->site->createNode('page');
      $player = $this->site->createUser(array('player'));
      $this->setPageChat(TRUE);
      try {
        $this->login($player);
        $this->get('node/' . $page->nid);
        $this->assertStatus(403, 'Chat-enabled content needs "view rpg chat"');
      }
      finally {
        $this->setPageChat(FALSE);
      }
      $this->get('node/' . $page->nid);
      $this->assertStatus(200);
    });
  }

  public function testRoomsNeedViewRpgChat() {
    $this->withoutPermission('view rpg chat', array('anonymous user', 'authenticated user', 'player'), function () {
      $room = $this->site->createRoom();
      $this->login($this->site->createUser(array('player')));
      $this->get('node/' . $room->nid);
      $this->assertStatus(403);

      $this->login($this->chatter());
      $this->get('node/' . $room->nid);
      $this->assertStatus(200);
    });
  }

  public function testAnonymousCannotOpenRooms() {
    $this->withoutPermission('view rpg chat', array('anonymous user', 'authenticated user', 'player'), function () {
      $room = $this->site->createRoom();
      $this->anonymous();
      $this->get('node/' . $room->nid);
      $this->assertStatus(403);
    });
  }

  public function testChatAdminsCanOpenRooms() {
    $room = $this->site->createRoom();
    $this->login($this->site->createUser(array(), array('administer rpg chat', 'view vaxia ic chat')));
    $this->get('node/' . $room->nid);
    $this->assertStatus(200);
  }

  public function testOtherContentIsUnaffected() {
    $page = $this->site->createNode('page');
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $page->nid);
    $this->assertStatus(200);
  }

  // ---------------------------------------------------------------------------
  // Room page.
  // ---------------------------------------------------------------------------

  public function testRoomPageShowsChatWithUserListAndRefresh() {
    $room = $this->site->createRoom();
    $this->site->postComment($room, $this->chatter(), 'Hello from the tavern');
    $this->login($this->chatter());
    $this->get('node/' . $room->nid);

    $xpath = $this->xpath();
    $this->assertSame(1, $xpath->query('//div[@id="rpg-chat-wrapper"]//div[@id="rpg-chat"]')->length);
    $this->assertText('Hello from the tavern');
    $this->assertText('Users in this room');
    $this->assertGreaterThan(0, $xpath->query('//form[contains(@id, "comment-form")]//input[@value="Refresh"]')->length, 'Refresh button on the post form');
    $this->assertStringContainsString('rpg_chat/js/rpg_chat.js', $this->content);
    $this->assertStringContainsString('"refreshRate":60', $this->content);
  }

  public function testPresentUsersAreListed() {
    $room = $this->site->createRoom();
    $other = $this->chatter();
    $this->login($other);
    $this->get('node/' . $room->nid);
    $this->login($this->chatter());
    $this->get('node/' . $room->nid);
    $this->assertContains($other->name, $this->userList());
  }

  /**
   * Current behaviour, intent unconfirmed: loading a room prunes visits older
   * than a minute, though the list itself allows 15.
   */
  public function testVisitorsDropOffAfterAMinute() {
    $room = $this->site->createRoom();
    $gone = $this->chatter();
    $this->site->setRoomVisit($room, $gone, strtotime('-2 minutes'));
    $this->login($this->chatter());
    $this->get('node/' . $room->nid);
    $this->assertNotContains($gone->name, $this->userList());
  }

  // ---------------------------------------------------------------------------
  // Refresh endpoint.
  // ---------------------------------------------------------------------------

  public function testRefreshReturnsChatAsAjaxInsert() {
    $room = $this->site->createRoom();
    $this->site->postComment($room, $this->chatter(), 'Latest line');
    $me = $this->chatter();
    $this->login($me);

    $commands = $this->refresh($room);

    $this->assertCount(1, $commands);
    $this->assertSame('insert', $commands[0]['command']);
    $this->assertStringContainsString('id="rpg-chat-wrapper"', $commands[0]['data']);
    $this->assertStringContainsString('Latest line', $commands[0]['data']);
    $this->assertStringContainsString($me->name, $commands[0]['data'], 'You are in the user list');
  }

  public function testRefreshMarksYouPresentForOthers() {
    $room = $this->site->createRoom();
    $me = $this->chatter();
    $this->login($me);
    $this->refresh($room);
    $this->login($this->chatter());
    $this->get('node/' . $room->nid);
    $this->assertContains($me->name, $this->userList());
  }

  public function testRefreshOfMissingRoomReturnsNoChat() {
    $this->login($this->chatter());
    $this->get('system/rpg_chat/999999');
    $this->assertNull(json_decode($this->content, TRUE));
    $this->assertStringNotContainsString('rpg-chat-wrapper', $this->content);
  }

  /**
   * @group knownbug
   * SECURITY: system/rpg_chat/NID has "access callback" TRUE and never checks
   * room access. A locked room turns a non-member away from its page, but the
   * refresh URL hands them every post.
   */
  public function testRefreshRespectsRoomAccess() {
    $member = $this->chatter();
    $room = $this->site->createRoom(array('field_locked_to_users' => 1, 'field_users' => array(array('uid' => $member->uid))));
    $this->site->postComment($room, $member, 'Secret plans');

    $outsider = $this->chatter();
    $this->login($outsider);
    $this->get('node/' . $room->nid);
    $this->assertStringNotContainsString('Secret plans', $this->content, 'The page turns them away');

    $this->get('system/rpg_chat/' . $room->nid);
    $this->assertStringNotContainsString('Secret plans', $this->content);
  }

  // ---------------------------------------------------------------------------
  // Settings, forms and markup.
  // ---------------------------------------------------------------------------

  public function testContentTypesHaveAChatSwitch() {
    $this->login($this->superuser());
    $this->get('admin/structure/types/manage/rpg-chatroom');
    $this->assertText('RPG chat settings');
    $this->assertSame(1, $this->xpath()->query('//input[@name="rpg_chat" and @checked]')->length, 'On for chat rooms');
    $this->get('admin/structure/types/manage/page');
    $this->assertSame(0, $this->xpath()->query('//input[@name="rpg_chat" and @checked]')->length, 'Off for pages');
  }

  public function testRoomEditFormHasChatSettings() {
    $room = $this->site->createRoom();
    $this->login($this->superuser());
    $this->get('node/' . $room->nid . '/edit');
    $this->assertText('RPG chat settings');
  }

  public function testRoomsAndPostsCarryChatMarkup() {
    $room = $this->site->createRoom();
    $this->site->postComment($room, $this->chatter(), 'Marked up');
    $this->login($this->chatter());
    $this->get('node/' . $room->nid);
    $this->assertGreaterThan(0, $this->xpath()->query('//*[contains(concat(" ", @class, " "), " rpg-chat-node ")]')->length);
    $this->assertGreaterThan(0, $this->xpath()->query('//*[contains(concat(" ", @class, " "), " rpg-chat-post ")][contains(., "Marked up")]')->length);
  }

  public function testPostEditsAlwaysKeepARevision() {
    $room = $this->site->createRoom();
    $post = $this->site->postComment($room, $this->chatter(), 'Original words');
    $this->login($this->superuser());
    $this->get('comment/' . $post->cid . '/edit');
    $this->assertStatus(200);
    // The "Create new revision" box is hidden and forced on.
    $this->assertSame(0, $this->xpath()->query('//input[@type="checkbox" and @name="revision"]')->length);
  }

  public function testPlayersCannotOpenTheRpgAdminSection() {
    $this->login($this->site->createUser(array('player')));
    $this->get('admin/rpg');
    $this->assertStatus(403);
  }

  /**
   * @group knownbug
   * admin/rpg requires "administer rpg settings", a permission no module
   * defines: nobody but user 1 can open the RPG admin section (its settings
   * pages have their own permissions).
   */
  public function testRpgAdminsCanOpenTheRpgAdminSection() {
    $this->login($this->site->createUser(array(), array('administer rpg chat', 'access administration pages')));
    $this->get('admin/rpg');
    $this->assertStatus(200);
  }

  public function testDeletedRoomsReturnNoChat() {
    $room = $this->site->createRoom();
    $this->inSession($this->superuser(), function () use ($room) {
      $this->submitForm('node/' . $room->nid . '/delete', array(), 'Delete');
    });
    $this->login($this->chatter());
    $this->get('system/rpg_chat/' . $room->nid);
    $this->assertStringNotContainsString('rpg-chat-wrapper', $this->content);
  }

}
