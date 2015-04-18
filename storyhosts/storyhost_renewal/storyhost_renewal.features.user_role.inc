<?php
/**
 * @file
 * storyhost_renewal.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function storyhost_renewal_user_default_roles() {
  $roles = array();

  // Exported role: evaluator-expired.
  $roles['evaluator-expired'] = array(
    'name' => 'evaluator-expired',
    'weight' => 22,
  );

  // Exported role: storyteller-expired.
  $roles['storyteller-expired'] = array(
    'name' => 'storyteller-expired',
    'weight' => 21,
  );

  return $roles;
}
