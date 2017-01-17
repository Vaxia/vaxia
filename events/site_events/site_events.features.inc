<?php
/**
 * @file
 * site_events.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function site_events_ctools_plugin_api($module = NULL, $api = NULL) {
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
function site_events_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
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
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function site_events_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Event status
  $workflows['Event status'] = array(
    'name' => 'Event status',
    'tab_roles' => 'author,3,21',
    'options' => 'a:4:{s:16:"comment_log_node";s:1:"0";s:15:"comment_log_tab";s:1:"0";s:13:"name_as_title";s:1:"1";s:12:"watchdog_log";s:1:"0";}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => -50,
        'sysid' => 1,
        'status' => 1,
        'name' => 'Event status',
      ),
      1 => array(
        'state' => 'scheduled',
        'weight' => -20,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Event status',
      ),
      2 => array(
        'state' => 'cancelled',
        'weight' => -20,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Event status',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'scheduled',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,administrator,lead',
        'state' => 'scheduled',
        'target_state' => 'cancelled',
      ),
      2 => array(
        'roles' => 'workflow_features_author_name,administrator,lead',
        'state' => 'cancelled',
        'target_state' => 'scheduled',
      ),
    ),
    'node_types' => array(
      0 => 'event',
    ),
  );

  return $workflows;
}
