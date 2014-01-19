<?php
/**
 * @file
 * calendar_feature.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function calendar_feature_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'calendar_feed';
  $view->description = 'Calendar feed for export to other systems';
  $view->tag = 'Calendar';
  $view->base_table = 'node';
  $view->human_name = 'Calendar Feed';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
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
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_when']['id'] = 'field_when';
  $handler->display->display_options['fields']['field_when']['table'] = 'field_data_field_when';
  $handler->display->display_options['fields']['field_when']['field'] = 'field_when';
  $handler->display->display_options['fields']['field_when']['label'] = '';
  $handler->display->display_options['fields']['field_when']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_when']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
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
    'event' => 'event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: When -  start date (field_when) */
  $handler->display->display_options['filters']['field_when_value']['id'] = 'field_when_value';
  $handler->display->display_options['filters']['field_when_value']['table'] = 'field_data_field_when';
  $handler->display->display_options['filters']['field_when_value']['field'] = 'field_when_value';
  $handler->display->display_options['filters']['field_when_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_when_value']['default_date'] = 'first day of this month';
  $handler->display->display_options['filters']['field_when_value']['year_range'] = '-0:+1';

  /* Display: Calendar Feed */
  $handler = $view->new_display('feed', 'Calendar Feed', 'calendar_feed');
  $handler->display->display_options['display_description'] = 'Calendar feed for export to other systems';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'date_ical';
  $handler->display->display_options['style_options']['cal_name'] = 'Events';
  $handler->display->display_options['row_plugin'] = 'date_ical';
  $handler->display->display_options['row_options']['date_field'] = 'field_data_field_when.field_when_value';
  $handler->display->display_options['row_options']['summary_field'] = 'default_title';
  $handler->display->display_options['row_options']['location_field'] = 'none';
  $handler->display->display_options['path'] = 'calendar/ical';
  $export['calendar_feed'] = $view;

  return $export;
}
