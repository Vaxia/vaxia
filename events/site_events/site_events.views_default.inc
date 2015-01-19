<?php
/**
 * @file
 * site_events.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function site_events_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'site_events_feed';
  $view->description = 'Events feed for export to other systems';
  $view->tag = 'Site Events';
  $view->base_table = 'node';
  $view->human_name = 'Site Events Feed';
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
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
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
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = '';
  $handler->display->display_options['fields']['changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_when']['id'] = 'field_when';
  $handler->display->display_options['fields']['field_when']['table'] = 'field_data_field_when';
  $handler->display->display_options['fields']['field_when']['field'] = 'field_when';
  $handler->display->display_options['fields']['field_when']['label'] = '';
  $handler->display->display_options['fields']['field_when']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_when']['empty'] = '[changed]';
  $handler->display->display_options['fields']['field_when']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Sort criterion: Content: When -  start date (field_when) */
  $handler->display->display_options['sorts']['field_when_value']['id'] = 'field_when_value';
  $handler->display->display_options['sorts']['field_when_value']['table'] = 'field_data_field_when';
  $handler->display->display_options['sorts']['field_when_value']['field'] = 'field_when_value';
  $handler->display->display_options['sorts']['field_when_value']['order'] = 'DESC';
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
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
    'proposal' => 'proposal',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: When -  start date (field_when) */
  $handler->display->display_options['filters']['field_when_value']['id'] = 'field_when_value';
  $handler->display->display_options['filters']['field_when_value']['table'] = 'field_data_field_when';
  $handler->display->display_options['filters']['field_when_value']['field'] = 'field_when_value';
  $handler->display->display_options['filters']['field_when_value']['operator'] = '<=';
  $handler->display->display_options['filters']['field_when_value']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['field_when_value']['default_date'] = '+1 month';
  $handler->display->display_options['filters']['field_when_value']['year_range'] = '-1:+3';

  /* Display: Calendar Feed */
  $handler = $view->new_display('feed', 'Sites Event Feed', 'site_events_feed');
  $handler->display->display_options['display_description'] = 'Sites event feed for export to other systems';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'date_ical';
  $handler->display->display_options['style_options']['uses_fields'] = TRUE;
  $handler->display->display_options['style_options']['cal_name'] = 'Events';
  $handler->display->display_options['row_plugin'] = 'date_ical';
  $handler->display->display_options['row_options']['date_field'] = 'field_data_field_when.field_when_value';
  $handler->display->display_options['row_options']['summary_field'] = 'default_title';
  $handler->display->display_options['row_options']['location_field'] = 'none';
  $handler->display->display_options['path'] = 'calendar/ical';

  /* Display: Calendar Event List */
  $handler = $view->new_display('page', 'Sites Event List', 'site_events_list');
  $handler->display->display_options['display_description'] = 'Sites Event List';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = '';
  $handler->display->display_options['fields']['changed']['exclude'] = TRUE;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_when']['id'] = 'field_when';
  $handler->display->display_options['fields']['field_when']['table'] = 'field_data_field_when';
  $handler->display->display_options['fields']['field_when']['field'] = 'field_when';
  $handler->display->display_options['fields']['field_when']['label'] = '';
  $handler->display->display_options['fields']['field_when']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_when']['empty'] = '[changed]';
  $handler->display->display_options['fields']['field_when']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['path'] = 'calendar/event-list';

  /* Display: Calendar */
  $handler = $view->new_display('page', 'Site Events Calendar', 'site_events_calendar');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Site Events Calendar';
  $handler->display->display_options['display_description'] = 'Site Events Calendar';
  $handler->display->display_options['defaults']['cache'] = FALSE;
  $handler->display->display_options['cache']['type'] = 'php';
  $handler->display->display_options['cache']['php_cache_results'] = 'return FALSE;';
  $handler->display->display_options['cache']['php_cache_output'] = 'return FALSE;';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'fullcalendar';
  $handler->display->display_options['style_options']['right'] = '';
  $handler->display->display_options['style_options']['timeformat'] = '';
  $handler->display->display_options['style_options']['editable'] = TRUE;
  $handler->display->display_options['style_options']['fields']['title_field'] = 'nothing';
  $handler->display->display_options['style_options']['fields']['date_field'] = array(
    'field_when' => 'field_when',
  );
  $handler->display->display_options['style_options']['fields']['title'] = TRUE;
  $handler->display->display_options['style_options']['fields']['date'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['label'] = 'Extra info';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = 'To add events to this calendar, add an <a href="http://vaxia.org/?q=node/add/event">Event</a>.
To add this calendar to your own Google calendar or other software, use this URL: <a href="http://vaxia.org/?q=calendar/ical">http://vaxia.org/?q=calendar/ical</a>.';
  $handler->display->display_options['footer']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
  /* Field: Content: Updated/commented date */
  $handler->display->display_options['fields']['last_updated']['id'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['table'] = 'node_comment_statistics';
  $handler->display->display_options['fields']['last_updated']['field'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['label'] = '';
  $handler->display->display_options['fields']['last_updated']['exclude'] = TRUE;
  $handler->display->display_options['fields']['last_updated']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['last_updated']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['last_updated']['date_format'] = 'just_the_time_per_zone';
  $handler->display->display_options['fields']['last_updated']['second_date_format'] = 'privatemsg_current_day';
  $handler->display->display_options['fields']['last_updated']['timezone'] = 'America/New_York';
  /* Field: Content: When */
  $handler->display->display_options['fields']['field_when']['id'] = 'field_when';
  $handler->display->display_options['fields']['field_when']['table'] = 'field_data_field_when';
  $handler->display->display_options['fields']['field_when']['field'] = 'field_when';
  $handler->display->display_options['fields']['field_when']['label'] = '';
  $handler->display->display_options['fields']['field_when']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_when']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_when']['empty'] = '[last_updated]';
  $handler->display->display_options['fields']['field_when']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_when']['settings'] = array(
    'format_type' => 'just_the_time_per_zone',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[title] - A/SH [name] ([field_when])';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['path'] = 'calendar';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Site Events Calendar';
  $handler->display->display_options['menu']['description'] = 'Site Events Calendar';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['site_events_feed'] = $view;

  return $export;
}
