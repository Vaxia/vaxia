<?php
/**
 * @file
 * site_wiki.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function site_wiki_taxonomy_default_vocabularies() {
  return array(
    'department' => array(
      'name' => 'Department',
      'machine_name' => 'department',
      'description' => 'Select a department for this article.',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-6',
    ),
    'location_depth' => array(
      'name' => 'Location Type',
      'machine_name' => 'location_depth',
      'description' => 'Location Type',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-3',
    ),
    'organization_type' => array(
      'name' => 'Organization Type',
      'machine_name' => 'organization_type',
      'description' => 'Organization Type',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-2',
    ),
    'realm' => array(
      'name' => 'Realm',
      'machine_name' => 'realm',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-5',
    ),
    'setting_type' => array(
      'name' => 'Setting Type',
      'machine_name' => 'setting_type',
      'description' => 'Type of sitting article',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-4',
    ),
    'wiki_tags' => array(
      'name' => 'Categories',
      'machine_name' => 'wiki_tags',
      'description' => 'Use tags to group articles on similar topics into categories.',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '-7',
    ),
  );
}
