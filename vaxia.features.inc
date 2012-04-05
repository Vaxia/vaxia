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
  return array("version" => "3.0");
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
 * Implements hook_workflow_default_workflows().
 */
function vaxia_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Art evaluation workflow
  $workflows['Art evaluation workflow'] = array(
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
        'roles' => 'workflow_features_author_name,authenticated user',
        'state' => '(creation)',
        'target_state' => 'draft',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,authenticated user',
        'state' => 'draft',
        'target_state' => 'submitted',
      ),
      2 => array(
        'roles' => 'art-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'draft',
      ),
      3 => array(
        'roles' => 'art-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'approved',
      ),
      4 => array(
        'roles' => 'art-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'disapproved',
      ),
      5 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      6 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'submitted',
      ),
      7 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'disapproved',
      ),
      8 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'draft',
      ),
      9 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'submitted',
      ),
      10 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'approved',
      ),
    ),
    'node_types' => array(
      0 => 'artwork',
    ),
  );

  // Exported workflow: Character evaluation workflow
  $workflows['Character evaluation workflow'] = array(
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
        'roles' => 'workflow_features_author_name,authenticated user',
        'state' => '(creation)',
        'target_state' => 'draft',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,authenticated user',
        'state' => 'draft',
        'target_state' => 'submitted',
      ),
      2 => array(
        'roles' => 'char-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'draft',
      ),
      3 => array(
        'roles' => 'char-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'approved',
      ),
      4 => array(
        'roles' => 'char-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'disapproved',
      ),
      5 => array(
        'roles' => 'char-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'inactive',
      ),
      6 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      7 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'submitted',
      ),
      8 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'disapproved',
      ),
      9 => array(
        'roles' => 'workflow_features_author_name,char-evaluator,administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'inactive',
      ),
      10 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'draft',
      ),
      11 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'submitted',
      ),
      12 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'approved',
      ),
      13 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'inactive',
      ),
      14 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'inactive',
        'target_state' => 'draft',
      ),
      15 => array(
        'roles' => 'char-evaluator,administrator-vaxia',
        'state' => 'inactive',
        'target_state' => 'submitted',
      ),
      16 => array(
        'roles' => 'char-evaluator,administrator-vaxia',
        'state' => 'inactive',
        'target_state' => 'approved',
      ),
      17 => array(
        'roles' => 'char-evaluator,administrator-vaxia',
        'state' => 'inactive',
        'target_state' => 'disapproved',
      ),
    ),
    'node_types' => array(
      0 => 'character_sheet',
    ),
  );

  // Exported workflow: Item evaluation workflow
  $workflows['Item evaluation workflow'] = array(
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
        'roles' => 'workflow_features_author_name,authenticated user',
        'state' => '(creation)',
        'target_state' => 'draft',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,authenticated user',
        'state' => 'draft',
        'target_state' => 'submitted',
      ),
      2 => array(
        'roles' => 'item-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'draft',
      ),
      3 => array(
        'roles' => 'item-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'approved',
      ),
      4 => array(
        'roles' => 'item-evaluator,administrator-vaxia',
        'state' => 'submitted',
        'target_state' => 'disapproved',
      ),
      5 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      6 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'submitted',
      ),
      7 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'approved',
        'target_state' => 'disapproved',
      ),
      8 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'draft',
      ),
      9 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'submitted',
      ),
      10 => array(
        'roles' => 'administrator-vaxia',
        'state' => 'disapproved',
        'target_state' => 'approved',
      ),
    ),
    'node_types' => array(
      0 => 'items',
    ),
  );

  return $workflows;
}
