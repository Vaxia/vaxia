<?php
/**
 * @file
 * vaxia_wiki.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vaxia_wiki_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function vaxia_wiki_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function vaxia_wiki_flag_default_flags() {
  $flags = array();
  // Exported flag: "Delete".
  $flags['delete'] = array(
    'entity_type' => 'node',
    'title' => 'Delete',
    'global' => 1,
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
    'module' => 'vaxia_wiki',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Expand".
  $flags['expand'] = array(
    'entity_type' => 'node',
    'title' => 'Expand',
    'global' => 1,
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
    'module' => 'vaxia_wiki',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Rewrite".
  $flags['rewrite'] = array(
    'entity_type' => 'node',
    'title' => 'Rewrite',
    'global' => 1,
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
    'module' => 'vaxia_wiki',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}
