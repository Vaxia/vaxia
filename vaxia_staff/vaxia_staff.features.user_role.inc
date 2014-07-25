<?php
/**
 * @file
 * vaxia_staff.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function vaxia_staff_user_default_roles() {
  $roles = array();

  // Exported role: knows-the-system.
  $roles['knows-the-system'] = array(
    'name' => 'knows-the-system',
    'weight' => '18',
  );

  return $roles;
}
