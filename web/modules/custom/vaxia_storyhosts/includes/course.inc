<?php

/**
 * @file
 * The storyhost course (D7 sh_test, storyhost_renewal): ASH tests move players
 * through ASH 1 and ASH 2, sessions qualify or not, ASHs' players are asked
 * for feedback, storyhost powers expire after a year and are renewed by
 * retaking the tests.
 */

use Drupal\node\NodeInterface;
use Drupal\user\Entity\Role;
use Drupal\user\Entity\User;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\Character;
use Drupal\vaxia_storyhosts\CourseTexts;

/**
 * A course text with its tokens filled in.
 */
function vaxia_storyhosts_text(string $key, string $part, array $tokens = []): string {
  return strtr(CourseTexts::TEXTS[$key][$part], $tokens);
}

/**
 * Mails a course text to an account.
 */
function vaxia_storyhosts_mail_text(string $key, string $to, array $tokens = []): void {
  \Drupal::service('plugin.manager.mail')->mail('vaxia_storyhosts', 'text', $to, 'en', [
    'subject' => vaxia_storyhosts_text($key, 'subject', $tokens),
    'body' => vaxia_storyhosts_text($key, 'message', $tokens),
  ]);
}

/**
 * Mails a course text to every active user with any of the roles.
 */
function vaxia_storyhosts_mail_text_to_roles(string $key, array $roles, array $tokens = []): void {
  $ids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', $roles, 'IN')->execute();
  foreach (\Drupal\user\Entity\User::loadMultiple($ids) as $account) {
    vaxia_storyhosts_mail_text($key, $account->getEmail(), $tokens);
  }
}

/**
 * The production quiz nodes of the course.
 */
function vaxia_storyhosts_course_quiz(string $which): int {
  return (int) \Drupal::config('vaxia_storyhosts.settings')->get('sh_test_quiz_' . $which);
}

/**
 * Whether the user has run at least $ran qualifying sessions.
 */
function vaxia_storyhosts_sessions_ran(int $uid, int $ran): bool {
  $storage = \Drupal::entityTypeManager()->getStorage('node');
  $ids = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'session_report')->condition('uid', $uid)->execute();
  return count(array_filter($storage->loadMultiple($ids), 'vaxia_storyhosts_session_counts')) >= $ran;
}

/**
 * Implements hook_vaxia_quiz_passed(): the course's Rules on passing a quiz.
 */
function vaxia_storyhosts_vaxia_quiz_passed(NodeInterface $quiz, UserInterface $account) {
  $nid = (int) $quiz->id();
  $today = date('Y-m-d');
  $messenger = \Drupal::messenger();
  $tokens = ['[user:name]' => $account->getDisplayName()];
  $character = $account->get('field_storyhost_character')->entity;
  $any = fn(array $roles) => (bool) array_intersect($roles, $account->getRoles());

  // "Record ASH 1/2/Eval pass".
  if ($nid === vaxia_storyhosts_course_quiz('ash1')) {
    $account->set('field_took_ash1', $today);
    $messenger->addStatus(vaxia_storyhosts_text('rules_sh_test_1_mark_pass.drupal_message.0', 'message'));
  }
  if (in_array($nid, [vaxia_storyhosts_course_quiz('ash2'), vaxia_storyhosts_course_quiz('ash2_renewal')], TRUE)) {
    $account->set('field_took_ash2', $today);
    $messenger->addStatus(vaxia_storyhosts_text('rules_sh_test_2_mark_pass.drupal_message.0', 'message'));
  }
  if ($nid === vaxia_storyhosts_course_quiz('eval')) {
    $account->set('field_took_eval_test', $today);
    $messenger->addStatus(vaxia_storyhosts_text('sh_test_rules_sh_test_4_mark_pass.drupal_message.0', 'message'));
  }

  // "Promote to ASH 1".
  if ($nid === vaxia_storyhosts_course_quiz('ash1') && $account->hasRole('player') && $account->hasRole('eligible_for_sh')
    && !$any(['storyteller_newbie', 'storyteller_assistant', 'storyteller_candidate', 'storyteller'])) {
    $account->addRole('storyteller_newbie');
    vaxia_storyhosts_mail_text_to_roles('rules_sh_test_1_response.mail_to_users_of_role.0', ['storyteller', 'administrator'], $tokens);
    vaxia_storyhosts_mail_text('rules_sh_test_1_response.mail.1', $account->getEmail(), $tokens);
    if ($character) {
      Character::forceState($character, 'character_evaluation_approved', 'Set Storyhost character - if exists - to approved on passing ASH 1');
    }
  }

  // "Promote to ASH 2": an ASH 1 with three qualifying sessions.
  if ($nid === vaxia_storyhosts_course_quiz('ash2') && $account->hasRole('player') && $account->hasRole('storyteller_newbie') && $account->hasRole('eligible_for_sh')
    && !$any(['storyteller_assistant', 'storyteller_candidate', 'storyteller']) && vaxia_storyhosts_sessions_ran((int) $account->id(), 3)) {
    $account->removeRole('storyteller_newbie');
    $account->addRole('storyteller_assistant');
    vaxia_storyhosts_mail_text_to_roles('rules_sh_test_2_response.mail_to_users_of_role.0', ['storyteller', 'administrator'], $tokens);
    vaxia_storyhosts_mail_text('rules_sh_test_2_response.mail.1', $account->getEmail(), $tokens);
    vaxia_storyhosts_mail_text_to_roles('rules_sh_test_2_response.mail_to_users_of_role.2', ['administrator', 'storyteller', 'lead', 'storyteller_assistant', 'storyteller_candidate'], $tokens);
  }

  // "SH Renewal - Renew Storyhost": both tests passed within the last month.
  $month_ago = date('Y-m-d', strtotime('-1 month'));
  if (in_array($nid, [vaxia_storyhosts_course_quiz('ash1'), vaxia_storyhosts_course_quiz('ash2_renewal')], TRUE)
    && $any(['storyteller_expired', 'storyteller']) && $character && $character->bundle() === 'character_sheet'
    && (string) $account->get('field_took_ash1')->value > $month_ago && (string) $account->get('field_took_ash2')->value > $month_ago) {
    $account->set('field_powers_expire', date('Y-m-d', strtotime('+3 months')));
    $character->set('field_titles', 'SH');
    $character->save();
    $account->removeRole('storyteller_expired');
    $account->addRole('storyteller');
    vaxia_storyhosts_mail_text('rules_sh_renewal.mail.0', $account->getEmail(), $tokens);
    vaxia_storyhosts_mail_text_to_roles('rules_sh_renewal.mail_to_users_of_role.1', ['storyteller'], $tokens);
  }
  $account->save();
}

