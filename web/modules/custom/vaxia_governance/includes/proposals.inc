<?php

/**
 * @file
 * Proposals (D7 proposal): site votes run as webforms. The author builds the
 * ballot and launches it; it stays open for two weeks from its last edit.
 * Only those eligible when the ballot was set up may vote.
 */

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Url;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\vaxia_core\Cron;
use Drupal\webform\Entity\Webform;

function vaxia_governance_setting(string $key): string {
  return (string) \Drupal::config('vaxia_governance.settings')->get($key);
}

/**
 * The proposal's webform, if any.
 */
function vaxia_governance_webform(NodeInterface $node): ?Webform {
  return $node->hasField('webform') ? $node->get('webform')->entity : NULL;
}

function vaxia_governance_is_open(NodeInterface $node): bool {
  return $node->hasField('webform') && !$node->get('webform')->isEmpty() && $node->get('webform')->status === 'open';
}

function vaxia_governance_set_open(NodeInterface $node, bool $open): void {
  $was_open = vaxia_governance_is_open($node);
  $node->get('webform')->status = $open ? 'open' : 'closed';
  $node->save();
  // D7 vaxia_rpg "Email proposal owner on close".
  if ($was_open && !$open && $node->bundle() === 'proposal' && $node->getOwner()->isActive()) {
    vaxia_core_rule_mail('proposal_proposal_vaxia_email_owner_on_close.mail.0', $node->getOwner()->getEmail(), [
      '[node:title]' => $node->label(),
      '[node:url]' => $node->toUrl('canonical', ['absolute' => TRUE])->toString(),
      '[node:author]' => $node->getOwner()->getDisplayName(),
      '[site:current-date]' => \Drupal::service('date.formatter')->format(\Drupal::time()->getRequestTime(), 'medium'),
    ]);
  }
}

/**
 * Submissions (votes) on the proposal.
 */
function vaxia_governance_vote_count(NodeInterface $node): int {
  $webform = vaxia_governance_webform($node);
  if (!$webform) {
    return 0;
  }
  return (int) \Drupal::entityQuery('webform_submission')->accessCheck(FALSE)
    ->condition('webform_id', $webform->id())->condition('entity_type', 'node')->condition('entity_id', $node->id())
    ->count()->execute();
}

/**
 * Takes the snapshot of who may vote: holders of $roles, less holders of
 * $barred and shadowbanned users (D7 called a function no module defined, so
 * shadowbanned users could vote).
 */
function vaxia_governance_set_eligible(NodeInterface $node, array $roles, array $barred = []): void {
  $db = \Drupal::database();
  $db->delete('proposal_eligable_users')->condition('nid', $node->id())->execute();
  if (!$roles) {
    return;
  }
  $uids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', $roles, 'IN')->execute();
  foreach (User::loadMultiple($uids) as $account) {
    if (array_intersect(array_merge($barred, ['shadowbanned_user']), $account->getRoles())) {
      continue;
    }
    $db->insert('proposal_eligable_users')->fields(['nid' => $node->id(), 'uid' => $account->id()])->execute();
  }
}

function vaxia_governance_is_eligible(NodeInterface $node, int $uid): bool {
  if (!$uid || in_array('shadowbanned_user', User::load($uid)->getRoles(), TRUE)) {
    return FALSE;
  }
  return (bool) \Drupal::database()->query('SELECT 1 FROM {proposal_eligable_users} WHERE nid = :nid AND uid = :uid', [':nid' => $node->id(), ':uid' => $uid])->fetchField();
}

function vaxia_governance_eligible_count(NodeInterface $node): int {
  return (int) \Drupal::database()->query('SELECT COUNT(uid) FROM {proposal_eligable_users} WHERE nid = :nid', [':nid' => $node->id()])->fetchField();
}

/**
 * Eligible voters who commented in the active window from $start.
 */
function vaxia_governance_active_count(NodeInterface $node, int $start): int {
  $end = strtotime('+' . vaxia_governance_setting('proposal_active_timeframe'), $start);
  return (int) \Drupal::database()->query('SELECT COUNT(u.uid) FROM {proposal_eligable_users} u WHERE u.nid = :nid AND u.uid IN (SELECT uid FROM {comment_field_data} WHERE changed > :start AND changed < :end)', [':nid' => $node->id(), ':start' => $start, ':end' => $end])->fetchField();
}

/**
 * Sets a whole form (and its children) disabled.
 */
function vaxia_governance_disable(array &$element): void {
  $element['#disabled'] = TRUE;
  foreach (Element::children($element) as $child) {
    vaxia_governance_disable($element[$child]);
  }
}

