<?php
/**
 * @file
 * vaxia_sh_test.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function vaxia_sh_test_taxonomy_default_vocabularies() {
  return array(
    'ash_test' => array(
      'name' => 'ASH Test',
      'machine_name' => 'ash_test',
      'description' => 'Taxonomy for tests',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
