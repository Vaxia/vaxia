<?php

namespace Vaxia\Tests;

use Drupal\comment\Entity\Comment;
use Drupal\Core\Session\AnonymousUserSession;
use Drupal\node\Entity\Node;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\taxonomy\Entity\Term;
use Drupal\user\Entity\Role;
use Drupal\user\Entity\User;

/**
 * The setup driver for the Drupal 11 site: the ONLY place tests may touch
 * Drupal internals. Same methods and return shapes as src/d7/Site.php, so the
 * tests don't change.
 *
 * Methods for D7 modules that aren't ported yet throw a LogicException naming
 * the module; each is implemented when its module is ported (Phase 4).
 */
class Site {

  /** @var int Counter for unique names. */
  private static $seq = 0;

  public function uniqueName($prefix = 'vt') {
    return $prefix . '_' . getmypid() . '_' . str_pad((string) (++self::$seq), 5, '0', STR_PAD_LEFT);
  }

  /**
   * Fails a test that needs a D7 module the port doesn't have yet.
   */
  private function notPorted($what, $module) {
    throw new \LogicException($what . ': arrives in Phase 4 with the port of ' . $module . '.');
  }

  // ---------------------------------------------------------------------------
  // Accounts.
  // ---------------------------------------------------------------------------

  /**
   * Creates an account. Same arguments as on D7: role names (labels),
   * extra permissions (through a throwaway role), and extra properties with
   * profile fields as 'field_x' => scalar value.
   *
   * @return object
   *   Account with uid, name, mail and pass_raw.
   */
  public function createUser(array $roles = array(), array $permissions = array(), array $values = array()) {
    $name = $values['name'] ?? $this->uniqueName('user');
    $rids = array();
    foreach ($roles as $roleName) {
      $rids[] = $this->roleId($roleName);
    }
    if ($permissions) {
      $rids[] = $this->roleId($this->createRole());
      $this->grantPermissions(end($rids), $permissions);
    }
    // Fields the site added since this process looked (e.g. election opt-ins).
    $known = \Drupal::service('entity_field.manager')->getFieldDefinitions('user', 'user');
    foreach (array_keys($values) as $key) {
      if (strpos($key, 'field_') === 0 && !isset($known[$key])) {
        \Drupal::service('entity_field.manager')->clearCachedFieldDefinitions();
        break;
      }
    }
    $account = User::create(array(
      'name' => $name,
      'mail' => $values['mail'] ?? $name . '@example.test',
      'pass' => 'pass',
      'status' => $values['status'] ?? 1,
      'roles' => $rids,
    ));
    if (isset($values['created'])) {
      $account->set('created', $values['created']);
    }
    // Registered players have ticked both required boxes.
    $values += array('field_age_requirement' => 1, 'field_terms_and_conditions' => 1);
    $this->setFields($account, $values);
    $account->save();
    $this->resetCaches();
    return (object) array('uid' => (int) $account->id(), 'name' => $account->getAccountName(), 'mail' => $account->getEmail(), 'pass_raw' => 'pass');
  }

  /**
   * D11 names for permissions written the D7 way (privatemsg's are renamed).
   */
  private function permissionNames(array $permissions) {
    static $renamed;
    $renamed = $renamed ?? require __DIR__ . '/renamed-permissions.php';
    return array_values(array_unique(array_map(function ($p) use ($renamed) {
      return $renamed[$p] ?? $p;
    }, $permissions)));
  }

  public function grantPermissions($roleName, array $permissions) {
    user_role_grant_permissions($this->roleId($roleName), $this->permissionNames($permissions));
  }

  public function revokePermissions($roleName, array $permissions) {
    user_role_revoke_permissions($this->roleId($roleName), $this->permissionNames($permissions));
  }

  public function roleHas($roleName, $permission) {
    $role = Role::load($this->roleId($roleName));
    return $role->hasPermission($this->permissionNames(array($permission))[0]);
  }

  /**
   * Creates a role; returns its name (label), as on D7.
   */
  public function createRole($name = NULL) {
    $name = $name ?: $this->uniqueName('role');
    $id = substr(preg_replace('/[^a-z0-9_]+/', '_', strtolower($name)), 0, 32);
    Role::create(array('id' => $id, 'label' => $name))->save();
    return $name;
  }

  public function addRole($account, $roleName) {
    $user = User::load($account->uid);
    $user->addRole($this->roleId($roleName));
    $user->save();
    $this->resetCaches();
  }

  public function removeRole($account, $roleName) {
    $user = User::load($account->uid);
    $user->removeRole($this->roleId($roleName));
    $user->save();
    $this->resetCaches();
  }

  public function removeRoleFromEveryone($roleName) {
    \Drupal::database()->delete('user__roles')->condition('roles_target_id', $this->roleId($roleName))->execute();
    $this->resetCaches();
  }

  /**
   * A role's id from its name. D7 names are D11 labels ("new account" is
   * new_account); an id works too.
   */
  public function roleId($roleName) {
    if ($roleName === 'authenticated user') {
      return 'authenticated';
    }
    if ($roleName === 'anonymous user') {
      return 'anonymous';
    }
    foreach (Role::loadMultiple() as $role) {
      if ($role->label() === $roleName || $role->id() === $roleName) {
        return $role->id();
      }
    }
    throw new \InvalidArgumentException("No role '$roleName'");
  }

