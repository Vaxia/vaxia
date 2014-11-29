<?php
/**
 * @file
 * top_tens.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function top_tens_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'top_tens';
  $view->description = 'View the top tens';
  $view->tag = 'Top Tens';
  $view->base_table = 'vaxia_xp_type_totals';
  $view->human_name = 'Top Tens';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Top Tens';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'type',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'type' => 'type',
    'title' => 'title',
    'field_artwork_image' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
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
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => ' </br>',
      'empty_column' => 0,
    ),
    'field_artwork_image' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '</br>',
      'empty_column' => 0,
    ),
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<style>
.view-content table{float:left;width:31%;margin-left:1em;}
.view-content table:nth-child(4){clear:left;}
.view-content table:nth-child(7){clear:left;}
.view-content table tr td{height:45px;}
.view-footer .view-filters{display:none;}
</style>
<p><b>Characters listed are in the top ten for the XP flavor, but are displayed in alphabetical order.</b></p>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Footer: Global: Text area */
  $handler->display->display_options['footer']['area']['id'] = 'area';
  $handler->display->display_options['footer']['area']['table'] = 'views';
  $handler->display->display_options['footer']['area']['field'] = 'area';
  $handler->display->display_options['footer']['area']['empty'] = TRUE;
  $handler->display->display_options['footer']['area']['content'] = '<div style="clear:both;">&nbsp;</div>
