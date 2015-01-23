<?php
/**
 * @file
 * vaxia_wiki.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function vaxia_wiki_field_group_info() {
  $export = array();

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
    'weight' => '3',
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
  $export['group_categories|node|wiki_page|default'] = $field_group;

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
    'label' => 'Categories',
    'weight' => '1',
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
        'required_fields' => 1,
        'classes' => 'group-categories field-group-fieldset',
        'description' => 'Categories are required for wiki articles - these help us search for articles and sort our information properly.',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_categories|node|wiki_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_navigation|node|wiki_page|default';
  $field_group->group_name = 'group_navigation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'wiki_page';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Navigation',
    'weight' => '0',
    'children' => array(
      0 => 'field_referencing_chat_rooms',
      1 => 'field_associated_npcs',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-navigation field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_navigation|node|wiki_page|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_navigation|node|wiki_page|form';
  $field_group->group_name = 'group_navigation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'wiki_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Navigation',
    'weight' => '2',
    'children' => array(
      0 => 'field_referencing_chat_rooms',
      1 => 'field_associated_npcs',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-navigation field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_navigation|node|wiki_page|form'] = $field_group;

  return $export;
}
