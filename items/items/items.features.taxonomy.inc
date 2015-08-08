<?php
/**
 * @file
 * items.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function items_taxonomy_default_vocabularies() {
  return array(
    'item_type' => array(
      'name' => 'Item type',
      'machine_name' => 'item_type',
      'description' => 'Item type',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 2,
    ),
    'weapon_category' => array(
      'name' => 'Weapon Category',
      'machine_name' => 'weapon_category',
      'description' => 'Weapon Category',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 1,
    ),
    'weapon_sub_category' => array(
      'name' => 'Weapon Sub-category',
      'machine_name' => 'weapon_sub_category',
      'description' => '',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
    ),
  );
}
