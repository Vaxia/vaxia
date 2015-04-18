<?php
/**
 * @file
 * suspend_players.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function suspend_players_user_default_roles() {
  $roles = array();

  // Exported role: suspended user.
  $roles['suspended user'] = array(
    'name' => 'suspended user',
    'weight' => 26,
  );

  return $roles;
}
