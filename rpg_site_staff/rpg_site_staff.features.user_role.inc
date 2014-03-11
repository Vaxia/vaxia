<?php
/**
 * @file
 * rpg_site_staff.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function rpg_site_staff_user_default_roles() {
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

  // Exported role: heads.
  $roles['heads'] = array(
    'name' => 'heads',
    'weight' => '12',
  );

  // Exported role: storyteller.
  $roles['storyteller'] = array(
    'name' => 'storyteller',
    'weight' => '9',
  );

  // Exported role: storyteller-expired.
  $roles['storyteller-expired'] = array(
    'name' => 'storyteller-expired',
    'weight' => '8',
  );

  return $roles;
}
