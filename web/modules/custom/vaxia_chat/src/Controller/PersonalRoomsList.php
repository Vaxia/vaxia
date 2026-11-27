<?php

namespace Drupal\vaxia_chat\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\node\Entity\Node;

/**
 * Staff list of personal rooms with their characters and settings (D7 view
 * personal_rooms at character-sheets/personal-rooms).
 */
class PersonalRoomsList extends ControllerBase {

  public function page() {
    $rows = [];
    $pairs = \Drupal::database()->query("SELECT f.entity_id AS character_nid, f.field_personal_room_target_id AS room_nid FROM {node__field_personal_room} f INNER JOIN {node_field_data} r ON r.nid = f.field_personal_room_target_id WHERE r.status = 1 AND r.type = 'rpg_chatroom' ORDER BY r.title")->fetchAll();
    $yes = fn($node, $field) => $node->hasField($field) && $node->get($field)->value ? $this->t('Yes') : $this->t('No');
    foreach ($pairs as $pair) {
      $room = Node::load($pair->room_nid);
      $character = Node::load($pair->character_nid);
      if (!$room || !$character) {
        continue;
      }
      $users = [];
      foreach ($room->hasField('field_users') ? $room->get('field_users') : [] as $item) {
        if ($item->entity) {
          $users[] = $item->entity->getDisplayName();
        }
      }
      $rows[] = [
        $room->toLink(),
        $character->toLink(),
        ($character->getOwner() ? $character->getOwner()->getDisplayName() : ''),
        $yes($room, 'field_is_personal_room'),
        $yes($room, 'field_private_room'),
        $yes($room, 'field_mature_posting'),
        $yes($room, 'field_locked_to_users'),
        implode(', ', $users),
      ];
    }
    return [
      '#type' => 'table',
      '#header' => [$this->t('Room'), $this->t('Character'), $this->t('Player'), $this->t('Personal'), $this->t('Public'), $this->t('Mature'), $this->t('Locked'), $this->t('Users')],
      '#rows' => $rows,
      '#empty' => $this->t('No personal rooms.'),
      '#cache' => ['max-age' => 0],
    ];
  }

}
