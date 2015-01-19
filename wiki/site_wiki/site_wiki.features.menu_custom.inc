<?php
/**
 * @file
 * site_wiki.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function site_wiki_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-wiki-links.
  $menus['menu-wiki-links'] = array(
    'menu_name' => 'menu-wiki-links',
    'title' => 'Wiki Links',
    'description' => 'Special displays for the wiki are linked here.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Special displays for the wiki are linked here.');
  t('Wiki Links');


  return $menus;
}
