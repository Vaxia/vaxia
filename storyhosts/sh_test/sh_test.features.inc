<?php
/**
 * @file
 * sh_test.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sh_test_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function sh_test_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function sh_test_node_info() {
  $items = array(
    'session_report' => array(
      'name' => t('Session report'),
      'base' => 'node_content',
      'description' => t('For SHs after a session, to include a list of users, characters and provide a record of events.'),
      'has_title' => '1',
      'title_label' => t('Session name'),
      'help' => '',
    ),
    'storyhost_application' => array(
      'name' => t('Storyhost Application'),
      'base' => 'node_content',
      'description' => t('An application to become a storyhost. Automatically created when a player passes ASH Test Three.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
