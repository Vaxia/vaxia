<?php
/**
 * @file
 * vaxia_wiki.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function vaxia_wiki_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_extra_details|node|wiki_page|form';
  $field_group->group_name = 'group_extra_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'wiki_page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_categories';
  $field_group->data = array(
    'label' => 'Extra Details',
    'weight' => '15',
    'children' => array(
      0 => 'field_tag_location',
      1 => 'field_organization_type',
      2 => 'field_provides_reputation',
      3 => 'field_has_goals',
      4 => 'field_org_membership_size',
      5 => 'field_org_diplomatic_influence',
      6 => 'field_org_material_strength',
      7 => 'field_org_economic_influence',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-extra-details field-group-fieldset',
        'required_fields' => 1,
      ),
    ),
  );
  $field_groups['group_extra_details|node|wiki_page|form'] = $field_group;

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
  $field_groups['group_navigation|node|wiki_page|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = TRUE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_navigation|node|wiki_page|form';
  $field_group->group_name = 'group_navigation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'wiki_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Navigation',
    'weight' => '3',
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
  $field_groups['group_navigation|node|wiki_page|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Extra Details');
  t('Navigation');

  return $field_groups;
}
