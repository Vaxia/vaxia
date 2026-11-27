<?php

/**
 * @file
 * Storyhost duties (D7 storyhost_duties): the activity table's counts, powers
 * expiry, and retiring storyhosts who lapse.
 */

use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\Character;

/**
 * Roles a retiring storyhost loses.
 */
const VAXIA_STORYHOSTS_SH_ROLES = [
  'starvation_mode_ash_approver', 'storyteller_newbie', 'storyteller_assistant',
  'storyteller_candidate', 'storyteller_expired', 'storyteller', 'lead',
];

/**
 * Whether a session report counts toward a storyhost's duties (D7
 * sh_test_session_counts()).
 */
function vaxia_storyhosts_session_counts(NodeInterface $session): bool {
  $config = \Drupal::config('vaxia_storyhosts.settings');
  if (!$session->isPublished()) {
    return FALSE;
  }
  if ($config->get('sh_test_session_party_size')) {
    $count = 0;
    foreach ($session->get('field_characters_involved')->referencedEntities() as $character) {
      if (!$config->get('sh_test_session_party_size_npcs') && Character::isNpc($character)) {
        continue;
      }
      if (!$config->get('sh_test_session_party_size_ashs') && Character::isStoryteller($character)) {
        continue;
      }
      if (!$config->get('sh_test_session_party_size_own_pcs') && (int) $character->getOwnerId() === (int) $session->getOwnerId()) {
        continue;
      }
      $count++;
    }
    if ($count < (int) $config->get('sh_test_session_party_size_min')) {
      return FALSE;
    }
  }
  if ($config->get('sh_test_session_expires') && $session->getCreatedTime() < strtotime($config->get('sh_test_session_expires_timeframe') . ' ago')) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Sessions run in the last year that count.
 */
function vaxia_storyhosts_counted_sessions(int $uid): int {
  $storage = \Drupal::entityTypeManager()->getStorage('node');
  $ids = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'session_report')->condition('uid', $uid)->condition('created', strtotime('1 year ago'), '>')->execute();
  return count(array_filter($storage->loadMultiple($ids), 'vaxia_storyhosts_session_counts'));
}

/**
 * Evaluations of others' content of a type, within 3 days of its creation, in
 * the last year. D7 compared a Unix time with a DATETIME, so every evaluation
 * counted as prompt.
 */
function vaxia_storyhosts_prompt_evaluations(int $uid, string $type, string $state): int {
  $query = \Drupal::database()->select('workflow_transition_history', 'w');
  $query->join('node_field_data', 'n', "n.nid = w.entity_id AND w.entity_type = 'node'");
  $query->condition('w.uid', $uid)
    ->condition('n.type', $type)
    ->condition('n.created', strtotime('1 year ago'), '>')
    ->where('n.uid <> w.uid')
    ->where('w.from_sid <> w.to_sid')
    ->condition('w.to_sid', $state)
    ->where('w.timestamp <= n.created + 259200');
  if ($type === 'character_sheet') {
    $query->leftJoin('node__field_is_npc', 'npc', 'npc.entity_id = n.nid');
    $query->leftJoin('node__field_is_storyteller', 'st', 'st.entity_id = n.nid');
    $query->where('COALESCE(npc.field_is_npc_value, 0) = 0 AND COALESCE(st.field_is_storyteller_value, 0) = 0');
  }
  $query->addExpression('COUNT(DISTINCT w.hid)', 'count');
  return (int) $query->execute()->fetchField();
}

function vaxia_storyhosts_created_count(int $uid, string $type): int {
  return (int) \Drupal::entityQuery('node')->accessCheck(FALSE)->condition('type', $type)->condition('uid', $uid)->condition('created', strtotime('1 year ago'), '>')->count()->execute();
}

function vaxia_storyhosts_revised_count(int $uid, string $type): int {
  $query = \Drupal::database()->select('node_revision', 'r');
  $query->join('node_field_data', 'n', 'n.nid = r.nid');
  $query->condition('r.revision_uid', $uid)->condition('n.type', $type)->condition('r.revision_timestamp', strtotime('1 year ago'), '>');
  return (int) $query->countQuery()->execute()->fetchField();
}

/**
 * Replies in the "SH Requests" forums in the last year.
 */