/**
 * Daily: storyhosts who passed ASH 1 or 2 over a year ago expire (D7 "SH
 * Renewal - Expire Storyhost", whose event never delivered a user).
 */
function vaxia_storyhosts_course_cron(): void {
  $year_ago = date('Y-m-d', strtotime('-1 year'));
  $storage = \Drupal::entityTypeManager()->getStorage('user');
  $query = $storage->getQuery()->accessCheck(FALSE)->condition('roles', 'storyteller')->exists('field_storyhost_character');
  $or = $query->orConditionGroup()->condition('field_took_ash1', $year_ago, '<')->condition('field_took_ash2', $year_ago, '<');
  foreach ($storage->loadMultiple($query->condition($or)->execute()) as $account) {
    $character = $account->get('field_storyhost_character')->entity;
    if (!$character || $character->bundle() !== 'character_sheet') {
      continue;
    }
    $character->set('field_titles', 'SH (retired)');
    $character->save();
    $account->set('field_powers_expire', date('Y-m-d'));
    $account->removeRole('storyteller');
    $account->addRole('storyteller_expired');
    $account->save();
    $tokens = ['[user:name]' => $account->getDisplayName()];
    vaxia_storyhosts_mail_text('rules_sh_general_expire.mail.0', $account->getEmail(), $tokens);
    vaxia_storyhosts_mail_text_to_roles('rules_sh_general_expire.mail_to_users_of_role.1', ['storyteller', 'lead', 'administrator'], $tokens);
  }
}

/**
 * Course notes on quiz pages and session reports; storyhost applications
 * hide their ballot from the applicant.
 */
