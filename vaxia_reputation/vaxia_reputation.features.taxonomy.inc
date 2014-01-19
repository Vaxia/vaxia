<?php
/**
 * @file
 * vaxia_reputation.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function vaxia_reputation_taxonomy_default_vocabularies() {
  return array(
    'userpoints' => array(
      'name' => 'Userpoints',
      'machine_name' => 'userpoints',
      'description' => 'Automatically created by the userpoints module',
      'hierarchy' => '1',
      'module' => 'userpoints',
      'weight' => '0',
    ),
  );
}
