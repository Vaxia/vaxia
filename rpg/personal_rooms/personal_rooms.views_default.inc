<?php
/**
 * @file
 * personal_rooms.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function personal_rooms_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'personal_rooms';
  $view->description = 'Personal rooms';
  $view->tag = 'Personal rooms';
  $view->base_table = 'node';
  $view->human_name = 'Personal rooms';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Personal rooms';
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
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'title_1' => 'title_1',
    'name' => 'name',
    'field_room_description' => 'field_room_description',
    'field_is_personal_room' => 'field_is_personal_room',
    'field_private_room' => 'field_is_personal_room',
    'field_mature_posting' => 'field_is_personal_room',
    'field_locked_to_users' => 'field_is_personal_room',
    'field_users' => 'field_is_personal_room',
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
    'field_room_description' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_is_personal_room' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'field_private_room' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_mature_posting' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_locked_to_users' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_users' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'field_personal_room_nid';
  /* Relationship: Content: Personal room (field_personal_room) - reverse */
  $handler->display->display_options['relationships']['reverse_field_personal_room_node']['id'] = 'reverse_field_personal_room_node';
  $handler->display->display_options['relationships']['reverse_field_personal_room_node']['table'] = 'node';
  $handler->display->display_options['relationships']['reverse_field_personal_room_node']['field'] = 'reverse_field_personal_room_node';
  $handler->display->display_options['relationships']['reverse_field_personal_room_node']['label'] = 'Character';
  $handler->display->display_options['relationships']['reverse_field_personal_room_node']['required'] = TRUE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid_1']['relationship'] = 'reverse_field_personal_room_node';
  $handler->display->display_options['relationships']['uid_1']['label'] = 'Player';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Room name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'reverse_field_personal_room_node';
  $handler->display->display_options['fields']['title_1']['label'] = 'Character';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['name']['label'] = 'Player';
  /* Field: Content: Room Description */
  $handler->display->display_options['fields']['field_room_description']['id'] = 'field_room_description';
  $handler->display->display_options['fields']['field_room_description']['table'] = 'field_data_field_room_description';
  $handler->display->display_options['fields']['field_room_description']['field'] = 'field_room_description';
  /* Field: Content: Personal room */
  $handler->display->display_options['fields']['field_is_personal_room']['id'] = 'field_is_personal_room';
  $handler->display->display_options['fields']['field_is_personal_room']['table'] = 'field_data_field_is_personal_room';
  $handler->display->display_options['fields']['field_is_personal_room']['field'] = 'field_is_personal_room';
  $handler->display->display_options['fields']['field_is_personal_room']['label'] = 'Settings';
  /* Field: Content: Private room */
  $handler->display->display_options['fields']['field_private_room']['id'] = 'field_private_room';
  $handler->display->display_options['fields']['field_private_room']['table'] = 'field_data_field_private_room';
  $handler->display->display_options['fields']['field_private_room']['field'] = 'field_private_room';
  /* Field: Content: Mature posting */
  $handler->display->display_options['fields']['field_mature_posting']['id'] = 'field_mature_posting';
  $handler->display->display_options['fields']['field_mature_posting']['table'] = 'field_data_field_mature_posting';
  $handler->display->display_options['fields']['field_mature_posting']['field'] = 'field_mature_posting';
  /* Field: Content: Locked to users */
  $handler->display->display_options['fields']['field_locked_to_users']['id'] = 'field_locked_to_users';
  $handler->display->display_options['fields']['field_locked_to_users']['table'] = 'field_data_field_locked_to_users';
  $handler->display->display_options['fields']['field_locked_to_users']['field'] = 'field_locked_to_users';
  /* Field: Content: Users */
  $handler->display->display_options['fields']['field_users']['id'] = 'field_users';
  $handler->display->display_options['fields']['field_users']['table'] = 'field_data_field_users';
  $handler->display->display_options['fields']['field_users']['field'] = 'field_users';
  $handler->display->display_options['fields']['field_users']['delta_offset'] = '0';
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
    'rpg_chatroom' => 'rpg_chatroom',
  );

  /* Display: Personal rooms */
  $handler = $view->new_display('page', 'Personal rooms', 'personal_rooms');
  $handler->display->display_options['display_description'] = 'Personal rooms';
  $handler->display->display_options['path'] = 'character-sheets/personal-rooms';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Personal rooms';
  $handler->display->display_options['menu']['description'] = 'Personal rooms';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia-sh';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['personal_rooms'] = $view;

  return $export;
}
