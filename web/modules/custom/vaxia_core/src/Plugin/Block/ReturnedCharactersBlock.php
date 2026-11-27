<?php

namespace Drupal\vaxia_core\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Tells players which of their characters went back to draft in the last
 * three days after having been submitted (D7 view vaxia_character_sheets,
 * display block_notify_ret_chars).
 */
#[Block(id: 'vaxia_character_sheets-block_notify_ret_chars', admin_label: new TranslatableMarkup('Notify Returned Chars'))]
class ReturnedCharactersBlock extends ViewBlockBase {

  const VIEW = 'vaxia_character_sheets';
  const DISPLAY = 'block_notify_ret_chars';
  const TITLE = 'Notify Returned Chars';

  protected function blockAccess(AccountInterface $account) {
    // The view's own roles (the block display didn't override them).
    return $this->roleAccess($account, ['administrator', 'anonymous', 'new_account', 'player']);
  }

  public function build() {
    $now = \Drupal::time()->getRequestTime();
    $cache = ['tags' => ['node_list'], 'contexts' => ['user']];
    // Own published sheets now in draft, put there less than three days ago
    // (D7 workflow_node.stamp), that were submitted at some point (D7 joined
    // every history row and kept those into "submitted": a sheet submitted
    // twice was listed twice; here once).
    $query = \Drupal::database()->select('node_field_data', 'n');
    $query->join('node__field_character_evaluation', 'w', 'w.entity_id = n.nid');
    $query->fields('n', ['nid', 'title']);
    $query->addExpression("(SELECT MAX(d.timestamp) FROM {workflow_transition_history} d WHERE d.entity_type = 'node' AND d.entity_id = n.nid AND d.to_sid = 'character_evaluation_draft')", 'stamp');
    $query->condition('n.type', 'character_sheet')
      ->condition('n.status', 1)
      ->condition('n.uid', \Drupal::currentUser()->id())
      ->condition('w.field_character_evaluation_value', 'character_evaluation_draft')
      ->where("EXISTS (SELECT 1 FROM {workflow_transition_history} s WHERE s.entity_type = 'node' AND s.entity_id = n.nid AND s.to_sid = 'character_evaluation_submitted')")
      ->having('stamp > :since', [':since' => $now - 3 * 86400])
      ->orderBy('n.nid')
      ->addTag('node_access');
    $items = [];
    $max_age = -1;
    foreach ($query->execute() as $row) {
      $items[] = [
        '#type' => 'link',
        '#title' => 'Your character "' . $row->title . '" has been returned to draft for further revision. Please check the "Workflow" tab on the character for details.',
        '#url' => \Drupal\Core\Url::fromRoute('entity.node.canonical', ['node' => $row->nid]),
      ];
      $left = max(0, (int) $row->stamp + 3 * 86400 - $now);
      $max_age = $max_age < 0 ? $left : min($max_age, $left);
    }
    $cache['max-age'] = $max_age;
    if (!$items) {
      return ['#cache' => $cache];
    }
    return $this->view(['content' => [
      '#type' => 'container',
      '#attributes' => ['class' => ['item-list', 'messages', 'status']],
      'list' => ['#theme' => 'item_list', '#items' => $items],
    ]], $cache);
  }

}
