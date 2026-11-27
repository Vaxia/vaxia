<?php

namespace Vaxia\Tests\Rpg\PersonalRooms;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Personal rooms: approving a player character creates a private chat room the
 * player may partly edit; admins choose which room fields players can touch.
 *
 * @group personal_rooms
 */
class PersonalRoomsTest extends VaxiaTestCase {

  /**
   * A chat room owned (through a character's personal room field) by a new player.
   */
  private function ownedRoom(array $playerPermissions = array('edit personal_rooms')) {
    $player = $this->site->createUser(array('player'), $playerPermissions);
    $room = $this->site->createRoom(array('title' => $this->site->uniqueName('Room')));
    $character = $this->site->createCharacter($player, array('field_personal_room' => $room->nid));
    return array($player, $character, $room);
  }

  private function setRoomFields(array $edit, array $view = array()) {
    $this->inSession($this->superuser(), function () use ($edit, $view) {
      $this->submitForm('admin/rpg/personal_rooms', array(
        'personal_rooms_fields_edit' => implode("\n", $edit),
        'personal_rooms_fields_view' => implode("\n", $view),
      ), 'Save configuration');
    });
  }

  /**
   * The personal room linked from a character's page, or NULL.
   */
  private function personalRoomOf($character) {
    return $this->inSession($this->superuser(), function () use ($character) {
      $this->get('node/' . $character->nid);
      foreach ($this->xpath()->query('//a[contains(text(), "Personal Room")]') as $a) {
        if (preg_match('#/node/(\d+)$#', $a->getAttribute('href'), $m)) {
          return (object) array('nid' => (int) $m[1], 'title' => trim($a->textContent));
        }
      }
      return NULL;
    });
  }

  // ---------------------------------------------------------------------------
  // Settings page.
  // ---------------------------------------------------------------------------

  public function testAdminSavesFieldLists() {
    $this->login($this->site->createUser(array(), array('admin personal_rooms')));
    $this->submitForm('admin/rpg/personal_rooms', array(
      'personal_rooms_fields_edit' => "title\nfield_private_room",
      'personal_rooms_fields_view' => 'field_mature_posting',
    ), 'Save configuration');
    $this->assertContains('The configuration options have been saved.', $this->messages('status'));
    $this->get('admin/rpg/personal_rooms');
    $this->assertSame("title\nfield_private_room", trim(str_replace("\r", '', $this->xpath()->query('//textarea[@name="personal_rooms_fields_edit"]')->item(0)->textContent)));
  }

  public function testSettingsNeedAdminPermission() {
    $this->login($this->site->createUser(array('player'), array('edit personal_rooms')));
    $this->get('admin/rpg/personal_rooms');
    $this->assertStatus(403);
  }

  // ---------------------------------------------------------------------------
  // Editing your own room.
  // ---------------------------------------------------------------------------

  public function testOwnerCanOpenRoomEditPage() {
    list($player, , $room) = $this->ownedRoom();
    $this->login($player);
    $this->get('node/' . $room->nid . '/edit');
    $this->assertStatus(200);
  }

  public function testOwnerWithoutPermissionCannotEdit() {
    $this->withoutPermission('edit personal_rooms', array('anonymous user', 'authenticated user', 'player'), function () {
      list($player, , $room) = $this->ownedRoom(array());
      $this->login($player);
      $this->get('node/' . $room->nid . '/edit');
      $this->assertStatus(403);
    });
  }

  public function testOtherPlayerCannotEditSomeoneElsesRoom() {
    list(, , $room) = $this->ownedRoom();
    $this->login($this->site->createUser(array('player'), array('edit personal_rooms')));
    $this->get('node/' . $room->nid . '/edit');
    $this->assertStatus(403);
  }

  public function testPermissionDoesNotOpenRoomsWithoutOwner() {
    $room = $this->site->createRoom();
    $this->login($this->site->createUser(array('player'), array('edit personal_rooms')));
    $this->get('node/' . $room->nid . '/edit');
    $this->assertStatus(403);
  }

  public function testOwnerOnlySeesAllowedFields() {
    list($player, , $room) = $this->ownedRoom();
    $this->setRoomFields(array('title', 'field_private_room'), array('field_mature_posting'));
    $this->login($player);
    $this->get('node/' . $room->nid . '/edit');

    $xpath = $this->xpath();
    $this->assertSame(1, $xpath->query('//input[(@name="title" or @name="title[0][value]") and not(@disabled)]')->length, 'Title editable');
    $this->assertGreaterThan(0, $xpath->query('//*[starts-with(@name, "field_private_room") and not(@disabled)]')->length, 'Private room editable');
    $this->assertGreaterThan(0, $xpath->query('//*[starts-with(@name, "field_mature_posting") and @disabled]')->length, 'Mature posting shown read-only');
    $this->assertSame(0, $xpath->query('//*[starts-with(@name, "field_room_description")]')->length, 'Description hidden');
    $this->assertSame(0, $xpath->query('//*[starts-with(@name, "field_users")]')->length, 'Users hidden');
  }

