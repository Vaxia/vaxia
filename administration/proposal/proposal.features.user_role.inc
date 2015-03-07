<?php
/**
 * @file
 * proposal.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function proposal_user_default_roles() {
  $roles = array();

  // Exported role: voter.
  $roles['voter'] = array(
    'name' => 'voter',
    'weight' => 11,
  );

  return $roles;
}
