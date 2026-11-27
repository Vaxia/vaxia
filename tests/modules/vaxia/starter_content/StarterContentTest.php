<?php

namespace Vaxia\Tests\Vaxia\StarterContent;

use Vaxia\Tests\VaxiaTestCase;

/**
 * scripts/starter-content.php: what a new site needs to be played from the
 * first login: the Storyhost realm and three example realms (Fantasy, Science
 * Fiction, Horror) with setting and skills pages, rooms, shop items and an
 * example character each; terms, forums with access grants, the storyhost
 * course quizzes, forms, Limbo, the welcome, system and policy pages; and the
 * settings pointing at them. The test site starts from the release config's
 * empty settings here, as a new site does.
 *
 * @group starter_content
 */
class StarterContentTest extends VaxiaTestCase {

  /** @var string What the first run printed (the class shares one database). */
  private static $firstRun;

  protected function setUp(): void {
    parent::setUp();
    if (self::$firstRun === NULL) {
      $this->site->useReleaseSettings();
      self::$firstRun = $this->site->runStarterContent();
    }
  }

  public static function tearDownAfterClass(): void {
    self::$firstRun = NULL;
    parent::tearDownAfterClass();
  }

  public function testItReportsWhatItMade() {
    $this->assertStringContainsString('quiz: 9', self::$firstRun);
    $this->assertStringContainsString('rpg_chatroom: 10', self::$firstRun);
    $this->assertStringContainsString('items: 12', self::$firstRun);
    $this->assertStringContainsString('character_sheet: 21', self::$firstRun);
    $this->assertStringContainsString('opening posts: 9', self::$firstRun);
    $this->assertStringContainsString('welcome posts: 1', self::$firstRun);
    $this->assertStringContainsString('play_suggestions: 15', self::$firstRun);
    $this->assertStringContainsString('skill presets: 30', self::$firstRun);
    $this->assertStringContainsString('Starter content ready.', self::$firstRun);
  }

  public function testRunningAgainMakesNothing() {
    $again = $this->site->runStarterContent();
    $this->assertStringNotContainsString('quiz:', $again);
    $this->assertStringNotContainsString('terms:', $again);
    $this->assertStringNotContainsString('welcome posts:', $again);
    $this->assertStringContainsString('Starter content ready.', $again);
  }

  public function testTheFrontPageIsTheWelcomePage() {
    $this->anonymous();
    $this->get('');
    $this->assertStatus(200);
    $this->assertText('Three worlds to play in');
  }

  public function testSettingsPointAtTheStarterContent() {
    $this->login($this->superuser());
    foreach (array('ash1' => 'ASH Test 1 - Story Hosting Basics', 'ash3' => 'ASH Test Final Exam') as $key => $title) {
      $this->get('node/' . $this->site->setting('vaxia_storyhosts.settings', 'sh_test_quiz_' . $key));
      $this->assertText($title);
    }
    $this->get('node/' . $this->site->setting('vaxia_conduct.settings', 'cold_storage_return_form'));
    $this->assertText('Returning Player');
    $this->get('node/' . $this->site->setting('vaxia_governance.settings', 'anon_mailer_webform'));
    $this->assertText('Anonymous Mailer');
    // The feedback forms' mails (review, 2026-11-26: the old site's node ids
    // were hard-coded).
    foreach (array('storyhost_feedback_form' => 'Storyhost Feedback', 'session_feedback_form' => 'Session Feedback') as $key => $title) {
      $this->get('node/' . $this->site->setting('vaxia_storyhosts.settings', $key));
      $this->assertText($title);
    }
    $this->assertCount(3, $this->site->setting('vaxia_storyhosts.settings', 'sh_request_forums'));
    $this->assertCount(1, $this->site->setting('vaxia_core.settings', 'vaxia_limbo_room'));
  }

  public function testEveryPageTheMailsLinkToExists() {
    $this->login($this->site->createUser(array('player')));
    foreach (array('wiki/terms-and-conditions', 'wiki/code-conduct', 'wiki/harassment-policy', 'wiki/real-life-harm', 'wiki/returning-players', 'wiki/care-and-feeding-your-character', 'wiki/beginners-guide-roleplaying', 'wiki/beginners-guide-sessions', 'wiki/sh-course', 'wiki/system-rules/basics/ash-1-responsibilities', 'wiki/system-rules/ruling/ash-2-responsibilities', 'sessions/storytellers-and-apprentices', 'proposals/site-governance', 'wiki/story-hosting-basics', 'wiki/advanced-story-hosting', 'wiki/evaluators', 'wiki/system-rules/items-and-equipment', 'wiki/system-rules', 'content/returning-player', 'content/session-feedback', 'content/storyhost-feedback', 'locations/limbo', 'quizzes/ash-test-1-story-hosting-basics', 'quizzes/ash-test-2-ruling', 'quizzes/ash-test-2-renewing-shs', 'quizzes/ash-test-3-evaluation', 'quizzes/ash-test-final-exam', 'quizzes/new-player-system-walkthrough') as $path) {
      $this->get($path);
      $this->assertStatus(200, $path);
    }
  }

