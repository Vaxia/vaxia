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

  // Exported role: eligible for SH.
  $roles['eligible for SH'] = array(
    'name' => 'eligible for SH',
    'weight' => 14,
  );

  // Exported role: evaluator.
  $roles['evaluator'] = array(
    'name' => 'evaluator',
    'weight' => 6,
  );

  // Exported role: storyteller.
  $roles['storyteller'] = array(
    'name' => 'storyteller',
    'weight' => 5,
  );

  // Exported role: storyteller-assistant.
  $roles['storyteller-assistant'] = array(
    'name' => 'storyteller-assistant',
    'weight' => 19,
  );

  // Exported role: storyteller-candidate.
  $roles['storyteller-candidate'] = array(
    'name' => 'storyteller-candidate',
    'weight' => 20,
  );

  // Exported role: storyteller-newbie.
  $roles['storyteller-newbie'] = array(
    'name' => 'storyteller-newbie',
    'weight' => 18,
  );

  return $roles;
}
