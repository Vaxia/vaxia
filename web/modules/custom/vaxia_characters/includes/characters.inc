<?php

/**
 * @file
 * Character sheet display and edit rules (D7 vaxia_characters).
 */

use Drupal\Core\Entity\Display\EntityViewDisplayInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\vaxia_core\Character;

/**
 * Implements hook_ENTITY_TYPE_view_alter() for nodes: sheet warnings, public
 * fields only for other players, SH notes for owner and staff.
 */
function vaxia_characters_node_view_alter(array &$build, NodeInterface $node, EntityViewDisplayInterface $display) {
  if (!Character::isSheet($node) || $build['#view_mode'] !== 'full') {
    return;
  }
  $build['#cache']['max-age'] = 0;
  $account = \Drupal::currentUser();
  $own = $node->getOwnerId() == $account->id();
  $messenger = \Drupal::messenger();
  $state = Character::state($node);
  if ($state === 'character_evaluation_xp_transferred') {
    $messenger->addStatus('This character is currently listed as transferred XP. This character CANNOT be returned to play, because the player has already moved the XP to another character.');
  }
  $xp = \Drupal::service('vaxia_core.xp')->get((int) $node->id(), 'xp');
  $staff = $own || $account->hasPermission('edit SH notes');
  if (!Character::isStoryteller($node) && $staff) {
    if (in_array($state, ['character_evaluation_creation', 'character_evaluation_draft', 'character_evaluation_submitted', NULL], TRUE) && $xp < 20) {
      $flags = Character::minmaxFlags($node);
      if ($flags) {
        $flags[] = (string) t('This is a character design approach that is likely to be reviewed by our evaluation staff as a potential min-maxed build. Please expect to be asked to revise.');
        $messenger->addWarning(implode(' ', $flags));
      }
    }
    // Not for NPCs (user, 2026-11-26).
    if (Character::isPlayable($node) && !Character::isNpc($node) && $xp < 20) {
      $messenger->addStatus('This character is currently listed below 20 XP. You may request assistance from our Evaluation team to revise the character.');
    }
  }
  // Derived stats (D7 computed fields): read-only averages of two stats,
  // after the stats. Built under their D7 field names, so the public-fields
  // setting below decides who sees them, as it did on D7 (user, 2026-11-26).
  $weight = 0;
  foreach (array_keys(Character::STATS) as $stat) {
    $weight = max($weight, (float) ($build['field_' . $stat]['#weight'] ?? 0));
  }
  $i = 0;
  foreach (Character::DERIVED as $derived_key => [$label, $one, $two]) {
    $key = 'field_' . $derived_key;
    $i++;
    if (!$node->hasField('field_' . $one) || !$node->hasField('field_' . $two)) {
      continue;
    }
    $value = ((float) $node->get('field_' . $one)->value + (float) $node->get('field_' . $two)->value) / 2;
    $build[$key] = [
      '#type' => 'item',
      '#title' => $label,
      '#markup' => (string) round($value, 1),
      '#wrapper_attributes' => ['class' => ['vaxia-derived-stat', 'field--label-inline', 'clearfix']],
      '#weight' => $weight + $i / 100,
    ];
  }
  // Other players see only the public fields.
  if (!$staff) {
    $public = array_filter(array_map('trim', explode("\n", (string) \Drupal::config('vaxia_core.settings')->get('vaxia_public_char_fields'))));
    foreach (array_keys($build) as $key) {
      if (is_string($key) && $key !== '' && $key[0] !== '#' && strpos($key, 'field_') === 0 && !in_array($key, $public, TRUE)) {
        unset($build[$key]);
      }
    }
  }
  if (!$staff) {
    unset($build['field_sh_notes'], $build['field_player_notes']);
  }
  if (!(!$own && $account->hasPermission('edit private SH notes'))) {
    unset($build['field_sh_private_notes']);
  }
}

/**
 * Transferred sheets are closed to players without "view inactive sheets".
 */
function vaxia_characters_sheet_gate(NodeInterface $node): ?string {
  if (Character::isSheet($node) && Character::state($node) === 'character_evaluation_xp_transferred' && !\Drupal::currentUser()->hasPermission('view inactive sheets')) {
    return 'You are not allowed to view this sheet. It has been removed from play.';
  }
  return NULL;
}

/**
 * Implements hook_form_BASE_FORM_ID_alter() for node forms: sheet edit rules.
 */
