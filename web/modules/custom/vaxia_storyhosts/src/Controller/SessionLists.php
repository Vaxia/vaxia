<?php

namespace Drupal\vaxia_storyhosts\Controller;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\node\NodeInterface;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\Character;
use Symfony\Component\HttpFoundation\Request;

/**
 * Session report lists (D7 "sessions" view: sessions, sessions/all,
 * node/%/sessions, user/%/sessions and the storyhost roll calls).
 */
class SessionLists extends ControllerBase {

  /**
   * Roles of active storyhosts and apprentices (D7 rids 9, 13, 10, 17).
   */
  const STORYHOST_ROLES = ['storyteller', 'storyteller_newbie', 'storyteller_assistant', 'storyteller_candidate'];

  /**
   * All session reports (D7 display "sessions"; /sessions and /sessions/all).
   */
  public function all(Request $request) {
    return $this->sessions($request, 'all');
  }

  /**
   * Sessions a character was in (D7 display "character_sessions").
   */
  public function character(Request $request, NodeInterface $node) {
    return $this->sessions($request, 'character', $node);
  }

  /**
   * node/%/sessions: whoever may edit the character sheet (D7 denied access
   * when the argument failed validation, but showed the tab on every node).
   */
  public static function characterAccess(NodeInterface $node, AccountInterface $account) {
    return AccessResult::allowedIf(Character::isSheet($node) && $node->access('update', $account))
      ->cachePerUser()->addCacheableDependency($node);
  }

  /**
   * Sessions a user ran (D7 display "sh_sessions").
   */
  public function user(Request $request, UserInterface $user) {
    return $this->sessions($request, 'user', NULL, $user);
  }

