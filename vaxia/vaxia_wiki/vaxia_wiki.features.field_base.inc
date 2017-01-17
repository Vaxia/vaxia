<?php
/**
 * @file
 * vaxia_wiki.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function vaxia_wiki_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_associated_npcs'.
  $field_bases['field_associated_npcs'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_associated_npcs',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'nodereferrer',
    'settings' => array(
      'multiple' => 0,
      'referrer_fields' => array(
        'field_artwork' => 0,
        'field_ash_3_application' => 0,
        'field_associated_forum_threads' => 0,
        'field_associated_record' => 0,
        'field_character' => 0,
        'field_character_owner' => 0,
        'field_characters_involved' => 0,
        'field_comment_character' => 0,
        'field_items' => 0,
        'field_npcs' => 0,
        'field_parent' => 0,
        'field_parent_wiki_page' => 0,
        'field_personal_folder' => 0,
        'field_storyhost_character' => 0,
        'field_wiki_source' => 0,
      ),
      'referrer_nodes_per_page' => 0,
      'referrer_order' => 'TITLE_ASC',
      'referrer_pager_element' => 0,
      'referrer_translations' => 0,
      'referrer_types' => array(
        'artwork' => 0,
        'character_sheet' => 'character_sheet',
        'event' => 0,
        'filedepot_folder' => 0,
        'forum' => 0,
        'items' => 0,
        'long_answer' => 0,
        'mass_contact' => 0,
        'multichoice' => 0,
        'notation' => 0,
        'page' => 0,
        'proposal' => 0,
        'quiz' => 0,
        'rpg_chatroom' => 0,
        'scene' => 0,
        'session_report' => 0,
        'storyhost_application' => 0,
        'strike' => 0,
        'webform' => 0,
        'wiki_page' => 0,
      ),
      'required' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'nodereferrer',
  );

  // Exported field_base: 'field_current_pp'.
  $field_bases['field_current_pp'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_current_pp',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'number',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'number_integer',
  );

  // Exported field_base: 'field_first_stat'.
  $field_bases['field_first_stat'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_first_stat',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'members' => 'Membership - Individuals',
        'soft' => 'Soft Power - Diplomacy',
        'hard' => 'Hard Power - Military',
        'econ' => 'Economic Power - Financial',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_org_goal'.
  $field_bases['field_org_goal'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_org_goal',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'stat_up' => 'Stat: Increase stat',
        'stat_down' => 'Stat: Decrease stat',
        'stat_defend' => 'Stat: Defend stat',
        'gov_up' => 'Government: Increase stat',
        'gov_down' => 'Government: Decrease stat',
        'gov_defend' => 'Government: Defend stat',
        'gov_achieve' => 'Government: Achieve a goal',
        'gov_resist' => 'Government: Resist a goal',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_organization'.
  $field_bases['field_organization'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_organization',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'views',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'view' => array(
          'args' => array(),
          'display_name' => 'eref_setting_orgs',
          'view_name' => 'wiki_organizations_list',
        ),
      ),
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_organization_type'.
  $field_bases['field_organization_type'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_organization_type',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'organization_type',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_priority'.
  $field_bases['field_priority'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_priority',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => 1,
        2 => 2,
        3 => 3,
        4 => 4,
        5 => 5,
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_referencing_chat_rooms'.
  $field_bases['field_referencing_chat_rooms'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_referencing_chat_rooms',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'nodereferrer',
    'settings' => array(
      'multiple' => 0,
      'referrer_fields' => array(
        'field_artwork' => 0,
        'field_ash_3_application' => 0,
        'field_associated_forum_threads' => 0,
        'field_associated_record' => 0,
        'field_character' => 0,
        'field_character_owner' => 0,
        'field_characters_involved' => 0,
        'field_comment_character' => 0,
        'field_items' => 0,
        'field_npcs' => 0,
        'field_parent' => 0,
        'field_parent_wiki_page' => 0,
        'field_personal_folder' => 0,
        'field_storyhost_character' => 0,
        'field_wiki_source' => 'field_wiki_source',
      ),
      'referrer_nodes_per_page' => 0,
      'referrer_order' => 'TITLE_ASC',
      'referrer_pager_element' => 0,
      'referrer_translations' => 0,
      'referrer_types' => array(
        'artwork' => 0,
        'character_sheet' => 0,
        'event' => 0,
        'filedepot_folder' => 0,
        'forum' => 0,
        'items' => 0,
        'long_answer' => 0,
        'mass_contact' => 0,
        'multichoice' => 0,
        'notation' => 0,
        'page' => 0,
        'proposal' => 0,
        'quiz' => 0,
        'rpg_chatroom' => 'rpg_chatroom',
        'scene' => 0,
        'session_report' => 0,
        'storyhost_application' => 0,
        'strike' => 0,
        'webform' => 0,
        'wiki_page' => 0,
      ),
      'required' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'nodereferrer',
  );

  // Exported field_base: 'field_second_stat'.
  $field_bases['field_second_stat'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_second_stat',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'members' => 'Membership - Individuals',
        'soft' => 'Soft Power - Diplomacy',
        'hard' => 'Hard Power - Military',
        'econ' => 'Economic Power - Financial',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_tag_department'.
  $field_bases['field_tag_department'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_tag_department',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'department',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_tag_location'.
  $field_bases['field_tag_location'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_tag_location',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'location_depth',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_tag_realm'.
  $field_bases['field_tag_realm'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_tag_realm',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'realm',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_tag_type'.
  $field_bases['field_tag_type'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_tag_type',
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'setting_type',
          'parent' => 0,
        ),
      ),
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  // Exported field_base: 'field_target'.
  $field_bases['field_target'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_target',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'views',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'view' => array(
          'args' => array(),
          'display_name' => 'eref_setting_orgs',
          'view_name' => 'wiki_organizations_list',
        ),
      ),
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_target_government'.
  $field_bases['field_target_government'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_target_government',
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'views',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'view' => array(
          'args' => array(),
          'display_name' => 'eref_setting_gov',
          'view_name' => 'wiki_organizations_list',
        ),
      ),
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_target_stat'.
  $field_bases['field_target_stat'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_target_stat',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'members' => 'Target Stat: Membership - Individuals',
        'soft' => 'Target Stat: Soft Power - Diplomacy',
        'hard' => 'Target Stat: Hard Power - Military',
        'econ' => 'Target Stat: Economic Power - Financial',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  return $field_bases;
}
