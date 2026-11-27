<?php

namespace Drupal\vaxia_chat;

use Drupal\Core\Session\AccountInterface;
use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Character;

/**
 * Personal rooms: a room per approved player character, which the
 * character's player may partly edit (D7 personal_rooms and its rule).
 */
class PersonalRooms {

  /**
   * The character whose personal room this is.
   */
  public static function owner(NodeInterface $room): ?NodeInterface {
    $nid = \Drupal::database()->query('SELECT entity_id FROM {node__field_personal_room} WHERE field_personal_room_target_id = :nid', [':nid' => $room->id()])->fetchField();
    return $nid ? Node::load($nid) : NULL;
  }

  /**
   * Whether the account edits the room only as its character's player.
   */
  public static function editsAsOwner(NodeInterface $room, AccountInterface $account): bool {
    if ($room->bundle() !== 'rpg_chatroom' || !$account->hasPermission('edit personal_rooms')) {
      return FALSE;
    }
    $owner = self::owner($room);
    return $owner && $owner->getOwnerId() == $account->id();
  }

  /**
   * Whether the account may edit the room the normal way.
   */
  public static function normalEditAccess(NodeInterface $room, AccountInterface $account): bool {
    return $account->hasPermission('bypass node access') || $account->hasPermission('edit any rpg_chatroom content')
      || ($room->getOwnerId() == $account->id() && $account->hasPermission('edit own rpg_chatroom content'));
  }

  public static function fieldList(string $key): array {
    return array_values(array_filter(array_map('trim', explode("\n", (string) \Drupal::config('vaxia_chat.settings')->get($key)))));
  }

  /**
   * Approving a player character gives it a personal room (D7 rule "Personal
   * rooms - create personal room"). The room now gets the character's realm
   * (known bug: the rule copied a field that doesn't exist).
   */
  public static function onCharacterApproved(NodeInterface $character): void {
    if ($character->get('field_personal_room')->entity || Character::isNpc($character) || Character::flag($character, 'field_is_open_npc') || Character::isStoryteller($character)) {
      return;
    }
    $room = Node::create([
      'type' => 'rpg_chatroom',
      'title' => $character->label() . ' Personal Room',
      'uid' => $character->getOwnerId(),
      'status' => 1,
      'field_private_room' => 0,
      'field_mature_posting' => 1,
      'field_is_personal_room' => 1,
      'field_room_description' => [
        'value' => 'A personal room is a room owned by a specific character. It does not show up on searches and posts here will not be automatically archived. Use this room for personal posts, crafting or other RP you want to keep to yourself. Be careful not to lock yourself out of the room if you set the users able to access the room.',
        'format' => 'wiki',
      ],
    ]);
    if ($character->hasField('field_tag_realm')) {
      $room->set('field_tag_realm', $character->get('field_tag_realm')->getValue());
    }
    foreach ($room->getFieldDefinitions() as $name => $definition) {
      if ($definition->getType() === 'comment') {
        $room->set($name, ['status' => 2]);
      }
    }
    $room->save();
    $character->set('field_personal_room', $room->id());
    $character->save();
  }

}
