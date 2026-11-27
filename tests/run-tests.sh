#!/bin/bash
# Runs the VAXIA PHPUnit suite inside the docker web container (tests/docker-d11).
#   tests/run-tests.sh                        # everything
#   tests/run-tests.sh modules/rpg/strikes    # one module
#   tests/run-tests.sh --group knownbug       # only the tests that pin bugs D7 had (fixed here)
# First time: (cd tests/docker-d11 && docker compose up -d --build web db && ./install.sh)
# "--target d11" is still accepted (the suite used to run against D7 too).
if [ "${1:-}" = "--target" ]; then
  if [ "$2" != d11 ]; then
    echo "Unknown target $2: the suite runs against the Drupal 11 site only (D7 is on the 7.x-1.x branch)." >&2
    exit 1
  fi
  shift 2
fi
cd "$(dirname "$0")/docker-d11" || exit 1
# Fresh error log per run (PHP notices seen during tests).
rm -f ../.build/php-errors.log
args=()
for a in "$@"; do
  if [ -e "../$a" ]; then a="/var/www/tests/$a"; fi
  args+=("$a")
done
exec docker compose exec -T -e VAXIA_TARGET=d11 -w /var/www/tests web phpunit -c /var/www/tests/phpunit.xml "${args[@]}"
