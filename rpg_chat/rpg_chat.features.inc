<?php
/**
 * @file
 * rpg_chat.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function rpg_chat_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function rpg_chat_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function rpg_chat_node_info() {
  $items = array(
    'rpg_chatroom' => array(
      'name' => t('RPG Chatroom'),
      'base' => 'node_content',
      'description' => t('RPG chatroom'),
      'has_title' => '1',
      'title_label' => t('Room name'),
      'help' => '',
    ),
  );
  return $items;
}
