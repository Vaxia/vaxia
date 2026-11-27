<?php

/**
 * @file
 * Starter content for a new VAXIA site: everything the game needs to be
 * played from the first login, created once after installing from config.
 *
 *   drush site:install --existing-config ...
 *   drush php:script scripts/starter-content.php
 *
 * (Or import fixtures/vaxia-starter.sql, which is a fresh install with this
 * already run: see the README.)
 *
 * From scripts/starter-content/:
 * - realms.yml: the administrative realm (Storyhost) and three example
 *   realms (Fantasy, Science Fiction, Horror), each with a setting page, a
 *   skills page and common skills (the character form's skill picker),
 *   three rooms, shop items, an example character (an open NPC
 *   storyhosts can play) and stock NPCs (open, generic NPCs for common
 *   encounters);
 * - pages.yml: the welcome page (front page), the system and storyhosting
 *   pages, and the community pages the site's mails link to;
 * - sh-course.yml: the storyhost course quizzes.
 * And here: game terms (content limits and ratings, departments, item types,
 * weapon classes, skill verbs...), the forums with their access grants, the
 * Limbo room with a welcome post, the returning-player, anonymous-mailer, feedback and department
 * join forms, opening posts in the realms' rooms, play suggestions, and the
 * settings pointing at all of it.
 * - walkthroughs.yml: the new-player walkthroughs and mediator training;
 * - menus.yml: the main and sidebar menus.
 *
 * Safe to run again: anything that exists (terms by name, content by path,
 * forms by id) is kept, and settings are only filled where empty.
 */

use Drupal\node\Entity\Node;
use Drupal\menu_link_content\Entity\MenuLinkContent;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\taxonomy\Entity\Term;
use Drupal\vaxia_core\Character;
use Drupal\vaxia_items\Shop;
use Drupal\webform\Entity\Webform;
use Symfony\Component\Yaml\Yaml;

$data = __DIR__ . '/starter-content/';
$realms_data = Yaml::parseFile($data . 'realms.yml');
$pages_data = Yaml::parseFile($data . 'pages.yml');
$course_data = Yaml::parseFile($data . 'sh-course.yml');
$walkthrough_data = Yaml::parseFile($data . 'walkthroughs.yml');

$terms = [
  'character_limits' => ['Graphic sexual content', 'Player vs player combat', 'Graphic violent content', 'Mind control', 'Loss of consent'],
  'content' => ['G', 'PG', 'PG-13', 'R', 'W: Possible character death', 'C: All fight - no talk', 'C: All talk - no fight', 'TW: Snakes and/or Spiders', 'TW: Graphic violence', 'TW: Sexual violence', 'TW: Mind control', 'TW: Graphic dead-stuff'],
  'department' => ['Administration', 'Setting', 'Social', 'System'],
  'item_type' => ['Companion/Drone', 'Armor/Shield', 'Weapon', 'Charged Item', 'Misc'],
  'location_depth' => ['Greater Region', 'Region', 'City', 'District', 'Establishment'],
  'organization_type' => ['Criminal', 'Military', 'Political', 'Population Group', 'Professional Org', 'Religious', 'Revolution', 'Species'],
  'setting_type' => ['Location', 'Formal Government', 'Template / Category List', 'Person', 'Species', 'Organization', 'Magic / Psionics', 'Science', 'Technology', 'Historical Event', 'Cultural Note', 'Ship', 'Mystery / Oddity'],
  'skill_verbs' => ['avoid', 'bolster', 'bypass', 'change nature', 'change shape', 'conceal', 'convince', 'craft', 'damage', 'discover', 'endure', 'know', 'move', 'operate', 'persist', 'predict', 'repair', 'summon', 'teleport', 'weaken'],
  'technology_languages' => ['Machine code', 'Corporate legacy systems', 'Gate protocols', 'Military encryption', 'Alien cipher'],
  'weapon_category' => ['Unarmed (0% / 0)', 'Light (15% / -10)', 'Medium (45% / 0)', 'Heavy (75% / 10)'],
  'weapon_sub_category' => ['Axe', 'Bow', 'Club / Blunt Object', 'Crossbow', 'Hand-to-hand', 'Long blade', 'Long-gun', 'Pistol', 'Polearm', 'Short blade'],
  'ash_test' => ['Department', 'Mediator Training', 'Newbie Helper', 'No category', 'Setting walkthrough', 'Social walkthrough', 'System walkthrough'],
];

// What each skill aspect (skill verb) does, shown on its term page.
$term_descriptions['skill_verbs'] = [
  'avoid' => 'Prevent hit point damage from occurring. Shields, etc.',
  'bolster' => 'Create a positive circumstance modifier or remove a negative one.',
  'bypass' => 'Avoid a protective effect without negating it.',
  'change nature' => 'Such as temperature or information content.',
  'change shape' => 'Such as volume, dimensions.',
  'conceal' => 'Hide people, information, or things.',
  'convince' => 'People skills and mental influence.',
  'craft' => 'Customized permanent effect.',
  'damage' => 'Deal hit point damage to people or objects.',
  'discover' => 'Gain new info about things that are.',
  'endure' => 'Prevent secondary effects of damage, such as not falling unconscious at zero.',
  'know' => 'Possess knowledge of pre-existing info, book learning.',
  'move' => 'Transport safely and quickly from point A to point B.',
  'operate' => 'Use a thing without changing it in any measurable way.',
  'persist' => 'Make a temporary effect, permanent.',
  'predict' => 'Gain new info about things that may be.',
  'repair' => 'Restore hit points to damaged people or objects.',
  'summon' => 'Create a temporary effect.',
  'teleport' => 'Movement, skipping space between.',
  'weaken' => 'Create a negative circumstance modifier or remove a positive one.',
];

