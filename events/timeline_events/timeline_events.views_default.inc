<?php
/**
 * @file
 * timeline_events.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function timeline_events_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'setting_timelines';
  $view->description = 'Setting Timelines';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Setting Timelines';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Setting Timelines';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'timelinejs';
  $handler->display->display_options['style_options']['timeline_theme']['width'] = '90';
  $handler->display->display_options['style_options']['timeline_fields']['date'] = 'field_session_held';
  $handler->display->display_options['style_options']['timeline_fields']['bodytext'] = 'body';
  $handler->display->display_options['style_options']['timeline_fields']['headline'] = 'title';
  $handler->display->display_options['style_options']['timeline_config']['hash_bookmark'] = '1';
  $handler->display->display_options['style_options']['timeline_config']['start_at_end'] = '1';
  $handler->display->display_options['style_options']['timeline_config']['start_zoom_adjust'] = '-2';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '300';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Session held */
  $handler->display->display_options['fields']['field_session_held']['id'] = 'field_session_held';
  $handler->display->display_options['fields']['field_session_held']['table'] = 'field_data_field_session_held';
  $handler->display->display_options['fields']['field_session_held']['field'] = 'field_session_held';
  $handler->display->display_options['fields']['field_session_held']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'session_report' => 'session_report',
    'timeline_event' => 'timeline_event',
  );
  /* Filter criterion: Content: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['value'] = array(
    725 => '725',
    726 => '726',
  );
  $handler->display->display_options['filters']['field_tag_realm_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['operator_id'] = 'field_tag_realm_tid_op';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['label'] = 'Realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['operator'] = 'field_tag_realm_tid_op';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['identifier'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    30 => 0,
    8 => 0,
    9 => 0,
    21 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    19 => 0,
    28 => 0,
    29 => 0,
    12 => 0,
    34 => 0,
    35 => 0,
    25 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    22 => 0,
    23 => 0,
    31 => 0,
    32 => 0,
  );
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['reduce'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['hierarchy'] = 1;

  /* Display: Setting Timelines */
  $handler = $view->new_display('page', 'Setting Timelines', 'timelines');
  $handler->display->display_options['display_description'] = 'Setting Timelines';
  $handler->display->display_options['path'] = 'wiki/timelines';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'wiki/timelines';
  $handler->display->display_options['menu']['name'] = 'menu-create-stuff';
  $export['setting_timelines'] = $view;

  return $export;
}
