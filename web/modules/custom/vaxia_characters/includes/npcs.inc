<?php

/**
 * @file
 * NPCs (D7 npcs and its Rules): submitted NPCs under the stat cap are
 * approved at once; those at or over it need oversight; approved NPCs keep
 * their numbers.
 */

use Drupal\Core\Form\FormStateInterface;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\vaxia_core\Character;

/**
 * Whether an NPC has a stat or skill at or over the cap (D7
 * npcs_check_needs_oversight()).
 */
function vaxia_characters_npc_needs_oversight(NodeInterface $node): bool {
  $cap = (float) \Drupal::config('vaxia_characters.settings')->get('npcs_stat_cap');
  foreach (array_keys(Character::STATS) as $stat) {
    if ($node->hasField('field_' . $stat) && (float) $node->get('field_' . $stat)->value >= $cap) {
      return TRUE;
    }
  }
  foreach (Character::skills($node) as $skill) {
    if ((float) $skill->get('field_skill_value')->value >= $cap) {
      return TRUE;
    }
  }
  return FALSE;
}

function vaxia_characters_is_any_npc(NodeInterface $node): bool {
  return Character::isNpc($node) || Character::flag($node, 'field_is_open_npc');
}

/**
 * A submitted NPC: oversight, or straight to approved (D7 Rules "NPCs -
 * Notify char eval re: OVERSIGHT NPC" and "... - pass_npc").
 *
 * @return bool
 *   TRUE when the NPC was handled (no player-character evaluation).
 */
function vaxia_characters_npc_submitted(NodeInterface $node): bool {
  if (!vaxia_characters_is_any_npc($node)) {
    return FALSE;
  }
  $messenger = \Drupal::messenger();
  if (vaxia_characters_npc_needs_oversight($node)) {
    $messenger->addWarning('This character falls under OVERSIGHT as it has a stat or skill at or over 70 points. Please consider if this character is absolutely necessary to have available for play on the site and be ready to validate the need and use of the character.');
    $ids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', ['storyteller', 'administrator'], 'IN')->execute();
    foreach (User::loadMultiple($ids) as $account) {
      \Drupal::service('plugin.manager.mail')->mail('vaxia_characters', 'text', $account->getEmail(), $account->getPreferredLangcode(), [
        'subject' => '[site:mail-prefix] A NPC has been submitted that needs OVERSIGHT',
        'body' => 'An NPC has been submitted that requires oversight: ' . $node->label() . "\n" . $node->toUrl('canonical', ['absolute' => TRUE])->toString(),
      ]);
    }
    return TRUE;
  }
  Character::forceStateLater($node, 'character_evaluation_approved', 'Passing NPC to approved. All stats and skills under 70. (executed by system:rules:oversight_npc_pass)');
  $messenger->addStatus('This character has all stat and skills under 70 points. Therefore the character does not require oversight and has been passed along directly to approval.');
  return TRUE;
}

/**
 * Approved NPCs: only descriptive fields stay editable.
 */
function vaxia_characters_npc_form(array &$form, NodeInterface $node) {
  if ($node->isNew() || !Character::isPlayable($node) || !Character::isNpc($node)) {
    return;
  }
  $allowed = ['field_age_app', 'field_background', 'field_personality', 'field_description', 'field_occupation', 'field_last_name', 'field_titles', 'field_sex', 'field_age_real', 'field_post_color', 'field_character_owner', 'field_sh_notes', 'field_sh_private_notes', 'field_player_notes', 'field_artwork_image'];
  foreach (Character::realmFields() as $fields) {
    $allowed[] = $fields['species'];
  }
  foreach ($form as $key => &$element) {
    if (is_array($element) && (strpos((string) $key, 'field_') === 0 || $key === 'title') && !in_array($key, $allowed, TRUE)) {
      $element['#disabled'] = TRUE;
    }
  }
  unset($element);
  \Drupal::messenger()->addStatus('You are attempting to edit an NPC that has already been approved for play. NPC numbers cannot be further edited after approval. Please revert this NPC to draft, and take it through the approval process again if you need to update numbers.');
}
