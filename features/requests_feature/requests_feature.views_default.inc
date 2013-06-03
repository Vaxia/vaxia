<?php
/**
 * @file
 * requests_feature.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function requests_feature_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'feature_request_list';
  $view->description = 'Feature request list';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Feature request list';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Feature request list';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    8 => '8',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_complexity' => 'field_complexity',
    'field_priority' => 'field_priority',
    'field_scheduled' => 'field_scheduled',
    'field_good_idea' => 'field_good_idea',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_complexity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_priority' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_scheduled' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_good_idea' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'This is a tracking system for the requested tech features and reported bugs for Vaxia. There are only (currently) so many developer hours to go around, and this list is intended to allow new features and bug fixes to be scheduled for release throughout the year.

If you have a request, we will gladly consider it, but we do need to know *exactly* what you have in mind. So the more details the better. Be clear, be exact, and describe what you want in step-by-step terms. If you do that, it makes it much easier for us to create what you want.';
  $handler->display->display_options['header']['area']['format'] = 'wiki';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Complexity */
  $handler->display->display_options['fields']['field_complexity']['id'] = 'field_complexity';
  $handler->display->display_options['fields']['field_complexity']['table'] = 'field_data_field_complexity';
  $handler->display->display_options['fields']['field_complexity']['field'] = 'field_complexity';
  /* Field: Content: Priority */
  $handler->display->display_options['fields']['field_priority']['id'] = 'field_priority';
  $handler->display->display_options['fields']['field_priority']['table'] = 'field_data_field_priority';
  $handler->display->display_options['fields']['field_priority']['field'] = 'field_priority';
  /* Field: Content: Scheduled */
  $handler->display->display_options['fields']['field_scheduled']['id'] = 'field_scheduled';
  $handler->display->display_options['fields']['field_scheduled']['table'] = 'field_data_field_scheduled';
  $handler->display->display_options['fields']['field_scheduled']['field'] = 'field_scheduled';
  /* Field: Field: Is this a good idea? */
  $handler->display->display_options['fields']['field_good_idea']['id'] = 'field_good_idea';
  $handler->display->display_options['fields']['field_good_idea']['table'] = 'field_data_field_good_idea';
  $handler->display->display_options['fields']['field_good_idea']['field'] = 'field_good_idea';
  $handler->display->display_options['fields']['field_good_idea']['click_sort_column'] = 'rating';
  $handler->display->display_options['fields']['field_good_idea']['settings'] = array(
    'widget' => array(
      'fivestar_widget' => 'sites/all/modules/fivestar/widgets/outline/outline.css',
    ),
    'expose' => 1,
    'style' => 'user',
    'text' => 'average',
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
    'feature_request' => 'feature_request',
  );
  /* Filter criterion: Content: Scheduled (field_scheduled) */
  $handler->display->display_options['filters']['field_scheduled_value']['id'] = 'field_scheduled_value';
  $handler->display->display_options['filters']['field_scheduled_value']['table'] = 'field_data_field_scheduled';
  $handler->display->display_options['filters']['field_scheduled_value']['field'] = 'field_scheduled_value';
  $handler->display->display_options['filters']['field_scheduled_value']['value'] = array(
    'Spring' => 'Spring',
    'Summer' => 'Summer',
    'Fall' => 'Fall',
    'Winter' => 'Winter',
    'Not scheduled' => 'Not scheduled',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'feature-request-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Feature requests';
  $handler->display->display_options['menu']['description'] = 'Feature requests';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['feature_request_list'] = $view;

  return $export;
}