$term_descriptions['weapon_category'] = [
  'Unarmed (0% / 0)' => 'Unarmed weapons give neither bonus nor penalty.',
  'Light (15% / -10)' => 'Light weapons hit for less, but can be used more often.',
  'Medium (45% / 0)' => 'Medium weapons are average for damage per hit and number of hits.',
  'Heavy (75% / 10)' => 'Heavy weapons hit harder, but are harder to use more often.',
];
$term_descriptions['weapon_sub_category'] = [
  'Axe' => 'Hafted chopping blades, from hatchets to great axes.',
  'Bow' => 'Short, long and compound bows.',
  'Club / Blunt Object' => 'Clubs, maces, hammers and anything heavy enough to swing.',
  'Crossbow' => 'Mechanical bows, light to heavy.',
  'Hand-to-hand' => 'Fists, feet and close grappling, with or without knuckles and claws.',
  'Long blade' => 'Swords long enough to need a scabbard on the hip or back.',
  'Long-gun' => 'Rifles, shotguns and other two-handed firearms or energy weapons.',
  'Pistol' => 'One-handed firearms and sidearms.',
  'Polearm' => 'Spears, halberds and staves: reach weapons.',
  'Short blade' => 'Knives, daggers and short swords.',
];
$term_descriptions['character_limits'] = [
  'Graphic sexual content' => 'The player is OK with explicit sexual content involving this character.',
  'Player vs player combat' => 'Other players\' characters may fight this character.',
  'Graphic violent content' => 'The player is OK with gore and detailed violence involving this character.',
  'Mind control' => 'This character may be charmed, possessed or mentally controlled.',
  'Loss of consent' => 'Story events may happen to this character without the player\'s say in the moment.',
];
$term_descriptions['content'] = [
  'G' => 'Suitable for everyone.',
  'PG' => 'Mild peril and conflict.',
  'PG-13' => 'Violence, fear and mature themes, nothing graphic.',
  'R' => 'Graphic violence, strong language or adult themes.',
  'W: Possible character death' => 'Warning: characters may die in this session.',
  'C: All fight - no talk' => 'Combat-focused: expect fights, not conversation.',
  'C: All talk - no fight' => 'Conversation-focused: intrigue, investigation or social play, no fights.',
  'TW: Snakes and/or Spiders' => 'Content warning: snakes or spiders appear.',
  'TW: Graphic violence' => 'Content warning: graphic violence.',
  'TW: Sexual violence' => 'Content warning: references to sexual violence.',
  'TW: Mind control' => 'Content warning: characters may be mentally controlled.',
  'TW: Graphic dead-stuff' => 'Content warning: corpses, decay or graphic death.',
];
$term_descriptions['department'] = [
  'Administration' => 'Running the site: accounts, servers, money and the other departments.',
  'Setting' => 'The worlds: lore, the wiki and setting consistency.',
  'Social' => 'The community: conduct, mediation and welcoming new members.',
  'System' => 'The game rules: mechanics, evaluations and balance.',
];
$term_descriptions['item_type'] = [
  'Companion/Drone' => 'An animal companion, drone or other helper, bought with build points.',
  'Armor/Shield' => 'Protection that absorbs damage.',
  'Weapon' => 'Anything made to fight with: it has a weapon class and type.',
  'Charged Item' => 'An item with a power used a set number of times, per day or at will.',
  'Misc' => 'Anything else: tools, charms and gear with small bonuses.',
];
$term_descriptions['location_depth'] = [
  'Greater Region' => 'A continent, star cluster or other very large area.',
  'Region' => 'A country, province or star system.',
  'City' => 'A city, town, station or settlement.',
  'District' => 'A neighbourhood or section of a city.',
  'Establishment' => 'A single place: a tavern, ship, shop or house.',
];
$term_descriptions['organization_type'] = [
  'Criminal' => 'Gangs, guilds of thieves, smuggling rings.',
  'Military' => 'Armies, navies, mercenary companies.',
  'Political' => 'Governments, courts, parties and factions.',
  'Population Group' => 'A people or community rather than a formal body.',
  'Professional Org' => 'Guilds, companies and trade associations.',
  'Religious' => 'Churches, cults and orders.',
  'Revolution' => 'Movements trying to overturn the order of things.',
  'Species' => 'A species considered as a group.',
];
$term_descriptions['setting_type'] = [
  'Location' => 'A place in one of the worlds.',
  'Formal Government' => 'A state or ruling body and how it works.',
  'Template / Category List' => 'A list or template page that collects other pages.',
  'Person' => 'A non-player character of note.',
  'Species' => 'A playable or non-playable species.',
  'Organization' => 'A group with members and goals (shows the organization fields).',
  'Magic / Psionics' => 'How magic or psychic powers work.',
  'Science' => 'Science and natural laws of a world.',
  'Technology' => 'Machines, devices and what they can do.',
  'Historical Event' => 'Something that happened in a world\'s past.',
  'Cultural Note' => 'Customs, beliefs, food, art and daily life.',
  'Ship' => 'A ship, starship or other vessel.',
  'Mystery / Oddity' => 'An unexplained thing, left for stories to uncover.',
];
$term_descriptions['ash_test'] = [
  'Department' => 'Quizzes for joining a department.',
  'Mediator Training' => 'The quiz for becoming a mediator.',
  'Newbie Helper' => 'The quiz for helping new members.',
  'No category' => 'Quizzes that fit no other category.',
  'Setting walkthrough' => 'The new-player walkthrough of the worlds.',
  'Social walkthrough' => 'The new-player walkthrough of the community.',
  'System walkthrough' => 'The new-player walkthrough of the game system.',
];
$term_descriptions['technology_languages'] = [
  'Machine code' => 'Talking to computers in their own terms.',
  'Corporate legacy systems' => 'Old, proprietary systems corporations never retired.',
  'Gate protocols' => 'The control systems of jump gates and their traffic.',
  'Military encryption' => 'Secure military communications and their ciphers.',
  'Alien cipher' => 'Machine languages not made by humans.',
];

// Weapon classes: [percentage, multi-action modifier].
$weapon_values = [
  'Heavy (75% / 10)' => [75, 10],
  'Light (15% / -10)' => [15, -10],
  'Medium (45% / 0)' => [45, 0],
  'Unarmed (0% / 0)' => [0, 0],
];

