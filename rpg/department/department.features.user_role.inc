<?php
/**
 * @file
 * department.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function department_user_default_roles() {
  $roles = array();

  // Exported role: knows-the-setting.
  $roles['knows-the-setting'] = array(
    'name' => 'knows-the-setting',
    'weight' => '10',
  );

  // Exported role: knows-the-social.
  $roles['knows-the-social'] = array(
    'name' => 'knows-the-social',
    'weight' => '9',
  );

  // Exported role: newbie-helper.
  $roles['newbie-helper'] = array(
    'name' => 'newbie-helper',
    'weight' => '12',
  );

  return $roles;
}
