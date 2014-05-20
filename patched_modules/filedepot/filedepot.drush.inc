<?php

/**
 * @file
 * Drush integration for filedepot.
 */

/**
 * The filedepot 3rd Party Javascript downloads URI.
 */
define('filedepot_DOWNLOAD_URI', 'http://www.nextide.ca/downloads/filedepot/filedepot_libraries-1.0.zip');

/**
 * Implements hook_drush_command().
 */
function filedepot_drush_command() {
  $items = array();

  // The key in the $items array is the name of the command.
  $items['filedepot-libraries'] = array(
    'description' => dt("Downloads the filedepot 3rd party javascript libraries."),
    'arguments' => array(
      'path' => dt('Optional. A path where to install the filedepot libraries. If omitted Drush will use the default location.'),
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function filedepot_drush_help($section) {
  switch ($section) {
    case 'drush:filedepot-libraries':
      return dt("Downloads the filedepot 3rd party javascript libraries from www.nextide.ca, default location is sites/all/libraries.");
  }
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_filedepot_post_pm_enable() {
  $extensions = func_get_args();
  // Deal with comma delimited extension list.
  if (strpos($extensions[0], ',') !== FALSE) {
    $extensions = explode(',', $extensions[0]);
  }

  if (in_array('filedepot', $extensions) && !drush_get_option('skip')) {
    drush_filedepot_libraries();
  }
}

/**
 * Commands to download the filedepot 3rd Party Javascript libraries.
 */
function drush_filedepot_libraries($path = 'sites/all/libraries') {
  if (!drush_shell_exec('unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }

  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }

  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);

  $dir1 = 'html_encoder';
  $dir2 = 'jquery.blockui';

  // Remove any existing filedepot library directories.
  if (is_dir($dir1)) {
    drush_log(dt('A existing filedepot 3rd party javascript libraries were overwritten at @path', array('@path' => "$path/$dir1")), 'notice');
  }
  if (is_dir($dir2)) {
    drush_log(dt('A existing filedepot 3rd party javascript libraries were overwritten at @path', array('@path' => "$path/$dir2")), 'notice');
  }

  // Remove any existing filedepot 3rd party javascript libraries archive.
  $filename = basename(filedepot_DOWNLOAD_URI);
  if (is_file($filename)) {
    drush_op('unlink', $filename);
  }

  // Download the zip archive.
  if (!drush_shell_exec('wget ' . filedepot_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . filedepot_DOWNLOAD_URI);
  }

  if (is_file($filename)) {
    // Decompress the zip archive.
    drush_shell_exec('unzip -qq -o ' . $filename);
    // Remove the zip archive.
    drush_op('unlink', $filename);
  }

  // Set working directory back to the previous working directory.
  chdir($olddir);

  if (is_dir($path . '/jquery.blockui') AND is_dir($path . '/html_encoder')) {
    drush_log(dt('filedepot 3rd party javascript libraries has been downloaded to @path.', array('@path' => $path)), 'success');
  }
  else {
    drush_log(dt('Drush was unable to download the 3rd party javascript libraries to @path', array('@path' => $path)), 'error');
  }
}
