<?php
/**
 * @file
 * strikes.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function strikes_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'strikes';
  $view->description = 'Notations a player has been involved in';
  $view->tag = 'Strikes';
  $view->base_table = 'node';
  $view->human_name = 'Notations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Notations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create strike content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_date_of_incident' => 'field_date_of_incident',
    'field_players' => 'field_players',
    'field_witnesses' => 'field_witnesses',
    'field_social_members' => 'field_social_members',
    'field_category' => 'field_category',
    'title' => 'title',
    'field_documentation' => 'field_documentation',
    'field_results' => 'field_results',
    'field_associated_strikes' => 'field_associated_strikes',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_date_of_incident' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_players' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_witnesses' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_social_members' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_category' => array(
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
    'field_documentation' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_results' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_associated_strikes' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<style>
.view-strikes .node-strike .node-title{display:none;}
.view-strikes .node-strike .submitted{display:none;}
</style>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No notations available.';
  $handler->display->display_options['empty']['area']['format'] = 'wiki';
  /* Relationship: Content: Players (field_players) */
  $handler->display->display_options['relationships']['field_players_uid']['id'] = 'field_players_uid';
  $handler->display->display_options['relationships']['field_players_uid']['table'] = 'field_data_field_players';
  $handler->display->display_options['relationships']['field_players_uid']['field'] = 'field_players_uid';
  $handler->display->display_options['relationships']['field_players_uid']['delta'] = '-1';
  /* Relationship: Content: Players Affected (field_witnesses) */
  $handler->display->display_options['relationships']['field_witnesses_uid']['id'] = 'field_witnesses_uid';
  $handler->display->display_options['relationships']['field_witnesses_uid']['table'] = 'field_data_field_witnesses';
  $handler->display->display_options['relationships']['field_witnesses_uid']['field'] = 'field_witnesses_uid';
  $handler->display->display_options['relationships']['field_witnesses_uid']['delta'] = '-1';
  /* Relationship: Content: Social members (field_social_members) */
  $handler->display->display_options['relationships']['field_social_members_uid']['id'] = 'field_social_members_uid';
  $handler->display->display_options['relationships']['field_social_members_uid']['table'] = 'field_data_field_social_members';
  $handler->display->display_options['relationships']['field_social_members_uid']['field'] = 'field_social_members_uid';
  $handler->display->display_options['relationships']['field_social_members_uid']['delta'] = '-1';
  /* Field: Content: Date of Incident */
  $handler->display->display_options['fields']['field_date_of_incident']['id'] = 'field_date_of_incident';
  $handler->display->display_options['fields']['field_date_of_incident']['table'] = 'field_data_field_date_of_incident';
  $handler->display->display_options['fields']['field_date_of_incident']['field'] = 'field_date_of_incident';
  $handler->display->display_options['fields']['field_date_of_incident']['label'] = 'Occurred On';
  $handler->display->display_options['fields']['field_date_of_incident']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Content: Players */
  $handler->display->display_options['fields']['field_players']['id'] = 'field_players';
  $handler->display->display_options['fields']['field_players']['table'] = 'field_data_field_players';
  $handler->display->display_options['fields']['field_players']['field'] = 'field_players';
  $handler->display->display_options['fields']['field_players']['label'] = 'Player(s) Involved';
  $handler->display->display_options['fields']['field_players']['delta_offset'] = '0';
  /* Field: Content: Players Affected */
  $handler->display->display_options['fields']['field_witnesses']['id'] = 'field_witnesses';
  $handler->display->display_options['fields']['field_witnesses']['table'] = 'field_data_field_witnesses';
  $handler->display->display_options['fields']['field_witnesses']['field'] = 'field_witnesses';
  $handler->display->display_options['fields']['field_witnesses']['label'] = 'Player(s) Affected';
  $handler->display->display_options['fields']['field_witnesses']['empty'] = 'No additional players affected.';
  $handler->display->display_options['fields']['field_witnesses']['delta_offset'] = '0';
  /* Field: Content: Social members */
  $handler->display->display_options['fields']['field_social_members']['id'] = 'field_social_members';
  $handler->display->display_options['fields']['field_social_members']['table'] = 'field_data_field_social_members';
  $handler->display->display_options['fields']['field_social_members']['field'] = 'field_social_members';
  $handler->display->display_options['fields']['field_social_members']['label'] = 'Social member(s) Involved';
  $handler->display->display_options['fields']['field_social_members']['delta_offset'] = '0';
  /* Field: Content: Category */
  $handler->display->display_options['fields']['field_category']['id'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['table'] = 'field_data_field_category';
  $handler->display->display_options['fields']['field_category']['field'] = 'field_category';
  $handler->display->display_options['fields']['field_category']['empty'] = 'See record';
  $handler->display->display_options['fields']['field_category']['delta_offset'] = '0';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Details';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Documentation */
  $handler->display->display_options['fields']['field_documentation']['id'] = 'field_documentation';
  $handler->display->display_options['fields']['field_documentation']['table'] = 'field_data_field_documentation';
  $handler->display->display_options['fields']['field_documentation']['field'] = 'field_documentation';
  $handler->display->display_options['fields']['field_documentation']['empty'] = 'No additional files';
  $handler->display->display_options['fields']['field_documentation']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_documentation']['delta_offset'] = '0';
  /* Field: Content: Results */
  $handler->display->display_options['fields']['field_results']['id'] = 'field_results';
  $handler->display->display_options['fields']['field_results']['table'] = 'field_data_field_results';
  $handler->display->display_options['fields']['field_results']['field'] = 'field_results';
  $handler->display->display_options['fields']['field_results']['label'] = 'Actions taken';
  $handler->display->display_options['fields']['field_results']['delta_offset'] = '0';
  /* Field: Content: Associated strikes */
  $handler->display->display_options['fields']['field_associated_strikes']['id'] = 'field_associated_strikes';
  $handler->display->display_options['fields']['field_associated_strikes']['table'] = 'field_data_field_associated_strikes';
  $handler->display->display_options['fields']['field_associated_strikes']['field'] = 'field_associated_strikes';
  $handler->display->display_options['fields']['field_associated_strikes']['empty'] = 'Green (no strikes)';
  $handler->display->display_options['fields']['field_associated_strikes']['type'] = 'nodereferrer_node';
  $handler->display->display_options['fields']['field_associated_strikes']['settings'] = array(
    'nodereferrer_view_mode' => 'artwork',
    'prefix_suffix' => 0,
  );
  $handler->display->display_options['fields']['field_associated_strikes']['delta_offset'] = '0';
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
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'record_of_interaction' => 'record_of_interaction',
    'strike' => 'strike',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  /* Filter criterion: Content: Category (field_category) */
  $handler->display->display_options['filters']['field_category_value']['id'] = 'field_category_value';
  $handler->display->display_options['filters']['field_category_value']['table'] = 'field_data_field_category';
  $handler->display->display_options['filters']['field_category_value']['field'] = 'field_category_value';
  $handler->display->display_options['filters']['field_category_value']['group'] = 1;
  $handler->display->display_options['filters']['field_category_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category_value']['expose']['operator_id'] = 'field_category_value_op';
  $handler->display->display_options['filters']['field_category_value']['expose']['label'] = 'Categories';
  $handler->display->display_options['filters']['field_category_value']['expose']['operator'] = 'field_category_value_op';
  $handler->display->display_options['filters']['field_category_value']['expose']['identifier'] = 'field_category_value';
  $handler->display->display_options['filters']['field_category_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_category_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_category_value']['expose']['remember_roles'] = array(
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
    29 => 0,
  );
  /* Filter criterion: Content: Severity (field_severity) */
  $handler->display->display_options['filters']['field_severity_value_1']['id'] = 'field_severity_value_1';
  $handler->display->display_options['filters']['field_severity_value_1']['table'] = 'field_data_field_severity';
  $handler->display->display_options['filters']['field_severity_value_1']['field'] = 'field_severity_value';
  $handler->display->display_options['filters']['field_severity_value_1']['value'] = array(
    'Yellow' => 'Yellow',
    'Red' => 'Red',
  );
  $handler->display->display_options['filters']['field_severity_value_1']['group'] = 1;
  $handler->display->display_options['filters']['field_severity_value_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_severity_value_1']['expose']['operator_id'] = 'field_severity_value_1_op';
  $handler->display->display_options['filters']['field_severity_value_1']['expose']['label'] = 'Strike Severity';
  $handler->display->display_options['filters']['field_severity_value_1']['expose']['operator'] = 'field_severity_value_1_op';
  $handler->display->display_options['filters']['field_severity_value_1']['expose']['identifier'] = 'field_severity_value_1';
  $handler->display->display_options['filters']['field_severity_value_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_severity_value_1']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_severity_value_1']['expose']['remember_roles'] = array(
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
    29 => 0,
  );
  $handler->display->display_options['filters']['field_severity_value_1']['expose']['reduce'] = TRUE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'record_of_interaction' => 'record_of_interaction',
    'strike' => 'strike',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
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
    29 => 0,
  );
  $handler->display->display_options['filters']['type']['expose']['reduce'] = TRUE;
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = 'between';
  $handler->display->display_options['filters']['created']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['created']['group'] = 1;
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Posted between';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
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
    29 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'field_players_uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Player(s)';
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
    29 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_1']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['relationship'] = 'field_social_members_uid';
  $handler->display->display_options['filters']['uid_1']['value'] = '';
  $handler->display->display_options['filters']['uid_1']['group'] = 1;
  $handler->display->display_options['filters']['uid_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_1']['expose']['operator_id'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Social Members(s)';
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
    29 => 0,
  );
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'field_players_uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_2']['id'] = 'uid_current_2';
  $handler->display->display_options['filters']['uid_current_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_2']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_2']['relationship'] = 'field_social_members_uid';
  $handler->display->display_options['filters']['uid_current_2']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_2']['group'] = 2;
  $handler->display->display_options['filters']['uid_current_2']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['uid_current_2']['expose']['label'] = 'Is the logged in user';
  $handler->display->display_options['filters']['uid_current_2']['expose']['operator'] = 'uid_current_2_op';
  $handler->display->display_options['filters']['uid_current_2']['expose']['identifier'] = 'uid_current_2';
  $handler->display->display_options['filters']['uid_current_2']['expose']['required'] = TRUE;

  /* Display: Notations */
  $handler = $view->new_display('page', 'Notations', 'notations');
  $handler->display->display_options['display_description'] = 'Notations';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'php';
  $handler->display->display_options['access']['php_access'] = 'global $user;
