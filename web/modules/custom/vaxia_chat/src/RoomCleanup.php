<?php

namespace Drupal\vaxia_chat;

use Drupal\node\Entity\Node;

/**
 * Deletes old rooms nobody posts in (D7 room_cleanup). A room stays while it,
 * or any room below it, has posts.
 *
 * D7's loop never ended once a room qualified (known bug); this one stops
 * when a pass protects no new parent room.
 */
class RoomCleanup {

  public static function run(): int {
    $messenger = \Drupal::messenger();
    $db = \Drupal::database();
    $cutoff = strtotime(\Drupal::config('vaxia_chat.settings')->get('room_cleanup_date') . ' ago');
    $messenger->addStatus('Cleaning up rooms, started looking at rooms older than: ' . date('m/d/Y 00:00:01', $cutoff) . '.');
    $protected = $db->query("SELECT DISTINCT c.entity_id FROM {comment_field_data} c INNER JOIN {node_field_data} n ON n.nid = c.entity_id WHERE c.entity_type = 'node' AND n.type = 'rpg_chatroom'")->fetchCol();
    $query = $db->select('node_field_data', 'n')->fields('n', ['nid'])->condition('n.type', 'rpg_chatroom')->condition('n.created', $cutoff, '<');
    if ($protected) {
      $query->condition('n.nid', $protected, 'NOT IN');
    }
    $candidates = $query->execute()->fetchCol();
    $messenger->addStatus('Identified ' . count($candidates) . ' rooms that may be cleaned up.');
    // Parents of rooms in use are in use too, up the tree.
    $frontier = $protected;
    while ($frontier) {
      $parents = $db->query("SELECT DISTINCT field_parent_target_id FROM {node__field_parent} WHERE bundle = 'rpg_chatroom' AND deleted = 0 AND entity_id IN (:ids[])", [':ids[]' => $frontier])->fetchCol();
      $frontier = array_values(array_diff($parents, $protected));
      $protected = array_merge($protected, $frontier);
    }
    $candidates = array_diff($candidates, $protected);
    $cleaned = 0;
    foreach ($candidates as $nid) {
      $posts = $db->query("SELECT COUNT(*) FROM {comment_field_data} WHERE entity_type = 'node' AND entity_id = :nid AND status = 1", [':nid' => $nid])->fetchField();
      if (!$posts && ($room = Node::load($nid))) {
        $room->delete();
        $cleaned++;
      }
    }
    $messenger->addStatus('Cleaned up ' . $cleaned . ' rooms.');
    \Drupal::logger('room_cleanup')->notice('Cleaned up @count rooms.', ['@count' => $cleaned]);
    \Drupal::state()->set('vaxia_chat.room_cleanup_last_run', date('m/d/Y H:i:s'));
    return $cleaned;
  }

}
