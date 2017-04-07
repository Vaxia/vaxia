<?php
/**
 * @file
 * storyhosts.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function storyhosts_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'character_goals';
  $view->description = 'Character goals for SH review';
  $view->tag = 'Storyhosts';
  $view->base_table = 'node';
  $view->human_name = 'Character goals';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Character goals';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    21 => '21',
    9 => '9',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'name' => 'name',
    'field_character_goals' => 'field_character_goals',
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
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_character_goals' => array(
      'sortable' => 1,
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
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Character';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Player';
  /* Field: Content: Character Goals */
  $handler->display->display_options['fields']['field_character_goals']['id'] = 'field_character_goals';
  $handler->display->display_options['fields']['field_character_goals']['table'] = 'field_data_field_character_goals';
  $handler->display->display_options['fields']['field_character_goals']['field'] = 'field_character_goals';
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
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Character Goals (field_character_goals) */
  $handler->display->display_options['filters']['field_character_goals_value']['id'] = 'field_character_goals_value';
  $handler->display->display_options['filters']['field_character_goals_value']['table'] = 'field_data_field_character_goals';
  $handler->display->display_options['filters']['field_character_goals_value']['field'] = 'field_character_goals_value';
  $handler->display->display_options['filters']['field_character_goals_value']['operator'] = 'not empty';
  $handler->display->display_options['filters']['field_character_goals_value']['group'] = 1;
  $handler->display->display_options['filters']['field_character_goals_value']['expose']['operator_id'] = 'field_character_goals_value_op';
  $handler->display->display_options['filters']['field_character_goals_value']['expose']['label'] = 'Character Goals (field_character_goals)';
  $handler->display->display_options['filters']['field_character_goals_value']['expose']['operator'] = 'field_character_goals_value_op';
  $handler->display->display_options['filters']['field_character_goals_value']['expose']['identifier'] = 'field_character_goals_value';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Character';
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
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
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Player';
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
  );
  /* Filter criterion: Content: Character Goals (field_character_goals) */
  $handler->display->display_options['filters']['field_character_goals_value_1']['id'] = 'field_character_goals_value_1';
  $handler->display->display_options['filters']['field_character_goals_value_1']['table'] = 'field_data_field_character_goals';
  $handler->display->display_options['filters']['field_character_goals_value_1']['field'] = 'field_character_goals_value';
  $handler->display->display_options['filters']['field_character_goals_value_1']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_character_goals_value_1']['group'] = 1;
  $handler->display->display_options['filters']['field_character_goals_value_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_character_goals_value_1']['expose']['operator_id'] = 'field_character_goals_value_1_op';
  $handler->display->display_options['filters']['field_character_goals_value_1']['expose']['label'] = 'Character Goals';
  $handler->display->display_options['filters']['field_character_goals_value_1']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_character_goals_value_1']['expose']['operator'] = 'field_character_goals_value_1_op';
  $handler->display->display_options['filters']['field_character_goals_value_1']['expose']['identifier'] = 'field_character_goals_value_1';
  $handler->display->display_options['filters']['field_character_goals_value_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_character_goals_value_1']['expose']['remember_roles'] = array(
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
  );

  /* Display: Character goals */
  $handler = $view->new_display('page', 'Character goals', 'character_goals');
  $handler->display->display_options['display_description'] = 'Character goals';
  $handler->display->display_options['path'] = 'character-goals';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Character goals';
  $handler->display->display_options['menu']['description'] = 'Character goals';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia-sh';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['character_goals'] = $view;

  $view = new view();
  $view->name = 'skill_inventory';
  $view->description = 'A list of skills in play on the site';
  $view->tag = 'Storyhosts';
  $view->base_table = 'field_collection_item';
  $view->human_name = 'Skill inventory';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Skill inventory';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    14 => '14',
    21 => '21',
    9 => '9',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'name' => 'name',
    'field_realm' => 'field_realm',
    'field_skill_name' => 'field_skill_name',
    'field_skill_desc' => 'field_skill_name',
  );
  $handler->display->display_options['style_options']['default'] = 'field_skill_name';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
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
    'field_skill_name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'field_skill_desc' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Field collection item: Entity with the Skill (field_skill) */
  $handler->display->display_options['relationships']['field_skill_node']['id'] = 'field_skill_node';
  $handler->display->display_options['relationships']['field_skill_node']['table'] = 'field_collection_item';
  $handler->display->display_options['relationships']['field_skill_node']['field'] = 'field_skill_node';
  $handler->display->display_options['relationships']['field_skill_node']['label'] = 'field_character';
  $handler->display->display_options['relationships']['field_skill_node']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'field_skill_node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_skill_node';
  $handler->display->display_options['fields']['title']['label'] = 'Character';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Player';
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['relationship'] = 'field_skill_node';
  $handler->display->display_options['fields']['field_tag_realm']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Field collection item: Name */
  $handler->display->display_options['fields']['field_skill_name']['id'] = 'field_skill_name';
  $handler->display->display_options['fields']['field_skill_name']['table'] = 'field_data_field_skill_name';
  $handler->display->display_options['fields']['field_skill_name']['field'] = 'field_skill_name';
  $handler->display->display_options['fields']['field_skill_name']['label'] = 'Skill';
  $handler->display->display_options['fields']['field_skill_name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_skill_name']['alter']['text'] = '<b>[field_skill_name]</b>';
  /* Field: Field collection item: Description */
  $handler->display->display_options['fields']['field_skill_desc']['id'] = 'field_skill_desc';
  $handler->display->display_options['fields']['field_skill_desc']['table'] = 'field_data_field_skill_desc';
  $handler->display->display_options['fields']['field_skill_desc']['field'] = 'field_skill_desc';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['relationship'] = 'field_skill_node';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = 'Operations for Character';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
  );
  /* Filter criterion: Field collection item: Name (field_skill_name) */
  $handler->display->display_options['filters']['field_skill_name_value_1']['id'] = 'field_skill_name_value_1';
  $handler->display->display_options['filters']['field_skill_name_value_1']['table'] = 'field_data_field_skill_name';
  $handler->display->display_options['filters']['field_skill_name_value_1']['field'] = 'field_skill_name_value';
  $handler->display->display_options['filters']['field_skill_name_value_1']['operator'] = 'not empty';
  $handler->display->display_options['filters']['field_skill_name_value_1']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'field_skill_node';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'users';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'field_skill_node';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Character';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
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
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Player';
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
  );
  /* Filter criterion: Field collection item: Name (field_skill_name) */
  $handler->display->display_options['filters']['field_skill_name_value']['id'] = 'field_skill_name_value';
  $handler->display->display_options['filters']['field_skill_name_value']['table'] = 'field_data_field_skill_name';
  $handler->display->display_options['filters']['field_skill_name_value']['field'] = 'field_skill_name_value';
  $handler->display->display_options['filters']['field_skill_name_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_skill_name_value']['group'] = 1;
  $handler->display->display_options['filters']['field_skill_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_skill_name_value']['expose']['operator_id'] = 'field_skill_name_value_op';
  $handler->display->display_options['filters']['field_skill_name_value']['expose']['label'] = 'Skill';
  $handler->display->display_options['filters']['field_skill_name_value']['expose']['operator'] = 'field_skill_name_value_op';
  $handler->display->display_options['filters']['field_skill_name_value']['expose']['identifier'] = 'field_skill_name_value';
  $handler->display->display_options['filters']['field_skill_name_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_skill_name_value']['expose']['remember_roles'] = array(
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
  );
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['relationship'] = 'field_skill_node';
  $handler->display->display_options['filters']['sid']['value'] = array(
    2 => '2',
    3 => '3',
    4 => '4',
    6 => '6',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  $handler->display->display_options['filters']['sid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['sid']['expose']['operator_id'] = 'sid_op';
  $handler->display->display_options['filters']['sid']['expose']['label'] = 'Current state';
  $handler->display->display_options['filters']['sid']['expose']['operator'] = 'sid_op';
  $handler->display->display_options['filters']['sid']['expose']['identifier'] = 'sid';
  $handler->display->display_options['filters']['sid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['sid']['expose']['multiple'] = TRUE;
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
  );
  $handler->display->display_options['filters']['sid']['expose']['reduce'] = TRUE;
  /* Filter criterion: Field collection item: Description (field_skill_desc) */
  $handler->display->display_options['filters']['field_skill_desc_value']['id'] = 'field_skill_desc_value';
  $handler->display->display_options['filters']['field_skill_desc_value']['table'] = 'field_data_field_skill_desc';
  $handler->display->display_options['filters']['field_skill_desc_value']['field'] = 'field_skill_desc_value';
  $handler->display->display_options['filters']['field_skill_desc_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_skill_desc_value']['group'] = 1;
  $handler->display->display_options['filters']['field_skill_desc_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_skill_desc_value']['expose']['operator_id'] = 'field_skill_desc_value_op';
  $handler->display->display_options['filters']['field_skill_desc_value']['expose']['label'] = 'Description';
  $handler->display->display_options['filters']['field_skill_desc_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_skill_desc_value']['expose']['operator'] = 'field_skill_desc_value_op';
  $handler->display->display_options['filters']['field_skill_desc_value']['expose']['identifier'] = 'field_skill_desc_value';
  $handler->display->display_options['filters']['field_skill_desc_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_skill_desc_value']['expose']['remember_roles'] = array(
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
  );
  /* Filter criterion: Content: This is an NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['relationship'] = 'field_skill_node';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    0 => '0',
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 1;
  $handler->display->display_options['filters']['field_is_npc_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_is_npc_value']['expose']['operator_id'] = 'field_is_npc_value_op';
  $handler->display->display_options['filters']['field_is_npc_value']['expose']['label'] = 'Is NPC?';
  $handler->display->display_options['filters']['field_is_npc_value']['expose']['operator'] = 'field_is_npc_value_op';
  $handler->display->display_options['filters']['field_is_npc_value']['expose']['identifier'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_is_npc_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_is_npc_value']['expose']['remember_roles'] = array(
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
  );
  $handler->display->display_options['filters']['field_is_npc_value']['expose']['reduce'] = TRUE;
  /* Filter criterion: Content: Storyhost (field_is_storyteller) */
  $handler->display->display_options['filters']['field_is_storyteller_value']['id'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['table'] = 'field_data_field_is_storyteller';
  $handler->display->display_options['filters']['field_is_storyteller_value']['field'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['relationship'] = 'field_skill_node';
  $handler->display->display_options['filters']['field_is_storyteller_value']['value'] = array(
    0 => '0',
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_storyteller_value']['group'] = 1;
  $handler->display->display_options['filters']['field_is_storyteller_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_is_storyteller_value']['expose']['operator_id'] = 'field_is_storyteller_value_op';
  $handler->display->display_options['filters']['field_is_storyteller_value']['expose']['label'] = 'Is Storyhost?';
  $handler->display->display_options['filters']['field_is_storyteller_value']['expose']['operator'] = 'field_is_storyteller_value_op';
  $handler->display->display_options['filters']['field_is_storyteller_value']['expose']['identifier'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_is_storyteller_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_is_storyteller_value']['expose']['remember_roles'] = array(
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
    25 => 0,
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    28 => 0,
  );
  $handler->display->display_options['filters']['field_is_storyteller_value']['expose']['reduce'] = TRUE;

  /* Display: Skill inventory */
  $handler = $view->new_display('page', 'Skill inventory', 'skill_inventory');
  $handler->display->display_options['display_description'] = 'Skill inventory';
  $handler->display->display_options['path'] = 'skill-inventory';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Skill inventory';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia-sh';
  $export['skill_inventory'] = $view;

  $view = new view();
  $view->name = 'storyhost_characters';
  $view->description = 'Storyhost characters';
  $view->tag = 'Storyhosts';
  $view->base_table = 'node';
  $view->human_name = 'Storyhost characters';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Storyhost characters';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'name' => 'name',
    'field_storyhost_character' => 'name',
    'rid' => 'rid',
    'field_titles' => 'title',
    'title' => 'title',
    'edit_node' => 'edit_node',
    'delete_node' => 'delete_node',
    'sid' => 'sid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br>',
      'empty_column' => 0,
    ),
    'field_storyhost_character' => array(
      'sortable' => 0,
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
    'field_titles' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_node' => array(
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
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::workflow_vbo_given_state_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
  );
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Player';
  /* Field: User: Storyhost Character */
  $handler->display->display_options['fields']['field_storyhost_character']['id'] = 'field_storyhost_character';
  $handler->display->display_options['fields']['field_storyhost_character']['table'] = 'field_data_field_storyhost_character';
  $handler->display->display_options['fields']['field_storyhost_character']['field'] = 'field_storyhost_character';
  $handler->display->display_options['fields']['field_storyhost_character']['relationship'] = 'uid';
  $handler->display->display_options['fields']['field_storyhost_character']['label'] = 'Listed SH Character';
  /* Field: User: Roles */
  $handler->display->display_options['fields']['rid']['id'] = 'rid';
  $handler->display->display_options['fields']['rid']['table'] = 'users_roles';
  $handler->display->display_options['fields']['rid']['field'] = 'rid';
  $handler->display->display_options['fields']['rid']['relationship'] = 'uid';
  /* Field: Content: Titles */
  $handler->display->display_options['fields']['field_titles']['id'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['table'] = 'field_data_field_titles';
  $handler->display->display_options['fields']['field_titles']['field'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Storyhost Character';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '[field_titles] [title]';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Field: Workflow: Current state */
  $handler->display->display_options['fields']['sid']['id'] = 'sid';
  $handler->display->display_options['fields']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['fields']['sid']['field'] = 'sid';
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
    'character_sheet' => 'character_sheet',
  );
  /* Filter criterion: Content: Storyhost (field_is_storyteller) */
  $handler->display->display_options['filters']['field_is_storyteller_value']['id'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['table'] = 'field_data_field_is_storyteller';
  $handler->display->display_options['filters']['field_is_storyteller_value']['field'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['value'] = array(
    1 => '1',
  );
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['rid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['operator_id'] = 'rid_op';
  $handler->display->display_options['filters']['rid']['expose']['label'] = 'Roles';
  $handler->display->display_options['filters']['rid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['operator'] = 'rid_op';
  $handler->display->display_options['filters']['rid']['expose']['identifier'] = 'rid';
  $handler->display->display_options['filters']['rid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['rid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    30 => 0,
    8 => 0,
    9 => 0,
    14 => 0,
    21 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    19 => 0,
    28 => 0,
    29 => 0,
    12 => 0,
    25 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    20 => 0,
    22 => 0,
    23 => 0,
  );

  /* Display: Storyhost characters */
  $handler = $view->new_display('page', 'Storyhost characters', 'sh_chars');
  $handler->display->display_options['display_description'] = 'Storyhost characters';
  $handler->display->display_options['path'] = 'admin/content/storyhost-characters';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'SH characters';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['storyhost_characters'] = $view;

  $view = new view();
  $view->name = 'vaxia_moderation_log';
  $view->description = 'Vaxia moderation log';
  $view->tag = 'Storyhosts';
  $view->base_table = 'character_sheet_log';
  $view->human_name = 'Vaxia moderation log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Character sheet moderation log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    21 => '21',
    9 => '9',
    10 => '10',
    17 => '17',
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
    'lid' => 'lid',
    'type' => 'type',
    'title' => 'title',
    'name_1' => 'name_1',
    'message' => 'message',
    'name' => 'name',
    'timestamp' => 'timestamp',
    'hostname' => 'hostname',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'lid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'name_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'message' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'hostname' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  /* Relationship: Character sheet log: Character sheet log node */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'character_sheet_log';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  /* Relationship: Character sheet log: Character sheet log user */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'character_sheet_log';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['relationship'] = 'nid';
  /* Field: Character sheet log: Log ID */
  $handler->display->display_options['fields']['lid']['id'] = 'lid';
  $handler->display->display_options['fields']['lid']['table'] = 'character_sheet_log';
  $handler->display->display_options['fields']['lid']['field'] = 'lid';
  /* Field: Character sheet log: Log type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'character_sheet_log';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['name_1']['label'] = 'Player';
  $handler->display->display_options['fields']['name_1']['hide_alter_empty'] = FALSE;
  /* Field: Character sheet log: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'character_sheet_log';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Logged user';
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = FALSE;
  /* Field: Character sheet log: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'character_sheet_log';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Character sheet log: Hostname */
  $handler->display->display_options['fields']['hostname']['id'] = 'hostname';
  $handler->display->display_options['fields']['hostname']['table'] = 'character_sheet_log';
  $handler->display->display_options['fields']['hostname']['field'] = 'hostname';
  $handler->display->display_options['fields']['hostname']['hide_alter_empty'] = FALSE;
  /* Sort criterion: Character sheet log: Timestamp */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'character_sheet_log';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Filter criterion: Character sheet log: Log type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'character_sheet_log';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'not';
  $handler->display->display_options['filters']['type']['value'] = 'silver';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Log type';
  $handler->display->display_options['filters']['type']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    30 => 0,
    8 => 0,
    9 => 0,
    14 => 0,
    21 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    19 => 0,
    28 => 0,
    29 => 0,
    12 => 0,
    25 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    20 => 0,
    22 => 0,
    23 => 0,
  );
  /* Filter criterion: Character sheet log: Message */
  $handler->display->display_options['filters']['message']['id'] = 'message';
  $handler->display->display_options['filters']['message']['table'] = 'character_sheet_log';
  $handler->display->display_options['filters']['message']['field'] = 'message';
  $handler->display->display_options['filters']['message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator_id'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['label'] = 'Message';
  $handler->display->display_options['filters']['message']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['identifier'] = 'message';
  $handler->display->display_options['filters']['message']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['remember_roles'] = array(
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
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
  );
  /* Filter criterion: Character sheet log: Timestamp */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'character_sheet_log';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['exposed'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator_id'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['label'] = 'Timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['operator'] = 'timestamp_op';
  $handler->display->display_options['filters']['timestamp']['expose']['identifier'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['timestamp']['expose']['remember_roles'] = array(
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
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'nid';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Character';
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid_1';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Player';
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
    21 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_1']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['value'] = '';
  $handler->display->display_options['filters']['uid_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_1']['expose']['operator_id'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Moderator';
  $handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['uid_1']['expose']['remember_roles'] = array(
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
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
    24 => 0,
    25 => 0,
  );

  /* Display: Vaxia moderation log */
  $handler = $view->new_display('page', 'Vaxia moderation log', 'vaxia_moderation_log');
  $handler->display->display_options['display_description'] = 'Vaxia moderation log';
  $handler->display->display_options['path'] = 'character-sheets/moderation_log';
  $handler->display->display_options['menu']['title'] = 'Moderation log';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  /* Display: Moderation log */
  $handler = $view->new_display('block', 'Moderation log', 'moderation_log_block');
  $handler->display->display_options['display_description'] = 'Moderation log';
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Character sheet log: Log type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'character_sheet_log';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['hide_alter_empty'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Logged user';
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = FALSE;
  /* Field: Character sheet log: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'character_sheet_log';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  $handler->display->display_options['fields']['timestamp']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Character sheet log: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'character_sheet_log';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['text'] = '[message]';
  $handler->display->display_options['fields']['message']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'raw';
  $handler->display->display_options['arguments']['nid']['default_argument_options']['index'] = '1';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['arguments']['nid']['validate_options']['access'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate_options']['access_op'] = 'update';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Character sheet log: Log type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'character_sheet_log';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'not';
  $handler->display->display_options['filters']['type']['value'] = 'daily_silver';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Log type';
  $handler->display->display_options['filters']['type']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    30 => 0,
    8 => 0,
    9 => 0,
    14 => 0,
    21 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    19 => 0,
    28 => 0,
    29 => 0,
    12 => 0,
    25 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    20 => 0,
    22 => 0,
    23 => 0,
    31 => 0,
    32 => 0,
  );
  $handler->display->display_options['block_description'] = 'Moderation log';
  $export['vaxia_moderation_log'] = $view;

  return $export;
}
