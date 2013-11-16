<?php
/**
 * @file
 * Provides custom views fields.
 */

/**
* Implements hook_views_data_alter().
*/
function item_shop_views_data_alter(&$data) {
  // Add the shop form field to nodes.
  $data['node']['item_shop'] = array(
    'field' => array(
      'title' => t('Item shop field'),
      'help' => t('Item shop field'),
      'handler' => 'item_shop_field_handler_add_item',
    ),
  );
}