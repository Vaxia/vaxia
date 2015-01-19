<?php
/**
 * @file
 * vaxia_elections.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function vaxia_elections_user_default_roles() {
  $roles = array();

  // Exported role: lead.
  $roles['lead'] = array(
    'name' => 'lead',
    'weight' => '7',
  );

  // Exported role: starvation-mode-ash-approver.
  $roles['starvation-mode-ash-approver'] = array(
    'name' => 'starvation-mode-ash-approver',
    'weight' => '15',
  );

  return $roles;
}
