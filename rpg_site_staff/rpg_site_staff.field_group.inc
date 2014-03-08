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
  $field_group->identifier = 'group_details|user|user|default';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '0',
    'children' => array(
      0 => 'field_artwork',
      1 => 'field_xp_over_time',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-details field-group-fieldset',
      ),
    ),
  );
  $export['group_details|user|user|default'] = $field_group;

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
    'weight' => '5',
    'children' => array(
      0 => 'field_staff_vaxia_world_head',
      1 => 'field_staff_sirian_world_head',
      2 => 'field_staff_academy_head',
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
  $field_group->identifier = 'group_sh_dates|user|user|default';
  $field_group->group_name = 'group_sh_dates';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'SH Dates',
    'weight' => '2',
    'children' => array(
      0 => 'field_became_sh',
      1 => 'field_became_evaluator',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-sh-dates field-group-fieldset',
      ),
    ),
  );
  $export['group_sh_dates|user|user|default'] = $field_group;

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
    'label' => 'Proposals and Elections',
    'weight' => '5',
    'children' => array(
      0 => 'field_became_voter',
      1 => 'field_staff_vaxia_world_head',
      2 => 'field_staff_sirian_world_head',
      3 => 'field_staff_academy_head',
    ),
    'format_type' => 'hidden',
    'format_settings' => array(
      'label' => 'Proposals and Elections',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_voting|user|user|default'] = $field_group;

  return $export;
}
