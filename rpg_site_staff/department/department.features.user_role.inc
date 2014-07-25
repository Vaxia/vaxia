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
    'weight' => '20',
  );

  // Exported role: knows-the-social.
  $roles['knows-the-social'] = array(
    'name' => 'knows-the-social',
    'weight' => '19',
  );

  // Exported role: newbie-helper.
  $roles['newbie-helper'] = array(
    'name' => 'newbie-helper',
    'weight' => '21',
  );

  return $roles;
}
