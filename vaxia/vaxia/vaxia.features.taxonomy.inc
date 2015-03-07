<?php
/**
 * @file
 * vaxia.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function vaxia_taxonomy_default_vocabularies() {
  return array(
    'character_limits' => array(
      'name' => 'Character limits',
      'machine_name' => 'character_limits',
      'description' => 'Character limits',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
