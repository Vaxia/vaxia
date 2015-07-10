<?php
/**
 * @file
 * rpg_chat.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function rpg_chat_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_access_settings|node|rpg_chatroom|form';
  $field_group->group_name = 'group_access_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'rpg_chatroom';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Access settings',
    'weight' => '3',
    'children' => array(
      0 => 'field_mature_posting',
      1 => 'field_locked_to_users',
      2 => 'field_users',
      3 => 'field_private_room',
      4 => 'field_is_personal_room',
      5 => 'field_room_description',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-access-settings field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_access_settings|node|rpg_chatroom|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_description|node|rpg_chatroom|default';
  $field_group->group_name = 'group_description';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'rpg_chatroom';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Description',
    'weight' => '2',
    'children' => array(
      0 => 'field_wiki_source',
      1 => 'field_room_description',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-description field-group-fieldset',
      ),
    ),
  );
  $export['group_description|node|rpg_chatroom|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_navigation|node|rpg_chatroom|default';
  $field_group->group_name = 'group_navigation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'rpg_chatroom';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Navigation',
    'weight' => '1',
    'children' => array(
      0 => 'field_parent',
      1 => 'field_children',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-navigation field-group-fieldset',
      ),
    ),
  );
  $export['group_navigation|node|rpg_chatroom|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_room_data|node|rpg_chatroom|form';
  $field_group->group_name = 'group_room_data';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'rpg_chatroom';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Room data',
    'weight' => '2',
    'children' => array(
      0 => 'field_rl_weather',
      1 => 'field_realm',
      2 => 'field_tag_realm',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-room-data field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_room_data|node|rpg_chatroom|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_source_info|node|rpg_chatroom|form';
  $field_group->group_name = 'group_source_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'rpg_chatroom';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Source info',
    'weight' => '1',
    'children' => array(
      0 => 'field_parent',
      1 => 'field_children',
      2 => 'field_wiki_source',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-source-info field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_source_info|node|rpg_chatroom|form'] = $field_group;

  return $export;
}
