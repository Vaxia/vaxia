<?php

namespace Drupal\vaxia_chat\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\node\Entity\Node;
use Drupal\vaxia_chat\Room;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\Response;

/**
 * The chat refresh: the room's posts and user list as one AJAX insert
 * command, without building the post form (D7 rpg_chat_refresh()).
 *
 * Unlike D7, it checks the visitor may view the room (known bug: anyone could
 * read any room through this path).
 */
class Refresh extends ControllerBase {

  public function refresh($nid) {
    $room = Node::load((int) $nid);
    if (!$room || !Room::isRoom($room) || !$room->access('view') || Room::denyReason($room, $this->currentUser()) !== NULL) {
      return new Response('');
    }
    // Drop "Your comment has been posted" and the like.
    $this->messenger()->deleteByType('status');
    Room::markPresent((int) $room->id());
    $comments = [];
    $field = NULL;
    foreach ($room->getFieldDefinitions() as $name => $definition) {
      if ($definition->getType() === 'comment') {
        $field = $name;
      }
    }
    if ($field && ($this->currentUser()->hasPermission('access comments') || $this->currentUser()->hasPermission('administer comments'))) {
      $storage = $this->entityTypeManager()->getStorage('comment');
      $per_page = (int) ($room->getFieldDefinition($field)->getSetting('per_page') ?: 50);
      $thread = $storage->loadThread($room, $field, \Drupal\comment\CommentManagerInterface::COMMENT_MODE_FLAT, $per_page);
      if ($thread) {
        $comments = $this->entityTypeManager()->getViewBuilder('comment')->viewMultiple($thread);
      }
    }
    $build = [
      '#type' => 'container',
      '#attributes' => ['id' => 'rpg-chat-wrapper', 'class' => ['rpg-chat-wrapper']],
      'users' => Room::userList((int) $room->id()),
      'chat' => ['#type' => 'container', '#attributes' => ['id' => 'rpg-chat', 'class' => ['rpg-chat']], 'comments' => $comments],
    ];
    $data = (string) \Drupal::service('renderer')->renderRoot($build);
    return new JsonResponse([[
      'command' => 'insert',
      'method' => NULL,
      'selector' => NULL,
      'data' => $data,
      'settings' => NULL,
    ]]);
  }

}