  public function findUser($name) {
    $account = user_load_by_name($name);
    return $account ? (object) array('uid' => (int) $account->id(), 'name' => $account->getAccountName(), 'mail' => $account->getEmail()) : NULL;
  }

  public function setUserFields($account, array $values) {
    $user = User::load($account->uid);
    $this->setFields($user, $values);
    $user->save();
    $this->resetCaches();
  }

  // ---------------------------------------------------------------------------
  // Content.
  // ---------------------------------------------------------------------------

  /**
   * Creates a node. Field values as scalars ('field_x' => 5) or D7-style items.
   */
  public function createNode($type, array $values = array()) {
    if (isset($values['webform']) && is_array($values['webform']) && !isset($values['webform']['target_id'])) {
      $settings = $values['webform'];
      $values['webform'] = array(
        'target_id' => $this->makeWebform($values['title'] ?? $type, $settings['roles'] ?? array(), $settings['components'] ?? array()),
        'status' => 'open',
      );
    }
    $node = Node::create(array(
      'type' => $type,
      'title' => $values['title'] ?? $this->uniqueName($type),
      'uid' => $values['uid'] ?? 1,
      'status' => $values['status'] ?? 1,
    ) + (isset($values['nid']) ? array('nid' => $values['nid']) : array()));
    $fields = array();
    foreach ($values as $key => $value) {
      if (!in_array($key, array('title', 'uid', 'status', 'comment', 'language', 'nid', 'is_new'), TRUE)) {
        $fields[$key] = $value;
      }
    }
    $this->setFields($node, $fields);
    // As on D7, comments are open on new content, and new content starts in
    // its workflow's creation state.
    foreach ($node->getFieldDefinitions() as $field => $definition) {
      if ($definition->getType() === 'comment' && !isset($values[$field])) {
        $node->set($field, array('status' => 2));
      }
      if ($definition->getType() === 'workflow' && $node->get($field)->isEmpty()) {
        $node->set($field, $definition->getSetting('workflow_type') . '_creation');
      }
    }
    $node->save();
    $this->resetCaches();
    return (object) array('nid' => (int) $node->id(), 'title' => $node->label(), 'uid' => (int) $node->getOwnerId(), 'type' => $type);
  }

  /**
   * Saves changes to an existing node as $account (default: its author).
   */
  public function saveNode($node, array $changes, $account = NULL) {
    $loaded = Node::load($node->nid);
    $this->setFields($loaded, $changes);
    $this->runAs($account ?: (object) array('uid' => $loaded->getOwnerId()), function () use ($loaded) {
      $loaded->save();
    });
    \Drupal::messenger()->deleteAll();
    $this->resetCaches();
  }

  public function createCharacter($owner, array $values = array()) {
    return $this->createNode('character_sheet', $values + array(
      'uid' => $owner->uid,
      'field_is_npc' => 0,
      'field_is_open_npc' => 0,
      'field_is_storyteller' => 0,
      'field_is_generic_npc' => 0,
      'field_tag_realm' => $this->term('realm', 'Vaxia'),
    ));
  }

  public function createRoom(array $values = array()) {
    return $this->createNode('rpg_chatroom', $values);
  }

  public function createNotation($player, array $values = array()) {
    return $this->createNode('notation', $values + array(
      'field_players' => array('uid' => $player->uid),
      'field_date_of_incident' => date('Y-m-d H:i:s'),
    ));
  }

  /**
   * Posts a chat comment, optionally as a character.
   */
  public function postComment($node, $author, $text, $character = NULL) {
    $loaded = Node::load($node->nid);
    $field = $this->commentField($loaded->bundle());
    $comment = Comment::create(array(
      'entity_type' => 'node',
      'entity_id' => $node->nid,
      'field_name' => $field,
      'comment_type' => $loaded->getFieldDefinition($field)->getSetting('comment_type'),
      'uid' => $author->uid,
      'name' => $author->name,
      'subject' => substr($text, 0, 20),
      'status' => 1,
      'comment_body' => array('value' => $text, 'format' => 'plain_text'),
    ));
    if ($character) {
      $comment->set('field_comment_character', $character->nid);
    }
    $comment->save();
    $this->resetCaches();
    return (object) array('cid' => (int) $comment->id(), 'nid' => (int) $node->nid);
  }

  public function createScene($author, array $comments) {
    $refs = array();
    foreach ($comments as $c) {
      $refs[] = array('target_id' => $c->cid);
    }
    return $this->createNode('scene', array('uid' => $author->uid, 'field_rp_posts' => $refs));
  }

  /**
   * Adds a skill (a "skill" paragraph; a field collection on D7) to a
   * character. Returns the skill's id.
   */
  public function addSkill($character, $name, $value, array $aspects = array()) {
    $values = array('type' => 'skill', 'field_skill_name' => $name, 'field_skill_value' => $value);
    foreach (array_values($aspects) as $i => $aspect) {
      $values[array('field_aspect_one', 'field_aspect_two', 'field_aspect_three')[$i]] = $this->term('skill_verbs', $aspect);
    }
    $skill = Paragraph::create($values);
    $skill->save();
    $node = Node::load($character->nid);
    $node->get('field_skill')->appendItem($skill);
    $node->save();
    $this->resetCaches();
    return (int) $skill->id();
  }

