<?php

namespace Drupal\vaxia_forum_access;

use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\user\Entity\Role;

/**
 * Per-forum role grants, as D7 forum_access 7.x-1.3 kept them.
 *
 * Grants per forum term and role: view, create ("post": new topics and
 * replies), update ("edit posts"), delete ("delete posts"). A forum with no
 * rows behaves as forum_access's defaults for a new forum (what its
 * hook_enable() wrote for forums without rows): anonymous may view,
 * authenticated may view and post.
 */
class ForumAccess {

  /**
   * Node access realm.
   */
  const REALM = 'vaxia_forum_access';

  /**
   * Cache tag invalidated whenever grants change.
   */
  const CACHE_TAG = 'vaxia_forum_access';

  /**
   * Grant columns, keyed by operation.
   */
  const OPS = ['view', 'create', 'update', 'delete'];

  /**
   * The grants forum_access gave a forum that had no rows.
   */
  public static function defaults(): array {
    return [
      'anonymous' => ['view' => 1, 'create' => 0, 'update' => 0, 'delete' => 0],
      'authenticated' => ['view' => 1, 'create' => 1, 'update' => 0, 'delete' => 0],
    ];
  }

  /**
   * Stable node access grant id for a role machine name.
   */
  public static function gid(string $role): int {
    return crc32($role);
  }

  /**
   * The forum vocabulary id.
   */
  public static function vocabulary(): string {
    return (string) \Drupal::config('forum.settings')->get('vocabulary');
  }

  /**
   * Whether an entity is a term in the forum vocabulary.
   */
  public static function isForumTerm(EntityInterface $entity): bool {
    return $entity->getEntityTypeId() === 'taxonomy_term' && $entity->bundle() === static::vocabulary();
  }

  /**
   * The forum a node is in (its first taxonomy_forums item), or 0.
   *
   * Like D7's forum_tid, every node type with the forum field counts (forum,
   * event, proposal, scene, session_report).
   */
  public static function forumTid(EntityInterface $node): int {
    if ($node->getEntityTypeId() !== 'node' || !$node->hasField('taxonomy_forums') || $node->get('taxonomy_forums')->isEmpty()) {
      return 0;
    }
    return (int) $node->get('taxonomy_forums')->target_id;
  }

  /**
   * Whether the forum has its own rows (otherwise defaults() apply).
   */
  public static function hasRows(int $tid): bool {
    return (bool) \Drupal::database()->select('vaxia_forum_access', 'fa')
      ->fields('fa', ['tid'])
      ->condition('tid', $tid)
      ->range(0, 1)
      ->execute()
      ->fetchField();
  }

  /**
   * Grants of a forum: role => [view, create, update, delete].
   */
  public static function grants(int $tid): array {
    $cache = &drupal_static(__METHOD__, []);
    if (!isset($cache[$tid])) {
      $grants = [];
      $rows = \Drupal::database()->select('vaxia_forum_access', 'fa')
        ->fields('fa')
        ->condition('tid', $tid)
        ->execute();
      foreach ($rows as $row) {
        $grants[$row->role] = [
          'view' => (int) $row->grant_view,
          'create' => (int) $row->grant_create,
          'update' => (int) $row->grant_update,
          'delete' => (int) $row->grant_delete,
        ];
      }
      $cache[$tid] = $grants ?: static::defaults();
    }
    return $cache[$tid];
  }

  /**
   * Replaces a forum's rows: role => [op => 0|1]; roles left out get none.
   */
  public static function save(int $tid, array $grants): void {
    $db = \Drupal::database();
    $transaction = $db->startTransaction();
    $db->delete('vaxia_forum_access')->condition('tid', $tid)->execute();
    $insert = $db->insert('vaxia_forum_access')->fields(['tid', 'role', 'grant_view', 'grant_update', 'grant_delete', 'grant_create']);
    foreach ($grants as $role => $ops) {
      $insert->values([
        'tid' => $tid,
        'role' => $role,
        'grant_view' => (int) !empty($ops['view']),
        'grant_update' => (int) !empty($ops['update']),
        'grant_delete' => (int) !empty($ops['delete']),
        'grant_create' => (int) !empty($ops['create']),
      ]);
    }
    $insert->execute();
    unset($transaction);
    static::reset();
  }

