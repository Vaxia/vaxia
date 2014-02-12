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
    'taxonomy_forums' => 'taxonomy_forums',
    'title' => 'title',
    'field_session_held' => 'field_session_held',
    'field_characters_involved' => 'field_characters_involved',
    'body' => 'body',
    'field_rate_session' => 'field_rate_session',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'taxonomy_forums' => array(
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
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
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
  $view->description = 'Show a list of SHs and ASHs';
  $view->tag = 'Staff';
  $view->base_table = 'users';
  $view->human_name = 'Storytellers and Assistants';
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
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
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
  $handler->display->display_options['fields']['php']['php_value'] = '$output = \'Storyhost\';
$this_user = user_load($data->uid);
$this_role = isset($this_user->roles) ? $this_user->roles : array();
if (!isset($this_role[9])) {
  $output = \'Assistant Storyhost\';
}
return $output;';
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
    15 => '15',
    9 => '9',
  );
  $handler->display->display_options['filters']['rid']['group'] = 1;
  $handler->display->display_options['filters']['rid']['reduce_duplicates'] = TRUE;

  /* Display: Storytellers and Assistants */
  $handler = $view->new_display('page', 'Storytellers and Assistants', 'sh_and_ash');
  $handler->display->display_options['path'] = 'wiki/storytellers-and-assistants';
  $export['storytellers_and_assistants'] = $view;

  return $export;
}
