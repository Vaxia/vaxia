<?php
/**
 * @file
 * rpg_chat.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function rpg_chat_field_default_fields() {
  $fields = array();

  // Exported field: 'node-rpg_chatroom-field_children'.
  $fields['node-rpg_chatroom-field_children'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_children',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
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
        'referrer_nodes_per_page' => '0',
        'referrer_order' => 'TITLE_ASC',
        'referrer_pager_element' => '0',
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
      'translatable' => '0',
      'type' => 'nodereferrer',
    ),
    'field_instance' => array(
      'bundle' => 'rpg_chatroom',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'nodereferrer',
          'settings' => array(),
          'type' => 'nodereferrer_default',
          'weight' => '2',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_children',
      'label' => 'Sub-locations',
      'required' => 0,
      'settings' => array(
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'nodereferrer',
        'settings' => array(),
        'type' => 'nodereferrer_list',
        'weight' => '5',
      ),
    ),
  );

  // Exported field: 'node-rpg_chatroom-field_parent'.
  $fields['node-rpg_chatroom-field_parent'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_parent',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid',
        ),
      ),
      'locked' => '0',
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
      'translatable' => '0',
      'type' => 'node_reference',
    ),
    'field_instance' => array(
      'bundle' => 'rpg_chatroom',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => '1',
        ),
        'print' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1',
        ),
        'rss' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1',
        ),
        'search_index' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1',
        ),
        'search_result' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'inline',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_parent',
      'label' => 'Parent-locations',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'node_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'node_reference/autocomplete',
          'size' => 60,
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => '3',
      ),
    ),
  );

  // Exported field: 'node-rpg_chatroom-field_realm'.
  $fields['node-rpg_chatroom-field_realm'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_realm',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Vaxia' => 'Vaxia',
          'Sirian' => 'Sirian',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'rpg_chatroom',
      'default_value' => array(
        0 => array(
          'value' => 'Vaxia',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '6',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_realm',
      'label' => 'Realm',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '1',
      ),
    ),
  );

  // Exported field: 'node-rpg_chatroom-field_rl_weather'.
  $fields['node-rpg_chatroom-field_rl_weather'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_rl_weather',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(),
      'indexes' => array(
        'lid' => array(
          0 => 'lid',
        ),
      ),
      'locked' => '0',
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
                'collect' => '0',
                'default' => '',
                'weight' => '6',
              ),
              'city' => array(
                'collect' => '0',
                'default' => 'New York',
                'weight' => '8',
              ),
              'country' => array(
                'collect' => '1',
                'default' => 'us',
                'weight' => '14',
              ),
              'locpick' => array(
                'collect' => '1',
                'weight' => '20',
              ),
              'name' => array(
                'collect' => '0',
                'default' => 'New York City',
                'weight' => '2',
              ),
              'postal_code' => array(
                'collect' => '0',
                'default' => '10004',
                'weight' => '12',
              ),
              'province' => array(
                'collect' => '0',
                'default' => 'New York',
                'weight' => '10',
                'widget' => 'autocomplete',
              ),
              'street' => array(
                'collect' => '0',
                'default' => '4 New York Plaza',
                'weight' => '4',
              ),
            ),
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'location',
    ),
    'field_instance' => array(
      'bundle' => 'rpg_chatroom',
      'default_value' => array(
        0 => array(
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
                  'collect' => '0',
                  'default' => '',
                  'weight' => '6',
                ),
                'city' => array(
                  'collect' => '0',
                  'default' => 'New York',
                  'weight' => '8',
                ),
                'country' => array(
                  '#parents' => array(
                    0 => 'field_rl_weather',
                    1 => 'und',
                    2 => 0,
                  ),
                  'collect' => '1',
                  'default' => 'us',
                  'weight' => '14',
                ),
                'delete_location' => array(
                  'default' => FALSE,
                  'nodiff' => TRUE,
                ),
                'is_primary' => array(
                  'default' => 0,
                ),
                'latitude' => array(
                  'default' => 0,
                ),
                'lid' => array(
                  'default' => FALSE,
                ),
                'locpick' => array(
                  '#parents' => array(
                    0 => 'field_rl_weather',
                    1 => 'und',
                    2 => 0,
                  ),
                  'collect' => '1',
                  'default' => FALSE,
                  'nodiff' => TRUE,
                  'weight' => '20',
                ),
                'longitude' => array(
                  'default' => 0,
                ),
                'name' => array(
                  'collect' => '0',
                  'default' => 'New York City',
                  'weight' => '2',
                ),
                'postal_code' => array(
                  'collect' => '0',
                  'default' => '10004',
                  'weight' => '12',
                ),
                'province' => array(
                  'collect' => '0',
                  'default' => 'New York',
                  'weight' => '10',
                  'widget' => 'autocomplete',
                ),
                'source' => array(
                  'default' => 0,
                ),
                'street' => array(
                  'collect' => '0',
                  'default' => '4 New York Plaza',
                  'weight' => '4',
                ),
              ),
            ),
          ),
          'country' => 'us',
          'locpick' => array(
            'user_latitude' => '42.5530802889558',
            'user_longitude' => '-74.35546875',
          ),
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '4',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_rl_weather',
      'label' => 'RL weather',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'location_cck',
        'settings' => array(),
        'type' => 'location',
        'weight' => '4',
      ),
    ),
  );

  // Exported field: 'node-rpg_chatroom-field_wiki_source'.
  $fields['node-rpg_chatroom-field_wiki_source'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_wiki_source',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid',
        ),
      ),
      'locked' => '0',
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
      'translatable' => '0',
      'type' => 'node_reference',
    ),
    'field_instance' => array(
      'bundle' => 'rpg_chatroom',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'node_reference',
          'settings' => array(
            'node_reference_view_mode' => 'artwork',
          ),
          'type' => 'node_reference_node',
          'weight' => '3',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_wiki_source',
      'label' => 'Wiki source',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'node_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'node_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => '2',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Parent-locations');
  t('RL weather');
  t('Realm');
  t('Sub-locations');
  t('Wiki source');

  return $fields;
}
