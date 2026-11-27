
// --- VAXIA test stack (appended by tests/docker-d11/install.sh) ---
$databases['default']['default'] = [
  'database' => 'vaxia', 'username' => 'vaxia', 'password' => 'vaxia',
  'host' => 'db', 'port' => '3306', 'driver' => 'mysql', 'prefix' => '',
  'namespace' => 'Drupal\\mysql\\Driver\\Database\\mysql', 'autoload' => 'core/modules/mysql/src/Driver/Database/mysql/',
  'init_commands' => ['isolation_level' => 'SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED'],
];
$settings['hash_salt'] = 'vaxia-test-only-salt';
$settings['config_sync_directory'] = '../config/sync';
$settings['trusted_host_patterns'] = ['^localhost$', '^127\.0\.0\.1$', '^web$'];
$settings['file_private_path'] = '/var/www/vaxia/private';
// Show PHP errors on pages, like the D7 test site, so tests see them.
$config['system.logging']['error_level'] = 'verbose';
// Test site: no CSS/JS aggregation, so pages name the files they load.
$config['system.performance']['css']['preprocess'] = FALSE;
$config['system.performance']['js']['preprocess'] = FALSE;
// Config work (restore-baseline.sh / export-config.sh with VAXIA_SITE=config):
// a copy of the baseline in vaxia_config, so test runs (which reload the
// baseline into vaxia per test class) can't undo changes mid-export.
if (getenv('VAXIA_SITE') === 'config') {
  $databases['default']['default']['database'] = 'vaxia_config';
}
// The starter site (docker compose service "demo", load-demo.sh): the
// fixtures in their own database, mail collected, not sent.
if (getenv('VAXIA_SITE') === 'demo') {
  $databases['default']['default']['database'] = 'vaxia_demo';
  $settings['file_public_path'] = 'sites/demo/files';
  $settings['file_private_path'] = '/var/www/vaxia/private-demo';
  $config['system.mail']['interface']['default'] = 'test_mail_collector';
}
