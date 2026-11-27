<?php

/**
 * @file
 * Elections (D7 rpg_election, vaxia_elections): staff elections run as
 * proposal webforms. Candidates hold a "run" role and haven't opted out of
 * the position; winners get the elected role and are recorded per cycle for
 * term limits.
 */

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Serialization\Yaml;
use Drupal\field\Entity\FieldConfig;
use Drupal\field\Entity\FieldStorageConfig;
use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\vaxia_core\Cron;
use Drupal\webform\Entity\Webform;

function vaxia_governance_election_config() {
  return \Drupal::config('vaxia_governance.settings');
}

/**
 * Positions: machine key => name.
 */
function vaxia_governance_positions(): array {
  $positions = [];
  foreach (preg_split('/\R/', (string) vaxia_governance_election_config()->get('election_positions')) as $name) {
    $name = trim($name);
    if ($name !== '') {
      $key = trim(str_replace(' ', '_', strtolower(preg_replace('/[^A-Za-z0-9_ ]/', '', $name))));
      $positions[$key] = $name;
    }
  }
  return $positions;
}

function vaxia_governance_position_seats(string $key): int {
  $seats = vaxia_governance_election_config()->get('election_position_counts') ?: [];
  return (int) ($seats[$key] ?? 2);
}

/**
 * Each position gets a user field "Eligible for POSITION", on by default
 * (also for every active account when the position is new).
 */
