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
  $handler->display->display_options['title'] = 'Sirian Sessions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = '3600';
  $handler->display->display_options['cache']['results_lifespan_custom'] = '0';
  $handler->display->display_options['cache']['output_lifespan'] = '3600';
  $handler->display->display_options['cache']['output_lifespan_custom'] = '0';
  $handler->display->display_options['query']['type'] = 'views_query';
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
  /* Field: Content: Session held */
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
  /* Contextual filter: Content: Realm (field_realm) */
  $handler->display->display_options['arguments']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['arguments']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['arguments']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['arguments']['field_realm_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_realm_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_realm_value']['default_argument_options']['argument'] = 'Vaxia';
  $handler->display->display_options['arguments']['field_realm_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_realm_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_realm_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_realm_value']['validate']['fail'] = 'ignore';
  $handler->display->display_options['arguments']['field_realm_value']['limit'] = '0';
  $handler->display->display_options['arguments']['field_realm_value']['case'] = 'ucfirst';
  $handler->display->display_options['arguments']['field_realm_value']['transform_dash'] = TRUE;
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

  /* Display: Vaxia Sessions */
  $handler = $view->new_display('page', 'Vaxia Sessions', 'wiki_vaxia_sessions');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Vaxia Sessions';
  $handler->display->display_options['display_description'] = 'A list of all sessions';
  $handler->display->display_options['path'] = 'wiki/vaxia/session-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Vaxia Sessions';
  $handler->display->display_options['menu']['description'] = 'Sessions held in the Vaxia setting';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Sirian Sessions */
  $handler = $view->new_display('page', 'Sirian Sessions', 'wiki_sirian_sessions');
  $handler->display->display_options['display_description'] = 'A list of all Sirian sessions';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Realm (field_realm) */
  $handler->display->display_options['arguments']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['arguments']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['arguments']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['arguments']['field_realm_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_realm_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_realm_value']['default_argument_options']['argument'] = 'Sirian';
  $handler->display->display_options['arguments']['field_realm_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_realm_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_realm_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_realm_value']['validate']['fail'] = 'ignore';
  $handler->display->display_options['arguments']['field_realm_value']['limit'] = '0';
  $handler->display->display_options['arguments']['field_realm_value']['case'] = 'ucfirst';
  $handler->display->display_options['arguments']['field_realm_value']['transform_dash'] = TRUE;
  $handler->display->display_options['path'] = 'wiki/sirian/session-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Sirian Sessions';
  $handler->display->display_options['menu']['description'] = 'Sessions held in the Sirian setting';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['wiki_display_sessions'] = $view;

  $view = new view();
  $view->name = 'wiki_npcs_list';
  $view->description = 'NPC master list for wiki display';
  $view->tag = 'Vaxia Wiki';
  $view->base_table = 'node';
  $view->human_name = 'Vaxia wiki NPCs List';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'NPCs List';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '35';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'title' => 'title',
    'name' => 'name',
    'field_realm' => 'field_realm',
    'field_race' => 'field_race',
    'field_description' => 'field_description',
    'field_last_name' => 'title',
    'field_titles' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
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
      'separator' => ' ',
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
    'field_race' => array(
      'sortable' => 1,
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
    'field_last_name' => array(
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
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::node_assign_owner_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::workflow_vbo_given_state_action' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::workflow_vbo_next_state_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::clone_action_clone' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::flag_node_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_sticky_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_make_unsticky_action' => array(
      'selected' => 0,
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
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::node_promote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_publish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpromote_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::mollom_action_unpublish_node' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_save_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::node_unpublish_by_keyword_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::pathauto_node_update_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
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
  $handler->display->display_options['fields']['field_realm']['id'] = 'field_realm';
  $handler->display->display_options['fields']['field_realm']['table'] = 'field_data_field_realm';
  $handler->display->display_options['fields']['field_realm']['field'] = 'field_realm';
  /* Field: Content: Species */
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
    21 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
    22 => 0,
    23 => 0,
  );
  $handler->display->display_options['filters']['sid']['expose']['reduce'] = TRUE;
  /* Filter criterion: Content: Realm (field_realm) */
  $handler->display->display_options['filters']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['filters']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['value'] = array(
    'Vaxia' => 'Vaxia',
    'Sirian' => 'Sirian',
  );
  $handler->display->display_options['filters']['field_realm_value']['group'] = 1;
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
    4 => 0,
    3 => 0,
    11 => 0,
  );
  /* Filter criterion: Content: Species (field_race) */
  $handler->display->display_options['filters']['field_race_value']['id'] = 'field_race_value';
  $handler->display->display_options['filters']['field_race_value']['table'] = 'field_data_field_race';
  $handler->display->display_options['filters']['field_race_value']['field'] = 'field_race_value';
  $handler->display->display_options['filters']['field_race_value']['value'] = array(
    'human' => 'human',
    'elf' => 'elf',
    'half-elf' => 'half-elf',
    'dwarf' => 'dwarf',
    'gnome' => 'gnome',
    'halfling' => 'halfling',
    'orc' => 'orc',
    'half-orc' => 'half-orc',
    'goblin' => 'goblin',
    'neth' => 'neth',
    'half-neth' => 'half-neth',
    'lurai' => 'lurai',
    'aratay' => 'aratay',
    'mojingan' => 'mojingan',
    'centaur' => 'centaur',
    'merfolk' => 'merfolk',
    'yeek' => 'yeek',
    'human_clanspeople' => 'human_clanspeople',
    'human_dalhan' => 'human_dalhan',
    'human_mountainese' => 'human_mountainese',
    'human_rohearn' => 'human_rohearn',
    'human_gargorian' => 'human_gargorian',
    'human_demi' => 'human_demi',
    'other' => 'other',
  );
  $handler->display->display_options['filters']['field_race_value']['group'] = 1;
  $handler->display->display_options['filters']['field_race_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_race_value']['expose']['operator_id'] = 'field_race_value_op';
  $handler->display->display_options['filters']['field_race_value']['expose']['label'] = 'Race';
  $handler->display->display_options['filters']['field_race_value']['expose']['operator'] = 'field_race_value_op';
  $handler->display->display_options['filters']['field_race_value']['expose']['identifier'] = 'field_race_value';
  $handler->display->display_options['filters']['field_race_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_race_value']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_race_value']['expose']['remember_roles'] = array(
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
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'NPC Name';
  $handler->display->display_options['filters']['title']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['id'] = 'field_is_open_npc_value_1';
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['table'] = 'field_data_field_is_open_npc';
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['field'] = 'field_is_open_npc_value';
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['value'] = array(
    0 => '0',
    1 => '1',
  );
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['group'] = 1;
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['expose']['operator_id'] = 'field_is_open_npc_value_1_op';
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['expose']['label'] = 'Open NPC';
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['expose']['operator'] = 'field_is_open_npc_value_1_op';
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['expose']['identifier'] = 'field_is_open_npc_value_1';
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_is_open_npc_value_1']['reduce_duplicates'] = TRUE;
  /* Filter criterion: Content: Background (field_background) */
  $handler->display->display_options['filters']['field_background_value']['id'] = 'field_background_value';
  $handler->display->display_options['filters']['field_background_value']['table'] = 'field_data_field_background';
  $handler->display->display_options['filters']['field_background_value']['field'] = 'field_background_value';
  $handler->display->display_options['filters']['field_background_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_background_value']['expose']['operator_id'] = 'field_background_value_op';
  $handler->display->display_options['filters']['field_background_value']['expose']['label'] = 'Background';
  $handler->display->display_options['filters']['field_background_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_background_value']['expose']['operator'] = 'field_background_value_op';
  $handler->display->display_options['filters']['field_background_value']['expose']['identifier'] = 'field_background_value';
  $handler->display->display_options['filters']['field_background_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_background_value']['expose']['remember_roles'] = array(
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

  /* Display: NPCs List */
  $handler = $view->new_display('page', 'NPCs List', 'npcs_list');
  $handler->display->display_options['display_description'] = 'List out available NPCs';
  $handler->display->display_options['path'] = 'wiki/npcs-list';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Available NPC List';
  $handler->display->display_options['menu']['description'] = 'A list of all NPCs available in the wiki';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-wiki-links';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['wiki_npcs_list'] = $view;

  return $export;
}
