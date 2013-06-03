<?php
/**
 * @file
 * vaxia.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function vaxia_user_default_roles() {
  $roles = array();

  // Exported role: administrator-vaxia.
  $roles['administrator-vaxia'] = array(
    'name' => 'administrator-vaxia',
    'weight' => '9',
  );

  // Exported role: evaluator.
  $roles['evaluator'] = array(
    'name' => 'evaluator',
    'weight' => '7',
  );

  // Exported role: player.
  $roles['player'] = array(
    'name' => 'player',
    'weight' => '3',
  );

  // Exported role: storyteller.
  $roles['storyteller'] = array(
    'name' => 'storyteller',
    'weight' => '4',
  );

  // Exported role: storyteller-assistant.
  $roles['storyteller-assistant'] = array(
    'name' => 'storyteller-assistant',
    'weight' => '5',
  );

  return $roles;
}
