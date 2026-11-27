# VAXIA tests

Black-box PHPUnit tests for the VAXIA Drupal 11 site: pages, forms,
permissions and saved rows, checked over HTTP. They were written against the
Drupal 7 site before the upgrade and then pointed at Drupal 11, so they pin
what the site did before as well as what it does now. (The D7 site is on the
7.x-1.x branch.)

## First-time setup (about 10 minutes)

```
cd tests/docker-d11
docker compose up -d --build web db   # web (PHP 8.3 + Apache, :8078) and db (MariaDB 10.11)
./install.sh                          # composer install, site from config/sync, writes tests/.build/d11-baseline.sql
```

Re-run `./install.sh` after changing config, or after adding a hook function
to an existing module: the snapshot keeps Drupal's list of hook
implementations, so tests don't see a new hook until it is rebuilt.

## Running

```
tests/run-tests.sh                          # everything
tests/run-tests.sh modules/rpg/vacation     # one module
tests/run-tests.sh --group knownbug         # only the tests that pin bugs the D7 site had
```

The test site is browsable at http://localhost:8078 (admin / admin), bound to
127.0.0.1 only. Test classes reload the database, so don't run two suites at
once.

## Config work

The test database is reloaded per test class, so config changes are made on a
copy (`vaxia_config`) and exported from there:

```
VAXIA_SITE=config tests/docker-d11/restore-baseline.sh
docker compose -f tests/docker-d11/docker-compose.yml exec -e VAXIA_SITE=config web drush ...
VAXIA_SITE=config tests/docker-d11/export-config.sh   # writes config/sync
tests/docker-d11/install.sh                            # new snapshots from config/sync
```

`d11-config-baseline.sql` is a fresh install of the release config and
`d11-baseline.sql` the test layout: production sidebar blocks removed, cron
from page visits off, item values not recalculated, mail collected, and the
settings the tests expect (site name "VAXIA-test", mail prefix "Vaxia", and
the node and term ids the tests create their quizzes, forms and forums with).
The test site gets no starter content: tests create what they use.

## The starter fixtures

`fixtures/vaxia-starter.sql` is a fresh install of `config/sync` with
`scripts/starter-content.php` run, for new sites to import. After changing
the config or the starter content, rebuild it (and commit it):

```
tests/docker-d11/install.sh          # if the config changed
tests/docker-d11/make-fixtures.sh
```

It uses the config copy (`vaxia_config`), not the test database, and removes
everything site-specific (admin password, private key, cron key, caches,
logs). `modules/vaxia/starter_content` tests the script itself.

## CircleCI

`.circleci/config.yml` runs the same Docker stack on machine executors: setup
(build the image, cached composer install, install the test site, lint the
custom code, harness check) then test boxes (test files split by timing data,
one PHPUnit run per file, junit results).

Pick a run with the `RUN` pipeline parameter (Trigger Pipeline > Add Parameter):
`PULLREQUEST` (default), `FULLSET`, `KNOWNBUG`, or `DEBUG` with `GROUP` (a
module's `@group`, e.g. `strikes`) and `BOXES`.

## How it works

Everything that knows about Drupal lives in `src/d11/`:

- `bootstrap.php`: boots Drupal in-process.
- `Site.php`: the setup driver (below).
- `TargetCase.php` (the base of `VaxiaTestCase`): database snapshot, caches,
  mail capture, cron, and how the site shows messages, roles and workflow
  states. It also maps D7 names to D11 ones: `field_x[und][0][value]` becomes
  `field_x[0][value]`, and core buttons D11 labels differently (`BUTTON_LABELS`).

- **Arrange** through `$this->site` (`src/d11/Site.php`). It creates users,
  characters, rooms and posts, sets configuration that has no admin form, and
  does "time travel" (backdating a record, resetting a daily cron guard). It's
  the only code that touches Drupal.
- **Act** over HTTP: login, forms, the Workflow tab, cron. Each user gets their
  own browser session (`login($account)` switches between them).
  `triggerAjax()` fires an element's `#ajax` (as a browser does on change) and
  applies the returned markup, so the next `submitForm()` sees the rebuilt form.
- Shared flows live in traits under `src/`: `DiceTrait` (post dice rolls with
  items in a chat room, read the rendered results), `ProposalTrait` (create,
  launch, vote on, close proposals), `StrikeFormTrait`.
- Production grants many permissions to anonymous, authenticated and player
  users. A test of a permission gate revokes it for the test with
  `withoutPermission($permission, $roles, $fn)`.
- **Assert** over HTTP: page text, messages, admin screens (`rolesOf`,
  `isBlocked`, `workflowState`, `profileText`, `roleHasPermission`), plus
  captured mail.
- `HarnessTest` fails if a module test calls a Drupal or module function
  directly.
- The database is restored from the snapshot once per test class, so tests in
  a class share it. Keep assertions about your own users and content, or reset
  shared settings in `setUp()`.
- PHP notices that pages print go to `.build/php-errors.log` (cleared on each
  run). `pageErrorsFrom('x.module')` returns the current test's.
- Tests live in `modules/<area>/<module>/`. Some still check
  `getenv('VAXIA_TARGET')` from when they also ran on D7; it is always `d11`.

The bugs the D7 site had are listed in `KNOWN-BUGS.md`.
