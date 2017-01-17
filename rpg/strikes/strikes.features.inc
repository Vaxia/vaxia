<?php
/**
 * @file
 * strikes.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function strikes_ctools_plugin_api($module = NULL, $api = NULL) {
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
function strikes_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function strikes_node_info() {
  $items = array(
    'notation' => array(
      'name' => t('Notation'),
      'base' => 'node_content',
      'description' => t('A record of interaction is used by social to keep track of interactions with the social team.'),
      'has_title' => '1',
      'title_label' => t('Short summary'),
      'help' => '',
    ),
    'strike' => array(
      'name' => t('Strike'),
      'base' => 'node_content',
      'description' => t('A strike is a disciplinary action taken against a player for a documented on-site offense.'),
      'has_title' => '1',
      'title_label' => t('Short summary'),
      'help' => t('STOP. If you are building a strike against a player on this site, make very very sure what you are doing qualifies under a strike condition. Abuse of strikes is a strike itself.'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function strikes_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Notation approval
  $workflows['Notation approval'] = array(
    'name' => 'Notation approval',
    'tab_roles' => '21,29',
    'options' => 'a:4:{s:16:"comment_log_node";s:1:"1";s:15:"comment_log_tab";s:1:"1";s:13:"name_as_title";s:1:"0";s:12:"watchdog_log";s:1:"0";}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => -50,
        'sysid' => 1,
        'status' => 1,
        'name' => 'Notation approval',
      ),
      1 => array(
        'state' => 'greenie / no strike',
        'weight' => -19,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Notation approval',
      ),
      2 => array(
        'state' => 'strike submitted',
        'weight' => -18,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Notation approval',
      ),
      3 => array(
        'state' => 'strike approved',
        'weight' => -17,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Notation approval',
      ),
      4 => array(
        'state' => 'strike NOT approved',
        'weight' => -16,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Notation approval',
      ),
      5 => array(
        'state' => 'greenie NOT approved',
        'weight' => -15,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Notation approval',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'strike submitted',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'greenie / no strike',
      ),
      2 => array(
        'roles' => 'administrator,lead',
        'state' => 'greenie / no strike',
        'target_state' => 'strike submitted',
      ),
      3 => array(
        'roles' => 'administrator,lead',
        'state' => 'greenie / no strike',
        'target_state' => 'strike approved',
      ),
      4 => array(
        'roles' => 'administrator,lead',
        'state' => 'greenie / no strike',
        'target_state' => 'strike NOT approved',
      ),
      5 => array(
        'roles' => 'administrator,lead',
        'state' => 'greenie / no strike',
        'target_state' => 'greenie NOT approved',
      ),
      6 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike submitted',
        'target_state' => 'strike approved',
      ),
      7 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike submitted',
        'target_state' => 'strike NOT approved',
      ),
      8 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike submitted',
        'target_state' => 'greenie / no strike',
      ),
      9 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike submitted',
        'target_state' => 'greenie NOT approved',
      ),
      10 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike approved',
        'target_state' => 'strike submitted',
      ),
      11 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike approved',
        'target_state' => 'strike NOT approved',
      ),
      12 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike approved',
        'target_state' => 'greenie / no strike',
      ),
      13 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike approved',
        'target_state' => 'greenie NOT approved',
      ),
      14 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike NOT approved',
        'target_state' => 'strike submitted',
      ),
      15 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike NOT approved',
        'target_state' => 'strike approved',
      ),
      16 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike NOT approved',
        'target_state' => 'greenie / no strike',
      ),
      17 => array(
        'roles' => 'administrator,lead',
        'state' => 'strike NOT approved',
        'target_state' => 'greenie NOT approved',
      ),
      18 => array(
        'roles' => 'administrator,lead',
        'state' => 'greenie NOT approved',
        'target_state' => 'greenie / no strike',
      ),
      19 => array(
        'roles' => 'administrator,lead',
        'state' => 'greenie NOT approved',
        'target_state' => 'strike submitted',
      ),
      20 => array(
        'roles' => 'administrator,lead',
        'state' => 'greenie NOT approved',
        'target_state' => 'strike approved',
      ),
      21 => array(
        'roles' => 'administrator,lead',
        'state' => 'greenie NOT approved',
        'target_state' => 'strike NOT approved',
      ),
    ),
    'node_types' => array(
      0 => 'notation',
    ),
  );

  return $workflows;
}
