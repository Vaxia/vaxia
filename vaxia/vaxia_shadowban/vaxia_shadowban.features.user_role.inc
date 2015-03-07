<?php
/**
 * @file
 * vaxia_shadowban.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function vaxia_shadowban_user_default_roles() {
  $roles = array();

  // Exported role: shadowbanned user.
  $roles['shadowbanned user'] = array(
    'name' => 'shadowbanned user',
    'weight' => 25,
  );

  return $roles;
}
