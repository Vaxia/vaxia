<?php

namespace Drupal\vaxia_forum_access;

use Drupal\Core\DependencyInjection\ContainerBuilder;
use Drupal\Core\DependencyInjection\ServiceProviderBase;

/**
 * Swaps in forum listings and index counts that cover every node type filed
 * into a forum (see VaxiaForumManager, VaxiaForumIndexStorage).
 */
class VaxiaForumAccessServiceProvider extends ServiceProviderBase {

  public function alter(ContainerBuilder $container) {
    if ($container->hasDefinition('forum_manager')) {
      $container->getDefinition('forum_manager')->setClass(VaxiaForumManager::class);
    }
    if ($container->hasDefinition('forum.index_storage')) {
      $container->getDefinition('forum.index_storage')->setClass(VaxiaForumIndexStorage::class);
    }
  }

}
