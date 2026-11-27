<?php

namespace Drupal\vaxia_core\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Database\Query\PagerSelectExtender;
use Drupal\Core\Database\Query\TableSortExtender;
use Drupal\Core\Link;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\Character;
use Drupal\workflow\Entity\WorkflowState;
use Symfony\Component\HttpFoundation\Request;

/**
 * Character sheet lists (D7 views vaxia_character_sheets and
 * administration_characters): all approved characters, your own, a user's
 * characters tab, the evaluation queue and the content admin list.
 */
class CharacterLists extends ControllerBase {

  /**
   * Account ids for a D7 "Player" filter (comma-separated user names), or
   * NULL when a name is unknown: D7 failed the exposed form ("Unable to find
   * user") and listed nothing.
   */
  public static function uidsForNames(string $names): ?array {
    $names = array_filter(array_map('trim', explode(',', $names)), fn($n) => $n !== '');
    $uids = [];
    foreach ($names as $name) {
      $uid = \Drupal::database()->query('SELECT uid FROM {users_field_data} WHERE name = :name', [':name' => $name])->fetchField();
      if ($uid === FALSE) {
        \Drupal::messenger()->addError(t('Unable to find user: @users', ['@users' => $name]));
        return NULL;
      }
      $uids[] = (int) $uid;
    }
    return $uids;
  }

  /**
   * Published-or-not character sheets with their owner (D7's required "uid"
   * relationship), workflow state, NPC flag and "last updated" (newest of the
   * node's change and its last comment).
   */
  private function sheets(array $header, string $sort, string $direction, int $limit) {
    $query = \Drupal::database()->select('node_field_data', 'n')->extend(PagerSelectExtender::class)->extend(TableSortExtender::class);
    $query->join('users_field_data', 'u', 'u.uid = n.uid');
    $query->leftJoin('node__field_character_evaluation', 'w', 'w.entity_id = n.nid');
    $query->leftJoin('node__field_is_npc', 'npc', 'npc.entity_id = n.nid');
    $query->leftJoin('comment_entity_statistics', 's', "s.entity_id = n.nid AND s.entity_type = 'node'");
    $query->fields('n', ['nid']);
    $query->addExpression('GREATEST(n.changed, COALESCE(s.last_comment_timestamp, 0))', 'last_updated');
    $query->condition('n.type', 'character_sheet');
    $query->limit($limit);
    // Table click sorts replace the view's sorts (D7 table "override").
    $default = array_filter($header, fn($h) => is_array($h) && isset($h['sort']));
    if (\Drupal::request()->query->has('order') || $default) {
      $query->orderByHeader($header);
    }
    else {
      $query->orderBy($sort, $direction);
    }
    return $query;
  }

  private function date($timestamp): string {
    return \Drupal::service('date.formatter')->format($timestamp, 'short');
  }

  /**
   * Table rows for the selected nodes, in query order.
   */
  private function rows(array $results, array $columns): array {
    $nodes = $this->entityTypeManager()->getStorage('node')->loadMultiple(array_keys($results));
    $states = WorkflowState::loadMultiple();
    $realms = [];
    if (in_array('realm', $columns, TRUE) && $results) {
      $query = \Drupal::database()->select('node__field_tag_realm', 'r');
      $query->join('taxonomy_term_field_data', 't', 't.tid = r.field_tag_realm_target_id');
      $query->fields('r', ['entity_id'])->fields('t', ['name'])->condition('r.entity_id', array_keys($results), 'IN')->orderBy('r.delta');
      foreach ($query->execute() as $row) {
        $realms[$row->entity_id][] = $row->name;
      }
    }
    $rows = [];
    foreach ($results as $nid => $last_updated) {
      if (!isset($nodes[$nid])) {
        continue;
      }
      $node = $nodes[$nid];
      $owner = $node->getOwner();
      $sid = $node->get('field_character_evaluation')->value;
      $row = [];
      foreach ($columns as $column) {
        switch ($column) {
          case 'title':
            $row[] = Link::createFromRoute($node->label(), 'entity.node.canonical', ['node' => $nid]);
            break;

          case 'player':
            $row[] = $owner ? $owner->toLink($owner->getDisplayName()) : '';
            break;

          case 'realm':
            $row[] = implode(', ', $realms[$nid] ?? []);
            break;

          case 'created':
            $row[] = $this->date($node->getCreatedTime());
            break;

          case 'last_updated':
            $row[] = $this->date($last_updated);
            break;

          case 'state':
            $row[] = isset($states[$sid]) ? $states[$sid]->label() : '';
            break;

          case 'edit':
            $row[] = Link::createFromRoute($this->t('edit'), 'entity.node.edit_form', ['node' => $nid]);
            break;
        }
      }
      $rows[] = $row;
    }
    return $rows;
  }

