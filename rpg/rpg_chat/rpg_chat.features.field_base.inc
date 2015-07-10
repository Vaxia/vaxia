<?php
/**
 * @file
 * rpg_chat.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function rpg_chat_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_children'
  $field_bases['field_children'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_children',
    'field_permissions' => array(
      'type' => 0,
    ),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'nodereferrer',
    'settings' => array(
      'multiple' => 0,
      'referrer_fields' => array(
        'field_artwork' => 0,
        'field_comment_character' => 0,
        'field_current_character' => 0,
        'field_items' => 0,
        'field_npcs' => 0,
        'field_parent' => 'field_parent',
      ),
      'referrer_nodes_per_page' => 0,
      'referrer_order' => 'TITLE_ASC',
      'referrer_pager_element' => 0,
      'referrer_translations' => 0,
      'referrer_types' => array(
        'article' => 0,
        'artwork' => 0,
        'character_sheet' => 0,
        'forum' => 0,
        'items' => 0,
        'npc' => 0,
        'page' => 0,
        'rpg_chatroom' => 'rpg_chatroom',
      ),
      'required' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'nodereferrer',
  );

  // Exported field_base: 'field_is_personal_room'
  $field_bases['field_is_personal_room'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_is_personal_room',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'This is NOT a personal room owned by a character.',
        1 => 'This is a personal room owned by a character.',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_locked_to_users'
  $field_bases['field_locked_to_users'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_locked_to_users',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Do not lock to specific users',
        1 => 'Lock to specific users',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_mature_posting'
  $field_bases['field_mature_posting'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_mature_posting',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Mature posts are NOT allowed in this room.',
        1 => 'Mature posts are allowed in this room.',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_parent'
  $field_bases['field_parent'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_parent',
    'field_permissions' => array(
      'type' => 0,
    ),
    'indexes' => array(
      'nid' => array(
        0 => 'nid',
      ),
    ),
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'article' => 0,
        'artwork' => 0,
        'character_sheet' => 0,
        'forum' => 0,
        'items' => 0,
        'npc' => 0,
        'page' => 0,
        'rpg_chatroom' => 'rpg_chatroom',
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
  );

  // Exported field_base: 'field_private_room'
  $field_bases['field_private_room'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_private_room',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Posts in this room do NOT show in Power Search.',
        1 => 'Posts in this room show in Power Search.',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_rl_weather'
  $field_bases['field_rl_weather'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_rl_weather',
    'field_permissions' => array(
      'type' => 0,
    ),
    'indexes' => array(
      'lid' => array(
        0 => 'lid',
      ),
    ),
    'locked' => 0,
    'module' => 'location_cck',
    'settings' => array(
      'gmap_macro' => '[gmap ]',
      'gmap_marker' => 'drupal',
      'location_settings' => array(
        'display' => array(
          'hide' => array(
            'additional' => 0,
            'city' => 0,
            'coords' => 0,
            'country' => 0,
            'country_name' => 0,
            'locpick' => 0,
            'map_link' => 0,
            'name' => 0,
            'postal_code' => 0,
            'province' => 0,
            'province_name' => 0,
            'street' => 0,
          ),
        ),
        'form' => array(
          'fields' => array(
            'additional' => array(
              'collect' => 0,
              'default' => '',
              'weight' => 6,
            ),
            'city' => array(
              'collect' => 0,
              'default' => 'New York',
              'weight' => 8,
            ),
            'country' => array(
              'collect' => 1,
              'default' => 'us',
              'weight' => 14,
            ),
            'locpick' => array(
              'collect' => 1,
              'weight' => 20,
            ),
            'name' => array(
              'collect' => 0,
              'default' => 'New York City',
              'weight' => 2,
            ),
            'postal_code' => array(
              'collect' => 0,
              'default' => 10004,
              'weight' => 12,
            ),
            'province' => array(
              'collect' => 0,
              'default' => 'New York',
              'weight' => 10,
              'widget' => 'autocomplete',
            ),
            'street' => array(
              'collect' => 0,
              'default' => '4 New York Plaza',
              'weight' => 4,
            ),
          ),
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'location',
  );

  // Exported field_base: 'field_room_description'
  $field_bases['field_room_description'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_room_description',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_users'
  $field_bases['field_users'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_users',
    'indexes' => array(
      'uid' => array(
        0 => 'uid',
      ),
    ),
    'locked' => 0,
    'module' => 'user_reference',
    'settings' => array(
      'referenceable_roles' => array(
        2 => 0,
        3 => 0,
        8 => 8,
        9 => 0,
        10 => 0,
        12 => 0,
        13 => 0,
        14 => 0,
        15 => 0,
        17 => 0,
        19 => 0,
        20 => 0,
        21 => 0,
        22 => 0,
        23 => 0,
        24 => 0,
        25 => 0,
        26 => 0,
        27 => 0,
        28 => 0,
        29 => 0,
        30 => 0,
        31 => 0,
        32 => 0,
        33 => 0,
      ),
      'referenceable_status' => array(
        0 => 0,
        1 => 1,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'user_reference',
  );

  // Exported field_base: 'field_wiki_source'
  $field_bases['field_wiki_source'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_wiki_source',
    'indexes' => array(
      'nid' => array(
        0 => 'nid',
      ),
    ),
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'article' => 0,
        'artwork' => 0,
        'character_sheet' => 0,
        'event' => 0,
        'feature_request' => 0,
        'forum' => 0,
        'items' => 0,
        'long_answer' => 0,
        'matching' => 0,
        'multichoice' => 0,
        'npc' => 0,
        'page' => 0,
        'proposal' => 0,
        'quiz' => 0,
        'quiz_directions' => 0,
        'quiz_evaluation' => 0,
        'rpg_chatroom' => 0,
        'session_report' => 0,
        'short_answer' => 0,
        'storyhost_application' => 0,
        'strike' => 0,
        'truefalse' => 0,
        'webform' => 0,
        'wiki_page' => 'wiki_page',
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
  );

  return $field_bases;
}
