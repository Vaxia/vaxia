<?php
/**
 * @file
 * proposal.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function proposal_field_group_info() {
  $field_groups = array();

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
    'weight' => '5',
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
  $field_groups['group_add_info|node|proposal|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_settings|node|proposal|form';
  $field_group->group_name = 'group_settings';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'proposal';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Settings',
    'weight' => '3',
    'children' => array(
      0 => 'field_proposal_type',
      1 => 'field_grievance_members',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-settings field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_settings|node|proposal|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Additional information');
  t('Settings');

  return $field_groups;
}
