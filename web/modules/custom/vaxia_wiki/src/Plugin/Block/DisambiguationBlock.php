<?php

namespace Drupal\vaxia_wiki\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\node\NodeInterface;

/**
 * Other wiki pages with the same title as the one being viewed.
 *
 * @Block(
 *   id = "site_wiki_disamb",
 *   admin_label = @Translation("Site wiki disambiguation")
 * )
 */
class DisambiguationBlock extends BlockBase {

  public function build() {
    $node = \Drupal::routeMatch()->getParameter('node');
    if (!$node instanceof NodeInterface || $node->bundle() !== 'wiki_page') {
      return [];
    }
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'wiki_page')->condition('title', $node->label())->condition('nid', $node->id(), '<>')->execute();
    $items = [];
    foreach ($storage->loadMultiple($ids) as $other) {
      $sample = mb_substr(trim(strip_tags((string) check_markup($other->get('body')->value, $other->get('body')->format))), 0, 255);
      $items[] = ['#markup' => $this->t('Did you mean @link', ['@link' => $other->toLink()->toString()]) . ': <span class="wiki-disamb-sample" style="font-size:80%;">' . htmlspecialchars($sample) . '</span>'];
    }
    if (!$items) {
      return ['#cache' => ['max-age' => 0]];
    }
    return [
      'intro' => ['#markup' => $this->t('Other wiki articles share the same title...')],
      'list' => ['#theme' => 'item_list', '#items' => $items],
      '#cache' => ['max-age' => 0],
    ];
  }

}
