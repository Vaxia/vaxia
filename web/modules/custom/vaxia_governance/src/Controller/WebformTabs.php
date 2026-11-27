<?php

namespace Drupal\vaxia_governance\Controller;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\node\NodeInterface;

/**
 * D7 webform's per-node tabs, kept at their paths: the ballot builder,
 * results, settings and e-mails (the last two closed for proposals).
 */
class WebformTabs extends ControllerBase {

  private static function hasWebform(NodeInterface $node): bool {
    return $node->hasField('webform') && !$node->get('webform')->isEmpty();
  }

  public static function ballotAccess(AccountInterface $account, NodeInterface $node) {
    return AccessResult::allowedIf($node->bundle() === 'proposal' && self::hasWebform($node) && $node->access('update', $account))->cachePerUser()->addCacheableDependency($node);
  }

  /**
   * Proposal results: staff any time, others once it closes. Other webforms:
   * whoever may see all their submissions.
   */
  public static function resultsAccess(AccountInterface $account, NodeInterface $node) {
    if (!self::hasWebform($node)) {
      return AccessResult::forbidden();
    }
    if ($node->bundle() === 'proposal') {
      return AccessResult::allowedIf($account->hasPermission('administer proposal') || (!vaxia_governance_is_open($node) && $node->access('view', $account)))->setCacheMaxAge(0);
    }
    return AccessResult::allowedIf($account->hasPermission('view webform submissions any node') || ($account->hasPermission('view webform submissions own node') && (int) $node->getOwnerId() === (int) $account->id()))->cachePerUser();
  }

  /**
   * Settings and e-mails: node editors, never on proposals.
   */
  public static function settingsAccess(AccountInterface $account, NodeInterface $node) {
    return AccessResult::allowedIf(self::hasWebform($node) && $node->bundle() !== 'proposal' && $node->access('update', $account))->cachePerUser()->addCacheableDependency($node);
  }

  public function results(NodeInterface $node) {
    if ($node->bundle() === 'proposal') {
      return vaxia_governance_results($node) + ['#cache' => ['max-age' => 0]];
    }
    return $this->redirect('entity.node.webform.results_submissions', ['node' => $node->id()]);
  }

  public function settings(NodeInterface $node) {
    return $this->entityFormBuilder()->getForm($node->get('webform')->entity, 'settings');
  }

  public function emails(NodeInterface $node) {
    $rows = [];
    foreach ($node->get('webform')->entity->getHandlers() as $handler) {
      $rows[] = [$handler->label(), $handler->getPluginId()];
    }
    return ['#type' => 'table', '#header' => [$this->t('E-mail'), $this->t('Type')], '#rows' => $rows, '#empty' => $this->t('No e-mails are set up for this form.')];
  }

}
