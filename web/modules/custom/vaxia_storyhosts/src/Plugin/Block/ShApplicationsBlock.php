<?php

namespace Drupal\vaxia_storyhosts\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Storyhost applications that are open or changed in the last day, listed by
 * applicant (D7 view sh_applications, block display).
 */
#[Block(id: 'sh_applications-sh_applications', admin_label: new TranslatableMarkup('SH Applications'))]
class ShApplicationsBlock extends ViewBlockBase {

  const VIEW = 'sh_applications';
  const DISPLAY = 'sh_applications';
  const TITLE = 'Applications';

  protected function blockAccess(AccountInterface $account) {
    return AccessResult::allowedIfHasPermission($account, 'view sh test results');
  }

  public function build() {
    // D7 paged 5 at a time; the block shows the first page.
    [$nodes, $max_age] = $this->openOrRecent('storyhost_application', 86400, 5);
    $cache = ['tags' => ['node_list', 'user_list'], 'max-age' => $max_age];
    if (!$nodes) {
      return ['#cache' => $cache];
    }
    $items = [];
    foreach ($nodes as $node) {
      // The link text is the applicant's name (D7 rewrote the title as it).
      $items[] = [
        'title' => ['#type' => 'link', '#title' => $node->getOwner()->getAccountName(), '#url' => $node->toUrl()],
        'status' => ['#markup' => ' ' . $this->webformStatus($node)],
      ];
    }
    return $this->view(['content' => ['#theme' => 'item_list', '#items' => $items]], $cache);
  }

}