  /**
   * Places a block in the site theme, shown only on $pages (one path per line).
   * D7 module/delta pairs map to the D11 block plugin with the delta's id
   * (numeric deltas, as favorites/0, to the plugin named after the module).
   */
  public function placeBlock($module, $delta, $region, $pages = '') {
    $manager = \Drupal::service('plugin.manager.block');
    $plugin = $manager->hasDefinition($delta) ? $delta : (is_numeric($delta) && $manager->hasDefinition($module) ? $module : NULL);
    if (!$plugin) {
      $this->notPorted('Block ' . $module . '/' . $delta, $module);
    }
    $theme = \Drupal::config('system.theme')->get('default');
    // D7 region names on this theme (Olivero has one sidebar).
    $regions = array_keys(system_region_list($theme));
    $aliases = array('sidebar_first' => 'sidebar', 'sidebar_second' => 'sidebar');
    if (!in_array($region, $regions, TRUE) && isset($aliases[$region]) && in_array($aliases[$region], $regions, TRUE)) {
      $region = $aliases[$region];
    }
    $id = substr(preg_replace('/[^a-z0-9_]+/', '_', $theme . '_' . $module . '_' . $delta), 0, 64);
    // The test decides where this block goes: drop the site's own placement.
    foreach (\Drupal::entityTypeManager()->getStorage('block')->loadByProperties(array('theme' => $theme, 'plugin' => $plugin)) as $other) {
      $other->delete();
    }
    if ($existing = \Drupal\block\Entity\Block::load($id)) {
      $existing->delete();
    }
    $block = \Drupal\block\Entity\Block::create(array(
      'id' => $id,
      'theme' => $theme,
      'region' => $region,
      'plugin' => $plugin,
      'weight' => -46,
      'settings' => array('label_display' => '0'),
    ));
    if ($pages !== '') {
      $paths = array_map(function ($p) {
        return '/' . ltrim(trim($p), '/');
      }, explode("\n", $pages));
      $block->setVisibilityConfig('request_path', array('id' => 'request_path', 'pages' => implode("\n", $paths), 'negate' => FALSE));
    }
    $block->save();
  }

  public function deleteNodesOfType($type) {
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(FALSE)->condition('type', $type)->execute();
    $storage->delete($storage->loadMultiple($ids));
  }

  /**
   * A managed public file, as a file field item for createNode().
   */
  public function createFile($filename, $contents = 'test') {
    $file = \Drupal::service('file.repository')->writeData($contents, 'public://' . $filename);
    return array('target_id' => (int) $file->id(), 'display' => 1, 'description' => '');
  }

  /**
   * A term at a fixed term id (code that hardcodes production term ids).
   */
  public function createTermAt($vocabulary, $tid, $name) {
    if (!Term::load($tid)) {
      Term::create(array('tid' => $tid, 'vid' => $vocabulary, 'name' => $name))->save();
      $this->resetCaches();
    }
    return $tid;
  }

  public function createWeaponClass($name, $percentage, $modifier, $weight = 0) {
    $term = Term::create(array(
      'vid' => 'weapon_category', 'name' => $name, 'weight' => $weight,
      'field_percentage' => $percentage, 'field_multi_action_mod' => $modifier,
    ));
    $term->save();
    $this->resetCaches();
    return (int) $term->id();
  }

  /**
   * Term id by vocabulary and name, created if missing.
   */
  public function term($vocabulary, $name) {
    $found = \Drupal::entityTypeManager()->getStorage('taxonomy_term')->loadByProperties(array('vid' => $vocabulary, 'name' => $name));
    if ($found) {
      return (int) reset($found)->id();
    }
    $term = Term::create(array('vid' => $vocabulary, 'name' => $name));
    $term->save();
    return (int) $term->id();
  }

  // ---------------------------------------------------------------------------
  // Modules not ported yet (Phase 4).
  // ---------------------------------------------------------------------------

  /**
   * Forgets every dice roll ever made (a site that has seen none).
   */
  public function deleteDiceRolls() {
    \Drupal::database()->delete('dice_roller')->execute();
  }

  /**
   * A one-question quiz (a quiz node with a scored webform).
   */
  public function createMultichoiceQuiz($title, $question, array $alternatives) {
    $options = array();
    $correct = NULL;
    foreach (array_keys($alternatives) as $i => $answer) {
      $options[(string) $i] = $answer;
      if ($alternatives[$answer]) {
        $correct = (string) $i;
      }
    }
    $webform = \Drupal\webform\Entity\Webform::create(array(
      'id' => strtolower($this->uniqueName('quiz')),
      'title' => $title,
      'elements' => \Drupal\Core\Serialization\Yaml::encode(array(
        'question_1' => array('#type' => 'radios', '#title' => $question, '#options' => $options, '#required' => TRUE),
        'actions' => array('#type' => 'webform_actions', '#submit__label' => 'Finish'),
      )),
    ));
    $webform->addWebformHandler(\Drupal::service('plugin.manager.webform.handler')->createInstance('vaxia_quiz_score', array(
      'id' => 'vaxia_quiz_score',
      'handler_id' => 'vaxia_quiz_score',
      'settings' => array('answers' => array('question_1' => $correct), 'pass_rate' => 75),
    )));
    $webform->save();
    return $this->createNode('quiz', array('title' => $title, 'webform' => array('target_id' => $webform->id(), 'status' => 'open')));
  }

