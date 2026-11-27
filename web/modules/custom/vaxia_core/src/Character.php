<?php

namespace Drupal\vaxia_core;

use Drupal\node\NodeInterface;
use Drupal\workflow\Entity\WorkflowState;

/**
 * Facts about character sheets (D7 character_sheet helpers).
 */
class Character {

  /**
   * Content types that are Vaxia character sheets.
   */
  const TYPES = ['character_sheet'];

  /**
   * Stats, in sheet order: field suffix => label.
   */
  const STATS = [
    'life' => 'Life',
    'endurance' => 'Endurance',
    'strength' => 'Strength',
    'dexterity' => 'Dexterity',
    'intelligence' => 'Intelligence',
    'spirituality' => 'Spirituality',
    'charisma' => 'Charisma',
  ];

  /**
   * Derived stats (D7 computed fields): key => [label, stat, stat], the
   * average of the two. D7 labelled reflexes "Finesse". Shown on sheets and
   * rolled in the dice roller (rounded down there, as D7's integer stats).
   */
  const DERIVED = [
    'health' => ['Health', 'life', 'endurance'],
    'constitution' => ['Constitution', 'endurance', 'strength'],
    'agility' => ['Agility', 'strength', 'dexterity'],
    'reflexes' => ['Finesse', 'dexterity', 'intelligence'],
    'awareness' => ['Awareness', 'intelligence', 'spirituality'],
    'presence' => ['Presence', 'spirituality', 'charisma'],
    'appearance' => ['Appearance', 'charisma', 'life'],
  ];

  public static function isSheet(?NodeInterface $node): bool {
    return $node && in_array($node->bundle(), self::TYPES, TRUE);
  }

  /**
   * The node's workflow field name, if its type has one.
   */
  public static function workflowField(NodeInterface $node): ?string {
    foreach ($node->getFieldDefinitions() as $name => $definition) {
      if ($definition->getType() === 'workflow') {
        return $name;
      }
    }
    return NULL;
  }

  /**
   * Current workflow state id (e.g. character_evaluation_approved).
   */
  public static function state(NodeInterface $node): ?string {
    $field = self::workflowField($node);
    return $field ? $node->get($field)->value : NULL;
  }

  /**
   * Moves a sheet to a workflow state, whoever is acting (D7
   * workflow_execute_transition(..., TRUE)).
   */
  public static function forceState(NodeInterface $node, string $sid, string $comment = ''): void {
    $field = self::workflowField($node);
    if (!$field) {
      return;
    }
    // With the entity: workflow works out a default state from its owner.
    $transition = \Drupal\workflow\Entity\WorkflowTransition::create([self::state($node), 'field_name' => $field, 'entity' => $node]);
    $transition->setTargetEntity($node);
    $transition->setValues($sid, (int) \Drupal::currentUser()->id(), \Drupal::time()->getRequestTime(), $comment, TRUE);
    // executeAndUpdateEntity() ignores its $force argument; force() sticks.
    $transition->force(TRUE);
    $transition->executeAndUpdateEntity(TRUE);
  }

  /**
   * Moves a sheet to a workflow state once the current request is done (its
   * response is ready). For moves decided while another transition is being
   * saved: made at once, workflow's outer save would overwrite them.
   */
  public static function forceStateLater(NodeInterface $node, string $sid, string $comment = ''): void {
    if (PHP_SAPI === 'cli') {
      self::forceState($node, $sid, $comment);
      return;
    }
    self::$later[] = [(int) $node->id(), $sid, $comment];
  }

  /**
   * Moves queued by forceStateLater(), run by DeferredStates.
   */
  public static function runLater(): void {
    while ($move = array_shift(self::$later)) {
      [$nid, $sid, $comment] = $move;
      \Drupal::entityTypeManager()->getStorage('node')->resetCache([$nid]);
      if ($node = \Drupal\node\Entity\Node::load($nid)) {
        self::forceState($node, $sid, $comment);
      }
    }
  }

  /**
   * @var array
   */
  private static array $later = [];

  public static function stateLabel(NodeInterface $node): string {
    $sid = self::state($node);
    $state = $sid ? WorkflowState::load($sid) : NULL;
    return $state ? (string) $state->label() : '';
  }

  /**
   * In a playable state (D7 _character_sheet_is_playable()).
   */
  public static function isPlayable(NodeInterface $node): bool {
    if ($node->isNew()) {
      return FALSE;
    }
    $playable = \Drupal::config('vaxia_core.settings')->get('character_sheet_vaxia_playable') ?: [];
    return in_array(self::state($node), $playable, TRUE);
  }

  public static function flag(NodeInterface $node, string $field): bool {
    return $node->hasField($field) && (bool) $node->get($field)->value;
  }

  public static function isStoryteller(NodeInterface $node): bool {
    return self::flag($node, 'field_is_storyteller');
  }

