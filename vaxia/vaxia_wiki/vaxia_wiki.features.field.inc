<?php
/**
 * @file
 * vaxia_wiki.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function vaxia_wiki_field_default_fields() {
  $fields = array();

  // Exported field: 'node-wiki_page-field_associated_npcs'.
  $fields['node-wiki_page-field_associated_npcs'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_associated_npcs',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'nodereferrer',
      'settings' => array(
        'multiple' => 0,
        'referrer_fields' => array(
          'field_artwork' => 0,
          'field_ash_3_application' => 0,
          'field_associated_forum_threads' => 0,
          'field_associated_locations' => 'field_associated_locations',
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
        'referrer_nodes_per_page' => '0',
        'referrer_order' => 'TITLE_ASC',
        'referrer_pager_element' => '0',
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
      'translatable' => '0',
      'type' => 'nodereferrer',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'above',
          'module' => 'nodereferrer',
          'settings' => array(),
          'type' => 'nodereferrer_default',
          'weight' => '3',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_associated_npcs',
      'label' => 'Associated NPCs',
      'required' => 0,
      'settings' => array(
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'nodereferrer',
        'settings' => array(),
        'type' => 'nodereferrer_list',
        'weight' => '5',
      ),
    ),
  );

  // Exported field: 'node-wiki_page-field_organization_type'.
  $fields['node-wiki_page-field_organization_type'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_organization_type',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'organization_type',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '10',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_organization_type',
      'label' => 'Organization Type',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '16',
      ),
    ),
  );

  // Exported field: 'node-wiki_page-field_referencing_chat_rooms'.
  $fields['node-wiki_page-field_referencing_chat_rooms'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_referencing_chat_rooms',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'nodereferrer',
      'settings' => array(
        'multiple' => 0,
        'referrer_fields' => array(
          'field_artwork' => 0,
          'field_ash_3_application' => 0,
          'field_associated_forum_threads' => 0,
          'field_associated_locations' => 0,
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
        'referrer_nodes_per_page' => '0',
        'referrer_order' => 'TITLE_ASC',
        'referrer_pager_element' => '0',
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
      'translatable' => '0',
      'type' => 'nodereferrer',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'above',
          'module' => 'nodereferrer',
          'settings' => array(),
          'type' => 'nodereferrer_default',
          'weight' => '2',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_referencing_chat_rooms',
      'label' => 'Associated Chat Rooms',
      'required' => 0,
      'settings' => array(
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'nodereferrer',
        'settings' => array(),
        'type' => 'nodereferrer_list',
        'weight' => '4',
      ),
    ),
  );

  // Exported field: 'node-wiki_page-field_tag_department'.
  $fields['node-wiki_page-field_tag_department'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_tag_department',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'department',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '6',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_tag_department',
      'label' => 'Department',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '12',
      ),
    ),
  );

  // Exported field: 'node-wiki_page-field_tag_realm'.
  $fields['node-wiki_page-field_tag_realm'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_tag_realm',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'realm',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '7',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_tag_realm',
      'label' => 'Realm',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '13',
      ),
    ),
  );

  // Exported field: 'node-wiki_page-field_tag_type'.
  $fields['node-wiki_page-field_tag_type'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_tag_type',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'setting_type',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '8',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_tag_type',
      'label' => 'Type',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '14',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Associated Chat Rooms');
  t('Associated NPCs');
  t('Department');
  t('Organization Type');
  t('Realm');
  t('Type');

  return $fields;
}
