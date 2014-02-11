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

  // Exported role: player.
  $roles['player'] = array(
    'name' => 'player',
    'weight' => '2',
  );

  return $roles;
}
