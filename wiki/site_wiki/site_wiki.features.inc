<?php
/**
 * @file
 * site_wiki.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function site_wiki_ctools_plugin_api($module = NULL, $api = NULL) {
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
function site_wiki_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function site_wiki_node_info() {
  $items = array(
    'wiki_page' => array(
      'name' => t('Wiki page'),
      'base' => 'node_content',
      'description' => t('A page in the wiki'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function site_wiki_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Wiki workflow
  $workflows['Wiki workflow'] = array(
    'name' => 'Wiki workflow',
    'tab_roles' => '14,4,3',
    'options' => 'a:4:{s:16:"comment_log_node";s:1:"1";s:15:"comment_log_tab";s:1:"1";s:13:"name_as_title";s:1:"1";s:12:"watchdog_log";s:1:"1";}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => -50,
        'sysid' => 1,
        'status' => 1,
        'name' => 'Wiki workflow',
      ),
      1 => array(
        'state' => 'open',
        'weight' => 0,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Wiki workflow',
      ),
      2 => array(
        'state' => 'locked',
        'weight' => 0,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Wiki workflow',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'open',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'locked',
      ),
      2 => array(
        'roles' => 'administrator',
        'state' => 'open',
        'target_state' => 'locked',
      ),
      3 => array(
        'roles' => 'administrator',
        'state' => 'locked',
        'target_state' => 'open',
      ),
    ),
    'node_types' => array(
      0 => 'wiki_page',
    ),
  );

  return $workflows;
}
