<?php
/**
 * @file
 * vaxia_wiki.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function vaxia_wiki_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'vaxia_wiki';
  $view->description = 'Vaxia Wiki';
  $view->tag = 'Vaxia Wiki';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia Wiki';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recently edited wiki';
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
    'name' => 'name',
    'created' => 'created',
    'name_1' => 'name_1',
    'last_updated' => 'last_updated',
    'field_additional_tagging' => 'field_additional_tagging',
    'field_tag_department' => 'field_additional_tagging',
    'field_tag_realm' => 'field_additional_tagging',
    'field_tag_type' => 'field_additional_tagging',
    'field_tag_location' => 'field_additional_tagging',
    'field_organization_type' => 'field_additional_tagging',
    'field_referencing_chat_rooms' => 'field_referencing_chat_rooms',
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
    'name_1' => array(
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
    'field_additional_tagging' => array(
      'align' => '',
      'separator' => ', ',
      'empty_column' => 0,
    ),
    'field_tag_department' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_tag_realm' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_tag_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_tag_location' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_organization_type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_referencing_chat_rooms' => array(
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
  /* Relationship: Content revision: User */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'node_revision';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
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
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'privatemsg_current_day';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['name_1']['label'] = 'Updated by';
  /* Field: Content: Updated/commented date */
  $handler->display->display_options['fields']['last_updated']['id'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['table'] = 'node_comment_statistics';
  $handler->display->display_options['fields']['last_updated']['field'] = 'last_updated';
  $handler->display->display_options['fields']['last_updated']['label'] = 'Updated date';
  $handler->display->display_options['fields']['last_updated']['date_format'] = 'short';
  $handler->display->display_options['fields']['last_updated']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Content: Categories */
  $handler->display->display_options['fields']['field_additional_tagging']['id'] = 'field_additional_tagging';
  $handler->display->display_options['fields']['field_additional_tagging']['table'] = 'field_data_field_additional_tagging';
  $handler->display->display_options['fields']['field_additional_tagging']['field'] = 'field_additional_tagging';
  $handler->display->display_options['fields']['field_additional_tagging']['delta_offset'] = '0';
  /* Field: Content: Department */
  $handler->display->display_options['fields']['field_tag_department']['id'] = 'field_tag_department';
  $handler->display->display_options['fields']['field_tag_department']['table'] = 'field_data_field_tag_department';
  $handler->display->display_options['fields']['field_tag_department']['field'] = 'field_tag_department';
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['field_tag_type']['id'] = 'field_tag_type';
  $handler->display->display_options['fields']['field_tag_type']['table'] = 'field_data_field_tag_type';
  $handler->display->display_options['fields']['field_tag_type']['field'] = 'field_tag_type';
  /* Field: Content: Location Type */
  $handler->display->display_options['fields']['field_tag_location']['id'] = 'field_tag_location';
  $handler->display->display_options['fields']['field_tag_location']['table'] = 'field_data_field_tag_location';
  $handler->display->display_options['fields']['field_tag_location']['field'] = 'field_tag_location';
  /* Field: Content: Organization Type */
  $handler->display->display_options['fields']['field_organization_type']['id'] = 'field_organization_type';
  $handler->display->display_options['fields']['field_organization_type']['table'] = 'field_data_field_organization_type';
  $handler->display->display_options['fields']['field_organization_type']['field'] = 'field_organization_type';
  /* Field: Content: Associated Chat Rooms */
  $handler->display->display_options['fields']['field_referencing_chat_rooms']['id'] = 'field_referencing_chat_rooms';
  $handler->display->display_options['fields']['field_referencing_chat_rooms']['table'] = 'field_data_field_referencing_chat_rooms';
  $handler->display->display_options['fields']['field_referencing_chat_rooms']['field'] = 'field_referencing_chat_rooms';
  $handler->display->display_options['fields']['field_referencing_chat_rooms']['empty'] = 'none';
  $handler->display->display_options['fields']['field_referencing_chat_rooms']['settings'] = array(
    'prefix_suffix' => 0,
  );
  /* Sort criterion: Content: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
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
    'wiki_page' => 'wiki_page',
  );

  /* Display: Recently edited wiki */
  $handler = $view->new_display('page', 'Recently edited wiki', 'page');
  $handler->display->display_options['display_description'] = 'Recently edited wiki';
  $handler->display->display_options['path'] = 'wiki/recently-edited';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Recently updated';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $export['vaxia_wiki'] = $view;

  $view = new view();
  $view->name = 'vaxia_wiki_admin';
  $view->description = 'Site wiki listings for wiki articles';
  $view->tag = 'Vaxia Wiki';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia wiki admin';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Site wiki search';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer nodes';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'input_required';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['text_input_required'] = '';
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'wiki';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'title' => 'title',
    'type' => 'type',
    'name' => 'name',
    'status' => 'status',
    'changed' => 'changed',
    'delete_node' => 'edit_node',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
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
    'type' => array(
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
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Book: Top level book */
  $handler->display->display_options['relationships']['bid']['id'] = 'bid';
  $handler->display->display_options['relationships']['bid']['table'] = 'book';
  $handler->display->display_options['relationships']['bid']['field'] = 'bid';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
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
    'action::views_bulk_operations_move_to_book_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_remove_from_book_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'medium';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'wiki_page' => 'wiki_page',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Categories (field_additional_tagging) */
  $handler->display->display_options['filters']['field_additional_tagging_tid']['id'] = 'field_additional_tagging_tid';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['table'] = 'field_data_field_additional_tagging';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['field'] = 'field_additional_tagging_tid';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['value'] = '';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_additional_tagging_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['operator_id'] = 'field_additional_tagging_tid_op';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['operator'] = 'field_additional_tagging_tid_op';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['identifier'] = 'field_additional_tagging_tid';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_additional_tagging_tid']['vocabulary'] = 'wiki_tags';
  /* Filter criterion: Content: Department (field_tag_department) */
  $handler->display->display_options['filters']['field_tag_department_tid']['id'] = 'field_tag_department_tid';
  $handler->display->display_options['filters']['field_tag_department_tid']['table'] = 'field_data_field_tag_department';
  $handler->display->display_options['filters']['field_tag_department_tid']['field'] = 'field_tag_department_tid';
  $handler->display->display_options['filters']['field_tag_department_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_department_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['operator_id'] = 'field_tag_department_tid_op';
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['label'] = 'Department';
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['operator'] = 'field_tag_department_tid_op';
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['identifier'] = 'field_tag_department_tid';
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_tag_department_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_department_tid']['vocabulary'] = 'department';
  /* Filter criterion: Content: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_realm_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['operator_id'] = 'field_tag_realm_tid_op';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['label'] = 'Realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['operator'] = 'field_tag_realm_tid_op';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['identifier'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  /* Filter criterion: Content: Type (field_tag_type) */
  $handler->display->display_options['filters']['field_tag_type_tid']['id'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['table'] = 'field_data_field_tag_type';
  $handler->display->display_options['filters']['field_tag_type_tid']['field'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['operator_id'] = 'field_tag_type_tid_op';
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['operator'] = 'field_tag_type_tid_op';
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['identifier'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_tag_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_type_tid']['vocabulary'] = 'setting_type';
  /* Filter criterion: Content: Location Type (field_tag_location) */
  $handler->display->display_options['filters']['field_tag_location_tid']['id'] = 'field_tag_location_tid';
  $handler->display->display_options['filters']['field_tag_location_tid']['table'] = 'field_data_field_tag_location';
  $handler->display->display_options['filters']['field_tag_location_tid']['field'] = 'field_tag_location_tid';
  $handler->display->display_options['filters']['field_tag_location_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_location_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['operator_id'] = 'field_tag_location_tid_op';
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['label'] = 'Location';
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['operator'] = 'field_tag_location_tid_op';
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['identifier'] = 'field_tag_location_tid';
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_tag_location_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_location_tid']['vocabulary'] = 'location_depth';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title_1']['id'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['table'] = 'node';
  $handler->display->display_options['filters']['title_1']['field'] = 'title';
  $handler->display->display_options['filters']['title_1']['relationship'] = 'bid';
  $handler->display->display_options['filters']['title_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator_id'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['label'] = 'In book';
  $handler->display->display_options['filters']['title_1']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['operator'] = 'title_1_op';
  $handler->display->display_options['filters']['title_1']['expose']['identifier'] = 'title_1';
  $handler->display->display_options['filters']['title_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title_1']['expose']['remember_roles'] = array(
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

  /* Display: Site wiki articles */
  $handler = $view->new_display('page', 'Site wiki articles', 'site_wiki_content');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Site wiki articles';
  $handler->display->display_options['display_description'] = 'The display of wiki entries on administrative content pages';
  $handler->display->display_options['path'] = 'admin/content/wiki-content';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Site wiki content';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Flagged delete */
  $handler = $view->new_display('page', 'Flagged delete', 'flagged_delete');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Flagged delete';
  $handler->display->display_options['display_description'] = 'Flagged delete';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    21 => '21',
    9 => '9',
  );
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No articles flagged.';
  $handler->display->display_options['empty']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: delete */
  $handler->display->display_options['relationships']['flag_content_rel_2']['id'] = 'flag_content_rel_2';
  $handler->display->display_options['relationships']['flag_content_rel_2']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel_2']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_2']['label'] = 'delete_flag';
  $handler->display->display_options['relationships']['flag_content_rel_2']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_2']['flag'] = 'delete';
  $handler->display->display_options['relationships']['flag_content_rel_2']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::flag_node_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'medium';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'wiki_page' => 'wiki_page',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Flags: Flagged */
  $handler->display->display_options['filters']['flagged_2']['id'] = 'flagged_2';
  $handler->display->display_options['filters']['flagged_2']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged_2']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged_2']['relationship'] = 'flag_content_rel_2';
  $handler->display->display_options['filters']['flagged_2']['value'] = '1';
  $handler->display->display_options['filters']['flagged_2']['group'] = 1;
  $handler->display->display_options['filters']['flagged_2']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['flagged_2']['expose']['label'] = 'Flagged for delete';
  $handler->display->display_options['filters']['flagged_2']['expose']['description'] = 'The article has been marked for deletion. The content is irrelevant (spam) or redundant with another article.';
  $handler->display->display_options['filters']['flagged_2']['expose']['operator'] = 'flagged_2_op';
  $handler->display->display_options['filters']['flagged_2']['expose']['identifier'] = 'flagged_delete';
  $handler->display->display_options['filters']['flagged_2']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['flagged_2']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 2;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '0';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  $handler->display->display_options['path'] = 'wiki/flagged-delete';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Flagged for delete';
  $handler->display->display_options['menu']['description'] = 'Flagged for delete';
  $handler->display->display_options['menu']['weight'] = '100';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Flagged rewrite */
  $handler = $view->new_display('page', 'Flagged rewrite', 'flagged_rewrite');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Flagged rewrite';
  $handler->display->display_options['display_description'] = 'Flagged rewrite';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    21 => '21',
    9 => '9',
  );
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No articles flagged.';
  $handler->display->display_options['empty']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: rewrite */
  $handler->display->display_options['relationships']['flag_content_rel_1']['id'] = 'flag_content_rel_1';
  $handler->display->display_options['relationships']['flag_content_rel_1']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel_1']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel_1']['label'] = 'rewrite_flag';
  $handler->display->display_options['relationships']['flag_content_rel_1']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel_1']['flag'] = 'rewrite';
  $handler->display->display_options['relationships']['flag_content_rel_1']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::flag_node_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'medium';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'wiki_page' => 'wiki_page',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Flags: Flagged */
  $handler->display->display_options['filters']['flagged_2']['id'] = 'flagged_2';
  $handler->display->display_options['filters']['flagged_2']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged_2']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged_2']['relationship'] = 'flag_content_rel_1';
  $handler->display->display_options['filters']['flagged_2']['value'] = '1';
  $handler->display->display_options['filters']['flagged_2']['group'] = 1;
  $handler->display->display_options['filters']['flagged_2']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['flagged_2']['expose']['label'] = 'Flagged for delete';
  $handler->display->display_options['filters']['flagged_2']['expose']['description'] = 'The article has been marked for deletion. The content is irrelevant (spam) or redundant with another article.';
  $handler->display->display_options['filters']['flagged_2']['expose']['operator'] = 'flagged_2_op';
  $handler->display->display_options['filters']['flagged_2']['expose']['identifier'] = 'flagged_delete';
  $handler->display->display_options['filters']['flagged_2']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['flagged_2']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 2;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '0';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  $handler->display->display_options['path'] = 'wiki/flagged-rewrite';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Flagged for rewrite';
  $handler->display->display_options['menu']['description'] = 'Flagged rewrite';
  $handler->display->display_options['menu']['weight'] = '100';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Flagged expand */
  $handler = $view->new_display('page', 'Flagged expand', 'flagged_expand');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Flagged expand';
  $handler->display->display_options['display_description'] = 'Flagged expand';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
    21 => '21',
    9 => '9',
  );
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No articles flagged.';
  $handler->display->display_options['empty']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Flags: expand */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'node';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['label'] = 'expand_flag';
  $handler->display->display_options['relationships']['flag_content_rel']['required'] = 0;
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'expand';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::flag_node_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'medium';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Field: Content: Published */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'node';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['not'] = 0;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['groups'] = array(
    1 => 'AND',
    2 => 'OR',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'wiki_page' => 'wiki_page',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Flags: Flagged */
  $handler->display->display_options['filters']['flagged_2']['id'] = 'flagged_2';
  $handler->display->display_options['filters']['flagged_2']['table'] = 'flagging';
  $handler->display->display_options['filters']['flagged_2']['field'] = 'flagged';
  $handler->display->display_options['filters']['flagged_2']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['filters']['flagged_2']['value'] = '1';
  $handler->display->display_options['filters']['flagged_2']['group'] = 1;
  $handler->display->display_options['filters']['flagged_2']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['flagged_2']['expose']['label'] = 'Flagged for delete';
  $handler->display->display_options['filters']['flagged_2']['expose']['description'] = 'The article has been marked for deletion. The content is irrelevant (spam) or redundant with another article.';
  $handler->display->display_options['filters']['flagged_2']['expose']['operator'] = 'flagged_2_op';
  $handler->display->display_options['filters']['flagged_2']['expose']['identifier'] = 'flagged_delete';
  $handler->display->display_options['filters']['flagged_2']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['flagged_2']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 2;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '0';
  $handler->display->display_options['filters']['status_1']['group'] = 2;
  $handler->display->display_options['path'] = 'wiki/flagged-expand';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Flagged for expand';
  $handler->display->display_options['menu']['description'] = 'Flagged for expand';
  $handler->display->display_options['menu']['weight'] = '100';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['vaxia_wiki_admin'] = $view;

  $view = new view();
  $view->name = 'vaxia_wiki_search';
  $view->description = 'Vaxia Wiki Search';
  $view->tag = 'Vaxia Wiki';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia Wiki Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Wiki search';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'input_required';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['text_input_required'] = '';
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'wiki';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'title' => 'title',
    'type' => 'type',
    'name' => 'name',
    'status' => 'status',
    'changed' => 'changed',
    'delete_node' => 'edit_node',
    'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'views_bulk_operations' => array(
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
    'type' => array(
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
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<style>
.view-display-id-wiki_search .view-filters{border:1px solid;padding:0.5em}
.view-display-id-wiki_search .views-exposed-widget{float:left;}
.view-display-id-wiki_search .views-exposed-widget input.form-text{width:80px;}
.view-display-id-wiki_search .views-submit-button{clear:left;}
</style>
Search for a wiki article by tag(s).';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Relationship: Book: Top level book */
  $handler->display->display_options['relationships']['bid']['id'] = 'bid';
  $handler->display->display_options['relationships']['bid']['table'] = 'book';
  $handler->display->display_options['relationships']['bid']['field'] = 'bid';
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
  $handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
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
    'action::views_bulk_operations_move_to_book_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_remove_from_book_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'medium';
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  /* Field: Content: Delete link */
  $handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
  $handler->display->display_options['fields']['delete_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = '';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'wiki_page' => 'wiki_page',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Body (body) */
  $handler->display->display_options['filters']['body_value']['id'] = 'body_value';
  $handler->display->display_options['filters']['body_value']['table'] = 'field_data_body';
  $handler->display->display_options['filters']['body_value']['field'] = 'body_value';
  $handler->display->display_options['filters']['body_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['body_value']['group'] = 1;
  $handler->display->display_options['filters']['body_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['body_value']['expose']['operator_id'] = 'body_value_op';
  $handler->display->display_options['filters']['body_value']['expose']['label'] = 'Content';
  $handler->display->display_options['filters']['body_value']['expose']['operator'] = 'body_value_op';
  $handler->display->display_options['filters']['body_value']['expose']['identifier'] = 'body_value';
  $handler->display->display_options['filters']['body_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['body_value']['expose']['remember_roles'] = array(
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
    33 => 0,
  );
  /* Filter criterion: Content: Categories (field_additional_tagging) */
  $handler->display->display_options['filters']['field_additional_tagging_tid']['id'] = 'field_additional_tagging_tid';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['table'] = 'field_data_field_additional_tagging';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['field'] = 'field_additional_tagging_tid';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['value'] = '';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_additional_tagging_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['operator_id'] = 'field_additional_tagging_tid_op';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['label'] = 'Category';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['operator'] = 'field_additional_tagging_tid_op';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['identifier'] = 'field_additional_tagging_tid';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_additional_tagging_tid']['expose']['remember_roles'] = array(
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
    33 => 0,
  );
  $handler->display->display_options['filters']['field_additional_tagging_tid']['vocabulary'] = 'wiki_tags';
  $handler->display->display_options['filters']['field_additional_tagging_tid']['error_message'] = FALSE;
  /* Filter criterion: Content: Department (field_tag_department) */
  $handler->display->display_options['filters']['field_tag_department_tid']['id'] = 'field_tag_department_tid';
  $handler->display->display_options['filters']['field_tag_department_tid']['table'] = 'field_data_field_tag_department';
  $handler->display->display_options['filters']['field_tag_department_tid']['field'] = 'field_tag_department_tid';
  $handler->display->display_options['filters']['field_tag_department_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_department_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['operator_id'] = 'field_tag_department_tid_op';
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['label'] = 'Department';
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['operator'] = 'field_tag_department_tid_op';
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['identifier'] = 'field_tag_department_tid';
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_tag_department_tid']['expose']['remember_roles'] = array(
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
    33 => 0,
  );
  $handler->display->display_options['filters']['field_tag_department_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_department_tid']['vocabulary'] = 'department';
  $handler->display->display_options['filters']['field_tag_department_tid']['error_message'] = FALSE;
  /* Filter criterion: Content: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_realm_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['operator_id'] = 'field_tag_realm_tid_op';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['label'] = 'Realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['operator'] = 'field_tag_realm_tid_op';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['identifier'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['remember_roles'] = array(
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
    33 => 0,
  );
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['error_message'] = FALSE;
  /* Filter criterion: Content: Type (field_tag_type) */
  $handler->display->display_options['filters']['field_tag_type_tid']['id'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['table'] = 'field_data_field_tag_type';
  $handler->display->display_options['filters']['field_tag_type_tid']['field'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['operator_id'] = 'field_tag_type_tid_op';
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['operator'] = 'field_tag_type_tid_op';
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['identifier'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_tag_type_tid']['expose']['remember_roles'] = array(
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
    33 => 0,
  );
  $handler->display->display_options['filters']['field_tag_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_type_tid']['vocabulary'] = 'setting_type';
  $handler->display->display_options['filters']['field_tag_type_tid']['error_message'] = FALSE;
  /* Filter criterion: Content: Location Type (field_tag_location) */
  $handler->display->display_options['filters']['field_tag_location_tid']['id'] = 'field_tag_location_tid';
  $handler->display->display_options['filters']['field_tag_location_tid']['table'] = 'field_data_field_tag_location';
  $handler->display->display_options['filters']['field_tag_location_tid']['field'] = 'field_tag_location_tid';
  $handler->display->display_options['filters']['field_tag_location_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_location_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['operator_id'] = 'field_tag_location_tid_op';
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['label'] = 'Location';
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['operator'] = 'field_tag_location_tid_op';
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['identifier'] = 'field_tag_location_tid';
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_tag_location_tid']['expose']['remember_roles'] = array(
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
    33 => 0,
  );
  $handler->display->display_options['filters']['field_tag_location_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_location_tid']['vocabulary'] = 'location_depth';
  $handler->display->display_options['filters']['field_tag_location_tid']['error_message'] = FALSE;

  /* Display: Wiki search block */
  $handler = $view->new_display('block', 'Wiki search block', 'wiki_search_block');
  $handler->display->display_options['display_description'] = 'Wiki search block';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['defaults']['query'] = FALSE;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'input_required';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
  $handler->display->display_options['exposed_form']['options']['text_input_required'] = '';
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'wiki';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'title' => 'title',
    'body' => 'body',
  );
  $handler->display->display_options['row_options']['separator'] = ' - ';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No wiki pages found matching, or no search performed.';
  $handler->display->display_options['empty']['area']['format'] = 'wiki';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = 'Type';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '140';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['block_description'] = 'Wiki search';

  /* Display: Wiki search */
  $handler = $view->new_display('page', 'Wiki search', 'wiki_search');
  $handler->display->display_options['display_description'] = 'Wiki search';
  $handler->display->display_options['path'] = 'search/wiki';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Wiki search';
  $handler->display->display_options['menu']['description'] = 'Wiki search';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['vaxia_wiki_search'] = $view;

  $view = new view();
  $view->name = 'wiki_display_sessions';
  $view->description = 'A list of your sessions';
  $view->tag = 'Vaxia Wiki';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia wiki sessions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sirian sessions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = '3600';
  $handler->display->display_options['cache']['results_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['output_lifespan'] = '3600';
  $handler->display->display_options['cache']['output_lifespan_custom'] = '0';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
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
  /* Field: Content: Event held */
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
  /* Filter criterion: Content: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['value'] = array(
    725 => '725',
  );
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['hierarchy'] = 1;

  /* Display: Vaxia Sessions */
  $handler = $view->new_display('page', 'Vaxia Sessions', 'wiki_vaxia_sessions');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Vaxia sessions';
  $handler->display->display_options['display_description'] = 'A list of all sessions';
  $handler->display->display_options['path'] = 'wiki/vaxia/session-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Vaxia sessions';
  $handler->display->display_options['menu']['description'] = 'Sessions held in the Vaxia setting';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Sirian Sessions */
  $handler = $view->new_display('page', 'Sirian Sessions', 'wiki_sirian_sessions');
  $handler->display->display_options['display_description'] = 'A list of all Sirian sessions';
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
    'session_report' => 'session_report',
  );
  /* Filter criterion: Content: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['value'] = array(
    726 => '726',
  );
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['hierarchy'] = 1;
  $handler->display->display_options['path'] = 'wiki/sirian/session-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Sirian sessions';
  $handler->display->display_options['menu']['description'] = 'Sessions held in the Sirian setting';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Vaxia Sessions Calendar */
  $handler = $view->new_display('feed', 'Vaxia Sessions Calendar', 'feed_1');
  $handler->display->display_options['display_description'] = 'Vaxia Sessions Calendar';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'date_ical';
  $handler->display->display_options['style_options']['cal_name'] = 'Vaxia Sessions';
  $handler->display->display_options['row_plugin'] = 'date_ical';
  $handler->display->display_options['row_options']['date_field'] = 'field_data_field_session_held.field_session_held_value';
  $handler->display->display_options['row_options']['summary_field'] = 'default_title';
  $handler->display->display_options['row_options']['location_field'] = 'none';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Event held (field_session_held) */
  $handler->display->display_options['sorts']['field_session_held_value']['id'] = 'field_session_held_value';
  $handler->display->display_options['sorts']['field_session_held_value']['table'] = 'field_data_field_session_held';
  $handler->display->display_options['sorts']['field_session_held_value']['field'] = 'field_session_held_value';
  $handler->display->display_options['sorts']['field_session_held_value']['order'] = 'DESC';
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
    'session_report' => 'session_report',
  );
  /* Filter criterion: Content: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['value'] = array(
    725 => '725',
  );
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['hierarchy'] = 1;
  $handler->display->display_options['path'] = 'sessions/calendar/vaxia';
  $handler->display->display_options['sitename_title'] = 0;

  /* Display: Sirian Sessions Calendar */
  $handler = $view->new_display('feed', 'Sirian Sessions Calendar', 'feed_2');
  $handler->display->display_options['display_description'] = 'Sirian Sessions Calendar';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'date_ical';
  $handler->display->display_options['style_options']['cal_name'] = 'Sirian Sessions';
  $handler->display->display_options['row_plugin'] = 'date_ical';
  $handler->display->display_options['row_options']['date_field'] = 'field_data_field_session_held.field_session_held_value';
  $handler->display->display_options['row_options']['summary_field'] = 'default_title';
  $handler->display->display_options['row_options']['location_field'] = 'none';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Event held (field_session_held) */
  $handler->display->display_options['sorts']['field_session_held_value']['id'] = 'field_session_held_value';
  $handler->display->display_options['sorts']['field_session_held_value']['table'] = 'field_data_field_session_held';
  $handler->display->display_options['sorts']['field_session_held_value']['field'] = 'field_session_held_value';
  $handler->display->display_options['sorts']['field_session_held_value']['order'] = 'DESC';
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
    'session_report' => 'session_report',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['value'] = array(
    726 => '726',
  );
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['hierarchy'] = 1;
  $handler->display->display_options['path'] = 'sessions/calendar/sirian';
  $export['wiki_display_sessions'] = $view;

  $view = new view();
  $view->name = 'wiki_governance';
  $view->description = 'Vaxia Wiki Governance';
  $view->tag = 'Vaxia Wiki';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia Wiki Governance';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Wiki governance';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'webform_form_body' => 'title',
    'webform_results' => 'title',
    'changed' => 'changed',
    'webform_submission_count_node' => 'title',
    'field_associated_files' => 'title',
    'field_associated_forum_threads' => 'title',
    'body' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'webform_form_body' => array(
      'align' => '',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
    'webform_results' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'webform_submission_count_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_associated_files' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_associated_forum_threads' => array(
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
  );
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Closed on date';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['fields']['changed']['second_date_format'] = 'privatemsg_current_day';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Governance Proposal';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['text'] = '<b>[title]</b>';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Associated Files */
  $handler->display->display_options['fields']['field_associated_files']['id'] = 'field_associated_files';
  $handler->display->display_options['fields']['field_associated_files']['table'] = 'field_data_field_associated_files';
  $handler->display->display_options['fields']['field_associated_files']['field'] = 'field_associated_files';
  $handler->display->display_options['fields']['field_associated_files']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_associated_files']['delta_offset'] = '0';
  /* Field: Content: Associated Forum Threads */
  $handler->display->display_options['fields']['field_associated_forum_threads']['id'] = 'field_associated_forum_threads';
  $handler->display->display_options['fields']['field_associated_forum_threads']['table'] = 'field_data_field_associated_forum_threads';
  $handler->display->display_options['fields']['field_associated_forum_threads']['field'] = 'field_associated_forum_threads';
  $handler->display->display_options['fields']['field_associated_forum_threads']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  /* Field: Webform: Webform form body */
  $handler->display->display_options['fields']['webform_form_body']['id'] = 'webform_form_body';
  $handler->display->display_options['fields']['webform_form_body']['table'] = 'node';
  $handler->display->display_options['fields']['webform_form_body']['field'] = 'webform_form_body';
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
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
    'proposal' => 'proposal',
  );
  /* Filter criterion: Content: Type (field_proposal_type) */
  $handler->display->display_options['filters']['field_proposal_type_value']['id'] = 'field_proposal_type_value';
  $handler->display->display_options['filters']['field_proposal_type_value']['table'] = 'field_data_field_proposal_type';
  $handler->display->display_options['filters']['field_proposal_type_value']['field'] = 'field_proposal_type_value';
  $handler->display->display_options['filters']['field_proposal_type_value']['value'] = array(
    'amendment' => 'amendment',
  );
  /* Filter criterion: Webform: Status */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'webform';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '0';

  /* Display: Wiki governance list */
  $handler = $view->new_display('page', 'Wiki governance list', 'page');
  $handler->display->display_options['display_description'] = 'Wiki governance list';
  $handler->display->display_options['path'] = 'wiki/site-governance';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Site governance';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['wiki_governance'] = $view;

  $view = new view();
  $view->name = 'wiki_npcs_list';
  $view->description = 'Vaxia Wiki NPCs';
  $view->tag = 'Vaxia Wiki';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia Wiki NPCs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Setting NPCs';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_titles' => 'title',
    'title' => 'title',
    'field_last_name' => 'title',
    'name' => 'name',
    'field_realm' => 'field_realm',
    'field_tag_realm' => 'field_tag_realm',
    'field_race' => 'field_race',
    'field_sirian_species' => 'field_race',
    'field_description' => 'field_description',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
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
    'field_last_name' => array(
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
    'field_tag_realm' => array(
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
    'field_sirian_species' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_description' => array(
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
  $handler->display->display_options['header']['area']['content'] = '<style>
.view-id-wiki_npcs_list .view-filters{border:1px solid;padding:0.5em}
.view-id-wiki_npcs_list .views-exposed-widget{float:left;}
.view-id-wiki_npcs_list .views-exposed-widget input.form-text{width:80px;}
.view-id-wiki_npcs_list .views-submit-button{clear:left;}
</style>
Search for an NPC.';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Content: Titles */
  $handler->display->display_options['fields']['field_titles']['id'] = 'field_titles';
  $handler->display->display_options['fields']['field_titles']['table'] = 'field_data_field_titles';
  $handler->display->display_options['fields']['field_titles']['field'] = 'field_titles';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Last Name */
  $handler->display->display_options['fields']['field_last_name']['id'] = 'field_last_name';
  $handler->display->display_options['fields']['field_last_name']['table'] = 'field_data_field_last_name';
  $handler->display->display_options['fields']['field_last_name']['field'] = 'field_last_name';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Storyhost';
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['type'] = 'taxonomy_term_reference_plain';
  /* Field: Content: Vaxia Species */
  $handler->display->display_options['fields']['field_race']['id'] = 'field_race';
  $handler->display->display_options['fields']['field_race']['table'] = 'field_data_field_race';
  $handler->display->display_options['fields']['field_race']['field'] = 'field_race';
  $handler->display->display_options['fields']['field_race']['label'] = 'Species';
  /* Field: Content: Sirian Species */
  $handler->display->display_options['fields']['field_sirian_species']['id'] = 'field_sirian_species';
  $handler->display->display_options['fields']['field_sirian_species']['table'] = 'field_data_field_sirian_species';
  $handler->display->display_options['fields']['field_sirian_species']['field'] = 'field_sirian_species';
  /* Field: Field: Description */
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
  /* Filter criterion: Content: This is an NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
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
    6 => '6',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
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
    21 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
  );
  $handler->display->display_options['filters']['sid']['expose']['reduce'] = TRUE;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'NPC Name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Background (field_background) */
  $handler->display->display_options['filters']['field_background_value']['id'] = 'field_background_value';
  $handler->display->display_options['filters']['field_background_value']['table'] = 'field_data_field_background';
  $handler->display->display_options['filters']['field_background_value']['field'] = 'field_background_value';
  $handler->display->display_options['filters']['field_background_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_background_value']['group'] = 1;
  $handler->display->display_options['filters']['field_background_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_background_value']['expose']['operator_id'] = 'field_background_value_op';
  $handler->display->display_options['filters']['field_background_value']['expose']['label'] = 'Background';
  $handler->display->display_options['filters']['field_background_value']['expose']['operator'] = 'field_background_value_op';
  $handler->display->display_options['filters']['field_background_value']['expose']['identifier'] = 'field_background_value';
  $handler->display->display_options['filters']['field_background_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_background_value']['expose']['remember_roles'] = array(
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

  /* Display: Setting NPCs */
  $handler = $view->new_display('page', 'Setting NPCs', 'npcs_list');
  $handler->display->display_options['display_description'] = 'List out available NPCs';
  $handler->display->display_options['path'] = 'wiki/npcs-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'NPCs by Setting';
  $handler->display->display_options['menu']['description'] = 'A list of all NPCs available in the wiki';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['wiki_npcs_list'] = $view;

  $view = new view();
  $view->name = 'wiki_organizations_list';
  $view->description = 'Vaxia Wiki Orgs and Govs';
  $view->tag = 'Vaxia Wiki';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia Wiki Orgs and Govs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Setting Organizations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_tag_realm' => 'field_tag_realm',
    'field_organization_type' => 'field_organization_type',
    'body' => 'body',
    'field_provides_reputation' => 'field_provides_reputation',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'field_tag_realm' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_organization_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
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
    'field_provides_reputation' => array(
      'sortable' => 1,
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
  $handler->display->display_options['header']['area']['content'] = '<style>
.view-id-wiki_organizations_list .view-filters{border:1px solid;padding:0.5em}
.view-id-wiki_organizations_list .views-exposed-widget{float:left;}
.view-id-wiki_organizations_list .views-exposed-widget input.form-text{width:80px;}
.view-id-wiki_organizations_list .views-submit-button{clear:left;}
</style>
A list of all organizations in the system.';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Relationship: Content: Type (field_tag_type) */
  $handler->display->display_options['relationships']['field_tag_type_tid']['id'] = 'field_tag_type_tid';
  $handler->display->display_options['relationships']['field_tag_type_tid']['table'] = 'field_data_field_tag_type';
  $handler->display->display_options['relationships']['field_tag_type_tid']['field'] = 'field_tag_type_tid';
  $handler->display->display_options['relationships']['field_tag_type_tid']['label'] = 'Type';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['delta_offset'] = '0';
  /* Field: Content: Organization Type */
  $handler->display->display_options['fields']['field_organization_type']['id'] = 'field_organization_type';
  $handler->display->display_options['fields']['field_organization_type']['table'] = 'field_data_field_organization_type';
  $handler->display->display_options['fields']['field_organization_type']['field'] = 'field_organization_type';
  $handler->display->display_options['fields']['field_organization_type']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['alter']['max_length'] = '200';
  $handler->display->display_options['fields']['body']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['body']['alter']['trim'] = TRUE;
  /* Field: Content: Provides reputation */
  $handler->display->display_options['fields']['field_provides_reputation']['id'] = 'field_provides_reputation';
  $handler->display->display_options['fields']['field_provides_reputation']['table'] = 'field_data_field_provides_reputation';
  $handler->display->display_options['fields']['field_provides_reputation']['field'] = 'field_provides_reputation';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['expose']['label'] = 'Title';
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
    'wiki_page' => 'wiki_page',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Type (field_tag_type) */
  $handler->display->display_options['filters']['field_tag_type_tid']['id'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['table'] = 'field_data_field_tag_type';
  $handler->display->display_options['filters']['field_tag_type_tid']['field'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['value'] = array(
    0 => '729',
  );
  $handler->display->display_options['filters']['field_tag_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_type_tid']['vocabulary'] = 'setting_type';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    30 => 0,
    8 => 0,
    9 => 0,
    21 => 0,
    24 => 0,
    26 => 0,
    27 => 0,
    19 => 0,
    28 => 0,
    29 => 0,
    12 => 0,
    34 => 0,
    35 => 0,
    25 => 0,
    13 => 0,
    10 => 0,
    17 => 0,
    15 => 0,
    22 => 0,
    23 => 0,
    31 => 0,
    32 => 0,
  );
  /* Filter criterion: Content: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['value'] = array(
    725 => '725',
    726 => '726',
  );
  $handler->display->display_options['filters']['field_tag_realm_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_realm_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['operator_id'] = 'field_tag_realm_tid_op';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['label'] = 'Realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['operator'] = 'field_tag_realm_tid_op';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['identifier'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_tag_realm_tid']['expose']['reduce'] = TRUE;
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  /* Filter criterion: Content: Provides reputation (field_provides_reputation) */
  $handler->display->display_options['filters']['field_provides_reputation_value']['id'] = 'field_provides_reputation_value';
  $handler->display->display_options['filters']['field_provides_reputation_value']['table'] = 'field_data_field_provides_reputation';
  $handler->display->display_options['filters']['field_provides_reputation_value']['field'] = 'field_provides_reputation_value';
  $handler->display->display_options['filters']['field_provides_reputation_value']['value'] = array(
    'all' => 'all',
    0 => '0',
    1 => '1',
  );
  $handler->display->display_options['filters']['field_provides_reputation_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['operator_id'] = 'field_provides_reputation_value_op';
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['label'] = 'Provides reputation';
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['operator'] = 'field_provides_reputation_value_op';
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['identifier'] = 'field_provides_reputation_value';
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['reduce'] = TRUE;

  /* Display: Setting Organizations */
  $handler = $view->new_display('page', 'Setting Organizations', 'setting_orgs');
  $handler->display->display_options['display_description'] = 'List out available Organizations';
  $handler->display->display_options['path'] = 'wiki/organizations-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Organizations by Setting';
  $handler->display->display_options['menu']['description'] = 'A list of all Organizations available in the wiki';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: ERef: Setting Orgs */
  $handler = $view->new_display('entityreference', 'ERef: Setting Orgs', 'eref_setting_orgs');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Wiki Organizations';
  $handler->display->display_options['display_description'] = 'ERef: Setting Orgs';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'title' => 'title',
    'field_tag_realm' => 0,
    'field_organization_type' => 0,
    'body' => 0,
    'field_provides_reputation' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['label'] = '';
  $handler->display->display_options['fields']['field_tag_realm']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_tag_realm']['alter']['text'] = ' ([field_tag_realm])';
  $handler->display->display_options['fields']['field_tag_realm']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'wiki_page' => 'wiki_page',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Type (field_tag_type) */
  $handler->display->display_options['filters']['field_tag_type_tid']['id'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['table'] = 'field_data_field_tag_type';
  $handler->display->display_options['filters']['field_tag_type_tid']['field'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['value'] = array(
    0 => '729',
  );
  $handler->display->display_options['filters']['field_tag_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_type_tid']['vocabulary'] = 'setting_type';
  /* Filter criterion: Content: Provides reputation (field_provides_reputation) */
  $handler->display->display_options['filters']['field_provides_reputation_value']['id'] = 'field_provides_reputation_value';
  $handler->display->display_options['filters']['field_provides_reputation_value']['table'] = 'field_data_field_provides_reputation';
  $handler->display->display_options['filters']['field_provides_reputation_value']['field'] = 'field_provides_reputation_value';
  $handler->display->display_options['filters']['field_provides_reputation_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_provides_reputation_value']['group'] = 1;
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['operator_id'] = 'field_provides_reputation_value_op';
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['label'] = 'Provides reputation';
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['operator'] = 'field_provides_reputation_value_op';
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['identifier'] = 'field_provides_reputation_value';
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_provides_reputation_value']['expose']['reduce'] = TRUE;

  /* Display: ERef: Setting Govs */
  $handler = $view->new_display('entityreference', 'ERef: Setting Govs', 'eref_setting_gov');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Wiki Organizations';
  $handler->display->display_options['display_description'] = 'ERef: Setting Govs';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'title' => 'title',
    'field_tag_realm' => 0,
    'field_organization_type' => 0,
    'body' => 0,
    'field_provides_reputation' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['label'] = '';
  $handler->display->display_options['fields']['field_tag_realm']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_tag_realm']['alter']['text'] = ' ([field_tag_realm])';
  $handler->display->display_options['fields']['field_tag_realm']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'wiki_page' => 'wiki_page',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Type (field_tag_type) */
  $handler->display->display_options['filters']['field_tag_type_tid']['id'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['table'] = 'field_data_field_tag_type';
  $handler->display->display_options['filters']['field_tag_type_tid']['field'] = 'field_tag_type_tid';
  $handler->display->display_options['filters']['field_tag_type_tid']['value'] = array(
    0 => '1251',
  );
  $handler->display->display_options['filters']['field_tag_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_tag_type_tid']['vocabulary'] = 'setting_type';
  $export['wiki_organizations_list'] = $view;

  return $export;
}
