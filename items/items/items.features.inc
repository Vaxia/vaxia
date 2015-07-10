<?php
/**
 * @file
 * items.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function items_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "corresponding_node_references" && $api == "default_corresponding_node_references_presets") {
    return array("version" => "1");
  }
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
function items_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function items_node_info() {
  $items = array(
    'items' => array(
      'name' => t('Item'),
      'base' => 'node_content',
      'description' => t('Character held items.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function items_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Item evaluation
  $workflows['Item evaluation'] = array(
    'name' => 'Item evaluation',
    'tab_roles' => 'author,3,9',
    'options' => 'a:4:{s:16:"comment_log_node";s:1:"1";s:15:"comment_log_tab";s:1:"1";s:13:"name_as_title";s:1:"1";s:12:"watchdog_log";s:1:"1";}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => -50,
        'sysid' => 1,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      1 => array(
        'state' => 'draft',
        'weight' => -19,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      2 => array(
        'state' => 'submitted',
        'weight' => -18,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      3 => array(
        'state' => 'approved',
        'weight' => -17,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      4 => array(
        'state' => 'disapproved',
        'weight' => -16,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      5 => array(
        'state' => 'needs recharge',
        'weight' => -15,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      6 => array(
        'state' => 'needs daily refresh',
        'weight' => -14,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      7 => array(
        'state' => 'removed from game',
        'weight' => -13,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => '(creation)',
        'target_state' => 'draft',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => '(creation)',
        'target_state' => 'submitted',
      ),
      2 => array(
        'roles' => 'administrator',
        'state' => '(creation)',
        'target_state' => 'approved',
      ),
      3 => array(
        'roles' => 'administrator',
        'state' => '(creation)',
        'target_state' => 'disapproved',
      ),
      4 => array(
        'roles' => 'administrator',
        'state' => '(creation)',
        'target_state' => 'needs recharge',
      ),
      5 => array(
        'roles' => 'administrator',
        'state' => '(creation)',
        'target_state' => 'needs daily refresh',
      ),
      6 => array(
        'roles' => 'administrator',
        'state' => '(creation)',
        'target_state' => 'removed from game',
      ),
      7 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => 'draft',
        'target_state' => 'submitted',
      ),
      8 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'approved',
      ),
      9 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'disapproved',
      ),
      10 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'needs recharge',
      ),
      11 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'needs daily refresh',
      ),
      12 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'removed from game',
      ),
      13 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'submitted',
        'target_state' => 'draft',
      ),
      14 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'submitted',
        'target_state' => 'approved',
      ),
      15 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'submitted',
        'target_state' => 'disapproved',
      ),
      16 => array(
        'roles' => 'administrator',
        'state' => 'submitted',
        'target_state' => 'needs recharge',
      ),
      17 => array(
        'roles' => 'administrator',
        'state' => 'submitted',
        'target_state' => 'removed from game',
      ),
      18 => array(
        'roles' => 'administrator',
        'state' => 'submitted',
        'target_state' => 'needs daily refresh',
      ),
      19 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      20 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'approved',
        'target_state' => 'submitted',
      ),
      21 => array(
        'roles' => 'administrator',
        'state' => 'approved',
        'target_state' => 'disapproved',
      ),
      22 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'approved',
        'target_state' => 'needs recharge',
      ),
      23 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'approved',
        'target_state' => 'removed from game',
      ),
      24 => array(
        'roles' => 'storyteller',
        'state' => 'approved',
        'target_state' => 'needs daily refresh',
      ),
      25 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'draft',
      ),
      26 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'submitted',
      ),
      27 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'approved',
      ),
      28 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'needs recharge',
      ),
      29 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'removed from game',
      ),
      30 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'needs daily refresh',
      ),
      31 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'draft',
      ),
      32 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'submitted',
      ),
      33 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'approved',
      ),
      34 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'disapproved',
      ),
      35 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'removed from game',
      ),
      36 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'needs daily refresh',
      ),
      37 => array(
        'roles' => 'administrator',
        'state' => 'needs daily refresh',
        'target_state' => 'draft',
      ),
      38 => array(
        'roles' => 'administrator',
        'state' => 'needs daily refresh',
        'target_state' => 'submitted',
      ),
      39 => array(
        'roles' => 'administrator',
        'state' => 'needs daily refresh',
        'target_state' => 'approved',
      ),
      40 => array(
        'roles' => 'administrator',
        'state' => 'needs daily refresh',
        'target_state' => 'disapproved',
      ),
      41 => array(
        'roles' => 'administrator',
        'state' => 'needs daily refresh',
        'target_state' => 'needs recharge',
      ),
      42 => array(
        'roles' => 'administrator',
        'state' => 'needs daily refresh',
        'target_state' => 'removed from game',
      ),
      43 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'draft',
      ),
      44 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'submitted',
      ),
      45 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'approved',
      ),
      46 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'disapproved',
      ),
      47 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'needs recharge',
      ),
      48 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'needs daily refresh',
      ),
    ),
    'node_types' => array(
      0 => 'items',
    ),
  );

  return $workflows;
}
