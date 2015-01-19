<?php
/**
 * @file
 * site_events.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function site_events_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function site_events_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function site_events_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('An event is a session, get together, or other event.'),
      'has_title' => '1',
      'title_label' => t('Subject'),
      'help' => '',
    ),
  );
  return $items;
}