// Forums, parents before children ("Parent / Child" paths). Grants: v(iew) u(pdate) d(elete) c(reate) per role.
$forums = [
  [
    'name' => 'General discussion',
    'parent' => '',
    'weight' => 0,
    'description' => 'General topics, questions about the site and off-topic conversations.',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'player' => 'c',
    ],
  ],
  [
    'name' => 'Feedback and Questions',
    'parent' => '',
    'weight' => 1,
    'description' => 'If you have feedback on our wiki or forums, or just aren\'t sure where to put a question about the site - this is the place! This is where new players can pop in and post their question or feedback to improve navigation. Questions asked, answers given.',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'vc',
      'lead' => 'vudc',
      'new_account' => 'v',
      'player' => 'vc',
    ],
  ],
  [
    'name' => 'Policies',
    'parent' => '',
    'weight' => 2,
    'description' => 'Site policy questions and changes, elections and their results and any administrative stuff outside of departments.',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'player' => 'c',
    ],
  ],
  [
    'name' => 'Session announcements',
    'parent' => '',
    'weight' => 3,
    'description' => 'If you have a session coming up! Post it here as an event so it will show up on the calendar!',
    'grants' => [
      'administrator' => 'vc',
      'anonymous' => 'v',
      'authenticated' => 'v',
      'player' => 'c',
      'storyteller' => 'c',
      'storyteller_assistant' => 'c',
      'storyteller_candidate' => 'c',
      'storyteller_newbie' => 'c',
    ],
  ],
  [
    'name' => '{realm}',
    'parent' => '',
    'weight' => 4,
    'description' => 'Forums for the {realm} realm ({world}): in character, SH requests, session reports and news.',
    'container' => 1,
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'vc',
    ],
  ],
  [
    'name' => 'Storyhosting',
    'parent' => '',
    'weight' => 6,
    'description' => 'Players - spoilers may be on any of these forums, read with caution.',
    'container' => 1,
    'grants' => [
      'administrator' => 'vc',
      'anonymous' => 'vc',
      'authenticated' => 'vc',
      'player' => 'vc',
    ],
  ],
  [
    'name' => 'Departments',
    'parent' => '',
    'weight' => 7,
    'description' => 'Department forums - you will need to be a member of the department to post on these forums.',
    'container' => 1,
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'vc',
    ],
  ],
  [
    'name' => 'Bugs and Fixes',
    'parent' => '',
    'weight' => 8,
    'description' => 'Bugs and Fixes',
    'container' => 1,
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'vc',
    ],
  ],
  [
    'name' => 'News and Events',
    'parent' => '{realm} / In Character',
    'weight' => 0,
    'description' => 'And IC location for news clippings, job postings, and the current IC rumor mill.',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'player' => 'c',
    ],
  ],
  [
    'name' => 'Session Reports',
    'parent' => '{realm}',
    'weight' => 0,
    'description' => 'Session Reports',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'storyteller' => 'c',
      'storyteller_assistant' => 'c',
      'storyteller_candidate' => 'c',
      'storyteller_newbie' => 'c',
    ],
  ],
  [
    'name' => 'In Character',
    'parent' => '{realm}',
    'weight' => 1,
    'description' => 'In Character',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'player' => 'c',
    ],
  ],
  [
    'name' => 'Out of Character',
    'parent' => '{realm}',
    'weight' => 2,
    'description' => 'Out of Character',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'player' => 'c',
    ],
  ],
  [
    'name' => 'SH Requests',
    'parent' => '{realm}',
    'weight' => 3,
    'description' => 'SH Requests',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'player' => 'c',
    ],
  ],
  [
    'name' => 'Mailer',
    'parent' => 'Policies',
    'weight' => 0,
    'description' => 'The big stuff! This is used for specific handling of grievances and anon mailers. Anything else should go under the policies forum.',
    'grants' => [
      'administrator' => 'c',
      'anonymous' => 'v',
      'authenticated' => 'v',
      'lead' => 'c',
    ],
  ],
  [
    'name' => 'Leads Discussion',
    'parent' => 'Storyhosting',
    'weight' => 0,
    'description' => 'A private forum for lead discussion - so that records are available to the next years worth of leads.',
    'grants' => [
      'administrator' => 'vc',
      'lead' => 'vc',
    ],
  ],
  [
    'name' => 'Storyhosting Questions',
    'parent' => 'Storyhosting',
    'weight' => 0,
    'description' => 'Ask your A/SH questions here: How to plan a session, how to handle players, advice and the like.',
    'grants' => [
      'administrator' => 'c',
      'anonymous' => 'v',
      'authenticated' => 'v',
      'storyteller' => 'c',
      'storyteller_assistant' => 'c',
      'storyteller_candidate' => 'c',
      'storyteller_newbie' => 'c',
    ],
  ],
  [
    'name' => 'Scene Forums',
    'parent' => 'Storyhosting',
    'weight' => 1,
    'description' => 'The scene forums are meant as an easy to browse way to track scenes as they are submitted to the system.',
    'grants' => [
      'administrator' => 'c',
      'lead' => 'u',
      'player' => 'vc',
      'storyteller' => 'u',
      'storyteller_assistant' => 'u',
      'storyteller_candidate' => 'u',
    ],
  ],
  [
    'name' => 'Technical Requests',
    'parent' => 'Bugs and Fixes',
    'weight' => 0,
    'description' => 'Bugs, feature requests, all that stuff goes here.',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'vc',
      'lead' => 'vudc',
      'new_account' => 'v',
      'player' => 'vc',
    ],
  ],
  [
    'name' => 'Resolved',
    'parent' => 'Bugs and Fixes',
    'weight' => 1,
    'description' => 'Resolved bugs / requests. Threads that have been answered, fixed or the like.',
    'grants' => [
      'administrator' => 'vc',
      'authenticated' => 'v',
    ],
  ],
  [
    'name' => 'Mediators',
    'parent' => 'Departments / Social Department',
    'weight' => 0,
    'description' => 'Mediation conversations need to occur on this forum so future mediators can see the history of previous conversations as needed. The mediation forums are NOT open for viewing by anyone but Mediators and Leads (keeping interested parties in the room only). This is the ONLY forum on site that is not publicly readable.',
    'grants' => [
      'administrator' => 'vc',
      'lead' => 'vc',
      'mediator' => 'vc',
    ],
  ],
  [
    'name' => 'Setting Department',
    'parent' => 'Departments',
    'weight' => 0,
    'description' => 'Setting department forums. If you would like to post on these forums you will need to become a member of the Setting department. If you would like to do that please take (and pass) the <a href="/quizzes/new-player-setting-walkthrough">Setting training walkthrough</a>.',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'knows_the_setting' => 'c',
      'lead' => 'c',
    ],
  ],
  [
    'name' => 'Social Department',
    'parent' => 'Departments',
    'weight' => 1,
    'description' => 'Social department forums. If you would like to post on these forums you will need to become a member of the Social department. If you would like to do that please take (and pass) the <a href="/quizzes/new-player-social-walkthrough">Social training walkthrough</a>',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'knows_the_social' => 'c',
      'lead' => 'c',
    ],
  ],
  [
    'name' => 'System Department',
    'parent' => 'Departments',
    'weight' => 2,
    'description' => 'System department forums. If you would like to post on these forums you will need to become a member of the System department. If you would like to do that please take (and pass) the <a href="/quizzes/new-player-system-walkthrough">System training walkthrough</a>.',
    'grants' => [
      'anonymous' => 'v',
      'authenticated' => 'v',
      'knows_the_system' => 'c',
      'lead' => 'c',
    ],
  ],
];


$etm = \Drupal::entityTypeManager();
$term_storage = $etm->getStorage('taxonomy_term');
$made = [];
$count = function (string $what) use (&$made) {
  $made[$what] = ($made[$what] ?? 0) + 1;
};
// "The Meridian Reach" => "the-meridian-reach".
$slug = fn(string $text) => trim(preg_replace('/[^a-z0-9]+/', '-', strtolower($text)), '-');
$text = fn(string $body) => ['value' => trim($body), 'format' => 'wiki'];

