<?php
/**
 * @file
 * site_events.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function site_events_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-event-body'.
  $field_instances['node-event-body'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'ical' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 400,
        ),
        'type' => 'text_trimmed',
        'weight' => 0,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
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
          'plain_text' => 'plain_text',
          'wiki' => 0,
        ),
        'allowed_formats_toggle' => 1,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'display_summary' => 1,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-event-field_content_warnings'.
  $field_instances['node-event-field_content_warnings'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'tid' => 880,
      ),
    ),
    'deleted' => 0,
    'description' => 'If you can, please make note of any content warnings for this event. Not all of our players are comfortable with all topics so please let attendees know in advance what to expect. That way they can decide if this is a session that they are interested in attending. Note - "graphic" means you plan to go into detail in your descriptions. Consider this the difference between the violence in an summer blockbuster action movie as opposed to the violence in a slasher-horror movie.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 6,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_content_warnings',
    'label' => 'Content',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-event-field_format'.
  $field_instances['node-event-field_format'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'value' => 'fast',
      ),
    ),
    'deleted' => 0,
    'description' => 'What rate of play to expect for this session. Fast posts are a scheduled time where players and SH are posting constantly. Slow posts are sessions where SHs and players may post once or twice a day allowing players who aren\'t on the same timezone to play.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 5,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_format',
    'label' => 'Format',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-event-field_max_seating'.
  $field_instances['node-event-field_max_seating'] = array(
    'bundle' => 'event',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter the max number of seats you can handle for this event. If you don\'t have a cap for number of seats, just leave blank.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 4,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_max_seating',
    'label' => 'Max seating',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => 2,
      'prefix' => '',
      'suffix' => ' seats',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-event-field_tag_realm'.
  $field_instances['node-event-field_tag_realm'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'tid' => 725,
      ),
    ),
    'deleted' => 0,
    'description' => 'Select Vaxia or Sirian for session announcements. For events that aren\'t tied to sessions (elections, meetings, or the like) then select Storyhost.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 3,
      ),
      'ical' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
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
      'type' => 'options_select',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-event-field_when'.
  $field_instances['node-event-field_when'] = array(
    'bundle' => 'event',
    'deleted' => 0,
    'description' => 'This field can handle your timezone shift for you. Make sure you configure your preferred time-zone under <a href="/user">your account information</a>. After you set that, dates and times throughout this site will be displayed using your time zone. This allows you to enter times in your preferred time zone. They will display for other site members in their preferred time zone and on the calendar in default EST.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'long',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_remaining_days' => FALSE,
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 2,
      ),
      'ical' => array(
        'label' => 'inline',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_when',
    'label' => 'When',
    'required' => 1,
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
        'increment' => 15,
        'input_format' => 'm/d/Y - g:i:sa',
        'input_format_custom' => '',
        'label_position' => 'above',
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-0:+3',
      ),
      'type' => 'date_popup',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-event-taxonomy_forums'.
  $field_instances['node-event-taxonomy_forums'] = array(
    'bundle' => 'event',
    'default_value' => array(
      0 => array(
        'tid' => 879,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 0,
      ),
      'ical' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'taxonomy_forums',
    'label' => 'Forums',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('Content');
  t('Enter the max number of seats you can handle for this event. If you don\'t have a cap for number of seats, just leave blank.');
  t('Format');
  t('Forums');
  t('If you can, please make note of any content warnings for this event. Not all of our players are comfortable with all topics so please let attendees know in advance what to expect. That way they can decide if this is a session that they are interested in attending. Note - "graphic" means you plan to go into detail in your descriptions. Consider this the difference between the violence in an summer blockbuster action movie as opposed to the violence in a slasher-horror movie.');
  t('Max seating');
  t('Realm');
  t('Select Vaxia or Sirian for session announcements. For events that aren\'t tied to sessions (elections, meetings, or the like) then select Storyhost.');
  t('This field can handle your timezone shift for you. Make sure you configure your preferred time-zone under <a href="/user">your account information</a>. After you set that, dates and times throughout this site will be displayed using your time zone. This allows you to enter times in your preferred time zone. They will display for other site members in their preferred time zone and on the calendar in default EST.');
  t('What rate of play to expect for this session. Fast posts are a scheduled time where players and SH are posting constantly. Slow posts are sessions where SHs and players may post once or twice a day allowing players who aren\'t on the same timezone to play.');
  t('When');

  return $field_instances;
}
