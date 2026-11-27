<?php

namespace Drupal\vaxia_governance\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Role applications that are open or changed in the last day, for leads and
 * mediators (D7 view applications, block display).
 */
#[Block(id: 'applications-applications', admin_label: new TranslatableMarkup('Applications'))]
class ApplicationsBlock extends ViewBlockBase {

  const VIEW = 'applications';
  const DISPLAY = 'applications';
  const TITLE = 'Applications';

  protected function blockAccess(AccountInterface $account) {
    return $this->roleAccess($account, ['lead', 'mediator']);
  }

  public function build() {
    // D7 paged 5 at a time; the block shows the first page.
    [$nodes, $max_age] = $this->openOrRecent('application', 86400, 5);
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
    return $this->view(['content' => ['#theme' => 'item_list', '#items' => $items]], $cache);
  }

}
