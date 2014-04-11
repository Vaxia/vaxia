<?php
/**
 * @file
 * rpg_site_staff.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function rpg_site_staff_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-vaxia-sh.
  $menus['menu-vaxia-sh'] = array(
    'menu_name' => 'menu-vaxia-sh',
    'title' => 'Vaxia SH',
    'description' => 'A menu for all the SH related links.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('A menu for all the SH related links.');
  t('Vaxia SH');


  return $menus;
}
