<?php

/**
 * @file
 * Character evaluation (D7 vaxia_evaluation's Rules): what happens when a
 * character is submitted, approved, sent back or commented on.
 */

use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\vaxia_characters\EvaluationTexts;
use Drupal\vaxia_core\Character;

/**
 * A text from EvaluationTexts with its tokens filled in.
 */
function vaxia_characters_text(string $key, string $part, NodeInterface $node, string $comment = ''): string {
  $text = EvaluationTexts::TEXTS[$key][$part];
  return strtr($text, [
    '[node:title]' => $node->label(),
    '[node:url]' => $node->toUrl('canonical', ['absolute' => TRUE])->toString(),
    '[node:author]' => $node->getOwner()->getDisplayName(),
    '[date:+1 week]' => date('m/d/Y', strtotime('+1 week')),
    '[node:workflow-current-state-log-entry]' => $comment,
    '[node:workflow-current-state-updating-user-name]' => \Drupal::currentUser()->getDisplayName(),
  ]);
}

/**
 * Mails a text to an account (if active) or to every administrator.
 */
function vaxia_characters_mail_text(string $key, NodeInterface $node, ?User $to = NULL, string $comment = ''): void {
  $recipients = [];
  if ($to) {
    $recipients = $to->isActive() ? [$to] : [];
  }
  else {
    $ids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', 'administrator')->execute();
    $recipients = User::loadMultiple($ids);
  }
  foreach ($recipients as $account) {
    \Drupal::service('plugin.manager.mail')->mail('vaxia_characters', 'text', $account->getEmail(), $account->getPreferredLangcode(), [
      'subject' => EvaluationTexts::TEXTS[$key]['subject'],
      'body' => vaxia_characters_text($key, 'message', $node, $comment),
    ]);
  }
}

/**
 * Implements hook_mail().
 */
function vaxia_characters_mail($key, &$message, $params) {
  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];
}

/**
 * The comment on the node's latest workflow transition.
 */
function vaxia_characters_last_comment(NodeInterface $node): string {
  return (string) \Drupal::database()->queryRange("SELECT comment FROM {workflow_transition_history} WHERE entity_type = 'node' AND entity_id = :nid ORDER BY hid DESC", 0, 1, [':nid' => $node->id()])->fetchField();
}

/**
 * Reacts to a character's workflow move (D7 Rules on workflow_state_changed).
 */
function vaxia_characters_evaluate(NodeInterface $node, string $before, string $after): void {
  $owner = $node->getOwner();
  $messenger = \Drupal::messenger();
  if ($after === 'character_evaluation_submitted' && in_array($before, ['character_evaluation_creation', 'character_evaluation_draft'], TRUE)
    && !Character::isNpc($node) && !Character::isStoryteller($node)) {
    $user = User::load($owner->id());
    $field = fn($name) => $user->hasField($name) ? (int) $user->get($name)->value : 0;
    // "Catch Other and Condition characters under HXP": these concepts need 40 HXP.
    $other = FALSE;
    foreach (Character::realmFields() as $fields) {
      $other = $other || ($node->hasField($fields['species']) && $node->get($fields['species'])->value === 'other');
    }
    $conditions = FALSE;
    foreach (Character::realmFields() as $fields) {
      $conditions = $conditions || ($fields['conditions'] !== '' && $node->hasField($fields['conditions']) && !$node->get($fields['conditions'])->isEmpty());
    }
    if ($field('field_xp_over_time') < 41 && ($other || $conditions)) {
      Character::forceStateLater($node, 'character_evaluation_draft', 'This character has been returned to draft by the system. The player is under the HXP requirement for playing characters with Species - Other, or Conditions.');
      $messenger->addError(EvaluationTexts::TEXTS['rules_char_eval_other_and_conditions_under_hxp.drupal_message.0']['message']);
      return;
    }
    // "Catch too many Min Max characters": players whose submissions are
    // mostly min-maxed can't submit another min-maxed one.
    $submitted = $field('field_characters_submitted');
    $minmax = $field('field_characters_minmax');
    if (\Drupal::currentUser()->id() == $owner->id() && $submitted > 3 && $minmax > 0
      && \Drupal::service('vaxia_core.xp')->get((int) $node->id(), 'xp') == 0
      && Character::minmaxFlags($node) && $minmax / $submitted > 0.5) {
      Character::forceStateLater($node, 'character_evaluation_draft', "This character has been returned to draft by the system. The majority of this player's submissions over time have been flagged under the min-max settings. Most characters submitted should not fall under min-max flags.");
      $user->set('field_characters_minmax', $minmax + 1);
      $user->set('field_characters_submitted', $submitted - 1);
      $user->save();
      $messenger->addError(EvaluationTexts::TEXTS['rules_catch_too_many_min_max_characters.drupal_message.0']['message']);
      return;
    }
  }
  if ($after === 'character_evaluation_submitted' && $owner->isActive()) {
    // "Notify char eval re: new char".
    vaxia_characters_mail_text('items_notify_char_eval_re_new_char.mail_to_users_of_role.0', $node);
    $messenger->addStatus(vaxia_characters_text('items_notify_char_eval_re_new_char.drupal_message.1', 'message', $node));
    $user = User::load($owner->id());
    if ($user->hasField('field_characters_submitted')) {
      $user->set('field_characters_submitted', (int) $user->get('field_characters_submitted')->value + 1);
      $user->save();
    }
  }
  if ($before === 'character_evaluation_submitted' && $after === 'character_evaluation_approved' && $owner->isActive()) {
    // "Notify player when char approved": they're a player now.
    vaxia_characters_mail_text('items_notify_player_when_char_approved.mail.0', $node, User::load($owner->id()), vaxia_characters_last_comment($node));
    $user = User::load($owner->id());
    $user->addRole('player');
    $user->removeRole('new_account');
    $user->save();
  }
  if ($before === 'character_evaluation_submitted' && $after === 'character_evaluation_draft' && $owner->isActive()) {
    // "Notify player when char returned to draft".
    vaxia_characters_mail_text('items_notify_player_when_char_disapproved.mail.0', $node, User::load($owner->id()), vaxia_characters_last_comment($node));
  }
}

/**
 * Implements hook_ENTITY_TYPE_insert() for workflow transitions: a comment
 * without a move on a submitted character goes to its player (D7 rule
 * "Notify player when char workflow commented").
 */
function vaxia_characters_workflow_transition_insert($transition) {
  $node = $transition->getTargetEntity();
  if (!Character::isSheet($node) || $transition->getFromSid() !== $transition->getToSid()
    || $transition->getToSid() !== 'character_evaluation_submitted' || trim((string) $transition->getComment()) === ''
    || !$node->getOwner()->isActive()) {
    return;
  }
  \Drupal::messenger()->addStatus(EvaluationTexts::TEXTS['vaxia_evaluation_notify_player_when_char_workflow.drupal_message.0']['message']);
  vaxia_characters_mail_text('vaxia_evaluation_notify_player_when_char_workflow.mail.1', $node, User::load($node->getOwnerId()), (string) $transition->getComment());
}
