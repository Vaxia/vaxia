#!/bin/bash
# Installs the D11 test site from config/sync and snapshots it to
# tests/.build/d11-baseline.sql (restored before each test class).
# First time: docker compose up -d --build ; (the composer install runs here).
set -euo pipefail
cd "$(dirname "$0")"
x() { docker compose exec -T web bash -c "$1"; }
# Installs into vaxia_config (VAXIA_SITE=config), not the test DB: test runs
# reload the baseline into vaxia per test class and would break an install
# in progress. The snapshot then serves the test DB as before.
y() { docker compose exec -T -e VAXIA_SITE=config web bash -c "$1"; }
until docker compose exec -T web bash -c 'php -r "new PDO(\"mysql:host=db\", \"vaxia\", \"vaxia\");"' 2>/dev/null; do sleep 2; done
x 'cd /var/www/vaxia && composer install --no-interaction --no-progress -q'
x 'cd /var/www/vaxia/web/sites/default && chmod u+w . settings.php 2>/dev/null; cp default.settings.php settings.php && cat /var/www/tests/docker-d11/settings.append.php >> settings.php && mkdir -p files /var/www/vaxia/private && chown -R www-data files /var/www/vaxia/private'
docker compose exec -T db mariadb -uroot -proot -e "CREATE DATABASE IF NOT EXISTS vaxia_config; GRANT ALL ON vaxia_config.* TO 'vaxia'@'%'"
if ls ../../config/sync/*.yml >/dev/null 2>&1; then
  y 'drush sql:drop -y -q'
y 'drush si --existing-config -y --account-name=admin --account-pass=admin --account-mail=admin@example.test -q'
else
  y 'drush si minimal -y --account-name=admin --account-pass=admin --account-mail=admin@example.test -q'
fi
# As on the D7 test site, the superuser also holds the administrator role
# (production mails "administrator" users, e.g. item evaluators).
y 'drush user:role:add -q administrator admin'
# The forum module's install creates a first forum on D7; the D11 site has no
# forum module yet, so make the same term (scene and session forms need one).
y 'drush php:eval "\Drupal\taxonomy\Entity\Term::create([\"vid\" => \"forums\", \"name\" => \"General discussion\"])->save();"'
# Test-only state (not config): collect mail instead of sending it.
y 'drush php:eval "\Drupal::state()->set(\"system.test_mail_collector\", []); " && drush cset -y system.mail interface.default test_mail_collector -q'
y 'chown -R www-data /var/www/vaxia/web/sites/default/files /var/www/vaxia/private && drush cr -q'
# Config work restores this one (restore-baseline.sh with VAXIA_SITE=config):
# the full site, before the test-only changes below, so exports keep them.
mkdir -p ../.build
docker compose exec -T db mariadb-dump -uroot -proot --single-transaction vaxia_config > ../.build/d11-config-baseline.sql
# Test-only layout, like the D7 test site: production's sidebar and content
# list placements (menus, views lists, clock, who's online...) would put user
# and character names on every page and break page-wide assertions. Keep the
# page blocks the suite was written against; block tests place their own.
y 'drush php:eval "\$keep = [\"branding\", \"account_menu\", \"main_menu\", \"breadcrumbs\", \"page_title\", \"messages\", \"local_tasks\", \"local_actions\", \"content\", \"user_login\", \"background_changer\", \"dice_ruler\", \"dice_ruler_player_assist\", \"play_suggestions\", \"site_wiki_templates\", \"site_wiki_disamb\"]; foreach (\Drupal\block\Entity\Block::loadMultiple() as \$b) { if (\$b->getTheme() === \"vaxia_theme\" && !in_array(substr(\$b->id(), 12), \$keep, TRUE)) { \$b->delete(); } }"'
# No cron from page visits in tests (production: automated_cron hourly, as
# D7's cron_safe_threshold): it would run every cron job mid-test whenever a
# request found the last run over an hour old. Tests call cron themselves.
y 'drush cset -y automated_cron.settings interval 0 -q'
# Item values as typed (production recalculates them on every save, as D7's
# items_recalculate_values did; the D7 test site has it off and the item tests
# set values directly).
y 'drush cset -y vaxia_items.settings items_recalculate_values 0 -q'
# The site name and mail prefix the tests' mail assertions use ("at VAXIA-test",
# "[Vaxia] ..."): the release config ships "VAXIA" and takes the prefix from it.
y 'drush cset -y system.site name VAXIA-test -q && drush cset -y vaxia_core.settings vaxia_mail_prefix Vaxia -q'
# The node ids the tests create their quizzes and forms with (the D7 site's;
# the release config leaves them empty for scripts/starter-content.php).
y 'drush php:eval "\$f = \Drupal::configFactory(); \$f->getEditable(\"vaxia_conduct.settings\")->set(\"cold_storage_return_form\", 11863)->save(); \$g = \$f->getEditable(\"vaxia_governance.settings\"); \$apps = \$g->get(\"role_app_settings\"); \$apps[\"storyteller\"][\"quiz\"] = [7181]; \$apps[\"mediator\"][\"quiz\"] = [11416]; \$g->set(\"anon_mailer_webform\", 9368)->set(\"walkthrough_quizzes\", [\"system\" => 9781, \"setting\" => 10344, \"social\" => 10231])->set(\"role_app_settings\", \$apps)->save(); \$s = \$f->getEditable(\"vaxia_storyhosts.settings\"); foreach ([\"ash1\" => 7037, \"ash2\" => 7050, \"ash2_renewal\" => 7781, \"eval\" => 9359, \"ash3\" => 7181] as \$k => \$nid) { \$s->set(\"sh_test_quiz_\" . \$k, \$nid); } \$s->set(\"sh_request_forums\", [10, 11])->set(\"storyhost_feedback_form\", 9832)->set(\"session_feedback_form\", 9907)->save(); \$f->getEditable(\"vaxia_core.settings\")->set(\"motd_page\", 11918)->save(); \$f->getEditable(\"vaxia_governance.settings\")->set(\"department_join_forms\", [\"social\" => 12927, \"setting\" => 12928, \"system\" => 12929])->save();"'
# Forum access writes node grants: without a rebuild the baseline keeps the
# nid-0 "all" row and core skips node access filtering on every listing.
y 'drush php:eval "\\Drupal::service(\\Drupal\\node\\NodeAccessRebuild::class)->rebuild();"'
# Snapshot without cache contents: a restored baseline then rebuilds caches
# (routes, hooks, container) from the current code, so code changes don't
# need a reinstall.
y 'drush php:eval "foreach (\Drupal::database()->query(\"SHOW TABLES LIKE '"'"'cache%'"'"'\")->fetchCol() as \$t) { \Drupal::database()->truncate(\$t)->execute(); }"'
# Test runs reload the baseline (dropping every table) per test class: one
# starting mid-install leaves a half-installed site (uid 1 still the core
# placeholder). Refuse to snapshot that.
name=$(docker compose exec -T db mariadb -uroot -proot -N -e "SELECT name FROM users_field_data WHERE uid = 1" vaxia_config 2>/dev/null | tr -d '\r')
if [ "$name" != admin ]; then
  echo "Install did not finish (uid 1 is '$name'): baseline NOT written. Stop test runs and rerun." >&2
  exit 1
fi
mkdir -p ../.build
docker compose exec -T db mariadb-dump -uroot -proot --single-transaction vaxia_config > ../.build/d11-baseline.sql
echo "D11 baseline: $(du -h ../.build/d11-baseline.sql | cut -f1)"
