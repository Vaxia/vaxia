<?php
/**
 * @file
 * items.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function items_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_creation_history|node|items|default';
  $field_group->group_name = 'group_creation_history';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_notes';
  $field_group->data = array(
    'label' => 'Creation History',
    'weight' => '52',
    'children' => array(
      0 => 'field_creation_history',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Creation History',
      'instance_settings' => array(
        'classes' => 'group-creation-history field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_creation_history|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|items|default';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_character_owner',
      1 => 'field_realm',
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
  $export['group_details|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|items|form';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '3',
    'children' => array(
      0 => 'field_character_owner',
      1 => 'field_realm',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-details field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_details|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_item_stats|node|items|artwork';
  $field_group->group_name = 'group_item_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Modifies',
    'weight' => '3',
    'children' => array(
      0 => 'field_item_agility',
      1 => 'field_item_appearance',
      2 => 'field_item_awareness',
      3 => 'field_item_charisma',
      4 => 'field_item_constitution',
      5 => 'field_item_dexterity',
      6 => 'field_item_endurance',
      7 => 'field_item_health',
      8 => 'field_item_intelligence',
      9 => 'field_item_life',
      10 => 'field_item_presence',
      11 => 'field_item_reflexes',
      12 => 'field_item_skill',
      13 => 'field_item_spirituality',
      14 => 'field_item_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Modifies',
      'instance_settings' => array(
        'classes' => 'group-overlaps field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_item_stats|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_item_stats|node|items|form';
  $field_group->group_name = 'group_item_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Modifies',
    'weight' => '7',
    'children' => array(
      0 => 'field_item_agility',
      1 => 'field_item_appearance',
      2 => 'field_item_awareness',
      3 => 'field_item_charisma',
      4 => 'field_item_constitution',
      5 => 'field_item_dexterity',
      6 => 'field_item_endurance',
      7 => 'field_item_health',
      8 => 'field_item_intelligence',
      9 => 'field_item_life',
      10 => 'field_item_presence',
      11 => 'field_item_reflexes',
      12 => 'field_item_skill',
      13 => 'field_item_spirituality',
      14 => 'field_item_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Modifies',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-item-stats field-group-fieldset',
        'description' => 'Directly modify a character (or item if overlapping) stat. Negative numbers will lower the stat.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_item_stats|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|items|artwork';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '7',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_sh_notes',
      2 => 'field_sh_private_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Notes',
      'instance_settings' => array(
        'classes' => ' group-notes field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_notes|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|items|default';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '8',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_sh_notes',
      2 => 'field_sh_private_notes',
      3 => 'workflow_current_state',
      4 => 'group_creation_history',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Notes',
      'instance_settings' => array(
        'classes' => ' group-notes field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_notes|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|items|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '9',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_sh_notes',
      2 => 'field_sh_private_notes',
      3 => 'field_creation_history',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-notes field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_notes|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overlaps|node|items|artwork';
  $field_group->group_name = 'group_overlaps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Overlaps',
    'weight' => '4',
    'children' => array(
      0 => 'field_item_over_agility',
      1 => 'field_item_over_appearance',
      2 => 'field_item_over_awareness',
      3 => 'field_item_over_charisma',
      4 => 'field_item_over_constitution',
      5 => 'field_item_over_dexterity',
      6 => 'field_item_over_endurance',
      7 => 'field_item_over_health',
      8 => 'field_item_over_intellegence',
      9 => 'field_item_over_life',
      10 => 'field_item_over_presence',
      11 => 'field_item_over_reflexes',
      12 => 'field_item_over_skill',
      13 => 'field_item_over_spirituality',
      14 => 'field_item_over_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Overlaps',
      'instance_settings' => array(
        'classes' => ' group-overlaps field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_overlaps|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overlaps|node|items|default';
  $field_group->group_name = 'group_overlaps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Overlaps',
    'weight' => '7',
    'children' => array(
      0 => 'field_item_over_agility',
      1 => 'field_item_over_appearance',
      2 => 'field_item_over_awareness',
      3 => 'field_item_over_charisma',
      4 => 'field_item_over_constitution',
      5 => 'field_item_over_dexterity',
      6 => 'field_item_over_endurance',
      7 => 'field_item_over_health',
      8 => 'field_item_over_intellegence',
      9 => 'field_item_over_life',
      10 => 'field_item_over_presence',
      11 => 'field_item_over_reflexes',
      12 => 'field_item_over_skill',
      13 => 'field_item_over_spirituality',
      14 => 'field_item_over_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Overlaps',
      'instance_settings' => array(
        'classes' => ' group-overlaps field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_overlaps|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overlaps|node|items|form';
  $field_group->group_name = 'group_overlaps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Overlaps',
    'weight' => '8',
    'children' => array(
      0 => 'field_item_over_agility',
      1 => 'field_item_over_appearance',
      2 => 'field_item_over_awareness',
      3 => 'field_item_over_charisma',
      4 => 'field_item_over_constitution',
      5 => 'field_item_over_dexterity',
      6 => 'field_item_over_endurance',
      7 => 'field_item_over_health',
      8 => 'field_item_over_intellegence',
      9 => 'field_item_over_life',
      10 => 'field_item_over_presence',
      11 => 'field_item_over_reflexes',
      12 => 'field_item_over_skill',
      13 => 'field_item_over_spirituality',
      14 => 'field_item_over_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Overlaps',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-overlaps field-group-fieldset',
        'description' => 'Replace a character stat with the item stat.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_overlaps|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_penalties|node|items|artwork';
  $field_group->group_name = 'group_penalties';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Penalties',
    'weight' => '6',
    'children' => array(
      0 => 'field_item_pen_agility',
      1 => 'field_item_pen_appearance',
      2 => 'field_item_pen_awareness',
      3 => 'field_item_pen_charisma',
      4 => 'field_item_pen_constitution',
      5 => 'field_item_pen_dexterity',
      6 => 'field_item_pen_endurance',
      7 => 'field_item_pen_health',
      8 => 'field_item_pen_intellegence',
      9 => 'field_item_pen_life',
      10 => 'field_item_pen_presence',
      11 => 'field_item_pen_reflexes',
      12 => 'field_item_pen_skill',
      13 => 'field_item_pen_spirituality',
      14 => 'field_item_pen_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Penalties',
      'instance_settings' => array(
        'classes' => 'group-overlaps field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_penalties|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_penalties|node|items|default';
  $field_group->group_name = 'group_penalties';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Penalties',
    'weight' => '11',
    'children' => array(
      0 => 'field_item_pen_agility',
      1 => 'field_item_pen_appearance',
      2 => 'field_item_pen_awareness',
      3 => 'field_item_pen_charisma',
      4 => 'field_item_pen_constitution',
      5 => 'field_item_pen_dexterity',
      6 => 'field_item_pen_endurance',
      7 => 'field_item_pen_health',
      8 => 'field_item_pen_intellegence',
      9 => 'field_item_pen_life',
      10 => 'field_item_pen_presence',
      11 => 'field_item_pen_reflexes',
      12 => 'field_item_pen_skill',
      13 => 'field_item_pen_spirituality',
      14 => 'field_item_pen_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Penalties',
      'instance_settings' => array(
        'classes' => ' group-penalties field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_penalties|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_penalties|node|items|form';
  $field_group->group_name = 'group_penalties';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Penalties',
    'weight' => '9',
    'children' => array(
      0 => 'field_item_pen_agility',
      1 => 'field_item_pen_appearance',
      2 => 'field_item_pen_awareness',
      3 => 'field_item_pen_charisma',
      4 => 'field_item_pen_constitution',
      5 => 'field_item_pen_dexterity',
      6 => 'field_item_pen_endurance',
      7 => 'field_item_pen_health',
      8 => 'field_item_pen_intellegence',
      9 => 'field_item_pen_life',
      10 => 'field_item_pen_presence',
      11 => 'field_item_pen_reflexes',
      12 => 'field_item_pen_skill',
      13 => 'field_item_pen_spirituality',
      14 => 'field_item_pen_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Penalties',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-penalties field-group-fieldset',
        'description' => 'If a stat is below the requirement, how much to penalize by. Use negative numbers to reduce the stat.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_penalties|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_percentages|node|items|artwork';
  $field_group->group_name = 'group_percentages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Percentages',
    'weight' => '2',
    'children' => array(
      0 => 'field_item_perc_strength',
      1 => 'field_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Percentages',
      'instance_settings' => array(
        'classes' => ' group-overlaps field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_percentages|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_percentages|node|items|default';
  $field_group->group_name = 'group_percentages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Percentages',
    'weight' => '7',
    'children' => array(
      0 => 'field_item_perc_strength',
      1 => 'field_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Percentages',
      'instance_settings' => array(
        'classes' => ' group-percentages field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_percentages|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_percentages|node|items|form';
  $field_group->group_name = 'group_percentages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_details';
  $field_group->data = array(
    'label' => 'Additional percent',
    'weight' => '6',
    'children' => array(
      0 => 'field_item_perc_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Additional percent',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-percentages field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_percentages|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_requires|node|items|artwork';
  $field_group->group_name = 'group_requires';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Requires',
    'weight' => '5',
    'children' => array(
      0 => 'field_item_required_agility',
      1 => 'field_item_required_appearance',
      2 => 'field_item_required_awareness',
      3 => 'field_item_required_charisma',
      4 => 'field_item_required_constitution',
      5 => 'field_item_required_dexterity',
      6 => 'field_item_required_endurance',
      7 => 'field_item_required_health',
      8 => 'field_item_required_intelligence',
      9 => 'field_item_required_life',
      10 => 'field_item_required_presence',
      11 => 'field_item_required_reflexes',
      12 => 'field_item_required_skill',
      13 => 'field_item_required_spirituality',
      14 => 'field_item_required_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Requires',
      'instance_settings' => array(
        'classes' => 'group-overlaps field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_requires|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_requires|node|items|default';
  $field_group->group_name = 'group_requires';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Requires',
    'weight' => '10',
    'children' => array(
      0 => 'field_item_required_agility',
      1 => 'field_item_required_appearance',
      2 => 'field_item_required_awareness',
      3 => 'field_item_required_charisma',
      4 => 'field_item_required_constitution',
      5 => 'field_item_required_dexterity',
      6 => 'field_item_required_endurance',
      7 => 'field_item_required_health',
      8 => 'field_item_required_intelligence',
      9 => 'field_item_required_life',
      10 => 'field_item_required_presence',
      11 => 'field_item_required_reflexes',
      12 => 'field_item_required_skill',
      13 => 'field_item_required_spirituality',
      14 => 'field_item_required_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Requires',
      'instance_settings' => array(
        'classes' => ' group-requires field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_requires|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_requires|node|items|form';
  $field_group->group_name = 'group_requires';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Requires',
    'weight' => '8',
    'children' => array(
      0 => 'field_item_required_agility',
      1 => 'field_item_required_appearance',
      2 => 'field_item_required_awareness',
      3 => 'field_item_required_charisma',
      4 => 'field_item_required_constitution',
      5 => 'field_item_required_dexterity',
      6 => 'field_item_required_endurance',
      7 => 'field_item_required_health',
      8 => 'field_item_required_intelligence',
      9 => 'field_item_required_life',
      10 => 'field_item_required_presence',
      11 => 'field_item_required_reflexes',
      12 => 'field_item_required_skill',
      13 => 'field_item_required_spirituality',
      14 => 'field_item_required_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Requires',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-requires field-group-fieldset',
        'description' => 'Set minimum stat required by item to avoid penalties.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_requires|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_shop_details|node|items|default';
  $field_group->group_name = 'group_shop_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Shop details',
    'weight' => '3',
    'children' => array(
      0 => 'field_value',
      1 => 'field_in_shop',
      2 => 'field_declared_value',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-shop-details field-group-fieldset',
      ),
    ),
  );
  $export['group_shop_details|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_shop_details|node|items|form';
  $field_group->group_name = 'group_shop_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Shop details',
    'weight' => '4',
    'children' => array(
      0 => 'field_value',
      1 => 'field_in_shop',
      2 => 'field_declared_value',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-shop-details field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_shop_details|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_stats|node|items|default';
  $field_group->group_name = 'group_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Modifies',
    'weight' => '6',
    'children' => array(
      0 => 'field_item_agility',
      1 => 'field_item_appearance',
      2 => 'field_item_awareness',
      3 => 'field_item_charisma',
      4 => 'field_item_constitution',
      5 => 'field_item_dexterity',
      6 => 'field_item_endurance',
      7 => 'field_item_health',
      8 => 'field_item_intelligence',
      9 => 'field_item_life',
      10 => 'field_item_presence',
      11 => 'field_item_reflexes',
      12 => 'field_item_skill',
      13 => 'field_item_spirituality',
      14 => 'field_item_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Modifies',
      'instance_settings' => array(
        'classes' => ' group-stats field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_stats|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_uses|node|items|default';
  $field_group->group_name = 'group_uses';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Item Uses',
    'weight' => '4',
    'children' => array(
      0 => 'field_item_uses',
      1 => 'field_pp_created',
      2 => 'field_item_use_day',
      3 => 'field_item_spent_uses',
      4 => 'field_item_type',
      5 => 'field_weapon_class',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-uses field-group-fieldset',
      ),
    ),
  );
  $export['group_uses|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_uses|node|items|form';
  $field_group->group_name = 'group_uses';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Item Uses',
    'weight' => '5',
    'children' => array(
      0 => 'field_item_uses',
      1 => 'field_pp_created',
      2 => 'field_item_use_day',
      3 => 'field_item_spent_uses',
      4 => 'field_item_type',
      5 => 'field_weapon_class',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-uses field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_uses|node|items|form'] = $field_group;

  return $export;
}
