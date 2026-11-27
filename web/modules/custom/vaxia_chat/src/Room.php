<?php

namespace Drupal\vaxia_chat;

use Drupal\node\NodeInterface;

/**
 * Chat rooms: which content is a room, and who is in it (D7 rpg_chat).
 */
class Room {

  public static function isChatType(string $type): bool {
    return in_array($type, \Drupal::config('vaxia_chat.settings')->get('rpg_chat_types') ?: [], TRUE);
  }

  public static function isRoom($node): bool {
    return $node instanceof NodeInterface && self::isChatType($node->bundle());
  }

  /**
   * Why an account may not open a room (D7 vaxia_chatrooms_node_view()), or
   * NULL when it may. Rooms marked mature need an 18+ player; locked rooms
   * need a player on the room's user list; in-character rooms need "view
   * vaxia ic chat". "bypass vaxia ic chat" skips the checks.
   *
   * @return string|null
   *   The message to show, '' to turn away silently.
   */
  public static function denyReason(NodeInterface $room, \Drupal\Core\Session\AccountInterface $account): ?string {
    if ($account->hasPermission('bypass vaxia ic chat')) {
      return NULL;
    }
    $flag = fn($entity, $field) => $entity && $entity->hasField($field) && (bool) $entity->get($field)->value;
    $mature = $flag($room, 'field_mature_posting');
    $locked = $flag($room, 'field_locked_to_users');
    $user = $account->isAnonymous() ? NULL : \Drupal\user\Entity\User::load($account->id());
    if ($mature && !$flag($user, 'field_mature')) {
      return $account->isAnonymous() ? '' : 'You are not allowed to view this room. You are underage.';
    }
    if ($locked) {
      $listed = $user && $room->hasField('field_users') && in_array((int) $account->id(), array_map('intval', array_column($room->get('field_users')->getValue(), 'target_id')), TRUE);
      if (!$listed) {
        return $account->isAnonymous() ? '' : 'You are not allowed to view this room. This room is locked.';
      }
    }
    if (!$account->hasPermission('view vaxia ic chat') && !\Drupal\vaxia_core\Character::isLimbo($room)) {
      return 'You are not allowed to view this room. This room is locked to anonymous users.';
    }
    $shadowbanned = FALSE;
    \Drupal::moduleHandler()->invokeAllWith('vaxia_chat_hidden_user', function (callable $hook) use ($account, &$shadowbanned) {
      $shadowbanned = $shadowbanned || $hook($account);
    });
    if ($shadowbanned && ($mature || $locked)) {
      return 'You are not allowed to view this room. This room is locked to other users.';
    }
    return NULL;
  }

  /**
   * Marks the current user present (not shadowbanned users).
   */
  public static function markPresent(int $nid): void {
    $account = \Drupal::currentUser();
    if ($account->isAnonymous()) {
      return;
    }
    $db = \Drupal::database();
    $db->delete('rpg_chat_room_users')->condition('timestamp', strtotime('-15 minutes'), '<')->execute();
    $db->delete('rpg_chat_room_users')->condition('nid', $nid)->condition('uid', $account->id())->execute();
    $hidden = FALSE;
    \Drupal::moduleHandler()->invokeAllWith('vaxia_chat_hidden_user', function (callable $hook) use ($account, &$hidden) {
      $hidden = $hidden || $hook($account);
    });
    if ($hidden) {
      return;
    }
    $db->merge('rpg_chat_room_users')->keys(['nid' => $nid, 'uid' => $account->id()])->fields(['timestamp' => \Drupal::time()->getCurrentTime()])->execute();
  }

  /**
   * Names of users who had the room open in the last minute.
   */
  public static function presentNames(int $nid): array {
    $db = \Drupal::database();
    $db->delete('rpg_chat_room_users')->condition('timestamp', strtotime('-1 minutes'), '<')->execute();
    $names = $db->query('SELECT u.name FROM {rpg_chat_room_users} r LEFT JOIN {users_field_data} u ON u.uid = r.uid WHERE r.nid = :nid ORDER BY r.timestamp', [':nid' => $nid])->fetchCol();
    return array_map(fn($n) => $n !== NULL && $n !== 'null' ? $n : (string) t('Lurker*'), $names);
  }

  /**
   * "Users in this room" and the names (the heading shows even when empty).
   */
  public static function userList(int $nid): array {
    return [
      '#type' => 'inline_template',
      '#template' => '<div id="rpg-chat-userlist" class="rpg-chat-userlist"><div class="item-list"><h3>{{ title }}</h3><ul>{% for name in names %}<li>{{ name }}</li>{% endfor %}</ul></div></div>',
      '#context' => ['title' => t('Users in this room'), 'names' => self::presentNames($nid)],
    ];
  }

  public static function save(NodeInterface $node): void {
    \Drupal::database()->merge('rpg_chat_room')->keys(['nid' => $node->id()])->fields([
      'min_refresh' => 60,
      'allowed_users' => serialize([]),
      'banned_users' => serialize([]),
    ])->execute();
  }

  public static function delete(int $nid): void {
    \Drupal::database()->delete('rpg_chat_room')->condition('nid', $nid)->execute();
    \Drupal::database()->delete('rpg_chat_room_users')->condition('nid', $nid)->execute();
  }

  /**
   * Room nid => title, for settings pages.
   */
  public static function all(): array {
    return \Drupal::database()->query('SELECT r.nid, n.title FROM {rpg_chat_room} r LEFT JOIN {node_field_data} n ON r.nid = n.nid ORDER BY n.title')->fetchAllKeyed();
  }

  /**
   * Web-safe colour: letters and digits only; hex values get a "#".
   */
  public static function color(string $value): string {
    $named = ['aqua', 'black', 'blue', 'fuchsia', 'gray', 'green', 'lime', 'maroon', 'navy', 'olive', 'purple', 'red', 'silver', 'teal', 'white', 'yellow'];
    $value = preg_replace('/[^0-9a-z]/i', '', strtolower(trim($value)));
    return in_array($value, $named, TRUE) ? $value : '#' . $value;
  }

}
