<?php
/**
 * @file
 * sh_test.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function sh_test_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-vaxia-ash.
  $menus['menu-vaxia-ash'] = array(
    'menu_name' => 'menu-vaxia-ash',
    'title' => 'Vaxia ASH',
    'description' => 'A menu for all the ASH related links.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('A menu for all the ASH related links.');
  t('Vaxia ASH');


  return $menus;
}
