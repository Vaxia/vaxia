<?php
/**
 * @file
 * ban_players.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function ban_players_user_default_roles() {
  $roles = array();

  // Exported role: banned user.
  $roles['banned user'] = array(
    'name' => 'banned user',
    'weight' => 25,
  );

  return $roles;
}
