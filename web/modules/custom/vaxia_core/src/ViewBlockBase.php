<?php

namespace Drupal\vaxia_core;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Session\AccountInterface;

/**
 * Base for blocks that replace D7 Views list blocks: the D7 view title as the
 * default block title, D7 role access, and the D7 views markup around the
 * rows (view-header / view-content / view-empty / view-footer), so theme CSS
 * written for the D7 view keeps working.
 */
abstract class ViewBlockBase extends BlockBase {

  /**
   * D7 view name, display id and view title.
   */
  const VIEW = '';
  const DISPLAY = '';
  const TITLE = '';

  public function defaultConfiguration() {
    return ['label' => static::TITLE] + parent::defaultConfiguration();
  }

  /**
   * D7 views "role" access: any of the roles (D11 role ids). The superuser
   * passes, as uid 1 did on D7.
   */
  protected function roleAccess(AccountInterface $account, array $roles): AccessResult {
    if ((int) $account->id() === 1) {
      return AccessResult::allowed()->addCacheContexts(['user']);
    }
    return AccessResult::allowedIf((bool) array_intersect($roles, $account->getRoles()))->addCacheContexts(['user.roles']);
  }

  /**
   * The D7 views wrapper around the parts given (header, exposed, content,
   * empty, footer; NULL or [] parts are left out).
   */
  protected function view(array $parts, array $cache, array $classes = []): array {
    $build = [
      '#type' => 'container',
      '#attributes' => ['class' => array_merge(['view', 'view-' . str_replace('_', '-', static::VIEW), 'view-id-' . static::VIEW, 'view-display-id-' . static::DISPLAY], $classes)],
      '#cache' => $cache,
    ];
    foreach ($parts as $key => $part) {
      if ($part === NULL || $part === [] || $part === '') {
        continue;
      }
      $build[$key] = [
        '#type' => 'container',
        '#attributes' => ['class' => [$key === 'exposed' ? 'view-filters' : 'view-' . $key]],
        'part' => is_array($part) ? $part : ['#markup' => $part],
      ];
    }
    return $build;
  }

  /**
   * Published nodes of a webform type that are open, or were changed within
   * $window seconds (D7: Webform status open OR updated > -N), newest first.
   *
   * @return array
   *   [nodes, max-age until the oldest closed one drops out].
   */
  protected function openOrRecent(string $type, int $window, int $limit): array {
    $now = \Drupal::time()->getRequestTime();
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)
      ->condition('type', $type)
      ->condition('status', 1)
      ->sort('created', 'DESC')
      ->range(0, $limit);
    $query->condition($query->orConditionGroup()
      ->condition('webform.status', 'open')
      ->condition('changed', $now - $window, '>'));
    $nodes = $storage->loadMultiple($query->execute());
    $max_age = -1;
    foreach ($nodes as $node) {
      if ($node->get('webform')->status !== 'open') {
        $left = max(0, $node->getChangedTime() + $window - $now);
        $max_age = $max_age < 0 ? $left : min($max_age, $left);
      }
    }
    return [$nodes, $max_age];
  }

  /**
   * D7 webform status field, as the views rewrote it: "(Open)" / "(Closed)".
   */
  protected function webformStatus($node): string {
    return '(' . ($node->get('webform')->status === 'open' ? $this->t('Open') : $this->t('Closed')) . ')';
  }

}
