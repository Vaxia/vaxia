<?php
/**
 * @file
 * site_wiki.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function site_wiki_ctools_plugin_api() {
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
function site_wiki_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function site_wiki_flag_default_flags() {
  $flags = array();
  // Exported flag: "Delete".
  $flags['delete'] = array(
    'entity_type' => 'node',
    'title' => 'Delete',
    'global' => '1',
    'types' => array(
      0 => 'wiki_page',
    ),
    'flag_short' => 'Mark to delete',
    'flag_long' => 'Mark this page for deletion',
    'flag_message' => 'This wiki page has been marked for deletion.',
    'unflag_short' => 'Don\'t delete',
    'unflag_long' => 'Un-mark this page for deletion',
    'unflag_message' => 'This wiki page has been un-marked for deletion.',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 'full',
      'teaser' => 'teaser',
      'rss' => 0,
      'search_index' => 0,
      'search_result' => 0,
      'ical' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'artwork' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'module' => 'site_wiki',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Expand".
  $flags['expand'] = array(
    'entity_type' => 'node',
    'title' => 'Expand',
    'global' => '1',
    'types' => array(
      0 => 'wiki_page',
    ),
    'flag_short' => 'Mark to Expand',
    'flag_long' => 'Mark this page as needing more information',
    'flag_message' => 'This page has been marked to be expanded on with more information',
    'unflag_short' => 'Don\'t expand',
    'unflag_long' => 'Un-mark this page for expansion',
    'unflag_message' => 'This page has been un-marked for expansion',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 'full',
      'teaser' => 'teaser',
      'rss' => 0,
      'search_index' => 0,
      'search_result' => 0,
      'ical' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'artwork' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'module' => 'site_wiki',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Rewrite".
  $flags['rewrite'] = array(
    'entity_type' => 'node',
    'title' => 'Rewrite',
    'global' => '1',
    'types' => array(
      0 => 'wiki_page',
    ),
    'flag_short' => 'Mark to Rewrite',
    'flag_long' => 'Mark this page for rewrite',
    'flag_message' => 'This page has been marked for rewriting to address spelling, grammar or tone issues.',
    'unflag_short' => 'Don\'t rewrite',
    'unflag_long' => 'Un-mark this page for rewrite',
    'unflag_message' => 'This page has been un-marked for rewriting',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'full' => 'full',
      'teaser' => 'teaser',
      'rss' => 0,
      'search_index' => 0,
      'search_result' => 0,
      'ical' => 0,
      'diff_standard' => 0,
      'token' => 0,
      'artwork' => 0,
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'module' => 'site_wiki',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

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
        'weight' => '-50',
        'sysid' => '1',
        'status' => '1',
        'name' => 'Wiki workflow',
      ),
      1 => array(
        'state' => 'open',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Wiki workflow',
      ),
      2 => array(
        'state' => 'locked',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Wiki workflow',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name,evaluator',
        'state' => '(creation)',
        'target_state' => 'open',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,evaluator',
        'state' => '(creation)',
        'target_state' => 'locked',
      ),
      2 => array(
        'roles' => 'administrator,evaluator',
        'state' => 'open',
        'target_state' => 'locked',
      ),
      3 => array(
        'roles' => 'administrator,evaluator',
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
