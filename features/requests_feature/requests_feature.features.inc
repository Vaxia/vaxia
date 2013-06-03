<?php
/**
 * @file
 * requests_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function requests_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function requests_feature_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function requests_feature_node_info() {
  $items = array(
    'feature_request' => array(
      'name' => t('Feature request'),
      'base' => 'node_content',
      'description' => t('Make a feature request'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}
