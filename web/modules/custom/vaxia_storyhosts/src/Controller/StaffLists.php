<?php

namespace Drupal\vaxia_storyhosts\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Drupal\user\Entity\Role;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\Character;
use Symfony\Component\HttpFoundation\Request;

/**
 * Staff pages from D7 views: user/%/applications (sh_applications),
 * admin/content/storyhost-characters (storyhost_characters) and
 * character-sheets/moderation_log (vaxia_moderation_log).
 */
class StaffLists extends ControllerBase {

  /**
   * D7 string filter operators offered on the moderation log.
   */
  const STRING_OPERATORS = [
    '=' => 'Is equal to',
    '!=' => 'Is not equal to',
    'contains' => 'Contains',
    'starts' => 'Starts with',
    'not_starts' => 'Does not start with',
    'ends' => 'Ends with',
    'not_ends' => 'Does not end with',
    'not' => 'Does not contain',
  ];

  /**
   * D7 date filter operators offered on the moderation log.
   */
  const DATE_OPERATORS = [
    '<' => 'Is less than',
    '<=' => 'Is less than or equal to',
    '=' => 'Is equal to',
    '!=' => 'Is not equal to',
    '>=' => 'Is greater than or equal to',
    '>' => 'Is greater than',
  ];

  /**
   * A user's published applications (D7 sh_applications display "page_1").
   */
  public function applications(Request $request, UserInterface $user) {
    $header = [
      ['data' => $this->t('Application Name'), 'field' => 'title', 'specifier' => 'title'],
      ['data' => $this->t('Application Status'), 'field' => 'webform.status', 'specifier' => 'webform.status'],
      ['data' => $this->t('Post date'), 'field' => 'created', 'specifier' => 'created', 'sort' => 'desc'],
    ];
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)
      ->condition('type', ['application', 'storyhost_application'], 'IN')
      ->condition('status', 1)
      ->condition('uid', $user->id())
      ->tableSort($header)
      ->pager(35);
    $rows = [];
    $formatter = \Drupal::service('date.formatter');
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      $open = $node->hasField('webform') && $node->get('webform')->status === 'open';
      $rows[] = [$node->toLink(), '(' . ($open ? $this->t('Open') : $this->t('Closed')) . ')', $formatter->format($node->getCreatedTime(), 'short')];
    }
    return [
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['contexts' => ['url.query_args', 'user.node_grants:view'], 'tags' => ['node_list']],
    ];
  }

  /**
   * Published storyhost characters (D7 storyhost_characters display "sh_chars").
   */
  public function storyhostCharacters(Request $request) {
    $roles = [];
    foreach (Role::loadMultiple() as $id => $role) {
      if (!in_array($id, ['anonymous', 'authenticated'], TRUE)) {
        $roles[$id] = $role->label();
      }
    }
    $selected = array_values(array_intersect((array) ($request->query->all()['rid'] ?? []), array_keys($roles)));
    $operator = (string) $request->query->get('rid_op', 'or');
    $operators = ['or' => $this->t('Is one of'), 'and' => $this->t('Is all of'), 'not' => $this->t('Is none of')];
    if (!isset($operators[$operator])) {
      $operator = 'or';
    }

    $header = [
      ['data' => $this->t('Player'), 'field' => 'uid.entity.name', 'specifier' => 'uid.entity.name'],
      $this->t('Listed SH Character'),
      $this->t('Roles'),
      ['data' => $this->t('Storyhost Character'), 'field' => 'title', 'specifier' => 'title'],
      $this->t('Edit link'),
      $this->t('Delete link'),
      ['data' => $this->t('Current state'), 'field' => 'field_character_evaluation', 'specifier' => 'field_character_evaluation'],
    ];
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'character_sheet')->condition('status', 1)->condition('field_is_storyteller', 1);
    if ($selected) {
      $with = \Drupal::database()->select('user__roles', 'r')->fields('r', ['entity_id'])->condition('r.roles_target_id', $selected, 'IN')->groupBy('r.entity_id');
      if ($operator === 'and') {
        $with->having('COUNT(DISTINCT r.roles_target_id) = :count', [':count' => count($selected)]);
      }
      $uids = $with->execute()->fetchCol();
      if ($operator === 'not') {
        if ($uids) {
          $query->condition('uid', $uids, 'NOT IN');
        }
      }
      else {
        $query->condition('uid', $uids ?: [-1], 'IN');
      }
    }
    if ($request->query->has('order')) {
      $query->tableSort($header);
    }
    else {
      $query->sort('created', 'DESC');
    }
    $query->pager(35);

    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      $owner = $node->getOwner();
      $listed = $owner->get('field_storyhost_character')->entity;
      $role_labels = [];
      foreach ($owner->getRoles(TRUE) as $rid) {
        if (isset($roles[$rid])) {
          $role_labels[] = $roles[$rid];
        }
      }
      $rows[] = [
        $owner->toLink(),
        $listed ? $listed->toLink() : '',
        implode(', ', $role_labels),
        $node->toLink(trim($node->get('field_titles')->value . ' ' . $node->label())),
        $node->access('update') ? $node->toLink($this->t('edit'), 'edit-form') : '',
        $node->access('delete') ? $node->toLink($this->t('delete'), 'delete-form') : '',
        Character::stateLabel($node),
      ];
    }
    $filters = ['#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get', 'class' => ['views-exposed-form']]];
    $filters['rid_op'] = ['#type' => 'select', '#title' => $this->t('Operator'), '#name' => 'rid_op', '#options' => $operators, '#value' => $operator];
    $filters['rid'] = ['#type' => 'select', '#title' => $this->t('Roles'), '#name' => 'rid[]', '#multiple' => TRUE, '#options' => $roles, '#value' => $selected, '#attributes' => ['multiple' => 'multiple']];
    $filters['submit'] = ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']];
    return [
      'filters' => $filters,
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['contexts' => ['url.query_args', 'user.permissions', 'user.node_grants:view'], 'tags' => ['node_list', 'user_list', 'config:user_role_list']],
    ];
  }

  /**
   * The character sheet change log (D7 vaxia_moderation_log page).
   */
  public function moderationLog(Request $request) {
    $input = $request->query->all();
    $keys = ['type', 'type_op', 'message', 'message_op', 'timestamp', 'timestamp_op', 'title', 'title_op', 'uid', 'uid_1'];
    $exposed = array_intersect_key($input, array_flip($keys));
    $value = function ($key, $default = '') use ($input) {
      return is_scalar($input[$key] ?? NULL) ? trim((string) $input[$key]) : $default;
    };
    // The log type filter starts as "does not contain silver" (D7 default).
    $filters = [
      'type' => [$exposed ? $value('type') : 'silver', $exposed ? $value('type_op', 'not') : 'not'],
      'message' => [$value('message'), $value('message_op', '=')],
      'title' => [$value('title'), $value('title_op', '=')],
    ];

    $database = \Drupal::database();
    $query = $database->select('character_sheet_log', 'l')->extend('Drupal\Core\Database\Query\PagerSelectExtender')->extend('Drupal\Core\Database\Query\TableSortExtender');
    $query->leftJoin('node_field_data', 'n', 'n.nid = l.nid');
    $query->leftJoin('users_field_data', 'owner', 'owner.uid = n.uid');
    $query->leftJoin('users_field_data', 'logged', 'logged.uid = l.uid');
    $query->fields('l', ['lid', 'nid', 'uid', 'type', 'message', 'hostname', 'timestamp']);
    $query->addField('n', 'title');
    $query->addField('owner', 'uid', 'owner_uid');
    $columns = ['type' => 'l.type', 'message' => 'l.message', 'title' => 'n.title'];
    foreach ($filters as $key => [$text, $operator]) {
      if ($text === '' || !isset(self::STRING_OPERATORS[$operator])) {
        continue;
      }
      $like = $database->escapeLike($text);
      $map = [
        '=' => [$text, '='],
        '!=' => [$text, '<>'],
        'contains' => ['%' . $like . '%', 'LIKE'],
        'starts' => [$like . '%', 'LIKE'],
        'not_starts' => [$like . '%', 'NOT LIKE'],
        'ends' => ['%' . $like, 'LIKE'],
        'not_ends' => ['%' . $like, 'NOT LIKE'],
        'not' => ['%' . $like . '%', 'NOT LIKE'],
      ];
      $query->condition($columns[$key], $map[$operator][0], $map[$operator][1]);
    }
    $timestamp = $value('timestamp');
    $timestamp_op = $value('timestamp_op', '=');
    if ($timestamp !== '' && isset(self::DATE_OPERATORS[$timestamp_op]) && strtotime($timestamp) !== FALSE) {
      $query->condition('l.timestamp', strtotime($timestamp), $timestamp_op === '!=' ? '<>' : $timestamp_op);
    }
    foreach (['uid' => 'owner.uid', 'uid_1' => 'l.uid'] as $key => $column) {
      $names = array_filter(array_map('trim', explode(',', $value($key))), 'strlen');
      if ($names) {
        $uids = $this->entityTypeManager()->getStorage('user')->getQuery()->accessCheck(FALSE)->condition('name', $names, 'IN')->execute();
        $query->condition($column, $uids ?: [-1], 'IN');
      }
    }

    $header = [
      ['data' => $this->t('Log ID'), 'field' => 'l.lid'],
      ['data' => $this->t('Log type'), 'field' => 'l.type'],
      ['data' => $this->t('Title'), 'field' => 'n.title'],
      $this->t('Player'),
      $this->t('Message'),
      $this->t('Logged user'),
      ['data' => $this->t('Timestamp'), 'field' => 'l.timestamp'],
      ['data' => $this->t('Hostname'), 'field' => 'l.hostname'],
    ];
    // Security (D7 showed every user's IP address to all players): the
    // Hostname column is for administrators and leads only.
    $show_ip = (bool) array_intersect(['administrator', 'lead'], $this->currentUser()->getRoles());
    if (!$show_ip) {
      array_pop($header);
    }
    if ($request->query->has('order')) {
      $query->orderByHeader($header);
    }
    else {
      $query->orderBy('l.timestamp', 'DESC')->orderBy('l.lid', 'DESC');
    }
    $query->limit(35);
    $results = $query->execute()->fetchAll();

    $uids = [];
    foreach ($results as $row) {
      $uids[] = $row->uid;
      $uids[] = $row->owner_uid;
    }
    $accounts = $this->entityTypeManager()->getStorage('user')->loadMultiple(array_filter(array_unique($uids)));
    $formatter = \Drupal::service('date.formatter');
    $rows = [];
    foreach ($results as $row) {
      $owner = $accounts[$row->owner_uid] ?? NULL;
      $logged = $accounts[$row->uid] ?? NULL;
      $rows[] = [
        $row->lid,
        $row->type,
        $row->title === NULL ? '' : Link::fromTextAndUrl($row->title, Url::fromRoute('entity.node.canonical', ['node' => $row->nid])),
        $owner ? $owner->toLink() : '',
        $row->message,
        $logged ? $logged->toLink() : '',
        $formatter->format($row->timestamp, 'short'),
      ];
      if ($show_ip) {
        $rows[array_key_last($rows)][] = $row->hostname;
      }
    }

    $form = ['#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get', 'class' => ['views-exposed-form']]];
    $string_operators = array_map(fn($label) => $this->t($label), self::STRING_OPERATORS);
    foreach (['type' => $this->t('Log type'), 'message' => $this->t('Message')] as $key => $label) {
      $form[$key . '_op'] = ['#type' => 'select', '#title' => $this->t('Operator'), '#name' => $key . '_op', '#options' => $string_operators, '#value' => $filters[$key][1]];
      $form[$key] = ['#type' => 'textfield', '#title' => $label, '#name' => $key, '#value' => $filters[$key][0]];
    }
    $form['timestamp_op'] = ['#type' => 'select', '#title' => $this->t('Operator'), '#name' => 'timestamp_op', '#options' => array_map(fn($label) => $this->t($label), self::DATE_OPERATORS), '#value' => $timestamp_op];
    $form['timestamp'] = ['#type' => 'textfield', '#title' => $this->t('Timestamp'), '#name' => 'timestamp', '#value' => $timestamp];
    $form['title_op'] = ['#type' => 'select', '#title' => $this->t('Operator'), '#name' => 'title_op', '#options' => $string_operators, '#value' => $filters['title'][1]];
    $form['title'] = ['#type' => 'textfield', '#title' => $this->t('Character'), '#name' => 'title', '#value' => $filters['title'][0]];
    $form['uid'] = ['#type' => 'textfield', '#title' => $this->t('Player'), '#name' => 'uid', '#value' => $value('uid')];
    $form['uid_1'] = ['#type' => 'textfield', '#title' => $this->t('Moderator'), '#name' => 'uid_1', '#value' => $value('uid_1')];
    $form['submit'] = ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']];

    return [
      'filters' => $form,
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows],
      'pager' => ['#type' => 'pager'],
      // The log is a custom table with no cache tags.
      '#cache' => ['max-age' => 0],
    ];
  }

}
