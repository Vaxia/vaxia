<?php
/**
 * @file
 * rpg_site_staff.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function rpg_site_staff_taxonomy_default_vocabularies() {
  return array(
    'sagas' => array(
      'name' => 'Sagas',
      'machine_name' => 'sagas',
      'description' => 'Saga labels for session reports.',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
