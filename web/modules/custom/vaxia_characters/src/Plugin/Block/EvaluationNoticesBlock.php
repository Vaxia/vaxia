<?php

namespace Drupal\vaxia_characters\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\node\Entity\NodeType;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Character sheets, items and scenes waiting for evaluation, newest first,
 * grouped by type (D7 view storyhost_evaluation, block display).
 */
#[Block(id: 'storyhost_evaluation-sh_evaluation', admin_label: new TranslatableMarkup('Evaluation notices'))]
class EvaluationNoticesBlock extends ViewBlockBase {

  const VIEW = 'storyhost_evaluation';
  const DISPLAY = 'sh_evaluation';
  const TITLE = 'Evaluation notices';

  /**
   * Type => [workflow field, its "submitted" state].
   */
  const SUBMITTED = [
    'character_sheet' => ['field_character_evaluation', 'character_evaluation_submitted'],
    'items' => ['field_item_evaluation', 'item_evaluation_submitted'],
    'scene' => ['field_scene_evaluation', 'scene_evaluation_submitted'],
  ];

  protected function blockAccess(AccountInterface $account) {
    return $this->roleAccess($account, ['administrator', 'lead', 'storyteller']);
  }

  public function build() {
    $cache = ['tags' => ['node_list']];
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)
      ->condition('status', 1)
      ->sort('created', 'DESC')
      // D7 showed 10 with a mini pager; the block shows the first page.
      ->range(0, 10);
    $or = $query->orConditionGroup();
    foreach (self::SUBMITTED as $type => [$field, $state]) {
      $or->condition($query->andConditionGroup()->condition('type', $type)->condition($field, $state));
    }
    $nodes = $storage->loadMultiple($query->condition($or)->execute());
    if (!$nodes) {
      return ['#cache' => $cache];
    }
    // Grouped by type, groups in order of their newest node.
    $groups = [];
    foreach ($nodes as $node) {
      $for = $node->hasField('field_sh_attention_to') ? trim((string) $node->get('field_sh_attention_to')->value) : '';
      $groups[$node->bundle()][] = [
        '#type' => 'link',
        '#title' => $node->label() . ' (for ' . ($for === '' ? 'any A/SH' : $for) . ')',
        '#url' => $node->toUrl(),
      ];
    }
    $content = [];
    foreach ($groups as $type => $items) {
      $content[$type] = ['#theme' => 'item_list', '#title' => 'Evaluate ' . NodeType::load($type)->label() . 's', '#items' => $items];
    }
    $cache['tags'][] = 'config:node_type_list';
    return $this->view(['content' => $content], $cache);
  }

}
