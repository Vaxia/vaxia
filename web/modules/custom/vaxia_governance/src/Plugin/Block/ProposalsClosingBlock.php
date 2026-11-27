<?php

namespace Drupal\vaxia_governance\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Open proposals last revised eleven or more days ago (so closing soon), for
 * voters (D7 view proposals, display proposals_closing).
 */
#[Block(id: 'proposals-proposals_closing', admin_label: new TranslatableMarkup('Proposals closing'))]
class ProposalsClosingBlock extends ViewBlockBase {

  const VIEW = 'proposals';
  const DISPLAY = 'proposals_closing';
  const TITLE = 'Proposals closing';

  protected function blockAccess(AccountInterface $account) {
    return $this->roleAccess($account, ['voter']);
  }

  public function build() {
    $now = \Drupal::time()->getRequestTime();
    $since = $now - 11 * 86400;
    // Open published proposals whose current revision is 11+ days old (D7
    // node_revision.timestamp), newest first; D7 paged 15 at a time.
    $query = \Drupal::database()->select('node_field_data', 'n');
    $query->join('node_revision', 'r', 'r.vid = n.vid');
    $query->join('node__webform', 'w', 'w.entity_id = n.nid');
    $query->fields('n', ['nid']);
    $query->condition('n.type', 'proposal')
      ->condition('n.status', 1)
      ->condition('w.webform_status', 'open')
      ->condition('r.revision_timestamp', $since, '<=')
      ->orderBy('n.created', 'DESC')
      ->range(0, 15)
      ->addTag('node_access');
    $nodes = \Drupal::entityTypeManager()->getStorage('node')->loadMultiple($query->execute()->fetchCol());
    // Proposals still young today come of age later.
    $next = \Drupal::database()->query("SELECT MIN(r.revision_timestamp) FROM {node_field_data} n INNER JOIN {node_revision} r ON r.vid = n.vid INNER JOIN {node__webform} w ON w.entity_id = n.nid WHERE n.type = 'proposal' AND n.status = 1 AND w.webform_status = 'open' AND r.revision_timestamp > :since", [':since' => $since])->fetchField();
    $cache = ['tags' => ['node_list'], 'max-age' => $next ? max(0, (int) $next - $since) : -1];
    if (!$nodes) {
      return ['#cache' => $cache];
    }
    $items = [];
    foreach ($nodes as $node) {
      $items[] = $node->toLink()->toRenderable();
    }
    return $this->view([
      'header' => $this->t("These proposals are nearing their close date. If you haven't voted in them yet, please do! We need your input! Closing proposals:"),
      'content' => ['#theme' => 'item_list', '#items' => $items],
    ], $cache);
  }

}
