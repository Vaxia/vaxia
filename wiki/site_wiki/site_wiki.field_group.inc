<?php
/**
 * @file
 * site_wiki.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function site_wiki_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_assoc_files|node|wiki_page|default';
  $field_group->group_name = 'group_assoc_files';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'wiki_page';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Assoc. Files',
    'weight' => '3',
    'children' => array(
      0 => 'field_wiki_files',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-assoc-files field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_assoc_files|node|wiki_page|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = TRUE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_assoc_files|node|wiki_page|form';
  $field_group->group_name = 'group_assoc_files';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'wiki_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Assoc. Files',
    'weight' => '5',
    'children' => array(
      0 => 'field_wiki_images',
      1 => 'field_wiki_files',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-assoc-files field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_assoc_files|node|wiki_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_categories|node|wiki_page|default';
  $field_group->group_name = 'group_categories';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'wiki_page';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Categories',
    'weight' => '4',
    'children' => array(
      0 => 'field_additional_tagging',
      1 => 'field_tag_department',
      2 => 'field_tag_realm',
      3 => 'field_tag_type',
      4 => 'field_tag_location',
      5 => 'field_organization_type',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Categories',
      'instance_settings' => array(
        'classes' => 'group-categories field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_categories|node|wiki_page|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_categories|node|wiki_page|form';
  $field_group->group_name = 'group_categories';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'wiki_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Details',
    'weight' => '1',
    'children' => array(
      0 => 'field_additional_tagging',
      1 => 'field_tag_department',
      2 => 'field_tag_realm',
      3 => 'field_tag_type',
      4 => 'group_extra_details',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-categories field-group-fieldset',
        'description' => 'Details are required for wiki articles - these help us search for articles and sort our information properly. Please fill these out completely!',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $field_groups['group_categories|node|wiki_page|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Assoc. Files');
  t('Categories');
  t('Details');

  return $field_groups;
}