function vaxia_storyhosts_course_view(array &$build, NodeInterface $node, string $view_mode = 'full'): void {
  $account = \Drupal::currentUser();
  $messenger = \Drupal::messenger();
  // The quiz's own page only: as teasers (the /node list, search) the notices
  // showed on whatever page listed the quiz (user, 2026-11-25).
  if ($node->bundle() === 'quiz' && $view_mode === 'full') {
    $nid = (int) $node->id();
    $eligible = in_array('eligible_for_sh', $account->getRoles(), TRUE);
    $course = array_map('vaxia_storyhosts_course_quiz', ['ash1', 'ash2', 'eval', 'ash3']);
    if (in_array($nid, $course, TRUE) && !$eligible) {
      $messenger->addStatus(t("You are not yet eligible to become a Storyhost. We want to make sure that potential Storyhosts have experience with the community and the system before they start running sessions. We measure that by keeping track of how much XP you've earned across all of your characters. You need 40 XP to become eligible to become a SH. You may still take this quiz as a practice run."));
    }
    // D7 warned those who did qualify and not those who didn't.
    if ($nid === vaxia_storyhosts_course_quiz('ash2') && $eligible
      && (!in_array('storyteller_newbie', $account->getRoles(), TRUE) || !vaxia_storyhosts_sessions_ran((int) $account->id(), 3))) {
      $messenger->addStatus(t('You are not eligible to pass ASH Test 2. You either are not currently in the ASH program as an ASH 1, or you have not run the required number of sessions yet. You may still take this quiz as a practice run.'));
    }
    if ($nid === vaxia_storyhosts_course_quiz('ash3') && $eligible
      && (!in_array('storyteller_assistant', $account->getRoles(), TRUE) || !vaxia_storyhosts_sessions_ran((int) $account->id(), 8))) {
      $messenger->addStatus(t('You are not eligible to pass ASH Test 3. You either are not currently in the ASH program as an ASH 2, or you have not run the required number of sessions yet. Because this quiz is a long format essay, you may begin answering questions at any time leading up to your qualification - so long as you qualify before you submit your last answers then the submission will be accepted. You may still take this quiz as a practice run.'));
    }
    $build['#cache']['max-age'] = 0;
  }
  if ($node->bundle() === 'session_report' && $account->hasPermission('view session qualify')) {
    $config = \Drupal::config('vaxia_storyhosts.settings');
    $message = t('This session report %display count towards session requirements on site.', ['%display' => vaxia_storyhosts_session_counts($node) ? 'does' : 'does NOT']);
    $requirements = [];
    if ($config->get('sh_test_session_party_size')) {
      $requirements[] = t('At least %size_min in the party.', ['%size_min' => $config->get('sh_test_session_party_size_min')]);
      $requirements[] = t('NPCs %npcs count.', ['%npcs' => $config->get('sh_test_session_party_size_npcs') ? 'DO' : 'do not']);
      $requirements[] = t('A/SH accounts %ashs count.', ['%ashs' => $config->get('sh_test_session_party_size_ashs') ? 'DO' : 'do not']);
      $requirements[] = t("The A/SH's own PCs %own_pcs count.", ['%own_pcs' => $config->get('sh_test_session_party_size_own_pcs') ? 'DO' : 'do not']);
    }
    if ($config->get('sh_test_session_expires')) {
      $requirements[] = t('The session was run within the last %timeframe.', ['%timeframe' => $config->get('sh_test_session_expires_timeframe')]);
    }
    $build['session_report_state'] = [
      '#markup' => '<div class="session_report_state">' . $message . ' ' . implode(' ', array_map('strval', $requirements)) . '</div>',
      '#weight' => 1003,
    ];
    $build['#cache']['max-age'] = 0;
  }
  if ($node->bundle() === 'storyhost_application' && (int) $node->getOwnerId() === (int) $account->id()) {
    // Applicants don't vote on their own application.
    unset($build['webform']);
    $build['#cache']['max-age'] = 0;
  }
}

/**
 * A new session report by an ASH asks its players for feedback.
 */
function vaxia_storyhosts_course_session_insert(NodeInterface $node): void {
  if ($node->bundle() !== 'session_report') {
    return;
  }
  $author = $node->getOwner();
  if (!$author || !array_intersect(['storyteller_newbie', 'storyteller_assistant', 'storyteller_candidate'], $author->getRoles())) {
    return;
  }
  $tokens = [
    '[node:author:name]' => $author->getDisplayName(),
    '[node:title]' => $node->label(),
    '[node:url]' => $node->toUrl('canonical', ['absolute' => TRUE])->toString(),
  ];
  foreach ($node->get('field_characters_involved')->referencedEntities() as $character) {
    vaxia_storyhosts_mail_text('sh_test_get_session_feedback_for_ash.mail.0', $character->getOwner()->getEmail(), $tokens);
  }
}

/**
 * Whether enough storyhosts approved an ASH 3 application (D7
 * sh_test_check_ash_3_approvals()). Below the minimum number of holders of
 * the approving roles the course is in starvation mode: the starvation
 * approvers decide, against a lower bar.
 */
function vaxia_storyhosts_ash3_approved(NodeInterface $node): bool {
  $config = \Drupal::config('vaxia_storyhosts.settings');
  $holders = function (string $permission): array {
    $roles = array_keys(array_filter(Role::loadMultiple(), fn($role) => $role->hasPermission($permission)));
    return $roles ? \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', $roles, 'IN')->execute() : [];
  };
  $starvation = count($holders('approve sh test')) < (int) $config->get('sh_test_starvation_mode');
  // The applicant doesn't count.
  $approvers = array_diff($holders($starvation ? 'approve sh starvation' : 'approve sh test'), [$node->getOwnerId()]);
  // D7 divided by the approver count even when nobody held an approving role.
  if (!$approvers) {
    return FALSE;
  }
  $percent = vaxia_governance_approvals($node) / count($approvers) * 100;
  return $percent > (float) $config->get($starvation ? 'sh_test_approval_min_starvation' : 'sh_test_approval_min');
}

