<?php
/**
 * @file
 * vaxia.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vaxia_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function vaxia_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function vaxia_image_default_styles() {
  $styles = array();

  // Exported image style: tiny-thumbnail.
  $styles['tiny-thumbnail'] = array(
    'name' => 'tiny-thumbnail',
    'label' => 'tiny-thumbnail',
    'effects' => array(
      2 => array(
        'label' => 'Resize',
        'help' => 'Resizing will make images an exact set of dimensions. This may cause images to be stretched or shrunk disproportionately.',
        'effect callback' => 'image_resize_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_resize',
        'data' => array(
          'width' => '40',
          'height' => '40',
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function vaxia_node_info() {
  $items = array(
    'character_sheet' => array(
      'name' => t('Character sheet'),
      'base' => 'node_content',
      'description' => t('Character sheet'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => t('Step 1: Creating a new character, the guidelines can be found here: <a href="http://vaxia.org/wiki/character-creation">http://vaxia.org/wiki/character-creation</a>
<br/><br/>
Step 2: Save, when ready to submit change the workflow to submitted.
<br/><br/>
Things to remember: you get 300 points to allocate between stats and skills pre-play. Those points are flat. 30 points in a stat is 30 points from your pool, 20 points in a skill is 20 from the pool. It is only once in play, when you are spending XP, that the cost of higher values will scale.'),
    ),
  );
  return $items;
}
