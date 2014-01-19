<?php
/**
 * @file
 * vaxia_accounts.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function vaxia_accounts_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'strikes';
  $view->description = 'Player\'s earned strikes';
  $view->tag = 'Vaxia';
  $view->base_table = 'node';
  $view->human_name = 'Strikes';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Strikes';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    4 => '4',
    9 => '9',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'title' => 'title',
    'field_category' => 'field_category',
    'field_severity' => 'field_severity',
    'field_documentation' => 'field_documentation',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_category' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_severity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_documentation' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'This account has no strikes against it.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Strike date';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Category */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  /* Field: Content: Severity */
  $handler->display->display_options['fields']['field_severity']['id'] = 'field_severity';
  $handler->display->display_options['fields']['field_severity']['table'] = 'field_data_field_severity';
  $handler->display->display_options['fields']['field_severity']['field'] = 'field_severity';
  /* Field: Content: Documentation */
  $handler->display->display_options['fields']['field_documentation']['id'] = 'field_documentation';
  $handler->display->display_options['fields']['field_documentation']['table'] = 'field_data_field_documentation';
  $handler->display->display_options['fields']['field_documentation']['field'] = 'field_documentation';
  $handler->display->display_options['fields']['field_documentation']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_documentation']['delta_offset'] = '0';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Player (field_player) */
  $handler->display->display_options['arguments']['field_player_uid']['id'] = 'field_player_uid';
  $handler->display->display_options['arguments']['field_player_uid']['table'] = 'field_data_field_player';
  $handler->display->display_options['arguments']['field_player_uid']['field'] = 'field_player_uid';
  $handler->display->display_options['arguments']['field_player_uid']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['field_player_uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_player_uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_player_uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_player_uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_player_uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_player_uid']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['field_player_uid']['validate']['fail'] = 'access denied';
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
    'strike' => 'strike',
  );

  /* Display: Strikes */
  $handler = $view->new_display('page', 'Strikes', 'strikes');
  $handler->display->display_options['path'] = 'user/%/strikes';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Strikes';
  $handler->display->display_options['menu']['description'] = 'Strikes earned';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'features';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['strikes'] = $view;

  return $export;
}
