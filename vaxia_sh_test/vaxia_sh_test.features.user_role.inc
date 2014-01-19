<?php
/**
 * @file
 * vaxia_sh_test.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function vaxia_sh_test_user_default_roles() {
  $roles = array();

  // Exported role: eligible for SH.
  $roles['eligible for SH'] = array(
    'name' => 'eligible for SH',
    'weight' => '2',
  );

  // Exported role: evaluator.
  $roles['evaluator'] = array(
    'name' => 'evaluator',
    'weight' => '9',
  );

  // Exported role: storyteller-candidate.
  $roles['storyteller-candidate'] = array(
    'name' => 'storyteller-candidate',
    'weight' => '6',
  );

  // Exported role: storyteller-expired.
  $roles['storyteller-expired'] = array(
    'name' => 'storyteller-expired',
    'weight' => '7',
  );

  // Exported role: storyteller-newbie.
  $roles['storyteller-newbie'] = array(
    'name' => 'storyteller-newbie',
    'weight' => '4',
  );

  return $roles;
}
