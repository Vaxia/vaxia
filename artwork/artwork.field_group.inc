<?php
/**
 * @file
 * artwork.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function artwork_field_group_info() {
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
        'classes' => ' group-artwork field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_artwork|node|artwork|form'] = $field_group;

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
        'classes' => ' group-details field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_details|node|artwork|form'] = $field_group;

  return $export;
}
