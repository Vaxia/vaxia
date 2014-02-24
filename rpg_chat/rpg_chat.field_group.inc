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
  $field_group->identifier = 'group_navigation|node|rpg_chatroom|default';
  $field_group->group_name = 'group_navigation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'rpg_chatroom';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Navigation',
    'weight' => '0',
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

  return $export;
}