if ($user->uid == arg(1)) {
return TRUE;
}
return FALSE;';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
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
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'notation' => 'notation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
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
    29 => 0,
  );
  $handler->display->display_options['filters']['type']['expose']['reduce'] = TRUE;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'field_players_uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['filters']['uid_current']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_1']['id'] = 'uid_current_1';
  $handler->display->display_options['filters']['uid_current_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_1']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_1']['relationship'] = 'field_witnesses_uid';
  $handler->display->display_options['filters']['uid_current_1']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_1']['group'] = 2;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current_2']['id'] = 'uid_current_2';
  $handler->display->display_options['filters']['uid_current_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current_2']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current_2']['relationship'] = 'field_social_members_uid';
  $handler->display->display_options['filters']['uid_current_2']['value'] = '1';
  $handler->display->display_options['filters']['uid_current_2']['group'] = 2;
  $handler->display->display_options['filters']['uid_current_2']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['uid_current_2']['expose']['label'] = 'Is the logged in user';
  $handler->display->display_options['filters']['uid_current_2']['expose']['operator'] = 'uid_current_2_op';
  $handler->display->display_options['filters']['uid_current_2']['expose']['identifier'] = 'uid_current_2';
  $handler->display->display_options['filters']['uid_current_2']['expose']['required'] = TRUE;
  $handler->display->display_options['path'] = 'user/%/notations';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Notations';
  $handler->display->display_options['menu']['description'] = 'Notations';
  $handler->display->display_options['menu']['weight'] = '98';
  $handler->display->display_options['menu']['name'] = 'features';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: All Notations */
  $handler = $view->new_display('page', 'All Notations', 'all_notations');
  $handler->display->display_options['display_description'] = 'All Notations';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    21 => '21',
    29 => '29',
  );
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Category (field_category) */
  $handler->display->display_options['filters']['field_category_value']['id'] = 'field_category_value';
  $handler->display->display_options['filters']['field_category_value']['table'] = 'field_data_field_category';
  $handler->display->display_options['filters']['field_category_value']['field'] = 'field_category_value';
  $handler->display->display_options['filters']['field_category_value']['group'] = 1;
  $handler->display->display_options['filters']['field_category_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_category_value']['expose']['operator_id'] = 'field_category_value_op';
  $handler->display->display_options['filters']['field_category_value']['expose']['label'] = 'Categories';
  $handler->display->display_options['filters']['field_category_value']['expose']['operator'] = 'field_category_value_op';
  $handler->display->display_options['filters']['field_category_value']['expose']['identifier'] = 'field_category_value';
  $handler->display->display_options['filters']['field_category_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_category_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_category_value']['expose']['remember_roles'] = array(
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
    29 => 0,
  );
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'notation' => 'notation',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
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
    29 => 0,
  );
  $handler->display->display_options['filters']['type']['expose']['reduce'] = TRUE;
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = 'between';
  $handler->display->display_options['filters']['created']['value']['type'] = 'offset';
  $handler->display->display_options['filters']['created']['group'] = 1;
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Posted between';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
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
    29 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'field_players_uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Player(s)';
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
    29 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['filters']['uid_1']['table'] = 'users';
  $handler->display->display_options['filters']['uid_1']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_1']['relationship'] = 'field_social_members_uid';
  $handler->display->display_options['filters']['uid_1']['value'] = '';
  $handler->display->display_options['filters']['uid_1']['group'] = 1;
  $handler->display->display_options['filters']['uid_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_1']['expose']['operator_id'] = 'uid_1_op';
  $handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Social Members(s)';
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
    29 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid_2']['id'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['table'] = 'users';
  $handler->display->display_options['filters']['uid_2']['field'] = 'uid';
  $handler->display->display_options['filters']['uid_2']['relationship'] = 'field_witnesses_uid';
  $handler->display->display_options['filters']['uid_2']['value'] = '';
  $handler->display->display_options['filters']['uid_2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid_2']['expose']['operator_id'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['label'] = 'Affected';
  $handler->display->display_options['filters']['uid_2']['expose']['operator'] = 'uid_2_op';
  $handler->display->display_options['filters']['uid_2']['expose']['identifier'] = 'uid_2';
  $handler->display->display_options['filters']['uid_2']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['uid_2']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    8 => 0,
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
    9 => 0,
    20 => 0,
    14 => 0,
    21 => 0,
    3 => 0,
    22 => 0,
    23 => 0,
  );
  $handler->display->display_options['path'] = 'notations';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Social Notations (NEW)';
  $handler->display->display_options['menu']['description'] = 'All notations over time';
  $handler->display->display_options['menu']['weight'] = '99';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia-sh';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Players w/3 Yellows */
  $handler = $view->new_display('views_rules', 'Players w/3 Yellows', 'players_over_strike');
  $handler->display->display_options['display_description'] = 'Players w/3 Yellows';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '2';
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Players (field_players) */
  $handler->display->display_options['relationships']['field_players_uid']['id'] = 'field_players_uid';
  $handler->display->display_options['relationships']['field_players_uid']['table'] = 'field_data_field_players';
  $handler->display->display_options['relationships']['field_players_uid']['field'] = 'field_players_uid';
  $handler->display->display_options['relationships']['field_players_uid']['delta'] = '-1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Players (field_players) */
  $handler->display->display_options['arguments']['field_players_uid']['id'] = 'field_players_uid';
  $handler->display->display_options['arguments']['field_players_uid']['table'] = 'field_data_field_players';
  $handler->display->display_options['arguments']['field_players_uid']['field'] = 'field_players_uid';
  $handler->display->display_options['arguments']['field_players_uid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_players_uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_players_uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_players_uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_players_uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'strike' => 'strike',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  /* Filter criterion: Content: Post date */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'node';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['operator'] = '>';
  $handler->display->display_options['filters']['created']['value']['value'] = '-1 year';
  $handler->display->display_options['filters']['created']['value']['type'] = 'offset';
  /* Filter criterion: Content: Severity (field_severity) */
  $handler->display->display_options['filters']['field_severity_value']['id'] = 'field_severity_value';
  $handler->display->display_options['filters']['field_severity_value']['table'] = 'field_data_field_severity';
  $handler->display->display_options['filters']['field_severity_value']['field'] = 'field_severity_value';
  $handler->display->display_options['filters']['field_severity_value']['value'] = array(
    'Yellow' => 'Yellow',
  );
  $handler->display->display_options['rules_parameter'] = array(
    'field_players_uid' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Content: Players',
      'name' => 'field_players_uid',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'title' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'text',
      'label' => 'Details',
      'name' => 'title',
    ),
  );
  $export['strikes'] = $view;

  return $export;
}
