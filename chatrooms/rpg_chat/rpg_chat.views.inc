<?php

/**
 * @file
 * Basic views integration for validation logs.
 */

/**
 * Implements hook_views_data(). From Views module.
 */
function rpg_chat_views_data() {
  $data['rpg_chat_room']['table']['group'] = t('RPG Chat');
  $data['rpg_chat_room']['table']['join'] = array(
    'node' => array(
      'field' => 'nid',
      'left_table' => 'node',
      'left_field' => 'nid',
    ),
  );
  $data['rpg_chat_room']['min_refresh'] = array(
    'title' => t('Minimum refresh'),
    'help' => t('The minimium refresh rate for this room'),
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
  $data['rpg_chat_room']['mature'] = array(
    'title' => t('Mature'),
    'help' => t('If the room is flagged mature or not.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['rpg_chat_room']['locked'] = array(
    'title' => t('Locked'),
    'help' => t('If the room is flagged logged or not.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
/*
  // For the moment not exposed to Views pending needed rewrite for it.
  // TODO later.
  $data['rpg_chat_room']['allowed_users'] = array(
    'title' => t('Allowed users'),
    'help' => t('Serialized array of allowed users.'),
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
  $data['rpg_chat_room']['banned_users'] = array(
    'title' => t('Banned users'),
    'help' => t('Serialized array of banned users.'),
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
*/
  return $data;
}