  public function testTheWalkthroughsAreTheOriginalsAdapted() {
    $this->login($this->site->createUser(array('player')));
    $this->get('quizzes/new-player-system-walkthrough');
    $questions = array();
    foreach ($this->xpath()->query('//input[starts-with(@name, "question_")]') as $input) {
      $questions[preg_replace('/\[.*$/', '', $input->getAttribute('name'))] = TRUE;
    }
    $this->assertCount(21, $questions);
    $this->assertNoText('Vaxia');
    $this->get('quizzes/new-player-setting-walkthrough');
    $this->assertText('Eldmoor');
    $this->get('quizzes/new-player-social-walkthrough');
    $this->assertStatus(200);
    $this->get('quizzes/mediator-training');
    $this->assertStatus(200);
  }

  public function testDepartmentJoinFormsAndMessageOfTheDay() {
    $this->login($this->superuser());
    foreach (array('social' => 'Join Social Department', 'setting' => 'Join Setting Department', 'system' => 'Join System Department') as $key => $title) {
      $this->get('node/' . $this->site->setting('vaxia_governance.settings', 'department_join_forms')[$key]);
      $this->assertText($title);
    }
    $this->get('node/' . $this->site->setting('vaxia_core.settings', 'motd_page'));
    $this->assertText('Message of the Day');
  }


  public function testForumGrantsKeepPrivateForumsPrivate() {
    $this->login($this->site->createUser(array('player')));
    $this->get('forum');
    $this->assertText('SH Requests');
    $this->assertNoText('Leads Discussion');
    $this->login($this->site->createUser(array('lead')));
    $this->get('forum');
    $this->assertText('Leads Discussion');
  }

  public function testCharactersCanPickTheStarterRealms() {
    $this->login($this->site->createUser(array('player')));
    $this->get('node/add/character_sheet');
    $options = array();
    foreach ($this->xpath()->query('//select[@name="field_tag_realm"]/option') as $option) {
      $options[] = trim($option->textContent);
    }
    foreach (array('Fantasy', 'Science Fiction', 'Horror', 'Storyhost') as $realm) {
      $this->assertContains($realm, $options);
    }
  }

  public function testTheCourseQuizzesHaveTheirQuestions() {
    $this->login($this->site->createUser(array('player')));
    $this->get('quizzes/ash-test-1-story-hosting-basics');
    $questions = array();
    foreach ($this->xpath()->query('//input[starts-with(@name, "question_")]') as $input) {
      $questions[preg_replace('/\[.*$/', '', $input->getAttribute('name'))] = TRUE;
    }
    $this->assertCount(12, $questions);
    $this->get('quizzes/ash-test-final-exam');
    $this->assertSame(9, $this->xpath()->query('//textarea[starts-with(@name, "question_")]')->length);
  }

  public function testEachRealmHasItsPagesRoomsAndCharacter() {
    $this->login($this->site->createUser(array('player')));
    foreach (array('wiki/eldmoor' => 'Thornwick', 'wiki/the-meridian-reach' => 'Port Kessler', 'wiki/hollow-creek' => 'Blackwood Asylum', 'wiki/eldmoor/skills' => 'Swordplay', 'wiki/the-meridian-reach/skills' => 'Starship Engineering', 'wiki/hollow-creek/skills' => 'Occult Lore') as $path => $text) {
      $this->get($path);
      $this->assertStatus(200, $path);
      $this->assertText($text);
    }
    foreach (array('the-prancing-stag', 'the-whispering-wood', 'ruins-of-castle-greymoor', 'port-kessler-promenade', 'bridge-of-the-meridian', 'the-derelict-icarus', 'the-last-light-diner', 'blackwood-asylum', 'ravensworth-manor') as $room) {
      $this->get('locations/' . $room);
      $this->assertStatus(200, $room);
    }
    $this->login($this->superuser());
    foreach (array('wren-ashdown' => 'Halfling', 'talia-okonkwo' => 'Chief engineer', 'eli-marsh' => 'Occult Lore') as $character => $text) {
      $this->get('characters/' . $character);
      $this->assertStatus(200, $character);
      $this->assertText($text);
    }
  }