function vaxia_storyhosts_sh_forum_replies(int $uid): int {
  $forums = \Drupal::config('vaxia_storyhosts.settings')->get('sh_request_forums') ?: [];
  if (!$forums) {
    return 0;
  }
  $query = \Drupal::database()->select('comment_field_data', 'c');
  $query->join('node__taxonomy_forums', 't', "t.entity_id = c.entity_id AND c.entity_type = 'node'");
  $query->condition('t.taxonomy_forums_target_id', $forums, 'IN')->condition('c.uid', $uid)->condition('c.created', strtotime('1 year ago'), '>');
  return (int) $query->countQuery()->execute()->fetchField();
}

/**
 * Threads with messages from others the storyhost hasn't read, and when the
 * oldest was sent.
 */
function vaxia_storyhosts_unread_pm(int $uid): array {
  $db = \Drupal::database();
  if (!$db->schema()->tableExists('pm_index')) {
    return [];
  }
  $query = $db->select('pm_index', 'i');
  $query->leftJoin('pm_thread_history', 'h', 'h.thread_group = i.group AND h.uid = i.owner');
  $query->join('privatemsg_thread__private_messages', 'tm', 'tm.entity_id = i.id');
  $query->join('pm_message', 'm', 'm.id = tm.private_messages_target_id');
  $query->condition('i.owner', $uid)->isNull('i.deleted')->condition('m.owner', $uid, '<>')
    ->where('m.created >= COALESCE(h.access_timestamp, 0)');
  $query->addField('i', 'group');
  $query->addExpression('MIN(m.created)', 'oldest');
  $query->groupBy('i.group');
  return $query->execute()->fetchAllKeyed();
}

function vaxia_storyhosts_unread_threads(int $uid): int {
  return count(vaxia_storyhosts_unread_pm($uid));
}

function vaxia_storyhosts_oldest_unread(int $uid): string {
  $unread = vaxia_storyhosts_unread_pm($uid);
  return $unread ? \Drupal::service('date.formatter')->format(min($unread)) : (string) t('all read');
}

/**
 * Pushes a storyhost's powers expiry out to at least the given time.
 */
function vaxia_storyhosts_extend_powers(UserInterface $account, string $until): void {
  $target = date('Y-m-d', strtotime($until));
  $current = (string) $account->get('field_powers_expire')->value;
  if ($current === '' || $current < $target) {
    $account->set('field_powers_expire', $target);
    $account->save();
  }
}

/**
 * A storyhost evaluating someone else's character or item keeps powers for 6
 * months (D7 "SH Duties - Update power expire on eval char/item").
 */
function vaxia_storyhosts_duties_transition($transition): void {
  $node = $transition->getTargetEntity();
  $moves = ['character_sheet' => ['character_evaluation_submitted', 'character_evaluation_approved'], 'items' => ['item_evaluation_submitted', 'item_evaluation_approved']];
  if (!$node instanceof NodeInterface || !isset($moves[$node->bundle()])
    || [$transition->getFromSid(), $transition->getToSid()] !== $moves[$node->bundle()]) {
    return;
  }
  $evaluator = User::load($transition->getOwnerId());
  if ($evaluator && $evaluator->hasRole('storyteller') && (int) $evaluator->id() !== (int) $node->getOwnerId()) {
    vaxia_storyhosts_extend_powers($evaluator, '+6 months');
  }
}

/**
 * Session reports keep their storyhost's powers for 3 months (D7 "SH Duties -
 * Update power expire on session creation/update").
 */
function vaxia_storyhosts_duties_session(NodeInterface $node): void {
  if ($node->bundle() !== 'session_report' || (int) $node->getOwnerId() !== (int) \Drupal::currentUser()->id()) {
    return;
  }
  if (!$node->isNew() && isset($node->original) && $node->getCreatedTime() <= strtotime('-3 days')) {
    return;
  }
  $author = User::load($node->getOwnerId());
  if ($author && $author->hasRole('storyteller') && vaxia_storyhosts_session_counts($node)) {
    vaxia_storyhosts_extend_powers($author, '+3 months');
  }
}

/**
 * Retires a storyhost: character titled "SH (retired)", staff roles removed,
 * mails to them and their peers.
 */
