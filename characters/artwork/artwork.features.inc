<?php
/**
 * @file
 * artwork.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function artwork_ctools_plugin_api($module = NULL, $api = NULL) {
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
function artwork_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function artwork_node_info() {
  $items = array(
    'artwork' => array(
      'name' => t('Artwork'),
      'base' => 'node_content',
      'description' => t('Artwork for character sheets'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Step 1: When creating artwork we are under Fair Use policies here. In general, we also ask that you keep the art you use for your characters at the PG-13 level. Vaxia admin reserve the right to tighten down on the approval for art if the freedom is abused.
<br/><br/>
Step 2: Save, when ready to submit change the workflow to approved to activate the art for the site.'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function artwork_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Art evaluation
  $workflows['Art evaluation'] = array(
    'name' => 'Art evaluation',
    'tab_roles' => 'author,3',
    'options' => 'a:4:{s:16:"comment_log_node";s:1:"1";s:15:"comment_log_tab";s:1:"1";s:13:"name_as_title";s:1:"1";s:12:"watchdog_log";s:1:"1";}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => -50,
        'sysid' => 1,
        'status' => 1,
        'name' => 'Art evaluation',
      ),
      1 => array(
        'state' => 'approved',
        'weight' => -19,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Art evaluation',
      ),
      2 => array(
        'state' => 'removed from site',
        'weight' => -18,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Art evaluation',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'approved',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,administrator,lead',
        'state' => 'approved',
        'target_state' => 'removed from site',
      ),
      2 => array(
        'roles' => 'administrator',
        'state' => 'removed from site',
        'target_state' => 'approved',
      ),
    ),
    'node_types' => array(
      0 => 'artwork',
    ),
  );

  return $workflows;
}