/**
 * The course's webform Rules (D7 sh_test, run on every submission).
 */
function vaxia_storyhosts_course_submission($submission): void {
  $node = $submission->getSourceEntity();
  if (!$node instanceof NodeInterface) {
    return;
  }
  $submitter = $submission->getOwner();
  $roles = $submitter ? $submitter->getRoles() : [];
  // D7 mailed the submission's own values ([data:KEY-value]).
  $tokens = [
    '[site:current-date]' => \Drupal::service('date.formatter')->format(\Drupal::time()->getRequestTime(), 'medium'),
    '[data:storyhost-value]' => trim((string) $submission->getElementData('storyhost')),
    '[data:feedback-value]' => (string) $submission->getElementData('feedback'),
  ];

  // "SH Test - Send Feedback on Performance to ASH": the Storyhost Feedback
  // form (setting storyhost_feedback_form; D7 hard-coded its node), filled in
  // by staff, mailed to the storyhost it names and to the leads.
  $settings = \Drupal::config('vaxia_storyhosts.settings');
  if ((int) $node->id() === (int) $settings->get('storyhost_feedback_form') && array_intersect(['storyteller', 'storyteller_assistant', 'storyteller_candidate', 'lead'], $roles)) {
    $name = $tokens['[data:storyhost-value]'];
    $ids = $name === '' ? [] : \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('name', $name)->range(0, 1)->execute();
    if ($storyhost = User::load((int) reset($ids))) {
      vaxia_storyhosts_mail_text('rules_send_feedback_to_sh_from_webform.mail.0', $storyhost->getEmail(), $tokens);
    }
    vaxia_storyhosts_mail_text_to_roles('rules_send_feedback_to_sh_from_webform.mail_to_users_of_role.1', ['lead'], $tokens);
  }

  // "SH Test - Send Player Feedback on Session to Leads": the Session
  // Feedback form (setting session_feedback_form), filled in by players,
  // mailed to the storyhosts (not to the ASH it names). D7 looked the
  // storyhost up from the storyhost feedback form's latest submission, unused.
  if ((int) $node->id() === (int) $settings->get('session_feedback_form') && in_array('player', $roles, TRUE)) {
    vaxia_storyhosts_mail_text_to_roles('sh_test_send_session_feedback_to_sh_from_webform.mail_to_users_of_role.0', ['storyteller'], $tokens);
  }

  // "SH Test - Promote to Storyhost": a vote that carries a candidate's
  // storyteller application (enough storyhosts approve, and enough of the
  // application's approvers) makes them a full storyhost. Other application
  // roles are granted by vaxia_governance_role_app_vote().
  if ($node->bundle() !== 'application' || (string) $node->get('field_applied_role_id')->value !== 'storyteller' || !function_exists('vaxia_governance_approvals')) {
    return;
  }
  $author = User::load($node->getOwnerId());
  $character = $author ? $author->get('field_storyhost_character')->entity : NULL;
  if (!$character || $character->bundle() !== 'character_sheet' || array_diff(['eligible_for_sh', 'player', 'storyteller_candidate'], $author->getRoles())
    || !vaxia_storyhosts_ash3_approved($node)
    || vaxia_governance_approval_percent($node) < (float) \Drupal::config('vaxia_governance.settings')->get('application_approval_min')) {
    return;
  }
  vaxia_governance_set_open($node, FALSE);
  $character->set('field_titles', 'SH');
  $character->save();
  $author->removeRole('storyteller_candidate');
  $author->addRole('storyteller');
  $today = date('Y-m-d');
  foreach (['field_took_ash1', 'field_took_ash2', 'field_took_ash3', 'field_became_sh'] as $field) {
    $author->set($field, $today);
  }
  $author->set('field_powers_expire', date('Y-m-d', strtotime('+3 months')));
  $author->save();
  $tokens = ['[node:author:name]' => $author->getDisplayName()];
  \Drupal::messenger()->addStatus(vaxia_storyhosts_text('rules_response_sh_application.drupal_message.0', 'message', $tokens));
  vaxia_storyhosts_mail_text('rules_response_sh_application.mail.1', $author->getEmail(), $tokens);
  vaxia_storyhosts_mail_text_to_roles('rules_response_sh_application.mail_to_users_of_role.2', ['storyteller', 'starvation_mode_ash_approver'], $tokens);
}
