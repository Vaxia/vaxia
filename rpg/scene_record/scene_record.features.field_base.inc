<?php
/**
 * @file
 * scene_record.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function scene_record_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_difficulty'
  $field_bases['field_difficulty'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_difficulty',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        25 => '25: Most rolls, gaining silver',
        40 => '40: 1-5 Build Points, 25PP per',
        50 => '50: 6-10 Build Points, 50PP per',
        60 => '60: 11-15 Build Points, 75PP per',
        70 => '70: 16-20 Build Points, 100PP per',
        80 => '80: 21 - 25 Build Points, 125PP per',
        90 => '90: 26 - 30 Build Points, 150PP per',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_integer',
  );

  // Exported field_base: 'field_goal'
  $field_bases['field_goal'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_goal',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'misc' => 'Generic ruling',
        'item' => 'Crafting an item',
        'silver' => 'Making some silver',
        'fame' => 'Gain reputation',
        'maint' => 'Maintenance',
        'save_point' => 'Save point',
        'priv' => 'Private record',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_rp_bonus'
  $field_bases['field_rp_bonus'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_rp_bonus',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'No',
        1 => 'Yes',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_boolean',
  );

  // Exported field_base: 'field_rp_posts'
  $field_bases['field_rp_posts'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_rp_posts',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'direction' => 'ASC',
          'property' => 'cid',
          'type' => 'property',
        ),
        'target_bundles' => array(
          'comment_node_rpg_chatroom' => 'comment_node_rpg_chatroom',
        ),
      ),
      'target_type' => 'comment',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_scene'
  $field_bases['field_scene'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_scene',
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

  // Exported field_base: 'field_scene_character'
  $field_bases['field_scene_character'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_scene_character',
    'indexes' => array(
      'nid' => array(
        0 => 'nid',
      ),
    ),
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'application' => 0,
        'artwork' => 0,
        'character_sheet' => 'character_sheet',
        'event' => 0,
        'filedepot_folder' => 0,
        'forum' => 0,
        'items' => 0,
        'long_answer' => 0,
        'mass_contact' => 0,
        'multichoice' => 0,
        'notation' => 0,
        'page' => 0,
        'play_suggestions' => 0,
        'proposal' => 0,
        'quiz' => 0,
        'rpg_chatroom' => 0,
        'scene' => 0,
        'session_report' => 0,
        'storyhost_application' => 0,
        'strike' => 0,
        'timeline_event' => 0,
        'webform' => 0,
        'wiki_page' => 0,
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

  // Exported field_base: 'field_scene_notes'
  $field_bases['field_scene_notes'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_scene_notes',
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

  // Exported field_base: 'field_sh_attention_to'
  $field_bases['field_sh_attention_to'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_sh_attention_to',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(
      'max_length' => 255,
    ),
    'translatable' => 0,
    'type' => 'text',
  );

  return $field_bases;
}
