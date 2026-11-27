<?php

/**
 * @file
 * Role applications (D7 role_app): passing a role's quiz files an
 * application; holders of the approving roles vote on it; once enough approve,
 * the applicant gets the role (never storyteller: that goes through the
 * storyhost course).
 */

use Drupal\Core\Serialization\Yaml;
use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\Role;
use Drupal\user\Entity\User;
use Drupal\user\UserInterface;
use Drupal\webform\Entity\Webform;

/**
 * Settings of one application role: quiz (nids), eligible, view, approve.
 */
function vaxia_governance_role_app(string $rid): array {
  $all = (array) \Drupal::config('vaxia_governance.settings')->get('role_app_settings');
  return ($all[$rid] ?? []) + ['quiz' => [], 'eligible' => '', 'view' => [], 'approve' => []];
}

/**
 * The role a quiz applies for, if any.
 */
function vaxia_governance_quiz_role(NodeInterface $quiz): string {
  foreach ((array) \Drupal::config('vaxia_governance.settings')->get('role_app_roles') as $rid) {
    if (in_array((int) $quiz->id(), array_map('intval', vaxia_governance_role_app($rid)['quiz']), TRUE)) {
      return $rid;
    }
  }
  return '';
}

/**
 * Who may approve the applicant (holders of the approving roles, not them).
 */
function vaxia_governance_approvers(int $applicant, string $rid): array {
  $roles = array_values(array_filter(vaxia_governance_role_app($rid)['approve']));
  if (!$roles) {
    return [];
  }
  $uids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', $roles, 'IN')->execute();
  unset($uids[$applicant]);
  return array_diff($uids, [$applicant]);
}

/**
 * Approving votes on an application.
 */
function vaxia_governance_approvals(NodeInterface $node): int {
  $webform = vaxia_governance_webform($node);
  if (!$webform) {
    return 0;
  }
  $ids = \Drupal::entityQuery('webform_submission')->accessCheck(FALSE)->condition('webform_id', $webform->id())->condition('entity_type', 'node')->condition('entity_id', $node->id())->execute();
  $count = 0;
  foreach (\Drupal::entityTypeManager()->getStorage('webform_submission')->loadMultiple($ids) as $submission) {
    $count += $submission->getElementData('approval') === 'approve' ? 1 : 0;
  }
  return $count;
}

/**
 * Percent of possible approvers who approved (0 without approvers; D7
 * divided by zero).
 */
function vaxia_governance_approval_percent(NodeInterface $node): float {
  $approvers = count(vaxia_governance_approvers((int) $node->getOwnerId(), (string) $node->get('field_applied_role_id')->value));
  return $approvers ? vaxia_governance_approvals($node) / $approvers * 100 : 0;
}

/**
 * The applicant's answers to the quiz they passed, for the application body.
 */
function vaxia_governance_quiz_answers(UserInterface $account, NodeInterface $quiz): string {
  $sid = \Drupal::database()->queryRange('SELECT sid FROM {vaxia_quiz_result} WHERE uid = :uid AND nid = :nid ORDER BY rid DESC', 0, 1, [':uid' => $account->id(), ':nid' => $quiz->id()])->fetchField();
  $submission = $sid ? \Drupal::entityTypeManager()->getStorage('webform_submission')->load($sid) : NULL;
  if (!$submission) {
    return '';
  }
  $elements = $submission->getWebform()->getElementsDecodedAndFlattened();
  $output = '';
  foreach ($submission->getData() as $key => $value) {
    $element = $elements[$key] ?? [];
    $answer = is_array($value) ? implode(', ', $value) : (string) ($element['#options'][$value] ?? $value);
    $output .= '<h3><b>' . htmlspecialchars((string) ($element['#title'] ?? $key)) . '</b></h3><p>---</p><p>' . htmlspecialchars($answer) . '</p><hr><br/>';
  }
  return $output;
}

/**
 * Files an application for the quiz's role, if the player may apply.
 */
