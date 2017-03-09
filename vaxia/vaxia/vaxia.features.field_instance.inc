<?php
/**
 * @file
 * vaxia.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function vaxia_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'field_collection_item-field_skill-field_aspect_one'.
  $field_instances['field_collection_item-field_skill-field_aspect_one'] = array(
    'bundle' => 'field_skill',
    'default_value' => array(
      0 => array(
        'tid' => 1141,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_aspect_one',
    'label' => '1.',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-field_skill-field_aspect_three'.
  $field_instances['field_collection_item-field_skill-field_aspect_three'] = array(
    'bundle' => 'field_skill',
    'default_value' => array(
      0 => array(
        'tid' => 1141,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_aspect_three',
    'label' => '3.',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 5,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-field_skill-field_aspect_two'.
  $field_instances['field_collection_item-field_skill-field_aspect_two'] = array(
    'bundle' => 'field_skill',
    'default_value' => array(
      0 => array(
        'tid' => 1141,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_aspect_two',
    'label' => '2.',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 4,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-field_skill-field_skill_desc'.
  $field_instances['field_collection_item-field_skill-field_skill_desc'] = array(
    'bundle' => 'field_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_skill_desc',
    'label' => 'Description',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 'filtered_html',
          'full_html' => 'full_html',
          'php_code' => 'php_code',
          'plain_text' => 'plain_text',
          'whiteboard' => 'whiteboard',
          'wiki' => 'wiki',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 2,
      ),
      'type' => 'text_textarea',
      'weight' => 2,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-field_skill-field_skill_name'.
  $field_instances['field_collection_item-field_skill-field_skill_name'] = array(
    'bundle' => 'field_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_skill_name',
    'label' => 'Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 0,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-field_skill-field_skill_value'.
  $field_instances['field_collection_item-field_skill-field_skill_value'] = array(
    'bundle' => 'field_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_skill_value',
    'label' => 'Value',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-field_skill-field_tech_languages'.
  $field_instances['field_collection_item-field_skill-field_tech_languages'] = array(
    'bundle' => 'field_skill',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'field_collection_table',
        'settings' => array(
          'add' => '',
          'cssfield' => FALSE,
          'cssrowcol' => FALSE,
          'delete' => '',
          'description' => 0,
          'edit' => '',
          'empty' => 1,
          'hide_empty' => FALSE,
          'view_mode' => 'full',
        ),
        'type' => 'field_collection_table_view',
        'weight' => 6,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_tech_languages',
    'label' => 'Tech contexts',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'field_collection',
      'settings' => array(),
      'type' => 'field_collection_embed',
      'weight' => 6,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-field_tech_languages-field_tech_language'.
  $field_instances['field_collection_item-field_tech_languages-field_tech_language'] = array(
    'bundle' => 'field_tech_languages',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_tech_language',
    'label' => 'Tech language',
    'required' => 0,
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

  // Exported field_instance:
  // 'field_collection_item-field_tech_languages-field_tech_proficiency'.
  $field_instances['field_collection_item-field_tech_languages-field_tech_proficiency'] = array(
    'bundle' => 'field_tech_languages',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'field_tech_proficiency',
    'label' => 'Proficiency',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_achivements'.
  $field_instances['node-character_sheet-field_achivements'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'IC achievements! Titles earned, championships won. Fun stuff!',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 9,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_achivements',
    'label' => 'Achivements',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 'filtered_html',
          'full_html' => 'full_html',
          'php_code' => 'php_code',
          'plain_text' => 'plain_text',
          'whiteboard' => 'whiteboard',
          'wiki' => 'wiki',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_age'.
  $field_instances['node-character_sheet-field_age'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'computed_field',
        'settings' => array(),
        'type' => 'computed_field_plain',
        'weight' => 12,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_age',
    'label' => 'Age',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'computed_field',
      'settings' => array(),
      'type' => 'computed',
      'weight' => 29,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_age_app'.
  $field_instances['node-character_sheet-field_age_app'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 11,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_age_app',
    'label' => 'Apparent Age',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 25,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_agility'.
  $field_instances['node-character_sheet-field_agility'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'computed_field',
        'settings' => array(),
        'type' => 'computed_field_plain',
        'weight' => 15,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_agility',
    'label' => 'Agility',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'computed_field',
      'settings' => array(),
      'type' => 'computed',
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_appearance'.
  $field_instances['node-character_sheet-field_appearance'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'computed_field',
        'settings' => array(),
        'type' => 'computed_field_plain',
        'weight' => 23,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_appearance',
    'label' => 'Appearance',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'computed_field',
      'settings' => array(),
      'type' => 'computed',
      'weight' => 28,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_artwork_image'.
  $field_instances['node-character_sheet-field_artwork_image'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'medium',
        ),
        'type' => 'image',
        'weight' => 3,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_artwork_image',
    'label' => 'Image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 5240,
      'file_directory' => 'character_image',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'imagefield_crop',
      'settings' => array(
        'collapsible' => 2,
        'croparea' => '500x500',
        'enforce_minimum' => 0,
        'enforce_ratio' => 0,
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
        'resolution' => '',
      ),
      'type' => 'imagefield_crop_widget',
      'weight' => 37,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_awareness'.
  $field_instances['node-character_sheet-field_awareness'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'computed_field',
        'settings' => array(),
        'type' => 'computed_field_plain',
        'weight' => 19,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_awareness',
    'label' => 'Awareness',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'computed_field',
      'settings' => array(),
      'type' => 'computed',
      'weight' => 24,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_background'.
  $field_instances['node-character_sheet-field_background'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => '',
        'format' => 'plain_text',
      ),
    ),
    'deleted' => 0,
    'description' => 'Rank and money are things earned in sessions and by playing your character. Therefore characters are <b>not</b> allowed to begin play holding high ranking noble titles or excessive wealth. Character concepts that violate this requirement will not be reviewed and will be be automatically returned to draft.</br>
</br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_background',
    'label' => 'Background',
    'required' => 1,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 0,
          'full_html' => 0,
          'php_code' => 0,
          'plain_text' => 'plain_text',
          'wiki' => 0,
        ),
        'allowed_formats_toggle' => 1,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
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
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_background_color'.
  $field_instances['node-character_sheet-field_background_color'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_background_color',
    'label' => 'Default Post Background',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_birth_day'.
  $field_instances['node-character_sheet-field_birth_day'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 10,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_birth_day',
    'label' => 'Day',
    'required' => 1,
    'settings' => array(
      'max' => 31,
      'min' => 1,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 28,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_birth_month'.
  $field_instances['node-character_sheet-field_birth_month'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => '01',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 12,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_birth_month',
    'label' => 'Birth Date',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 27,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_character_goals'.
  $field_instances['node-character_sheet-field_character_goals'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => '',
        'format' => 'plain_text',
      ),
    ),
    'deleted' => 0,
    'description' => 'If your character has any over-arching life goals. Things they want to do or see done that may require an A/SH to help achieve - make note of them here.
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 47,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_character_goals',
    'label' => 'Character Goals',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 0,
          'full_html' => 0,
          'php_code' => 0,
          'plain_text' => 'plain_text',
          'wiki' => 0,
        ),
        'allowed_formats_toggle' => 1,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
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
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 52,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_charisma'.
  $field_instances['node-character_sheet-field_charisma'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 25,
      ),
    ),
    'deleted' => 0,
    'description' => 'Charisma (CHA): Mental Resistance; Influence',
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
        'weight' => 22,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_charisma',
    'label' => 'Charisma',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 27,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_conditions'.
  $field_instances['node-character_sheet-field_conditions'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => 'Most characters should start play with no conditions. Starting play with a \'Condition\' requires approval from the Setting department. (Setting approval is never guaranteed.) Please take a look here for more information about <a href="http://vaxia.org/wiki/setting-approval-conditions" target="_blank">getting Setting approval for Conditions</a> and <a href="http://vaxia.org/wiki/setting-approval-mental-disorders" target="_blank">getting Setting approval for Mental Conditions</a> including amnesia and multiple personality disorders.
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 9,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_conditions',
    'label' => 'Conditions',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 23,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_constitution'.
  $field_instances['node-character_sheet-field_constitution'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'computed_field',
        'settings' => array(),
        'type' => 'computed_field_plain',
        'weight' => 13,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_constitution',
    'label' => 'Constitution',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'computed_field',
      'settings' => array(),
      'type' => 'computed',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_content_limits'.
  $field_instances['node-character_sheet-field_content_limits'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '<br/>
Mark these topics as things your character is ok with if and ONLY if you are ok with the character experiencing them in play.
</br><br/>
This gives you a way to tell an A/SH in advance what sort of things are ok for this character. At the beginning of a session, please review your permission toggles to make sure they are what you want. You will NOT be allowed to change them in the middle of a session. This helps our A/SHs keep play going quickly with the least disruption.
</br><br/>
If this is a special occasion where you trust the A/SH involved and want to give them individual permission to cross these lines, please make sure to explicitly state that in PMs or in the room you will be posting in so we have it on record. If an A/SH crosses one of these lines without your permission, please tell our Social department as soon as you can so our Social department can take action.
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 44,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_content_limits',
    'label' => 'Permissions',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 53,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_currently_wearing'.
  $field_instances['node-character_sheet-field_currently_wearing'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Mark the items your character is usually wearing and they will be automatically selected in the items dropdown for you.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_currently_wearing',
    'label' => 'Currently wearing',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 35,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_description'.
  $field_instances['node-character_sheet-field_description'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => '',
        'format' => 'plain_text',
      ),
    ),
    'deleted' => 0,
    'description' => 'The first description listed will be used on your posts and for viewing by other players. You can add additional descriptions and sort them as needed if your character has more than one \'appearance\'.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_description',
    'label' => 'Descriptions',
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
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 50,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_dexterity'.
  $field_instances['node-character_sheet-field_dexterity'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 25,
      ),
    ),
    'deleted' => 0,
    'description' => 'Dexterity (DEX): Physical Coordination; Deftness',
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
        'weight' => 16,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_dexterity',
    'label' => 'Dexterity',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_economy'.
  $field_instances['node-character_sheet-field_economy'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'You can invest silver instead on the Sheet tab.',
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
        'weight' => 34,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_economy',
    'label' => 'Economy',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 32,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_endurance'.
  $field_instances['node-character_sheet-field_endurance'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 25,
      ),
    ),
    'deleted' => 0,
    'description' => 'Endurance (END): Physical Resistance; Fortitude',
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
        'weight' => 12,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_endurance',
    'label' => 'Endurance',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_health'.
  $field_instances['node-character_sheet-field_health'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'computed_field',
        'settings' => array(),
        'type' => 'computed_field_plain',
        'weight' => 11,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_health',
    'label' => 'Health',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'computed_field',
      'settings' => array(),
      'type' => 'computed',
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_intelligence'.
  $field_instances['node-character_sheet-field_intelligence'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 25,
      ),
    ),
    'deleted' => 0,
    'description' => 'Intellect (INT): Mental Acuity; Reasoning',
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
        'weight' => 18,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_intelligence',
    'label' => 'Intelligence',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 23,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_is_generic_npc'.
  $field_instances['node-character_sheet-field_is_generic_npc'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Generic NPCs are intended for general use in running sessions. If this is a generic NPC, make sure to mark this as an NPC as well.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 5,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_is_generic_npc',
    'label' => 'This is a generic NPC',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_is_npc'.
  $field_instances['node-character_sheet-field_is_npc'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_is_npc',
    'label' => 'This is an NPC',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_is_storyteller'.
  $field_instances['node-character_sheet-field_is_storyteller'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Mark this sheet as a storyteller sheet.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
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
    'field_name' => 'field_is_storyteller',
    'label' => 'Storyhost',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_items'.
  $field_instances['node-character_sheet-field_items'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Items are added to characters from the Item form, not the Character Sheet form. If you need to add an item to this character, go to the item to set the IC owner.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'node_reference',
        'settings' => array(
          'node_reference_view_mode' => 'artwork',
        ),
        'type' => 'node_reference_node',
        'weight' => 35,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_items',
    'label' => 'Items',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'node_reference',
      'settings' => array(
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'node_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'node_reference_autocomplete',
      'weight' => 33,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_last_name'.
  $field_instances['node-character_sheet-field_last_name'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_last_name',
    'label' => 'Last Name',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_life'.
  $field_instances['node-character_sheet-field_life'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 25,
      ),
    ),
    'deleted' => 0,
    'description' => 'Life (LIF): Hitpoints',
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
        'weight' => 10,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_life',
    'label' => 'Life',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_occupation'.
  $field_instances['node-character_sheet-field_occupation'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 6,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_occupation',
    'label' => 'Occupation',
    'required' => 1,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
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
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_other_species'.
  $field_instances['node-character_sheet-field_other_species'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'If you have selected "Other" for your species you MUST select an option here to clarify what sort of \'Other\' species this character is. If this is not an NPC expect to need to get Setting approval. (Setting approval is never guaranteed.) Please take a look here for more information about <a href="/wiki/setting-approval-pcs" target="_blank">getting Setting approval for PCs</a>.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 8,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_other_species',
    'label' => 'Other Species',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 22,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_personal_room'.
  $field_instances['node-character_sheet-field_personal_room'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A personal room is intended for a character to roleplay crafting and personal scenes in. It is automatically set to be NOT displayed on the public search. Posts in these rooms will NOT be archived. Personal rooms are automatically created once the character is approved.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'node_reference',
        'settings' => array(),
        'type' => 'node_reference_default',
        'weight' => 13,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_personal_room',
    'label' => 'Personal room',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'node_reference',
      'settings' => array(
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'node_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'node_reference_autocomplete',
      'weight' => 30,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_personality'.
  $field_instances['node-character_sheet-field_personality'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_personality',
    'label' => 'Personality',
    'required' => 1,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
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
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_player_notes'.
  $field_instances['node-character_sheet-field_player_notes'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => '',
        'format' => 'plain_text',
      ),
    ),
    'deleted' => 0,
    'description' => 'Personal notes for yourself about your character.
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 46,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_player_notes',
    'label' => 'Player Notes',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 0,
          'full_html' => 0,
          'php_code' => 0,
          'plain_text' => 'plain_text',
          'wiki' => 0,
        ),
        'allowed_formats_toggle' => 1,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => -47,
            ),
            'php_code' => array(
              'weight' => -46,
            ),
            'plain_text' => array(
              'weight' => -50,
            ),
            'wiki' => array(
              'weight' => -48,
            ),
          ),
        ),
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 51,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_post_color'.
  $field_instances['node-character_sheet-field_post_color'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Enter as a hex value such as "001100" or text such as "red".
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_post_color',
    'label' => 'Default Post Color #',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 'filtered_html',
          'full_html' => 'full_html',
          'php_code' => 'php_code',
          'plain_text' => 'plain_text',
          'wiki' => 'wiki',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
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
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 6,
      ),
      'type' => 'text_textfield',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_presence'.
  $field_instances['node-character_sheet-field_presence'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'computed_field',
        'settings' => array(),
        'type' => 'computed_field_plain',
        'weight' => 21,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_presence',
    'label' => 'Presence',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'computed_field',
      'settings' => array(),
      'type' => 'computed',
      'weight' => 26,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_property'.
  $field_instances['node-character_sheet-field_property'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => '',
        'format' => 'plain_text',
      ),
    ),
    'deleted' => 0,
    'description' => 'For A/SHs - always feel free to add another property field to give each item a field.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 37,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_property',
    'label' => 'Additional property',
    'required' => 0,
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
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 2,
      ),
      'type' => 'text_textarea',
      'weight' => 34,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_public_information'.
  $field_instances['node-character_sheet-field_public_information'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => '',
        'format' => 'wiki',
      ),
    ),
    'deleted' => 0,
    'description' => 'Include any public information other characters could easily find out about this character. For example: The information a chatty bartender, or a social media search would reveal.
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_public_information',
    'label' => 'Public information',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 0,
          'full_html' => 0,
          'php_code' => 0,
          'plain_text' => 0,
          'wiki' => 'wiki',
        ),
        'allowed_formats_toggle' => 1,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
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
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_race'.
  $field_instances['node-character_sheet-field_race'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 'human',
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => 'If the species you want to play is not listed here, select \'Other\'. (Setting approval is never guaranteed.) Please take a look here for more information about <a href="/wiki/setting-approval-pcs" target="_blank">getting Setting approval for PCs</a>.
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 6,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_race',
    'label' => 'Vaxia Species',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_real_age'.
  $field_instances['node-character_sheet-field_real_age'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 18,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 11,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_real_age',
    'label' => 'Real Age',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => 1,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 26,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_reflexes'.
  $field_instances['node-character_sheet-field_reflexes'] = array(
    'bundle' => 'character_sheet',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'computed_field',
        'settings' => array(),
        'type' => 'computed_field_plain',
        'weight' => 17,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_reflexes',
    'label' => 'Finesse',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'computed_field',
      'settings' => array(),
      'type' => 'computed',
      'weight' => 22,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_sex'.
  $field_instances['node-character_sheet-field_sex'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 10,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sex',
    'label' => 'Apparent Gender',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 'filtered_html',
          'full_html' => 'full_html',
          'php_code' => 'php_code',
          'plain_text' => 'plain_text',
          'whiteboard' => 'whiteboard',
          'wiki' => 'wiki',
        ),
        'allowed_formats_toggle' => 0,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 24,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_sh_notes'.
  $field_instances['node-character_sheet-field_sh_notes'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 19,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sh_notes',
    'label' => 'SH Notes',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
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
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 40,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_sh_private_notes'.
  $field_instances['node-character_sheet-field_sh_private_notes'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 20,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sh_private_notes',
    'label' => 'Private SH Notes',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
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
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 41,
    ),
  );

  // Exported field_instance:
  // 'node-character_sheet-field_sh_public_information'.
  $field_instances['node-character_sheet-field_sh_public_information'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => '',
      ),
    ),
    'deleted' => 0,
    'description' => 'SH provided public information about your character.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sh_public_information',
    'label' => 'SH Public information',
    'required' => 0,
    'settings' => array(
      'better_formats' => array(
        'allowed_formats' => array(
          'filtered_html' => 0,
          'full_html' => 0,
          'php_code' => 0,
          'plain_text' => 0,
          'whiteboard' => 0,
          'wiki' => 'wiki',
        ),
        'allowed_formats_toggle' => 1,
        'default_order_toggle' => 0,
        'default_order_wrapper' => array(
          'formats' => array(
            'filtered_html' => array(
              'weight' => 0,
            ),
            'full_html' => array(
              'weight' => 1,
            ),
            'php_code' => array(
              'weight' => 11,
            ),
            'plain_text' => array(
              'weight' => 10,
            ),
            'whiteboard' => array(
              'weight' => 0,
            ),
            'wiki' => array(
              'weight' => 0,
            ),
          ),
        ),
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_sirian_languages'.
  $field_instances['node-character_sheet-field_sirian_languages'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'All starting characters may select up to three languages at "native" proficiency: <a href="/wiki/sirian-languages" target="_blank">Languages of Sirian</a>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 16,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sirian_languages',
    'label' => 'Languages',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_sirian_species'.
  $field_instances['node-character_sheet-field_sirian_species'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 'human',
      ),
    ),
    'deleted' => 0,
    'description' => 'If the species you want to play is not listed here, select \'Other\'. (Setting approval is never guaranteed.) Please take a look here for more information about <a href="/wiki/setting-approval-pcs" target="_blank">getting Setting approval for PCs</a>.
<br><br>',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 7,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sirian_species',
    'label' => 'Sirian Species',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_skill'.
  $field_instances['node-character_sheet-field_skill'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '<br>
Skill aspects determine what your skill can do in our system. Information on aspects can be found <a href="/forums/skill-verbs/aspects" target="_blank">here</a>.
If and ONLY if this is a Sirian tech skill: select two technical contexts to be familiar with. Skills start with two "proficient" contexts by default.
<br>',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'field_collection_table',
        'settings' => array(
          'add' => '',
          'cssfield' => FALSE,
          'cssrowcol' => FALSE,
          'delete' => '',
          'description' => 0,
          'edit' => '',
          'empty' => 0,
          'hide_empty' => FALSE,
          'view_mode' => 'full',
        ),
        'type' => 'field_collection_table_view',
        'weight' => 13,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_skill',
    'label' => 'Skill',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'field_collection',
      'settings' => array(),
      'type' => 'field_collection_embed',
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_spirituality'.
  $field_instances['node-character_sheet-field_spirituality'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 25,
      ),
    ),
    'deleted' => 0,
    'description' => 'Spirit (SPIR): Mental Power; Wisdom',
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
        'weight' => 20,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_spirituality',
    'label' => 'Spirit',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => '',
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 25,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_strength'.
  $field_instances['node-character_sheet-field_strength'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 25,
      ),
    ),
    'deleted' => 0,
    'description' => 'Strength (STR): Physical Power; Toughness',
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
        'weight' => 14,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_strength',
    'label' => 'Strength',
    'required' => 1,
    'settings' => array(
      'max' => '',
      'min' => 0,
      'prefix' => '',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 19,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_tag_realm'.
  $field_instances['node-character_sheet-field_tag_realm'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'tid' => 725,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_plain',
        'weight' => 3,
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
      'weight' => 19,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_titles'.
  $field_instances['node-character_sheet-field_titles'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_titles',
    'label' => 'Titles',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_vaxia_languages'.
  $field_instances['node-character_sheet-field_vaxia_languages'] = array(
    'bundle' => 'character_sheet',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'All starting characters may select up to three languages at "native" proficiency: <a href="/wiki/vaxia-languages" target="_blank">Languages of Vaxia</a>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 17,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_vaxia_languages',
    'label' => 'Languages',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-character_sheet-field_visibility'.
  $field_instances['node-character_sheet-field_visibility'] = array(
    'bundle' => 'character_sheet',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
    'deleted' => 0,
    'description' => 'Uncheck this box to hide this character\'s posts from the power search. This is useful if you are playing a character whose actions you want to keep private. Be aware, while this will hide the posts of this character from the search it will not hide the posts of other character\'s in a scene with you if they are not also set for private mode.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 45,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_visibility',
    'label' => 'Visibility',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 0,
      ),
      'type' => 'options_onoff',
      'weight' => 54,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('1.');
  t('2.');
  t('3.');
  t('<br/>
Mark these topics as things your character is ok with if and ONLY if you are ok with the character experiencing them in play.
</br><br/>
This gives you a way to tell an A/SH in advance what sort of things are ok for this character. At the beginning of a session, please review your permission toggles to make sure they are what you want. You will NOT be allowed to change them in the middle of a session. This helps our A/SHs keep play going quickly with the least disruption.
</br><br/>
If this is a special occasion where you trust the A/SH involved and want to give them individual permission to cross these lines, please make sure to explicitly state that in PMs or in the room you will be posting in so we have it on record. If an A/SH crosses one of these lines without your permission, please tell our Social department as soon as you can so our Social department can take action.
<br><br>');
  t('<br>
Skill aspects determine what your skill can do in our system. Information on aspects can be found <a href="/forums/skill-verbs/aspects" target="_blank">here</a>.
If and ONLY if this is a Sirian tech skill: select two technical contexts to be familiar with. Skills start with two "proficient" contexts by default.
<br>');
  t('A personal room is intended for a character to roleplay crafting and personal scenes in. It is automatically set to be NOT displayed on the public search. Posts in these rooms will NOT be archived. Personal rooms are automatically created once the character is approved.');
  t('Achivements');
  t('Additional property');
  t('Age');
  t('Agility');
  t('All starting characters may select up to three languages at "native" proficiency: <a href="/wiki/sirian-languages" target="_blank">Languages of Sirian</a>');
  t('All starting characters may select up to three languages at "native" proficiency: <a href="/wiki/vaxia-languages" target="_blank">Languages of Vaxia</a>');
  t('Apparent Age');
  t('Apparent Gender');
  t('Appearance');
  t('Awareness');
  t('Background');
  t('Birth Date');
  t('Character Goals');
  t('Charisma');
  t('Charisma (CHA): Mental Resistance; Influence');
  t('Conditions');
  t('Constitution');
  t('Currently wearing');
  t('Day');
  t('Default Post Background');
  t('Default Post Color #');
  t('Description');
  t('Descriptions');
  t('Dexterity');
  t('Dexterity (DEX): Physical Coordination; Deftness');
  t('Economy');
  t('Endurance');
  t('Endurance (END): Physical Resistance; Fortitude');
  t('Enter as a hex value such as "001100" or text such as "red".
<br><br>');
  t('Finesse');
  t('For A/SHs - always feel free to add another property field to give each item a field.');
  t('Generic NPCs are intended for general use in running sessions. If this is a generic NPC, make sure to mark this as an NPC as well.');
  t('Health');
  t('IC achievements! Titles earned, championships won. Fun stuff!');
  t('If the species you want to play is not listed here, select \'Other\'. (Setting approval is never guaranteed.) Please take a look here for more information about <a href="/wiki/setting-approval-pcs" target="_blank">getting Setting approval for PCs</a>.
<br><br>');
  t('If you have selected "Other" for your species you MUST select an option here to clarify what sort of \'Other\' species this character is. If this is not an NPC expect to need to get Setting approval. (Setting approval is never guaranteed.) Please take a look here for more information about <a href="/wiki/setting-approval-pcs" target="_blank">getting Setting approval for PCs</a>.');
  t('If your character has any over-arching life goals. Things they want to do or see done that may require an A/SH to help achieve - make note of them here.
<br><br>');
  t('Image');
  t('Include any public information other characters could easily find out about this character. For example: The information a chatty bartender, or a social media search would reveal.
<br><br>');
  t('Intellect (INT): Mental Acuity; Reasoning');
  t('Intelligence');
  t('Items');
  t('Items are added to characters from the Item form, not the Character Sheet form. If you need to add an item to this character, go to the item to set the IC owner.');
  t('Languages');
  t('Last Name');
  t('Life');
  t('Life (LIF): Hitpoints');
  t('Mark the items your character is usually wearing and they will be automatically selected in the items dropdown for you.');
  t('Mark this sheet as a storyteller sheet.');
  t('Most characters should start play with no conditions. Starting play with a \'Condition\' requires approval from the Setting department. (Setting approval is never guaranteed.) Please take a look here for more information about <a href="http://vaxia.org/wiki/setting-approval-conditions" target="_blank">getting Setting approval for Conditions</a> and <a href="http://vaxia.org/wiki/setting-approval-mental-disorders" target="_blank">getting Setting approval for Mental Conditions</a> including amnesia and multiple personality disorders.
<br><br>');
  t('Name');
  t('Occupation');
  t('Other Species');
  t('Permissions');
  t('Personal notes for yourself about your character.
<br><br>');
  t('Personal room');
  t('Personality');
  t('Player Notes');
  t('Presence');
  t('Private SH Notes');
  t('Proficiency');
  t('Public information');
  t('Rank and money are things earned in sessions and by playing your character. Therefore characters are <b>not</b> allowed to begin play holding high ranking noble titles or excessive wealth. Character concepts that violate this requirement will not be reviewed and will be be automatically returned to draft.</br>
</br>');
  t('Real Age');
  t('Realm');
  t('SH Notes');
  t('SH Public information');
  t('SH provided public information about your character.');
  t('Sirian Species');
  t('Skill');
  t('Spirit');
  t('Spirit (SPIR): Mental Power; Wisdom');
  t('Storyhost');
  t('Strength');
  t('Strength (STR): Physical Power; Toughness');
  t('Tech contexts');
  t('Tech language');
  t('The first description listed will be used on your posts and for viewing by other players. You can add additional descriptions and sort them as needed if your character has more than one \'appearance\'.');
  t('This is a generic NPC');
  t('This is an NPC');
  t('Titles');
  t('Uncheck this box to hide this character\'s posts from the power search. This is useful if you are playing a character whose actions you want to keep private. Be aware, while this will hide the posts of this character from the search it will not hide the posts of other character\'s in a scene with you if they are not also set for private mode.');
  t('Value');
  t('Vaxia Species');
  t('Visibility');
  t('You can invest silver instead on the Sheet tab.');

  return $field_instances;
}
