<?php
/**
 * @file
 * vaxia_sh_test.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vaxia_sh_test_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function vaxia_sh_test_node_info() {
  $items = array(
    'storyhost_application' => array(
      'name' => t('Storyhost Application'),
      'base' => 'node_content',
      'description' => t('An application to become a storyhost. Automatically created when a player passes ASH Test Three.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}