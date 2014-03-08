<?php
/**
 * @file
 * sh_test.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function sh_test_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_ash_dates|user|user|default';
  $field_group->group_name = 'group_ash_dates';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_storyhosting';
  $field_group->data = array(
    'label' => 'A/SH Test and Renewal Dates',
    'weight' => '6',
    'children' => array(
      0 => 'field_took_ash1',
      1 => 'field_took_ash2',
      2 => 'field_took_ash3',
      3 => 'field_took_eval_test',
      4 => 'field_ash_3_application',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'A/SH Test and Renewal Dates',
      'instance_settings' => array(
        'classes' => 'group-ash-dates field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_ash_dates|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_storyhosting|user|user|default';
  $field_group->group_name = 'group_storyhosting';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Storyhosting',
    'weight' => '3',
    'children' => array(
      0 => 'group_ash_dates',
      1 => 'group_latest_ash_3',
      2 => 'group_storyhost',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-storyhosting field-group-fieldset',
      ),
    ),
  );
  $export['group_storyhosting|user|user|default'] = $field_group;

  return $export;
}
