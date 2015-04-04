<?php
/**
 * @file
 * scene_record.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function scene_record_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function scene_record_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function scene_record_node_info() {
  $items = array(
    'scene' => array(
      'name' => t('Scene'),
      'base' => 'node_content',
      'description' => t('A scene to record.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Need help using this? We have a help page! Check it: <a href="http://vaxia.org/wiki/scene-creation-guide" target="_blank">Scene Creation Guide</a>.

When you\'re ready for a Storyhost to evaluate your scene, set it to submitted and save the scene. Don\'t forget to save and submit if you want credit for the scene! With this system we no longer need you to link to the scene in the forums - which is useful for things you\'d rather keep private.'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function scene_record_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Scene evaluation
  $workflows['Scene evaluation'] = array(
    'name' => 'Scene evaluation',
    'tab_roles' => 'author,3,9',
    'options' => 'a:4:{s:16:"comment_log_node";s:1:"1";s:15:"comment_log_tab";s:1:"1";s:13:"name_as_title";s:1:"1";s:12:"watchdog_log";s:1:"1";}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => -50,
        'sysid' => 1,
        'status' => 1,
        'name' => 'Scene evaluation',
      ),
      1 => array(
        'state' => 'draft',
        'weight' => -20,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Scene evaluation',
      ),
      2 => array(
        'state' => 'submitted',
        'weight' => -20,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Scene evaluation',
      ),
      3 => array(
        'state' => 'ruled',
        'weight' => -20,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Scene evaluation',
      ),
      4 => array(
        'state' => 'not for submission',
        'weight' => -20,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Scene evaluation',
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
        'target_state' => 'ruled',
      ),
      3 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => '(creation)',
        'target_state' => 'not for submission',
      ),
      4 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => 'draft',
        'target_state' => 'submitted',
      ),
      5 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'ruled',
      ),
      6 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => 'draft',
        'target_state' => 'not for submission',
      ),
      7 => array(
        'roles' => 'workflow_features_author_name,administrator,storyteller',
        'state' => 'submitted',
        'target_state' => 'draft',
      ),
      8 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'submitted',
        'target_state' => 'ruled',
      ),
      9 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => 'submitted',
        'target_state' => 'not for submission',
      ),
      10 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'ruled',
        'target_state' => 'draft',
      ),
      11 => array(
        'roles' => 'administrator,storyteller',
        'state' => 'ruled',
        'target_state' => 'submitted',
      ),
      12 => array(
        'roles' => 'administrator',
        'state' => 'ruled',
        'target_state' => 'not for submission',
      ),
      13 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => 'not for submission',
        'target_state' => 'draft',
      ),
      14 => array(
        'roles' => 'workflow_features_author_name,administrator',
        'state' => 'not for submission',
        'target_state' => 'submitted',
      ),
      15 => array(
        'roles' => 'administrator',
        'state' => 'not for submission',
        'target_state' => 'ruled',
      ),
    ),
    'node_types' => array(
      0 => 'scene',
    ),
  );

  return $workflows;
}