/**
 * Implements hook_form_BASE_FORM_ID_alter() for node forms: the help text, and
 * a new proposal gets its ballot.
 */
function vaxia_governance_form_node_form_alter(array &$form, FormStateInterface $form_state) {
  $node = $form_state->getFormObject()->getEntity();
  if ($node->bundle() !== 'proposal') {
    return;
  }
  $form['proposal_help'] = ['#markup' => vaxia_governance_setting('proposal_node_help'), '#weight' => -1000];
  // The ballot is the proposal's own; it isn't picked on the form.
  $form['webform']['#access'] = FALSE;
  $form['actions']['submit']['#submit'][] = 'vaxia_governance_proposal_form_submit';
}

/**
 * New proposals get a ballot (closed until launched; grievances open at once,
 * anonymously, with a question per accused member) and their voters.
 */
function vaxia_governance_proposal_form_submit(array $form, FormStateInterface $form_state) {
  $node = $form_state->getFormObject()->getEntity();
  if (!vaxia_governance_webform($node)) {
    $grievance = $node->get('field_proposal_type')->value === 'grievance';
    $elements = [];
    if ($grievance) {
      foreach ($node->get('field_grievance_members')->referencedEntities() as $member) {
        $elements['member_' . $member->id()] = [
          '#type' => 'radios',
          '#title' => (string) t('Should @member be removed from their position(s) on the site for misuse of power?', ['@member' => $member->getDisplayName()]),
          '#options' => ['Yes' => 'Yes', 'No' => 'No'],
          '#required' => TRUE,
        ];
      }
    }
    $elements['additional_comments'] = ['#type' => 'textarea', '#title' => (string) t('Additional Comments')];
    $webform = Webform::create([
      'id' => 'proposal_' . $node->id(),
      'title' => mb_substr($node->label(), 0, 255),
      'elements' => \Drupal\Core\Serialization\Yaml::encode($elements),
    ]);
    $webform->setSetting('confirmation_message', 'Thank you for participating!');
    $webform->setSetting('confirmation_type', 'message');
    $webform->setSetting('limit_user', 1);
    $webform->setSetting('form_close_message', 'This webform is no longer accepting submissions.');
    $rules = $webform->getAccessRules();
    $rules['create']['roles'] = ['voter'];
    // Votes are secret: only staff with results access see who voted what.
    $webform->setAccessRules($rules);
    $webform->save();
    $node->set('webform', ['target_id' => $webform->id(), 'status' => $grievance ? 'open' : 'closed']);
    if ($grievance) {
      $node->setOwnerId(1);
    }
    $node->save();
    vaxia_governance_set_eligible($node, ['voter']);
  }
  \Drupal::messenger()->addStatus(t('The webform %title has been updated. Add new fields to your webform with the form below. When you are ready to open the proposal to voters go to the View tab and click the button to launch it.', ['%title' => $node->label()]));
  $form_state->setRedirectUrl(Url::fromRoute('vaxia_governance.ballot', ['node' => $node->id()]));
}

/**
 * Proposal page: open/closed notes, the ballot (disabled when closed or for
 * those who may not vote), help, participation and results.
 */
