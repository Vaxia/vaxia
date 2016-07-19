<?php
/**
 * @file
 * vaxia.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function vaxia_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-vaxia.
  $menus['menu-vaxia'] = array(
    'menu_name' => 'menu-vaxia',
    'title' => 'Vaxia',
    'description' => 'Vaxia basic navigation',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Vaxia');
  t('Vaxia basic navigation');

  return $menus;
}
