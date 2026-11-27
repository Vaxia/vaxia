<?php

namespace Drupal\vaxia_items\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Database\Query\PagerSelectExtender;
use Drupal\Core\Database\Query\TableSortExtender;
use Drupal\Core\Link;
use Drupal\vaxia_core\Controller\CharacterLists;
use Drupal\workflow\Entity\WorkflowState;
use Symfony\Component\HttpFoundation\Request;

/**
 * Staff item lists (D7 views character_items, item_shop display
 * item_shop_admin, oversight_items).
 */
class ItemLists extends ControllerBase {

  private function date($timestamp): string {
    return \Drupal::service('date.formatter')->format($timestamp, 'short');
  }

  private function filters(array $elements): array {
    return [
      '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get', 'class' => ['views-exposed-form']],
      'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit'], '#weight' => 100],
    ] + $elements;
  }

  private function listCache(): array {
    return ['contexts' => ['url.query_args', 'user.roles'], 'tags' => ['node_list', 'user_list', 'config:workflow_state_list', 'config:vaxia_items.settings', 'taxonomy_term_list']];
  }

  /**
   * Links to an item's owning characters.
   */
  private function owners($item): array {
    $links = [];
    foreach ($item->get('field_character_owner')->referencedEntities() as $character) {
      $links[] = $character->toLink()->toString();
    }
    return ['data' => ['#markup' => implode(', ', $links)]];
  }

  /**
   * Published items in some workflow states with an item-name and creator
   * filter, newest first, 35 a page (D7 character_items).
   */
  private function characterItems(Request $request, string $state, bool $evaluate) {
    $header = [
      ['data' => $this->t('Item'), 'field' => 'n.title'],
      ['data' => $this->t('Character'), 'field' => 'o.field_character_owner_target_id'],
      ['data' => $this->t('Creator'), 'field' => 'u.name'],
      ['data' => $this->t('Created'), 'field' => 'n.created'],
      ['data' => $this->t('Updated'), 'field' => 'n.changed'],
      ['data' => $evaluate ? $this->t('Workflow') : $this->t('Current state'), 'field' => 'w.field_item_evaluation_value'],
    ];
    if ($evaluate) {
      $header[] = $this->t('Operations');
      $header[] = $this->t('Delete');
    }
    $query = \Drupal::database()->select('node_field_data', 'n')->extend(PagerSelectExtender::class)->extend(TableSortExtender::class);
    $query->join('users_field_data', 'u', 'u.uid = n.uid');
    $query->join('node__field_item_evaluation', 'w', 'w.entity_id = n.nid');
    $query->leftJoin('node__field_character_owner', 'o', 'o.entity_id = n.nid AND o.delta = 0');
    $query->fields('n', ['nid'])->condition('n.type', 'items')->condition('n.status', 1)
      ->condition('w.field_item_evaluation_value', $state)->limit(35);
    if ($request->query->has('order')) {
      $query->orderByHeader($header);
    }
    else {
      $query->orderBy('n.created', 'DESC');
    }
    $title = trim((string) $request->query->get('title'));
    if ($title !== '') {
      $query->condition('n.title', '%' . \Drupal::database()->escapeLike($title) . '%', 'LIKE');
    }
    $names = trim((string) $request->query->get('uid'));
    $uids = $names === '' ? [] : CharacterLists::uidsForNames($names);
    if ($uids) {
      $query->condition('n.uid', $uids, 'IN');
    }
    $nids = $uids === NULL ? [] : $query->execute()->fetchCol();
    $nodes = $this->entityTypeManager()->getStorage('node')->loadMultiple($nids);
    $states = WorkflowState::loadMultiple();
    $rows = [];
    foreach ($nids as $nid) {
      $item = $nodes[$nid];
      $creator = $item->getOwner();
      $sid = $item->get('field_item_evaluation')->value;
      $row = [
        $item->toLink(),
        $this->owners($item),
        $creator ? $creator->toLink($creator->getDisplayName()) : '',
        $this->date($item->getCreatedTime()),
        $this->date($item->getChangedTime()),
        isset($states[$sid]) ? $states[$sid]->label() : '',
      ];
      if ($evaluate) {
        $row[] = Link::createFromRoute($this->t('edit'), 'entity.node.edit_form', ['node' => $nid]);
        $row[] = Link::createFromRoute($this->t('delete'), 'entity.node.delete_form', ['node' => $nid]);
      }
      $rows[] = $row;
    }
    return [
      'filters' => $this->filters([
        'title' => ['#type' => 'textfield', '#title' => $this->t('Item Name'), '#name' => 'title', '#value' => $title],
        'uid' => ['#type' => 'textfield', '#title' => $this->t('Creator'), '#name' => 'uid', '#value' => $names],
      ]),
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows],
      'pager' => ['#type' => 'pager'],
      '#cache' => $uids === NULL ? ['max-age' => 0] : $this->listCache(),
    ];
  }

  /**
   * Approved items (character-items and its default tab character-items/all).
   */
  public function all(Request $request) {
    return $this->characterItems($request, 'item_evaluation_approved', FALSE);
  }

  /**
   * Items submitted for evaluation.
   */
  public function evaluate(Request $request) {
    return $this->characterItems($request, 'item_evaluation_submitted', TRUE);
  }

  /**
   * Storytellers' list of everything in the item shop: approved, published
   * items marked "in shop", cheapest first, no pager.
   */
  public function shop(Request $request) {
    $header = [
      ['data' => $this->t('Items'), 'field' => 'n.title'],
      ['data' => $this->t('Type'), 'field' => 't.field_item_type_target_id'],
      ['data' => $this->t('Cost'), 'field' => 'v.field_value_value'],
      $this->t('Realm'),
    ];
    $query = \Drupal::database()->select('node_field_data', 'n')->extend(TableSortExtender::class);
    $query->join('node__field_in_shop', 's', 's.entity_id = n.nid');
    $query->join('node__field_item_evaluation', 'w', 'w.entity_id = n.nid');
    $query->leftJoin('node__field_item_type', 't', 't.entity_id = n.nid AND t.delta = 0');
    $query->leftJoin('node__field_value', 'v', 'v.entity_id = n.nid');
    $query->fields('n', ['nid'])->condition('n.type', 'items')->condition('n.status', 1)
      ->condition('s.field_in_shop_value', 1)->condition('w.field_item_evaluation_value', 'item_evaluation_approved');
    if ($request->query->has('order')) {
      $query->orderByHeader($header);
    }
    else {
      $query->orderBy('v.field_value_value')->orderBy('n.title');
    }
    $title = trim((string) $request->query->get('title'));
    if ($title !== '') {
      $query->condition('n.title', '%' . \Drupal::database()->escapeLike($title) . '%', 'LIKE');
    }
    $terms = $this->entityTypeManager()->getStorage('taxonomy_term');
    $any = ['All' => $this->t('- Any -')];
    $types = $any;
    foreach ($terms->loadTree('item_type') as $term) {
      $types[$term->tid] = $term->name;
    }
    $realms = $any;
    foreach ($terms->loadTree('realm') as $term) {
      $realms[$term->tid] = str_repeat('-', $term->depth) . $term->name;
    }
    $type = (string) $request->query->get('field_item_type_tid', 'All');
    if ($type !== 'All' && isset($types[$type])) {
      $query->join('node__field_item_type', 'ft', 'ft.entity_id = n.nid');
      $query->condition('ft.field_item_type_target_id', $type);
    }
    // Realm filter with depth: a realm takes in its child realms.
    $realm = (string) $request->query->get('field_tag_realm_tid', 'All');
    if ($realm !== 'All' && isset($realms[$realm])) {
      $tids = [(int) $realm];
      foreach ($terms->loadTree('realm', (int) $realm) as $child) {
        $tids[] = (int) $child->tid;
      }
      $query->join('node__field_tag_realm', 'fr', 'fr.entity_id = n.nid');
      $query->condition('fr.field_tag_realm_target_id', $tids, 'IN');
    }
    $nids = array_unique($query->execute()->fetchCol());
    $nodes = $this->entityTypeManager()->getStorage('node')->loadMultiple($nids);
    $rows = [];
    foreach ($nids as $nid) {
      $item = $nodes[$nid];
      $cost = (string) $item->get('field_value')->value;
      $rows[] = [
        $item->toLink(),
        implode(', ', array_map(fn($t) => $t->label(), $item->get('field_item_type')->referencedEntities())),
        // D7 "empty" text, with zero counted as empty.
        $cost === '' || $cost === '0' ? $this->t('Free (Lifestyle)') : $cost,
        implode(', ', array_map(fn($t) => $t->label(), $item->get('field_tag_realm')->referencedEntities())),
      ];
    }
    $select = fn($key, $title, $options, $value) => ['#type' => 'select', '#title' => $title, '#name' => $key, '#options' => $options, '#value' => isset($options[$value]) ? $value : 'All'];
    return [
      'filters' => $this->filters([
        'title' => ['#type' => 'textfield', '#title' => $this->t('Item name'), '#name' => 'title', '#value' => $title],
        'field_item_type_tid' => $select('field_item_type_tid', $this->t('Item type'), $types, $type),
        'field_tag_realm_tid' => $select('field_tag_realm_tid', $this->t('Realm'), $realms, $realm),
      ]),
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows, '#empty' => $this->t('No items found or this character may not be allowed access to the shop at this time.')],
      '#cache' => $this->listCache(),
    ];
  }

  /**
   * Approved (or recharging) items owned by published characters, one row
   * per owning character, by character name, with the oversight test applied
   * to each row (D7 views_php filters), no pager.
   */
  private function oversight(Request $request, callable $keep, string $header_text) {
    $header = [
      ['data' => $this->t('Title'), 'field' => 'n.title'],
      $this->t('Declared value'),
      $this->t('Value'),
      ['data' => $this->t('Player'), 'field' => 'u.name'],
      ['data' => $this->t('Character'), 'field' => 'c.title', 'sort' => 'asc'],
      ['data' => $this->t('Updated date'), 'field' => 'n.changed'],
      ['data' => $this->t('Workflow'), 'field' => 'w.field_item_evaluation_value'],
      $this->t('Edit link'),
    ];
    $query = \Drupal::database()->select('node_field_data', 'n')->extend(TableSortExtender::class);
    $query->join('users_field_data', 'u', 'u.uid = n.uid');
    $query->join('node__field_item_evaluation', 'w', 'w.entity_id = n.nid');
    $query->leftJoin('node__field_character_owner', 'o', 'o.entity_id = n.nid');
    $query->leftJoin('node_field_data', 'c', 'c.nid = o.field_character_owner_target_id');
    $query->fields('n', ['nid'])->fields('c', ['nid', 'title']);
    $query->condition('n.type', 'items')->condition('n.status', 1)->condition('c.status', 1)
      ->condition('w.field_item_evaluation_value', ['item_evaluation_approved', 'item_evaluation_needs_recharge', 'item_evaluation_needs_daily_refresh'], 'IN');
    $query->orderByHeader($header);
    $character = trim((string) $request->query->get('title'));
    if ($character !== '') {
      $query->condition('c.title', $character);
    }
    $names = trim((string) $request->query->get('uid'));
    $uids = $names === '' ? [] : CharacterLists::uidsForNames($names);
    if ($uids) {
      $query->condition('n.uid', $uids, 'IN');
    }
    $results = $uids === NULL ? [] : $query->execute()->fetchAll();
    $nodes = $this->entityTypeManager()->getStorage('node')->loadMultiple(array_unique(array_map(fn($r) => $r->nid, $results)));
    $states = WorkflowState::loadMultiple();
    $rows = [];
    foreach ($results as $result) {
      $item = $nodes[$result->nid];
      if (!$keep($item)) {
        continue;
      }
      $owner = $item->getOwner();
      $sid = $item->get('field_item_evaluation')->value;
      $rows[] = [
        $item->toLink(),
        (string) $item->get('field_declared_value')->value,
        (string) $item->get('field_value')->value,
        $owner ? $owner->toLink($owner->getDisplayName()) : '',
        Link::createFromRoute($result->title, 'entity.node.canonical', ['node' => $result->c_nid]),
        $this->date($item->getChangedTime()),
        isset($states[$sid]) ? $states[$sid]->label() : '',
        Link::createFromRoute($this->t('edit'), 'entity.node.edit_form', ['node' => $result->nid]),
      ];
    }
    return [
      'header' => ['#type' => 'processed_text', '#text' => $header_text, '#format' => 'plain_text'],
      'filters' => $this->filters([
        'title' => ['#type' => 'textfield', '#title' => $this->t('Character'), '#name' => 'title', '#value' => $character],
        'uid' => ['#type' => 'textfield', '#title' => $this->t('Player'), '#name' => 'uid', '#value' => $names],
      ]),
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows],
      '#cache' => $uids === NULL ? ['max-age' => 0] : $this->listCache(),
    ];
  }

  /**
   * Items with a stat modifier at or over the oversight limit (D7
   * items_check_needs_oversight(); its percentage check looked up fields
   * that don't exist, so only the modifiers count).
   */
  public function oversightItems(Request $request) {
    $limit = (float) $this->config('vaxia_items.settings')->get('oversight_limit');
    $keep = function ($item) use ($limit) {
      foreach (['life', 'endurance', 'strength', 'dexterity', 'intelligence', 'spirituality', 'charisma'] as $stat) {
        $field = 'field_item_' . $stat;
        if ($item->hasField($field) && (float) $item->get($field)->value >= $limit) {
          return TRUE;
        }
      }
      return FALSE;
    };
    return $this->oversight($request, $keep, 'These are Items that require additional [color=red]OVERSIGHT[/color]. These Items either have a stat over 7, or a skill over 7, or both. Please keep use of these powerful Items to a reasonable rate. At any point these Items may be subject to further review due to power levels.');
  }

  /**
   * Items declared or valued over 5000. D7 compared the text values as
   * numbers PHP 7 style: "12,000" read as 12, so comma-formatted values
   * never showed.
   */
  public function expensiveItems(Request $request) {
    $keep = fn($item) => (float) $item->get('field_declared_value')->value > 5000 || (float) $item->get('field_value')->value > 5000;
    return $this->oversight($request, $keep, 'These are Items that require additional [color=red]OVERSIGHT[/color]. These Items are extremely valuable and risk economic damage in the game world. At any point these Items may be subject to further review due to power levels.');
  }

}
