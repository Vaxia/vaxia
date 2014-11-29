<?php
/**
 * @file
 * wiki_feature.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function wiki_feature_field_default_fields() {
  $fields = array();

  // Exported field: 'node-wiki_page-body'.
  $fields['node-wiki_page-body'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'node',
      ),
      'field_name' => 'body',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_with_summary',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'artwork' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '0',
        ),
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(
            'trim_length' => 600,
          ),
          'type' => 'text_summary_or_trimmed',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Body',
      'required' => 0,
      'settings' => array(
        'better_formats' => array(
          'allowed_formats' => array(
            'full_html' => 0,
            'php_code' => 0,
            'plain_text' => 0,
            'wiki' => 'wiki',
          ),
          'allowed_formats_toggle' => 1,
          'default_order_toggle' => 0,
          'default_order_wrapper' => array(
            'formats' => array(
              'full_html' => array(
                'weight' => '1',
              ),
              'php_code' => array(
                'weight' => '11',
              ),
              'plain_text' => array(
                'weight' => '10',
              ),
              'wiki' => array(
                'weight' => '0',
              ),
            ),
          ),
        ),
        'display_summary' => 1,
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '20',
          'summary_rows' => 5,
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '3',
      ),
    ),
  );

  // Exported field: 'node-wiki_page-field_additional_tagging'.
  $fields['node-wiki_page-field_additional_tagging'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_additional_tagging',
      'field_permissions' => array(
        'type' => '0',
      ),
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
            'vocabulary' => 'wiki_tags',
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
      'description' => 'Add categories to the wiki page to help us locate information about it. Example tags include: Vaxia, Sirian, Setting, Social, and System...',
      'display' => array(
        'artwork' => array(
          'label' => 'inline',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2',
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '11',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_additional_tagging',
      'label' => 'Categories',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'taxonomy',
        'settings' => array(
          'autocomplete_path' => 'taxonomy/autocomplete',
          'size' => 60,
        ),
        'type' => 'taxonomy_autocomplete',
        'weight' => '11',
      ),
    ),
  );

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
      'cardinality' => '-1',
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
      'cardinality' => '-1',
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
      'default_value' => array(
        0 => array(
          'tid' => '723',
        ),
      ),
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

  // Exported field: 'node-wiki_page-field_tag_location'.
  $fields['node-wiki_page-field_tag_location'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_tag_location',
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
            'vocabulary' => 'location_depth',
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
          'weight' => '9',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_tag_location',
      'label' => 'Location Type',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '15',
      ),
    ),
  );

  // Exported field: 'node-wiki_page-field_tag_realm'.
  $fields['node-wiki_page-field_tag_realm'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
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
      'default_value' => array(
        0 => array(
          'tid' => '727',
        ),
      ),
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
      'cardinality' => '-1',
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

  // Exported field: 'node-wiki_page-field_wiki_files'.
  $fields['node-wiki_page-field_wiki_files'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '10',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_wiki_files',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'file',
      'settings' => array(
        'display_default' => 0,
        'display_field' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'file',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'deleted' => '0',
      'description' => 'Upload any files to go with this article.',
      'display' => array(
        'artwork' => array(
          'label' => 'inline',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '4',
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'file',
          'settings' => array(),
          'type' => 'file_table',
          'weight' => '4',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_wiki_files',
      'label' => 'Files',
      'required' => 0,
      'settings' => array(
        'description_field' => 1,
        'file_directory' => 'wiki/files',
        'file_extensions' => 'txt pdf',
        'max_filesize' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'file',
        'settings' => array(
          'progress_indicator' => 'throbber',
        ),
        'type' => 'file_generic',
        'weight' => '6',
      ),
    ),
  );

  // Exported field: 'node-wiki_page-field_wiki_images'.
  $fields['node-wiki_page-field_wiki_images'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '5',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_wiki_images',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'image',
    ),
    'field_instance' => array(
      'bundle' => 'wiki_page',
      'deleted' => '0',
      'description' => 'Upload any images to go with this article.',
      'display' => array(
        'artwork' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => 'file',
            'image_style' => 'medium',
          ),
          'type' => 'image',
          'weight' => '1',
        ),
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => 'file',
            'image_style' => 'medium',
          ),
          'type' => 'image',
          'weight' => '3',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_wiki_images',
      'label' => 'Images',
      'required' => 0,
      'settings' => array(
        'alt_field' => 1,
        'default_image' => 0,
        'file_directory' => 'wiki/images',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 1,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'large',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => '5',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Add categories to the wiki page to help us locate information about it. Example tags include: Vaxia, Sirian, Setting, Social, and System...');
  t('Associated Chat Rooms');
  t('Associated NPCs');
  t('Body');
  t('Categories');
  t('Department');
  t('Files');
  t('Images');
  t('Location Type');
  t('Organization Type');
  t('Realm');
  t('Type');
  t('Upload any files to go with this article.');
  t('Upload any images to go with this article.');

  return $fields;
}
