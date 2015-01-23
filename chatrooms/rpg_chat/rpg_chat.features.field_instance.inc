<?php
/**
 * @file
 * rpg_chat.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function rpg_chat_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-rpg_chatroom-field_children'
  $field_instances['node-rpg_chatroom-field_children'] = array(
    'bundle' => 'rpg_chatroom',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'nodereferrer',
        'settings' => array(),
        'type' => 'nodereferrer_default',
        'weight' => 2,
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
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_locked_to_users'
  $field_instances['node-rpg_chatroom-field_locked_to_users'] = array(
    'bundle' => 'rpg_chatroom',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_locked_to_users',
    'label' => 'Locked to users',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_mature_posting'
  $field_instances['node-rpg_chatroom-field_mature_posting'] = array(
    'bundle' => 'rpg_chatroom',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_mature_posting',
    'label' => 'Mature posting',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_npcs'
  $field_instances['node-rpg_chatroom-field_npcs'] = array(
    'bundle' => 'rpg_chatroom',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Make a note of NPCs commonly found in this  location.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'node_reference',
        'settings' => array(),
        'type' => 'node_reference_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_npcs',
    'label' => 'NPCs',
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
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_parent'
  $field_instances['node-rpg_chatroom-field_parent'] = array(
    'bundle' => 'rpg_chatroom',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'node_reference',
        'settings' => array(),
        'type' => 'node_reference_default',
        'weight' => 1,
      ),
      'print' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'rss' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'search_index' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'search_result' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'node_reference',
        'settings' => array(),
        'type' => 'node_reference_default',
        'weight' => 0,
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
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_private_room'
  $field_instances['node-rpg_chatroom-field_private_room'] = array(
    'bundle' => 'rpg_chatroom',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_private_room',
    'label' => 'Private room',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 0,
      ),
      'type' => 'options_onoff',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_realm'
  $field_instances['node-rpg_chatroom-field_realm'] = array(
    'bundle' => 'rpg_chatroom',
    'default_value' => array(
      0 => array(
        'value' => 'Vaxia',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
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
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_rl_weather'
  $field_instances['node-rpg_chatroom-field_rl_weather'] = array(
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
                '#parents' => array(
                  0 => 'field_rl_weather',
                  1 => 'und',
                  2 => 0,
                ),
                'collect' => 1,
                'default' => 'us',
                'weight' => 14,
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
                'collect' => 1,
                'default' => FALSE,
                'nodiff' => TRUE,
                'weight' => 20,
              ),
              'longitude' => array(
                'default' => 0,
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
              'source' => array(
                'default' => 0,
              ),
              'street' => array(
                'collect' => 0,
                'default' => '4 New York Plaza',
                'weight' => 4,
              ),
            ),
          ),
        ),
        'country' => 'us',
        'locpick' => array(
          'user_latitude' => '42.5530802889558',
          'user_longitude' => -74.35546875,
        ),
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
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
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_users'
  $field_instances['node-rpg_chatroom-field_users'] = array(
    'bundle' => 'rpg_chatroom',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Select the users to lock this room for. Only these users may post here.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_users',
    'label' => 'Users',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'user_reference',
      'settings' => array(
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'user_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'user_reference_autocomplete',
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-rpg_chatroom-field_wiki_source'
  $field_instances['node-rpg_chatroom-field_wiki_source'] = array(
    'bundle' => 'rpg_chatroom',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'node_reference',
        'settings' => array(
          'node_reference_view_mode' => 'artwork',
        ),
        'type' => 'node_reference_node',
        'weight' => 3,
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
        'size' => 60,
      ),
      'type' => 'node_reference_autocomplete',
      'weight' => 4,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Locked to users');
  t('Make a note of NPCs commonly found in this  location.');
  t('Mature posting');
  t('NPCs');
  t('Parent-locations');
  t('Private room');
  t('RL weather');
  t('Realm');
  t('Select the users to lock this room for. Only these users may post here.');
  t('Sub-locations');
  t('Users');
  t('Wiki source');

  return $field_instances;
}
