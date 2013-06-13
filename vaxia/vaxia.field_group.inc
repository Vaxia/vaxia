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
  $field_group->identifier = 'group_artwork|node|artwork|form';
  $field_group->group_name = 'group_artwork';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'artwork';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Art',
    'weight' => '1',
    'children' => array(
      0 => 'field_artwork_image',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_artwork|node|artwork|form'] = $field_group;

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
    'weight' => '4',
    'children' => array(
      0 => 'field_artwork',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
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
      0 => 'field_artwork',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_artwork|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_art|node|npc|form';
  $field_group->group_name = 'group_art';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'npc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Art',
    'weight' => '3',
    'children' => array(
      0 => 'field_artwork',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_art|node|npc|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_associates|node|character_sheet|default';
  $field_group->group_name = 'group_associates';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Associates',
    'weight' => '8',
    'children' => array(
      0 => 'field_npcs',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_associates|node|character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_associates|node|character_sheet|form';
  $field_group->group_name = 'group_associates';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Associates',
    'weight' => '9',
    'children' => array(
      0 => 'field_npcs',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_associates|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|artwork|form';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'artwork';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '0',
    'children' => array(
      0 => 'field_artwork_credit',
      1 => 'field_website',
      2 => 'title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_details|node|artwork|form'] = $field_group;

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
    'weight' => '3',
    'children' => array(
      0 => 'field_age_app',
      1 => 'field_age_real',
      2 => 'field_race',
      3 => 'field_realm',
      4 => 'field_sex',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
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
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_details|node|character_sheet|form'] = $field_group;

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
    'weight' => '0',
    'children' => array(
      0 => 'field_item_desc',
      1 => 'field_item_history',
      2 => 'field_value',
      3 => 'field_character_owner',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
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
    'weight' => '1',
    'children' => array(
      0 => 'field_item_desc',
      1 => 'field_item_history',
      2 => 'field_value',
      3 => 'field_character_owner',
      4 => 'title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_details|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_details|node|npc|form';
  $field_group->group_name = 'group_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'npc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '2',
    'children' => array(
      0 => 'field_age_app',
      1 => 'field_age_real',
      2 => 'field_race',
      3 => 'field_sex',
      4 => 'field_realm',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_details|node|npc|form'] = $field_group;

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
    'weight' => '2',
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
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_item_stats|node|items|form'] = $field_group;

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
    'weight' => '7',
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
        'classes' => '',
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
        'classes' => '',
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
    'weight' => '9',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_sh_notes',
      2 => 'field_sh_private_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
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
    'weight' => '10',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_sh_notes',
      2 => 'field_sh_private_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_notes|node|character_sheet|form'] = $field_group;

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
    'weight' => '16',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_sh_notes',
      2 => 'field_sh_private_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
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
    'weight' => '6',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_sh_notes',
      2 => 'field_sh_private_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_notes|node|items|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|npc|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'npc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '4',
    'children' => array(
      0 => 'field_sh_notes',
      1 => 'field_player_notes',
      2 => 'field_sh_private_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_notes|node|npc|form'] = $field_group;

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
    'weight' => '4',
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
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
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
    'weight' => '5',
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
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_penalties|node|items|form'] = $field_group;

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
    'weight' => '2',
    'children' => array(),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
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
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Percentages',
    'weight' => '3',
    'children' => array(
      0 => 'field_percents',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_percentages|node|items|form'] = $field_group;

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
    'weight' => '2',
    'children' => array(
      0 => 'field_background',
      1 => 'field_description',
      2 => 'field_last_name',
      3 => 'field_occupation',
      4 => 'field_personality',
      5 => 'field_titles',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
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
    'weight' => '3',
    'children' => array(
      0 => 'field_background',
      1 => 'field_description',
      2 => 'field_last_name',
      3 => 'field_occupation',
      4 => 'field_personality',
      5 => 'field_titles',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_personal|node|character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_personal|node|npc|form';
  $field_group->group_name = 'group_personal';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'npc';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Personal',
    'weight' => '1',
    'children' => array(
      0 => 'body',
      1 => 'field_last_name',
      2 => 'field_titles',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_personal|node|npc|form'] = $field_group;

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
    'weight' => '3',
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
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
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
    'weight' => '4',
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
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_requires|node|items|form'] = $field_group;

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
    'weight' => '6',
    'children' => array(
      0 => 'field_skill',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
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
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
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
    'weight' => '5',
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
        'classes' => '',
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
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_stats|node|character_sheet|form'] = $field_group;

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
    'weight' => '1',
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
      15 => 'field_percents',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Modifies',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_stats|node|items|default'] = $field_group;

  return $export;
}