function vaxia_governance_proposal_view(array &$build, NodeInterface $node, string $view_mode): void {
  if ($node->bundle() !== 'proposal' || !vaxia_governance_webform($node)) {
    return;
  }
  $build['#cache']['max-age'] = 0;
  $messenger = \Drupal::messenger();
  $account = \Drupal::currentUser();
  $timeframe = vaxia_governance_setting('proposal_open_timeframe');
  $open = vaxia_governance_is_open($node);
  $voted = vaxia_governance_vote_count($node);
  if ($view_mode === 'full') {
    if ($open) {
      $messenger->addStatus(t('This proposal will stay open for two weeks since it was last updated. It is set to close on @date.', ['@date' => date('l, F j, Y', strtotime($timeframe, $node->getChangedTime()))]));
    }
    else {
      $messenger->addStatus(t('This proposal is closed.'));
    }
    if ($open && !vaxia_governance_is_eligible($node, (int) $account->id())) {
      $messenger->addWarning(t('You are not eligible to vote on this issue. Either you do not have the correct site permissions to vote, or you gained them after the proposal was opened for voting.'));
    }
    if (!$open) {
      // A closed webform shows only its closed message; show the questions,
      // disabled, as D7 did.
      $ballot = vaxia_governance_webform($node)->getSubmissionForm(['entity_type' => 'node', 'entity_id' => $node->id()]);
      vaxia_governance_disable($ballot);
      $ballot['help_done'] = ['#markup' => '<br/><div class="proposal_done_message"><b>' . t('This webform is no longer accepting submissions.') . '</b></div>', '#weight' => 1000];
      $build['webform'] = ['ballot' => $ballot, '#weight' => 1000];
    }
  }
  $build['proposal_help'] = [
    '#markup' => '<div class="proposal_help"><br/><br/>' . t("<b>Proposal help:</b><br/>Proposals are meant to be a way to formally make a request for the input of site members. Forum threads and five-star ratings are not accurate enough for the big things. So here's how this works:<br/> <ul> <li>If you change your mind after you vote, you will be able to go back in and delete or change your vote up until the proposal closes.</li> <li>The form will be open for %timeframe, after which it will automatically close.</li> <li>If the original poster edits it in that timespan the timer will re-set. This is to make sure everyone can get a chance to weigh in.</li> <li>Only users active when the proposal was created can vote on it. This is to avoid ballot stuffing.</li> <li>No one, not even the original poster will be able to see the names or IP addresses of the people do vote.</li> <li>After the two week window has passed, the proposal will close and results will be publicly availble in a tab on this page.</li> </ul></div>", ['%timeframe' => $timeframe]),
    '#weight' => 1001,
  ];
  $potential = vaxia_governance_eligible_count($node);
  if ($potential) {
    $percent = number_format($voted / $potential * 100);
    $start = $open ? $node->getChangedTime() : strtotime('-2 weeks', $node->getChangedTime());
    $active = vaxia_governance_active_count($node, $start);
    // D7 divided by zero when no eligible voter had commented.
    $percent_active = $active ? number_format($voted / $active * 100) : 0;
    $build['proposal_response'] = [
      '#markup' => '<div class="proposal_response" style="margin:1em 0;">' . t('%voted out of %potential_voters potential voters (%percent %) responsed to this proposal. %voted2 out of %active_voters potential voters active on the site while the proposal was open (%percent_active %) responded to this proposal.', [
        '%voted' => $voted, '%potential_voters' => $potential, '%percent' => $percent, '%voted2' => $voted, '%active_voters' => $active, '%percent_active' => $percent_active,
      ]) . '</div>',
      '#weight' => 1002,
    ];
    $participation = t('This type of proposal does not require a minimum participation from the site.') . ' <span style="font-weight:bold;color:green;">' . t('Enough') . '</span> voters have participated for the results of this proposal to be accepted.';
    if (in_array($node->get('field_proposal_type')->value, ['amendment', 'grievance'], TRUE)) {
      $enough = (float) vaxia_governance_setting('proposal_required_perc') <= (float) $percent_active;
      $participation = t('Based on the percentage of active voter responses: <span style="font-weight:bold;color:@color">%message</span>  voters have participated for the results of this proposal to be accepted.', ['@color' => $enough ? 'green' : 'red', '%message' => $enough ? t('Enough') : t('NOT enough')]);
    }
    $build['proposal_results_participation'] = ['#markup' => '<div class="proposal_result">' . $participation . '</div><br/>', '#weight' => 1003];
  }
  else {
    $build['proposal_response'] = ['#markup' => '<div class="proposal_response" style="margin:1em 0;">' . t('There are no potential voters for this proposal at this time.') . '</div>', '#weight' => 1002];
  }
  if (!$open && $voted === 0 && $view_mode === 'full') {
    $messenger->addStatus(t('This proposal has not yet been opened. Please give them time to finish creating the proposal.'));
  }
  if (!$open || $account->hasPermission('administer proposal')) {
    $build['proposal_results'] = ['results' => vaxia_governance_results($node), '#prefix' => '<div class="proposal_result">', '#suffix' => '</div>', '#weight' => 1004];
  }
}

/**
 * Summary of responses per question, then each (anonymous) response.
 */