  /**
   * A quiz scored as D7 quiz scored it. $questions: element key => array(
   *   'title' => ..., 'type' => 'radios' | 'checkboxes' | 'textarea',
   *   'options' => array(label => array(score if chosen, score if not chosen)),
   *   'max_score' => its points in the quiz (default: its own maximum),
   *   'question_max' => its own maximum (default: from the options),
   *   'boolean' => all or nothing,
   * ). Options are keyed 0, 1, ... in order. $settings: pass_rate, randomize,
   * summary_pass, summary_default, takes.
   */
  public function createScoredQuiz($title, array $questions, array $settings = array()) {
    $elements = array();
    $scoring = array();
    $answers = array();
    foreach ($questions as $key => $question) {
      $type = $question['type'] ?? 'radios';
      $elements[$key] = array('#type' => $type, '#title' => $question['title'] ?? $key);
      if ($type === 'textarea') {
        $scoring[$key] = array('free_text' => TRUE, 'question_max' => $question['question_max'] ?? 10) + (isset($question['max_score']) ? array('max_score' => $question['max_score']) : array());
        continue;
      }
      $options = array();
      $scores = array();
      $best = NULL;
      foreach (array_keys($question['options']) as $i => $label) {
        $options[(string) $i] = $label;
        $scores[(string) $i] = $question['options'][$label];
        if ($question['options'][$label][0] > 0 && ($best === NULL || $question['options'][$label][0] > $scores[$best][0])) {
          $best = (string) $i;
        }
      }
      $elements[$key]['#options'] = $options;
      $scoring[$key] = array('multi' => $type === 'checkboxes', 'boolean' => !empty($question['boolean']), 'scores' => $scores)
        + (isset($question['max_score']) ? array('max_score' => $question['max_score']) : array())
        + (isset($question['question_max']) ? array('question_max' => $question['question_max']) : array());
      if ($type === 'radios' && $best !== NULL) {
        $answers[$key] = $best;
      }
    }
    $elements['actions'] = array('#type' => 'webform_actions', '#submit__label' => 'Finish');
    $webform = \Drupal\webform\Entity\Webform::create(array(
      'id' => strtolower($this->uniqueName('quiz')),
      'title' => $title,
      'elements' => \Drupal\Core\Serialization\Yaml::encode($elements),
    ));
    $webform->addWebformHandler(\Drupal::service('plugin.manager.webform.handler')->createInstance('vaxia_quiz_score', array(
      'id' => 'vaxia_quiz_score',
      'handler_id' => 'vaxia_quiz_score',
      'settings' => array('answers' => $answers, 'questions' => $scoring) + $settings + array('pass_rate' => 75),
    )));
    $webform->save();
    return $this->createNode('quiz', array('title' => $title, 'webform' => array('target_id' => $webform->id(), 'status' => 'open')));
  }

  /**
   * Recorded attempts at a quiz, oldest first: array('uid', 'score', 'passed').
   */
  public function quizAttempts($quiz) {
    $rows = array();
    foreach (\Drupal::database()->query('SELECT uid, score, passed FROM {vaxia_quiz_result} WHERE nid = :nid ORDER BY rid', array(':nid' => $quiz->nid)) as $row) {
      $rows[] = array('uid' => (int) $row->uid, 'score' => (float) $row->score, 'passed' => (int) $row->passed);
    }
    return $rows;
  }

  /**
   * Adds to a character's XP pool of a type (xp, spent_xp, rxp, bxp, kxp).
   */
  public function addCharacterXp($character, $type, $amount) {
    \Drupal::service('vaxia_core.xp')->update((int) $character->nid, (float) $amount, $type);
    $this->resetCaches();
  }

  /**
   * Adds to a character's flavour XP total of a type (physical, transfer, ...).
   */
  public function addFlavourXp($character, $type, $amount) {
    \Drupal::service('vaxia_core.xp')->addFlavour((int) $character->nid, (float) $amount, $type);
  }

  /**
   * Writes a line to a character's moderation log (character_sheet_log).
   */
  public function addSheetLog($character, $account, $type, $message, $timestamp) {
    \Drupal::database()->insert('character_sheet_log')->fields(array(
      'nid' => $character->nid,
      'uid' => $account ? $account->uid : 0,
      'type' => $type,
      'message' => $message,
      'hostname' => '127.0.0.1',
      'timestamp' => $timestamp,
    ))->execute();
  }

  /**
   * Moves every workflow history entry of a node to $timestamp.
   */
  public function backdateWorkflowHistory($node, $timestamp) {
    \Drupal::database()->update('workflow_transition_history')->fields(array('timestamp' => $timestamp))->condition('entity_type', 'node')->condition('entity_id', $node->nid)->execute();
  }

  /**
   * Records that $account last read $node at $timestamp (history module).
   */
  public function setNodeRead($account, $node, $timestamp) {
    \Drupal::database()->merge('history')->keys(array('uid' => $account->uid, 'nid' => $node->nid))->fields(array('timestamp' => $timestamp))->execute();
  }

  // ---------------------------------------------------------------------------
  // Webforms: D7 webform settings ("roles" as role ids or names, "components")
  // become a Webform 6 webform attached to the node's "webform" field.
  // ---------------------------------------------------------------------------

