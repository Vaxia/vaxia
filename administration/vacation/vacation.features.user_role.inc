<?php
/**
 * @file
 * vacation.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function vacation_user_default_roles() {
  $roles = array();

  // Exported role: vacation mode.
  $roles['vacation mode'] = array(
    'name' => 'vacation mode',
    'weight' => '22',
  );

  return $roles;
}
