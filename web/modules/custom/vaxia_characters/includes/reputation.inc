<?php

/**
 * @file
 * Reputation with groups (D7 rpg_rep): staff give it on the Sheet tab, it
 * shows on the sheet and on chat posts, and drifts toward zero each month.
 * Groups are wiki pages marked "provides reputation" in the character's realm.
 */

use Drupal\comment\CommentInterface;
use Drupal\Core\Entity\Display\EntityViewDisplayInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Character;

/**
 * Reputation groups of a realm: nid => title.
 */
function vaxia_characters_rep_groups(NodeInterface $character): array {
  $realm = $character->hasField('field_tag_realm') ? $character->get('field_tag_realm')->target_id : NULL;
  if (!$realm) {
    return [];
  }
  return \Drupal::database()->query("SELECT n.nid, n.title FROM {node__field_provides_reputation} f INNER JOIN {node_field_data} n ON n.nid = f.entity_id INNER JOIN {node__field_tag_realm} r ON r.entity_id = f.entity_id WHERE f.field_provides_reputation_value = 1 AND r.field_tag_realm_target_id = :realm ORDER BY n.title", [':realm' => $realm])->fetchAllKeyed();
}

/**
 * "Group: 30 rep for a modifier of 25 to rolls." per group with reputation.
 */
function vaxia_characters_rep_lines(NodeInterface $character): array {
  $xp = \Drupal::service('vaxia_core.xp');
  $cap = 25;
  $lines = [];
  foreach (vaxia_characters_rep_groups($character) as $gid => $title) {
    $rep = $xp->get((int) $character->id(), 'rep_' . $gid);
    if ($rep != 0) {
      $bonus = abs($rep) > $cap ? ($rep > 0 ? $cap : -$cap) : $rep;
      $lines[] = t('@group_name: @gxp rep for a modifier of @bonus to rolls.', ['@group_name' => $title, '@gxp' => $rep, '@bonus' => $bonus]);
    }
  }
  return $lines;
}

function vaxia_characters_may_see_rep(NodeInterface $character, $owner_id = NULL): bool {
  $account = \Drupal::currentUser();
  return $account->hasPermission('view reputation xp') || (($owner_id ?? $character->getOwnerId()) == $account->id() && $account->hasPermission('view own reputation xp'));
}

/**
 * Sheet tab: reputation details and "Add reputation".
 */
function vaxia_characters_rep_sheet(array &$form, NodeInterface $node) {
  if (vaxia_characters_may_see_rep($node) && ($lines = vaxia_characters_rep_lines($node))) {
    $form['node_info']['rpg_rep'] = ['#type' => 'item', '#title' => t('Reputation details'), '#weight' => 20, 'list' => ['#theme' => 'item_list', '#items' => $lines]];
  }
  $account = \Drupal::currentUser();
  if ($account->hasPermission('give reputation xp') && !Character::isStoryteller($node) && !Character::isNpc($node) && $node->getOwnerId() != $account->id()) {
    $form['add_rpg_rep'] = ['#type' => 'details', '#title' => t('Add reputation'), '#open' => TRUE];
    $form['add_rpg_rep']['rpg_rep_amount'] = ['#type' => 'textfield', '#title' => t('Add reputation'), '#size' => 6, '#description' => t('Adding a negative value will subtract reputation.')];
    $form['add_rpg_rep']['rpg_rep_group'] = ['#type' => 'select', '#options' => vaxia_characters_rep_groups($node), '#title' => t('For group'), '#description' => t('Select the group to add reputation for.')];
    $form['add_rpg_rep']['add_rpg_rep_submit'] = [
      '#type' => 'submit',
      '#value' => t('Add reputation'),
      '#validate' => ['vaxia_characters_rep_validate'],
      '#submit' => ['vaxia_characters_rep_submit'],
    ];
  }
}

function vaxia_characters_rep_validate(array &$form, FormStateInterface $form_state) {
  $amount = $form_state->getValue('rpg_rep_amount');
  if (empty($amount)) {
    $form_state->setErrorByName('rpg_rep_amount', t('Empty value given for reputation.'));
  }
  if (!is_numeric($amount)) {
    $form_state->setErrorByName('rpg_rep_amount', t('Reputation given is not a numeric value'));
  }
}

function vaxia_characters_rep_submit(array &$form, FormStateInterface $form_state) {
  $node = Node::load($form_state->get('nid'));
  $gid = (int) $form_state->getValue('rpg_rep_group');
  $groups = vaxia_characters_rep_groups($node);
  $amount = (float) $form_state->getValue('rpg_rep_amount');
  if (!isset($groups[$gid]) || $amount == 0) {
    return;
  }
  $xp = \Drupal::service('vaxia_core.xp');
  $xp->update((int) $node->id(), $amount, 'rep_' . $gid);
  $args = ['@xp' => $amount, '@group' => $groups[$gid], '@title' => $node->label(), '@user' => \Drupal::currentUser()->getAccountName()];
  \Drupal::messenger()->addStatus($amount > 0 ? t('@xp reputation for @group has been added to this sheet.', $args) : t('@xp reputation for @group has been removed from this sheet.', $args));
  // D7 logged with an undefined $character (known bug).
  $xp->log((int) $node->id(), 'rep_' . $gid, (string) t('@xp reputation for @group added to @title by @user.', $args));
}

/**
 * Implements hook_ENTITY_TYPE_view() for comments: reputation on chat posts.
 */
function vaxia_characters_comment_view(array &$build, CommentInterface $comment, EntityViewDisplayInterface $display, $view_mode) {
  $character = $comment->hasField('field_comment_character') ? $comment->get('field_comment_character')->entity : NULL;
  if (!Character::isSheet($character) || !vaxia_characters_may_see_rep($character, $comment->getOwnerId())) {
    return;
  }
  if ($lines = vaxia_characters_rep_lines($character)) {
    $build['rpg_rep'] = [
      // D7's collapsible fieldset, collapsed: a closed details element.
      '#type' => 'details',
      '#open' => FALSE,
      '#title' => t('Reputation'),
      '#attributes' => ['class' => ['rpg-reputation', 'collapsible', 'collapsed']],
      'amounts' => ['#theme' => 'item_list', '#items' => $lines],
      '#weight' => 90,
    ];
  }
  $build['#cache']['max-age'] = 0;
}

/**
 * Monthly drift toward zero, on the 1st (D7 rpg_rep_cron()).
 */
function vaxia_characters_rep_cron() {
  $state = \Drupal::state();
  if ((int) $state->get('vaxia_cron.rpg_rep_sheet_cron', 0) >= strtotime('-1 day')) {
    return;
  }
  $state->set('vaxia_cron.rpg_rep_sheet_cron', time());
  if (date('d') !== '01') {
    return;
  }
  $xp = \Drupal::service('vaxia_core.xp');
  $storage = \Drupal::entityTypeManager()->getStorage('node');
  foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(FALSE)->condition('type', Character::TYPES, 'IN')->condition('status', 1)->execute()) as $node) {
    if (Character::isNpc($node) || Character::isStoryteller($node) || !Character::isPlayable($node)) {
      continue;
    }
    foreach (vaxia_characters_rep_groups($node) as $gid => $group) {
      $rep = $xp->get((int) $node->id(), 'rep_' . $gid);
      if ($rep != 0) {
        $change = $rep > 0 ? -1 : 1;
        $xp->update((int) $node->id(), $change, 'rep_' . $gid);
        $xp->log((int) $node->id(), 'monthly_rpg_rep', 'Reputation changed by ' . $change . ' for group ' . $group . ', was ' . $rep);
      }
    }
  }
}
