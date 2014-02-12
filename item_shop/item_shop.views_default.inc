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
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'use item shop';
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
    'nid' => 'nid',
    'item_shop' => 'item_shop',
    'title' => 'title',
    'field_value' => 'field_value',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'item_shop' => array(
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
    'field_value' => array(
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
  $handler->display->display_options['header']['area']['label'] = 'Help display';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = 'The following generic items are available for sale. You may buy them out of the shop directly here, without needing to ask an SH for permission. These items are considered to be commonplace and available for your character. If you would like a modified version of any of these items, please ask an SH and they can make arrangements. SH requests are usually handled on the forums - since any SH can help you out.';
  $handler->display->display_options['header']['area']['format'] = 'plain_text';
  /* Footer: Global: PHP */
  $handler->display->display_options['footer']['php']['id'] = 'php';
  $handler->display->display_options['footer']['php']['table'] = 'views';
  $handler->display->display_options['footer']['php']['field'] = 'php';
  $handler->display->display_options['footer']['php']['label'] = 'Output economy information';
  $handler->display->display_options['footer']['php']['empty'] = TRUE;
  $handler->display->display_options['footer']['php']['php_output'] = '<?php
// Get arguments.
global $user;
$node = node_load(arg(1));
if ($user->uid == $node->uid) {
  // Grab the view, and load it with correct arguments.
  $lang = $node->language;
  $realm = isset($node->field_realm[$lang][0][\'value\']) ? $node->field_realm[$lang][0][\'value\'] : \'Vaxia\';
  $economy = isset($node->field_economy[$lang][0][\'value\']) ? $node->field_economy[$lang][0][\'value\'] : 0;
  // Figure out the players silver and economic values.
  $silver = _character_sheet_get_xp($node->nid, \'silver\');
  $silver_spent = _character_sheet_get_xp($node->nid, \'spent_silver\');
  $silver_invested = _character_sheet_get_xp($node->nid, \'invest_silver\');
  $available_silver = $silver - ($silver_spent + $silver_invested);
  // Available economy.
  $worth = _vaxia_calculate_worth($economy);
  // Grand total available.
  $total_worth = $available_silver + $worth;
  // Display.
  $output  = \'\';
  $output .= \'</br>\'. t(\'You have :silver available to spend.\', array(\':silver\' => number_format($available_silver,2)));
  $output .= \'</br>\'. t(\'If you cash in your economy levels, you have an additional :worth silver available to spend.\',
    array(\':worth\' => number_format($worth,2)));
  $output .= \'</br>\'. t(\'Your total worth including silver and economy is :total_worth silver.\',
    array(\':total_worth\' => number_format($total_worth,2)));
}
print $output;
?>';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No items found.';
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
  $handler->display->display_options['fields']['item_shop']['label'] = '';
  $handler->display->display_options['fields']['item_shop']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Items';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Value */
  $handler->display->display_options['fields']['field_value']['id'] = 'field_value';
  $handler->display->display_options['fields']['field_value']['table'] = 'field_data_field_value';
  $handler->display->display_options['fields']['field_value']['field'] = 'field_value';
  $handler->display->display_options['fields']['field_value']['label'] = 'Cost';
  $handler->display->display_options['fields']['field_value']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_value']['alter']['text'] = '[field_value] silver';
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
  $handler->display->display_options['filters']['title']['group'] = 1;
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Item name';
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
    18 => 0,
  );
  /* Filter criterion: Content: Value (field_value) */
  $handler->display->display_options['filters']['field_value_value']['id'] = 'field_value_value';
  $handler->display->display_options['filters']['field_value_value']['table'] = 'field_data_field_value';
  $handler->display->display_options['filters']['field_value_value']['field'] = 'field_value_value';
  $handler->display->display_options['filters']['field_value_value']['group'] = 1;
  $handler->display->display_options['filters']['field_value_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_value_value']['expose']['operator_id'] = 'field_value_value_op';
  $handler->display->display_options['filters']['field_value_value']['expose']['label'] = 'Item cost';
  $handler->display->display_options['filters']['field_value_value']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['field_value_value']['expose']['operator'] = 'field_value_value_op';
  $handler->display->display_options['filters']['field_value_value']['expose']['identifier'] = 'field_value_value';
  $handler->display->display_options['filters']['field_value_value']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['field_value_value']['expose']['remember_roles'] = array(
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
    18 => 0,
  );
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['group'] = 1;
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = 'return item_shop_check_can_afford($row->nid);';
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php_1']['id'] = 'php_1';
  $handler->display->display_options['filters']['php_1']['table'] = 'views';
  $handler->display->display_options['filters']['php_1']['field'] = 'php';
  $handler->display->display_options['filters']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php_1']['php_filter'] = 'return item_shop_check_already_has($row->nid);';

  /* Display: Item Shop */
  $handler = $view->new_display('page', 'Item Shop', 'item_shop');
  $handler->display->display_options['path'] = 'node/%/item_shop';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Item shop';
  $handler->display->display_options['menu']['description'] = 'Item shop';
  $handler->display->display_options['menu']['weight'] = '4';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['item_shop'] = $view;

  return $export;
}
