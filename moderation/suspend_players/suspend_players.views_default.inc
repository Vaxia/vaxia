<?php
/**
 * @file
 * suspend_players.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function suspend_players_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'suspensions';
  $view->description = 'Notations a player has been involved in';
  $view->tag = 'Strikes';
  $view->base_table = 'node';
  $view->human_name = 'Suspensions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Notations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create strike content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
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
    'title' => 'title',
    'field_documentation' => 'title',
    'field_category' => 'title',
    'field_players' => 'field_players',
    'field_witnesses' => 'field_witnesses',
    'field_social_members' => 'field_social_members',
    'field_results' => 'field_results',
    'field_yellow_strike' => 'field_yellow_strike',
    'field_red_strike' => 'field_red_strike',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = 'field_date_of_incident';
  $handler->display->display_options['style_options']['info'] = array(
    'field_date_of_incident' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '<br>',
      'empty_column' => 0,
    ),
    'field_documentation' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_category' => array(
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
    'field_results' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_yellow_strike' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_red_strike' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
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
.view-strikes ul{margin:0;padding-left:15px;}
</style>
<div>
Players involved include all players in this interaction - victim(s) and aggressor(s). It is the job of Social to sort out which is which - the notation is NOT the point to make that decision.</br>
Bystanders are any players who were involved witnesses or tried to get the situation to cool off but were not acting as members of the Social department. "Bystanders and witnesses" so to speak.</br>
Social members are members of the social department that got involved.
</div>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No notations available.';
  $handler->display->display_options['empty']['area']['format'] = 'wiki';
  /* Relationship: Content: Player (field_players) */
  $handler->display->display_options['relationships']['field_players_uid']['id'] = 'field_players_uid';
  $handler->display->display_options['relationships']['field_players_uid']['table'] = 'field_data_field_players';
  $handler->display->display_options['relationships']['field_players_uid']['field'] = 'field_players_uid';
  $handler->display->display_options['relationships']['field_players_uid']['delta'] = '-1';
  /* Relationship: Content: Players Bystanding (field_witnesses) */
  $handler->display->display_options['relationships']['field_witnesses_uid']['id'] = 'field_witnesses_uid';
  $handler->display->display_options['relationships']['field_witnesses_uid']['table'] = 'field_data_field_witnesses';
  $handler->display->display_options['relationships']['field_witnesses_uid']['field'] = 'field_witnesses_uid';
  $handler->display->display_options['relationships']['field_witnesses_uid']['delta'] = '-1';
  /* Relationship: Content: Social Members (field_social_members) */
  $handler->display->display_options['relationships']['field_social_members_uid']['id'] = 'field_social_members_uid';
  $handler->display->display_options['relationships']['field_social_members_uid']['table'] = 'field_data_field_social_members';
  $handler->display->display_options['relationships']['field_social_members_uid']['field'] = 'field_social_members_uid';
  $handler->display->display_options['relationships']['field_social_members_uid']['delta'] = '-1';
  /* Field: COUNT(Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count';
  $handler->display->display_options['fields']['nid']['label'] = 'Number yellows';
  /* Sort criterion: Content: Date of Incident (field_date_of_incident) */
  $handler->display->display_options['sorts']['field_date_of_incident_value']['id'] = 'field_date_of_incident_value';
  $handler->display->display_options['sorts']['field_date_of_incident_value']['table'] = 'field_data_field_date_of_incident';
  $handler->display->display_options['sorts']['field_date_of_incident_value']['field'] = 'field_date_of_incident_value';
  $handler->display->display_options['sorts']['field_date_of_incident_value']['order'] = 'DESC';
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

  /* Display: Check for 3 Yellows */
  $handler = $view->new_display('views_rules', 'Check for 3 Yellows', 'check_yellows');
  $handler->display->display_options['display_description'] = 'Check for 3 Yellows';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Player (field_struck_player) */
  $handler->display->display_options['arguments']['field_struck_player_uid']['id'] = 'field_struck_player_uid';
  $handler->display->display_options['arguments']['field_struck_player_uid']['table'] = 'field_data_field_struck_player';
  $handler->display->display_options['arguments']['field_struck_player_uid']['field'] = 'field_struck_player_uid';
  $handler->display->display_options['arguments']['field_struck_player_uid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_struck_player_uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_struck_player_uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_struck_player_uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_struck_player_uid']['summary_options']['items_per_page'] = '25';
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
  /* Filter criterion: Content: Severity (field_severity) */
  $handler->display->display_options['filters']['field_severity_value']['id'] = 'field_severity_value';
  $handler->display->display_options['filters']['field_severity_value']['table'] = 'field_data_field_severity';
  $handler->display->display_options['filters']['field_severity_value']['field'] = 'field_severity_value';
  $handler->display->display_options['filters']['field_severity_value']['value'] = array(
    'Yellow' => 'Yellow',
  );
  /* Filter criterion: Content: Date of Incident (field_date_of_incident) */
  $handler->display->display_options['filters']['field_date_of_incident_value']['id'] = 'field_date_of_incident_value';
  $handler->display->display_options['filters']['field_date_of_incident_value']['table'] = 'field_data_field_date_of_incident';
  $handler->display->display_options['filters']['field_date_of_incident_value']['field'] = 'field_date_of_incident_value';
  $handler->display->display_options['filters']['field_date_of_incident_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_date_of_incident_value']['default_date'] = '-1 year';
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = 'return ($row->nid < 2)';
  $handler->display->display_options['rules_parameter'] = array(
    'field_struck_player_uid' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Content: Player (field_struck_player)',
      'name' => 'field_struck_player_uid',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'nid' => array(
      'enabled' => 1,
      'rendered' => 1,
      'type' => 'integer',
      'label' => 'Yellow count',
      'name' => 'yellow_count',
    ),
  );

  /* Display: Check for Multiple Reds */
  $handler = $view->new_display('views_rules', 'Check for Multiple Reds', 'check_reds');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Check for Multiple Reds';
  $handler->display->display_options['display_description'] = 'Check for Multiple Reds';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Player (field_struck_player) */
  $handler->display->display_options['relationships']['field_struck_player_uid']['id'] = 'field_struck_player_uid';
  $handler->display->display_options['relationships']['field_struck_player_uid']['table'] = 'field_data_field_struck_player';
  $handler->display->display_options['relationships']['field_struck_player_uid']['field'] = 'field_struck_player_uid';
  $handler->display->display_options['relationships']['field_struck_player_uid']['delta'] = '-1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: COUNT(Content: Nid) */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['group_type'] = 'count';
  $handler->display->display_options['fields']['nid']['label'] = 'Number reds';
  /* Field: User: Banned until */
  $handler->display->display_options['fields']['field_banned_until']['id'] = 'field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['table'] = 'field_data_field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['field'] = 'field_banned_until';
  $handler->display->display_options['fields']['field_banned_until']['relationship'] = 'field_struck_player_uid';
  $handler->display->display_options['fields']['field_banned_until']['label'] = 'Currently banned';
  $handler->display->display_options['fields']['field_banned_until']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'Months Ban';
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '([nid] - 1) * 3';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'New Ban';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = '$ban = !empty($data->users_field_data_field_struck_player__field_data_field_banne)
  ? $data->users_field_data_field_struck_player__field_data_field_banne : \'now\';
