<?php
/**
 * @file
 * rpg_site_staff.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function rpg_site_staff_user_default_roles() {
  $roles = array();

  // Exported role: head.
  $roles['head'] = array(
    'name' => 'head',
    'weight' => '12',
  );

  // Exported role: knows-the-system.
  $roles['knows-the-system'] = array(
    'name' => 'knows-the-system',
    'weight' => '18',
  );

  return $roles;
}
