<?php
/**
 * @file
 * wiki_features.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function wiki_features_filter_default_formats() {
  $formats = array();

  // Exported format: Wiki.
  $formats['wiki'] = array(
    'format' => 'wiki',
    'name' => 'Wiki',
    'cache' => '0',
    'status' => '1',
    'weight' => '0',
    'filters' => array(
      'filter_html' => array(
        'weight' => '-48',
        'status' => '1',
        'settings' => array(
          'allowed_html' => '<p> <br /> <br> <a> <em> <strong> <cite> <blockquote> <code> <ul> <ol> <li> <dl> <dt> <dd>',
          'filter_html_help' => 1,
          'filter_html_nofollow' => 0,
        ),
      ),
      'bbcode' => array(
        'weight' => '-47',
        'status' => '1',
        'settings' => array(
          'bbcode_make_links' => '1',
          'bbcode_filter_nofollow' => '1',
          'bbcode_encode_mailto' => '1',
          'bbcode_paragraph_breaks' => '2',
          'bbcode_debug' => '0',
        ),
      ),
      'freelinking' => array(
        'weight' => '-45',
        'status' => '1',
        'settings' => array(),
      ),
      'Mediawiki Format' => array(
        'weight' => '-44',
        'status' => '1',
        'settings' => array(),
      ),
      'filter_htmlcorrector' => array(
        'weight' => '-41',
        'status' => '1',
        'settings' => array(),
      ),
    ),
  );

  return $formats;
}
