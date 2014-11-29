<?php
/**
 * @file
 * ban_players.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function ban_players_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'ban_player';
  $view->description = 'View a list of players banned';
  $view->tag = 'Ban player';
  $view->base_table = 'users';
  $view->human_name = 'Ban player';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Banned IP';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer users';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'created' => 'created',
    'login' => 'login',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
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
    'login' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  /* Field: User: Last login */
  $handler->display->display_options['fields']['login']['id'] = 'login';
  $handler->display->display_options['fields']['login']['table'] = 'users';
  $handler->display->display_options['fields']['login']['field'] = 'login';
  $handler->display->display_options['fields']['login']['date_format'] = 'time ago';
  /* Field: User: Banned IP */
  $handler->display->display_options['fields']['field_banned_ip']['id'] = 'field_banned_ip';
  $handler->display->display_options['fields']['field_banned_ip']['table'] = 'field_data_field_banned_ip';
  $handler->display->display_options['fields']['field_banned_ip']['field'] = 'field_banned_ip';
  $handler->display->display_options['fields']['field_banned_ip']['delta_offset'] = '0';
  /* Field: User: Banned until */
  $handler->display->display_options['fields']['field_banned_until']['id'] = 'field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['table'] = 'field_data_field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['field'] = 'field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: User: Reason for ban */
  $handler->display->display_options['fields']['field_reason_for_ban']['id'] = 'field_reason_for_ban';
  $handler->display->display_options['fields']['field_reason_for_ban']['table'] = 'field_data_field_reason_for_ban';
  $handler->display->display_options['fields']['field_reason_for_ban']['field'] = 'field_reason_for_ban';
  /* Sort criterion: User: Name */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    31 => '31',
  );
  $handler->display->display_options['filters']['rid']['group'] = 1;

  /* Display: Banned */
  $handler = $view->new_display('page', 'Banned', 'banned');
  $handler->display->display_options['display_description'] = 'Players banned';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    21 => '21',
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'These players are in "cold storage". They were imported from an earlier version of the site, or have been deactivated as players for some reason. If the player returns, use this interface to find their account to reactivate it.';
  $handler->display->display_options['header']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  /* Field: User: Last login */
  $handler->display->display_options['fields']['login']['id'] = 'login';
  $handler->display->display_options['fields']['login']['table'] = 'users';
  $handler->display->display_options['fields']['login']['field'] = 'login';
  $handler->display->display_options['fields']['login']['date_format'] = 'time ago';
  /* Field: User: Banned IP */
  $handler->display->display_options['fields']['field_banned_ip']['id'] = 'field_banned_ip';
  $handler->display->display_options['fields']['field_banned_ip']['table'] = 'field_data_field_banned_ip';
  $handler->display->display_options['fields']['field_banned_ip']['field'] = 'field_banned_ip';
  $handler->display->display_options['fields']['field_banned_ip']['label'] = 'Banned IPs';
  $handler->display->display_options['fields']['field_banned_ip']['delta_offset'] = '0';
  /* Field: User: Banned until */
  $handler->display->display_options['fields']['field_banned_until']['id'] = 'field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['table'] = 'field_data_field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['field'] = 'field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['empty'] = 'PERMANENT';
  $handler->display->display_options['fields']['field_banned_until']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: User: Reason for ban */
  $handler->display->display_options['fields']['field_reason_for_ban']['id'] = 'field_reason_for_ban';
  $handler->display->display_options['fields']['field_reason_for_ban']['table'] = 'field_data_field_reason_for_ban';
  $handler->display->display_options['fields']['field_reason_for_ban']['field'] = 'field_reason_for_ban';
  $handler->display->display_options['fields']['field_reason_for_ban']['label'] = 'Reason for IP ban';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Roles */
  $handler->display->display_options['filters']['rid']['id'] = 'rid';
  $handler->display->display_options['filters']['rid']['table'] = 'users_roles';
  $handler->display->display_options['filters']['rid']['field'] = 'rid';
  $handler->display->display_options['filters']['rid']['value'] = array(
    31 => '31',
  );
  $handler->display->display_options['filters']['rid']['group'] = 1;
  /* Filter criterion: User: Name (raw) */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'users';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['name']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
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
  /* Filter criterion: User: E-mail */
  $handler->display->display_options['filters']['mail']['id'] = 'mail';
  $handler->display->display_options['filters']['mail']['table'] = 'users';
  $handler->display->display_options['filters']['mail']['field'] = 'mail';
  $handler->display->display_options['filters']['mail']['operator'] = 'contains';
  $handler->display->display_options['filters']['mail']['group'] = 1;
  $handler->display->display_options['filters']['mail']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator_id'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['label'] = 'E-mail';
  $handler->display->display_options['filters']['mail']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['operator'] = 'mail_op';
  $handler->display->display_options['filters']['mail']['expose']['identifier'] = 'mail';
  $handler->display->display_options['filters']['mail']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['mail']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['path'] = 'players/banned';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Ban player';
  $handler->display->display_options['menu']['description'] = 'Banned players';
  $handler->display->display_options['menu']['weight'] = '1001';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Banned IP */
  $handler = $view->new_display('views_rules', 'Banned IP', 'banned_users');
  $handler->display->display_options['display_description'] = 'Banned IP';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: User: Banned IP (field_banned_ip) */
  $handler->display->display_options['arguments']['field_banned_ip_value']['id'] = 'field_banned_ip_value';
  $handler->display->display_options['arguments']['field_banned_ip_value']['table'] = 'field_data_field_banned_ip';
  $handler->display->display_options['arguments']['field_banned_ip_value']['field'] = 'field_banned_ip_value';
  $handler->display->display_options['arguments']['field_banned_ip_value']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_banned_ip_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_banned_ip_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_banned_ip_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_banned_ip_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_banned_ip_value']['limit'] = '0';
  $handler->display->display_options['rules_parameter'] = array(
    'field_banned_ip_value' => array(
      'enabled' => 1,
      'type' => 'text',
      'label' => 'User: Banned IP (field_banned_ip)',
      'name' => 'field_banned_ip_value',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'uid' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'integer',
      'label' => 'Uid',
      'name' => 'uid',
    ),
    'name' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'text',
      'label' => 'Name',
      'name' => 'name',
    ),
    'created' => array(
      'enabled' => 0,
      'rendered' => 0,
      'type' => '',
      'label' => 'Created date',
      'name' => 'created',
    ),
    'login' => array(
      'enabled' => 0,
      'rendered' => 0,
      'type' => '',
      'label' => 'Last login',
      'name' => 'login',
    ),
    'field_banned_ip' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'list<text>',
      'label' => 'Banned IP',
      'name' => 'field_banned_ip',
    ),
    'field_banned_until' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'date',
      'label' => 'Banned until',
      'name' => 'field_banned_until',
    ),
    'field_reason_for_ban' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'text',
      'label' => 'Reason for ban',
      'name' => 'field_reason_for_ban',
    ),
  );
  $export['ban_player'] = $view;

  return $export;
}
