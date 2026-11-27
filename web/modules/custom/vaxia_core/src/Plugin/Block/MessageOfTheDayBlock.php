<?php

namespace Drupal\vaxia_core\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\node\Entity\Node;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * The body of the "Message of the Day" wiki page (D7 view message_of_the_day,
 * display motd_block).
 */
#[Block(id: 'message_of_the_day-motd_block', admin_label: new TranslatableMarkup('Message of the Day'))]
class MessageOfTheDayBlock extends ViewBlockBase {

  const VIEW = 'message_of_the_day';
  const DISPLAY = 'motd_block';
  const TITLE = 'Message of the Day';

  protected function blockAccess(AccountInterface $account) {
    return AccessResult::allowedIfHasPermission($account, 'access content');
  }

  public function build() {
    // The wiki page holding the message (vaxia_core.settings motd_page).
    $nid = (int) \Drupal::config('vaxia_core.settings')->get('motd_page');
    $cache = ['tags' => ['node:' . $nid, 'node_list', 'config:vaxia_core.settings']];
    $node = $nid ? Node::load($nid) : NULL;
    if (!$node || $node->bundle() !== 'wiki_page' || !$node->isPublished() || !$node->access('view')) {
      return ['#cache' => $cache];
    }
    $body = $node->get('body')->first();
    if (!$body || trim((string) $body->value) === '') {
      return ['#cache' => $cache];
    }
    return $this->view([
      'content' => ['#type' => 'processed_text', '#text' => $body->value, '#format' => $body->format],
    ], $cache);
  }

}
