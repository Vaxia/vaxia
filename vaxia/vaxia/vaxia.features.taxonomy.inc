<?php
/**
 * @file
 * vaxia.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function vaxia_taxonomy_default_vocabularies() {
  return array(
    'character_limits' => array(
      'name' => 'Character limits',
      'machine_name' => 'character_limits',
      'description' => 'Character limits',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'skill_verbs' => array(
      'name' => 'Skill Verbs',
      'machine_name' => 'skill_verbs',
      'description' => 'For character sheets, the skill verbs that go with a sheet.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
    'technology_languages' => array(
      'name' => 'Technology Languages',
      'machine_name' => 'technology_languages',
      'description' => 'Technology Languages',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
