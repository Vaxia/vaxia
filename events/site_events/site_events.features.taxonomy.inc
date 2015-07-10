<?php
/**
 * @file
 * site_events.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function site_events_taxonomy_default_vocabularies() {
  return array(
    'content' => array(
      'name' => 'Events: Content',
      'machine_name' => 'content',
      'description' => 'Events: Content',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => -1,
    ),
  );
}
