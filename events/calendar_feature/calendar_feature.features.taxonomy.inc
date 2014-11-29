<?php
/**
 * @file
 * calendar_feature.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function calendar_feature_taxonomy_default_vocabularies() {
  return array(
    'content' => array(
      'name' => 'Content warnings',
      'machine_name' => 'content',
      'description' => 'Content warnings',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-1',
    ),
    'forums' => array(
      'name' => 'Forums',
      'machine_name' => 'forums',
      'description' => 'Forum navigation vocabulary',
      'hierarchy' => '1',
      'module' => 'forum',
      'weight' => '-10',
    ),
  );
}
