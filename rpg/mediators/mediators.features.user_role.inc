<?php
/**
 * @file
 * mediators.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function mediators_user_default_roles() {
  $roles = array();

  // Exported role: mediator.
  $roles['mediator'] = array(
    'name' => 'mediator',
    'weight' => 13,
  );

  return $roles;
}