  public function testEachRealmHasStockNpcs() {
    // Open, generic NPCs named for their role (user, 2026-11-26); the world
    // keeps same-named roles apart.
    $this->login($this->superuser());
    foreach (array('eldmoor/generic-shopkeeper' => 'Haggling', 'hollow-creek/generic-shopkeeper' => 'Folk Remedies', 'the-meridian-reach/generic-security-officer' => 'Crowd Control', 'eldmoor/generic-town-guard' => 'Spear and Shield') as $path => $text) {
      $this->get('characters/' . $path);
      $this->assertStatus(200, $path);
      $this->assertText($text);
    }
    // Storyhosts may post as them in their realm's rooms; players may not.
    $groups = function () {
      $groups = array();
      foreach ($this->xpath()->query('//select[@name="field_comment_character"]//optgroup') as $group) {
        foreach ($group->getElementsByTagName('option') as $option) {
          $groups[$group->getAttribute('label')][] = trim($option->textContent);
        }
      }
      return $groups;
    };
    $this->login($this->site->createUser(array('player', 'storyteller'), array('view rpg chat', 'view vaxia ic chat')));
    $this->get('locations/the-prancing-stag');
    $generic = $groups()['Generic NPCs'] ?? array();
    foreach (array('Generic Town Guard', 'Generic Barkeep', 'Generic Tavern Server', 'Generic Shopkeeper', 'Generic Bandit', 'Generic Hedge Witch') as $npc) {
      $this->assertContains($npc, $generic);
    }
    $this->assertNotContains('Generic Cultist', $generic);
    $this->login($this->site->createUser(array('player'), array('view rpg chat', 'view vaxia ic chat')));
    $this->get('locations/the-prancing-stag');
    $this->assertArrayNotHasKey('Generic NPCs', $groups());
    $this->assertStringNotContainsString('Generic Town Guard', $this->pageText());
  }

  public function testTheShopOffersTheCharactersRealmsItems() {
    $owner = $this->site->createUser(array('player'), array('use item shop'));
    $character = $this->site->createCharacter($owner, array('field_tag_realm' => $this->site->term('realm', 'Fantasy'), 'field_economy' => 25));
    $this->approveCharacter($character);
    $this->login($owner);
    $this->get('node/' . $character->nid . '/item_shop');
    $this->assertText('Ranger\'s Longbow');
    $this->assertText('Healing Draught');
    $this->assertNoText('Coil Pistol');
    $this->assertNoText('Holy Water');
  }

  public function testTermPagesExplainTheirTerms() {
    $this->login($this->site->createUser(array('player')));
    foreach (array(array('skill_verbs', 'conceal', 'Hide people, information, or things.'), array('weapon_category', 'Heavy (75% / 10)', 'Heavy weapons hit harder'), array('content', 'PG-13', 'nothing graphic')) as list($vocabulary, $name, $text)) {
      $this->get('taxonomy/term/' . $this->site->term($vocabulary, $name));
      $this->assertStatus(200, $name);
      $this->assertText($text);
    }
  }

  public function testTheCharacterFormOffersCommonSkills() {
    $this->login($this->site->createUser(array('player')));
    $this->get('node/add/character_sheet');
    $this->assertSame(1, $this->xpath()->query('//select[@name="vaxia_skill_preset"]')->length);
    // The presets reach the page's script with their realm and aspects.
    preg_match('/<script type="application\/json" data-drupal-selector="drupal-settings-json">(.*?)<\/script>/s', $this->content, $m);
    $presets = json_decode($m[1] ?? '{}', TRUE)['vaxiaSkillPresets'] ?? array();
    $byName = array();
    foreach ($presets as $preset) {
      $byName[$preset['name']][] = $preset;
    }
    $this->assertCount(30, $presets);
    $this->assertSame((string) $this->site->term('realm', 'Fantasy'), $byName['Archery'][0]['realm']);
    $this->assertCount(3, $byName['Archery'][0]['aspects']);
    // Firearms is a common skill in two realms.
    $this->assertCount(2, $byName['Firearms']);
  }

  public function testLimboStartsWithAWelcome() {
    $this->login($this->site->createUser(array('player')));
    $this->get('locations/limbo');
    $this->assertText('Welcome to your new site!');
    $this->assertGreaterThan(0, $this->xpath()->query('//a[@href="/wiki/character-creation"]')->length);
  }

  public function testLimboOffersPlaySuggestions() {
    $this->login($this->site->createUser(array('player')));
    $this->get('locations/limbo');
    $this->assertSame(1, $this->xpath()->query('//input[@value="I\'m bored."]')->length);
  }

