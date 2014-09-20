<?php
/**
 * @file
 * wiki_feature.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function wiki_feature_taxonomy_default_vocabularies() {
  return array(
    'wiki_tags' => array(
      'name' => 'Categories',
      'machine_name' => 'wiki_tags',
      'description' => 'Use tags to group articles on similar topics into categories.',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