function vaxia_storyhosts_retire(UserInterface $account, string $why): void {
  $character = $account->get('field_storyhost_character')->entity;
  if (!$character || $character->bundle() !== 'character_sheet') {
    return;
  }
  $character->set('field_titles', 'SH (retired)');
  $character->save();
  foreach (VAXIA_STORYHOSTS_SH_ROLES as $role) {
    $account->removeRole($role);
  }
  $account->save();
  $name = $account->getDisplayName();
  if ($why === 'neglect') {
    vaxia_storyhosts_send_to_roles(['storyteller', 'lead'], '[site:mail-prefix] ' . $name . ' has lost administrative privileges',
      $name . " has not run required sessions or evaluated characters or items within the required timeframe and has lost all permissions above player as a result.\n\nYou are receiving this email because you share some of the same roles, and need to be aware of the changes. If this is a position that needs to be critically filled, please consult with other leadership on the site to recruit or nominate new members to this position as soon as possible.\n\nWhile we understand the need to prioritize life over game, we do need members in leadership positions to uphold their responsibilities. " . $name . "'s loss of position due to absence does not preclude re-gaining the position upon return to the site - on the condition that the return is intended to be a long-term return. " . $name . ' may need to go through the SH course a second time to fully regain permissions.');
    vaxia_storyhosts_send($account->getEmail(), '[site:mail-prefix] You have lost administrative privileges',
      $name . ",\n\nDespite being listed as active, you have not run sessions or evaluated characters or items within the required timeframe. As a result, you have automatically lost your privileges as a story host, and been reverted to the role of player.\n\nWhile we understand the need for everyone to prioritize life over game, we also need those in the role of evaluator to uphold their responsibilities, including but not limited to running sessions for the site. Your loss of position due to absence does not prevent you from re-gaining the position if and when you return to the site, provided you retake and again complete the SH course, including all tests and session requirements involved.\n\nIf you have any questions about the policies on inactive SHs, use the site's Contact form or post to the Policies forum to get clarification.");
  }
  else {
    vaxia_storyhosts_send_to_roles(['storyteller', 'administrator'], '[site:mail-prefix] Administrative privileges lost ',
      $name . " has been absent from the site for 6 months, and has lost all permissions above player as a result.\n\nYou are receiving this email because you share some of the same roles, and need to be aware of the changes. If this is a position that needs to be critically filled, please consult with other leadership on the site to recruit or nominate new members to this position.\n\nWhile we understand the need to prioritize life over game, we do need members in leadership positions to uphold their responsibilities. " . $name . "'s loss of position due to absence does not preclude re-gaining the position upon return to the site - on the condition that the return is intended to be a long-term return.");
    vaxia_storyhosts_send($account->getEmail(), '[site:mail-prefix] Storyhost privileges lost ',
      $name . ",\n\nDespite being listed as active, you have been completely absent from the site for six months without logging in. As a result, you have automatically lost your privileges as a story host, and been reverted to the role of player.\n\nWhile we understand the need for everyone to prioritize life over game, we also need those in the role of SH to uphold their responsibilities, including but not limited to running sessions for the site. Your loss of position due to absence does not prevent you from re-gaining the position if and when you return to the site, provided you retake and again complete the ASH course, including all tests and session requirements involved therein.\n\nSHs aware of sustained absences from the site have the power to set themselves on \"Vacation Mode\" via their account settings. Doing so will temporarily suspend your permissions, but you can be reactivated at any time in the future through the same settings pane.\n\nIf you have any questions about the policies on inactive SHs, use the site's Contact form or post to the Policies forum to get clarification.");
  }
}

/**
 * Daily sweep: storyhosts whose powers lapsed, or who've been away 6 months,
 * retire (D7's rules listened for an event that never delivered a user).
 */
function vaxia_storyhosts_duties_cron(): void {
  $storage = \Drupal::entityTypeManager()->getStorage('user');
  $lapsed = $storage->getQuery()->accessCheck(FALSE)->condition('status', 1)
    ->condition('roles', ['storyteller_expired', 'storyteller', 'lead'], 'IN')
    ->condition('field_powers_expire', date('Y-m-d'), '<')->execute();
  foreach ($storage->loadMultiple($lapsed) as $account) {
    vaxia_storyhosts_retire($account, 'neglect');
  }
  $away = $storage->getQuery()->accessCheck(FALSE)->condition('status', 1)
    ->condition('roles', ['storyteller_newbie', 'storyteller_assistant', 'storyteller_candidate', 'storyteller_expired', 'storyteller', 'lead'], 'IN')
    ->condition('access', 0, '>')->condition('access', strtotime('-6 months'), '<')->execute();
  foreach ($storage->loadMultiple($away) as $account) {
    vaxia_storyhosts_retire($account, 'away');
  }
}
