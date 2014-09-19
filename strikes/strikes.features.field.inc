<?php
/**
 * @file
 * strikes.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function strikes_field_default_fields() {
  $fields = array();

  // Exported field: 'node-notation-body'.
  $fields['node-notation-body'] = array(
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
      'bundle' => 'notation',
      'default_value' => array(
        0 => array(
          'summary' => '',
          'value' => '',
          'format' => 'wiki',
        ),
      ),
      'deleted' => '0',
      'description' => 'Describe the situation, include players related to the event - and all documentary evidence as an attached file. THIS IS REQUIRED. We want documentation to be transparent so the next time an issue comes up the reviewing SH can see all prior data incase you yourself are no longer available for comment. Please leave a track record for others.',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '6',
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
      'label' => 'Details of Interaction',
      'required' => 1,
      'settings' => array(
        'better_formats' => array(
          'allowed_formats' => array(
            'filtered_html' => 0,
            'full_html' => 0,
            'php_code' => 0,
            'plain_text' => 'plain_text',
            'whiteboard' => 0,
            'wiki' => 0,
          ),
          'allowed_formats_toggle' => 1,
          'default_order_toggle' => 0,
          'default_order_wrapper' => array(
            'formats' => array(
              'filtered_html' => array(
                'weight' => '0',
              ),
              'full_html' => array(
                'weight' => '1',
              ),
              'php_code' => array(
                'weight' => '11',
              ),
              'plain_text' => array(
                'weight' => '10',
              ),
              'whiteboard' => array(
                'weight' => '0',
              ),
              'wiki' => array(
                'weight' => '0',
              ),
            ),
          ),
        ),
        'display_summary' => 0,
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
        'weight' => '8',
      ),
    ),
  );

  // Exported field: 'node-notation-field_associated_strikes'.
  $fields['node-notation-field_associated_strikes'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_associated_strikes',
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
          'field_associated_record' => 'field_associated_record',
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
          'article' => 0,
          'artwork' => 0,
          'character_sheet' => 0,
          'event' => 0,
          'filedepot_folder' => 0,
          'forum' => 0,
          'items' => 0,
          'long_answer' => 0,
          'mass_contact' => 0,
          'multichoice' => 0,
          'page' => 0,
          'proposal' => 0,
          'quiz' => 0,
          'record_of_interaction' => 0,
          'rpg_chatroom' => 0,
          'scene' => 0,
          'session_report' => 0,
          'storyhost_application' => 0,
          'strike' => 'strike',
          'webform' => 0,
          'wiki_page' => 0,
        ),
        'required' => FALSE,
      ),
      'translatable' => '0',
      'type' => 'nodereferrer',
    ),
    'field_instance' => array(
      'bundle' => 'notation',
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
          'weight' => '5',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_associated_strikes',
      'label' => 'Associated strikes',
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
        'weight' => '7',
      ),
    ),
  );

  // Exported field: 'node-notation-field_category'.
  $fields['node-notation-field_category'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_category',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'cheating' => 'Rules',
          'policy-violation' => 'Site Policy',
          'language' => 'Verbal abuse',
          'IC-harassment' => 'IC harassment',
          'OOC-harassment' => 'OOC harassment',
          'sexual' => 'Sexual harassment',
          'disruption' => 'Session disruption',
          'griefing' => 'Player disturbance (griefing)',
          'manipulation' => 'Emotional manipulation (abusive)',
          'noncompliance' => 'Non-compliance with site directives',
          'misc' => 'Misc',
        ),
        'allowed_values_function' => '',
        'allowed_values_php' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'notation',
      'default_value' => NULL,
      'default_value_function' => '',
      'default_value_php' => '',
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
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_category',
      'label' => 'Category',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '5',
      ),
    ),
  );

  // Exported field: 'node-notation-field_date_of_incident'.
  $fields['node-notation-field_date_of_incident'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_date_of_incident',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'date',
      'settings' => array(
        'cache_count' => '4',
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 0,
          'minute' => 0,
          'month' => 'month',
          'second' => 0,
          'year' => 'year',
        ),
        'repeat' => '0',
        'timezone_db' => '',
        'todate' => '',
        'tz_handling' => 'none',
      ),
      'translatable' => '0',
      'type' => 'datetime',
    ),
    'field_instance' => array(
      'bundle' => 'notation',
      'deleted' => '0',
      'description' => 'When did the original incident occur (may be different from when the notation was added to the system).',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'above',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
            'show_repeat_rule' => 'show',
          ),
          'type' => 'date_default',
          'weight' => 8,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_date_of_incident',
      'label' => 'Date of Incident',
      'required' => 0,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'same',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => '15',
          'input_format' => 'm/d/Y - g:i:sa',
          'input_format_custom' => '',
          'label_position' => 'above',
          'repeat_collapsed' => 0,
          'text_parts' => array(),
          'year_range' => '-3:+3',
        ),
        'type' => 'date_popup',
        'weight' => '1',
      ),
    ),
  );

  // Exported field: 'node-notation-field_documentation'.
  $fields['node-notation-field_documentation'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_documentation',
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
      'bundle' => 'notation',
      'deleted' => '0',
      'description' => 'Screenshots, interviews, and other documentary evidence of misconduct. Please include this information as part of the case for the strike so that if a pattern emerges other SHs have all documentation on hand as needed if you are not available at the time. Remember we clear out comments to the archives over time.',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'above',
          'module' => 'file',
          'settings' => array(),
          'type' => 'file_default',
          'weight' => '7',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_documentation',
      'label' => 'Documentation',
      'required' => 0,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => 'record',
        'file_extensions' => 'txt gif jpg bmp png pdf',
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
        'weight' => '9',
      ),
    ),
  );

  // Exported field: 'node-notation-field_players'.
  $fields['node-notation-field_players'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_players',
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid',
          ),
          'table' => 'users',
        ),
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid',
        ),
      ),
      'locked' => '0',
      'module' => 'user_reference',
      'settings' => array(
        'referenceable_roles' => array(
          2 => '2',
          3 => 0,
          8 => 0,
          9 => 0,
          10 => 0,
          12 => 0,
          13 => 0,
          14 => 0,
          15 => 0,
          17 => 0,
          19 => 0,
          20 => 0,
          21 => 0,
          22 => 0,
          23 => 0,
          24 => 0,
          25 => 0,
          26 => 0,
          27 => 0,
          28 => 0,
          29 => 0,
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'user_reference',
    ),
    'field_instance' => array(
      'bundle' => 'notation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Include all players in this interaction as well as the referencing Social member.',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'above',
          'module' => 'user_reference',
          'settings' => array(),
          'type' => 'user_reference_default',
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_players',
      'label' => 'Players Involved',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'user_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'user_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'user_reference_autocomplete',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'node-notation-field_results'.
  $fields['node-notation-field_results'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_results',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'na' => 'No additional actions taken',
          'positive' => 'Positive feedback given to one or more Players',
          'chill' => 'One or more Players requested to disengage in situation',
          'training' => 'Training given to one or more Players',
          'mediation' => 'Mediation between one or more Players',
          'apology' => 'Private apology requested from one or more Players',
          'apology-public' => 'Public apology requested from one or more Players',
          'custody' => 'Alternate RP schedules arranged between one or more Players',
          'break-1d' => 'One or more Players requested to take a break from the site (1 day)',
          'break-3d' => 'One or more Players requested to take a break from the site (3 days)',
          'break-1w' => 'One or more Players requested to take a break from the site (1 week)',
          'yellow' => 'Yellow Strike given to one or more Players',
          'red' => 'Red Strike given to one or more Players',
          'site-ban' => 'Permanent site-ban given to one or more Players',
          'other' => 'Other action taken',
        ),
        'allowed_values_function' => '',
        'allowed_values_php' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'notation',
      'default_value' => NULL,
      'default_value_function' => '',
      'default_value_php' => '',
      'deleted' => '0',
      'description' => 'Check all actions taken as a result of this.',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_results',
      'label' => 'Results',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '6',
      ),
    ),
  );

  // Exported field: 'node-notation-field_social_members'.
  $fields['node-notation-field_social_members'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_social_members',
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid',
          ),
          'table' => 'users',
        ),
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid',
        ),
      ),
      'locked' => '0',
      'module' => 'user_reference',
      'settings' => array(
        'referenceable_roles' => array(
          2 => '2',
          3 => 0,
          8 => 0,
          9 => 0,
          10 => 0,
          12 => 0,
          13 => 0,
          14 => 0,
          15 => 0,
          17 => 0,
          19 => 0,
          20 => 0,
          21 => 0,
          22 => 0,
          23 => 0,
          24 => 0,
          25 => 0,
          26 => 0,
          27 => 0,
          28 => 0,
          29 => 0,
          30 => 0,
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'user_reference',
    ),
    'field_instance' => array(
      'bundle' => 'notation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Members of the social department that got involved.',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'above',
          'module' => 'user_reference',
          'settings' => array(),
          'type' => 'user_reference_default',
          'weight' => '2',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_social_members',
      'label' => 'Social Members',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'user_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'user_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'user_reference_autocomplete',
        'weight' => '4',
      ),
    ),
  );

  // Exported field: 'node-notation-field_witnesses'.
  $fields['node-notation-field_witnesses'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_witnesses',
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid',
          ),
          'table' => 'users',
        ),
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid',
        ),
      ),
      'locked' => '0',
      'module' => 'user_reference',
      'settings' => array(
        'referenceable_roles' => array(
          2 => '2',
          3 => 0,
          8 => 0,
          9 => 0,
          10 => 0,
          12 => 0,
          13 => 0,
          14 => 0,
          15 => 0,
          17 => 0,
          19 => 0,
          20 => 0,
          21 => 0,
          22 => 0,
          23 => 0,
          24 => 0,
          25 => 0,
          26 => 0,
          27 => 0,
          28 => 0,
          29 => 0,
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'user_reference',
    ),
    'field_instance' => array(
      'bundle' => 'notation',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Include any players who were involved witnesses or tried to get the situation to cool off but were not acting as members of the Social department. "Bystanders and witnesses" so to speak.',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'default' => array(
          'label' => 'above',
          'module' => 'user_reference',
          'settings' => array(),
          'type' => 'user_reference_default',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_witnesses',
      'label' => 'Players Affected',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'user_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'user_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'user_reference_autocomplete',
        'weight' => '3',
      ),
    ),
  );

  // Exported field: 'node-strike-field_associated_record'.
  $fields['node-strike-field_associated_record'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_associated_record',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid',
        ),
      ),
      'locked' => '0',
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'article' => 0,
          'artwork' => 0,
          'character_sheet' => 0,
          'event' => 0,
          'filedepot_folder' => 0,
          'forum' => 0,
          'items' => 0,
          'long_answer' => 0,
          'mass_contact' => 0,
          'multichoice' => 0,
          'notation' => 'notation',
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
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'node_reference',
    ),
    'field_instance' => array(
      'bundle' => 'strike',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Required - we must have a record of interaction. That record holds all the documentation for this strike.',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2',
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_associated_record',
      'label' => 'Associated Record',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'node_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'node_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'node_reference_autocomplete',
        'weight' => '1',
      ),
    ),
  );

  // Exported field: 'node-strike-field_players'.
  $fields['node-strike-field_players'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_players',
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid',
          ),
          'table' => 'users',
        ),
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid',
        ),
      ),
      'locked' => '0',
      'module' => 'user_reference',
      'settings' => array(
        'referenceable_roles' => array(
          2 => '2',
          3 => 0,
          8 => 0,
          9 => 0,
          10 => 0,
          12 => 0,
          13 => 0,
          14 => 0,
          15 => 0,
          17 => 0,
          19 => 0,
          20 => 0,
          21 => 0,
          22 => 0,
          23 => 0,
          24 => 0,
          25 => 0,
          26 => 0,
          27 => 0,
          28 => 0,
          29 => 0,
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'user_reference',
    ),
    'field_instance' => array(
      'bundle' => 'strike',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Select the player this strike applies to.',
      'display' => array(
        'artwork' => array(
          'label' => 'hidden',
          'module' => 'user_reference',
          'settings' => array(),
          'type' => 'user_reference_default',
          'weight' => '1',
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'user_reference',
          'settings' => array(),
          'type' => 'user_reference_default',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_players',
      'label' => 'Players',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'user_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'user_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'user_reference_autocomplete',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'node-strike-field_severity'.
  $fields['node-strike-field_severity'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_severity',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'Yellow' => 'Yellow',
          'Red' => 'Red (suspension)',
        ),
        'allowed_values_function' => '',
        'allowed_values_php' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'strike',
      'default_value' => NULL,
      'default_value_function' => '',
      'default_value_php' => '',
      'deleted' => '0',
      'description' => 'A yellow warning, like a yellow card in soccer is a moderate warning but does not immediately require a suspension. A red warning is a ban-able offense and should come with a temporary suspension from the site. Multiple yellow warnings can be upgraded to a red. Multiple red warnings can be upgraded to a permanent ban, as needed for the health of the site.',
      'display' => array(
        'artwork' => array(
          'label' => 'hidden',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '0',
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
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
      'field_name' => 'field_severity',
      'label' => 'Severity',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '5',
      ),
    ),
  );

  // Exported field: 'node-strike-field_social_members'.
  $fields['node-strike-field_social_members'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_social_members',
      'foreign keys' => array(
        'uid' => array(
          'columns' => array(
            'uid' => 'uid',
          ),
          'table' => 'users',
        ),
      ),
      'indexes' => array(
        'uid' => array(
          0 => 'uid',
        ),
      ),
      'locked' => '0',
      'module' => 'user_reference',
      'settings' => array(
        'referenceable_roles' => array(
          2 => '2',
          3 => 0,
          8 => 0,
          9 => 0,
          10 => 0,
          12 => 0,
          13 => 0,
          14 => 0,
          15 => 0,
          17 => 0,
          19 => 0,
          20 => 0,
          21 => 0,
          22 => 0,
          23 => 0,
          24 => 0,
          25 => 0,
          26 => 0,
          27 => 0,
          28 => 0,
          29 => 0,
          30 => 0,
        ),
        'referenceable_status' => array(
          0 => 0,
          1 => 0,
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'user_reference',
    ),
    'field_instance' => array(
      'bundle' => 'strike',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'artwork' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '3',
        ),
        'default' => array(
          'label' => 'inline',
          'module' => 'user_reference',
          'settings' => array(),
          'type' => 'user_reference_default',
          'weight' => '2',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_social_members',
      'label' => 'Social members',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'user_reference',
        'settings' => array(
          'autocomplete_match' => 'contains',
          'autocomplete_path' => 'user_reference/autocomplete',
          'size' => '60',
        ),
        'type' => 'user_reference_autocomplete',
        'weight' => '3',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A yellow warning, like a yellow card in soccer is a moderate warning but does not immediately require a suspension. A red warning is a ban-able offense and should come with a temporary suspension from the site. Multiple yellow warnings can be upgraded to a red. Multiple red warnings can be upgraded to a permanent ban, as needed for the health of the site.');
  t('Associated Record');
  t('Associated strikes');
  t('Category');
  t('Check all actions taken as a result of this.');
  t('Date of Incident');
  t('Describe the situation, include players related to the event - and all documentary evidence as an attached file. THIS IS REQUIRED. We want documentation to be transparent so the next time an issue comes up the reviewing SH can see all prior data incase you yourself are no longer available for comment. Please leave a track record for others.');
  t('Details of Interaction');
  t('Documentation');
  t('Include all players in this interaction as well as the referencing Social member.');
  t('Include any players who were involved witnesses or tried to get the situation to cool off but were not acting as members of the Social department. "Bystanders and witnesses" so to speak.');
  t('Members of the social department that got involved.');
  t('Players');
  t('Players Affected');
  t('Players Involved');
  t('Required - we must have a record of interaction. That record holds all the documentation for this strike.');
  t('Results');
  t('Screenshots, interviews, and other documentary evidence of misconduct. Please include this information as part of the case for the strike so that if a pattern emerges other SHs have all documentation on hand as needed if you are not available at the time. Remember we clear out comments to the archives over time.');
  t('Select the player this strike applies to.');
  t('Severity');
  t('Social Members');
  t('Social members');
  t('When did the original incident occur (may be different from when the notation was added to the system).');

  return $fields;
}
