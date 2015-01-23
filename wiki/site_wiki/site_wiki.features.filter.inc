<?php
/**
 * @file
 * site_wiki.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function site_wiki_filter_default_formats() {
  $formats = array();

  // Exported format: Wiki.
  $formats['wiki'] = array(
    'format' => 'wiki',
    'name' => 'Wiki',
    'cache' => 0,
    'status' => 1,
    'weight' => 0,
    'filters' => array(
      'filter_html' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(
          'allowed_html' => '<p> <br /> <br> <a> <em> <strong> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt> <dd> <b> <i> <h1> <h2> <h3> <hr>',
          'filter_html_help' => 0,
          'filter_html_nofollow' => 0,
        ),
      ),
      'spoiler' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(),
      ),
      'bbcode' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(
          'bbcode_make_links' => 0,
          'bbcode_filter_nofollow' => 1,
          'bbcode_encode_mailto' => 0,
          'bbcode_paragraph_breaks' => 2,
          'bbcode_debug' => 0,
        ),
      ),
      'filter_url' => array(
        'weight' => -46,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
      'freelinking' => array(
        'weight' => -45,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -40,
        'status' => 1,
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