function vaxia_governance_position_fields(): void {
  foreach (vaxia_governance_positions() as $key => $name) {
    $field_name = 'field_staff_' . $key;
    if (FieldStorageConfig::loadByName('user', $field_name)) {
      continue;
    }
    FieldStorageConfig::create(['field_name' => $field_name, 'entity_type' => 'user', 'type' => 'boolean'])->save();
    FieldConfig::create([
      'field_name' => $field_name,
      'entity_type' => 'user',
      'bundle' => 'user',
      'label' => 'Eligible for ' . $name,
      'description' => 'I would like to be considered eligible for ' . $name . ' positions during rpg_elections.',
      'default_value' => [['value' => 1]],
    ])->save();
    \Drupal::service('entity_display.repository')->getFormDisplay('user', 'user')->setComponent($field_name, ['type' => 'boolean_checkbox', 'settings' => ['display_label' => TRUE]])->save();
    \Drupal::service('entity_display.repository')->getViewDisplay('user', 'user')->setComponent($field_name, ['type' => 'boolean', 'label' => 'inline'])->save();
    $db = \Drupal::database();
    foreach ($db->query('SELECT uid FROM {users_field_data} WHERE status = 1 AND uid > 0')->fetchCol() as $uid) {
      foreach (['user__' . $field_name, 'user_revision__' . $field_name] as $table) {
        if ($db->schema()->tableExists($table)) {
          $db->merge($table)->keys(['entity_id' => $uid, 'deleted' => 0, 'delta' => 0, 'langcode' => 'en'])->fields(['bundle' => 'user', 'revision_id' => $uid, $field_name . '_value' => 1])->execute();
        }
      }
    }
    \Drupal::entityTypeManager()->getStorage('user')->resetCache();
    \Drupal::messenger()->addStatus(t('Created user field instance: @key', ['@key' => $key]));
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for the account form: only those who
 * may run see the opt-in boxes. (D7 warned on every profile while no run role
 * was set.)
 */
function vaxia_governance_form_user_form_alter(array &$form, FormStateInterface $form_state) {
  $account = $form_state->getFormObject()->getEntity();
  $run = (array) vaxia_governance_election_config()->get('election_run_roles');
  if (array_intersect($run, $account->getRoles())) {
    return;
  }
  foreach (array_keys($form) as $key) {
    if (strpos((string) $key, 'field_staff_') === 0) {
      $form[$key]['#access'] = FALSE;
    }
  }
}

function vaxia_governance_last_cycle(): int {
  return (int) (\Drupal::database()->query('SELECT MAX(cycle) FROM {rpg_election_winners}')->fetchField() ?: 1);
}

function vaxia_governance_election(int $nid): ?object {
  $row = \Drupal::database()->query('SELECT * FROM {rpg_election_nodes} WHERE nid = :nid', [':nid' => $nid])->fetchObject();
  return $row ?: NULL;
}

function vaxia_governance_at_term_limit(int $uid, string $position): bool {
  $config = vaxia_governance_election_config();
  if (!$config->get('election_term_limits')) {
    return FALSE;
  }
  $limit = (int) $config->get('election_term_limit_limits');
  $held = (int) \Drupal::database()->query('SELECT COUNT(uid) FROM {rpg_election_winners} WHERE uid = :uid AND position = :p AND cycle > :c', [':uid' => $uid, ':p' => $position, ':c' => vaxia_governance_last_cycle() - $limit])->fetchField();
  return $held >= $limit;
}

/**
 * Winners of this election's cycle already seated in a position.
 */
function vaxia_governance_current_holders(string $position, int $cycle): array {
  return User::loadMultiple(\Drupal::database()->query('SELECT uid FROM {rpg_election_winners} WHERE position = :p AND cycle = :c', [':p' => $position, ':c' => $cycle])->fetchCol());
}

/**
 * Candidates for a position: run roles, not already elected, opted in, under
 * the term limit. Name => name.
 */
function vaxia_governance_candidates(string $key): array {
  $config = vaxia_governance_election_config();
  $run = array_values(array_filter((array) $config->get('election_run_roles')));
  if (!$run) {
    return [];
  }
  $query = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', $run, 'IN')->condition('field_staff_' . $key, 1)->sort('name');
  $candidates = [];
  foreach (User::loadMultiple($query->execute()) as $account) {
    if ($config->get('election_role') && $account->hasRole($config->get('election_role'))) {
      continue;
    }
    if (!vaxia_governance_at_term_limit((int) $account->id(), $key)) {
      $candidates[$account->getAccountName()] = $account->getAccountName();
    }
  }
  return $candidates;
}

/**
 * Vacates the elected role: the superuser holds it until the election ends.
 */
function vaxia_governance_vacate_seats(): void {
  $role = vaxia_governance_election_config()->get('election_role');
  if (!$role) {
    return;
  }
  foreach (User::loadMultiple(\Drupal::entityQuery('user')->accessCheck(FALSE)->condition('roles', $role)->execute()) as $account) {
    $account->removeRole($role);
    $account->save();
  }
  $admin = User::load(1);
  $admin->addRole($role);
  $admin->save();
}

/**
 * Opens an election: a proposal with a seat-by-seat ballot.
 */
function vaxia_governance_open_election(): ?NodeInterface {
  vaxia_governance_position_fields();
  $config = vaxia_governance_election_config();
  $positions = vaxia_governance_positions();
  $intro = '<h3><b>' . t('Election') . '</b></h3><p>' . $config->get('election_election_intro') . '</p><p>' . t('Available positions are:') . '</p><ul><li>' . implode('</li><li>', array_map('htmlspecialchars', $positions)) . "</li></ul><br/><p>Please take a moment to review the candidates for this year and select those you feel to be the best choices for the available positions.</p><p>Where more than one winner for a position is allowed, select your set of top choices for each position. The winners will be selected from the combined results of all choices for the postion.</p><p>The order of the votes doesn't matter - both candidates you pick for a given department go into the pool of votes evenly. Your A vote doesn't get any more weight than your B vote, so select them in any order you choose.</p>";
  $values = [
    'type' => 'proposal',
    'title' => date('m/d/Y') . ' ' . t('Site Election: @date', ['@date' => date('Y')]),
    'uid' => 1,
    'status' => 1,
    'body' => ['value' => $intro, 'format' => 'full_html'],
    'field_proposal_type' => 'proposal',
  ];
  // D7 saved an empty forum id here and the save failed.
  if ($config->get('election_default_forum')) {
    $values['taxonomy_forums'] = $config->get('election_default_forum');
  }
  $node = Node::create($values);
  $node->save();

  $cycle = vaxia_governance_last_cycle() + 1;
  \Drupal::database()->insert('rpg_election_nodes')->fields([
    'nid' => $node->id(), 'cycle' => $cycle, 'opened' => \Drupal::time()->getRequestTime(), 'closed' => -1,
    'positions' => json_encode($positions), 'winners' => json_encode([]),
  ])->execute();

  $elements = [];
  $ready = FALSE;
  $descs = $config->get('election_position_descs') ?: [];
  foreach ($positions as $key => $name) {
    $candidates = vaxia_governance_candidates($key);
    if (!$candidates) {
      $elements[$key . '_not_position'] = ['#type' => 'webform_markup', '#markup' => (string) t('There are no eligable candidates for @position.', ['@position' => $name])];
      \Drupal::messenger()->addStatus(t('This rpg_election has no candidates for @position.', ['@position' => $name]));
      continue;
    }
    if (trim((string) ($descs[$key] ?? '')) !== '') {
      $elements[$key . '_not_position'] = ['#type' => 'webform_markup', '#markup' => '<h4>' . htmlspecialchars($name) . '</h4>' . trim($descs[$key])];
    }
    $held = array_values(vaxia_governance_current_holders($key, $cycle));
    foreach (array_slice(range('a', 'z'), 0, vaxia_governance_position_seats($key)) as $i => $letter) {
      if ($i < count($held)) {
        $elements[$key . '_' . $letter . '_not_position'] = ['#type' => 'webform_markup', '#markup' => $name . ' ' . strtoupper($letter) . ': ' . t('is currently held by @name.', ['@name' => $held[$i]->getDisplayName()])];
        continue;
      }
      $elements[$key . '_' . $letter] = ['#type' => 'select', '#title' => $name . ' ' . strtoupper($letter), '#options' => $candidates, '#empty_option' => '- None -'];
      $ready = TRUE;
    }
  }
  if ($ready) {
    $elements['additional_comments'] = ['#type' => 'textarea', '#title' => (string) t('Additional Comments')];
  }
  $webform = Webform::create(['id' => 'proposal_' . $node->id(), 'title' => mb_substr($node->label(), 0, 255), 'elements' => Yaml::encode($elements)]);
  $webform->setSetting('confirmation_message', 'Thank you for participating!');
  $webform->setSetting('confirmation_type', 'message');
  $webform->setSetting('limit_user', 1);
  $voters = [];
  foreach (\Drupal\user\Entity\Role::loadMultiple() as $role) {
    if ($role->hasPermission('can vote on rpg_elections')) {
      $voters[] = $role->id();
    }
  }
  $rules = $webform->getAccessRules();
  $rules['create']['roles'] = $voters;
  $webform->setAccessRules($rules)->save();
  $node->set('webform', ['target_id' => $webform->id(), 'status' => $ready ? 'open' : 'closed']);
  if (!$ready) {
    $node->setTitle($node->label() . ' ' . t('(Cancelled)'));
    $node->setUnpublished();
    \Drupal::messenger()->addError(t('This rpg_election has no candidates for ANY position. It has been automatically closed without runoff.'));
  }
  $node->save();
  vaxia_governance_set_eligible($node, $voters);
  if ($ready) {
    \Drupal::messenger()->addStatus(t('A new rpg_election has been created at @link.', ['@link' => $node->toLink()->toString()]));
  }
  return $node;
}

/**
 * Votes per submission: position => uids.
 */
function vaxia_governance_election_results(NodeInterface $node): array {
  $webform = vaxia_governance_webform($node);
  if (!$webform) {
    return [];
  }
  $ids = \Drupal::entityQuery('webform_submission')->accessCheck(FALSE)->condition('webform_id', $webform->id())->condition('entity_type', 'node')->condition('entity_id', $node->id())->execute();
  $results = [];
  foreach (\Drupal::entityTypeManager()->getStorage('webform_submission')->loadMultiple($ids) as $sid => $submission) {
    $results[$sid] = [];
    foreach ($submission->getData() as $key => $name) {
      if ($key === 'additional_comments' || !preg_match('/^(.+)_[a-z]$/', $key, $m) || (string) $name === '') {
        continue;
      }
      $users = \Drupal::entityTypeManager()->getStorage('user')->loadByProperties(['name' => $name]);
      if ($account = reset($users)) {
        $results[$sid][$m[1]][$account->id()] = (int) $account->id();
      }
    }
  }
  return $results;
}

/**
 * Winners per position: most votes first, at least the minimum votes and
 * share, one seat per person. (D7 sorted the vote groups as arrays, not by
 * count.)
 */
function vaxia_governance_election_winners(NodeInterface $node, array $results): array {
  $config = vaxia_governance_election_config();
  $counts = [];
  foreach ($results as $positions) {
    foreach ($positions as $position => $uids) {
      foreach ($uids as $uid) {
        $counts[$position][$uid] = ($counts[$position][$uid] ?? 0) + 1;
      }
    }
  }
  $mins = [];
  if ($config->get('election_min_numb') && (float) $config->get('election_min_numb_val') > 0) {
    $mins[] = (float) $config->get('election_min_numb_val');
  }
  if ($config->get('election_min_perc') && (float) $config->get('election_min_perc_val') > 0) {
    $mins[] = floor(count($results) * (float) $config->get('election_min_perc_val') / 100);
  }
  $min = $mins ? max($mins) : 0;
  $by_count = [];
  foreach ($counts as $position => $uids) {
    foreach ($uids as $uid => $votes) {
      if ($votes >= $min) {
        $by_count[$votes][$position][] = $uid;
      }
    }
  }
  krsort($by_count);
  $election = vaxia_governance_election((int) $node->id());
  $free = [];
  foreach (array_keys(vaxia_governance_positions()) as $position) {
    $free[$position] = vaxia_governance_position_seats($position) - count(vaxia_governance_current_holders($position, (int) $election->cycle));
  }
  asort($free);
  $winners = [];
  $seated = [];
  foreach ($by_count as $level) {
    $won = [];
    foreach ($free as $position => $seats) {
      foreach ($level[$position] ?? [] as $uid) {
        if (count($winners[$position] ?? []) < $seats && !in_array($uid, $seated, TRUE)) {
          $winners[$position][] = $uid;
          $won[] = $uid;
        }
      }
    }
    $seated = array_merge($seated, $won);
  }
  return $winners;
}

/**
 * A closed election seats its winners (D7 did this on the proposal's save).
 */
function vaxia_governance_election_update(NodeInterface $node): void {
  if ($node->bundle() !== 'proposal' || vaxia_governance_is_open($node)) {
    return;
  }
  $election = vaxia_governance_election((int) $node->id());
  if (!$election || (int) $election->closed !== -1) {
    return;
  }
  $role = vaxia_governance_election_config()->get('election_role');
  $winners = vaxia_governance_election_winners($node, vaxia_governance_election_results($node));
  if ($winners && $role) {
    $admin = User::load(1);
    $admin->removeRole($role);
    $admin->save();
    foreach ($winners as $position => $uids) {
      foreach ($uids as $uid) {
        $account = User::load($uid);
        $account->addRole($role);
        $account->save();
        \Drupal::database()->merge('rpg_election_winners')->keys(['uid' => $uid, 'cycle' => $election->cycle, 'position' => $position])->execute();
      }
    }
  }
  \Drupal::database()->update('rpg_election_nodes')->fields(['closed' => \Drupal::time()->getRequestTime(), 'winners' => json_encode($winners)])->condition('eid', $election->eid)->execute();
}

/**
 * Open election proposals.
 */
function vaxia_governance_open_elections(): array {
  $nids = \Drupal::database()->query('SELECT nid FROM {rpg_election_nodes} WHERE closed = -1')->fetchCol();
  return array_filter(Node::loadMultiple($nids), fn($n) => $n->isPublished() && vaxia_governance_is_open($n));
}

/**
 * Daily: on election day (last election plus the election rate), opens the
 * year's election. D7's query failed there, so it never ran.
 */
function vaxia_governance_election_cron(): void {
  if (!Cron::onceADay('election_cron', '00:00:01')) {
    return;
  }
  $config = vaxia_governance_election_config();
  // The last election day is state, not config: a config import would reset
  // it (review, 2026-11-26). A new site has none: its first cron starts the
  // clock, so the first election comes one election rate later.
  $state = \Drupal::state();
  $last = (string) $state->get('vaxia_governance.election_day', '');
  if ($last === '') {
    $state->set('vaxia_governance.election_day', date('m/d/Y'));
    return;
  }
  // On election day, or the first cron after it: cron runs from page visits,
  // so a day without visitors mustn't skip the election for good (review,
  // 2026-11-26).
  $next = strtotime($config->get('election_rate'), strtotime($last));
  if ($next === FALSE || $next > strtotime('today')) {
    return;
  }
  // The day it was due, so a late run doesn't push later elections back.
  $state->set('vaxia_governance.election_day', date('m/d/Y', $next));
  vaxia_governance_vacate_seats();
  vaxia_governance_open_election();
}

/**
 * An ASH 1 who files a session report may approve ASH 3 when storyhosts are
 * scarce (D7 vaxia_elections rule).
 */
function vaxia_governance_session_insert(NodeInterface $node): void {
  if ($node->bundle() !== 'session_report') {
    return;
  }
  $author = User::load($node->getOwnerId());
  if ($author && $author->hasRole('eligible_for_sh') && $author->hasRole('storyteller_newbie') && !$author->hasRole('starvation_mode_ash_approver')) {
    $author->addRole('starvation_mode_ash_approver');
    $author->save();
  }
}