function vaxia_characters_form_node_form_alter(&$form, FormStateInterface $form_state) {
  $node = $form_state->getFormObject()->getEntity();
  if (!Character::isSheet($node)) {
    return;
  }
  vaxia_characters_npc_form($form, $node);
  $account = \Drupal::currentUser();
  $own = $node->isNew() || $node->getOwnerId() == $account->id();
  $config = \Drupal::config('vaxia_core.settings');
  $messenger = \Drupal::messenger();
  if (!Character::isPlayable($node)) {
    foreach (['field_economy', 'field_npcs', 'field_items', 'field_property', 'field_achivements', 'field_skill_review'] as $field) {
      if (isset($form[$field])) {
        $form[$field]['#access'] = FALSE;
      }
    }
    if ($own && !$account->hasPermission('create NPCs')) {
      $messenger->addStatus(t('This character is currently not playable.'));
      // Stats and skills from the allowed ranges.
      $stat_range = range((int) $config->get('vaxia_pc_stat_min'), (int) $config->get('vaxia_pc_stat_max'));
      foreach (array_keys(Character::STATS) as $stat) {
        if (isset($form['field_' . $stat]['widget'][0]['value'])) {
          $element = &$form['field_' . $stat]['widget'][0]['value'];
          $element['#type'] = 'select';
          $element['#options'] = array_combine($stat_range, $stat_range);
          unset($element['#min'], $element['#max'], $element['#step'], $element['#size']);
          unset($element);
        }
      }
      $skill_range = array_merge([0], range((int) $config->get('vaxia_pc_skill_min'), (int) $config->get('vaxia_pc_skill_max')));
      foreach ($form['field_skill']['widget'] ?? [] as $delta => $item) {
        if (is_int($delta) && isset($form['field_skill']['widget'][$delta]['subform']['field_skill_value']['widget'][0]['value'])) {
          $element = &$form['field_skill']['widget'][$delta]['subform']['field_skill_value']['widget'][0]['value'];
          $element['#type'] = 'select';
          $element['#options'] = array_combine($skill_range, $skill_range);
          unset($element['#min'], $element['#max'], $element['#step'], $element['#size']);
          unset($element);
        }
      }
      // "Other" species and Conditions need enough HXP (and no shadowban).
      $user = User::load($account->id());
      $hxp = $user && $user->hasField('field_xp_over_time') ? (int) $user->get('field_xp_over_time')->value : 0;
      $hidden = FALSE;
      \Drupal::moduleHandler()->invokeAllWith('vaxia_chat_hidden_user', function (callable $hook) use ($account, &$hidden) {
        $hidden = $hidden || $hook($account);
      });
      if ($hxp < (int) $config->get('vaxia_other_min_hxp') || $hidden) {
        foreach (Character::realmFields() as $fields) {
          unset($form[$fields['species']]['widget']['#options']['other']);
          if (isset($form[$fields['conditions']])) {
            $form[$fields['conditions']]['#disabled'] = TRUE;
          }
        }
      }
    }
    if (!$node->isNew() && $own && Character::state($node) === 'character_evaluation_submitted') {
      $messenger->addStatus(t('This character is currently in submit and cannot be edited by the owner.'));
      $form['#disabled'] = TRUE;
    }
    if (!$node->isNew() && $own && Character::state($node) === 'character_evaluation_draft' && \Drupal::service('vaxia_core.xp')->get((int) $node->id(), 'xp') != 0) {
      $messenger->addStatus(t('This character is currently in draft, but has earned XP and cannot be edited by the owner.'));
      $form['#disabled'] = TRUE;
    }
  }
  elseif ($own && isset($form['field_skill']['widget'])) {
    // Playable: owners keep their skills (no adding or removing).
    $form['field_skill']['widget']['add_more']['#access'] = FALSE;
    foreach ($form['field_skill']['widget'] as $delta => $item) {
      if (is_int($delta) && isset($form['field_skill']['widget'][$delta]['top']['links'])) {
        $form['field_skill']['widget'][$delta]['top']['links']['#access'] = FALSE;
      }
    }
  }
  if (!$own && !$account->hasPermission('edit SH notes') && isset($form['field_sh_notes'])) {
    $form['field_sh_notes']['#disabled'] = TRUE;
  }
  if (!(!$own && $account->hasPermission('edit private SH notes')) && isset($form['field_sh_private_notes'])) {
    $form['field_sh_private_notes']['#access'] = FALSE;
  }
  if ((!$account->hasPermission('create SHs') || Character::storyhostCharacter((int) $node->getOwnerId())) && isset($form['field_is_storyteller'])) {
    $form['field_is_storyteller']['#access'] = FALSE;
  }
  // "Is open NPC" too: an open NPC skips evaluation (npcs.inc), so a player
  // could tick it to have a sheet approved at once (review, 2026-11-26).
  if (!$account->hasPermission('create NPCs')) {
    foreach (['field_is_npc', 'field_is_generic_npc', 'field_is_open_npc'] as $field) {
      if (isset($form[$field])) {
        $form[$field]['#access'] = FALSE;
      }
    }
  }
  if (!$account->hasPermission('edit personal rooms') && isset($form['field_personal_room'])) {
    $form['field_personal_room']['#access'] = FALSE;
  }
}