function vaxia_governance_role_app_quiz_passed(NodeInterface $quiz, UserInterface $account): void {
  $rid = vaxia_governance_quiz_role($quiz);
  if ($rid === '') {
    return;
  }
  $role = Role::load($rid);
  $settings = vaxia_governance_role_app($rid);
  if (!$role || $settings['eligible'] === '' || !$account->hasRole($settings['eligible'])) {
    \Drupal::messenger()->addStatus(t('You are not eligable for @role, the application was not created.', ['@role' => $role ? $role->label() : $rid]));
    return;
  }
  $node = Node::create([
    'type' => 'application',
    'title' => t('@role Application (@name)', ['@role' => ucwords($role->label()), '@name' => $account->getAccountName()]),
    'uid' => $account->id(),
    'status' => 1,
    'promote' => 0,
    'body' => ['value' => vaxia_governance_quiz_answers($account, $quiz), 'format' => 'full_html'],
    'field_applied_role_id' => $rid,
  ]);
  $node->save();
  $webform = Webform::create([
    'id' => 'application_' . $node->id(),
    'title' => mb_substr($node->label(), 0, 255),
    'elements' => Yaml::encode([
      'approval' => ['#type' => 'select', '#title' => 'Approve', '#required' => TRUE, '#options' => [
        'approve' => (string) t('Yes. I approve this candidate for @role.', ['@role' => $role->label()]),
        'disapprove' => (string) t('No. I would like to see some improvements and have included the details below.'),
      ]],
      'comments' => ['#type' => 'textarea', '#title' => 'Comments', '#required' => TRUE],
    ]),
  ]);
  $webform->setSetting('confirmation_message', 'Thank you!');
  $webform->setSetting('confirmation_type', 'message');
  $webform->setSetting('limit_user', 1);
  $rules = $webform->getAccessRules();
  $rules['create']['roles'] = array_values(array_filter($settings['approve']));
  $webform->setAccessRules($rules)->save();
  $node->set('webform', ['target_id' => $webform->id(), 'status' => 'open']);
  $node->save();
}

/**
 * An approving vote may carry the application: role granted, ballot closed.
 */
function vaxia_governance_role_app_vote($submission): void {
  $node = $submission->getSourceEntity();
  if (!$node instanceof NodeInterface || $node->bundle() !== 'application' || !vaxia_governance_is_open($node)) {
    return;
  }
  $rid = (string) $node->get('field_applied_role_id')->value;
  if ($rid === '' || $rid === 'storyteller') {
    return;
  }
  if (vaxia_governance_approval_percent($node) >= (float) \Drupal::config('vaxia_governance.settings')->get('application_approval_min')) {
    $applicant = User::load($node->getOwnerId());
    $applicant->addRole($rid);
    $applicant->save();
    vaxia_governance_set_open($node, FALSE);
    // D7 rule "Vaxia Role App - Check webform submission" (production DB
    // only): tell the approver and mail the applicant.
    $tokens = [
      '[node:author:name]' => $applicant->getAccountName(),
      '[node:title]' => $node->label(),
      '[node:url]' => $node->toUrl('canonical', ['absolute' => TRUE])->toString(),
    ];
    \Drupal::messenger()->addStatus(vaxia_core_rule_text('role_app_vaxia_role_app_check_webform_submission.drupal_message.0', 'message', $tokens));
    vaxia_core_rule_mail('role_app_vaxia_role_app_check_webform_submission.mail.1', $applicant->getEmail(), $tokens);
  }
}

/**
 * Application page: the tally, for the applicant and staff.
 */
function vaxia_governance_role_app_view(array &$build, NodeInterface $node): void {
  if ($node->bundle() !== 'application' || !vaxia_governance_webform($node)) {
    return;
  }
  $build['#cache']['max-age'] = 0;
  $account = \Drupal::currentUser();
  if ((int) $node->getOwnerId() !== (int) $account->id() && !$account->hasPermission('view role application') && !$account->hasPermission('administrate role application')) {
    return;
  }
  $approvers = count(vaxia_governance_approvers((int) $node->getOwnerId(), (string) $node->get('field_applied_role_id')->value));
  $build['application_sums'] = [
    '#markup' => '<div class="application_sums">' . t('@count out of @approvers (@percentage %) have responded to this application.', [
      '@count' => vaxia_governance_approvals($node), '@approvers' => $approvers, '@percentage' => number_format(vaxia_governance_approval_percent($node)),
    ]) . '</div>',
    '#weight' => 1004,
  ];
}
