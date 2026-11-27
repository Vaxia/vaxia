#!/bin/bash
# Puts the D11 site back to the clean baseline (tests/.build/d11-baseline.sql)
# so config changes made next and exported with export-config.sh don't carry
# whatever the last test run left in the database (settings, blocks, roles).
set -euo pipefail
cd "$(dirname "$0")"
# VAXIA_SITE=config restores into vaxia_config instead (see settings.append.php).
DB=vaxia; [ "${VAXIA_SITE:-}" = config ] && DB=vaxia_config
docker compose exec -T db mariadb -uroot -proot -e "CREATE DATABASE IF NOT EXISTS $DB; GRANT ALL ON $DB.* TO 'vaxia'@'%'"
# Config work starts from the full site (install.sh keeps it before the
# test-only block layout changes); tests from the test baseline.
SQL=../.build/d11-baseline.sql; [ "$DB" = vaxia_config ] && SQL=../.build/d11-config-baseline.sql
docker compose exec -T db mariadb -uroot -proot $DB < $SQL
docker compose exec -T -e VAXIA_SITE="${VAXIA_SITE:-}" web bash -c 'cd /var/www/vaxia && vendor/bin/drush cr -q'
echo "D11 site ($DB) restored to the baseline."