function vaxia_governance_results(NodeInterface $node): array {
  $webform = vaxia_governance_webform($node);
  $elements = array_filter($webform->getElementsDecodedAndFlattened(), fn($e) => !in_array($e['#type'] ?? '', ['webform_actions', 'webform_markup', 'processed_text'], TRUE));
  $ids = \Drupal::entityQuery('webform_submission')->accessCheck(FALSE)->condition('webform_id', $webform->id())->condition('entity_type', 'node')->condition('entity_id', $node->id())->execute();
  $submissions = \Drupal::entityTypeManager()->getStorage('webform_submission')->loadMultiple($ids);
  $summary = [];
  foreach ($elements as $key => $element) {
    if (empty($element['#options'])) {
      continue;
    }
    $summary[] = [['data' => ['#markup' => '<strong>' . htmlspecialchars((string) $element['#title']) . '</strong>'], 'colspan' => 2]];
    foreach ($element['#options'] as $value => $label) {
      $count = count(array_filter($submissions, fn($s) => in_array((string) $value, array_map('strval', (array) $s->getElementData($key)), TRUE)));
      $summary[] = [$label, $count];
    }
  }
  $detail = [];
  foreach ($submissions as $submission) {
    $row = [];
    foreach ($elements as $key => $element) {
      $value = $submission->getElementData($key);
      $row[] = is_array($value) ? implode(', ', $value) : (string) ($element['#options'][$value] ?? $value);
    }
    $detail[] = $row;
  }
  return [
    'help' => ['#markup' => '<div class="webform-results-analysis-help">' . t('Summary of responses reflects how many users selected a given value in the poll. In general, the larger the number the stronger the overall preference for that result. Text responses are briefly summarized but should be read individually.') . '</div>'],
    'summary' => ['#type' => 'table', '#caption' => t('Summary of responses'), '#rows' => $summary, '#empty' => t('There are no submissions for this form.'), '#attributes' => ['class' => ['webform-results-analysis']]],
    'detail' => ['#type' => 'table', '#caption' => t('Detailed responses'), '#header' => array_map(fn($e) => (string) ($e['#title'] ?? ''), array_values($elements)), '#rows' => $detail, '#empty' => t('There are no submissions for this form.'), '#attributes' => ['class' => ['webform-detailed-responses']]],
  ];
}

/**
 * Daily: proposals untouched for the open timeframe close.
 */
function vaxia_governance_proposal_cron(): void {
  if (!Cron::onceADay('proposal_cron', '00:01:00')) {
    return;
  }
  $cutoff = strtotime(vaxia_governance_setting('proposal_open_timeframe') . ' ago');
  $cutoff = strtotime(date('m/d/Y 12:59:00', strtotime('+1 day', $cutoff)));
  $storage = \Drupal::entityTypeManager()->getStorage('node');
  $ids = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'proposal')->condition('status', 1)->condition('changed', $cutoff, '<=')->condition('webform.status', 'open')->execute();
  foreach ($storage->loadMultiple($ids) as $node) {
    vaxia_governance_set_open($node, FALSE);
  }
}

/**
 * Posting keeps players voters for six months (D7 Rules + role_expire; the
 * expiry is kept in user data and enforced by cron).
 */
function vaxia_governance_voter_activity(int $uid, bool $comment): void {
  $account = User::load($uid);
  if (!$account || !$account->hasRole('player') || $account->isBlocked()) {
    return;
  }
  if (!$account->hasRole('voter')) {
    $account->addRole('voter');
    $account->save();
  }
  \Drupal::service('user.data')->set('vaxia_governance', $uid, 'voter_expires', strtotime('+' . vaxia_governance_setting('proposal_voter_timeframe')));
}

function vaxia_governance_voter_cron(): void {
  $expired = \Drupal::database()->query("SELECT uid FROM {users_data} WHERE module = 'vaxia_governance' AND name = 'voter_expires' AND CAST(value AS UNSIGNED) < :now", [':now' => \Drupal::time()->getRequestTime()])->fetchCol();
  foreach (User::loadMultiple($expired) as $account) {
    $account->removeRole('voter');
    $account->save();
    \Drupal::service('user.data')->delete('vaxia_governance', $account->id(), 'voter_expires');
  }
}

/**
 * Implements hook_webform_submission_form_alter(): on
 * an open proposal, only those eligible may vote.
 */
function vaxia_governance_webform_submission_form_alter(array &$form, FormStateInterface $form_state, $form_id) {
  $node = $form_state->getFormObject()->getEntity()->getSourceEntity();
  if ($node instanceof NodeInterface && in_array($node->bundle(), ['application', 'storyhost_application'], TRUE) && (int) $node->getOwnerId() === (int) \Drupal::currentUser()->id()) {
    // Applicants don't vote on their own application.
    vaxia_governance_disable($form);
    $form['#validate'][] = 'vaxia_governance_ineligible_validate';
    return;
  }
  if (!$node instanceof NodeInterface || $node->bundle() !== 'proposal' || !vaxia_governance_is_open($node) || !empty($form['#disabled'])) {
    return;
  }
  if (!vaxia_governance_is_eligible($node, (int) \Drupal::currentUser()->id())) {
    // The page says why (vaxia_governance_proposal_view()).
    vaxia_governance_disable($form);
    $form['#validate'][] = 'vaxia_governance_ineligible_validate';
  }
}

function vaxia_governance_ineligible_validate(array &$form, FormStateInterface $form_state) {
  $form_state->setErrorByName('', t('You are not eligible to vote on this issue.'));
}