  /**
   * Webform 6 element for a D7 webform component.
   */
  private function webformElement(array $component) {
    $element = array('#title' => $component['name'] ?? $component['form_key']);
    $type = $component['type'] ?? 'textfield';
    if ($type === 'select') {
      $options = array();
      foreach (preg_split('/\R/', (string) ($component['extra']['items'] ?? '')) as $line) {
        if (trim($line) !== '') {
          list($key, $text) = array_pad(explode('|', $line, 2), 2, NULL);
          $options[trim($key)] = trim($text ?? $key);
        }
      }
      $element['#type'] = empty($component['extra']['aslist']) ? (empty($component['extra']['multiple']) ? 'radios' : 'checkboxes') : 'select';
      $element['#options'] = $options;
    }
    else {
      $element['#type'] = $type;
    }
    if (!empty($component['mandatory']) || !empty($component['required'])) {
      $element['#required'] = TRUE;
    }
    return $element;
  }

  /**
   * D11 role ids for D7 webform roles (1 anonymous, 2 authenticated, or names).
   */
  private function webformRoles(array $roles) {
    $ids = array();
    foreach ($roles as $role) {
      $ids[] = $role === 1 || $role === '1' ? 'anonymous' : ($role === 2 || $role === '2' ? 'authenticated' : $this->roleId($role));
    }
    return $ids ?: array('authenticated');
  }

  /**
   * Creates a webform; returns its id.
   */
  private function makeWebform($title, array $roles, array $components) {
    $elements = array();
    foreach ($components as $component) {
      $elements[$component['form_key']] = $this->webformElement($component);
    }
    $webform = \Drupal\webform\Entity\Webform::create(array(
      'id' => strtolower($this->uniqueName('webform')),
      'title' => $title,
      'elements' => \Drupal\Core\Serialization\Yaml::encode($elements),
    ));
    $rules = $webform->getAccessRules();
    $rules['create']['roles'] = $this->webformRoles($roles);
    $webform->setAccessRules($rules);
    $webform->save();
    return $webform->id();
  }

  private function nodeWebform($node) {
    $loaded = Node::load($node->nid);
    return \Drupal\webform\Entity\Webform::load($loaded->get('webform')->target_id);
  }

  public function addWebformQuestion($node, $formKey, $label, array $options, $type = 'select') {
    $items = array();
    foreach ($options as $key => $text) {
      $items[] = $key . '|' . $text;
    }
    $webform = $this->nodeWebform($node);
    $elements = $webform->getElementsDecoded();
    $elements[$formKey] = $this->webformElement(array(
      'form_key' => $formKey, 'name' => $label, 'type' => $type, 'mandatory' => 1,
      'extra' => $type === 'select' ? array('items' => implode("\n", $items), 'multiple' => 0, 'aslist' => 1) : array(),
    ));
    $webform->setElements($elements)->save();
    $this->resetCaches();
  }

  public function createWebformAt($nid, $title, array $roles, array $components) {
    if (Node::load($nid)) {
      return;
    }
    $this->createNode('webform', array('nid' => $nid, 'title' => $title, 'webform' => array('roles' => $roles, 'components' => $components)));
  }

  public function closeAllProposals() {
    foreach (Node::loadMultiple(\Drupal::entityQuery('node')->accessCheck(FALSE)->condition('type', 'proposal')->execute()) as $node) {
      $node->get('webform')->status = 'closed';
      $node->save();
    }
    $this->resetCaches();
  }

  public function setWebformOpen($node, $open) {
    $loaded = Node::load($node->nid);
    $loaded->get('webform')->status = $open ? 'open' : 'closed';
    $loaded->save();
    $this->resetCaches();
  }

  public function setWebformRoles($node, array $roleNames) {
    $webform = $this->nodeWebform($node);
    $rules = $webform->getAccessRules();
    $rules['create']['roles'] = $this->webformRoles($roleNames);
    $webform->setAccessRules($rules)->save();
    $this->resetCaches();
  }

  public function createProposal(array $roleNames, array $values = array()) {
    return $this->createNode('proposal', $values + array('webform' => array('roles' => $roleNames, 'components' => array())));
  }

  public function createWebformNode($type, array $values, array $roles = array()) {
    return $this->createNode($type, $values + array('webform' => array('roles' => $roles, 'components' => array())));
  }

  public function createWebform(array $roles, array $components) {
    return $this->createNode('webform', array('webform' => array('roles' => $roles, 'components' => $components)));
  }

  /**
   * Records a full-marks attempt by $account; returns the messages it set.
   */
  public function passQuiz($account, $quiz) {
    // Settings saved through the site since this process last looked.
    $this->resetCaches();
    $messenger = \Drupal::messenger();
    $messenger->deleteAll();
    $this->runAs($account, function () use ($quiz, $account) {
      \Drupal\vaxia_core\Quiz::record(Node::load($quiz->nid), User::load($account->uid), 100);
    });
    $messages = array();
    foreach ($messenger->deleteAll() as $list) {
      foreach ($list as $m) {
        $messages[] = strip_tags((string) $m);
      }
    }
    // Content it created (applications) shows up in cached lists.
    $this->expireViewsCache();
    $this->resetCaches();
    return $messages;
  }

  public function setVacationEnd($account, $timestamp) {
    \Drupal::database()->update('vacation')->fields(array('end' => $timestamp))->condition('uid', $account->uid)->execute();
  }

  /**
   * Records a past chat-room visit.
   */
  public function setRoomVisit($room, $account, $timestamp) {
    \Drupal::database()->merge('rpg_chat_room_users')->keys(array('nid' => $room->nid, 'uid' => $account->uid))->fields(array('timestamp' => $timestamp))->execute();
  }

