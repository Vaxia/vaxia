# VAXIA

VAXIA is a play-by-post roleplaying community site on Drupal 11 (PHP 8.3):
character sheets with stats, skills, XP and silver; storyhosted chat rooms
with dice; items and an item shop; scenes and sessions; a storyhost course;
player governance (proposals, elections, departments); and a wiki. A starter
site with three example realms (fantasy, science fiction and horror) gets a
new community playing from the first login.

This repository is a Composer project in the standard Drupal layout:

- `composer.json`, `composer.lock`: Drupal core and contrib
  (`composer install` fetches them into `vendor/` and `web/`).
- `web/modules/custom/`, `web/themes/custom/`: the VAXIA modules and theme.
- `config/sync/`: the site configuration.
- `patches/`: contrib fixes, applied by composer-patches.
- `fixtures/vaxia-starter.sql`: a ready starter site to import.
- `scripts/starter-content.php` and `scripts/starter-content/`: the starter
  content (the fixtures are a fresh install with it run).
- `tests/`: the black-box PHPUnit suite and its Docker stack (see
  `tests/README.md`).

## Setting up a new site

1. Get the code and its dependencies:
   ```
   composer install
   ```
2. Copy `web/sites/default/default.settings.php` to `settings.php` and set:
   - `$databases` to an empty database;
   - `$settings['config_sync_directory'] = '../config/sync';`
   - `$settings['file_private_path']` to a directory outside the web root;
   - `$settings['hash_salt']` to a long random string of your own;
   - `$settings['trusted_host_patterns']` for your domain.
3. Load the starter site. Either:
   - **Import the fixtures** (quickest):
     ```
     mysql yourdb < fixtures/vaxia-starter.sql
     vendor/bin/drush cache:rebuild
     vendor/bin/drush user:login --uri=https://your.site   # admin has a random password: set yours
     ```
   - **Or install and add the starter content:**
     ```
     vendor/bin/drush site:install --existing-config --account-name=admin --account-mail=you@example.com
     vendor/bin/drush php:script scripts/starter-content.php
     ```
4. Before opening the site:
   - Set the site name and email (Administration > Configuration > Basic site
     settings). Mail subjects start with "[site name]"; Administration >
     VAXIA > Settings can set a different prefix.
   - Adapt the policy pages (Terms and Conditions, Code of Conduct and the
     rest) to your community.
   - Read the new-player walkthroughs, the mediator training and the
     storyhost course quizzes. They are the original community's, with the
     example realms in place: adapt them to your rules and worlds.
   - Give your founding staff their roles (People > edit the account >
     Roles): **storyteller** for storyhosts, who evaluate characters and
     items and run sessions; **lead** for the department leads; and
     **mediator** for your first mediators. A new site has none of them:
     until there are storyhosts only leads and administrators can evaluate
     characters, and only mediators approve mediator applications. Later
     mediators apply after the mediator training; an administrator marks
     who may apply with the **eligible for mediator** role. For example:
     ```
     vendor/bin/drush user:role:add storyteller yourname
     vendor/bin/drush user:role:add lead yourname
     ```
5. Cron runs hourly from page visits (`automated_cron`). A server crontab
   (`drush cron`) works too.

New accounts confirm they meet the age requirement on the sign-up form. Under
age, the account is blocked. Accounts made with `drush user:create` count as
under age until unblocked.

## What the starter site has

It's ready to play from the first login:

- **Realms.** Storyhost (the administrative realm: storyhosting and the
  system pages) and three example realms, each a small world built on its
  genre's tropes:
  - **Fantasy (Eldmoor).** The Prancing Stag tavern, the Whispering Wood and
    the ruins of Castle Greymoor.
  - **Science Fiction (the Meridian Reach).** Port Kessler, the freighter
    Meridian and the derelict Icarus.
  - **Horror (Hollow Creek).** The Last Light Diner, Blackwood Asylum and
    Ravensworth Manor.

  Each realm has a setting page with places, factions and story hooks, a
  skills page, three rooms, four shop items, an example character (an open
  NPC storyhosts can play) with five skills and gear, and six stock NPCs for
  common encounters (open, generic NPCs named for their role: Generic Town
  Guard, Generic Barkeep, Generic Bartender and the like).
- **The system pages.** Character creation, skills and aspects, dice, XP,
  economy and items, storyhosting basics and the storyhost course.
- **The storyhost course.** The original VAXIA course quizzes: ASH Tests 1 and
  2, the renewal test, the evaluation exam and the final exam.
- **The community.** Limbo (the out-of-character room), the forums with their
  access grants, and the returning-player, anonymous-mailer and feedback
  forms.

The content comes from `scripts/starter-content/`: `realms.yml` (realms,
rooms, skills, items, characters and stock NPCs), `pages.yml` (wiki pages),
`sh-course.yml` (storyhost course), `walkthroughs.yml` (new-player
walkthroughs and mediator training) and `menus.yml` (the menus). Edit those
before running the script to start with your own worlds. The script can be
run again: it adds what's missing and leaves what's there.

**Adding a realm.** Add the realm term (Structure > Taxonomy > Realm), give
character sheets a species and a languages list field for it, and add the
three to `realm_fields` in `vaxia_core.settings`. So other players see a
character's species, add the species field to "Publicly visible character
fields" (Administration > VAXIA > Settings). Calendars
(`/sessions/calendar/<realm-name>`), session lists
(`/wiki/<realm-name>/session-list`), the timeline, the session filters and
the item shop pick it up from there.

## Local development

```
cd tests/docker-d11
docker compose up -d --build web db
./install.sh          # composer install, installs the test site from config/sync
```

The test site is then at http://localhost:8078 (admin / admin, bound to
127.0.0.1). See `tests/README.md` for running the tests and changing config.

## History

VAXIA ran on Drupal 7 until 2017. That site is on the `7.x-1.x` branch.
The old site data can be migrated to the new Drupal 11 version.
If you wish to restart the original Vaxia under the D11 codebase, please contact me
because there are a number of things to discuss before restarting the original site.