  /**
   * The D7 exposed form: GET fields over the table.
   */
  private function filters(array $elements): array {
    return [
      '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get', 'class' => ['views-exposed-form']],
      'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit'], '#weight' => 100],
    ] + $elements;
  }

  private function playerField(string $value, string $description = ''): array {
    return ['#type' => 'textfield', '#title' => $this->t('Player'), '#name' => 'uid', '#value' => $value, '#description' => $description, '#description_display' => 'after'];
  }

  private function listCache(array $contexts = []): array {
    return [
      'contexts' => array_merge(['url.query_args', 'user.roles'], $contexts),
      'tags' => ['node_list', 'user_list', 'comment_list', 'config:workflow_state_list'],
    ];
  }

  /**
   * Approved, published player characters of active accounts (character-sheets
   * and its default tab character-sheets/all), oldest first, 35 a page.
   */
  public function all(Request $request) {
    $header = [
      ['data' => $this->t('Name'), 'field' => 'n.title'],
      ['data' => $this->t('Player'), 'field' => 'u.name'],
      ['data' => $this->t('Posted'), 'field' => 'n.created', 'sort' => 'asc'],
      ['data' => $this->t('Last updated'), 'field' => 'last_updated'],
      ['data' => $this->t('Current state'), 'field' => 'w.field_character_evaluation_value'],
    ];
    $query = $this->sheets($header, 'n.created', 'ASC', 35);
    $query->condition('n.status', 1)->condition('u.status', 1)
      ->condition('w.field_character_evaluation_value', 'character_evaluation_approved')
      ->condition('npc.field_is_npc_value', 0);
    $names = trim((string) $request->query->get('uid'));
    $uids = $names === '' ? [] : self::uidsForNames($names);
    if ($uids) {
      $query->condition('n.uid', $uids, 'IN');
    }
    $results = $uids === NULL ? [] : $query->execute()->fetchAllKeyed();
    return [
      'filters' => $this->filters(['uid' => $this->playerField($names, $this->t('Enter the player name to filter results.'))]),
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $this->rows($results, ['title', 'player', 'created', 'last_updated', 'state'])],
      'pager' => ['#type' => 'pager'],
      '#cache' => $uids === NULL ? ['max-age' => 0] : $this->listCache(),
    ];
  }

  /**
   * Your own published, non-NPC characters in draft, submitted, approved or
   * inactive, by name.
   */
  public function mine() {
    $header = [
      ['data' => $this->t('Name'), 'field' => 'n.title'],
      ['data' => $this->t('Player'), 'field' => 'u.name'],
      $this->t('Realm'),
      ['data' => $this->t('Posted'), 'field' => 'n.created'],
      ['data' => $this->t('Last updated'), 'field' => 'last_updated'],
      ['data' => $this->t('Current state'), 'field' => 'w.field_character_evaluation_value'],
    ];
    $query = $this->sheets($header, 'n.title', 'ASC', 35);
    $query->condition('n.uid', (int) $this->currentUser()->id())->condition('n.status', 1)
      ->condition('npc.field_is_npc_value', 0)
      ->condition('w.field_character_evaluation_value', ['character_evaluation_draft', 'character_evaluation_submitted', 'character_evaluation_approved', 'character_evaluation_inactive'], 'IN');
    return [
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $this->rows($query->execute()->fetchAllKeyed(), ['title', 'player', 'realm', 'created', 'last_updated', 'state'])],
      'pager' => ['#type' => 'pager'],
      '#cache' => $this->listCache(['user']),
    ];
  }

  /**
   * A user's Characters tab: their approved non-NPC characters; on your own
   * tab also those in draft, submitted or inactive. Unpublished ones only for
   * the owner and staff (D7 showed them to anyone).
   */
  public function user(UserInterface $user) {
    $header = [$this->t('Name'), $this->t('Player'), $this->t('Realm'), $this->t('Posted'), $this->t('Last updated'), $this->t('Current state')];
    $query = $this->sheets($header, 'n.title', 'ASC', 35);
    $query->condition('n.uid', (int) $user->id())->condition('npc.field_is_npc_value', 1, '<>');
    $states = $query->orConditionGroup()->condition('w.field_character_evaluation_value', 'character_evaluation_approved');
    if ((int) $user->id() === (int) $this->currentUser()->id()) {
      $states->condition('w.field_character_evaluation_value', ['character_evaluation_draft', 'character_evaluation_submitted', 'character_evaluation_inactive'], 'IN');
    }
    $query->condition($states);
    // Security (D7 had no published filter, so anyone saw unpublished sheets):
    // unpublished ones only for the owner and staff.
    if ((int) $user->id() !== (int) $this->currentUser()->id() && !array_intersect(['administrator', 'lead', 'storyteller'], $this->currentUser()->getRoles())) {
      $query->condition('n.status', 1);
    }
    return [
      'header' => ['#type' => 'processed_text', '#text' => 'These are the characters for this user that have been approved for play.', '#format' => 'wiki'],
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $this->rows($query->execute()->fetchAllKeyed(), ['title', 'player', 'realm', 'created', 'last_updated', 'state'])],
      'pager' => ['#type' => 'pager'],
      'footer' => ['#type' => 'processed_text', '#text' => '[url=/character-sheets/mine]Click here to see all of your own character sheets.[/url]', '#format' => 'wiki'],
      '#cache' => $this->listCache(['user', 'route']),
    ];
  }

  /**
   * The evaluation queue: submitted (or inactive) characters of active
   * accounts, most recently updated first.
   */
  public function evaluate(Request $request) {
    $header = [
      ['data' => $this->t('Name'), 'field' => 'n.title'],
      ['data' => $this->t('Player'), 'field' => 'u.name'],
      ['data' => $this->t('Posted'), 'field' => 'n.created'],
      ['data' => $this->t('Last updated'), 'field' => 'last_updated', 'sort' => 'desc'],
      ['data' => $this->t('Current state'), 'field' => 'w.field_character_evaluation_value'],
      $this->t('edit'),
    ];
    $query = $this->sheets($header, 'last_updated', 'DESC', 35);
    $query->condition('n.status', 1)->condition('u.status', 1);
    $allowed = ['character_evaluation_submitted', 'character_evaluation_inactive'];
    $sid = (string) $request->query->get('sid', 'All');
    $query->condition('w.field_character_evaluation_value', in_array($sid, $allowed, TRUE) ? [$sid] : $allowed, 'IN');
    $names = trim((string) $request->query->get('uid'));
    $uids = $names === '' ? [] : self::uidsForNames($names);
    if ($uids) {
      $query->condition('n.uid', $uids, 'IN');
    }
    $results = $uids === NULL ? [] : $query->execute()->fetchAllKeyed();
    $options = ['All' => $this->t('- Any -')];
    foreach (WorkflowState::loadMultiple($allowed) as $id => $state) {
      $options[$id] = $state->label();
    }
    $footer = "Characters marked as Other require approval from the Setting department.\n\nOther char eval considerations:\n\n* Please make sure the character provides a location of origin in its realm, so we know the plots and legal systems that may apply to them. If their current loyalties are different from home-land, please ask for those as well.\n\nThe player has been notified that it may take up to 72 hours before their character begins being addressed by the eval team. It may take much longer still to get the character to a state where it can be approved, especially if it is an Other species or has a Condition, so do not feel pressured to rush into making a decision on a character until you are fully confident that you have addressed any and all concerns with the character details, numbers and skills.\n\nIf a single player submits multiple characters on the same day, bear in mind that the 72-hour window does not overlap for the same account - it's 72 hours per character (so if they submit three characters, it may take 9 days before the third character is even addressed). We recommend evaluating only one character at a time for a given player. Once that character has been completely addressed and either approved or disapproved, then move on to the next.\n\nDon't hesitate to contact the System lead(s) or another evaller for a second opinion of you have questions about anything on the character sheet. Just keep the player posted about concerns with their character or any pending approvals from others, such as the Setting Lead(s).";
    return [
      'filters' => $this->filters([
        'sid' => ['#type' => 'select', '#title' => $this->t('Current state'), '#name' => 'sid', '#options' => $options, '#value' => isset($options[$sid]) ? $sid : 'All'],
        'uid' => $this->playerField($names, $this->t('Enter the player name to filter results.')),
      ]),
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $this->rows($results, ['title', 'player', 'created', 'last_updated', 'state', 'edit'])],
      'pager' => ['#type' => 'pager'],
      'footer' => ['#type' => 'processed_text', '#text' => $footer, '#format' => 'plain_text'],
      '#cache' => $uids === NULL ? ['max-age' => 0] : $this->listCache(),
    ];
  }

  /**
   * Content admin list of every character sheet, newest first, with the D7
   * exposed filters (species, name, player, workflow state, NPC flags).
   */
  public function admin(Request $request) {
    $input = $request->query->all();
    $value = fn($key) => is_scalar($input[$key] ?? NULL) ? trim((string) $input[$key]) : '';
    $definitions = \Drupal::service('entity_field.manager')->getFieldStorageDefinitions('node');
    $any = ['All' => $this->t('- Any -')];
    // A species filter per playable realm (vaxia_core.settings realm_fields).
    $species = [];
    foreach (Character::realmFields() as $realm => $fields) {
      if (isset($definitions[$fields['species']])) {
        $species[$fields['species']] = [$realm, $any + options_allowed_values($definitions[$fields['species']])];
      }
    }
    $workflow = $any;
    foreach (WorkflowState::loadMultiple() as $id => $state) {
      $workflow[$id] = $state->getWorkflow() ? $state->getWorkflow()->label() . ': ' . $state->label() : $state->label();
    }
    $npc = $any + [1 => $this->t('Yes'), 0 => $this->t('No')];

    $query = \Drupal::database()->select('node_field_data', 'n')->extend(PagerSelectExtender::class);
    $query->join('users_field_data', 'u', 'u.uid = n.uid');
    $query->leftJoin('node__field_character_evaluation', 'w', 'w.entity_id = n.nid');
    $query->leftJoin('comment_entity_statistics', 's', "s.entity_id = n.nid AND s.entity_type = 'node'");
    $query->fields('n', ['nid']);
    $query->addExpression('GREATEST(n.changed, COALESCE(s.last_comment_timestamp, 0))', 'last_updated');
    $query->condition('n.type', 'character_sheet')->orderBy('n.created', 'DESC')->limit(35);
    foreach (array_map(fn($filter) => $filter[1], $species) + ['field_is_npc' => $npc, 'field_is_open_npc' => $npc] as $field => $options) {
      $selected = $value($field . '_value');
      if ($selected !== '' && $selected !== 'All' && isset($options[$selected])) {
        $query->join('node__' . $field, $field, $field . '.entity_id = n.nid');
        $query->condition($field . '.' . $field . '_value', $selected);
      }
    }
    if ($value('title') !== '') {
      $query->condition('n.title', $value('title'));
    }
    if ($value('sid') !== '' && $value('sid') !== 'All' && isset($workflow[$value('sid')])) {
      $query->condition('w.field_character_evaluation_value', $value('sid'));
    }
    $uids = $value('uid') === '' ? [] : self::uidsForNames($value('uid'));
    if ($uids) {
      $query->condition('n.uid', $uids, 'IN');
    }
    $results = $uids === NULL ? [] : $query->execute()->fetchAllKeyed();

    $nodes = $this->entityTypeManager()->getStorage('node')->loadMultiple(array_keys($results));
    $states = WorkflowState::loadMultiple();
    $rows = [];
    foreach ($results as $nid => $last_updated) {
      if (!isset($nodes[$nid])) {
        continue;
      }
      $node = $nodes[$nid];
      $owner = $node->getOwner();
      $reviser = $node->getRevisionUser();
      $sid = $node->get('field_character_evaluation')->value;
      $rows[] = [
        $node->toLink(),
        $owner ? $owner->toLink($owner->getDisplayName()) : '',
        $node->isPublished() ? $this->t('Yes') : $this->t('No'),
        $this->date($node->getCreatedTime()),
        $reviser ? $reviser->toLink($reviser->getDisplayName()) : '',
        $this->date($last_updated),
        Link::createFromRoute($this->t('edit'), 'entity.node.edit_form', ['node' => $nid]),
        Link::createFromRoute($this->t('delete'), 'entity.node.delete_form', ['node' => $nid]),
        isset($states[$sid]) ? $states[$sid]->label() : '',
      ];
    }
    $select = fn($key, $title, $options) => ['#type' => 'select', '#title' => $title, '#name' => $key, '#options' => $options, '#value' => isset($options[$value($key)]) ? $value($key) : 'All'];
    return [
      'filters' => $this->filters(array_combine(
        array_map(fn($field) => $field . '_value', array_keys($species)),
        array_map(fn($field, $filter) => $select($field . '_value', $this->t('@realm species', ['@realm' => $filter[0]]), $filter[1]), array_keys($species), $species),
      ) + [
        'title' => ['#type' => 'textfield', '#title' => $this->t('Character'), '#name' => 'title', '#value' => $value('title')],
        'uid' => $this->playerField($value('uid')),
        'sid' => $select('sid', $this->t('Workflow'), $workflow),
        'field_is_npc_value' => $select('field_is_npc_value', $this->t('NPC'), $npc),
        'field_is_open_npc_value' => $select('field_is_open_npc_value', $this->t('Open NPC'), $npc),
      ]),
      'table' => [
        '#type' => 'table',
        '#header' => [$this->t('Title'), $this->t('Author'), $this->t('Published'), $this->t('Post date'), $this->t('Revised by'), $this->t('Updated date'), $this->t('Operations'), $this->t('Delete link'), $this->t('Current state')],
        '#rows' => $rows,
      ],
      'pager' => ['#type' => 'pager'],
      '#cache' => $uids === NULL ? ['max-age' => 0] : $this->listCache(),
    ];
  }

}
