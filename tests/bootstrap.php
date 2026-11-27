<?php

/**
 * @file
 * PHPUnit bootstrap: boots the target Drupal site in-process so Site.php can
 * create fixtures with the Drupal API, then tests check behaviour over HTTP.
 *
 * The site is the Drupal 11 test stack (tests/docker-d11); this runs inside
 * its "web" container. VAXIA_TARGET is always "d11" (the suite also ran
 * against D7, which lives on the 7.x-1.x branch).
 */

define('VAXIA_TARGET', 'd11');
define('VAXIA_TESTS_ROOT', __DIR__);
define('VAXIA_TEST_BASE_URL', getenv('VAXIA_TEST_BASE_URL') ?: 'http://localhost');
// The docker stack directory.
define('VAXIA_STACK_DIR', __DIR__ . '/docker-d11');
define('VAXIA_BASELINE_SQL', __DIR__ . '/.build/d11-baseline.sql');

if (!is_file(VAXIA_BASELINE_SQL)) {
  fwrite(STDERR, 'Missing ' . VAXIA_BASELINE_SQL . ": run tests/docker-d11/install.sh first.\n");
  exit(1);
}

// Vaxia\Tests\X loads src/X.php; the target-specific classes (Site,
// TargetCase) load from src/<target>/.
spl_autoload_register(function ($class) {
  $prefix = 'Vaxia\\Tests\\';
  if (strpos($class, $prefix) !== 0) {
    return;
  }
  $name = str_replace('\\', '/', substr($class, strlen($prefix)));
  $dir = in_array($name, array('Site', 'TargetCase'), TRUE) ? __DIR__ . '/src/' . VAXIA_TARGET : __DIR__ . '/src';
  if (is_file($dir . '/' . $name . '.php')) {
    require $dir . '/' . $name . '.php';
  }
});

/**
 * Hands PHP errors to PHPUnit for VAXIA's own code and the tests: a notice
 * raised there fails the test that hit it. Others go to .build/php-errors.log.
 */
function vaxia_tests_error_handler($vaxiaPath) {
  set_error_handler(function ($errno, $errstr, $errfile, $errline) use ($vaxiaPath) {
    if (!(error_reporting() & $errno) || $errno === E_DEPRECATED || $errno === E_USER_DEPRECATED || (defined('E_STRICT') && $errno === @constant('E_STRICT'))) {
      return TRUE;
    }
    if (strpos($errfile, $vaxiaPath) !== FALSE || strpos($errfile, VAXIA_TESTS_ROOT) === 0) {
      throw new \ErrorException($errstr, 0, $errno, $errfile, $errline);
    }
    file_put_contents(VAXIA_TESTS_ROOT . '/.build/php-errors.log', "[in-process] $errstr in $errfile:$errline\n", FILE_APPEND);
    return TRUE;
  });
}

require __DIR__ . '/src/' . VAXIA_TARGET . '/bootstrap.php';
