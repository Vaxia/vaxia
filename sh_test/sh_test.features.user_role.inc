<?php
/**
 * @file
 * sh_test.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function sh_test_user_default_roles() {
  $roles = array();

  // Exported role: storyteller-assistant.
  $roles['storyteller-assistant'] = array(
    'name' => 'storyteller-assistant',
    'weight' => '6',
  );

  // Exported role: storyteller-candidate.
  $roles['storyteller-candidate'] = array(
    'name' => 'storyteller-candidate',
    'weight' => '7',
  );

  // Exported role: storyteller-newbie.
  $roles['storyteller-newbie'] = array(
    'name' => 'storyteller-newbie',
    'weight' => '5',
  );

  return $roles;
}
