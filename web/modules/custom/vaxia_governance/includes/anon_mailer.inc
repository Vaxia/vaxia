<?php

/**
 * @file
 * Anonymous mailer (D7 anon_mailer): submissions of the mailer webform go to
 * leads and admins without the sender, and are announced on the forums.
 * D7 decided grievances by pasting submitted text into PHP code (remote code
 * execution by anyone); here the values are only compared.
 */

use Drupal\node\Entity\Node;
use Drupal\user\Entity\User;
use Drupal\vaxia_governance\AnonMailerTexts;

/**
 * Implements hook_ENTITY_TYPE_insert() for webform submissions.
 */
function vaxia_governance_webform_submission_insert($submission) {
  vaxia_governance_role_app_vote($submission);
  vaxia_governance_department_join($submission);
  $source = $submission->getSourceEntity();
  $config = \Drupal::config('vaxia_governance.settings');
  if (!$source || $source->getEntityTypeId() !== 'node' || (int) $source->id() !== (int) $config->get('anon_mailer_webform')) {
    return;
  }
  $elements = $submission->getWebform()->getElementsDecodedAndFlattened();
  // Option labels, as D7's [data:KEY-value] tokens gave them.
  $label = function (string $key) use ($submission, $elements): string {
    $value = $submission->getElementData($key);
    if (is_array($value)) {
      return implode(', ', $value);
    }
    return trim((string) ($elements[$key]['#options'][$value] ?? $value));
  };
  $tokens = [
    '[site:current-date]' => \Drupal::service('date.formatter')->format(\Drupal::time()->getRequestTime(), 'medium'),
    '[data:message-value]' => $label('message'),
    '[data:grievance_violation-value]' => $label('grievance_violation'),
    '[data:associated_files-value]' => $label('associated_files'),
  ];
  $text = fn(string $key, string $part) => strtr(AnonMailerTexts::TEXTS[$key][$part], $tokens);
  $messenger = \Drupal::messenger();

  // "Anon Mailer": forwarded to leads and admins.
  $messenger->addStatus($text('rules_anon_mailer.message', 'message'));
  $ids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', ['lead', 'administrator'], 'IN')->execute();
  foreach (User::loadMultiple($ids) as $account) {
    \Drupal::service('plugin.manager.mail')->mail('vaxia_governance', 'text', $account->getEmail(), 'en', [
      'subject' => $text('rules_anon_mailer.mail', 'subject'),
      'body' => $text('rules_anon_mailer.mail', 'message'),
    ]);
  }

  $grievance = $label('grievance_request') === 'This is a grievance request.';
  $complete = $label('grievance_violation') !== '' && trim((string) $submission->getElementData('grievance_names')) !== '';
  if ($grievance && !$complete) {
    $messenger->addStatus($text('anon_mailer_grievance_posting_incomplete.message', 'message'));
    return;
  }
  $rule = $grievance ? 'rules_grievance_posting' : 'rules_non_grievance_posting';
  $messenger->addStatus($text($rule . '.message', 'message'));
  Node::create([
    'type' => 'forum',
    'title' => $text($rule . '.post', 'subject'),
    'uid' => 1,
    'status' => 1,
    'promote' => 0,
    'body' => ['value' => $text($rule . '.post', 'message'), 'format' => 'filtered_html'],
    'taxonomy_forums' => $config->get('anon_mailer_form') ?: NULL,
  ])->save();
}

/**
 * Implements hook_mail().
 */
function vaxia_governance_mail($key, &$message, $params) {
  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];
}
