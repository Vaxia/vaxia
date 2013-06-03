<?php
/**
 * @file
 * whiteboard_feature.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function whiteboard_feature_filter_default_formats() {
  $formats = array();

  // Exported format: Whiteboard.
  $formats['whiteboard'] = array(
    'format' => 'whiteboard',
    'name' => 'Whiteboard',
    'cache' => '1',
    'status' => '1',
    'weight' => '0',
    'filters' => array(),
  );

  return $formats;
}