// Terms, found by name or created.
$term = function (string $vid, string $name, array $values = []) use ($term_storage, $count) {
  $found = $term_storage->loadByProperties(['vid' => $vid, 'name' => $name] + (isset($values['parent']) ? ['parent' => $values['parent']] : []));
  if ($found) {
    return reset($found);
  }
  $new = Term::create(['vid' => $vid, 'name' => $name] + $values);
  $new->save();
  $count('terms');
  return $new;
};
foreach ($terms as $vid => $names) {
  foreach ($names as $weight => $name) {
    $values = ['weight' => $weight];
    if ($vid === 'weapon_category') {
      $values += ['field_percentage' => $weapon_values[$name][0], 'field_multi_action_mod' => $weapon_values[$name][1]];
    }
    $made_term = $term($vid, $name, $values);
    // Descriptions, also for terms an earlier run made without one.
    if (isset($term_descriptions[$vid][$name]) && trim((string) $made_term->getDescription()) === '') {
      $made_term->setDescription($term_descriptions[$vid][$name])->setFormat('plain_text')->save();
    }
  }
}
$admin_realm = $realms_data['admin_realm']['name'];
$realm_tid = [$admin_realm => (int) $term('realm', $admin_realm, ['weight' => 0, 'description' => ['value' => $realms_data['admin_realm']['description'], 'format' => 'plain_text']])->id()];
foreach ($realms_data['realms'] as $weight => $realm) {
  $realm_tid[$realm['name']] = (int) $term('realm', $realm['name'], ['weight' => $weight + 1, 'description' => ['value' => $realm['description'], 'format' => 'plain_text']])->id();
}

// Forums, with their grants. "{realm}" forums (a container and its
// subforums) are repeated for each playable realm.
$realm_forums = [];
foreach ($forums as $forum) {
  if (!str_contains($forum['name'] . $forum['parent'], '{realm}')) {
    $realm_forums[] = $forum;
    continue;
  }
  foreach ($realms_data['realms'] as $i => $realm) {
    $copy = $forum;
    foreach (['name', 'parent', 'description'] as $key) {
      $copy[$key] = str_replace(['{realm}', '{world}'], [$realm['name'], $realm['world']], $forum[$key]);
    }
    if ($forum['name'] === '{realm}') {
      $copy['weight'] = $forum['weight'] + $i;
    }
    $realm_forums[] = $copy;
  }
}
usort($realm_forums, fn($a, $b) => ($a['parent'] === '' ? 0 : substr_count($a['parent'], ' / ') + 1) <=> ($b['parent'] === '' ? 0 : substr_count($b['parent'], ' / ') + 1));
$forum_tids = [];
$db = \Drupal::database();
foreach ($realm_forums as $forum) {
  $path = ($forum['parent'] === '' ? '' : $forum['parent'] . ' / ') . $forum['name'];
  $values = ['weight' => $forum['weight'], 'description' => ['value' => $forum['description'], 'format' => 'plain_text'], 'parent' => $forum['parent'] === '' ? 0 : $forum_tids[$forum['parent']]];
  if (!empty($forum['container'])) {
    $values['forum_container'] = 1;
  }
  $tid = (int) $term('forums', $forum['name'], $values)->id();
  $forum_tids[$path] = $tid;
  // Grants for forums that have none yet: a re-run keeps access an admin
  // changed (review, 2026-11-26).
  $granted = (int) $db->select('vaxia_forum_access', 'a')->condition('a.tid', $tid)->countQuery()->execute()->fetchField();
  foreach ($granted ? [] : $forum['grants'] ?? [] as $role => $grants) {
    $db->merge('vaxia_forum_access')->keys(['tid' => $tid, 'role' => $role])->fields([
      'grant_view' => (int) str_contains($grants, 'v'),
      'grant_update' => (int) str_contains($grants, 'u'),
      'grant_delete' => (int) str_contains($grants, 'd'),
      'grant_create' => (int) str_contains($grants, 'c'),
    ])->execute();
  }
}

// Content, found by its path alias or created with it.
$node = function (string $type, string $title, string $alias, array $values = []) use ($count) {
  $path = \Drupal::service('path_alias.manager')->getPathByAlias($alias);
  if (preg_match('#^/node/(\d+)$#', $path, $m) && ($existing = Node::load($m[1]))) {
    return $existing;
  }
  $new = Node::create(['type' => $type, 'title' => $title, 'uid' => 1, 'status' => 1, 'path' => ['alias' => $alias]] + $values);
  $new->save();
  $count($type);
  return $new;
};

// Webforms, found by id or created.
$webform = function (string $id, string $title, array $elements, array $settings = []) use ($count) {
  if ($existing = Webform::load($id)) {
    return $existing;
  }
  $elements['actions'] = ($elements['actions'] ?? []) + ['#type' => 'webform_actions', '#title' => 'Submit button(s)'];
  $new = Webform::create(['id' => $id, 'title' => $title, 'elements' => Yaml::dump($elements, 8, 2)]);
  foreach ($settings as $key => $value) {
    $new->setSetting($key, $value);
  }
  $new->save();
  $count('webforms');
  return $new;
};