  public static function isNpc(NodeInterface $node): bool {
    return self::flag($node, 'field_is_npc');
  }

  /**
   * "Title (owner) [nid:N]": the transfer autocomplete's label.
   */
  public static function transferLabel(NodeInterface $node): string {
    return $node->label() . ' (' . $node->getOwner()->getAccountName() . ') [nid:' . $node->id() . ']';
  }

  /**
   * The published character sheet a transfer label points at.
   */
  public static function fromTransferLabel(string $label): ?NodeInterface {
    if (!preg_match('/\[nid:([0-9]+)\]$/', $label, $m)) {
      return NULL;
    }
    $node = \Drupal::entityTypeManager()->getStorage('node')->load($m[1]);
    return ($node && self::isSheet($node) && $node->isPublished()) ? $node : NULL;
  }

  /**
   * A player's storyhost character (D7 _vaxia_get_SH_characters()).
   */
  public static function storyhostCharacter(int $uid): ?NodeInterface {
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(FALSE)
      ->condition('type', 'character_sheet')
      ->condition('uid', $uid)
      ->condition('field_is_storyteller', 1)
      ->sort('nid')
      ->range(0, 1)
      ->execute();
    return $ids ? $storage->load(reset($ids)) : NULL;
  }

  /**
   * Characters an account may post as in a room, grouped as D7's
   * vaxia_character_sheets:my_chars_ref view did: nid => title under
   * "Characters", "NPCs" or "Generic NPCs".
   *
   * - Own published sheets in a playable state, and own storyhost sheets.
   * - Published NPC sheets in a playable state (anyone may play them).
   * - Only the room's realm (plus the storyhost realm) when the room has one.
   */
  public static function postableOptions(int $uid, ?NodeInterface $room = NULL): array {
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(FALSE)
      ->condition('type', 'character_sheet')
      ->condition('status', 1)
      ->sort('field_is_npc')
      ->sort('field_is_generic_npc', 'DESC')
      ->sort('title');
    // NPCs only for those with "use NPCs" (storyhosts and admins; user,
    // 2026-11-26).
    $account = $uid == \Drupal::currentUser()->id() ? \Drupal::currentUser() : \Drupal\user\Entity\User::load($uid);
    $npcs = $account && $account->hasPermission('use NPCs');
    if ($npcs) {
      $or = $query->orConditionGroup()
        ->condition('uid', $uid)
        ->condition('field_is_npc', 1);
      $query->condition($or);
    }
    else {
      $query->condition('uid', $uid);
    }
    // In a realm's rooms: that realm's characters and the admin realm's
    // (storyhost characters); in admin-realm rooms, any.
    $admin = (int) array_key_first(\Drupal::entityTypeManager()->getStorage('taxonomy_term')->loadByProperties(['vid' => 'realm', 'name' => self::adminRealm()]) ?: [0 => NULL]);
    $realm = NULL;
    if ($room && $room->hasField('field_tag_realm') && !$room->get('field_tag_realm')->isEmpty()) {
      $realm = (int) $room->get('field_tag_realm')->target_id;
      if ($realm === $admin) {
        $realm = NULL;
      }
    }
    if ($realm) {
      $query->condition('field_tag_realm', array_filter([$realm, $admin]), 'IN');
    }
    $groups = [];
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      $own_sh = $node->getOwnerId() == $uid && self::isStoryteller($node);
      if (!$own_sh && !self::isPlayable($node)) {
        continue;
      }
      if (!$npcs && (self::isNpc($node) || self::flag($node, 'field_is_open_npc'))) {
        continue;
      }
      if (self::isNpc($node)) {
        $group = self::flag($node, 'field_is_generic_npc') ? 'Generic NPCs' : 'NPCs';
      }
      else {
        if ($node->getOwnerId() != $uid) {
          continue;
        }
        $group = 'Characters';
      }
      $groups[$group][$node->id()] = $node->label();
    }
    return $groups;
  }

  /**
   * The character an account last posted as (or NULL).
   */
  public static function lastPosted(int $uid): ?int {
    $nid = \Drupal::service('user.data')->get('vaxia', $uid, 'last_character');
    return $nid ? (int) $nid : NULL;
  }

  public static function setLastPosted(int $uid, ?int $nid): void {
    \Drupal::service('user.data')->set('vaxia', $uid, 'last_character', $nid);
  }

  /**
   * Rooms where posts earn no XP and need no character.
   */
  /**
   * The playable realms' own fields (vaxia_core.settings realm_fields), by
   * realm name: ['species' => field name, 'languages' => field name,
   * 'conditions' => field name, 'skill_fields' => [field names inside
   * skills]].
   */
  public static function realmFields(): array {
    $fields = [];
    foreach ((array) \Drupal::config('vaxia_core.settings')->get('realm_fields') as $row) {
      if (!empty($row['realm'])) {
        $fields[$row['realm']] = ['species' => (string) ($row['species'] ?? ''), 'languages' => (string) ($row['languages'] ?? ''), 'conditions' => (string) ($row['conditions'] ?? '')];
        // Fields inside skills (paragraphs) only this realm uses.
        $fields[$row['realm']]['skill_fields'] = array_values(array_filter((array) ($row['skill_fields'] ?? [])));
      }
    }
    return $fields;
  }

  /**
   * The administrative realm's name: storyhosting and system pages, not
   * played in (vaxia_core.settings admin_realm).
   */
  public static function adminRealm(): string {
    return (string) (\Drupal::config('vaxia_core.settings')->get('admin_realm') ?: 'Storyhost');
  }

  public static function isLimbo(?NodeInterface $room): bool {
    $limbo = \Drupal::config('vaxia_core.settings')->get('vaxia_limbo_room') ?: [];
    return $room && in_array((int) $room->id(), array_map('intval', $limbo), TRUE);
  }

  /**
   * Point cost of a value (D7 _vaxia_calculate_attribute_field_value()):
   * each point costs the multiplier, and one more per point above each ten.
   */
  public static function cost(float $multiplier, $value): float {
    $value = (int) $value;
    $total = 0;
    $current = 0;
    $base = 1;
    while ($current < $value) {
      $current++;
      $total += $base * $multiplier;
      if ($current > 10 && ($current % 10) == 0) {
        $base++;
      }
    }
    return $total;
  }

  /**
   * Min-max warnings for a sheet (D7 _vaxia_check_minmax_flags()): too many
   * low stats or values, too much on stats, too many maxed values, low skill
   * average. Empty when the sheet is balanced.
   *
   * @return string[]
   */
  public static function minmaxFlags(NodeInterface $node): array {
    $config = \Drupal::config('vaxia_core.settings');
    $high_skill = (float) $config->get('vaxia_pc_skill_max');
    $high_stat = (float) $config->get('vaxia_pc_stat_max');
    $stat_warn = (float) $config->get('vaxia_pc_stat_spend_warn');
    $min_warn = (float) $config->get('vaxia_pc_skill_warn');
    $max_warn = (float) $config->get('vaxia_pc_stat_high_warn');
    $min_max = (float) $config->get('vaxia_pc_stat_min_warn');
    $skill_ave_warn = (float) $config->get('vaxia_pc_skill_ave_warn');
    $on_stats = $on_skills = $maxed = $low_stats = $low_values = 0;
    foreach (array_keys(self::STATS) as $stat) {
      $value = $node->hasField('field_' . $stat) ? (float) $node->get('field_' . $stat)->value : 0;
      $on_stats += $value;
      $maxed += $value >= $high_stat ? 1 : 0;
      $low_stats += $value < $min_warn ? 1 : 0;
      $low_values += $value <= $min_warn ? 1 : 0;
    }
    $skills = self::skills($node);
    foreach ($skills as $skill) {
      $value = (float) $skill->get('field_skill_value')->value;
      $on_skills += $value;
      $maxed += $value >= $high_skill ? 1 : 0;
      $low_values += $value <= $min_warn ? 1 : 0;
    }
    $messages = [];
    if ($low_stats > 1) {
      $messages[] = (string) t('You have %low_stats stat below %min_warn. We recommend having higher stats.', ['%low_stats' => $low_stats, '%min_warn' => $min_warn]);
    }
    if ($low_values > $min_max) {
      $messages[] = (string) t('You have %low_vals values at %min_warn or below. We recommend no more than %min_max low values.', ['%low_vals' => $low_values, '%min_warn' => $min_warn, '%min_max' => $min_max]);
    }
    if ($on_stats > $stat_warn) {
      $messages[] = (string) t('You have spent %spent_on_stats points on stats. We recommend no more than %stat_max on stats.', ['%spent_on_stats' => $on_stats, '%stat_max' => $stat_warn]);
    }
    if ($maxed > $max_warn) {
      $messages[] = (string) t('You have %maxed_values maximized values.  We recommend no more than %max_max maxed values.', ['%maxed_values' => $maxed, '%max_max' => $max_warn]);
    }
    $average = $skills ? $on_skills / count($skills) : 0;
    if ($average < $skill_ave_warn) {
      $messages[] = (string) t('You have an average of %skill_average on skills. We recommend having higher skills on average.', ['%skill_average' => number_format($average, 2)]);
    }
    return $messages;
  }

  /**
   * Skill paragraphs on a sheet.
   *
   * @return \Drupal\paragraphs\ParagraphInterface[]
   */
  public static function skills(NodeInterface $node): array {
    $skills = [];
    if ($node->hasField('field_skill')) {
      foreach ($node->get('field_skill') as $item) {
        if ($item->entity) {
          $skills[] = $item->entity;
        }
      }
    }
    return $skills;
  }

}
