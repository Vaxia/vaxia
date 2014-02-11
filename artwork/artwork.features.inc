<?php
/**
 * @file
 * artwork.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function artwork_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function artwork_views_api() {
  return array("version" => "3.0");
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
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function artwork_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Art evaluation workflow
  $workflows['Art evaluation workflow'] = array(
    'name' => 'Art evaluation workflow',
    'tab_roles' => 'author,4,3',
    'options' => 'a:4:{s:16:"comment_log_node";i:1;s:15:"comment_log_tab";i:1;s:13:"name_as_title";i:1;s:12:"watchdog_log";i:1;}',
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
        'state' => 'approved',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
      3 => array(
        'state' => 'removed from site',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'approved',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,administrator-vaxia,administrator',
        'state' => 'draft',
        'target_state' => 'approved',
      ),
      2 => array(
        'roles' => 'workflow_features_author_name,administrator-vaxia,administrator',
        'state' => 'draft',
        'target_state' => 'removed from site',
      ),
      3 => array(
        'roles' => 'workflow_features_author_name,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      4 => array(
        'roles' => 'workflow_features_author_name,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'removed from site',
      ),
      5 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'removed from site',
        'target_state' => 'draft',
      ),
      6 => array(
        'roles' => 'administrator-vaxia,administrator',
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