// Quizzes: a webform of questions scored by the quiz handler, on a quiz
// node, taken by members.
$quiz = function (string $id, array $quiz, string $alias) use ($webform, $node, $text) {
  $elements = [];
  $answers = [];
  $scoring = [];
  foreach ($quiz['questions'] as $i => $question) {
    $key = 'question_' . ($i + 1);
    $element = ['#title' => $question['title']];
    if (trim($question['body']) !== '') {
      $element['#description'] = (string) check_markup($question['body'], 'wiki');
      $element['#description_display'] = 'before';
    }
    if ($question['type'] === 'multichoice') {
      $options = [];
      $scores = [];
      $best = NULL;
      $best_score = 0;
      foreach ($question['answers'] as $a => $answer) {
        $option = 'a' . ($a + 1);
        $options[$option] = trim(strip_tags((string) check_markup($answer['answer'], 'wiki')));
        $scores[$option] = [(int) $answer['chosen'], (int) $answer['not_chosen']];
        if ($answer['chosen'] > $best_score) {
          $best = $option;
          $best_score = $answer['chosen'];
        }
      }
      $element['#type'] = !empty($question['multi']) ? 'checkboxes' : 'radios';
      $element['#options'] = $options;
      if (!empty($question['random'])) {
        $element['#options_randomize'] = TRUE;
      }
      $scoring[$key] = ['max_score' => (int) $question['max_score'], 'question_max' => NULL, 'multi' => !empty($question['multi']), 'boolean' => !empty($question['boolean']), 'scores' => $scores];
      if (empty($question['multi']) && $best !== NULL) {
        $answers[$key] = $best;
      }
    }
    else {
      $element['#type'] = $question['type'] === 'short_answer' ? 'textfield' : 'textarea';
      $scoring[$key] = ['max_score' => (int) $question['max_score'], 'question_max' => 10, 'free_text' => TRUE];
    }
    if (empty($quiz['allow_skipping'])) {
      $element['#required'] = TRUE;
    }
    $elements[$key] = $element;
  }
  $elements['actions'] = ['#submit__label' => 'Finish'];
  $form = $webform('quiz_' . $id, $quiz['title'], $elements, ((int) ($quiz['takes'] ?? 0)) > 0 ? ['limit_user' => (int) $quiz['takes']] : ['form_previous_submissions' => FALSE]);
  if (!$form->getHandlers()->has('vaxia_quiz_score')) {
    $form->addWebformHandler(\Drupal::service('plugin.manager.webform.handler')->createInstance('vaxia_quiz_score', [
      'id' => 'vaxia_quiz_score',
      'handler_id' => 'vaxia_quiz_score',
      'settings' => [
        'answers' => $answers,
        'pass_rate' => (int) $quiz['pass_rate'],
        'questions' => $scoring,
        'randomize' => !empty($quiz['randomize']),
        'summary_pass' => (string) ($quiz['summary_pass'] ?? ''),
        'summary_default' => (string) ($quiz['summary_default'] ?? ''),
        'takes' => (int) ($quiz['takes'] ?? 0),
      ],
    ]));
    $rules = $form->getAccessRules();
    $rules['create']['roles'] = ['authenticated'];
    $form->setAccessRules($rules)->save();
  }
  return (int) $node('quiz', $quiz['title'], $alias, ['body' => $text($quiz['body'] ?? ''), 'webform' => ['target_id' => $form->id()]])->id();
};
$course_paths = [
  'ash1' => '/quizzes/ash-test-1-story-hosting-basics',
  'ash2' => '/quizzes/ash-test-2-ruling',
  'ash2_renewal' => '/quizzes/ash-test-2-renewing-shs',
  'eval' => '/quizzes/ash-test-3-evaluation',
  'ash3' => '/quizzes/ash-test-final-exam',
];
$course = [];
foreach ($course_paths as $key => $alias) {
  $course[$key] = $quiz($key, $course_data[$key], $alias);
}
// The new-player walkthroughs and mediator training.
$walkthroughs = [
  'system' => $quiz('walkthrough_system', $walkthrough_data['walkthrough_system'], '/quizzes/new-player-system-walkthrough'),
  'setting' => $quiz('walkthrough_setting', $walkthrough_data['walkthrough_setting'], '/quizzes/new-player-setting-walkthrough'),
  'social' => $quiz('walkthrough_social', $walkthrough_data['walkthrough_social'], '/quizzes/new-player-social-walkthrough'),
];
$mediator_quiz = $quiz('mediator', $walkthrough_data['mediator'], '/quizzes/mediator-training');

// Forms.
$returning = $webform('returning_player', 'Returning Player', [
  'account_email' => ['#title' => 'Account email', '#type' => 'email', '#description' => 'What was the email address for your account?', '#title_display' => 'inline', '#required' => TRUE],
  'actions' => ['#submit__label' => 'Lookup account'],
]);
$returning_nid = (int) $node('webform', 'Returning Player', '/content/returning-player', ['body' => $text('Accounts inactive for six months go to cold storage. Enter the email address of your account to bring it back.'), 'webform' => ['target_id' => $returning->id()]])->id();
$mailer = $webform('anonymous_mailer', 'Anonymous Mailer', [
  'message' => ['#title' => 'Your message', '#type' => 'textarea', '#description' => 'Enter your message for the heads here.', '#required' => TRUE],
  'associated_files' => ['#title' => 'Associated files', '#type' => 'managed_file', '#file_extensions' => 'gif jpg png txt pdf', '#max_filesize' => '2', '#description' => 'Add any files that will help us understand what you\'re mailing about.'],
  'email' => ['#title' => 'Your email', '#type' => 'email', '#description' => 'If you would like to get an emailed copy of your message, please include your email here. The heads will not receive this information on their email.'],
  'grievance' => [
    '#title' => 'I would like to make a Grievance', '#type' => 'details', '#open' => FALSE,
    'grievance_directions' => ['#type' => 'processed_text', '#admin_title' => 'Grievance directions', '#format' => 'wiki', '#text' => 'Grievances are used ONLY when a site member has abused a position of power and needs to be removed from position. A grievance is handled publicly. You will need to provide specific and verifiable evidence for each type of violation selected, and name the site member(s) whose position(s) need to be evaluated.'],
    'grievance_request' => ['#title' => 'Is this a grievance request?', '#type' => 'radios', '#options' => ['no' => 'This is NOT a grievance request.', 'yes' => 'This is a grievance request.'], '#default_value' => 'no', '#description' => 'By marking this as a Grievance, you accept the risk that you may not be able to remain anonymous over the course of the Grievance process.'],
    'grievance_violation' => ['#title' => 'What sort of grievance is it?', '#type' => 'checkboxes', '#options' => ['3F_fun' => 'Fun', '3F_free' => 'Freedom', '3F_fair' => 'Fairness', 'transparency' => 'Transparency', 'origin_power' => 'Origin of Power', 'site_process' => 'Site Governance Procedure', 'depart_policy' => 'Department Policy', 'depart_process' => 'Department Procedure'], '#description' => 'Select the guiding principle or site policy that has been violated.'],
    'grievance_names' => ['#title' => 'Who are you naming in this grievance?', '#type' => 'textfield', '#description' => 'List the site members whose position on the site needs to be reconsidered.'],
  ],
  'actions' => ['#submit__label' => 'Send message'],
]);
$mailer_nid = (int) $node('webform', 'Anonymous Mailer', '/anonymous-mailer', ['body' => $text('Write to the site\'s leads without giving your name.'), 'webform' => ['target_id' => $mailer->id()]])->id();
foreach (['session' => ['Session Feedback', '/content/session-feedback'], 'storyhost' => ['Storyhost Feedback', '/content/storyhost-feedback']] as $id => [$title, $alias]) {
  $form = $webform($id . '_feedback', $title, [
    'storyhost' => ['#title' => 'Storyhost', '#type' => 'textfield', '#default_value' => '[current-page:query:storyhost:clear]'],
    'feedback' => ['#title' => 'Your feedback', '#type' => 'textarea', '#required' => TRUE],
  ]);
  $feedback_nids[$id] = (int) $node('webform', $title, $alias, ['body' => $text('How did it go? Storyhosts read every response.'), 'webform' => ['target_id' => $form->id()]])->id();
}

