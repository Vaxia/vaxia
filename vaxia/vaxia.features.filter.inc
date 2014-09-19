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

  // Exported format: Plain text.
  $formats['plain_text'] = array(
    'format' => 'plain_text',
    'name' => 'Plain text',
    'cache' => '0',
    'status' => '1',
    'weight' => '10',
    'filters' => array(
      'filter_html' => array(
        'weight' => '-49',
        'status' => '1',
        'settings' => array(
          'allowed_html' => '<a> <em> <strong> <b> <i> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt> <dd> <p> <br> <br />',
          'filter_html_help' => 0,
          'filter_html_nofollow' => 0,
        ),
      ),
      'spoiler' => array(
        'weight' => '-48',
        'status' => '1',
        'settings' => array(),
      ),
      'bbcode' => array(
        'weight' => '-47',
        'status' => '1',
        'settings' => array(
          'bbcode_make_links' => '0',
          'bbcode_filter_nofollow' => '1',
          'bbcode_encode_mailto' => '0',
          'bbcode_paragraph_breaks' => '2',
          'bbcode_debug' => '0',
        ),
      ),
      'filter_url' => array(
        'weight' => '-46',
        'status' => '1',
        'settings' => array(
          'filter_url_length' => '72',
        ),
      ),
      'filter_htmlcorrector' => array(
        'weight' => '-40',
        'status' => '1',
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
