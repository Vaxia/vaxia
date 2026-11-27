<?php

namespace Drupal\vaxia_forum_access\Access;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Routing\Access\AccessInterface;
use Drupal\Core\Routing\RouteMatchInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\vaxia_forum_access\ForumAccess;

/**
 * comment/reply/{entity_type}/{entity}/... needs the forum's Post grant.
 */
class ReplyAccessCheck implements AccessInterface {

  public function access(RouteMatchInterface $route_match, AccountInterface $account) {
    $entity = $route_match->getParameter('entity');
    $tid = is_object($entity) ? ForumAccess::forumTid($entity) : 0;
    $result = (!$tid || ForumAccess::access('create', $tid, $account)) ? AccessResult::allowed() : AccessResult::forbidden('No Post grant in this forum.');
    $result->addCacheContexts(['user.roles', 'user.permissions'])->addCacheTags([ForumAccess::CACHE_TAG]);
    if (is_object($entity)) {
      $result->addCacheableDependency($entity);
    }
    return $result;
  }

}