  /**
   * Sets when a node last had a comment (its comment statistics).
   */
  public function setLastCommentTime($node, $timestamp) {
    \Drupal::database()->update('comment_entity_statistics')->fields(array('last_comment_timestamp' => $timestamp))->condition('entity_type', 'node')->condition('entity_id', $node->nid)->execute();
    $this->resetCaches();
  }

  public function recordElectionWin($account, $positionKey, $cycle) {
    \Drupal::database()->insert('rpg_election_winners')->fields(array('uid' => $account->uid, 'cycle' => $cycle, 'position' => $positionKey))->execute();
  }

  // ---------------------------------------------------------------------------
  // Configuration without an admin form.
  // ---------------------------------------------------------------------------

  /**
   * D7 variables with a D11 home. Module variables join this map as their
   * modules are ported.
   */
  private const VARIABLES = array(
    'cron_key' => array('state', 'system.cron_key'),
    'cron_last' => array('state', 'system.cron_last'),
    'theme_default' => array('config', 'system.theme', 'default'),
    'site_name' => array('config', 'system.site', 'name'),
    'sh_test_cron' => array('state', 'vaxia_cron.sh_test_cron'),
    // The last election day (state, so config imports keep it).
    'election_day' => array('state', 'vaxia_governance.election_day'),
    // Relaunch (D11 only): the current terms version and the terms page.
    'vaxia_terms_version' => array('state', 'vaxia_core.terms_version'),
    'vaxia_terms_path' => array('config', 'vaxia_core.relaunch', 'terms_path'),
  );

  public function setConfig($name, $value) {
    $where = $this->variable($name);
    if ($where[0] === 'state') {
      \Drupal::state()->set($where[1], $value);
    }
    else {
      \Drupal::configFactory()->getEditable($where[1])->set($where[2], $value)->save();
    }
  }

  public function deleteConfig($name) {
    $where = $this->variable($name);
    if ($where[0] === 'state') {
      \Drupal::state()->delete($where[1]);
    }
    else {
      // Back to the site's shipped value, as D7 variable_del() fell back to
      // the module default.
      $shipped = \Drupal::service('config.storage.sync')->read($where[1]) ?: array();
      $config = \Drupal::configFactory()->getEditable($where[1]);
      if (array_key_exists($where[2], $shipped)) {
        $config->set($where[2], $shipped[$where[2]])->save();
      }
      else {
        $config->clear($where[2])->save();
      }
    }
  }

  public function config($name, $default = NULL) {
    $this->resetCaches();
    $where = $this->variable($name);
    $value = $where[0] === 'state' ? \Drupal::state()->get($where[1]) : \Drupal::config($where[1])->get($where[2]);
    return $value ?? $default;
  }

  /**
   * Config objects of ported modules whose keys keep the D7 variable names.
   */
  private const CONFIG_OBJECTS = array('vaxia_core.settings', 'vaxia_dice.settings', 'vaxia_chat.settings', 'vaxia_items.settings', 'vaxia_characters.settings', 'vaxia_storyhosts.settings', 'vaxia_governance.settings', 'vaxia_conduct.settings');

  private function variable($name) {
    foreach (self::CONFIG_OBJECTS as $object) {
      if (array_key_exists($name, \Drupal::config($object)->getRawData())) {
        return array('config', $object, $name);
      }
    }
    if (!isset(self::VARIABLES[$name])) {
      $this->notPorted('D7 variable ' . $name, 'the module that owns it');
    }
    return self::VARIABLES[$name];
  }

  // ---------------------------------------------------------------------------
  // Time travel.
  // ---------------------------------------------------------------------------

  /**
   * Forgets that a once-a-day cron job ran today (vaxia_core Cron::onceADay()
   * keeps its last run in state "vaxia_cron.<job>").
   */
  public function resetDailyCron($variable) {
    \Drupal::state()->set('vaxia_cron.' . $variable, 0);
  }

  public function markDailyCronRan($variable, $timestamp) {
    \Drupal::state()->set('vaxia_cron.' . $variable, $timestamp);
  }

  /**
   * Puts the settings the starter content fills back to the release config's
   * empty values (the test baseline sets the tests' own ids in them).
   */
  public function useReleaseSettings() {
    $factory = \Drupal::configFactory();
    $factory->getEditable('vaxia_conduct.settings')->set('cold_storage_return_form', 0)->save();
    $governance = $factory->getEditable('vaxia_governance.settings');
    $apps = $governance->get('role_app_settings');
    foreach ($apps as $role => $app) {
      $apps[$role]['quiz'] = array();
    }
    $governance->set('anon_mailer_webform', 0)->set('anon_mailer_form', 0)->set('walkthrough_quizzes', array('system' => 0, 'setting' => 0, 'social' => 0))->set('role_app_settings', $apps)->save();
    $storyhosts = $factory->getEditable('vaxia_storyhosts.settings')->set('sh_request_forums', array())->set('storyhost_feedback_form', 0)->set('session_feedback_form', 0);
    foreach (array('ash1', 'ash2', 'ash2_renewal', 'eval', 'ash3') as $key) {
      $storyhosts->set('sh_test_quiz_' . $key, 0);
    }
    $storyhosts->save();
    $factory->getEditable('vaxia_core.settings')->set('vaxia_limbo_room', array())->set('motd_page', 0)->save();
    $factory->getEditable('vaxia_governance.settings')->set('department_join_forms', array('social' => 0, 'setting' => 0, 'system' => 0))->save();
    $factory->getEditable('system.site')->set('page.front', '/node')->save();
  }

