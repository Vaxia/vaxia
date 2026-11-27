<?php

namespace Drupal\vaxia_core\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\Request;

/**
 * Player pages (D7 players, vaxia_rpg, top_tens, vaxia_players views).
 */
class PlayerPages extends ControllerBase {

  private function date($timestamp): string {
    return $timestamp ? \Drupal::service('date.formatter')->format($timestamp, 'short') : (string) $this->t('Never');
  }

  /**
   * Active players, newest first, 30 a page. Cached for an hour per role set,
   * like D7's view (new players show up when the cache expires).
   */
  public function players(Request $request) {
    $storage = $this->entityTypeManager()->getStorage('user');
    $query = $storage->getQuery()->accessCheck(FALSE)->condition('status', 1)->condition('roles', 'player')->sort('created', 'DESC')->sort('uid', 'DESC')->pager(30);
    $name = trim((string) $request->query->get('name'));
    if ($name !== '') {
      $query->condition('name', $name, $request->query->get('name_op') === 'contains' || !$request->query->get('name_op') ? 'CONTAINS' : '=');
    }
    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $account) {
      $badges = array_map(fn($r) => $r->label(), array_filter(\Drupal\user\Entity\Role::loadMultiple($account->getRoles(TRUE)), fn($r) => !in_array($r->id(), ['player', 'new_account'], TRUE)));
      $rows[] = [$account->toLink(), $this->date($account->getCreatedTime()), $this->date($account->getLastLoginTime()), implode(', ', $badges)];
    }
    return [
      'filter' => [
        '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
        'name' => ['#type' => 'textfield', '#title' => $this->t('Name'), '#name' => 'name', '#value' => $name],
        'op' => ['#type' => 'hidden', '#name' => 'name_op', '#value' => 'contains'],
        'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']],
      ],
      'table' => ['#type' => 'table', '#caption' => $this->t('Players'), '#header' => [$this->t('Name'), $this->t('Created date'), $this->t('Last login'), $this->t('Badges')], '#rows' => $rows, '#empty' => $this->t('No players.')],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 3600, 'contexts' => ['user.roles', 'url.query_args'], 'tags' => ['vaxia_core:player_list']],
    ];
  }

  /**
   * Where players log in from (last IP and its location).
   */
  public function locations() {
    $storage = $this->entityTypeManager()->getStorage('user');
    $rows = [];
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(FALSE)->condition('status', 1)->exists('field_last_ip')->sort('name')->pager(50)->execute()) as $account) {
      $rows[] = [$account->toLink(), (string) $account->get('field_last_ip')->value, (string) $account->get('field_latitude')->value, (string) $account->get('field_longitude')->value];
    }
    return ['table' => ['#type' => 'table', '#header' => [$this->t('Name'), $this->t('Last IP'), $this->t('Latitude'), $this->t('Longitude')], '#rows' => $rows, '#empty' => $this->t('No locations.')], 'pager' => ['#type' => 'pager'], '#cache' => ['max-age' => 0]];
  }

  /**
   * Top ten approved player characters per flavour XP type.
   */
  public function topTens(Request $request) {
    $types = array_filter(array_map('trim', preg_split('/\R/', (string) $this->config('vaxia_core.settings')->get('vaxia_xp_types'))), fn($t) => $t !== '' && !in_array($t, ['transfer', 'site-award'], TRUE));
    $realms = array_filter((array) ($request->query->all()['field_tag_realm_tid'] ?? []));
    // Anonymous visitors see it too; the page cache ignores max-age.
    \Drupal::service('page_cache_kill_switch')->trigger();
    $build = ['#cache' => ['max-age' => 0]];
    foreach ($types as $type) {
      $query = \Drupal::database()->select('vaxia_xp_type_totals', 't');
      $query->join('node_field_data', 'n', 'n.nid = t.nid');
      $query->join('node__field_character_evaluation', 'w', 'w.entity_id = n.nid');
      $query->leftJoin('node__field_is_npc', 'npc', 'npc.entity_id = n.nid');
      $query->leftJoin('node__field_is_storyteller', 'st', 'st.entity_id = n.nid');
      $query->fields('n', ['nid', 'title'])->fields('t', ['xp'])
        ->condition('t.type', $type)->condition('n.status', 1)->condition('w.field_character_evaluation_value', 'character_evaluation_approved')
        ->where('COALESCE(npc.field_is_npc_value, 0) = 0 AND COALESCE(st.field_is_storyteller_value, 0) = 0')
        ->orderBy('t.xp', 'DESC')->range(0, 10);
      if ($realms) {
        $query->join('node__field_tag_realm', 'r', 'r.entity_id = n.nid');
        $query->condition('r.field_tag_realm_target_id', $realms, 'IN');
      }
      $rows = [];
      foreach ($query->execute() as $row) {
        $rows[] = [['data' => ['#type' => 'link', '#title' => $row->title, '#url' => \Drupal\Core\Url::fromRoute('entity.node.canonical', ['node' => $row->nid])], 'class' => ['views-field-title']], ['data' => (string) (float) $row->xp, 'class' => ['views-field-xp']]];
      }
      $build[$type] = ['#type' => 'table', '#caption' => $type, '#header' => [$this->t('Character'), $this->t('XP')], '#rows' => $rows, '#empty' => $this->t('No characters yet.')];
    }
    return $build;
  }

  public function topTensTitle() {
    return $this->t('Top Tens');
  }

  /**
   * Power search over chat posts in public, non-mature, unlocked rooms.
   */
  public function powerSearch(Request $request) {
    $words = trim((string) $request->query->get('comment_body_value'));
    $build['form'] = [
      '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
      'keys' => ['#type' => 'textfield', '#title' => $this->t('Post text'), '#name' => 'comment_body_value', '#value' => $words],
      'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Search'), '#attributes' => ['type' => 'submit']],
    ];
    $rows = [];
    if ($words !== '') {
      $query = \Drupal::database()->select('comment_field_data', 'c');
      $query->join('comment__comment_body', 'b', 'b.entity_id = c.cid');
      $query->join('node_field_data', 'n', "n.nid = c.entity_id AND c.entity_type = 'node'");
      $query->join('node__field_private_room', 'p', 'p.entity_id = n.nid');
      $query->leftJoin('node__field_mature_posting', 'm', 'm.entity_id = n.nid');
      $query->leftJoin('node__field_locked_to_users', 'l', 'l.entity_id = n.nid');
      $query->leftJoin('comment__field_comment_character', 'cc', 'cc.entity_id = c.cid');
      $query->fields('c', ['cid', 'subject', 'uid', 'created'])->fields('n', ['nid', 'title'])->fields('cc', ['field_comment_character_target_id']);
      $query->condition('c.status', 1)->condition('n.status', 1)->condition('p.field_private_room_value', 1)
        ->where('COALESCE(m.field_mature_posting_value, 0) = 0 AND COALESCE(l.field_locked_to_users_value, 0) = 0')
        ->condition('b.comment_body_value', '%' . \Drupal::database()->escapeLike($words) . '%', 'LIKE')
        ->orderBy('c.created', 'DESC')->range(0, 50);
      foreach ($query->execute() as $row) {
        $character = $row->field_comment_character_target_id ? \Drupal\node\Entity\Node::load($row->field_comment_character_target_id) : NULL;
        $author = \Drupal\user\Entity\User::load($row->uid);
        $rows[] = [
          \Drupal\Core\Link::createFromRoute($row->title, 'entity.node.canonical', ['node' => $row->nid]),
          $character ? $character->label() : '',
          $author ? $author->getDisplayName() : '',
          $row->subject,
          $this->date($row->created),
        ];
      }
    }
    $build['table'] = ['#type' => 'table', '#header' => [$this->t('Room'), $this->t('Character'), $this->t('Player'), $this->t('Post'), $this->t('Posted')], '#rows' => $rows, '#empty' => $words === '' ? $this->t('Enter words to search for.') : $this->t('No posts match.')];
    $build['#cache'] = ['max-age' => 0];
    return $build;
  }

  /**
   * Active accounts by last access, with an online flag (D7
   * player_online_time). Online means a session active within the last 30
   * minutes (production's user_block_seconds_online, read by user_stats).
   */
  public function onlineTime(Request $request) {
    $online = (string) $request->query->get('is_online', 'All');
    $names = trim((string) $request->query->get('uid'));
    $since = \Drupal::time()->getRequestTime() - 1800;
    $query = \Drupal::database()->select('users_field_data', 'u');
    $query->addExpression('(SELECT MAX(s.timestamp) FROM {sessions} s WHERE s.uid = u.uid) >= :since', 'is_online', [':since' => $since]);
    $query->fields('u', ['uid'])->condition('u.status', 1)->condition('u.uid', 0, '>')->orderBy('u.access', 'DESC');
    if ($online === '1') {
      $query->where('EXISTS (SELECT 1 FROM {sessions} s WHERE s.uid = u.uid AND s.timestamp >= :since1)', [':since1' => $since]);
    }
    elseif ($online === '0') {
      $query->where('NOT EXISTS (SELECT 1 FROM {sessions} s WHERE s.uid = u.uid AND s.timestamp >= :since0)', [':since0' => $since]);
    }
    if ($names !== '') {
      $query->condition('u.name', array_map('trim', explode(',', $names)), 'IN');
    }
    $flags = $query->extend('Drupal\Core\Database\Query\PagerSelectExtender')->limit(35)->execute()->fetchAllKeyed();
    $formatter = \Drupal::service('date.formatter');
    $now = \Drupal::time()->getRequestTime();
    $ago = fn($t) => $t ? $formatter->formatInterval($now - $t, 2) : '';
    $rows = [];
    foreach ($this->entityTypeManager()->getStorage('user')->loadMultiple(array_keys($flags)) as $account) {
      $rows[] = [
        $account->toLink(),
        $account->isActive() ? $this->t('Yes') : $this->t('No'),
        $flags[$account->id()] ? $this->t('Online') : $this->t('Offline'),
        $ago($account->getCreatedTime()),
        $ago($account->getLastLoginTime()),
        $ago($account->getLastAccessedTime()),
      ];
    }
    $any = ['All' => $this->t('- Any -')];
    return [
      'filter' => [
        '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
        'is_online' => ['#type' => 'select', '#title' => $this->t('Online'), '#name' => 'is_online', '#options' => $any + ['1' => $this->t('Yes'), '0' => $this->t('No')], '#value' => $online],
        'uid' => ['#type' => 'textfield', '#title' => $this->t('Player'), '#name' => 'uid', '#value' => $names],
        'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']],
      ],
      'table' => ['#type' => 'table', '#header' => [$this->t('Name'), $this->t('Active'), $this->t('Is online'), $this->t('Created date'), $this->t('Last login'), $this->t('Last access')], '#rows' => $rows],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

  /**
   * The ten approved player characters with the most spent XP, under an
   * "hxp" heading (D7 total_top_ten; its realm filter pointed at a field
   * production didn't have, so Views dropped it).
   */
  public function totalTopTen() {
    $query = \Drupal::database()->select('character_sheet_xp', 'x');
    $query->join('node_field_data', 'n', 'n.nid = x.nid');
    $query->join('node__field_character_evaluation', 'w', 'w.entity_id = n.nid');
    $query->leftJoin('node__field_is_npc', 'npc', 'npc.entity_id = n.nid');
    $query->leftJoin('node__field_is_open_npc', 'onpc', 'onpc.entity_id = n.nid');
    $query->leftJoin('node__field_is_storyteller', 'st', 'st.entity_id = n.nid');
    $query->fields('n', ['nid'])->fields('x', ['xp'])
      ->condition('x.type', 'spent_xp')->condition('n.type', 'character_sheet')->condition('n.status', 1)
      ->condition('w.field_character_evaluation_value', 'character_evaluation_approved')
      ->where('COALESCE(npc.field_is_npc_value, 0) = 0 AND COALESCE(onpc.field_is_open_npc_value, 0) = 0 AND COALESCE(st.field_is_storyteller_value, 0) = 0')
      ->orderBy('x.xp', 'DESC')->range(0, 10);
    $xp = $query->execute()->fetchAllKeyed();
    $nodes = $this->entityTypeManager()->getStorage('node')->loadMultiple(array_keys($xp));
    $rows = [];
    foreach ($xp as $nid => $amount) {
      $node = $nodes[$nid];
      $image = $node->hasField('field_artwork_image') ? $node->get('field_artwork_image')->entity : NULL;
      $rows[] = [
        ['data' => (string) (float) $amount, 'class' => ['views-field-xp-1']],
        ['data' => $image ? ['#theme' => 'image_style', '#style_name' => 'thumbnail', '#uri' => $image->getFileUri()] : '', 'class' => ['views-field-field-artwork-image']],
        ['data' => $node->toLink(), 'class' => ['views-field-title']],
      ];
    }
    return [
      'hxp' => ['#type' => 'table', '#caption' => ['#markup' => '<h2>hxp</h2>'], '#header' => [$this->t('Amount'), '', $this->t('Character')], '#rows' => $rows],
      '#cache' => ['max-age' => 0],
    ];
  }

}
