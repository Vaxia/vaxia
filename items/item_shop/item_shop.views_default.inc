<?php
/**
 * @file
 * item_shop.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function item_shop_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'item_shop';
  $view->description = 'Display items for item shop';
  $view->tag = 'Item Shop';
  $view->base_table = 'node';
  $view->human_name = 'Item Shop';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Item shop';
  $handler->display->display_options['use_more'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'use item shop';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'views_bulk_operations' => 'views_bulk_operations',
    'title' => 'title',
    'field_item_type' => 'field_item_type',
    'field_value' => 'field_value',
    'field_realm' => 'field_realm',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
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
    'field_item_type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_value' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_realm' => array(
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
  $handler->display->display_options['header']['area']['label'] = 'Help display';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'These items are considered to be commonplace enough to always be available for your character. You may buy them out of the shop directly here, without needing to ask an SH for permission. 

[b]Setting Note:[/b] Weapons are considered to be \'lifestyle\' tools available for characters in the same way clothing, basic shelter, and food are available as part of your economic level. Since weapons do have stats (unlike most lifestyle items) we have included them in the shop for easy access.';
  $handler->display->display_options['header']['area']['format'] = 'plain_text';
  /* Footer: Global: PHP */
  $handler->display->display_options['footer']['php']['id'] = 'php';
  $handler->display->display_options['footer']['php']['table'] = 'views';
  $handler->display->display_options['footer']['php']['field'] = 'php';
  $handler->display->display_options['footer']['php']['label'] = 'Output economy information';
  $handler->display->display_options['footer']['php']['empty'] = TRUE;
  $handler->display->display_options['footer']['php']['php_output'] = '<b>Customization:</b> If you would like a modified version of any of these items, please ask an SH on the forums to help you out.
</br>
<?php
  return _item_shop_show_footer();
?>';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No items found or this character may not be allowed access to the shop at this time.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Item shop field */
  $handler->display->display_options['fields']['item_shop']['id'] = 'item_shop';
  $handler->display->display_options['fields']['item_shop']['table'] = 'node';
  $handler->display->display_options['fields']['item_shop']['field'] = 'item_shop';
  $handler->display->display_options['fields']['item_shop']['label'] = 'Purchase';
  $handler->display->display_options['fields']['item_shop']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Items';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Item type */
  $handler->display->display_options['fields']['field_item_type']['id'] = 'field_item_type';
  $handler->display->display_options['fields']['field_item_type']['table'] = 'field_data_field_item_type';
  $handler->display->display_options['fields']['field_item_type']['field'] = 'field_item_type';
  $handler->display->display_options['fields']['field_item_type']['label'] = 'Type';
  /* Field: Content: Value */
  $handler->display->display_options['fields']['field_value']['id'] = 'field_value';
  $handler->display->display_options['fields']['field_value']['table'] = 'field_data_field_value';
  $handler->display->display_options['fields']['field_value']['field'] = 'field_value';
  $handler->display->display_options['fields']['field_value']['label'] = 'Cost';
  $handler->display->display_options['fields']['field_value']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_value']['alter']['text'] = '[field_value] silver';
  $handler->display->display_options['fields']['field_value']['empty'] = 'Free (Lifestyle)';
  $handler->display->display_options['fields']['field_value']['empty_zero'] = TRUE;
  /* Sort criterion: Content: Value (field_value) */
  $handler->display->display_options['sorts']['field_value_value']['id'] = 'field_value_value';
  $handler->display->display_options['sorts']['field_value_value']['table'] = 'field_data_field_value';
  $handler->display->display_options['sorts']['field_value_value']['field'] = 'field_value_value';
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
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
    'items' => 'items',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: In shop (field_in_shop) */
  $handler->display->display_options['filters']['field_in_shop_value']['id'] = 'field_in_shop_value';
  $handler->display->display_options['filters']['field_in_shop_value']['table'] = 'field_data_field_in_shop';
  $handler->display->display_options['filters']['field_in_shop_value']['field'] = 'field_in_shop_value';
  $handler->display->display_options['filters']['field_in_shop_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_in_shop_value']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    15 => '15',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Can character afford item */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['ui_name'] = 'Can character afford item';
  $handler->display->display_options['filters']['php']['group'] = 1;
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = 'return item_shop_check_can_afford($row->nid);';
  /* Filter criterion: Does character already have item */
  $handler->display->display_options['filters']['php_1']['id'] = 'php_1';
  $handler->display->display_options['filters']['php_1']['table'] = 'views';
  $handler->display->display_options['filters']['php_1']['field'] = 'php';
  $handler->display->display_options['filters']['php_1']['ui_name'] = 'Does character already have item';
  $handler->display->display_options['filters']['php_1']['group'] = 1;
  $handler->display->display_options['filters']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php_1']['php_filter'] = 'return FALSE;
return item_shop_check_already_has($row->nid);';
  /* Filter criterion: Is character playable, has access */
  $handler->display->display_options['filters']['php_2']['id'] = 'php_2';
  $handler->display->display_options['filters']['php_2']['table'] = 'views';
  $handler->display->display_options['filters']['php_2']['field'] = 'php';
  $handler->display->display_options['filters']['php_2']['ui_name'] = 'Is character playable, has access';
  $handler->display->display_options['filters']['php_2']['group'] = 1;
  $handler->display->display_options['filters']['php_2']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php_2']['php_filter'] = '$char_nid = arg(1);
