<?php

/**
 * @file
 * Boots the Drupal 11 test site in-process (tests/docker-d11), so Site.php can
 * arrange fixtures with the Drupal API.
 */

use Drupal\Core\DrupalKernel;
use Symfony\Component\HttpFoundation\Request;

// Drupal 11 defines DRUPAL_ROOT itself while booting.
$vaxia_d11_root = getenv('DRUPAL_ROOT') ?: '/var/www/vaxia/web';

chdir($vaxia_d11_root);
$autoloader = require $vaxia_d11_root . '/../vendor/autoload.php';
$request = Request::create(VAXIA_TEST_BASE_URL . '/', 'GET', array(), array(), array(), array(
  'SCRIPT_NAME' => '/index.php',
  'SCRIPT_FILENAME' => $vaxia_d11_root . '/index.php',
  'REMOTE_ADDR' => '127.0.0.1',
  'HTTP_USER_AGENT' => 'phpunit',
));
$kernel = DrupalKernel::createFromRequest($request, $autoloader, 'prod');
$kernel->boot();
$kernel->preHandle($request);
\Drupal::requestStack()->push($request);

// As on D7: errors in VAXIA's own code (modules/custom) fail the test that hit
// them; core/contrib notices go to .build/php-errors.log.
restore_error_handler();
restore_exception_handler();
vaxia_tests_error_handler('/modules/custom/');
