<?php
/**
 * @file
 * rpg_site_staff.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function rpg_site_staff_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_election_settings|user|user|form';
  $field_group->group_name = 'group_election_settings';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Election Settings',
    'weight' => '3',
    'children' => array(
      0 => 'field_eligible_for_system_head',
      1 => 'field_eligible_for_setting_head',
      2 => 'field_became_voter',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-election-settings field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_election_settings|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_voting|user|user|default';
  $field_group->group_name = 'group_voting';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Voting',
    'weight' => '7',
    'children' => array(
      0 => 'field_eligible_for_system_head',
      1 => 'field_eligible_for_setting_head',
      2 => 'field_became_voter',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(),
    ),
  );
  $export['group_voting|user|user|default'] = $field_group;

  return $export;
}
