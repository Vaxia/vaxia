<?php
/**
 * @file
 * cold_storage.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function cold_storage_user_default_roles() {
  $roles = array();

  // Exported role: cold storage.
  $roles['cold storage'] = array(
    'name' => 'cold storage',
    'weight' => '21',
  );

  return $roles;
}