$ban_for = ($row->nid > 1) ? ($row->nid - 1) * 3 : 1;
$new_ban = strtotime(\'+\' . $ban_for . \' months\', strtotime($ban));
return date(\'m/d/Y H:i:s\', $new_ban);
';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Player (field_struck_player) */
  $handler->display->display_options['arguments']['field_struck_player_uid']['id'] = 'field_struck_player_uid';
  $handler->display->display_options['arguments']['field_struck_player_uid']['table'] = 'field_data_field_struck_player';
  $handler->display->display_options['arguments']['field_struck_player_uid']['field'] = 'field_struck_player_uid';
  $handler->display->display_options['arguments']['field_struck_player_uid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['field_struck_player_uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_struck_player_uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_struck_player_uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_struck_player_uid']['summary_options']['items_per_page'] = '25';
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
  /* Filter criterion: Content: Severity (field_severity) */
  $handler->display->display_options['filters']['field_severity_value']['id'] = 'field_severity_value';
  $handler->display->display_options['filters']['field_severity_value']['table'] = 'field_data_field_severity';
  $handler->display->display_options['filters']['field_severity_value']['field'] = 'field_severity_value';
  $handler->display->display_options['filters']['field_severity_value']['value'] = array(
    'min' => '',
    'max' => '',
    'value' => '',
    'Red' => 'Red',
  );
  /* Filter criterion: Content: Date of Incident (field_date_of_incident) */
  $handler->display->display_options['filters']['field_date_of_incident_value']['id'] = 'field_date_of_incident_value';
  $handler->display->display_options['filters']['field_date_of_incident_value']['table'] = 'field_data_field_date_of_incident';
  $handler->display->display_options['filters']['field_date_of_incident_value']['field'] = 'field_date_of_incident_value';
  $handler->display->display_options['filters']['field_date_of_incident_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_date_of_incident_value']['default_date'] = '-1 year';
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = 'return ($row->nid < 1);';
  $handler->display->display_options['rules_parameter'] = array(
    'field_struck_player_uid' => array(
      'enabled' => 1,
      'type' => 'integer',
      'label' => 'Content: Player (field_struck_player)',
      'name' => 'field_struck_player_uid',
    ),
  );
  $handler->display->display_options['rules_variables'] = array(
    'nid' => array(
      'enabled' => 1,
      'rendered' => 1,
      'type' => 'integer',
      'label' => 'Red count',
      'name' => 'red_count',
    ),
    'field_banned_until' => array(
      'enabled' => 0,
      'rendered' => 0,
      'type' => '',
      'label' => '(field_struck_player) Banned until',
      'name' => 'field_banned_until',
    ),
    'expression' => array(
      'enabled' => 1,
      'rendered' => 0,
      'type' => 'integer',
      'label' => 'New ban months',
      'name' => 'new_ban_months',
    ),
    'php' => array(
      'enabled' => 1,
      'rendered' => 1,
      'type' => 'date',
      'label' => 'New Ban',
      'name' => 'new_ban',
    ),
  );
  $export['suspensions'] = $view;

  return $export;
}
