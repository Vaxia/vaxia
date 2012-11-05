<?php
/**
 * @file
 * vaxia.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function vaxia_filter_default_formats() {
  $formats = array();

  // Exported format: Plain text with BBCode.
  $formats['plain_text'] = array(
    'format' => 'plain_text',
    'name' => 'Plain text with BBCode',
    'cache' => '0',
    'status' => '1',
    'weight' => '10',
    'filters' => array(
      'bbcode' => array(
        'weight' => '0',
        'status' => '1',
        'settings' => array(
          'bbcode_make_links' => '1',
          'bbcode_filter_nofollow' => '1',
          'bbcode_encode_mailto' => '1',
          'bbcode_paragraph_breaks' => '2',
          'bbcode_debug' => '0',
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => '10',
        'status' => '1',
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