  /**
   * Runs scripts/starter-content.php (as `drush php:script` does); returns
   * what it printed.
   */
  public function runStarterContent() {
    ob_start();
    require DRUPAL_ROOT . '/../scripts/starter-content.php';
    $this->resetCaches();
    return ob_get_clean();
  }

  /**
   * The value of a config setting.
   */
  public function setting($name, $key) {
    return \Drupal::config($name)->get($key);
  }

  public function stateValue($key) {
    return \Drupal::state()->get($key);
  }

  public function setLastCron($timestamp) {
    \Drupal::state()->set('system.cron_last', $timestamp);
  }

  public function backdateNode($node, $timestamp) {
    $db = \Drupal::database();
    $db->update('node_field_data')->fields(array('changed' => $timestamp))->condition('nid', $node->nid)->execute();
    $db->update('node_field_revision')->fields(array('changed' => $timestamp))->condition('nid', $node->nid)->execute();
    $db->update('node_revision')->fields(array('revision_timestamp' => $timestamp))->condition('nid', $node->nid)->execute();
    $this->resetCaches();
  }

  public function backdateNodeCreated($node, $timestamp) {
    $db = \Drupal::database();
    $db->update('node_field_data')->fields(array('created' => $timestamp))->condition('nid', $node->nid)->execute();
    $db->update('node_field_revision')->fields(array('created' => $timestamp))->condition('nid', $node->nid)->execute();
    $this->resetCaches();
  }

  public function backdateComment($comment, $timestamp) {
    \Drupal::database()->update('comment_field_data')->fields(array('created' => $timestamp, 'changed' => $timestamp))->condition('cid', $comment->cid)->execute();
    $this->resetCaches();
  }

  public function backdateAccountCreated($account, $timestamp) {
    \Drupal::database()->update('users_field_data')->fields(array('created' => $timestamp))->condition('uid', $account->uid)->execute();
    $this->resetCaches();
  }

  /**
   * Pushes a node's workflow history back a few seconds, so two moves in the
   * same second keep their order.
   */
  public function ageWorkflowHistory($node) {
    \Drupal::database()->query("UPDATE {workflow_transition_history} SET timestamp = timestamp - 5 WHERE entity_type = 'node' AND entity_id = :nid", array(':nid' => $node->nid));
  }

  /**
   * Expires cached lists (Views results cache by these tags).
   */
  public function expireViewsCache() {
    \Drupal::service('cache_tags.invalidator.checksum')->reset();
    \Drupal\Core\Cache\Cache::invalidateTags(array('node_list', 'comment_list', 'user_list', 'taxonomy_term_list', 'vaxia_core:player_list'));
  }

  public function unblockAllIps() {
    if (\Drupal::database()->schema()->tableExists('ban_ip')) {
      \Drupal::database()->delete('ban_ip')->execute();
    }
  }

  public function deletePublicPath($relative) {
    $path = DRUPAL_ROOT . '/' . $this->publicFilesPath() . '/' . $relative;
    if (is_dir($path)) {
      \Drupal::service('file_system')->deleteRecursive($path);
    }
    elseif (file_exists($path)) {
      unlink($path);
    }
  }

  public function publicFilesPath() {
    return \Drupal\Core\StreamWrapper\PublicStream::basePath();
  }

  /**
   * The node at a path alias, as an object with nid and title; NULL if none.
   */
  public function nodeAtAlias($alias) {
    $path = \Drupal::service('path_alias.manager')->getPathByAlias($alias);
    if (!preg_match('#^/node/(\d+)$#', $path, $m) || !($node = Node::load($m[1]))) {
      return NULL;
    }
    return (object) array('nid' => (int) $node->id(), 'title' => $node->label());
  }

  /**
   * The label of a node's workflow state (its first workflow field).
   */
  public function workflowStateLabel($node) {
    $loaded = Node::load($node->nid);
    foreach ($loaded->getFieldDefinitions() as $field => $definition) {
      if ($definition->getType() === 'workflow') {
        $state = \Drupal\workflow\Entity\WorkflowState::load((string) $loaded->get($field)->value);
        return $state ? $state->label() : NULL;
      }
    }
    return NULL;
  }

  /**
   * A menu's links: array of array(title, path) (the test site doesn't place
   * the sidebar menu blocks).
   */
  public function menuLinks($menu) {
    $links = array();
    foreach (\Drupal::entityTypeManager()->getStorage('menu_link_content')->loadByProperties(array('menu_name' => $menu)) as $link) {
      $url = $link->getUrlObject();
      $links[] = array($link->getTitle(), $url->isRouted() && $url->getRouteName() === '<nolink>' ? NULL : $url->toString());
    }
    return $links;
  }

  /**
   * Saves a menu link as the menu UI saves top-level links (parent '').
   */
  public function saveMenuLinkAsTheUiDoes($menu, $title) {
    foreach (\Drupal::entityTypeManager()->getStorage('menu_link_content')->loadByProperties(array('menu_name' => $menu, 'title' => $title)) as $link) {
      $link->set('parent', '')->save();
    }
  }

