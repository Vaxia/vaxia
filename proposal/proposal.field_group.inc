<?php
/**
 * @file
 * proposal.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function proposal_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_add_info|node|proposal|form';
  $field_group->group_name = 'group_add_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'proposal';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Additional information',
    'weight' => '3',
    'children' => array(
      0 => 'field_associated_forum_threads',
      1 => 'field_associated_files',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Additional information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-add-info field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_add_info|node|proposal|form'] = $field_group;

  return $export;
}
