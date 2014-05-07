<?php
/**
 * @file
 * vaxia.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function vaxia_user_default_permissions() {
  $permissions = array();

  // Exported permission: edit any rpg_chatroom content.
  $permissions['edit any rpg_chatroom content'] = array(
    'name' => 'edit any rpg_chatroom content',
    'roles' => array(
      0 => 'administrator',
      1 => 'storyteller',
      2 => 'storyteller-assistant',
      3 => 'storyteller-candidate',
    ),
    'module' => 'node',
  );

  return $permissions;
}
