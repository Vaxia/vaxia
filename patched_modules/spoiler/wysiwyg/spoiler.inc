<?php
/**
 * @file
 * Wysiwyg module's plugin file.
 *
 */

/**
 * Implements hook_wysiwyg_plugin().
 */
function spoiler_spoiler_plugin() {
  $plugins = array();

  $plugins['spoiler'] = array(
    'title' => t('Spoiler'),
    'vendor url' => 'http://drupal.org/project/spoiler',
    'icon file' => 'spoiler.png',
    'icon title' => t('Add selected text between spoiler tags.'),
    'settings' => array(),
  );

  return $plugins;
}
