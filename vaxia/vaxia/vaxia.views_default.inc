<?php
/**
 * @file
 * vaxia.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function vaxia_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'vaxia_character_sheets';
  $view->description = 'Vaxia character sheets';
  $view->tag = 'Vaxia Characters';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia character sheets';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Characters';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    1 => '1',
    30 => '30',
    8 => '8',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title_1' => 'title_1',
    'name' => 'name',
    'field_realm' => 'field_realm',
    'created' => 'created',
    'last_updated' => 'last_updated',
    'sid' => 'sid',
    'status' => 'status',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'title_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_realm' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_updated' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sid' => array(
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
    'edit_node' => array(
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
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = 'Name';
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Player';
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Posted';
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Updated/commented date */
  $handler->display->display_options['fields']['last_updated']['id'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['table'] = 'node_comment_statistics';
  $handler->display->display_options['fields']['last_updated']['field'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['label'] = 'Last updated';
  $handler->display->display_options['fields']['last_updated']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['last_updated']['date_format'] = 'short';
  /* Field: Workflow: Current state */
  $handler->display->display_options['fields']['sid']['id'] = 'sid';
  $handler->display->display_options['fields']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['fields']['sid']['field'] = 'sid';
  /* Sort criterion: User: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  $handler->display->display_options['sorts']['name']['relationship'] = 'uid';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;

  /* Display: Character Sheets */
  $handler = $view->new_display('page', 'Character Sheets', 'chars');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Character Sheets';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'users';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 1;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Player';
  $handler->display->display_options['filters']['uid']['expose']['description'] = 'Enter the player name to filter results.';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    8 => 0,
    9 => 0,
    12 => 0,
    13 => 0,
    10 => 0,
    11 => 0,
    6 => 0,
    7 => 0,
    5 => 0,
    4 => 0,
  );
  $handler->display->display_options['path'] = 'character-sheets/all';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Characters';
  $handler->display->display_options['menu']['description'] = 'Characters';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Characters';
  $handler->display->display_options['tab_options']['description'] = 'Characters';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'menu-vaxia';

  /* Display: My character sheets */
  $handler = $view->new_display('page', 'My character sheets', 'my_chars');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'My character sheets';
  $handler->display->display_options['display_description'] = 'My character sheets';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title_1' => 'title_1',
    'name' => 'name',
    'field_realm' => 'field_realm',
    'created' => 'created',
    'last_updated' => 'last_updated',
    'sid' => 'sid',
    'status' => 'status',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_realm' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_updated' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sid' => array(
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
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = 'Name';
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Player';
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = FALSE;
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Posted';
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Updated/commented date */
  $handler->display->display_options['fields']['last_updated']['id'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['table'] = 'node_comment_statistics';
  $handler->display->display_options['fields']['last_updated']['field'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['label'] = 'Last updated';
  $handler->display->display_options['fields']['last_updated']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['last_updated']['date_format'] = 'short';
  /* Field: Workflow: Current state */
  $handler->display->display_options['fields']['sid']['id'] = 'sid';
  $handler->display->display_options['fields']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['fields']['sid']['field'] = 'sid';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Author uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'node';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    6 => '6',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  $handler->display->display_options['path'] = 'character-sheets/mine';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'My characters';
  $handler->display->display_options['menu']['description'] = 'My characters';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';

  /* Display: User characters */
  $handler = $view->new_display('page', 'User characters', 'user_chars');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'My character sheets';
  $handler->display->display_options['display_description'] = 'User characters';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'timestamp' => 'timestamp',
    'title_1' => 'title_1',
    'name' => 'name',
    'created' => 'created',
    'last_updated' => 'last_updated',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'timestamp' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_updated' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
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
  $handler->display->display_options['header']['area']['content'] = 'These are the characters for this user that have been approved for play.';
  $handler->display->display_options['header']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = '[url=/character-sheets/mine]Click here to see all of your own character sheets.[/url]';
  $handler->display->display_options['footer']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = 'Name';
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Player';
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = FALSE;
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Posted';
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Updated/commented date */
  $handler->display->display_options['fields']['last_updated']['id'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['table'] = 'node_comment_statistics';
  $handler->display->display_options['fields']['last_updated']['field'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['label'] = 'Last updated';
  $handler->display->display_options['fields']['last_updated']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['last_updated']['date_format'] = 'short';
  /* Field: Workflow: Current state */
  $handler->display->display_options['fields']['sid']['id'] = 'sid';
  $handler->display->display_options['fields']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['fields']['sid']['field'] = 'sid';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'AND',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['operator'] = 'not';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  $handler->display->display_options['filters']['sid']['expose']['operator_id'] = 'sid_op';
  $handler->display->display_options['filters']['sid']['expose']['label'] = 'Current state';
  $handler->display->display_options['filters']['sid']['expose']['operator'] = 'sid_op';
  $handler->display->display_options['filters']['sid']['expose']['identifier'] = 'sid';
  $handler->display->display_options['filters']['sid']['expose']['required'] = TRUE;
  $handler->display->display_options['filters']['sid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['sid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['sid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    12 => 0,
    8 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    9 => 0,
    14 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
    18 => 0,
  );
  $handler->display->display_options['filters']['sid']['expose']['reduce'] = TRUE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 2;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value_1']['id'] = 'field_is_npc_value_1';
  $handler->display->display_options['filters']['field_is_npc_value_1']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value_1']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value_1']['operator'] = 'not';
  $handler->display->display_options['filters']['field_is_npc_value_1']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_npc_value_1']['group'] = 2;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid_1']['id'] = 'sid_1';
  $handler->display->display_options['filters']['sid_1']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid_1']['field'] = 'sid';
  $handler->display->display_options['filters']['sid_1']['value'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    6 => '6',
  );
  $handler->display->display_options['filters']['sid_1']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 2;
  $handler->display->display_options['path'] = 'user/%/characters';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Characters';
  $handler->display->display_options['menu']['description'] = 'Characters';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Refs: Chat Characters */
  $handler = $view->new_display('references', 'Refs: Chat Characters', 'my_chars_ref');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Refs: Chat Characters';
  $handler->display->display_options['display_description'] = 'Refs: Chat Characters';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'references_style';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'php',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'references_fields';
  $handler->display->display_options['row_options']['separator'] = ' ';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Titles */
  $handler->display->display_options['fields']['field_titles']['id'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['table'] = 'field_data_field_titles';
  $handler->display->display_options['fields']['field_titles']['field'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['label'] = '';
  $handler->display->display_options['fields']['field_titles']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_titles']['element_label_colon'] = FALSE;
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_last_name']['id'] = 'field_last_name';
  $handler->display->display_options['fields']['field_last_name']['table'] = 'field_data_field_last_name';
  $handler->display->display_options['fields']['field_last_name']['field'] = 'field_last_name';
  $handler->display->display_options['fields']['field_last_name']['label'] = '';
  $handler->display->display_options['fields']['field_last_name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_last_name']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = '[field_titles] [title_1] [field_last_name]';
  $handler->display->display_options['fields']['title_1']['alter']['max_length'] = '40';
  $handler->display->display_options['fields']['title_1']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title_1']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid']['link_to_user'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = '';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = 'return _vaxia_get_character_ref_types($data->nid);';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['sorts']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['sorts']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['sorts']['field_is_npc_value']['field'] = 'field_is_npc_value';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Realm (field_tag_realm) */
  $handler->display->display_options['arguments']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['default_argument_type'] = 'php';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['default_argument_options']['code'] = 'return _vaxia_filter_characters_by_room(arg(1));';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['field_tag_realm_tid']['validate']['type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['validate_options']['vocabularies'] = array(
    'realm' => 'realm',
  );
  $handler->display->display_options['arguments']['field_tag_realm_tid']['validate_options']['type'] = 'tids';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['validate']['fail'] = 'ignore';
  $handler->display->display_options['arguments']['field_tag_realm_tid']['break_phrase'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'AND',
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 2;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 2;
  /* Filter criterion: User Access Permissions */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['ui_name'] = 'User Access Permissions';
  $handler->display->display_options['filters']['php']['group'] = 2;
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = '// If the user is able to create open NPCs or is the owner then allow access.
global $user;
return !( user_access(\'use NPCs\') || $row->uid == $user->uid );';
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid_1']['id'] = 'sid_1';
  $handler->display->display_options['filters']['sid_1']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid_1']['field'] = 'sid';
  $handler->display->display_options['filters']['sid_1']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid_1']['group'] = 2;

  /* Display: Block: Character sheets */
  $handler = $view->new_display('block', 'Block: Character sheets', 'my_chars_block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'My Characters';
  $handler->display->display_options['display_description'] = 'Block: Character sheets';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Go';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'jump_menu';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'php',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['path'] = 'nid';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'No characters available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = '/node/[nid]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = '';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = '';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = 'return _vaxia_get_character_ref_types($data->nid);';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['sorts']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['sorts']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['sorts']['field_is_npc_value']['field'] = 'field_is_npc_value';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'AND',
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 2;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 2;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid_1']['id'] = 'sid_1';
  $handler->display->display_options['filters']['sid_1']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid_1']['field'] = 'sid';
  $handler->display->display_options['filters']['sid_1']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid_1']['group'] = 2;
  /* Filter criterion: User Access Permissions */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['ui_name'] = 'User Access Permissions';
  $handler->display->display_options['filters']['php']['group'] = 2;
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = 'global $user;
// If the user is able to create open NPCs or is the owner then allow access.
return !( user_access(\'use NPCs\') || $row->uid == $user->uid );';
  $handler->display->display_options['block_description'] = 'My character sheets block';

  /* Display: Refs: Active characters */
  $handler = $view->new_display('references', 'Refs: Active characters', 'active_chars_ref');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Refs: Active characters';
  $handler->display->display_options['display_description'] = 'Refs: Active characters';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'references_style';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'references_fields';
  $handler->display->display_options['row_options']['separator'] = ' ';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['type'] = 'taxonomy_term_reference_plain';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['text'] = '([name])';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'users';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status_1']['value'] = '1';

  /* Display: Evaluation */
  $handler = $view->new_display('page', 'Evaluation', 'eval_chars');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Evaluation';
  $handler->display->display_options['display_description'] = 'Evaluation';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    14 => '14',
    21 => '21',
    9 => '9',
  );
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title_1' => 'title_1',
    'name' => 'name',
    'created' => 'created',
    'last_updated' => 'last_updated',
    'sid' => 'sid',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'last_updated';
  $handler->display->display_options['style_options']['info'] = array(
    'title_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'last_updated' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['content'] = 'Characters marked as Other require approval from the Setting department.

Other char eval considerations:

* For Sirian characters - please make sure the character provides a location of origin so we know the plots and legal systems that may apply to them. If their current loyalties are different from home-land, please ask for those as well.

The player has been notified that it may take up to 72 hours before their character begins being addressed by the eval team. It may take much longer still to get the character to a state where it can be approved, especially if it is an Other species or has a Condition, so do not feel pressured to rush into making a decision on a character until you are fully confident that you have addressed any and all concerns with the character details, numbers and skills.

If a single player submits multiple characters on the same day, bear in mind that the 72-hour window does not overlap for the same account - it\'s 72 hours per character (so if they submit three characters, it may take 9 days before the third character is even addressed). We recommend evaluating only one character at a time for a given player. Once that character has been completely addressed and either approved or disapproved, then move on to the next.

Don\'t hesitate to contact the System lead(s) or another evaller for a second opinion of you have questions about anything on the character sheet. Just keep the player posted about concerns with their character or any pending approvals from others, such as the Setting Lead(s).';
  $handler->display->display_options['footer']['area']['format'] = 'plain_text';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = 'Name';
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Player';
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Posted';
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Content: Updated/commented date */
  $handler->display->display_options['fields']['last_updated']['id'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['table'] = 'node_comment_statistics';
  $handler->display->display_options['fields']['last_updated']['field'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['label'] = 'Last updated';
  $handler->display->display_options['fields']['last_updated']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['last_updated']['date_format'] = 'short';
  /* Field: Workflow: Current state */
  $handler->display->display_options['fields']['sid']['id'] = 'sid';
  $handler->display->display_options['fields']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['fields']['sid']['field'] = 'sid';
  $handler->display->display_options['fields']['sid']['element_label_colon'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'edit';
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'users';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    3 => '3',
    6 => '6',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  $handler->display->display_options['filters']['sid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['sid']['expose']['operator_id'] = 'sid_op';
  $handler->display->display_options['filters']['sid']['expose']['label'] = 'Current state';
  $handler->display->display_options['filters']['sid']['expose']['operator'] = 'sid_op';
  $handler->display->display_options['filters']['sid']['expose']['identifier'] = 'sid';
  $handler->display->display_options['filters']['sid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['sid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['sid']['expose']['reduce'] = TRUE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Player';
  $handler->display->display_options['filters']['uid']['expose']['description'] = 'Enter the player name to filter results.';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    8 => 0,
    9 => 0,
    12 => 0,
    13 => 0,
    10 => 0,
    11 => 0,
    6 => 0,
    7 => 0,
    5 => 0,
    4 => 0,
  );
  $handler->display->display_options['path'] = 'character-sheets/evaluate';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Character evaluation';
  $handler->display->display_options['menu']['description'] = 'Character evaluation';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';

  /* Display: Rules: User Characters */
  $handler = $view->new_display('views_rules', 'Rules: User Characters', 'user_chars_rules');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Rules: User Characters';
  $handler->display->display_options['display_description'] = 'Rules: User Characters';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['rules_parameter'] = array(
    'uid' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Content: Author uid',
      'name' => 'uid',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'nid' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'integer',
      'label' => 'Nid',
      'name' => 'nid',
    ),
    'title_1' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'text',
      'label' => 'Name',
      'name' => 'title_1',
    ),
  );

  /* Display: Rules: # Submitted Chars */
  $handler = $view->new_display('views_rules', 'Rules: # Submitted Chars', 'eval_chars_rules');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Rules: # Submitted Chars';
  $handler->display->display_options['display_description'] = 'Rules: # Submitted Chars';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '4';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    3 => '3',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    0 => '0',
  );
  /* Filter criterion: Content: Storyhost (field_is_storyteller) */
  $handler->display->display_options['filters']['field_is_storyteller_value']['id'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['table'] = 'field_data_field_is_storyteller';
  $handler->display->display_options['filters']['field_is_storyteller_value']['field'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['rules_parameter'] = array(
    'uid' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Content: Author uid',
      'name' => 'uid',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'nid' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'integer',
      'label' => 'Nid',
      'name' => 'nid',
    ),
    'title_1' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'text',
      'label' => 'Name',
      'name' => 'title_1',
    ),
  );

  /* Display: Rules: User Characters w/XP */
  $handler = $view->new_display('views_rules', 'Rules: User Characters w/XP', 'xp_chars_rules');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Rules: User Characters w/XP';
  $handler->display->display_options['display_description'] = 'Rules: User Characters w/XP';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 1;
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['group'] = 1;
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = '$has_xp = _character_sheet_get_xp($row->nid, \'xp\');
return ($has_xp < 1);';
  $handler->display->display_options['rules_parameter'] = array(
    'uid' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Content: Author uid',
      'name' => 'uid',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'nid' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'integer',
      'label' => 'Nid',
      'name' => 'nid',
    ),
    'title_1' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'text',
      'label' => 'Name',
      'name' => 'title_1',
    ),
  );

  /* Display: Rules: Character Birthdays */
  $handler = $view->new_display('views_rules', 'Rules: Character Birthdays', 'bday_chars');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Rules: Character Birthdays';
  $handler->display->display_options['display_description'] = 'Rules: Character Birthdays';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['group'] = 1;
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = '$day = (int) date(\'d\');
$month = (int) date(\'m\');
$node = node_load( (int) $row->nid );
$char_day = (int) $node->field_birth_day[\'und\'][0][\'value\'];
$char_month = (int) $node->field_birth_month[\'und\'][0][\'value\'];
return !($day == $char_day && $month == $char_month);';
  $handler->display->display_options['rules_parameter'] = array(
    'uid' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Content: Author uid',
      'name' => 'uid',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'nid' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'integer',
      'label' => 'Nid',
      'name' => 'nid',
    ),
  );

  /* Display: Block: Notify Returned Chars */
  $handler = $view->new_display('block', 'Block: Notify Returned Chars', 'block_notify_ret_chars');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Notify Returned Chars';
  $handler->display->display_options['display_description'] = 'Notify Returned Chars';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['wrapper_class'] = 'item-list messages status';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title_1']['alter']['text'] = 'Your character "[title_1]" has been returned to draft for further revision. Please check the "Workflow" tab on the character for details.';
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title_1']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Workflow: Previous state */
  $handler->display->display_options['filters']['sid_1']['id'] = 'sid_1';
  $handler->display->display_options['filters']['sid_1']['table'] = 'workflow_node_history';
  $handler->display->display_options['filters']['sid_1']['field'] = 'sid';
  $handler->display->display_options['filters']['sid_1']['value'] = array(
    3 => '3',
  );
  $handler->display->display_options['filters']['sid_1']['group'] = 1;
  /* Filter criterion: Workflow: Current time */
  $handler->display->display_options['filters']['stamp']['id'] = 'stamp';
  $handler->display->display_options['filters']['stamp']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['stamp']['field'] = 'stamp';
  $handler->display->display_options['filters']['stamp']['operator'] = '>';
  $handler->display->display_options['filters']['stamp']['value']['value'] = '-3 days';
  $handler->display->display_options['filters']['stamp']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['stamp']['group'] = 1;
  $handler->display->display_options['block_description'] = 'Notify Returned Chars';
  $export['vaxia_character_sheets'] = $view;

  return $export;
}