  public function testOwnerCanSaveAllowedField() {
    list($player, , $room) = $this->ownedRoom();
    $this->setRoomFields(array('title'));
    $this->login($player);
    $this->submitForm('node/' . $room->nid . '/edit', array('title' => 'My renamed room'), 'Save');
    $this->assertStringContainsString('My renamed room', $this->nodeText($room));
  }

  public function testEditorsWithNormalAccessSeeFullForm() {
    list(, , $room) = $this->ownedRoom();
    $this->setRoomFields(array('title'));
    $this->login($this->site->createUser(array(), array('edit personal_rooms', 'edit any rpg_chatroom content', 'view rpg chat')));
    $this->get('node/' . $room->nid . '/edit');
    $this->assertStatus(200);
    $this->assertGreaterThan(0, $this->xpath()->query('//*[starts-with(@name, "field_room_description")]')->length);
  }

  public function testNormalNodeEditAccessStillApplies() {
    $author = $this->site->createUser(array(), array('create page content', 'edit own page content'));
    $page = $this->site->createNode('page', array('uid' => $author->uid));
    $this->login($author);
    $this->get('node/' . $page->nid . '/edit');
    $this->assertStatus(200);

    $this->login($this->site->createUser());
    $this->get('node/' . $page->nid . '/edit');
    $this->assertStatus(403);
  }

  // ---------------------------------------------------------------------------
  // Room creation on character approval.
  // ---------------------------------------------------------------------------

  public function testApprovingPlayerCharacterCreatesPersonalRoom() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player, array('title' => $this->site->uniqueName('Aria')));

    $this->approveCharacter($character);

    $room = $this->personalRoomOf($character);
    $this->assertNotNull($room, 'Character page links its personal room');
    $this->assertSame($character->title . ' Personal Room', $room->title);
    $this->login($this->superuser());
    $this->get('node/' . $room->nid . '/edit');
    $xpath = $this->xpath();
    // Author box: "name" on D7; "uid[0][target_id]" holding "name (uid)" on D11.
    $this->assertStringStartsWith($player->name, $xpath->query('//input[@name="name" or @name="uid[0][target_id]"]')->item(0)->getAttribute('value'), 'Owned by the player');
    $this->assertSame(0, $xpath->query('//input[starts-with(@name, "field_private_room") and @checked]')->length, 'Not private');
    $this->assertSame(1, $xpath->query('//input[starts-with(@name, "field_mature_posting") and @checked]')->length, 'Mature posting on');
    $this->assertSame(1, $xpath->query('//input[starts-with(@name, "field_is_personal_room") and @checked]')->length, 'Marked personal');
    $this->assertStringContainsString('A personal room is a room owned by a specific character', $xpath->query('//textarea[starts-with(@name, "field_room_description")]')->item(0)->textContent);
  }

  public function testReapprovalKeepsExistingRoom() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player);
    $this->approveCharacter($character);
    $first = $this->personalRoomOf($character);
    $this->assertNotNull($first);

    $this->transition($character, 'draft');
    $this->approveCharacter($character);

    $this->assertSame($first->nid, $this->personalRoomOf($character)->nid);
  }

  public function testNoRoomForNpcOrStorytellerCharacters() {
    $player = $this->site->createUser(array('player'));
    foreach (array('field_is_npc', 'field_is_open_npc', 'field_is_storyteller') as $flag) {
      $character = $this->site->createCharacter($player, array($flag => 1));
      $this->approveCharacter($character);
      $this->assertNull($this->personalRoomOf($character), "No room when $flag is set");
    }
  }

  /**
   * @group knownbug
   * The rule copies node:field-realm into the room, but neither type has
   * field_realm (realm is field_tag_realm): the action fails (logged) and the
   * room never gets the character's realm.
   */
  public function testPersonalRoomGetsCharactersRealm() {
    $player = $this->site->createUser(array('player'));
    $this->site->term('realm', 'Sirian');
    $character = $this->site->createCharacter($player, array('field_tag_realm' => $this->site->term('realm', 'Sirian')));
    $this->approveCharacter($character);
    $room = $this->personalRoomOf($character);
    $this->login($this->superuser());
    $this->get('node/' . $room->nid . '/edit');
    $selected = $this->xpath()->query('//select[starts-with(@name, "field_tag_realm")]/option[@selected]')->item(0);
    $this->assertSame('Sirian', $selected ? trim($selected->textContent) : NULL);
  }

  // ---------------------------------------------------------------------------
  // Staff list view.
  // ---------------------------------------------------------------------------

  public function testStaffListPage() {
    $room = $this->site->createRoom(array('field_is_personal_room' => 1));
    // The list reaches rooms through the owning character.
    $this->site->createCharacter($this->site->createUser(array('player')), array('field_personal_room' => $room->nid));
    $this->login($this->site->createUser(array('lead')));
    $this->get('character-sheets/personal-rooms');
    $this->assertStatus(200);
    $this->assertText($room->title);
  }

  public function testStaffListHiddenFromPlayers() {
    $this->login($this->site->createUser(array('player')));
    $this->get('character-sheets/personal-rooms');
    $this->assertStatus(403);
  }

}
