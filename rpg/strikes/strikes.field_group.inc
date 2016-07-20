<?php
/**
 * @file
 * strikes.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function strikes_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_actions_taken|node|notation|default';
  $field_group->group_name = 'group_actions_taken';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'notation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Actions taken',
    'weight' => '2',
    'children' => array(
      0 => 'field_results',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-actions-taken field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_actions_taken|node|notation|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_actions_taken|node|notation|form';
  $field_group->group_name = 'group_actions_taken';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'notation';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Actions taken',
    'weight' => '5',
    'children' => array(
      0 => 'field_results',
      1 => 'workflow',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-actions-taken field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_actions_taken|node|notation|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_inc_report|node|notation|default';
  $field_group->group_name = 'group_inc_report';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'notation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Incident report',
    'weight' => '1',
    'children' => array(
      0 => 'body',
      1 => 'field_category',
      2 => 'field_documentation',
      3 => 'field_date_of_incident',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-inc-report field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_inc_report|node|notation|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_inc_report|node|notation|form';
  $field_group->group_name = 'group_inc_report';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'notation';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Incident report',
    'weight' => '3',
    'children' => array(
      0 => 'body',
      1 => 'field_category',
      2 => 'field_documentation',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-inc-report field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_inc_report|node|notation|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_members|node|notation|default';
  $field_group->group_name = 'group_members';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'notation';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Involved members',
    'weight' => '0',
    'children' => array(
      0 => 'field_players',
      1 => 'field_social_members',
      2 => 'field_witnesses',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-members field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_members|node|notation|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_members|node|notation|form';
  $field_group->group_name = 'group_members';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'notation';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Involved members',
    'weight' => '2',
    'children' => array(
      0 => 'field_players',
      1 => 'field_social_members',
      2 => 'field_witnesses',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-members field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_members|node|notation|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_strikes_given|node|notation|form';
  $field_group->group_name = 'group_strikes_given';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'notation';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_actions_taken';
  $field_group->data = array(
    'label' => 'Strikes given',
    'weight' => '6',
    'children' => array(
      0 => 'field_yellow_strike',
      1 => 'field_red_strike',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Strikes given',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-strikes-given field-group-fieldset',
        'description' => 'NOTE: Strikes are ONLY given when the notation is initially created! This is to prevent additional strikes being issued if a notation is being updated. If you need to give a strike for an existing notation - make a new one.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_strikes_given|node|notation|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Actions taken');
  t('Incident report');
  t('Involved members');
  t('Strikes given');

  return $field_groups;
}