$char = node_load($char_nid);
if ($char) {

// Is playable.
if (!_character_sheet_is_playable($char)) {
return TRUE;
}

// Has a workflow.
if (empty($char->workflow_state_name)) {
return TRUE;
}

// Is approved workflow.
if (!empty($char->workflow_state_name) && $char->workflow_state_name != \'approved\') {
return TRUE;
}

// Has access permission.
$access = !empty($char->field_item_shop[$char->language][0][\'value\']) ? true : false;
if (!$access) {
return TRUE;
}

}
return FALSE;';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Item name';
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
  /* Filter criterion: Content: Item type (field_item_type) */
  $handler->display->display_options['filters']['field_item_type_tid']['id'] = 'field_item_type_tid';
  $handler->display->display_options['filters']['field_item_type_tid']['table'] = 'field_data_field_item_type';
  $handler->display->display_options['filters']['field_item_type_tid']['field'] = 'field_item_type_tid';
  $handler->display->display_options['filters']['field_item_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_item_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['operator_id'] = 'field_item_type_tid_op';
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['label'] = 'Item type';
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['operator'] = 'field_item_type_tid_op';
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['identifier'] = 'field_item_type_tid';
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_item_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_item_type_tid']['vocabulary'] = 'item_type';

  /* Display: Item Shop */
  $handler = $view->new_display('page', 'Item Shop', 'item_shop');
  $handler->display->display_options['path'] = 'node/%/item_shop';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Item shop';
  $handler->display->display_options['menu']['description'] = 'Item shop';
  $handler->display->display_options['menu']['weight'] = '4';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Item Shop List */
  $handler = $view->new_display('page', 'Item Shop List', 'item_shop_admin');
  $handler->display->display_options['display_description'] = 'Item Shop Administration';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    9 => '9',
  );
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: Content */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'node';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::clone_action_clone' => array(
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
    'action::node_publish_action' => array(
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
  $handler->display->display_options['fields']['title']['label'] = 'Items';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Item type */
  $handler->display->display_options['fields']['field_item_type']['id'] = 'field_item_type';
  $handler->display->display_options['fields']['field_item_type']['table'] = 'field_data_field_item_type';
  $handler->display->display_options['fields']['field_item_type']['field'] = 'field_item_type';
  $handler->display->display_options['fields']['field_item_type']['label'] = 'Type';
  /* Field: Content: Value */
  $handler->display->display_options['fields']['field_value']['id'] = 'field_value';
  $handler->display->display_options['fields']['field_value']['table'] = 'field_data_field_value';
  $handler->display->display_options['fields']['field_value']['field'] = 'field_value';
  $handler->display->display_options['fields']['field_value']['label'] = 'Cost';
  $handler->display->display_options['fields']['field_value']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_value']['alter']['text'] = '[field_value] silver';
  $handler->display->display_options['fields']['field_value']['empty'] = 'Free (Lifestyle)';
  $handler->display->display_options['fields']['field_value']['empty_zero'] = TRUE;
  /* Field: Field: Realm */
  $handler->display->display_options['fields']['field_tag_realm']['id'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['field'] = 'field_tag_realm';
  $handler->display->display_options['fields']['field_tag_realm']['type'] = 'taxonomy_term_reference_plain';
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
    'items' => 'items',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: In shop (field_in_shop) */
  $handler->display->display_options['filters']['field_in_shop_value']['id'] = 'field_in_shop_value';
  $handler->display->display_options['filters']['field_in_shop_value']['table'] = 'field_data_field_in_shop';
  $handler->display->display_options['filters']['field_in_shop_value']['field'] = 'field_in_shop_value';
  $handler->display->display_options['filters']['field_in_shop_value']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['field_in_shop_value']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    15 => '15',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Item name';
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
  /* Filter criterion: Content: Item type (field_item_type) */
  $handler->display->display_options['filters']['field_item_type_tid']['id'] = 'field_item_type_tid';
  $handler->display->display_options['filters']['field_item_type_tid']['table'] = 'field_data_field_item_type';
  $handler->display->display_options['filters']['field_item_type_tid']['field'] = 'field_item_type_tid';
  $handler->display->display_options['filters']['field_item_type_tid']['group'] = 1;
  $handler->display->display_options['filters']['field_item_type_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['operator_id'] = 'field_item_type_tid_op';
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['label'] = 'Item type';
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['operator'] = 'field_item_type_tid_op';
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['identifier'] = 'field_item_type_tid';
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['field_item_type_tid']['expose']['remember_roles'] = array(
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
  $handler->display->display_options['filters']['field_item_type_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_item_type_tid']['vocabulary'] = 'item_type';
  /* Filter criterion: Field: Realm (field_tag_realm) */
  $handler->display->display_options['filters']['field_tag_realm_tid']['id'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['table'] = 'field_data_field_tag_realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['field'] = 'field_tag_realm_tid';
  $handler->display->display_options['filters']['field_tag_realm_tid']['value'] = '';
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
  $handler->display->display_options['filters']['field_tag_realm_tid']['type'] = 'select';
  $handler->display->display_options['filters']['field_tag_realm_tid']['vocabulary'] = 'realm';
  $handler->display->display_options['filters']['field_tag_realm_tid']['hierarchy'] = 1;
  $handler->display->display_options['path'] = 'item_shop';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Item Shop List';
  $handler->display->display_options['menu']['description'] = 'Item Shop List';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia-sh';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['item_shop'] = $view;

  return $export;
}
