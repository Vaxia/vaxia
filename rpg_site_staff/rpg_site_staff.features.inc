<?php
/**
 * @file
 * rpg_site_staff.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function rpg_site_staff_ctools_plugin_api() {
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
function rpg_site_staff_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function rpg_site_staff_node_info() {
  $items = array(
    'session_report' => array(
      'name' => t('Session report'),
      'base' => 'node_content',
      'description' => t('For SHs after a session, to include a list of users, characters and provide a record of events.'),
      'has_title' => '1',
      'title_label' => t('Session name'),
      'help' => '',
    ),
  );
  return $items;
}