// Department "Join" forms: players who passed the department's walkthrough
// in the last two weeks join it for a year by submitting one.
$join_forms = [];
foreach (['social' => 'Social', 'setting' => 'Setting', 'system' => 'System'] as $key => $name) {
  $form = $webform('join_' . $key . '_department', 'Join ' . $name . ' Department', ['actions' => ['#submit__label' => 'Join the ' . $name . ' department']]);
  $join_forms[$key] = (int) $node('webform', 'Join ' . $name . ' Department', '/content/join-' . $key . '-department', ['body' => $text('Do you want to join the ' . $name . ' department? Great! We can always use more department members to act as advice givers and backup support for the department leads. Pass the [url=/quizzes/new-player-' . $key . '-walkthrough]' . $name . ' walkthrough[/url] first; then, within two weeks, join here:'), 'webform' => ['target_id' => $form->id()]])->id();
}

// Rooms: Limbo (out of character, no XP, "I'm bored" there) and each
// realm's three.
$room = fn(string $title, string $realm, string $description) => (int) $node('rpg_chatroom', $title, '/locations/' . $slug($title), [
  'field_room_description' => $text($description),
  'field_tag_realm' => $realm_tid[$realm],
  'field_locked_to_users' => 0,
  'field_mature_posting' => 0,
  'field_private_room' => 1,
])->id();
$limbo = [$room('Limbo', $admin_realm, 'The out-of-character room: new members arrive here, and anyone can chat without a character. Posts here earn no XP.')];
// A welcome as Limbo's first post (user, 2026-11-26).
if (!\Drupal::entityQuery('comment')->accessCheck(FALSE)->condition('entity_id', $limbo[0])->condition('entity_type', 'node')->count()->execute()) {
  \Drupal\comment\Entity\Comment::create([
    'entity_type' => 'node', 'entity_id' => $limbo[0], 'field_name' => 'comment_node_rpg_chatroom', 'comment_type' => 'comment_node_rpg_chatroom',
    'uid' => 1, 'subject' => 'Welcome', 'status' => 1,
    'comment_body' => ['value' => trim($pages_data['limbo_welcome']), 'format' => 'filtered_html'],
    'field_comment_narrative' => 0,
  ])->save();
  $count('welcome posts');
}

// Pages: the front page and the system and community pages.
// Wiki pages: open for members to edit, or locked (staff only) for the
// system and policy pages.
$page = function (string $title, string $alias, string $body, ?string $realm, bool $locked = FALSE) use ($node, $text, $realm_tid) {
  $wiki = $node('wiki_page', $title, $alias, ['body' => $text($body)] + ($realm ? ['field_tag_realm' => $realm_tid[$realm]] : []));
  if (!in_array(Character::state($wiki), ['wiki_workflow_open', 'wiki_workflow_locked'], TRUE)) {
    Character::forceState($wiki, $locked ? 'wiki_workflow_locked' : 'wiki_workflow_open', 'Starter content.');
  }
  return (int) $wiki->id();
};
$front = $page($pages_data['front']['title'], $pages_data['front']['path'], $pages_data['front']['body'], NULL);
foreach ($pages_data['pages'] as $p) {
  $page($p['title'], $p['path'], $p['body'], ($p['realm'] ?? '') === 'admin' ? $admin_realm : NULL, ($p['realm'] ?? '') === 'admin');
}

