<?php
/**
 * @file
 * strikes.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function strikes_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function strikes_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function strikes_node_info() {
  $items = array(
    'strike' => array(
      'name' => t('Strike'),
      'base' => 'node_content',
      'description' => t('A strike is a disciplinary action taken against a player for a documented on-site offense.'),
      'has_title' => '1',
      'title_label' => t('(Date) - Short summary'),
      'help' => t('STOP. If you are building a strike against a player on this site, make very very sure what you are doing qualifies under a strike condition. Abuse of strikes is a strike itself.'),
    ),
  );
  return $items;
}
