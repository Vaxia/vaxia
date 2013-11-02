<?php
/**
 * @file
 * vaxia_npcs.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function vaxia_npcs_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'wiki_npcs_list';
  $view->description = 'NPC master list for wiki display';
  $view->tag = 'Vaxia NPCs';
  $view->base_table = 'node';
  $view->human_name = 'Wiki NPCs List';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Wiki NPCs List';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_realm' => 'field_realm',
    'field_race' => 'field_race',
    'field_description' => 'field_description',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
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
    'field_race' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_description' => array(
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
  $handler->display->display_options['header']['area']['label'] = 'Header text';
  $handler->display->display_options['header']['area']['content'] = 'The following NPCs are recorded in our system. These NPCs are currently not in use, when claimed by a SH they will no longer be available on this display as they are currently in use for a saga or session.';
  $handler->display->display_options['header']['area']['format'] = 'wiki';
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
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Storyhost';
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_realm']['id'] = 'field_realm';
  $handler->display->display_options['fields']['field_realm']['table'] = 'field_data_field_realm';
  $handler->display->display_options['fields']['field_realm']['field'] = 'field_realm';
  /* Field: Content: Race */
  $handler->display->display_options['fields']['field_race']['id'] = 'field_race';
  $handler->display->display_options['fields']['field_race']['table'] = 'field_data_field_race';
  $handler->display->display_options['fields']['field_race']['field'] = 'field_race';
  /* Field: Content: Description */
  $handler->display->display_options['fields']['field_description']['id'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['table'] = 'field_data_field_description';
  $handler->display->display_options['fields']['field_description']['field'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_description']['alter']['max_length'] = '255';
  $handler->display->display_options['fields']['field_description']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_description']['alter']['trim'] = TRUE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['expose']['label'] = 'Title';
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
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['required'] = TRUE;
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
    6 => '6',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Content: Realm (field_realm) */
  $handler->display->display_options['filters']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['filters']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['value'] = array(
    'all' => 'all',
    'Vaxia' => 'Vaxia',
    'Sirian' => 'Sirian',
  );
  $handler->display->display_options['filters']['field_realm_value']['group'] = 1;
  $handler->display->display_options['filters']['field_realm_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_realm_value']['expose']['operator_id'] = 'field_realm_value_op';
  $handler->display->display_options['filters']['field_realm_value']['expose']['label'] = 'Realm (field_realm)';
  $handler->display->display_options['filters']['field_realm_value']['expose']['operator'] = 'field_realm_value_op';
  $handler->display->display_options['filters']['field_realm_value']['expose']['identifier'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_realm_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_realm_value']['expose']['remember_roles'] = array(
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
    3 => 0,
    4 => 0,
    11 => 0,
    16 => 0,
  );
  /* Filter criterion: Content: Race (field_race) */
  $handler->display->display_options['filters']['field_race_value']['id'] = 'field_race_value';
  $handler->display->display_options['filters']['field_race_value']['table'] = 'field_data_field_race';
  $handler->display->display_options['filters']['field_race_value']['field'] = 'field_race_value';
  $handler->display->display_options['filters']['field_race_value']['value'] = array(
    'all' => 'all',
    'human' => 'human',
    'half-elf' => 'half-elf',
    'elf' => 'elf',
    'orc' => 'orc',
    'half-orc' => 'half-orc',
    'goblin' => 'goblin',
    'neth' => 'neth',
    'gnome' => 'gnome',
    'halfling' => 'halfling',
    'lurai' => 'lurai',
    'fairy' => 'fairy',
    'aratay' => 'aratay',
    'mojingan' => 'mojingan',
    'dwarf' => 'dwarf',
    'human_dalhan' => 'human_dalhan',
    'human_rohearn' => 'human_rohearn',
    'human_clanspeople' => 'human_clanspeople',
    'human_gargorian' => 'human_gargorian',
    'human_demi' => 'human_demi',
    'half_elfin' => 'half_elfin',
    'metal_robot' => 'metal_robot',
    'metal_nanobot' => 'metal_nanobot',
    'metal_ai' => 'metal_ai',
  );
  $handler->display->display_options['filters']['field_race_value']['group'] = 1;
  $handler->display->display_options['filters']['field_race_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_race_value']['expose']['operator_id'] = 'field_race_value_op';
  $handler->display->display_options['filters']['field_race_value']['expose']['label'] = 'Race (field_race)';
  $handler->display->display_options['filters']['field_race_value']['expose']['operator'] = 'field_race_value_op';
  $handler->display->display_options['filters']['field_race_value']['expose']['identifier'] = 'field_race_value';
  $handler->display->display_options['filters']['field_race_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_race_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_race_value']['expose']['remember_roles'] = array(
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
    3 => 0,
    4 => 0,
    11 => 0,
    16 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Storyhost Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
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
  );
  /* Filter criterion: Content: Open NPC (field_is_open_npc) */
  $handler->display->display_options['filters']['field_is_open_npc_value']['id'] = 'field_is_open_npc_value';
  $handler->display->display_options['filters']['field_is_open_npc_value']['table'] = 'field_data_field_is_open_npc';
  $handler->display->display_options['filters']['field_is_open_npc_value']['field'] = 'field_is_open_npc_value';
  $handler->display->display_options['filters']['field_is_open_npc_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_open_npc_value']['group'] = 2;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 2;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'vaxia_npcs_list');
  $handler->display->display_options['path'] = 'wiki/wiki-npcs-list';
  $export['wiki_npcs_list'] = $view;

  return $export;
}
