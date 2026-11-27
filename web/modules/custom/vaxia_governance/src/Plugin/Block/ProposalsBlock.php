<?php

namespace Drupal\vaxia_governance\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Proposals that are open or changed in the last three days, for players (D7
 * view proposals, display proposal_block).
 */
#[Block(id: 'proposals-proposal_block', admin_label: new TranslatableMarkup('Proposals'))]
class ProposalsBlock extends ViewBlockBase {

  const VIEW = 'proposals';
  const DISPLAY = 'proposal_block';
  const TITLE = 'Proposals';

  protected function blockAccess(AccountInterface $account) {
    return $this->roleAccess($account, ['player']);
  }

  public function build() {
    // D7 paged 15 at a time; the block shows the first page.
    [$nodes, $max_age] = $this->openOrRecent('proposal', 3 * 86400, 15);
    $cache = ['tags' => ['node_list'], 'max-age' => $max_age];
    if (!$nodes) {
      return ['#cache' => $cache];
    }
    $items = [];
    foreach ($nodes as $node) {
      $items[] = [
        'title' => $node->toLink()->toRenderable(),
        'status' => ['#markup' => ' ' . $this->webformStatus($node)],
      ];
    }
    return $this->view([
      'content' => ['#theme' => 'item_list', '#items' => $items],
      'footer' => $this->t('These proposals are currently under site consideration or have recently come to a final vote within the last month. See the <a href=":url">proposals</a> page for all proposals.', [':url' => '/proposals']),
    ], $cache);
  }

}
