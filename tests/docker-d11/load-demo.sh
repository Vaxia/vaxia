#!/bin/bash
# Loads fixtures/vaxia-starter.sql into the demo site (database vaxia_demo,
# http://localhost:8080) and prints a one-time admin login link. Reloading
# starts the demo over. Start the service first: docker compose up -d demo
set -euo pipefail
cd "$(dirname "$0")"
docker compose exec -T db mariadb -uroot -proot -e "DROP DATABASE IF EXISTS vaxia_demo; CREATE DATABASE vaxia_demo; GRANT ALL ON vaxia_demo.* TO 'vaxia'@'%'"
docker compose exec -T db mariadb -uroot -proot vaxia_demo < ../../fixtures/vaxia-starter.sql
docker compose exec -T demo bash -c 'mkdir -p /var/www/vaxia/web/sites/demo/files /var/www/vaxia/private-demo && chown -R www-data /var/www/vaxia/web/sites/demo /var/www/vaxia/private-demo && cd /var/www/vaxia && vendor/bin/drush cr -q && vendor/bin/drush state:set system.test_mail_collector "[]" --input-format=yaml -q'
echo "Demo site: http://localhost:8080"
docker compose exec -T demo bash -c 'cd /var/www/vaxia && vendor/bin/drush user:login --uri=http://localhost:8080 --no-browser'
