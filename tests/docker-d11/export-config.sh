#!/bin/bash
# Exports the D11 site's config to config/sync, then drops what tests
# leave behind in the database: their throwaway roles (role_<pid>_<n>), the
# actions for those roles, webforms they created (proposal_<nid>,
# application_<nid>, webform_/quiz_<pid>_<n>), and the test mail collector
# setting. Run after enabling a module or changing config, then ./install.sh.
#
# Exports to a scratch folder first and only replaces config/sync when the
# export finished and didn't lose more than 5% of the files: a half-installed
# site once wiped most of the config this way.
set -euo pipefail
cd "$(dirname "$0")"
SYNC=../../config/sync
docker compose exec -T -e VAXIA_SITE="${VAXIA_SITE:-}" web bash -c 'rm -rf /tmp/cex && mkdir /tmp/cex && cd /var/www/vaxia && vendor/bin/drush cex -y -q --destination=/tmp/cex && ls /tmp/cex | wc -l' > /tmp/vaxia-cex-count
new=$(tr -d ' \n' < /tmp/vaxia-cex-count)
old=$(ls "$SYNC" | wc -l | tr -d ' ')
if [ "$new" -lt $(( old * 95 / 100 )) ]; then
  echo "Export has $new files, config/sync has $old: NOT replacing (is the site fully installed?)" >&2
  exit 1
fi
rm -f "$SYNC"/*.yml
docker compose exec -T web bash -c 'cd /tmp/cex && tar cf - *.yml' | tar xf - -C "$SYNC"
cd "$SYNC"
rm -f user.role.role_[0-9]*_[0-9]*.yml system.action.user_*_role_action.role_[0-9]*_[0-9]*.yml \
  webform.webform.proposal_[0-9]*.yml webform.webform.application_[0-9]*.yml \
  webform.webform.webform_[0-9]*_[0-9]*.yml webform.webform.quiz_[0-9]*_[0-9]*.yml
# Test roles listed inside other config (e.g. privatemsg's allowed roles).
for f in $(grep -lE '^[[:space:]]*- role_[0-9]+_[0-9]+$' *.yml 2>/dev/null || true); do
  sed -i.bak -E '/^[[:space:]]*- role_[0-9]+_[0-9]+$/d' "$f" && rm -f "$f.bak"
done
git checkout -- system.mail.yml node.type.page.yml 2>/dev/null || true
echo "Exported: $(ls | wc -l | tr -d ' ') config files"
