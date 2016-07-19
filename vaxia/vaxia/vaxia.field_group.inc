<?php
/**
 * @file
 * vaxia.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function vaxia_field_group_info() {
  $field_groups = array();

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
  $field_groups['group_artwork|node|character_sheet|default'] = $field_group;

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
    'weight' => '6',
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
  $field_groups['group_artwork|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_aspects|field_collection_item|field_skill|default';
  $field_group->group_name = 'group_aspects';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_skill';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Aspects',
    'weight' => '8',
    'children' => array(
      0 => 'field_aspect_one',
      1 => 'field_aspect_two',
      2 => 'field_aspect_three',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Aspects',
      'instance_settings' => array(
        'id' => '',
        'classes' => 'group-aspects field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_aspects|field_collection_item|field_skill|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_aspects|field_collection_item|field_skill|form';
  $field_group->group_name = 'group_aspects';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_skill';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Aspects',
    'weight' => '3',
    'children' => array(
      0 => 'field_aspect_one',
      1 => 'field_aspect_two',
      2 => 'field_aspect_three',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Aspects',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-aspects field-group-fieldset',
        'description' => 'Select aspects for your skill to clarify for our char eval team what your skill can do. Information on aspects can be found <a href="http://vaxia.org/forums/skill-verbs/aspects" target="_blank">here</a>.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_aspects|field_collection_item|field_skill|form'] = $field_group;

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
    'weight' => '1',
    'children' => array(
      0 => 'field_is_npc',
      1 => 'field_is_storyteller',
      2 => 'field_post_color',
      3 => 'field_background_color',
      4 => 'title',
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
  $field_groups['group_basics|node|character_sheet|form'] = $field_group;

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
    'weight' => '4',
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
  $field_groups['group_descriptions|node|character_sheet|form'] = $field_group;

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
      1 => 'field_is_npc',
      2 => 'field_race',
      3 => 'field_sex',
      4 => 'field_is_storyteller',
      5 => 'field_conditions',
      6 => 'field_other_species',
      7 => 'field_sirian_species',
      8 => 'field_personal_room',
      9 => 'field_tag_realm',
      10 => 'field_age',
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
  $field_groups['group_details|node|character_sheet|default'] = $field_group;

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
    'weight' => '5',
    'children' => array(
      0 => 'field_age_app',
      1 => 'field_race',
      2 => 'field_sex',
      3 => 'field_conditions',
      4 => 'field_other_species',
      5 => 'field_sirian_species',
      6 => 'field_personal_room',
      7 => 'field_real_age',
      8 => 'field_birth_month',
      9 => 'field_birth_day',
      10 => 'field_tag_realm',
      11 => 'field_age',
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
  $field_groups['group_details|node|character_sheet|form'] = $field_group;

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
  $field_groups['group_items|node|character_sheet|default'] = $field_group;

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
    'weight' => '9',
    'children' => array(
      0 => 'field_economy',
      1 => 'field_items',
      2 => 'field_property',
      3 => 'field_currently_wearing',
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
  $field_groups['group_items|node|character_sheet|form'] = $field_group;

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
  $field_groups['group_notes|node|character_sheet|default'] = $field_group;

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
    'weight' => '10',
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
  $field_groups['group_notes|node|character_sheet|form'] = $field_group;

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
    'weight' => '1',
    'children' => array(
      0 => 'field_background',
      1 => 'field_description',
      2 => 'field_last_name',
      3 => 'field_occupation',
      4 => 'field_personality',
      5 => 'field_titles',
      6 => 'field_artwork_image',
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
  $field_groups['group_personal|node|character_sheet|default'] = $field_group;

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
    'weight' => '2',
    'children' => array(
      0 => 'field_background',
      1 => 'field_last_name',
      2 => 'field_occupation',
      3 => 'field_personality',
      4 => 'field_titles',
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
  $field_groups['group_personal|node|character_sheet|form'] = $field_group;

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
    'weight' => '0',
    'children' => array(
      0 => 'field_achivements',
      1 => 'field_public_information',
      2 => 'field_sh_public_information',
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
  $field_groups['group_public_info|node|character_sheet|default'] = $field_group;

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
    'weight' => '3',
    'children' => array(
      0 => 'field_achivements',
      1 => 'field_public_information',
      2 => 'field_sh_public_information',
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
  $field_groups['group_public_info|node|character_sheet|form'] = $field_group;

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
  $field_groups['group_sh_notes|node|character_sheet|default'] = $field_group;

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
    'weight' => '11',
    'children' => array(
      0 => 'field_sh_notes',
      1 => 'field_sh_private_notes',
      2 => 'field_item_shop',
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
  $field_groups['group_sh_notes|node|character_sheet|form'] = $field_group;

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
      1 => 'field_vaxia_languages',
      2 => 'field_sirian_languages',
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
  $field_groups['group_skills|node|character_sheet|default'] = $field_group;

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
    'weight' => '8',
    'children' => array(
      0 => 'field_skill',
      1 => 'field_vaxia_languages',
      2 => 'field_sirian_languages',
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
  $field_groups['group_skills|node|character_sheet|form'] = $field_group;

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
  $field_groups['group_stats|node|character_sheet|default'] = $field_group;

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
    'weight' => '7',
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
  $field_groups['group_stats|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tech_lang|field_collection_item|field_skill|default';
  $field_group->group_name = 'group_tech_lang';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_skill';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tech Languages',
    'weight' => '12',
    'children' => array(
      0 => 'field_tech_languages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-tech-lang field-group-fieldset',
      ),
    ),
  );
  $field_groups['group_tech_lang|field_collection_item|field_skill|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_tech_lang|field_collection_item|field_skill|form';
  $field_group->group_name = 'group_tech_lang';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_skill';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Tech languages',
    'weight' => '4',
    'children' => array(
      0 => 'field_tech_languages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Tech languages',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-tech-lang field-group-fieldset',
        'description' => 'If this is a Sirian tech skill, select two technical languages to be familiar with. Skills start with two languages by default.',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_tech_lang|field_collection_item|field_skill|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Art');
  t('Aspects');
  t('Basics');
  t('Descriptions');
  t('Details');
  t('Items');
  t('Notes');
  t('Personal');
  t('Public information');
  t('SH Notes');
  t('Skills');
  t('Stats');
  t('Tech Languages');
  t('Tech languages');

  return $field_groups;
}