<p>Since SHs can reward multiple flavors of XP in a session you should read the Top Ten rankings as a whole. While a character may have a high raking in Evil XP, they may also have a high ranking in Good XP. Check the character\'s sheet for a more accurate assessment of personality.</p>';
  $handler->display->display_options['footer']['area']['format'] = 'full_html';
  /* Relationship: Vaxia type xp: Vaxia sheet xp node */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'vaxia_xp_type_totals';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Vaxia type xp: Vaxia sheet xp type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'vaxia_xp_type_totals';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['text'] = '<h2>[type]</h2>';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type']['element_wrapper_type'] = 'h2';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = 'Character';
  /* Field: Field: Image */
  $handler->display->display_options['fields']['field_artwork_image']['id'] = 'field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['table'] = 'field_data_field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['field'] = 'field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['relationship'] = 'nid';
  $handler->display->display_options['fields']['field_artwork_image']['label'] = '';
  $handler->display->display_options['fields']['field_artwork_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artwork_image']['empty'] = '<img width="40" height="40" src="http://vaxia.org/sites/all/themes/vaxia_theme/logo.png">';
  $handler->display->display_options['fields']['field_artwork_image']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['field_artwork_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork_image']['settings'] = array(
    'image_style' => 'tiny-thumbnail',
    'image_link' => '',
  );
  /* Sort criterion: Vaxia type xp: Vaxia sheet xp type */
  $handler->display->display_options['sorts']['type']['id'] = 'type';
  $handler->display->display_options['sorts']['type']['table'] = 'vaxia_xp_type_totals';
  $handler->display->display_options['sorts']['type']['field'] = 'type';
  /* Sort criterion: Vaxia type xp: Vaxia sheet xp node */
  $handler->display->display_options['sorts']['xp']['id'] = 'xp';
  $handler->display->display_options['sorts']['xp']['table'] = 'vaxia_xp_type_totals';
  $handler->display->display_options['sorts']['xp']['field'] = 'xp';
  $handler->display->display_options['sorts']['xp']['order'] = 'DESC';
  /* Sort criterion: Global: PHP */
  $handler->display->display_options['sorts']['php']['id'] = 'php';
  $handler->display->display_options['sorts']['php']['table'] = 'views';
  $handler->display->display_options['sorts']['php']['field'] = 'php';
  $handler->display->display_options['sorts']['php']['use_php_setup'] = 0;
  $handler->display->display_options['sorts']['php']['php_sort'] = 'return strcmp($row1->type . $row1->title, $row2->type . $row2->title);';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'nid';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  /* Filter criterion: Vaxia type xp: Vaxia sheet xp type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'vaxia_xp_type_totals';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = '!=';
  $handler->display->display_options['filters']['type']['value'] = 'transfer';
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Vaxia type xp: Vaxia sheet xp type */
  $handler->display->display_options['filters']['type_2']['id'] = 'type_2';
  $handler->display->display_options['filters']['type_2']['table'] = 'vaxia_xp_type_totals';
  $handler->display->display_options['filters']['type_2']['field'] = 'type';
  $handler->display->display_options['filters']['type_2']['operator'] = '!=';
  $handler->display->display_options['filters']['type_2']['value'] = 'site-award';
  $handler->display->display_options['filters']['type_2']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'nid';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['relationship'] = 'nid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['relationship'] = 'nid';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 1;
  /* Filter criterion: Content: Open NPC (field_is_open_npc) */
  $handler->display->display_options['filters']['field_is_open_npc_value']['id'] = 'field_is_open_npc_value';
  $handler->display->display_options['filters']['field_is_open_npc_value']['table'] = 'field_data_field_is_open_npc';
  $handler->display->display_options['filters']['field_is_open_npc_value']['field'] = 'field_is_open_npc_value';
  $handler->display->display_options['filters']['field_is_open_npc_value']['relationship'] = 'nid';
  $handler->display->display_options['filters']['field_is_open_npc_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_open_npc_value']['group'] = 1;
  /* Filter criterion: Content: Storyhost (field_is_storyteller) */
  $handler->display->display_options['filters']['field_is_storyteller_value']['id'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['table'] = 'field_data_field_is_storyteller';
  $handler->display->display_options['filters']['field_is_storyteller_value']['field'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['relationship'] = 'nid';
  $handler->display->display_options['filters']['field_is_storyteller_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_storyteller_value']['group'] = 1;
  /* Filter criterion: Content: Realm (field_realm) */
  $handler->display->display_options['filters']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['filters']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['relationship'] = 'nid';
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
    21 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
  );
  $handler->display->display_options['filters']['field_realm_value']['expose']['reduce'] = TRUE;
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['group'] = 1;
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = 'return _top_tens_filter_type_already_displayed($row->type, $static);';

  /* Display: Top Tens */
  $handler = $view->new_display('page', 'Top Tens', 'top_ten_page');
  $handler->display->display_options['path'] = 'top-ten-displays';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Top tens';
  $handler->display->display_options['menu']['description'] = 'Top tens';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'menu-vaxia';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['top_tens'] = $view;

  $view = new view();
  $view->name = 'total_top_ten';
  $view->description = 'A view of the total top tens';
  $view->tag = 'Top Tens';
  $view->base_table = 'character_sheet_xp';
  $view->human_name = 'Top Tens - Supplemental';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Total Top Ten';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    8 => '8',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'php_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'type' => 'type',
    'php_1' => 'php_1',
    'xp_1' => 'xp_1',
    'field_artwork_image' => 'field_artwork_image',
    'title' => 'title',
    'php' => 'php',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
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
    'php_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'xp_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_artwork_image' => array(
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
      'separator' => '',
      'empty_column' => 0,
    ),
    'php' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Character sheet xp: Character sheet xp node */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'character_sheet_xp';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Character sheet xp: Character sheet xp type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'character_sheet_xp';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['text'] = '<h2>[type]</h2>';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = '';
  $handler->display->display_options['fields']['php_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['php_1']['alter']['text'] = '<h2>[php_1]</h2>';
  $handler->display->display_options['fields']['php_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_1']['php_value'] = 'return _top_tens_rename_type($row->type);';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  /* Field: Character sheet xp: Character sheet xp amount */
  $handler->display->display_options['fields']['xp_1']['id'] = 'xp_1';
  $handler->display->display_options['fields']['xp_1']['table'] = 'character_sheet_xp';
  $handler->display->display_options['fields']['xp_1']['field'] = 'xp';
  $handler->display->display_options['fields']['xp_1']['label'] = 'Amount';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<img width="40" height="40" src="http://vaxia.org/sites/all/themes/vaxia_theme/logo.png">';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Field: Field: Image */
  $handler->display->display_options['fields']['field_artwork_image']['id'] = 'field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['table'] = 'field_data_field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['field'] = 'field_artwork_image';
  $handler->display->display_options['fields']['field_artwork_image']['relationship'] = 'nid';
  $handler->display->display_options['fields']['field_artwork_image']['label'] = '';
  $handler->display->display_options['fields']['field_artwork_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_artwork_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_artwork_image']['settings'] = array(
    'image_style' => 'tiny-thumbnail',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = 'Character';
  /* Sort criterion: Character sheet xp: Character sheet xp type */
  $handler->display->display_options['sorts']['type']['id'] = 'type';
  $handler->display->display_options['sorts']['type']['table'] = 'character_sheet_xp';
  $handler->display->display_options['sorts']['type']['field'] = 'type';
  /* Sort criterion: Character sheet xp: Character sheet xp amount */
  $handler->display->display_options['sorts']['xp']['id'] = 'xp';
  $handler->display->display_options['sorts']['xp']['table'] = 'character_sheet_xp';
  $handler->display->display_options['sorts']['xp']['field'] = 'xp';
  $handler->display->display_options['sorts']['xp']['order'] = 'DESC';
  /* Filter criterion: Character sheet xp: Character sheet xp type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'character_sheet_xp';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['operator'] = 'word';
  $handler->display->display_options['filters']['type']['value'] = 'spent_xp';
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'nid';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'character_sheet' => 'character_sheet',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'nid';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Workflow: Current state */
  $handler->display->display_options['filters']['sid']['id'] = 'sid';
  $handler->display->display_options['filters']['sid']['table'] = 'workflow_node';
  $handler->display->display_options['filters']['sid']['field'] = 'sid';
  $handler->display->display_options['filters']['sid']['relationship'] = 'nid';
  $handler->display->display_options['filters']['sid']['value'] = array(
    4 => '4',
  );
  $handler->display->display_options['filters']['sid']['group'] = 1;
  /* Filter criterion: Content: NPC (field_is_npc) */
  $handler->display->display_options['filters']['field_is_npc_value']['id'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['table'] = 'field_data_field_is_npc';
  $handler->display->display_options['filters']['field_is_npc_value']['field'] = 'field_is_npc_value';
  $handler->display->display_options['filters']['field_is_npc_value']['relationship'] = 'nid';
  $handler->display->display_options['filters']['field_is_npc_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_npc_value']['group'] = 1;
  /* Filter criterion: Content: Open NPC (field_is_open_npc) */
  $handler->display->display_options['filters']['field_is_open_npc_value']['id'] = 'field_is_open_npc_value';
  $handler->display->display_options['filters']['field_is_open_npc_value']['table'] = 'field_data_field_is_open_npc';
  $handler->display->display_options['filters']['field_is_open_npc_value']['field'] = 'field_is_open_npc_value';
  $handler->display->display_options['filters']['field_is_open_npc_value']['relationship'] = 'nid';
  $handler->display->display_options['filters']['field_is_open_npc_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_open_npc_value']['group'] = 1;
  /* Filter criterion: Content: Storyhost (field_is_storyteller) */
  $handler->display->display_options['filters']['field_is_storyteller_value']['id'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['table'] = 'field_data_field_is_storyteller';
  $handler->display->display_options['filters']['field_is_storyteller_value']['field'] = 'field_is_storyteller_value';
  $handler->display->display_options['filters']['field_is_storyteller_value']['relationship'] = 'nid';
  $handler->display->display_options['filters']['field_is_storyteller_value']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['field_is_storyteller_value']['group'] = 1;
  /* Filter criterion: Content: Realm (field_realm) */
  $handler->display->display_options['filters']['field_realm_value']['id'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['table'] = 'field_data_field_realm';
  $handler->display->display_options['filters']['field_realm_value']['field'] = 'field_realm_value';
  $handler->display->display_options['filters']['field_realm_value']['relationship'] = 'nid';
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
    21 => 0,
    4 => 0,
    3 => 0,
    11 => 0,
  );
  $handler->display->display_options['filters']['field_realm_value']['expose']['reduce'] = TRUE;
  /* Filter criterion: Global: PHP */
  $handler->display->display_options['filters']['php']['id'] = 'php';
  $handler->display->display_options['filters']['php']['table'] = 'views';
  $handler->display->display_options['filters']['php']['field'] = 'php';
  $handler->display->display_options['filters']['php']['group'] = 1;
  $handler->display->display_options['filters']['php']['use_php_setup'] = 0;
  $handler->display->display_options['filters']['php']['php_filter'] = 'return _top_tens_filter_type_already_displayed($row->type, $static);';

  /* Display: Total Top Ten */
  $handler = $view->new_display('page', 'Total Top Ten', 'top_ten_page_two');
  $handler->display->display_options['path'] = 'total-top-ten';
  $export['total_top_ten'] = $view;

  return $export;
}
