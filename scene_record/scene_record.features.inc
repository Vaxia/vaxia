<?php
/**
 * @file
 * scene_record.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function scene_record_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function scene_record_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function scene_record_node_info() {
  $items = array(
    'scene' => array(
      'name' => t('Scene'),
      'base' => 'node_content',
      'description' => t('A scene to record.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Need help using this? We have a help page! Check it: <a href="http://vaxia.org/wiki/scene-creation-guide" target="_blank">Scene Creation Guide</a>.'),
    ),
  );
  return $items;
}
