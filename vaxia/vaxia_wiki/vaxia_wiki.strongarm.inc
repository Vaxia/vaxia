<?php
/**
 * @file
 * vaxia_wiki.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function vaxia_wiki_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_department_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_department_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_location_depth_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_location_depth_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_organization_type_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_organization_type_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_realm_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_realm_pattern'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pathauto_taxonomy_term_setting_type_pattern';
  $strongarm->value = '';
  $export['pathauto_taxonomy_term_setting_type_pattern'] = $strongarm;

  return $export;
}
