<?php
/**
 * @file
 * vaxia.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vaxia_ctools_plugin_api() {
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
function vaxia_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function vaxia_node_info() {
  $items = array(
    'artwork' => array(
      'name' => t('Artwork'),
      'base' => 'node_content',
      'description' => t('Artwork for character sheets'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'character_sheet' => array(
      'name' => t('Character sheet'),
      'base' => 'node_content',
      'description' => t('Character sheet'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'items' => array(
      'name' => t('Items'),
      'base' => 'node_content',
      'description' => t('Character held items.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
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

/**
 * Implements hook_workflow_features_default_workflow().
 */
function vaxia_workflow_features_default_workflow() {
  return array(
    'Art evaluation workflow' => array(
      'name' => 'Art evaluation workflow',
      'tab_roles' => 'author,5,4',
      'options' => 'a:3:{s:16:"comment_log_node";i:1;s:15:"comment_log_tab";i:1;s:13:"name_as_title";i:1;}',
      'states' => array(
        0 => array(
          'state' => '(creation)',
          'weight' => '-50',
          'sysid' => '1',
          'status' => '1',
        ),
        1 => array(
          'state' => 'draft',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        2 => array(
          'state' => 'submitted',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        3 => array(
          'state' => 'approved',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        4 => array(
          'state' => 'disapproved',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
      ),
      'transitions' => array(
        0 => array(
          'sid' => '(creation)',
          'target_sid' => 'draft',
          'roles' => 'author,2',
        ),
        1 => array(
          'sid' => 'draft',
          'target_sid' => 'submitted',
          'roles' => 'author,2',
        ),
        2 => array(
          'sid' => 'submitted',
          'target_sid' => 'draft',
          'roles' => '5,4',
        ),
        3 => array(
          'sid' => 'submitted',
          'target_sid' => 'approved',
          'roles' => '5,4',
        ),
        4 => array(
          'sid' => 'submitted',
          'target_sid' => 'disapproved',
          'roles' => '5,4',
        ),
        5 => array(
          'sid' => 'approved',
          'target_sid' => 'draft',
          'roles' => '4',
        ),
        6 => array(
          'sid' => 'approved',
          'target_sid' => 'submitted',
          'roles' => '4',
        ),
        7 => array(
          'sid' => 'approved',
          'target_sid' => 'disapproved',
          'roles' => '4',
        ),
        8 => array(
          'sid' => 'disapproved',
          'target_sid' => 'draft',
          'roles' => '4',
        ),
        9 => array(
          'sid' => 'disapproved',
          'target_sid' => 'submitted',
          'roles' => '4',
        ),
        10 => array(
          'sid' => 'disapproved',
          'target_sid' => 'approved',
          'roles' => '4',
        ),
      ),
      'node_types' => array(
        0 => array(
          'type' => 'artwork',
          'wid' => '1',
        ),
      ),
    ),
    'Character evaluation workflow' => array(
      'name' => 'Character evaluation workflow',
      'tab_roles' => 'author,6,4',
      'options' => 'a:3:{s:16:"comment_log_node";i:1;s:15:"comment_log_tab";i:1;s:13:"name_as_title";i:1;}',
      'states' => array(
        0 => array(
          'state' => '(creation)',
          'weight' => '-50',
          'sysid' => '1',
          'status' => '1',
        ),
        1 => array(
          'state' => 'draft',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        2 => array(
          'state' => 'submitted',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        3 => array(
          'state' => 'approved',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        4 => array(
          'state' => 'disapproved',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        5 => array(
          'state' => 'inactive',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
      ),
      'transitions' => array(
        0 => array(
          'sid' => '(creation)',
          'target_sid' => 'draft',
          'roles' => 'author,2',
        ),
        1 => array(
          'sid' => 'draft',
          'target_sid' => 'submitted',
          'roles' => 'author,2',
        ),
        2 => array(
          'sid' => 'submitted',
          'target_sid' => 'draft',
          'roles' => '6,4',
        ),
        3 => array(
          'sid' => 'submitted',
          'target_sid' => 'approved',
          'roles' => '6,4',
        ),
        4 => array(
          'sid' => 'submitted',
          'target_sid' => 'disapproved',
          'roles' => '6,4',
        ),
        5 => array(
          'sid' => 'submitted',
          'target_sid' => 'inactive',
          'roles' => '6,4',
        ),
        6 => array(
          'sid' => 'approved',
          'target_sid' => 'draft',
          'roles' => '4',
        ),
        7 => array(
          'sid' => 'approved',
          'target_sid' => 'submitted',
          'roles' => '4',
        ),
        8 => array(
          'sid' => 'approved',
          'target_sid' => 'disapproved',
          'roles' => '4',
        ),
        9 => array(
          'sid' => 'approved',
          'target_sid' => 'inactive',
          'roles' => 'author,6,4',
        ),
        10 => array(
          'sid' => 'disapproved',
          'target_sid' => 'draft',
          'roles' => '4',
        ),
        11 => array(
          'sid' => 'disapproved',
          'target_sid' => 'submitted',
          'roles' => '4',
        ),
        12 => array(
          'sid' => 'disapproved',
          'target_sid' => 'approved',
          'roles' => '4',
        ),
        13 => array(
          'sid' => 'disapproved',
          'target_sid' => 'inactive',
          'roles' => '4',
        ),
        14 => array(
          'sid' => 'inactive',
          'target_sid' => 'draft',
          'roles' => '4',
        ),
        15 => array(
          'sid' => 'inactive',
          'target_sid' => 'submitted',
          'roles' => '6,4',
        ),
        16 => array(
          'sid' => 'inactive',
          'target_sid' => 'approved',
          'roles' => '6,4',
        ),
        17 => array(
          'sid' => 'inactive',
          'target_sid' => 'disapproved',
          'roles' => '6,4',
        ),
      ),
      'node_types' => array(
        0 => array(
          'type' => 'character_sheet',
          'wid' => '2',
        ),
      ),
    ),
    'Item evaluation workflow' => array(
      'name' => 'Item evaluation workflow',
      'tab_roles' => 'author,7,4',
      'options' => 'a:3:{s:16:"comment_log_node";i:1;s:15:"comment_log_tab";i:1;s:13:"name_as_title";i:1;}',
      'states' => array(
        0 => array(
          'state' => '(creation)',
          'weight' => '-50',
          'sysid' => '1',
          'status' => '1',
        ),
        1 => array(
          'state' => 'draft',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        2 => array(
          'state' => 'submitted',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        3 => array(
          'state' => 'approved',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
        4 => array(
          'state' => 'disapproved',
          'weight' => '0',
          'sysid' => '0',
          'status' => '1',
        ),
      ),
      'transitions' => array(
        0 => array(
          'sid' => '(creation)',
          'target_sid' => 'draft',
          'roles' => 'author,2',
        ),
        1 => array(
          'sid' => 'draft',
          'target_sid' => 'submitted',
          'roles' => 'author,2',
        ),
        2 => array(
          'sid' => 'submitted',
          'target_sid' => 'draft',
          'roles' => '7,4',
        ),
        3 => array(
          'sid' => 'submitted',
          'target_sid' => 'approved',
          'roles' => '7,4',
        ),
        4 => array(
          'sid' => 'submitted',
          'target_sid' => 'disapproved',
          'roles' => '7,4',
        ),
        5 => array(
          'sid' => 'approved',
          'target_sid' => 'draft',
          'roles' => '4',
        ),
        6 => array(
          'sid' => 'approved',
          'target_sid' => 'submitted',
          'roles' => '4',
        ),
        7 => array(
          'sid' => 'approved',
          'target_sid' => 'disapproved',
          'roles' => '4',
        ),
        8 => array(
          'sid' => 'disapproved',
          'target_sid' => 'draft',
          'roles' => '4',
        ),
        9 => array(
          'sid' => 'disapproved',
          'target_sid' => 'submitted',
          'roles' => '4',
        ),
        10 => array(
          'sid' => 'disapproved',
          'target_sid' => 'approved',
          'roles' => '4',
        ),
      ),
      'node_types' => array(
        0 => array(
          'type' => 'items',
          'wid' => '3',
        ),
      ),
    ),
  );
}
