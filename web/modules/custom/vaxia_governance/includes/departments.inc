<?php

/**
 * @file
 * Departments (D7 department, vaxia_departments): site departments, each
 * with a role that may vote on proposals; elected leads name a stand-in
 * before a vacation; walkthrough quizzes; member lists.
 */

use Drupal\Core\Form\FormStateInterface;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\user\UserInterface;

/**
 * Departments: machine key => name. Before the list is saved, D7's defaults.
 */
function vaxia_governance_departments(bool $saved_only = FALSE): array {
  $list = (string) \Drupal::config('vaxia_governance.settings')->get('departments');
  if ($list === '') {
    if ($saved_only) {
      return [];
    }
    $list = "Setting\nSystem\nSocial";
  }
  $departments = [];
  foreach (preg_split('/\R/', $list) as $name) {
    $name = trim($name);
    if ($name !== '') {
      $departments[trim(str_replace(' ', '_', strtolower(preg_replace('/[^A-Za-z0-9 ]/', '', $name))))] = $name;
    }
  }
  return $departments;
}

/**
 * Department role ids: department key => role id.
 */
function vaxia_governance_department_roles(): array {
  return array_filter((array) \Drupal::config('vaxia_governance.settings')->get('department_roles'));
}

/**
 * Implements hook_form_FORM_ID_alter() for the election settings: once
 * departments are saved, the positions are their leads (D7 altered a form id
 * that no longer existed, so this never applied).
 */
function vaxia_governance_form__rpg_election_admin_settings_alter(array &$form, FormStateInterface $form_state) {
  $departments = vaxia_governance_departments(TRUE);
  if (!$departments) {
    return;
  }
  $positions = implode("\n", array_map(fn($d) => $d . ' ' . t('Lead'), $departments));
  $form['election_position_details']['election_positions']['#disabled'] = TRUE;
  $form['election_position_details']['election_positions']['#description'] = t('List postitions available on the site per department, one per row.');
  $form['election_position_details']['election_positions']['#default_value'] = $positions;
  $form['election_position_details']['election_positions']['#value'] = $positions;
}

/**
 * Proposal page: which departments may vote.
 */