  /**
   * Drops static caches and cached pages that depend on grants.
   */
  public static function reset(): void {
    drupal_static_reset(static::class . '::grants');
    drupal_static_reset(static::class . '::access');
    \Drupal::service('cache_tags.invalidator')->invalidateTags([static::CACHE_TAG]);
    \Drupal::entityTypeManager()->getAccessControlHandler('node')->resetCache();
    \Drupal::entityTypeManager()->getAccessControlHandler('taxonomy_term')->resetCache();
    \Drupal::entityTypeManager()->getAccessControlHandler('comment')->resetCache();
  }

  /**
   * D7 forum_access_access(): whether the account has a grant in a forum.
   *
   * @param string $op
   *   view, create, update or delete.
   * @param int $tid
   *   The forum; 0 means "in any forum".
   */
  public static function access(string $op, int $tid, AccountInterface $account): bool {
    if ($account->hasPermission('bypass node access')) {
      return TRUE;
    }
    if ($op === 'delete' && $account->hasPermission('delete any forum content')) {
      return TRUE;
    }
    if ($op === 'update' && $account->hasPermission('edit any forum content')) {
      return TRUE;
    }
    $cache = &drupal_static(__METHOD__, []);
    $key = $account->id() . ':' . implode(',', $account->getRoles()) . ':' . $op . ':' . $tid;
    if (!isset($cache[$key])) {
      $tids = [$tid];
      if ($tid === 0) {
        $tids = \Drupal::database()->select('taxonomy_term_field_data', 't')
          ->fields('t', ['tid'])
          ->condition('vid', static::vocabulary())
          ->distinct()
          ->execute()
          ->fetchCol();
      }
      $cache[$key] = FALSE;
      foreach ($tids as $one) {
        foreach (static::grants((int) $one) as $role => $ops) {
          if (!empty($ops[$op]) && in_array($role, $account->getRoles(), TRUE)) {
            $cache[$key] = TRUE;
            break 2;
          }
        }
      }
    }
    return $cache[$key];
  }

  /**
   * View grant on a forum term and on every container above it.
   *
   * D7 hid a forum whose container was hidden (the forum tree is built from
   * the visible terms only), so a hidden container hides what it holds.
   */
  public static function canViewTree(int $tid, AccountInterface $account): bool {
    $parents = \Drupal::entityTypeManager()->getStorage('taxonomy_term')->loadAllParents($tid);
    foreach (array_keys($parents) ?: [$tid] as $one) {
      if (!static::access('view', (int) $one, $account)) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Roles with "bypass node access" (D7 gave them no records: useless).
   */
  public static function bypassRoles(): array {
    $roles = [];
    foreach (Role::loadMultiple() as $id => $role) {
      if ($role->isAdmin() || $role->hasPermission('bypass node access')) {
        $roles[$id] = $id;
      }
    }
    return $roles;
  }

  /**
   * Re-writes node_access for the nodes in a forum.
   *
   * @return bool
   *   FALSE when the forum is too big and a full rebuild was flagged instead.
   */
  public static function rewriteForumNodes(int $tid, int $limit = 500): bool {
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $nids = $storage->getQuery()->accessCheck(FALSE)->condition('taxonomy_forums', $tid)->execute();
    if (count($nids) > $limit) {
      \Drupal::service('Drupal\node\NodeAccessRebuild')->setNeedsRebuild(TRUE);
      return FALSE;
    }
    $handler = \Drupal::entityTypeManager()->getAccessControlHandler('node');
    $grant_storage = \Drupal::service('node.grant_storage');
    foreach (array_chunk($nids, 50) as $chunk) {
      $storage->resetCache($chunk);
      foreach ($storage->loadMultiple($chunk) as $node) {
        $grant_storage->write($node, $handler->acquireGrants($node));
      }
    }
    return TRUE;
  }

}
