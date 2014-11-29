<?php
/**
 * @file
 * sh_test.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function sh_test_taxonomy_default_vocabularies() {
  return array(
    'ash_test' => array(
      'name' => 'ASH Test',
      'machine_name' => 'ash_test',
      'description' => 'Taxonomy for tests',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-9',
    ),
    'sagas' => array(
      'name' => 'Sagas',
      'machine_name' => 'sagas',
      'description' => 'Saga labels for session reports.',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-8',
    ),
  );
}