$verb = fn(string $name) => (int) $term('skill_verbs', $name)->id();
// Each realm: setting and skills pages, rooms, shop items, an example
// character carrying some of them.
$approved_item = \Drupal::config('vaxia_items.settings')->get('items_approved_workflow');
foreach ($realms_data['realms'] as $realm) {
  $name = $realm['name'];
  $world = $slug($realm['world']);
  $skills = "Example skills for " . $realm['world'] . ". Skills are yours to write (see [url=/wiki/skills-and-aspects]Skills and Aspects[/url]); these show the shape.\n\n[list]\n";
  foreach ($realm['skills'] as [$skill, $about, $aspects]) {
    $skills .= "[*][b]" . $skill . "[/b] (" . implode(', ', $aspects) . "): " . $about . "\n";
  }
  $skills .= "[/list]\n\nSee " . $realm['character']['name'] . " " . $realm['character']['last_name'] . " in the character lists for a finished sheet.";
  // Its common skills, for the character form's skill picker.
  foreach ($realm['skills'] as $weight => [$skill, $about, $aspects]) {
    $exists = \Drupal::entityQuery('taxonomy_term')->accessCheck(FALSE)->condition('vid', 'skill_presets')->condition('name', $skill)->condition('field_preset_realm', $realm_tid[$name])->count()->execute();
    if (!$exists) {
      Term::create([
        'vid' => 'skill_presets', 'name' => $skill, 'weight' => $weight,
        'description' => ['value' => $about, 'format' => 'plain_text'],
        'field_preset_realm' => $realm_tid[$name],
        'field_preset_aspects' => array_map($verb, $aspects),
      ])->save();
      $count('skill presets');
    }
  }
  $page($realm['world'], '/wiki/' . $world, $realm['setting'], $name);
  $page($realm['world'] . ': Skills', '/wiki/' . $world . '/skills', $skills, $name);
  foreach ($realm['rooms'] as $r) {
    $room_nid = $room($r['name'], $name, $r['description']);
    // An opening narration, so the room has a scene to answer (and "I'm
    // bored." can send players there: it suggests rooms with posts).
    if (!\Drupal::entityQuery('comment')->accessCheck(FALSE)->condition('entity_id', $room_nid)->condition('entity_type', 'node')->count()->execute()) {
      \Drupal\comment\Entity\Comment::create([
        'entity_type' => 'node', 'entity_id' => $room_nid, 'field_name' => 'comment_node_rpg_chatroom', 'comment_type' => 'comment_node_rpg_chatroom',
        'uid' => 1, 'subject' => $r['name'], 'status' => 1,
        'comment_body' => ['value' => trim($r['description']), 'format' => 'filtered_html'],
        'field_comment_narrative' => 1,
      ])->save();
      $count('opening posts');
    }
  }
  foreach ($realm['suggestions'] ?? [] as $suggestion) {
    if (!\Drupal::entityQuery('node')->accessCheck(FALSE)->condition('type', 'play_suggestions')->condition('title', $suggestion)->count()->execute()) {
      Node::create(['type' => 'play_suggestions', 'title' => $suggestion, 'uid' => 1, 'status' => 1, 'field_tag_realm' => $realm_tid[$name]])->save();
      $count('play_suggestions');
    }
  }
  $shop_items = [];
  foreach ($realm['items'] as $i) {
    $values = [
      'field_item_desc' => $text($i['description']),
      'field_item_type' => (int) $term('item_type', $i['type'])->id(),
      'field_tag_realm' => $realm_tid[$name],
      'field_in_shop' => 1,
      'field_sellback' => 1,
      'field_calc_value' => 1,
    ];
    if (!empty($i['class'])) {
      $values['field_weapon_class'] = (int) $term('weapon_category', $i['class'])->id();
    }
    if (!empty($i['sub_type'])) {
      $values['field_weapon_sub_type'] = (int) $term('weapon_sub_category', $i['sub_type'])->id();
    }
    foreach ($i['bonuses'] ?? [] as $stat => $bonus) {
      $values['field_item_' . $stat] = $bonus;
    }
    foreach (['absorbs' => 'field_armor_absorbs', 'absorb_type' => 'field_absorb_type', 'pp' => 'field_pp_created', 'use' => 'field_item_use_day', 'uses' => 'field_item_uses', 'build_points' => 'field_build_points'] as $key => $field) {
      if (isset($i[$key])) {
        $values[$field] = $i[$key];
      }
    }
    $item = $node('items', $i['name'], '/items/' . $world . '/' . $slug($i['name']), $values);
    // New (or left unapproved by a failed run): approve. A re-run keeps a
    // state an admin set (review, 2026-11-26).
    $item_state = (string) Character::state($item);
    if ($item_state === '' || str_ends_with($item_state, '_creation')) {
      Character::forceState($item, $approved_item, 'Starter content.');
    }
    $shop_items[$i['name']] = Node::load($item->id());
  }
  // The example character (an approved, open NPC), then the stock NPCs
  // (open, generic NPCs named for their role, as stand-ins for common
  // encounters; user, 2026-11-26).
  $sheets = [$realm['character'] + ['generic' => FALSE, 'alias' => '/characters/' . $slug($realm['character']['name'] . ' ' . $realm['character']['last_name'])]];
  foreach ($realm['npcs'] ?? [] as $npc) {
    // The same role can be in several realms: the world keeps them apart.
    $sheets[] = $npc + ['generic' => TRUE, 'alias' => '/characters/' . $world . '/' . $slug($npc['name'])];
  }
  foreach ($sheets as $c) {
    $alias = $c['alias'];
    $existing = \Drupal::service('path_alias.manager')->getPathByAlias($alias) !== $alias;
    $paragraphs = [];
    if (!$existing) {
      foreach ($c['skills'] as [$skill, $value, $about, $aspects]) {
        $paragraph = Paragraph::create(['type' => 'skill', 'field_skill_name' => $skill, 'field_skill_value' => $value, 'field_skill_desc' => $about, 'field_aspect_one' => $verb($aspects[0]), 'field_aspect_two' => $verb($aspects[1]), 'field_aspect_three' => $verb($aspects[2])]);
        $paragraph->save();
        $paragraphs[] = ['target_id' => $paragraph->id(), 'target_revision_id' => $paragraph->getRevisionId()];
      }
    }
    // Sheets allow Filtered HTML only.
    $html = fn(string $key) => isset($c[$key]) ? ['value' => $c[$key], 'format' => 'filtered_html'] : NULL;
    $values = array_filter([
      'field_last_name' => $c['last_name'] ?? NULL,
      'field_titles' => $c['titles'] ?? NULL,
      'field_sex' => $c['sex'] ?? NULL,
      'field_age_app' => isset($c['age']) ? (string) $c['age'] : NULL,
      'field_occupation' => $html('occupation'),
      'field_description' => isset($c['description']) ? [$html('description')] : NULL,
      'field_background' => $html('background'),
      'field_personality' => $html('personality'),
    ], fn($value) => $value !== NULL) + [
      'field_tag_realm' => $realm_tid[$name],
      $c['species'][0] => $c['species'][1],
      $c['languages'][0] => array_map(fn($l) => ['value' => $l], $c['languages'][1]),
      'field_economy' => (int) \Drupal::config('vaxia_core.settings')->get('vaxia_starting_econ'),
      'field_is_npc' => 1,
      'field_is_open_npc' => 1,
      'field_is_generic_npc' => (int) $c['generic'],
      'field_is_storyteller' => 0,
      'field_skill' => $paragraphs,
    ];
    foreach ($c['stats'] as $stat => $value) {
      $values['field_' . $stat] = $value;
    }
    $character = $node('character_sheet', $c['name'], $alias, $values);
    // New, or left half-made by a failed run (never approved): approve and
    // equip. A re-run keeps a state an admin set (review, 2026-11-26).
    $sheet_state = (string) Character::state($character);
    if (!$existing || $sheet_state === '' || $sheet_state === 'character_evaluation_creation') {
      Character::forceState($character, 'character_evaluation_approved', $c['generic'] ? 'Starter content: a stock NPC.' : 'Starter content: an example character.');
      $character = Node::load($character->id());
      $wearing = [];
      foreach ($c['items'] ?? [] as $item_name) {
        $copy = Shop::copy($shop_items[$item_name], 1, (int) $character->id(), FALSE, 'Starter content: ' . $c['name'] . '\'s equipment.');
        $wearing[] = ['target_id' => $copy->id()];
      }
      if ($wearing) {
        $character = Node::load($character->id());
        $character->set('field_currently_wearing', $wearing)->save();
      }
    }
  }
}

// Menus (menus.yml): the sidebar and main menus a new site would otherwise
// have empty (the original's links came with its data; user, 2026-11-26).
// Missing links only, matched by menu, parent and title.
$menus_data = Yaml::parseFile($data . 'menus.yml');
$link_storage = \Drupal::entityTypeManager()->getStorage('menu_link_content');
$links = [];
foreach ($menus_data['menus'] as $menu => $items) {
  foreach ($items as $item) {
    $links[] = [$menu, $item, NULL];
  }
}
while ($links) {
  [$menu, $item, $parent] = array_shift($links);
  $entries = [];
  if (isset($item['per_realm'])) {
    foreach ($realms_data['realms'] as $realm) {
      $entries[] = [str_replace('{realm}', $realm['name'], $item['title']), 'internal:' . str_replace('{realm}', $slug($realm['name']), $item['per_realm'])];
    }
  }
  elseif (!empty($item['heading'])) {
    $entries[] = [$item['title'], 'route:<nolink>'];
  }
  elseif (isset($item['forum'])) {
    $entries[] = [$item['title'], 'internal:/forum/' . $forum_tids[$item['forum']]];
  }
  else {
    $entries[] = [$item['title'], 'internal:' . $item['path']];
  }
  foreach ($entries as $weight_offset => [$title, $uri]) {
    $query = $link_storage->getQuery()->accessCheck(FALSE)->condition('menu_name', $menu)->condition('title', $title);
    if ($parent) {
      $query->condition('parent', $parent);
    }
    else {
      // Top level: the script saves NULL, the menu UI saves '' (review,
      // 2026-11-26: a re-run duplicated links an admin had saved).
      $query->condition($query->orConditionGroup()->notExists('parent')->condition('parent', ''));
    }
    $existing = $query->execute();
    if ($existing) {
      $link = $link_storage->load(reset($existing));
    }
    else {
      $link = MenuLinkContent::create([
        'menu_name' => $menu, 'title' => $title, 'link' => ['uri' => $uri],
        'weight' => (int) ($item['weight'] ?? 0) + $weight_offset, 'expanded' => !empty($item['children']), 'parent' => $parent,
      ]);
      $link->save();
      $count('menu links');
    }
    foreach ($item['children'] ?? [] as $child) {
      $links[] = [$menu, $child, 'menu_link_content:' . $link->uuid()];
    }
  }
}

