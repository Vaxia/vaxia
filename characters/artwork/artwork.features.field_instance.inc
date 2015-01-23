<?php
/**
 * @file
 * artwork.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function artwork_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-artwork-field_artwork_credit'
  $field_instances['node-artwork-field_artwork_credit'] = array(
    'bundle' => 'artwork',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'artwork' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_artwork_credit',
    'label' => 'Credit',
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
      'weight' => -3,
    ),
  );

  // Exported field_instance: 'node-artwork-field_artwork_image'
  $field_instances['node-artwork-field_artwork_image'] = array(
    'bundle' => 'artwork',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'artwork' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'medium',
        ),
        'type' => 'image',
        'weight' => 0,
      ),
      'default' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'medium',
        ),
        'type' => 'image',
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
    'field_name' => 'field_artwork_image',
    'label' => 'Image',
    'required' => 1,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => 'artwork',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '50MB',
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
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-artwork-field_website'
  $field_instances['node-artwork-field_website'] = array(
    'bundle' => 'artwork',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'artwork' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_website',
    'label' => 'Website',
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
      'weight' => -4,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Credit');
  t('Image');
  t('Website');

  return $field_instances;
}
