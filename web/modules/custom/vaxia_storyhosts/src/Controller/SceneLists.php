<?php

namespace Drupal\vaxia_storyhosts\Controller;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Character;
use Symfony\Component\HttpFoundation\Request;

/**
 * Scene lists (D7 scene_record "scenes" view: character-scenes/mine,
 * character-scenes/evaluate and the node/%/scenes tab).
 */
class SceneLists extends ControllerBase {

  /**
   * Scenes the current user wrote (D7 display "scenes"; also served at
   * /character-scenes, the parent D7 made for the default tab).
   */
  public function mine(Request $request) {
    return $this->scenes($request, 'mine');
  }

  /**
   * Submitted and ruled scenes, for storyhosts (D7 display "scenes_eval").
   */
  public function evaluate(Request $request) {
    return $this->scenes($request, 'evaluate');
  }

  /**
   * Scenes recorded for one character (D7 display "character_scenes").
   */
  public function character(Request $request, NodeInterface $node) {
    return $this->scenes($request, 'character', $node);
  }

  public function characterTitle(NodeInterface $node) {
    return $this->t('Scenes');
  }

  /**
   * node/%/scenes: players, on a character sheet they may edit. (D7 put the
   * tab on every node and answered "not found" off character sheets.)
   */
  public static function characterAccess(NodeInterface $node, AccountInterface $account) {
    return AccessResult::allowedIf($account->hasRole('player') && Character::isSheet($node) && $node->access('update', $account))
      ->cachePerUser()->addCacheableDependency($node);
  }

  private function scenes(Request $request, string $display, ?NodeInterface $character = NULL): array {
    $states = $display === 'mine'
      ? ['scene_evaluation_draft', 'scene_evaluation_submitted', 'scene_evaluation_ruled']
      : ['scene_evaluation_submitted', 'scene_evaluation_ruled'];
    $input = $request->query->all();
    $exposed = array_intersect_key($input, ['sid' => 1, 'field_scene_character_nid' => 1]);
    // As in D7 views: with no exposed input the filter's own values apply; once
    // the form is used, an empty state selection means "any state".
    $selected = $exposed ? array_values(array_intersect((array) ($input['sid'] ?? []), $states)) : $states;
    $character_filter = $display === 'mine' ? (string) ($input['field_scene_character_nid'] ?? '') : '';

    $header = [];
    $header[] = $this->t('Character');
    $header[] = ['data' => $this->t('Title'), 'field' => 'title', 'specifier' => 'title'];
    if ($display === 'evaluate') {
      $header[] = ['data' => $this->t('Name'), 'field' => 'uid.entity.name', 'specifier' => 'uid.entity.name'];
    }
    $header[] = $this->t('Attention To');
    $header[] = ['data' => $this->t('Last updated'), 'field' => 'changed', 'specifier' => 'changed'];
    $header[] = ['data' => $this->t('Current state'), 'field' => 'field_scene_evaluation', 'specifier' => 'field_scene_evaluation'];

    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'scene')->condition('status', 1);
    if ($selected) {
      $query->condition('field_scene_evaluation', $selected, 'IN');
    }
    if ($display === 'mine') {
      $query->condition('uid', $this->currentUser()->id());
      if ($character_filter !== '' && $character_filter !== 'All') {
        $query->condition('field_scene_character', (int) $character_filter);
      }
    }
    if ($character) {
      $query->condition('field_scene_character', $character->id());
    }
    // Click-sorting replaces the default newest-first order.
    if ($request->query->has('order')) {
      $query->tableSort($header);
    }
    else {
      $query->sort('created', 'DESC');
    }
    $query->pager(35);

    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $scene) {
      $links = [];
      foreach ($scene->get('field_scene_character')->referencedEntities() as $referenced) {
        $links[] = $referenced->toLink()->toString();
      }
      $row = [];
      $row[] = ['data' => ['#markup' => implode(', ', $links)]];
      $row[] = $scene->toLink();
      if ($display === 'evaluate') {
        $row[] = $scene->getOwner()->getDisplayName();
      }
      $row[] = (string) $scene->get('field_sh_attention_to')->value;
      // D7's "privatemsg_current_day" format (h:ia T): the time of day only.
      $row[] = \Drupal::service('date.formatter')->format($scene->getChangedTime(), 'custom', 'h:ia T');
      $row[] = Character::stateLabel($scene);
      $rows[] = $row;
    }

    $build = [];
    $build['filters'] = [
      '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get', 'class' => ['views-exposed-form']],
    ];
    $options = [];
    foreach (\Drupal::entityTypeManager()->getStorage('workflow_state')->loadMultiple($states) as $id => $state) {
      $options[$id] = $state->label();
    }
    $build['filters']['sid'] = [
      '#type' => 'select', '#title' => $this->t('Current state'), '#name' => 'sid[]', '#multiple' => TRUE,
      '#options' => $options, '#value' => $selected, '#attributes' => ['multiple' => 'multiple'],
    ];
    if ($display === 'mine') {
      $characters = \Drupal::database()->select('node_field_data', 'n')->fields('n', ['nid', 'title'])
        ->condition('n.type', 'character_sheet')->orderBy('n.title')->addTag('node_access')->execute()->fetchAllKeyed();
      $build['filters']['field_scene_character_nid'] = [
        '#type' => 'select', '#title' => $this->t('Character (field_scene_character)'), '#name' => 'field_scene_character_nid',
        '#options' => ['All' => $this->t('- Any -')] + $characters, '#value' => $character_filter === '' ? 'All' : $character_filter,
      ];
    }
    $build['filters']['submit'] = ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']];
    $build['table'] = ['#type' => 'table', '#header' => $header, '#rows' => $rows, '#empty' => $this->t('No scenes found.')];
    $build['pager'] = ['#type' => 'pager'];
    $build['#cache'] = ['contexts' => ['url.query_args', 'user.node_grants:view'], 'tags' => ['node_list', 'user_list']];
    if ($display === 'mine') {
      $build['#cache']['contexts'][] = 'user';
    }
    if ($character) {
      $build['#cache']['tags'][] = 'node:' . $character->id();
    }
    return $build;
  }

}