  /**
   * A forum's access grants for a role: array of view/update/delete/create.
   */
  public function forumGrants($tid, $role) {
    $row = \Drupal::database()->select('vaxia_forum_access', 'a')->fields('a', array('grant_view', 'grant_update', 'grant_delete', 'grant_create'))
      ->condition('a.tid', $tid)->condition('a.role', $role)->execute()->fetchAssoc();
    return $row ? array_map('intval', $row) : NULL;
  }

  public function setForumGrants($tid, $role, array $grants) {
    \Drupal::database()->merge('vaxia_forum_access')->keys(array('tid' => $tid, 'role' => $role))->fields($grants)->execute();
  }

  /**
   * Moves a node to a workflow state as the superuser (where the Workflow tab
   * can't be used).
   */
  public function forceWorkflowState($node, $stateName) {
    $loaded = Node::load($node->nid);
    foreach ($loaded->getFieldDefinitions() as $field => $definition) {
      if ($definition->getType() !== 'workflow') {
        continue;
      }
      $wid = $definition->getSetting('workflow_type');
      foreach (\Drupal\workflow\Entity\WorkflowState::loadMultiple(array(), $wid) as $sid => $state) {
        if ($state->label() === $stateName) {
          $this->runAs((object) array('uid' => 1), function () use ($loaded, $field, $sid) {
            $loaded->set($field, $sid);
            $loaded->save();
          });
          $this->resetCaches();
          return;
        }
      }
    }
    throw new \InvalidArgumentException("No workflow state '$stateName' for {$loaded->bundle()}");
  }

  // ---------------------------------------------------------------------------
  // Internals.
  // ---------------------------------------------------------------------------

  /**
   * The comment field on a node type (D7 migration names it comment_node_<type>).
   */
  private function commentField($bundle) {
    foreach (\Drupal::service('entity_field.manager')->getFieldDefinitions('node', $bundle) as $name => $definition) {
      if ($definition->getType() === 'comment') {
        return $name;
      }
    }
    throw new \InvalidArgumentException("Node type $bundle takes no comments");
  }

  /**
   * Sets fields from D7-style values: scalars, item lists, or items keyed the
   * D7 way (uid / nid / tid / fid become target_id; 'und' levels drop).
   */
  private function setFields($entity, array $values) {
    foreach ($values as $field => $value) {
      if (!$entity->hasField($field)) {
        continue;
      }
      if (is_array($value) && isset($value['und'])) {
        $value = $value['und'];
      }
      if ($value === array()) {
        $entity->set($field, array());
        continue;
      }
      $list = (is_array($value) && array_keys($value) === range(0, count($value) - 1)) ? $value : array($value);
      $type = $entity->getFieldDefinition($field)->getType();
      $items = array();
      foreach ($list as $item) {
        if (is_array($item)) {
          foreach (array('uid', 'nid', 'tid', 'fid') as $key) {
            if (isset($item[$key])) {
              $item['target_id'] = $item[$key];
              unset($item[$key]);
            }
          }
          // D7 date items: value (and value2, the end) as "Y-m-d H:i:s".
          if (in_array($type, array('datetime', 'daterange'), TRUE)) {
            $dateOnly = $entity->getFieldDefinition($field)->getSetting('datetime_type') === 'date';
            $convert = function ($v) use ($dateOnly) {
              return $dateOnly ? substr($v, 0, 10) : str_replace(' ', 'T', $v);
            };
            if (isset($item['value'])) {
              $item['value'] = $convert($item['value']);
            }
            if (isset($item['value2'])) {
              if ($type === 'daterange') {
                $item['end_value'] = $convert($item['value2']);
              }
              unset($item['value2']);
            }
          }
        }
        elseif ($type === 'datetime' && is_string($item)) {
          // Date-only fields store Y-m-d; date+time ones Y-m-dTH:i:s.
          $item = $entity->getFieldDefinition($field)->getSetting('datetime_type') === 'date' ? substr($item, 0, 10) : str_replace(' ', 'T', $item);
        }
        $items[] = $item;
      }
      $entity->set($field, $items);
    }
  }

  public function runAs($account, callable $fn) {
    $switcher = \Drupal::service('account_switcher');
    $switcher->switchTo($account->uid ? User::load($account->uid) : new AnonymousUserSession());
    try {
      return $fn();
    }
    finally {
      $switcher->switchBack();
    }
  }

  public function resetCaches() {
    drupal_static_reset();
    \Drupal::service('entity.memory_cache')->deleteAll();
    $manager = \Drupal::entityTypeManager();
    foreach ($manager->getDefinitions() as $id => $definition) {
      if ($manager->hasHandler($id, 'storage')) {
        $manager->getStorage($id)->resetCache();
      }
    }
    \Drupal::configFactory()->reset();
    \Drupal::state()->resetCache();
    // The checksum service remembers tags it invalidated and skips them
    // after: in this long-lived process that would hide later invalidations
    // from the site (cached lists going stale).
    \Drupal::service('cache_tags.invalidator.checksum')->reset();
  }


  /**
   * Backdates every submission made so far to the webform of node $nid.
   */
  public function backdateWebformSubmissions($nid, $timestamp) {
    \Drupal::database()->update('webform_submission')->fields(array('created' => $timestamp, 'completed' => $timestamp, 'changed' => $timestamp))->condition('entity_type', 'node')->condition('entity_id', $nid)->execute();
    $this->resetCaches();
  }

}
