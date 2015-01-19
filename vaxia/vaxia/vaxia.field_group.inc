<?php
/**
 * @file
 * vaxia.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function vaxia_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_artwork|node|character_sheet|default';
  $field_group->group_name = 'group_artwork';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Art',
    'weight' => '2',
    'children' => array(
      0 => 'field_artwork_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-artwork field-group-fieldset',
      ),
    ),
  );
  $export['group_artwork|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_artwork|node|character_sheet|form';
  $field_group->group_name = 'group_artwork';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Art',
    'weight' => '5',
    'children' => array(
      0 => 'field_artwork_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-artwork field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_artwork|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basics|node|character_sheet|form';
  $field_group->group_name = 'group_basics';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Basics',
    'weight' => '0',
    'children' => array(
      0 => 'field_is_npc',
      1 => 'field_is_open_npc',
      2 => 'field_is_storyteller',
      3 => 'field_post_color',
      4 => 'field_background_color',
      5 => 'field_associated_locations',
      6 => 'title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-basics field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_basics|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_descriptions|node|character_sheet|form';
  $field_group->group_name = 'group_descriptions';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Descriptions',
    'weight' => '3',
    'children' => array(
      0 => 'field_description',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-descriptions field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_descriptions|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|character_sheet|default';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_age_app',
      1 => 'field_age_real',
      2 => 'field_is_npc',
      3 => 'field_is_open_npc',
      4 => 'field_race',
      5 => 'field_realm',
      6 => 'field_sex',
      7 => 'field_is_storyteller',
      8 => 'field_conditions',
      9 => 'field_associated_locations',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-details field-group-fieldset',
      ),
    ),
  );
  $export['group_details|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|character_sheet|form';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '4',
    'children' => array(
      0 => 'field_age_app',
      1 => 'field_age_real',
      2 => 'field_race',
      3 => 'field_realm',
      4 => 'field_sex',
      5 => 'field_conditions',
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
  $export['group_details|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_items|node|character_sheet|default';
  $field_group->group_name = 'group_items';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Items',
    'weight' => '5',
    'children' => array(
      0 => 'field_economy',
      1 => 'field_items',
      2 => 'field_property',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-items field-group-fieldset',
      ),
    ),
  );
  $export['group_items|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_items|node|character_sheet|form';
  $field_group->group_name = 'group_items';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Items',
    'weight' => '8',
    'children' => array(
      0 => 'field_economy',
      1 => 'field_items',
      2 => 'field_property',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-items field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_items|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|character_sheet|default';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '6',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_character_goals',
      2 => 'field_content_limits',
      3 => 'field_visibility',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-notes field-group-fieldset',
      ),
    ),
  );
  $export['group_notes|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|character_sheet|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '9',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_character_goals',
      2 => 'field_content_limits',
      3 => 'field_visibility',
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
  $export['group_notes|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_personal|node|character_sheet|default';
  $field_group->group_name = 'group_personal';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Personal',
    'weight' => '0',
    'children' => array(
      0 => 'field_background',
      1 => 'field_description',
      2 => 'field_last_name',
      3 => 'field_occupation',
      4 => 'field_personality',
      5 => 'field_titles',
      6 => 'field_artwork_image',
      7 => 'field_achivements',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-personal field-group-fieldset',
      ),
    ),
  );
  $export['group_personal|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_personal|node|character_sheet|form';
  $field_group->group_name = 'group_personal';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Personal',
    'weight' => '1',
    'children' => array(
      0 => 'field_background',
      1 => 'field_last_name',
      2 => 'field_occupation',
      3 => 'field_personality',
      4 => 'field_titles',
      5 => 'field_achivements',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-personal field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_personal|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_public_info|node|character_sheet|default';
  $field_group->group_name = 'group_public_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Public information',
    'weight' => '1',
    'children' => array(
      0 => 'field_public_information',
      1 => 'field_sh_public_information',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-public-info field-group-fieldset',
      ),
    ),
  );
  $export['group_public_info|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_public_info|node|character_sheet|form';
  $field_group->group_name = 'group_public_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Public information',
    'weight' => '2',
    'children' => array(
      0 => 'field_public_information',
      1 => 'field_sh_public_information',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-public-info field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_public_info|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sh_notes|node|character_sheet|default';
  $field_group->group_name = 'group_sh_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'SH Notes',
    'weight' => '7',
    'children' => array(
      0 => 'field_sh_notes',
      1 => 'field_sh_private_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-sh-notes field-group-fieldset',
      ),
    ),
  );
  $export['group_sh_notes|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sh_notes|node|character_sheet|form';
  $field_group->group_name = 'group_sh_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'SH Notes',
    'weight' => '10',
    'children' => array(
      0 => 'field_sh_notes',
      1 => 'field_sh_private_notes',
      2 => 'field_skill_review',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-sh-notes field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_sh_notes|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|character_sheet|default';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '4',
    'children' => array(
      0 => 'field_skill',
      1 => 'field_languages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-skills field-group-fieldset',
      ),
    ),
  );
  $export['group_skills|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|character_sheet|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '7',
    'children' => array(
      0 => 'field_skill',
      1 => 'field_languages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-skills field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_skills|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_stats|node|character_sheet|default';
  $field_group->group_name = 'group_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Stats',
    'weight' => '3',
    'children' => array(
      0 => 'field_agility',
      1 => 'field_appearance',
      2 => 'field_awareness',
      3 => 'field_charisma',
      4 => 'field_constitution',
      5 => 'field_dexterity',
      6 => 'field_endurance',
      7 => 'field_health',
      8 => 'field_intelligence',
      9 => 'field_life',
      10 => 'field_presence',
      11 => 'field_reflexes',
      12 => 'field_spirituality',
      13 => 'field_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => ' group-stats field-group-fieldset',
      ),
    ),
  );
  $export['group_stats|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_stats|node|character_sheet|form';
  $field_group->group_name = 'group_stats';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Stats',
    'weight' => '6',
    'children' => array(
      0 => 'field_agility',
      1 => 'field_appearance',
      2 => 'field_awareness',
      3 => 'field_charisma',
      4 => 'field_constitution',
      5 => 'field_dexterity',
      6 => 'field_endurance',
      7 => 'field_health',
      8 => 'field_intelligence',
      9 => 'field_life',
      10 => 'field_presence',
      11 => 'field_reflexes',
      12 => 'field_spirituality',
      13 => 'field_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Stats',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => ' group-stats field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_stats|node|character_sheet|form'] = $field_group;

  return $export;
}