// The site's first account (uid 1) also holds the administrator role, so
// mails and menus for administrators reach it (as on the fixtures; review,
// 2026-11-26: "drush site:install" doesn't add it).
$first = \Drupal\user\Entity\User::load(1);
if ($first && !$first->hasRole('administrator')) {
  $first->addRole('administrator');
  $first->save();
}

// Settings, filled where empty.
$factory = \Drupal::configFactory();
$fill = function (string $name, string $key, $value) use ($factory) {
  $config = $factory->getEditable($name);
  // Unset, 0, or a list of zeros (e.g. department_join_forms) is empty.
  if (!array_filter((array) $config->get($key))) {
    $config->set($key, $value)->save();
  }
};
foreach ($course as $key => $nid) {
  $fill('vaxia_storyhosts.settings', 'sh_test_quiz_' . $key, $nid);
}
// Departments and elections, as the original site ran them (review,
// 2026-11-26: shipped empty, every yearly election cancelled itself): the
// Setting, Social and System departments, each with its walkthrough role,
// and two elected leads for each (one seat per person; the original's
// "two per setting" needed more storytellers than a new site has).
$governance_config = $factory->getEditable('vaxia_governance.settings');
if ((string) $governance_config->get('departments') === '' && (string) $governance_config->get('election_positions') === '') {
  $governance_config
    ->set('departments', "Setting\nSocial\nSystem")
    ->set('department_roles', ['setting' => 'knows_the_setting', 'social' => 'knows_the_social', 'system' => 'knows_the_system'])
    ->set('election_positions', "Setting Lead\nSocial Lead\nSystem Lead")
    ->set('election_position_counts', ['setting_lead' => 2, 'social_lead' => 2, 'system_lead' => 2])
    ->set('election_position_descs', [
      'setting_lead' => '<b>Setting:</b> The setting department is responsible for the development and maintenance of the game settings of this site. It creates and maintains documentation on the settings, stored publicly in the wiki, and provides training on setting details. It also approves world-changing sessions and sagas proposed by storyhosts and resolves any consistency issues between existing plots and sessions. The setting department confirms whether a thing exists in a setting, but is not responsible for the system mechanics for it.',
      'social_lead' => '<b>Social:</b> The social department is responsible for maintaining the health of the community. This includes moderation duties, interpersonal conflict resolution, communication assistance, helping newbies, creating and maintaining site and newbie tutorials and running community activities. In addition, the social department may act as a neutral party in disputes, assuming the department itself is not in conflict of interest.',
      'system_lead' => '<b>System:</b> The system department is responsible for the game mechanics and overall game balance of the system as implemented in the settings of this site. This includes creating and maintaining documentation on rules, training new ASHs and keeping existing SHs trained regularly, as well as maintaining the Apprentice Storyhost Course. The system department implements numbers and rules for in-game elements the setting department has confirmed exist, but does not confirm their existence in the setting.',
    ])
    ->set('election_role', 'lead')
    ->set('election_run_roles', ['storyteller' => 'storyteller'])
    ->set('election_default_forum', $forum_tids['Policies'])
    ->save();
  // The "Eligible for <position>" user fields (the election settings form
  // makes them on save).
  vaxia_governance_position_fields();
}
$fill('vaxia_storyhosts.settings', 'storyhost_feedback_form', $feedback_nids['storyhost']);
$fill('vaxia_storyhosts.settings', 'session_feedback_form', $feedback_nids['session']);
$fill('vaxia_storyhosts.settings', 'sh_request_forums', array_values(array_map(fn($realm) => $forum_tids[$realm['name'] . ' / SH Requests'], $realms_data['realms'])));
$governance = $factory->getEditable('vaxia_governance.settings');
if (!array_filter((array) $governance->get('walkthrough_quizzes'))) {
  $governance->set('walkthrough_quizzes', $walkthroughs);
}
$apps = (array) $governance->get('role_app_settings');
foreach (['storyteller' => $course['ash3'], 'mediator' => $mediator_quiz] as $role => $nid) {
  if (isset($apps[$role]) && empty($apps[$role]['quiz'])) {
    $apps[$role]['quiz'] = [$nid];
  }
}
$governance->set('role_app_settings', $apps)->save();
$fill('vaxia_governance.settings', 'anon_mailer_webform', $mailer_nid);
$fill('vaxia_governance.settings', 'anon_mailer_form', $forum_tids['Policies / Mailer']);
$fill('vaxia_conduct.settings', 'cold_storage_return_form', $returning_nid);
$fill('vaxia_governance.settings', 'department_join_forms', $join_forms);
$motd = \Drupal::service('path_alias.manager')->getPathByAlias('/wiki/message-of-the-day');
if (preg_match('#^/node/(\d+)$#', $motd, $m)) {
  $fill('vaxia_core.settings', 'motd_page', (int) $m[1]);
}
$fill('vaxia_core.settings', 'vaxia_limbo_room', array_combine($limbo, $limbo));
$site = $factory->getEditable('system.site');
if (in_array((string) $site->get('page.front'), ['', '/node', '/user/login'], TRUE)) {
  $site->set('page.front', '/node/' . $front)->save();
}
// "I'm bored." in Limbo.
if ($block = \Drupal\block\Entity\Block::load('vaxia_theme_play_suggestions')) {
  $visibility = $block->getVisibility();
  if (empty($visibility['request_path']['pages'])) {
    $visibility['request_path'] = ['id' => 'request_path', 'negate' => FALSE, 'pages' => implode("\n", array_map(fn($nid) => '/node/' . $nid, $limbo))];
    $block->set('visibility', $visibility)->enable()->save();
  }
}

// Forum grants take effect on content through node access records.
\Drupal::service(\Drupal\node\NodeAccessRebuild::class)->rebuild();
\Drupal::state()->set('vaxia_core.starter_content', \Drupal::time()->getRequestTime());
foreach ($made as $what => $n) {
  print $what . ': ' . $n . "\n";
}
print "Starter content ready. Review the policy pages before opening the site.\n";
