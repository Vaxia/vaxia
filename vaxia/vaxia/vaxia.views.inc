<?php

/**
 * @file
 * Basic views integration for character sheetss.
 */

/**
 * Implements hook_views_data(). From Views module.
 */
function vaxia_views_data() {
  // XP type table.
  $data['vaxia_xp_type_totals']['table']['group'] = t('Vaxia type xp');
  $data['vaxia_xp_type_totals']['table']['base'] = array(
    'field' => 'lid',
    'title' => t('Vaxia xp table'),
    'help' => t("Vaxia xp table contains xp content."),
    'weight' => -10,
  );
  $data['vaxia_xp_type_totals']['nid'] = array(
    'title' => t('Vaxia sheet xp node'),
    'help' => t('Node referenced by Vaxia sheet xp.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Character sheet node'),
    ),
  );
  $data['vaxia_xp_type_totals']['xp'] = array(
    'title' => t('Vaxia sheet xp node'),
    'help' => t('Xp amount.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['vaxia_xp_type_totals']['type'] = array(
    'title' => t('Vaxia sheet xp type'),
    'help' => t('Xp type.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  return $data;
}