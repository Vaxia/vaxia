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
    'weight' => '9',
    'children' => array(
      0 => 'field_took_ash1',
      1 => 'field_took_ash2',
      2 => 'field_took_ash3',
      3 => 'field_took_eval_test',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'A/SH Test and Renewal Dates',
      'instance_settings' => array(
        'classes' => 'group-ash-dates field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_ash_dates|user|user|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_hidden|user|user|form';
  $field_group->group_name = 'group_hidden';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Additional Information',
    'weight' => '5',
    'children' => array(
      0 => 'field_took_ash1',
      1 => 'field_took_ash2',
      2 => 'field_became_sh',
      3 => 'field_took_ash3',
      4 => 'field_xp_over_time',
      5 => 'field_took_eval_test',
      6 => 'field_became_evaluator',
      7 => 'field_became_voter',
      8 => 'field_ash_3_answers',
      9 => 'field_ash_3_application',
      10 => 'field_storyhost_character',
      11 => 'field_age_requirement',
      12 => 'field_took_system_walkthrough',
      13 => 'field_took_setting_walkthrough',
      14 => 'field_took_social_walkthrough',
      15 => 'field_powers_expire',
      16 => 'field_voting_expire',
      17 => 'field_personal_folder',
      18 => 'field_donated',
      19 => 'field_terms_and_conditions',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Additional Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-hidden field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_hidden|user|user|form'] = $field_group;

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
    'weight' => '5',
    'children' => array(
      0 => 'field_storyhost_character',
      1 => 'group_ash_dates',
      2 => 'group_sh_dates',
      3 => 'group_latest_ash_3',
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