function vaxia_governance_department_view(array &$build, NodeInterface $node): void {
  if ($node->bundle() !== 'proposal' || !($webform = vaxia_governance_webform($node))) {
    return;
  }
  $voting = $webform->getAccessRules()['create']['roles'] ?? [];
  $names = [];
  foreach (vaxia_governance_department_roles() as $key => $rid) {
    if (in_array($rid, $voting, TRUE) && isset(vaxia_governance_departments()[$key])) {
      $names[] = vaxia_governance_departments()[$key];
    }
  }
  if ($names) {
    $build['department_roles'] = ['#markup' => '<div class="department_roles" style="margin:1em 0;">' . t('Departments that can vote on this are: @list', ['@list' => implode(', ', $names)]) . '</div>', '#weight' => 1002];
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for the ballot builder: voting roles
 * are offered by department.
 */
function vaxia_governance_form_vaxia_governance_ballot_form_alter(array &$form, FormStateInterface $form_state) {
  if (empty($form['role_control']['roles']['#options'])) {
    return;
  }
  $labels = ['voter' => [t('Voters')]];
  foreach (vaxia_governance_department_roles() as $key => $rid) {
    if (isset(vaxia_governance_departments()[$key])) {
      $labels[$rid][] = vaxia_governance_departments()[$key] . ' ' . t('Department');
    }
  }
  $options = [];
  foreach ($form['role_control']['roles']['#options'] as $rid => $label) {
    if (isset($labels[$rid])) {
      $options[$rid] = implode(', ', array_map('strval', $labels[$rid]));
    }
  }
  $form['role_control']['roles']['#options'] = $options;
  $form['role_control']['roles']['#title'] = t('Departments that can vote on this proposal');
  $form['role_control']['disallowed_roles']['#access'] = FALSE;
}

/**
 * Implements hook_vaxia_vacation_form_alter(): an elected lead going away
 * names a stand-in from those who may run.
 */
function vaxia_governance_vaxia_vacation_form_alter(array &$form, FormStateInterface $form_state, array $context) {
  $account = $context['account'];
  $vacation = $context['vacation'];
  $config = \Drupal::config('vaxia_governance.settings');
  $elected = (string) $config->get('election_role');
  if ($vacation || !$elected || !$account->hasRole($elected) || !isset($form['vacation']['vacation_submit'])) {
    return;
  }
  $run = array_values(array_filter((array) $config->get('election_run_roles')));
  $members = [];
  if ($run) {
    $uids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', $run, 'IN')->sort('name')->execute();
    foreach (User::loadMultiple($uids) as $candidate) {
      if ((int) $candidate->id() !== (int) $account->id() && !$candidate->hasRole($elected)) {
        $members[$candidate->id()] = $candidate->getAccountName();
      }
    }
  }
  if (!$members) {
    $form['vacation']['temp_replacement'] = ['#markup' => '<p>' . t('There are no available temporary replacements for your position. You cannot go on vacation at this time.') . '</p>'];
    $form['vacation']['vacation_submit']['#access'] = FALSE;
    return;
  }
  $form['vacation']['temp_replacement'] = [
    '#type' => 'select',
    '#title' => t('Temporary replacement'),
    '#description' => t('Because you hold an elected position on this site, we need to make sure that your position is covered while you are away. Please select your temporary replacement. If you are gone for logner than expected, your temporary replacement may be automatically promoted to take over the position.'),
    '#options' => $members,
    // D7 used the count array_unshift() returns.
    '#default_value' => array_key_first($members),
  ];
  array_unshift($form['vacation']['vacation_submit']['#submit'], 'vaxia_governance_vacation_replacement_submit');
}

/**
 * Promotes the stand-in to the elected role while the lead is away.
 */
function vaxia_governance_vacation_replacement_submit(array &$form, FormStateInterface $form_state) {
  $uid = (int) $form_state->getValue('temp_replacement');
  $lead = $form_state->getFormObject()->getEntity();
  $stand_in = $uid ? User::load($uid) : NULL;
  $role = (string) \Drupal::config('vaxia_governance.settings')->get('election_role');
  if (!$stand_in || !$role) {
    return;
  }
  $stand_ins = \Drupal::state()->get('vaxia_governance.vacation_stand_ins', []);
  $stand_ins[$lead->id()] = $uid;
  \Drupal::state()->set('vaxia_governance.vacation_stand_ins', $stand_ins);
  $stand_in->addRole($role);
  $stand_in->save();
}

/**
 * Implements hook_vaxia_vacation_end(): the stand-in steps down.
 */
function vaxia_governance_vaxia_vacation_end($vacation) {
  $stand_ins = \Drupal::state()->get('vaxia_governance.vacation_stand_ins', []);
  $uid = $stand_ins[$vacation->uid] ?? 0;
  unset($stand_ins[$vacation->uid]);
  \Drupal::state()->set('vaxia_governance.vacation_stand_ins', $stand_ins);
  $role = (string) \Drupal::config('vaxia_governance.settings')->get('election_role');
  if ($uid && $role && ($stand_in = User::load($uid))) {
    $stand_in->removeRole($role);
    $stand_in->save();
  }
}

/**
 * Implements hook_vaxia_quiz_passed(): walkthrough quizzes.
 */
function vaxia_governance_vaxia_quiz_passed(NodeInterface $quiz, UserInterface $account) {
  vaxia_governance_role_app_quiz_passed($quiz, $account);
  $walkthroughs = [
    'system' => "Congratulations! You're up to date on how our system here works. If you have any questions feel free to ask one of our A/SHs for clarification. If there's anything in this walkthrough that we didn't cover please let us know.",
    'setting' => "Congratulations! You're up to date on our settings. If you have any questions feel free to ask one of our A/SHs for clarification. If there's anything in this walkthrough that we didn't cover please let us know.",
    'social' => "Congratulations! You're up to date on our social expectations here. If you have any questions feel free to ask one of our A/SHs for clarification. If there's anything in this walkthrough that we didn't cover please let us know.",
  ];
  $nids = (array) \Drupal::config('vaxia_governance.settings')->get('walkthrough_quizzes');
  foreach ($walkthroughs as $key => $message) {
    if ((int) ($nids[$key] ?? 0) === (int) $quiz->id()) {
      \Drupal::messenger()->addStatus($message);
      $account->set('field_took_' . $key . '_walkthrough', date('Y-m-d'));
      $account->save();
    }
  }
}

/**
 * "Department - Join Social/Setting/System Department" (D7 Rules kept only in
 * the production database): a player who took the department's walkthrough
 * in the last two weeks joins it for a year by submitting its "Join" form
 * (vaxia_governance.settings department_join_forms; D7 had no setting).
 */
function vaxia_governance_department_join($submission): void {
  // department => form node id (vaxia_governance.settings
  // department_join_forms).
  $forms = array_flip(array_filter(array_map('intval', (array) \Drupal::config('vaxia_governance.settings')->get('department_join_forms'))));
  $node = $submission->getSourceEntity();
  $key = $node instanceof NodeInterface ? ($forms[(int) $node->id()] ?? '') : '';
  $account = $key !== '' && $submission->getOwnerId() ? User::load($submission->getOwnerId()) : NULL;
  if (!$account || $account->hasRole('knows_the_' . $key)) {
    return;
  }
  $took = (string) $account->get('field_took_' . $key . '_walkthrough')->value;
  if ($took === '' || strtotime($took) <= strtotime('-2 weeks')) {
    return;
  }
  \Drupal::messenger()->addStatus(t('Welcome to the department! Department members are back-up, advisors, and generally on tap for contributing to department projects - we look forward to seeing your contributions to the site!'));
  $account->addRole('knows_the_' . $key);
  $account->save();
  // D7 role_expire; kept in user data and enforced by cron, like voters.
  \Drupal::service('user.data')->set('vaxia_governance', $account->id(), 'knows_the_' . $key . '_expires', strtotime('+1 year'));
}

/**
 * Department memberships from the "Join" forms end after a year.
 */
function vaxia_governance_department_cron(): void {
  foreach (['social', 'setting', 'system'] as $key) {
    $name = 'knows_the_' . $key . '_expires';
    $expired = \Drupal::database()->query("SELECT uid FROM {users_data} WHERE module = 'vaxia_governance' AND name = :name AND CAST(value AS UNSIGNED) < :now", [':name' => $name, ':now' => \Drupal::time()->getRequestTime()])->fetchCol();
    foreach (User::loadMultiple($expired) as $account) {
      $account->removeRole('knows_the_' . $key);
      $account->save();
      \Drupal::service('user.data')->delete('vaxia_governance', $account->id(), $name);
    }
  }
}
