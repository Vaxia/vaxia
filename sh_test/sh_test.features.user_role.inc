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
    'weight' => '4',
  );

  // Exported role: evaluator.
  $roles['evaluator'] = array(
    'name' => 'evaluator',
    'weight' => '11',
  );

  // Exported role: evaluator-expired.
  $roles['evaluator-expired'] = array(
    'name' => 'evaluator-expired',
    'weight' => '10',
  );

  // Exported role: storyteller.
  $roles['storyteller'] = array(
    'name' => 'storyteller',
    'weight' => '9',
  );

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

  // Exported role: storyteller-expired.
  $roles['storyteller-expired'] = array(
    'name' => 'storyteller-expired',
    'weight' => '8',
  );

  // Exported role: storyteller-newbie.
  $roles['storyteller-newbie'] = array(
    'name' => 'storyteller-newbie',
    'weight' => '5',
  );

  return $roles;
}
