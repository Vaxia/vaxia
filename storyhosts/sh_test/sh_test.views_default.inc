<?php
/**
 * @file
 * sh_test.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function sh_test_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'sessions';
  $view->description = 'A list of your sessions';
  $view->tag = 'Storyhosts';
  $view->base_table = 'node';
  $view->human_name = 'Sessions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sessions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_titles' => 'field_titles',
    'name' => 'name',
    'title_1' => 'name',
    'field_artwork_image' => 'name',
    'access' => 'access',
    'title' => 'title',
    'value' => 'title',
    'field_took_ash1' => 'field_took_ash1',
    'field_took_ash2' => 'field_took_ash1',
    'field_took_ash3' => 'field_took_ash1',
    'field_took_eval_test' => 'field_took_ash1',
    'field_became_evaluator' => 'field_took_ash1',
    'field_became_sh' => 'field_took_ash1',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'field_titles' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_artwork_image' => array(
      'sortable' => 0,
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
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'value' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_took_ash1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'field_took_ash2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_took_ash3' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_took_eval_test' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_became_evaluator' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_became_sh' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
  $handler->display->display_options['empty']['area']['content'] = 'No sessions found.';
  $handler->display->display_options['empty']['area']['format'] = 'wiki';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: Content: Vote results */
  $handler->display->display_options['relationships']['votingapi_cache']['id'] = 'votingapi_cache';
  $handler->display->display_options['relationships']['votingapi_cache']['table'] = 'node';
  $handler->display->display_options['relationships']['votingapi_cache']['field'] = 'votingapi_cache';
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
  /* Field: Content: Event held */
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
    21 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
  );
  $handler->display->display_options['filters']['field_realm_value']['expose']['reduce'] = TRUE;
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
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'A/SH Player';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'user';
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
    21 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
  );

  /* Display: Sessions */
  $handler = $view->new_display('page', 'Sessions', 'sessions');
  $handler->display->display_options['display_description'] = 'A list of all sessions';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['path'] = 'sessions/all';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Sessions';
  $handler->display->display_options['menu']['description'] = 'Sessions';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Sessions';
  $handler->display->display_options['tab_options']['description'] = 'Sessions';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'menu-vaxia';

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
  /* Field: Content: Event held */
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
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
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
  /* Field: Content: Event held */
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

  /* Display: Storyhosts */
  $handler = $view->new_display('page', 'Storyhosts', 'storyhosts');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Storyhosts and Apprentices';
  $handler->display->display_options['display_description'] = 'A list of all storyhosts';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_artwork_image' => 'field_artwork_image',
    'field_titles' => 'field_titles',
    'name' => 'name',
    'title_1' => 'name',
    'access' => 'name',
    'title' => 'name',
    'value' => 'name',
    'field_became_evaluator' => 'field_became_evaluator',
    'field_became_sh' => 'field_became_evaluator',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'field_artwork_image' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_titles' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'access' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'value' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_became_evaluator' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'field_became_sh' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '[b]This is a list of our Storyhosts and Apprentices over time. In case you were curious to know who ran all those sessions and keep our settings fresh and alive.[/b]';
  $handler->display->display_options['header']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: View area */
  $handler->display->display_options['footer']['view']['id'] = 'view';
  $handler->display->display_options['footer']['view']['table'] = 'views';
  $handler->display->display_options['footer']['view']['field'] = 'view';
  $handler->display->display_options['footer']['view']['label'] = 'Retired Storyhosts';
  $handler->display->display_options['footer']['view']['empty'] = TRUE;
  $handler->display->display_options['footer']['view']['view_to_insert'] = 'sessions:retired_shs';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Content: Votes */
  $handler->display->display_options['relationships']['votingapi_vote']['id'] = 'votingapi_vote';
  $handler->display->display_options['relationships']['votingapi_vote']['table'] = 'node';
  $handler->display->display_options['relationships']['votingapi_vote']['field'] = 'votingapi_vote';
  $handler->display->display_options['relationships']['votingapi_vote']['votingapi'] = array(
    'value_type' => 'percent',
    'tag' => 'good_session',
  );
  $handler->display->display_options['relationships']['votingapi_vote']['current_user'] = 0;
  /* Relationship: User: Storyhost Character (field_storyhost_character) */
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['id'] = 'field_storyhost_character_nid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['table'] = 'field_data_field_storyhost_character';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['field'] = 'field_storyhost_character_nid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['relationship'] = 'uid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['delta'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field: Image */
  $handler->display->display_options['fields']['field_artwork_image']['id'] = 'field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['table'] = 'field_data_field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['field'] = 'field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_artwork_image']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_artwork_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork_image']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_artwork_image']['group_column'] = 'entity_id';
  /* Field: Content: Titles */
  $handler->display->display_options['fields']['field_titles']['id'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['table'] = 'field_data_field_titles';
  $handler->display->display_options['fields']['field_titles']['field'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['relationship'] = 'field_storyhost_character_nid';
  $handler->display->display_options['fields']['field_titles']['exclude'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_storyhost_character_nid';
  $handler->display->display_options['fields']['title_1']['label'] = 'Storyhost';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = '[field_titles] [title_1]';
  /* Field: User: Last access */
  $handler->display->display_options['fields']['access']['id'] = 'access';
  $handler->display->display_options['fields']['access']['table'] = 'users';
  $handler->display->display_options['fields']['access']['field'] = 'access';
  $handler->display->display_options['fields']['access']['relationship'] = 'uid';
  $handler->display->display_options['fields']['access']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['access']['alter']['text'] = '<i>Last online:</i> [access]';
  $handler->display->display_options['fields']['access']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['access']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['title']['label'] = 'Sessions';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '<i>Number of sessions:</i> [title]';
  /* Field: Votes: Value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'votingapi_vote';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['fields']['value']['relationship'] = 'votingapi_vote';
  $handler->display->display_options['fields']['value']['group_type'] = 'avg';
  $handler->display->display_options['fields']['value']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['value']['alter']['text'] = '<i>Average approval:</i> [value]';
  $handler->display->display_options['fields']['value']['empty'] = 'No votes.';
  $handler->display->display_options['fields']['value']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['value']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['value']['precision'] = '2';
  $handler->display->display_options['fields']['value']['suffix'] = '%';
  /* Field: User: Became SH */
  $handler->display->display_options['fields']['field_became_sh']['id'] = 'field_became_sh';
  $handler->display->display_options['fields']['field_became_sh']['table'] = 'field_data_field_became_sh';
  $handler->display->display_options['fields']['field_became_sh']['field'] = 'field_became_sh';
  $handler->display->display_options['fields']['field_became_sh']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_became_sh']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_became_sh']['alter']['text'] = '<i>Became Storyhost:</i> [field_became_sh]';
  $handler->display->display_options['fields']['field_became_sh']['empty'] = 'Apprentice Storyhost';
  $handler->display->display_options['fields']['field_became_sh']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: User: Became Evaluator */
  $handler->display->display_options['fields']['field_became_evaluator']['id'] = 'field_became_evaluator';
  $handler->display->display_options['fields']['field_became_evaluator']['table'] = 'field_data_field_became_evaluator';
  $handler->display->display_options['fields']['field_became_evaluator']['field'] = 'field_became_evaluator';
  $handler->display->display_options['fields']['field_became_evaluator']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_became_evaluator']['label'] = 'Since';
  $handler->display->display_options['fields']['field_became_evaluator']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_became_evaluator']['alter']['text'] = '<i>Became Evaluator:</i> [field_became_evaluator]';
  $handler->display->display_options['fields']['field_became_evaluator']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    9 => '9',
    13 => '13',
    10 => '10',
    17 => '17',
  );
  $handler->display->display_options['filters']['rid']['group'] = 1;
  $handler->display->display_options['filters']['rid']['reduce_duplicates'] = TRUE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'users';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_2']['id'] = 'status_2';
  $handler->display->display_options['filters']['status_2']['table'] = 'users';
  $handler->display->display_options['filters']['status_2']['field'] = 'status';
  $handler->display->display_options['filters']['status_2']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status_2']['value'] = '0';
  $handler->display->display_options['filters']['status_2']['group'] = 2;
  $handler->display->display_options['path'] = 'sessions/storytellers-and-apprentices';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Storyhosts and Apprentices';
  $handler->display->display_options['menu']['description'] = 'Storyhosts and Apprentices';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Retired Storyhosts */
  $handler = $view->new_display('page', 'Retired Storyhosts', 'retired_shs');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Retired Storyhosts and Apprentices';
  $handler->display->display_options['display_description'] = 'A list of all storyhosts';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_artwork_image' => 'field_artwork_image',
    'field_titles' => 'field_titles',
    'name' => 'name',
    'title_1' => 'name',
    'access' => 'name',
    'title' => 'name',
    'value' => 'name',
    'field_became_evaluator' => 'field_became_evaluator',
    'field_became_sh' => 'field_became_evaluator',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'field_artwork_image' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_titles' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'access' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'value' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_became_evaluator' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'field_became_sh' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '</br>
[b]Our retired storyhosts and apprentices are no longer running on Vaxia... but their work lives on![/b]
</br>';
  $handler->display->display_options['header']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Content: Votes */
  $handler->display->display_options['relationships']['votingapi_vote']['id'] = 'votingapi_vote';
  $handler->display->display_options['relationships']['votingapi_vote']['table'] = 'node';
  $handler->display->display_options['relationships']['votingapi_vote']['field'] = 'votingapi_vote';
  $handler->display->display_options['relationships']['votingapi_vote']['votingapi'] = array(
    'value_type' => 'percent',
    'tag' => 'good_session',
  );
  $handler->display->display_options['relationships']['votingapi_vote']['current_user'] = 0;
  /* Relationship: User: Storyhost Character (field_storyhost_character) */
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['id'] = 'field_storyhost_character_nid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['table'] = 'field_data_field_storyhost_character';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['field'] = 'field_storyhost_character_nid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['relationship'] = 'uid';
  $handler->display->display_options['relationships']['field_storyhost_character_nid']['delta'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field: Image */
  $handler->display->display_options['fields']['field_artwork_image']['id'] = 'field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['table'] = 'field_data_field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['field'] = 'field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_artwork_image']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_artwork_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork_image']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  $handler->display->display_options['fields']['field_artwork_image']['group_column'] = 'entity_id';
  /* Field: Content: Titles */
  $handler->display->display_options['fields']['field_titles']['id'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['table'] = 'field_data_field_titles';
  $handler->display->display_options['fields']['field_titles']['field'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['relationship'] = 'field_storyhost_character_nid';
  $handler->display->display_options['fields']['field_titles']['exclude'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_storyhost_character_nid';
  $handler->display->display_options['fields']['title_1']['label'] = 'Storyhost';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = '[field_titles] [title_1]';
  /* Field: User: Last access */
  $handler->display->display_options['fields']['access']['id'] = 'access';
  $handler->display->display_options['fields']['access']['table'] = 'users';
  $handler->display->display_options['fields']['access']['field'] = 'access';
  $handler->display->display_options['fields']['access']['relationship'] = 'uid';
  $handler->display->display_options['fields']['access']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['access']['alter']['text'] = '<i>Last online:</i> [access]';
  $handler->display->display_options['fields']['access']['date_format'] = 'time ago';
  $handler->display->display_options['fields']['access']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['title']['label'] = 'Sessions';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '<i>Number of sessions:</i> [title]';
  /* Field: Votes: Value */
  $handler->display->display_options['fields']['value']['id'] = 'value';
  $handler->display->display_options['fields']['value']['table'] = 'votingapi_vote';
  $handler->display->display_options['fields']['value']['field'] = 'value';
  $handler->display->display_options['fields']['value']['relationship'] = 'votingapi_vote';
  $handler->display->display_options['fields']['value']['group_type'] = 'avg';
  $handler->display->display_options['fields']['value']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['value']['alter']['text'] = '<i>Average approval:</i> [value]';
  $handler->display->display_options['fields']['value']['empty'] = 'No votes.';
  $handler->display->display_options['fields']['value']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['value']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['value']['precision'] = '2';
  $handler->display->display_options['fields']['value']['suffix'] = '%';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'Since';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = 'Retired';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['rid']['operator'] = 'not';
  $handler->display->display_options['filters']['rid']['value'] = array(
    9 => '9',
    13 => '13',
    10 => '10',
    17 => '17',
  );
  $handler->display->display_options['filters']['rid']['group'] = 1;
  $handler->display->display_options['filters']['rid']['reduce_duplicates'] = TRUE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'users';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_2']['id'] = 'status_2';
  $handler->display->display_options['filters']['status_2']['table'] = 'users';
  $handler->display->display_options['filters']['status_2']['field'] = 'status';
  $handler->display->display_options['filters']['status_2']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status_2']['value'] = '0';
  $handler->display->display_options['filters']['status_2']['group'] = 2;
  $handler->display->display_options['path'] = 'sessions/storytellers-and-apprentices/retired';
  $handler->display->display_options['menu']['title'] = 'Storyhosts and Apprentices';
  $handler->display->display_options['menu']['description'] = 'Retired Storyhosts and Apprentices';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['sessions'] = $view;

  $view = new view();
  $view->name = 'sh_applications';
  $view->description = 'SH Applications';
  $view->tag = 'SH Test';
  $view->base_table = 'node';
  $view->human_name = 'SH Applications';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'SH Applications';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view sh test results';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'title' => 'title',
    'status' => 'status',
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '[name]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Webform: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'webform';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = '';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['status']['alter']['text'] = '([status])';
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
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
    'storyhost_application' => 'storyhost_application',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Webform: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'webform';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  /* Filter criterion: Content: Updated date */
  $handler->display->display_options['filters']['changed']['id'] = 'changed';
  $handler->display->display_options['filters']['changed']['table'] = 'node';
  $handler->display->display_options['filters']['changed']['field'] = 'changed';
  $handler->display->display_options['filters']['changed']['operator'] = '>';
  $handler->display->display_options['filters']['changed']['value']['value'] = '-1 days';
  $handler->display->display_options['filters']['changed']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['changed']['group'] = 2;

  /* Display: SH Applications */
  $handler = $view->new_display('block', 'SH Applications', 'sh_applications');
  $handler->display->display_options['display_description'] = 'SH Applications';
  $handler->display->display_options['block_description'] = 'SH Applications';
  $export['sh_applications'] = $view;

  return $export;
}
