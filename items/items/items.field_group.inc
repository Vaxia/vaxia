<?php
/**
 * @file
 * items.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function items_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_comp_stats|node|items|artwork';
  $field_group->group_name = 'group_comp_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = 'group_companion';
  $field_group->data = array(
    'label' => 'Companion Stats',
    'weight' => '9',
    'children' => array(
      0 => 'field_agility',
      1 => 'field_appearance',
      2 => 'field_awareness',
      3 => 'field_constitution',
      4 => 'field_health',
      5 => 'field_presence',
      6 => 'field_reflexes',
      7 => 'field_charisma',
      8 => 'field_dexterity',
      9 => 'field_endurance',
      10 => 'field_intelligence',
      11 => 'field_life',
      12 => 'field_spirituality',
      13 => 'field_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Companion Stats',
      'instance_settings' => array(
        'classes' => 'group-comp-stats field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_comp_stats|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_comp_stats|node|items|default';
  $field_group->group_name = 'group_comp_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_companion';
  $field_group->data = array(
    'label' => 'Companion Stats',
    'weight' => '9',
    'children' => array(
      0 => 'field_agility',
      1 => 'field_appearance',
      2 => 'field_awareness',
      3 => 'field_constitution',
      4 => 'field_health',
      5 => 'field_presence',
      6 => 'field_reflexes',
      7 => 'field_charisma',
      8 => 'field_dexterity',
      9 => 'field_endurance',
      10 => 'field_intelligence',
      11 => 'field_life',
      12 => 'field_spirituality',
      13 => 'field_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-comp-stats field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_comp_stats|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_comp_stats|node|items|form';
  $field_group->group_name = 'group_comp_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_companion';
  $field_group->data = array(
    'label' => 'Companion Stats',
    'weight' => '135',
    'children' => array(
      0 => 'field_agility',
      1 => 'field_appearance',
      2 => 'field_awareness',
      3 => 'field_constitution',
      4 => 'field_health',
      5 => 'field_presence',
      6 => 'field_reflexes',
      7 => 'field_charisma',
      8 => 'field_dexterity',
      9 => 'field_endurance',
      10 => 'field_intelligence',
      11 => 'field_life',
      12 => 'field_spirituality',
      13 => 'field_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Companion Stats',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-comp-stats field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_comp_stats|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_companion|node|items|artwork';
  $field_group->group_name = 'group_companion';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Companion',
    'weight' => '4',
    'children' => array(
      0 => 'group_skills',
      1 => 'group_comp_stats',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Companion',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-companion field-group-fieldset',
        'description' => 'Companion skills and stats, only used for companion animals and drones.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_companion|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_companion|node|items|default';
  $field_group->group_name = 'group_companion';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Companion',
    'weight' => '5',
    'children' => array(
      0 => 'group_skills',
      1 => 'group_comp_stats',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Companion',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-companion field-group-fieldset',
        'description' => 'Companion skills and stats, only used for companion animals and drones.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_companion|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_companion|node|items|form';
  $field_group->group_name = 'group_companion';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Companion',
    'weight' => '6',
    'children' => array(
      0 => 'group_skills',
      1 => 'group_comp_stats',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Companion',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-companion field-group-fieldset',
        'description' => 'Companion skills and stats, only used for companion animals and drones.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_companion|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_creation_history|node|items|default';
  $field_group->group_name = 'group_creation_history';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Creation History',
    'weight' => '13',
    'children' => array(),
    'format_type' => 'hidden',
    'format_settings' => array(
      'label' => 'Creation History',
      'instance_settings' => array(
        'classes' => 'group-creation-history field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_creation_history|node|items|default'] = $field_group;

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
      1 => 'field_tag_realm',
      2 => 'workflow_current_state',
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
  $field_groups['group_details|node|items|default'] = $field_group;

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
      2 => 'field_tag_realm',
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
  $field_groups['group_details|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_item_stats|node|items|artwork';
  $field_group->group_name = 'group_item_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = 'group_mod_stats';
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
  $field_groups['group_item_stats|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_item_stats|node|items|form';
  $field_group->group_name = 'group_item_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_mod_stats';
  $field_group->data = array(
    'label' => 'Modifies',
    'weight' => '8',
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
  $field_groups['group_item_stats|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mod_stats|node|items|artwork';
  $field_group->group_name = 'group_mod_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Modify Stats',
    'weight' => '3',
    'children' => array(
      0 => 'group_item_stats',
      1 => 'group_overlaps',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Modify Stats',
      'instance_settings' => array(
        'classes' => 'group-mod-stats field-group-fieldset',
        'description' => 'Modifications to a character\'s stats.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_mod_stats|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mod_stats|node|items|default';
  $field_group->group_name = 'group_mod_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Modify Stats',
    'weight' => '4',
    'children' => array(
      0 => 'group_stats',
      1 => 'group_overlaps',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Modify Stats',
      'instance_settings' => array(
        'classes' => 'group-mod-stats field-group-fieldset',
        'description' => 'Modifications to a character\'s stats.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_mod_stats|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mod_stats|node|items|form';
  $field_group->group_name = 'group_mod_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Modify Stats',
    'weight' => '5',
    'children' => array(
      0 => 'group_item_stats',
      1 => 'group_overlaps',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Modify Stats',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-mod-stats field-group-fieldset',
        'description' => 'Modify the stats of the character using the item. Use these fields for items that provide bonuses to the character, or replace the character\'s stat when rolled.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_mod_stats|node|items|form'] = $field_group;

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
    'weight' => '5',
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
  $field_groups['group_notes|node|items|artwork'] = $field_group;

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
    'weight' => '6',
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
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_notes|node|items|default'] = $field_group;

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
    'weight' => '8',
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
  $field_groups['group_notes|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overlaps|node|items|artwork';
  $field_group->group_name = 'group_overlaps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = 'group_mod_stats';
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
  $field_groups['group_overlaps|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overlaps|node|items|default';
  $field_group->group_name = 'group_overlaps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_mod_stats';
  $field_group->data = array(
    'label' => 'Overlaps',
    'weight' => '6',
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
  $field_groups['group_overlaps|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overlaps|node|items|form';
  $field_group->group_name = 'group_overlaps';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_mod_stats';
  $field_group->data = array(
    'label' => 'Overlaps',
    'weight' => '9',
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
  $field_groups['group_overlaps|node|items|form'] = $field_group;

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
    'weight' => '7',
    'children' => array(
      0 => 'field_in_shop',
      1 => 'field_declared_value',
      2 => 'field_sellback',
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
  $field_groups['group_shop_details|node|items|default'] = $field_group;

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
    'weight' => '7',
    'children' => array(
      0 => 'field_value',
      1 => 'field_in_shop',
      2 => 'field_declared_value',
      3 => 'field_calc_value',
      4 => 'field_sellback',
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
  $field_groups['group_shop_details|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|items|artwork';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = 'group_companion';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '10',
    'children' => array(
      0 => 'field_skill',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Skills',
      'instance_settings' => array(
        'classes' => 'group-skills field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_skills|node|items|artwork'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|items|default';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_companion';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '10',
    'children' => array(
      0 => 'field_skill',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-skills field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_skills|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|items|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_companion';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '150',
    'children' => array(
      0 => 'field_skill',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Skills',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-skills field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_skills|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_stats|node|items|default';
  $field_group->group_name = 'group_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_mod_stats';
  $field_group->data = array(
    'label' => 'Modifies',
    'weight' => '5',
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
  $field_groups['group_stats|node|items|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_uses|node|items|artwork';
  $field_group->group_name = 'group_uses';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'items';
  $field_group->mode = 'artwork';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Item Type',
    'weight' => '2',
    'children' => array(
      0 => 'field_item_uses',
      1 => 'field_pp_created',
      2 => 'field_item_use_day',
      3 => 'field_item_spent_uses',
      4 => 'field_item_type',
      5 => 'field_weapon_class',
      6 => 'field_item_difficulty',
      7 => 'field_armor_absorbs',
      8 => 'field_armor_penalty',
      9 => 'field_fixed_strength',
      10 => 'field_absorb_type',
      11 => 'field_build_points',
      12 => 'field_weapon_sub_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Item Type',
      'instance_settings' => array(
        'classes' => 'group-uses field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_uses|node|items|artwork'] = $field_group;

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
    'label' => 'Item Type',
    'weight' => '3',
    'children' => array(
      0 => 'field_item_uses',
      1 => 'field_pp_created',
      2 => 'field_item_use_day',
      3 => 'field_item_spent_uses',
      4 => 'field_item_type',
      5 => 'field_weapon_class',
      6 => 'field_item_difficulty',
      7 => 'field_armor_absorbs',
      8 => 'field_armor_penalty',
      9 => 'field_fixed_strength',
      10 => 'field_absorb_type',
      11 => 'field_build_points',
      12 => 'field_weapon_sub_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Item Type',
      'instance_settings' => array(
        'classes' => 'group-uses field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_uses|node|items|default'] = $field_group;

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
    'label' => 'Item Type',
    'weight' => '4',
    'children' => array(
      0 => 'field_item_uses',
      1 => 'field_pp_created',
      2 => 'field_item_use_day',
      3 => 'field_item_spent_uses',
      4 => 'field_item_type',
      5 => 'field_weapon_class',
      6 => 'field_item_difficulty',
      7 => 'field_armor_absorbs',
      8 => 'field_armor_penalty',
      9 => 'field_fixed_strength',
      10 => 'field_absorb_type',
      11 => 'field_build_points',
      12 => 'field_weapon_sub_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Item Type',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-uses field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_uses|node|items|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Companion');
  t('Companion Stats');
  t('Creation History');
  t('Details');
  t('Item Type');
  t('Modifies');
  t('Modify Stats');
  t('Notes');
  t('Overlaps');
  t('Shop details');
  t('Skills');

  return $field_groups;
}