  private function sessions(Request $request, string $display, ?NodeInterface $character = NULL, ?UserInterface $account = NULL): array {
    $input = $request->query->all();
    $exposed = array_intersect_key($input, ['field_saga_tid' => 1, 'user' => 1, 'field_tag_realm_tid' => 1]);
    $term_storage = $this->entityTypeManager()->getStorage('taxonomy_term');

    // Realm: every realm is offered; with no exposed input the D7 defaults apply.
    $realm_options = [];
    $default_realms = [];
    foreach ($term_storage->loadTree('realm') as $term) {
      $realm_options[$term->tid] = str_repeat('-', $term->depth) . $term->name;
      // The filter starts with every realm (D7 named its three).
      $default_realms[] = (string) $term->tid;
    }
    $realms = $exposed ? array_values(array_intersect(array_map('strval', (array) ($input['field_tag_realm_tid'] ?? [])), array_map('strval', array_keys($realm_options)))) : $default_realms;
    $saga_names = trim((string) ($input['field_saga_tid'] ?? ''));
    $user_names = trim((string) ($input['user'] ?? ''));

    $header = [];
    if ($display !== 'all') {
      $header[] = ['data' => $this->t('SH'), 'field' => 'uid.entity.name', 'specifier' => 'uid.entity.name'];
    }
    $header[] = ['data' => $this->t('Title'), 'field' => 'title', 'specifier' => 'title'];
    if ($display === 'all') {
      $header[] = $this->t('Saga');
      $header[] = $this->t('Realm');
    }
    $header[] = ['data' => $this->t('Session held'), 'field' => 'field_session_held', 'specifier' => 'field_session_held'];
    $header[] = $this->t('Characters');
    $header[] = $this->t('Details');
    $header[] = $this->t('Rate');
    if ($display === 'user') {
      $header[] = $this->t('Session Requirements');
    }

    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'session_report')->condition('status', 1);
    if ($realms) {
      $query->condition('field_tag_realm', $realms, 'IN');
    }
    if ($saga_names !== '') {
      $names = array_filter(array_map('trim', explode(',', $saga_names)), 'strlen');
      $tids = $names ? $term_storage->getQuery()->accessCheck(FALSE)->condition('vid', 'sagas')->condition('name', $names, 'IN')->execute() : [];
      $query->condition('field_saga', $tids ?: [0], 'IN');
    }
    if ($user_names !== '') {
      $names = array_filter(array_map('trim', explode(',', $user_names)), 'strlen');
      $uids = $names ? $this->entityTypeManager()->getStorage('user')->getQuery()->accessCheck(FALSE)->condition('name', $names, 'IN')->execute() : [];
      $query->condition('uid', $uids ?: [-1], 'IN');
    }
    if ($character) {
      $query->condition('field_characters_involved', $character->id());
    }
    if ($account) {
      $query->condition('uid', $account->id());
    }
    if ($request->query->has('order')) {
      $query->tableSort($header);
    }
    else {
      $query->sort('created', 'DESC');
    }
    $query->pager(35);

    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $session) {
      $row = [];
      if ($display !== 'all') {
        $row[] = ['data' => $session->getOwner()->toLink()->toRenderable()];
      }
      $row[] = $session->toLink();
      if ($display === 'all') {
        $saga = $session->get('field_saga')->entity;
        $row[] = $saga ? ['data' => ['#markup' => ' (' . $saga->toLink()->toString() . ')']] : '';
        $realm_names = [];
        foreach ($session->get('field_tag_realm')->referencedEntities() as $realm) {
          $realm_names[] = $realm->label();
        }
        $row[] = implode(', ', $realm_names);
      }
      $held = $session->get('field_session_held')->date;
      // Date-only field in D7's "long" format (l, F j, Y - H:i) minus the time.
      $row[] = $held ? $held->format('l, F j, Y') : '';
      $links = [];
      foreach ($session->get('field_characters_involved')->referencedEntities() as $involved) {
        $links[] = $involved->toLink()->toString();
      }
      $row[] = ['data' => ['#markup' => implode(', ', $links)]];
      $body = $session->get('body');
      $row[] = ['data' => $body->isEmpty() ? '' : ['#type' => 'processed_text', '#text' => $body->value, '#format' => $body->format]];
      $row[] = (string) $session->get('field_rate_session')->value;
      if ($display === 'user') {
        $row[] = vaxia_storyhosts_session_counts($session) ? 'yes' : 'no';
      }
      $rows[] = $row;
    }

    $build = [];
    if ($display === 'user') {
      $build['header'] = [
        '#type' => 'processed_text',
        '#format' => 'wiki',
        '#text' => "Session requirements are used to determine if a session qualifies to keep an SH active or qualifies towards an ASH's progression in the Storyhost course. The requirements for a session are as follows:\n\n* Included two or more PCs that do not belong to the running Storyhost.\n* The session was run within the last 12 months.",
      ];
    }
    $build['filters'] = ['#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get', 'class' => ['views-exposed-form']]];
    $build['filters']['field_saga_tid'] = ['#type' => 'textfield', '#title' => $this->t('Saga'), '#name' => 'field_saga_tid', '#value' => $saga_names];
    $build['filters']['user'] = ['#type' => 'textfield', '#title' => $this->t('A/SH Player'), '#name' => 'user', '#value' => $user_names];
    $build['filters']['field_tag_realm_tid'] = [
      '#type' => 'select', '#title' => $this->t('Realm'), '#name' => 'field_tag_realm_tid[]', '#multiple' => TRUE,
      '#options' => $realm_options, '#value' => $realms, '#attributes' => ['multiple' => 'multiple'],
    ];
    $build['filters']['submit'] = ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']];
    $build['table'] = ['#type' => 'table', '#header' => $header, '#rows' => $rows, '#empty' => $this->t('No sessions found.')];
    $build['pager'] = ['#type' => 'pager'];
    $build['#cache'] = ['contexts' => ['url.query_args', 'user.node_grants:view'], 'tags' => ['node_list', 'user_list', 'taxonomy_term_list']];
    if ($display === 'user') {
      // Whether a session counts depends on its age and the course settings.
      $build['#cache']['max-age'] = 0;
      \Drupal::service('page_cache_kill_switch')->trigger();
    }
    return $build;
  }

  /**
   * Active storyhosts and apprentices with their session counts, then the
   * retired ones (D7 displays "storyhosts" and, as its footer, "retired_shs").
   */
  public function storyhosts() {
    $build = $this->roster(FALSE);
    $build['retired'] = $this->roster(TRUE);
    // "Last online ... ago" is relative to now (anonymous visitors too).
    \Drupal::service('page_cache_kill_switch')->trigger();
    $build['#cache'] = ['max-age' => 0];
    return $build;
  }

  /**
   * Retired storyhosts and apprentices (D7 display "retired_shs").
   */
  public function retired() {
    \Drupal::service('page_cache_kill_switch')->trigger();
    return $this->roster(TRUE) + ['#cache' => ['max-age' => 0]];
  }

  /**
   * One storyhost roll call: one row per author of published session reports
   * who has (or, retired, has none of) the storyhost roles. Most sessions first.
   */
  private function roster(bool $retired): array {
    $database = \Drupal::database();
    $roles = $database->select('user__roles', 'r')->fields('r', ['entity_id'])->condition('r.roles_target_id', self::STORYHOST_ROLES, 'IN');
    $query = $database->select('node_field_data', 'n');
    $query->join('users_field_data', 'u', 'u.uid = n.uid');
    $query->leftJoin('node__field_rate_session', 'rate', 'rate.entity_id = n.nid');
    $query->addField('n', 'uid');
    $query->addExpression('COUNT(DISTINCT n.nid)', 'sessions');
    $query->addExpression('AVG(rate.field_rate_session_value)', 'rating');
    $query->condition('n.type', 'session_report')->condition('n.status', 1)
      ->condition('n.uid', $roles, $retired ? 'NOT IN' : 'IN')
      ->groupBy('n.uid')->orderBy('sessions', 'DESC')->addTag('node_access');
    $results = $query->execute()->fetchAllAssoc('uid');
    $accounts = $this->entityTypeManager()->getStorage('user')->loadMultiple(array_keys($results));
    $formatter = \Drupal::service('date.formatter');

    $rows = [];
    foreach ($results as $uid => $result) {
      $account = $accounts[$uid] ?? NULL;
      if (!$account) {
        continue;
      }
      $lines = [$account->toLink()->toString()];
      $character = $account->get('field_storyhost_character')->entity;
      if ($character) {
        $lines[] = $character->toLink(trim($character->get('field_titles')->value . ' ' . $character->label()))->toString();
      }
      if ($account->getLastAccessedTime()) {
        $lines[] = '<i>' . $this->t('Last online:') . '</i> ' . $this->t('%time ago', ['%time' => $formatter->formatTimeDiffSince($account->getLastAccessedTime(), ['granularity' => 2])]);
      }
      $lines[] = '<i>' . $this->t('Number of sessions:') . '</i> ' . $result->sessions;
      // D7 averaged fivestar votes (percent); ratings are now 1-5 stars.
      $lines[] = $result->rating === NULL ? (string) $this->t('No votes.') : '<i>' . $this->t('Average approval:') . '</i> ' . number_format($result->rating * 20, 2) . '%';
      if ($retired) {
        $since = [(string) $this->t('Retired')];
      }
      else {
        $since = [];
        $became_sh = $account->get('field_became_sh')->date;
        $since[] = $became_sh ? '<i>' . $this->t('Became Storyhost:') . '</i> ' . $became_sh->format('m/d/Y') : (string) $this->t('Apprentice Storyhost');
        $became_evaluator = $account->get('field_became_evaluator')->date;
        if ($became_evaluator) {
          $since[] = '<i>' . $this->t('Became Evaluator:') . '</i> ' . $became_evaluator->format('m/d/Y');
        }
      }
      $rows[] = [
        ['data' => ['#markup' => implode('</br>', $lines)]],
        ['data' => ['#markup' => implode('</br>', $since)]],
      ];
    }

    $header_text = $retired
      ? "</br>\n[b]Our retired storyhosts and apprentices are no longer running here... but their work lives on![/b]\n</br>"
      : '[b]This is a list of our Storyhosts and Apprentices over time. In case you were curious to know who ran all those sessions and keep our settings fresh and alive.[/b]';
    return [
      'header' => ['#type' => 'processed_text', '#text' => $header_text, '#format' => 'wiki'],
      'table' => ['#type' => 'table', '#header' => [$this->t('Name'), $this->t('Since')], '#rows' => $rows, '#attributes' => ['class' => [$retired ? 'vaxia-retired-storyhosts' : 'vaxia-storyhosts']]],
    ];
  }

}