  public function testTheMenusLinkToRealPages() {
    // A new site's menus (menus.yml; empty before: the original's links came
    // with its data. Review, 2026-11-26). Every link opens for staff.
    $this->assertContains(array('Create a character', '/node/add/character_sheet'), $this->site->menuLinks('menu-new-start-here'));
    $this->assertContains(array('Fantasy sessions', '/wiki/fantasy/session-list'), $this->site->menuLinks('menu-wiki-links'));
    $paths = array();
    foreach (array('main', 'menu-new-start-here', 'menu-walkthrough-tutorials', 'menu-view-stuff', 'menu-create-stuff', 'menu-vaxia', 'menu-wiki-links', 'menu-social-helpers', 'menu-vaxia-ash', 'menu-vaxia-sh', 'menu-leads') as $menu) {
      $links = $this->site->menuLinks($menu);
      $this->assertNotEmpty($links, $menu);
      foreach ($links as $link) {
        if ($link[1] !== NULL) {
          $paths[$link[1]] = $menu . ': ' . $link[0];
        }
      }
    }
    // Some lists are for given roles (artwork: players): staff who play too.
    $this->login($this->site->createUser(array('player', 'storyteller', 'lead', 'mediator', 'administrator')));
    foreach ($paths as $path => $label) {
      $this->get(ltrim($path, '/'));
      $this->assertStatus(200, $label . ' (' . $path . ')');
    }
  }

  public function testTheContactLinksWork() {
    $this->login($this->site->createUser(array('player')));
    $this->get('wiki/code-conduct');
    $this->assertGreaterThan(0, $this->xpath()->query('//a[@href="/form/contact"]')->length);
    $this->get('form/contact');
    $this->assertStatus(200);
  }

  public function testRunningAgainKeepsWhatAnAdminChanged() {
    // Review, 2026-11-26: a re-run approved shop items and reset forum grants.
    $item = $this->site->nodeAtAlias('/items/eldmoor/healing-draught');
    $this->assertNotNull($item);
    $this->site->forceWorkflowState($item, 'removed from game');
    $mediators = $this->site->term('forums', 'Mediators');
    $this->site->setForumGrants($mediators, 'administrator', array('grant_view' => 0, 'grant_update' => 0, 'grant_delete' => 0, 'grant_create' => 0));
    $this->site->runStarterContent();
    $this->assertSame('removed from game', $this->site->workflowStateLabel($item));
    $this->assertSame(array('grant_view' => 0, 'grant_update' => 0, 'grant_delete' => 0, 'grant_create' => 0), $this->site->forumGrants($mediators, 'administrator'));
  }

  public function testTheFixturesSendMailAndImportIntoMysql() {
    // The fixtures came from the test baseline, which collects mail instead
    // of sending it; and mariadb-dump's sandbox line stops stock MySQL
    // clients (review, 2026-11-26).
    $fixtures = file_get_contents(DRUPAL_ROOT . '/../fixtures/vaxia-starter.sql');
    $this->assertNotFalse($fixtures);
    $this->assertStringNotContainsString('enable the sandbox mode', $fixtures);
    $this->assertStringNotContainsString('test_mail_collector', $fixtures);
  }

  public function testDepartmentsAndElectionsAreSetUp() {
    // As the original site ran them (review, 2026-11-26: shipped empty, so
    // every yearly election cancelled itself).
    $this->assertSame("Setting\nSocial\nSystem", $this->site->setting('vaxia_governance.settings', 'departments'));
    $this->assertSame("Setting Lead\nSocial Lead\nSystem Lead", $this->site->setting('vaxia_governance.settings', 'election_positions'));
    $this->assertSame(array('setting_lead' => 2, 'social_lead' => 2, 'system_lead' => 2), $this->site->setting('vaxia_governance.settings', 'election_position_counts'));
    $this->assertSame('lead', $this->site->setting('vaxia_governance.settings', 'election_role'));
    $this->assertSame(array('setting' => 'knows_the_setting', 'social' => 'knows_the_social', 'system' => 'knows_the_system'), $this->site->setting('vaxia_governance.settings', 'department_roles'));
    $this->assertNotEmpty($this->site->setting('vaxia_governance.settings', 'election_default_forum'));
  }

  public function testRunningAgainKeepsMenuLinksAnAdminSaved() {
    // The menu UI saves top-level links with parent '', the script with
    // NULL: a re-run must still find them (review, 2026-11-26).
    $this->site->saveMenuLinkAsTheUiDoes('menu-new-start-here', 'Create a character');
    $before = count($this->site->menuLinks('menu-new-start-here'));
    $this->site->runStarterContent();
    $this->assertSame($before, count($this->site->menuLinks('menu-new-start-here')));
  }

}
