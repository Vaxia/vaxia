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
    'weight' => '12',
  );

  // Exported role: player.
  $roles['player'] = array(
    'name' => 'player',
    'weight' => '2',
  );

  // Exported role: storyteller.
  $roles['storyteller'] = array(
    'name' => 'storyteller',
    'weight' => '9',
  );

  // Exported role: storyteller-assistant.
  $roles['storyteller-assistant'] = array(
    'name' => 'storyteller-assistant',
    'weight' => '6',
  );

  return $roles;
}
