<?php
/**
 * @file
 * rpg_site_staff.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function rpg_site_staff_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sessions';
  $view->description = 'A list of your sessions';
  $view->tag = 'Staff';
  $view->base_table = 'node';
  $view->human_name = 'Sessions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sessions';
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
    'field_saga' => 'title',
    'field_realm' => 'field_realm',
    'field_session_held' => 'field_session_held',
    'field_characters_involved' => 'field_characters_involved',
    'body' => 'body',
    'field_rate_session' => 'field_rate_session',
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
    'field_saga' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_realm' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_session_held' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_characters_involved' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'body' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_rate_session' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Saga */
  $handler->display->display_options['fields']['field_saga']['id'] = 'field_saga';
  $handler->display->display_options['fields']['field_saga']['table'] = 'field_data_field_saga';
  $handler->display->display_options['fields']['field_saga']['field'] = 'field_saga';
  $handler->display->display_options['fields']['field_saga']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_saga']['alter']['text'] = ' ([field_saga])';
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_realm']['id'] = 'field_realm';
  $handler->display->display_options['fields']['field_realm']['table'] = 'field_data_field_realm';
  $handler->display->display_options['fields']['field_realm']['field'] = 'field_realm';
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
  );
  /* Field: Content: Characters involved */
  $handler->display->display_options['fields']['field_characters_involved']['id'] = 'field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['table'] = 'field_data_field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['field'] = 'field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['label'] = 'Characters';
  $handler->display->display_options['fields']['field_characters_involved']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = 'Details';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '300';
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  /* Field: Content: How good was this session? */
  $handler->display->display_options['fields']['field_rate_session']['id'] = 'field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['table'] = 'field_data_field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['field'] = 'field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['label'] = 'Rate';
  $handler->display->display_options['fields']['field_rate_session']['click_sort_column'] = 'rating';
  $handler->display->display_options['fields']['field_rate_session']['settings'] = array(
    'widget' => array(
      'fivestar_widget' => 'sites/all/modules/fivestar/widgets/outline/outline.css',
    ),
    'expose' => 0,
    'style' => 'average',
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
    'session_report' => 'session_report',
  );
  /* Filter criterion: Content: Realm (field_realm) */
  $handler->display->display_options['filters']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['filters']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['value'] = array(
    'all' => 'all',
    'Vaxia' => 'Vaxia',
    'Sirian' => 'Sirian',
  );
  $handler->display->display_options['filters']['field_realm_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_realm_value']['expose']['operator_id'] = 'field_realm_value_op';
  $handler->display->display_options['filters']['field_realm_value']['expose']['label'] = 'Realm';
  $handler->display->display_options['filters']['field_realm_value']['expose']['operator'] = 'field_realm_value_op';
  $handler->display->display_options['filters']['field_realm_value']['expose']['identifier'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_realm_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_realm_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    19 => 0,
    12 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    9 => 0,
    20 => 0,
    14 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
  );
  /* Filter criterion: Content: Saga (field_saga) */
  $handler->display->display_options['filters']['field_saga_tid']['id'] = 'field_saga_tid';
  $handler->display->display_options['filters']['field_saga_tid']['table'] = 'field_data_field_saga';
  $handler->display->display_options['filters']['field_saga_tid']['field'] = 'field_saga_tid';
  $handler->display->display_options['filters']['field_saga_tid']['value'] = '';
  $handler->display->display_options['filters']['field_saga_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_saga_tid']['expose']['operator_id'] = 'field_saga_tid_op';
  $handler->display->display_options['filters']['field_saga_tid']['expose']['label'] = 'Saga';
  $handler->display->display_options['filters']['field_saga_tid']['expose']['operator'] = 'field_saga_tid_op';
  $handler->display->display_options['filters']['field_saga_tid']['expose']['identifier'] = 'field_saga_tid';
  $handler->display->display_options['filters']['field_saga_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_saga_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_saga_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    19 => 0,
    12 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    9 => 0,
    20 => 0,
    14 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
  );
  $handler->display->display_options['filters']['field_saga_tid']['vocabulary'] = 'sagas';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'A/SH';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    19 => 0,
    12 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    9 => 0,
    20 => 0,
    14 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
  );

  /* Display: Sessions */
  $handler = $view->new_display('page', 'Sessions', 'sessions');
  $handler->display->display_options['display_description'] = 'A list of all sessions';
  $handler->display->display_options['path'] = 'sessions';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Sessions';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';

  /* Display: Character sessions */
  $handler = $view->new_display('page', 'Character sessions', 'character_sessions');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Character sessions';
  $handler->display->display_options['display_description'] = 'Character sessions';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'SH';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
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
  );
  /* Field: Content: Characters involved */
  $handler->display->display_options['fields']['field_characters_involved']['id'] = 'field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['table'] = 'field_data_field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['field'] = 'field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['label'] = 'Characters';
  $handler->display->display_options['fields']['field_characters_involved']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = 'Details';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '300';
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  /* Field: Content: How good was this session? */
  $handler->display->display_options['fields']['field_rate_session']['id'] = 'field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['table'] = 'field_data_field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['field'] = 'field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['label'] = 'Rate';
  $handler->display->display_options['fields']['field_rate_session']['click_sort_column'] = 'rating';
  $handler->display->display_options['fields']['field_rate_session']['settings'] = array(
    'widget' => array(
      'fivestar_widget' => 'sites/all/modules/fivestar/widgets/outline/outline.css',
    ),
    'expose' => 0,
    'style' => 'average',
    'text' => 'average',
  );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Characters involved (field_characters_involved) */
  $handler->display->display_options['arguments']['field_characters_involved_nid']['id'] = 'field_characters_involved_nid';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['table'] = 'field_data_field_characters_involved';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['field'] = 'field_characters_involved_nid';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_characters_involved_nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['field_characters_involved_nid']['validate_options']['types'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['arguments']['field_characters_involved_nid']['validate_options']['access'] = TRUE;
  $handler->display->display_options['arguments']['field_characters_involved_nid']['validate_options']['access_op'] = 'update';
  $handler->display->display_options['path'] = 'node/%/sessions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Sessions';
  $handler->display->display_options['menu']['description'] = 'Sessions this character has been in';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: SH sessions */
  $handler = $view->new_display('page', 'SH sessions', 'sh_sessions');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'SH Sessions';
  $handler->display->display_options['display_description'] = 'SH sessions';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'SH';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
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
  );
  /* Field: Content: Characters involved */
  $handler->display->display_options['fields']['field_characters_involved']['id'] = 'field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['table'] = 'field_data_field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['field'] = 'field_characters_involved';
  $handler->display->display_options['fields']['field_characters_involved']['label'] = 'Characters';
  $handler->display->display_options['fields']['field_characters_involved']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = 'Details';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '300';
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['html'] = TRUE;
  /* Field: Content: How good was this session? */
  $handler->display->display_options['fields']['field_rate_session']['id'] = 'field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['table'] = 'field_data_field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['field'] = 'field_rate_session';
  $handler->display->display_options['fields']['field_rate_session']['label'] = 'Rate';
  $handler->display->display_options['fields']['field_rate_session']['click_sort_column'] = 'rating';
  $handler->display->display_options['fields']['field_rate_session']['settings'] = array(
    'widget' => array(
      'fivestar_widget' => 'sites/all/modules/fivestar/widgets/outline/outline.css',
    ),
    'expose' => 0,
    'style' => 'average',
    'text' => 'average',
  );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['uid']['validate']['fail'] = 'access denied';
  $handler->display->display_options['path'] = 'user/%/sessions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Sessions';
  $handler->display->display_options['menu']['description'] = 'Sessions this user has run';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['sessions'] = $view;

  $view = new view();
  $view->name = 'storytellers_and_assistants';
  $view->description = 'Wiki A/SH List';
  $view->tag = 'Staff';
  $view->base_table = 'users';
  $view->human_name = 'Wiki A/SH List';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Storytellers and Assistants';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'field_titles' => 'name',
    'title' => 'name',
    'is_online' => 'is_online',
    'access' => 'access',
    'contact' => 'contact',
    'rid' => 'rid',
    'php' => 'php',
    'field_took_ash1' => 'php',
    'field_took_ash2' => 'php',
  );
  $handler->display->display_options['style_options']['default'] = 'php';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'field_titles' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'is_online' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'access' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'contact' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rid' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'field_took_ash1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_took_ash2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: User: Storyhost Character (field_storyhost_character) */
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['id'] = 'field_storyhost_character_nid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['table'] = 'field_data_field_storyhost_character';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['field'] = 'field_storyhost_character_nid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['delta'] = '-1';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Titles */
  $handler->display->display_options['fields']['field_titles']['id'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['table'] = 'field_data_field_titles';
  $handler->display->display_options['fields']['field_titles']['field'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['relationship'] = 'field_storyhost_character_nid';
  $handler->display->display_options['fields']['field_titles']['label'] = '';
  $handler->display->display_options['fields']['field_titles']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_titles']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_storyhost_character_nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '[field_titles] [title]';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: User: Is online */
  $handler->display->display_options['fields']['is_online']['id'] = 'is_online';
  $handler->display->display_options['fields']['is_online']['table'] = 'users';
  $handler->display->display_options['fields']['is_online']['field'] = 'is_online';
  $handler->display->display_options['fields']['is_online']['not'] = 0;
  /* Field: User: Last access */
  $handler->display->display_options['fields']['access']['id'] = 'access';
  $handler->display->display_options['fields']['access']['table'] = 'users';
  $handler->display->display_options['fields']['access']['field'] = 'access';
  $handler->display->display_options['fields']['access']['date_format'] = 'time ago';
  /* Field: User: Link to contact page */
  $handler->display->display_options['fields']['contact']['id'] = 'contact';
  $handler->display->display_options['fields']['contact']['table'] = 'users';
  $handler->display->display_options['fields']['contact']['field'] = 'contact';
  $handler->display->display_options['fields']['contact']['text'] = 'Contact';
  /* Field: User: Roles */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['label'] = '';
  $handler->display->display_options['fields']['rid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['rid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rid']['separator'] = '|';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Roles';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = 'return _rpg_site_staff_return_title($data->uid);';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '2';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: User: Took ASH Test One */
  $handler->display->display_options['fields']['field_took_ash1']['id'] = 'field_took_ash1';
  $handler->display->display_options['fields']['field_took_ash1']['table'] = 'field_data_field_took_ash1';
  $handler->display->display_options['fields']['field_took_ash1']['field'] = 'field_took_ash1';
  $handler->display->display_options['fields']['field_took_ash1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_took_ash1']['alter']['text'] = '<i>Took ASH Test One:</i> [field_took_ash1]';
  $handler->display->display_options['fields']['field_took_ash1']['type'] = 'format_interval';
  $handler->display->display_options['fields']['field_took_ash1']['settings'] = array(
    'interval' => '2',
    'interval_display' => 'time ago',
  );
  /* Field: User: Took ASH Test Two */
  $handler->display->display_options['fields']['field_took_ash2']['id'] = 'field_took_ash2';
  $handler->display->display_options['fields']['field_took_ash2']['table'] = 'field_data_field_took_ash2';
  $handler->display->display_options['fields']['field_took_ash2']['field'] = 'field_took_ash2';
  $handler->display->display_options['fields']['field_took_ash2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_took_ash2']['alter']['text'] = '<i>Took ASH Test Two:</i> [field_took_ash2] ';
  $handler->display->display_options['fields']['field_took_ash2']['type'] = 'format_interval';
  $handler->display->display_options['fields']['field_took_ash2']['settings'] = array(
    'interval' => '2',
    'interval_display' => 'time ago',
  );
  /* Sort criterion: User: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'users';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    13 => '13',
    10 => '10',
    17 => '17',
    15 => '15',
    9 => '9',
  );
  $handler->display->display_options['filters']['rid']['group'] = 1;
  $handler->display->display_options['filters']['rid']['reduce_duplicates'] = TRUE;

  /* Display: Storytellers and Assistants */
  $handler = $view->new_display('page', 'Storytellers and Assistants', 'sh_and_ash');
  $handler->display->display_options['path'] = 'wiki/storytellers-and-assistants';
  $export['storytellers_and_assistants'] = $view;

  $view = new view();
  $view->name = 'user_activity_totals';
  $view->description = 'Display roughly how active a player has been on the site';
  $view->tag = 'Staff';
  $view->base_table = 'users';
  $view->human_name = 'User Activity Totals';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'SH Activity Totals';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    4 => '4',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'status' => 'status',
    'rid' => 'rid',
    'uid' => 'uid',
    'php' => 'php',
  );
  $handler->display->display_options['style_options']['default'] = 'uid';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rid' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'uid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'php' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Field: User: Active */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'users';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: User: Roles */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['type'] = 'ul';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Content Created';
  $handler->display->display_options['fields']['php']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = 'return _rpg_site_staff_create_content_count_value($row->uid);';
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
echo _rpg_site_staff_content_count_display($value);
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'Content Revised';
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_1']['php_value'] = 'return _rpg_site_staff_revise_content_count_value($row->uid);';
  $handler->display->display_options['fields']['php_1']['php_output'] = '<?php
echo _rpg_site_staff_content_count_display($value);
?>';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    19 => 0,
    12 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    9 => 0,
    20 => 0,
    14 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
  );
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    13 => '13',
    10 => '10',
    17 => '17',
    15 => '15',
    9 => '9',
    20 => '20',
    14 => '14',
  );
  $handler->display->display_options['filters']['rid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['operator_id'] = 'rid_op';
  $handler->display->display_options['filters']['rid']['expose']['label'] = 'Roles';
  $handler->display->display_options['filters']['rid']['expose']['operator'] = 'rid_op';
  $handler->display->display_options['filters']['rid']['expose']['identifier'] = 'rid';
  $handler->display->display_options['filters']['rid']['expose']['required'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
    19 => 0,
    12 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    9 => 0,
    20 => 0,
    14 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
  );
  $handler->display->display_options['filters']['rid']['expose']['reduce'] = TRUE;
  $handler->display->display_options['filters']['rid']['reduce_duplicates'] = TRUE;

  /* Display: User Activity Totals */
  $handler = $view->new_display('page', 'User Activity Totals', 'user_activity_totals');
  $handler->display->display_options['display_description'] = 'User Activity Totals';
  $handler->display->display_options['path'] = 'admin/user-activity-totals';
  $export['user_activity_totals'] = $view;

  $view = new view();
  $view->name = 'wiki_display_sessions';
  $view->description = 'A list of your sessions';
  $view->tag = 'Staff';
  $view->base_table = 'node';
  $view->human_name = 'Wiki sessions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sirian Sessions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    8 => '8',
  );
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = '3600';
  $handler->display->display_options['cache']['results_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['output_lifespan'] = '3600';
  $handler->display->display_options['cache']['output_lifespan_custom'] = '0';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'field_saga',
      'rendered' => 1,
      'rendered_strip' => 1,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['separator'] = ' ';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: User: Storyhost Character (field_storyhost_character) */
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['id'] = 'field_storyhost_character_nid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['table'] = 'field_data_field_storyhost_character';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['field'] = 'field_storyhost_character_nid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['relationship'] = 'uid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['delta'] = '-1';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '<h2>[name]</h2>';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_storyhost_character_nid';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title_1']['empty'] = '[name]';
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Field: Content: Titles */
  $handler->display->display_options['fields']['field_titles']['id'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['table'] = 'field_data_field_titles';
  $handler->display->display_options['fields']['field_titles']['field'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['relationship'] = 'field_storyhost_character_nid';
  $handler->display->display_options['fields']['field_titles']['label'] = '';
  $handler->display->display_options['fields']['field_titles']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_titles']['element_label_colon'] = FALSE;
  /* Field: Content: Saga */
  $handler->display->display_options['fields']['field_saga']['id'] = 'field_saga';
  $handler->display->display_options['fields']['field_saga']['table'] = 'field_data_field_saga';
  $handler->display->display_options['fields']['field_saga']['field'] = 'field_saga';
  $handler->display->display_options['fields']['field_saga']['label'] = '';
  $handler->display->display_options['fields']['field_saga']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_saga']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_saga']['alter']['text'] = '<h2>[field_titles] [title_1] : [field_saga]</h2>';
  $handler->display->display_options['fields']['field_saga']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_saga']['empty'] = '<h2>[field_titles] [title_1] : Standalone Sessions</h2>';
  /* Field: Content: Session held */
  $handler->display->display_options['fields']['field_session_held']['id'] = 'field_session_held';
  $handler->display->display_options['fields']['field_session_held']['table'] = 'field_data_field_session_held';
  $handler->display->display_options['fields']['field_session_held']['field'] = 'field_session_held';
  $handler->display->display_options['fields']['field_session_held']['label'] = '';
  $handler->display->display_options['fields']['field_session_held']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_session_held']['alter']['text'] = '([field_session_held])';
  $handler->display->display_options['fields']['field_session_held']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_session_held']['settings'] = array(
    'format_type' => 'privatemsg_current_day',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '[title] <em>([field_session_held])</em>';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Sort criterion: User: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['sorts']['name']['relationship'] = 'uid';
  /* Sort criterion: Content: Saga (field_saga) */
  $handler->display->display_options['sorts']['field_saga_tid']['id'] = 'field_saga_tid';
  $handler->display->display_options['sorts']['field_saga_tid']['table'] = 'field_data_field_saga';
  $handler->display->display_options['sorts']['field_saga_tid']['field'] = 'field_saga_tid';
  $handler->display->display_options['sorts']['field_saga_tid']['order'] = 'DESC';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  /* Contextual filter: Content: Realm (field_realm) */
  $handler->display->display_options['arguments']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['arguments']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['arguments']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['arguments']['field_realm_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_realm_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_realm_value']['default_argument_options']['argument'] = 'Vaxia';
  $handler->display->display_options['arguments']['field_realm_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_realm_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_realm_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_realm_value']['validate']['fail'] = 'ignore';
  $handler->display->display_options['arguments']['field_realm_value']['limit'] = '0';
  $handler->display->display_options['arguments']['field_realm_value']['case'] = 'ucfirst';
  $handler->display->display_options['arguments']['field_realm_value']['transform_dash'] = TRUE;
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
  );

  /* Display: Vaxia Sessions */
  $handler = $view->new_display('page', 'Vaxia Sessions', 'wiki_vaxia_sessions');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Vaxia Sessions';
  $handler->display->display_options['display_description'] = 'A list of all sessions';
  $handler->display->display_options['path'] = 'wiki/vaxia/session-list';
  $handler->display->display_options['menu']['title'] = 'Sessions';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Sirian Sessions */
  $handler = $view->new_display('page', 'Sirian Sessions', 'wiki_sirian_sessions');
  $handler->display->display_options['display_description'] = 'A list of all Sirian sessions';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Realm (field_realm) */
  $handler->display->display_options['arguments']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['arguments']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['arguments']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['arguments']['field_realm_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_realm_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_realm_value']['default_argument_options']['argument'] = 'Sirian';
  $handler->display->display_options['arguments']['field_realm_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_realm_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_realm_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_realm_value']['validate']['fail'] = 'ignore';
  $handler->display->display_options['arguments']['field_realm_value']['limit'] = '0';
  $handler->display->display_options['arguments']['field_realm_value']['case'] = 'ucfirst';
  $handler->display->display_options['arguments']['field_realm_value']['transform_dash'] = TRUE;
  $handler->display->display_options['path'] = 'wiki/sirian/session-list';
  $handler->display->display_options['menu']['title'] = 'Sessions';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['wiki_display_sessions'] = $view;

  return $export;
}
