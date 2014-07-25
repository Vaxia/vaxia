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
      0 => 'field_xp_over_time',
      1 => 'field_artwork_image',
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
    'weight' => '4',
    'children' => array(
      0 => 'field_staff_setting_lead',
      1 => 'field_staff_social_lead',
      2 => 'field_staff_system_lead',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Election Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-election-settings field-group-fieldset',
        'description' => 'Configure your preferred settings for elections. When the next election comes up you will be assumed to be interested in all marked positions.',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_election_settings|user|user|form'] = $field_group;

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
  $field_group->identifier = 'group_sh_dates|user|user|default';
  $field_group->group_name = 'group_sh_dates';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_storyhosting';
  $field_group->data = array(
    'label' => 'SH Dates',
    'weight' => '8',
    'children' => array(
      0 => 'field_became_sh',
      1 => 'field_became_evaluator',
      2 => 'field_powers_expire',
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
    'weight' => '4',
    'children' => array(
      0 => 'field_became_voter',
      1 => 'field_voting_expire',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Proposals and Elections',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_voting|user|user|default'] = $field_group;

  return $export;
}
