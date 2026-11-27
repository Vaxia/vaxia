<?php

namespace Drupal\vaxia_dice;

use Drupal\Component\Utility\Html;
use Drupal\node\NodeInterface;
use Drupal\paragraphs\Entity\Paragraph;
use Drupal\vaxia_core\Character;

/**
 * The dice rows on a chat post, and turning them into roll commands (D7
 * vaxia.dice.inc).
 */
class DiceForm {

  /**
   * SH dice levels: value => label.
   */
  const SH_LEVELS = [
    '0' => 'No ability (0)',
    '15' => 'Low (15)',
    '25' => 'Average (25)',
    '30' => 'Dabbler (30)',
    '35' => 'Apprentice (35)',
    '45' => 'Trained (45)',
    '55' => 'Expert (55)',
    '60' => 'Known Expert (60)',
    '70' => 'Mastery (70) (NPC cap)',
  ];

  /**
   * The dice element for a character ("vaxia_rolls"): a placeholder without
   * one, so the character picker's AJAX has a wrapper to fill.
   */
  public static function element(?NodeInterface $character, string $type = 'character'): array {
    $element = [
      '#type' => 'fieldset',
      '#prefix' => '<div id="vaxia-dice-' . $type . '" class="vaxia-dice-roller' . ($character ? '' : ' vaxia-dice-roller-hidden') . '">',
      '#suffix' => '</div>',
      '#weight' => 10,
      '#tree' => TRUE,
    ];
    if (!$character) {
      return $element;
    }
    $element['#description'] = ['#markup' => '<div class="vaxia-dice-roller-node">' . $character->toLink()->toString() . '</div>'];
    $rows = (int) \Drupal::config('vaxia_core.settings')->get('vaxia_display_dice') ?: 6;
    $element['dice'] = Character::isStoryteller($character) ? self::storyhostRows($rows) : self::characterRows($character, $rows);
    $element['dice']['#type'] = 'table';
    $element['dice']['#header'] = [t('#'), t('d'), t('stat'), t('skill')];
    return $element;
  }

  private static function numberSelect(): array {
    return ['#type' => 'select', '#options' => range(0, 10), '#attributes' => ['class' => ['dice-number']]];
  }

  private static function d100(): array {
    return ['#type' => 'select', '#options' => ['100' => 100], '#default_value' => '100', '#disabled' => TRUE, '#attributes' => ['class' => ['dice-size']]];
  }

  /**
   * Storyhost rows: preset levels, then two free rows (any die, any number).
   */
  public static function storyhostRows(int $rows): array {
    $form = [];
    $sizes = [100, 20, 12, 10, 8, 6, 4, 2];
    for ($i = 0; $i < $rows; $i++) {
      $free = $i >= $rows - 2;
      $form[$i]['number'] = self::numberSelect();
      $form[$i]['size'] = $free ? ['#type' => 'select', '#options' => array_combine($sizes, $sizes), '#attributes' => ['class' => ['dice-size']]] : self::d100();
      foreach (['stat', 'skill'] as $key) {
        $form[$i][$key] = $free
          ? ['#type' => 'textfield', '#size' => 5, '#default_value' => '', '#attributes' => ['class' => ['dice-' . $key]]]
          : ['#type' => 'select', '#options' => self::SH_LEVELS, '#attributes' => ['class' => ['dice-' . $key]]];
      }
    }
    return $form;
  }

  /**
   * Character rows: d100 plus a stat and a skill from the sheet.
   */
  public static function characterRows(NodeInterface $character, int $rows): array {
    $stats = ['-1' => '<none>'];
    foreach (Character::STATS as $key => $label) {
      $stats[$key] = $label . ' (' . ($character->hasField('field_' . $key) ? (int) $character->get('field_' . $key)->value : 0) . ')';
    }
    // Derived stats, rounded down (D7's roller had them; review, 2026-11-26).
    foreach (Character::DERIVED as $key => [$label, $one, $two]) {
      $stats[$key] = $label . ' (' . intdiv((int) $character->get('field_' . $one)->value + (int) $character->get('field_' . $two)->value, 2) . ')';
    }
    $skills = ['-1' => '<none>'];
    foreach (Character::skills($character) as $skill) {
      $value = $skill->get('field_skill_value')->value;
      if (!empty($value) && $value > 0) {
        $skills[$skill->id()] = t('@name (@value)', ['@name' => $skill->get('field_skill_name')->value ?: t('Unk. skill'), '@value' => $value]);
      }
    }
    $form = [];
    for ($i = 0; $i < $rows; $i++) {
      $form[$i]['number'] = self::numberSelect();
      $form[$i]['size'] = self::d100();
      $form[$i]['stat'] = ['#type' => 'select', '#options' => $stats, '#attributes' => ['class' => ['dice-stat']]];
      $form[$i]['skill'] = ['#type' => 'select', '#options' => $skills, '#attributes' => ['class' => ['dice-skill']]];
    }
    return $form;
  }

  /**
   * Roll commands for submitted dice rows: [['command' => '1d100+45',
   * 'note' => 'str (5) + <a ...>Smithing</a> (40)'], ...].
   */
  public static function commands(array $submitted, NodeInterface $character): array {
    $rows = [];
    foreach ($submitted['dice'] ?? [] as $roll) {
      if (!is_array($roll) || empty($roll['number']) || $roll['number'] <= 0) {
        continue;
      }
      $roll['size'] = !empty($roll['size']) ? $roll['size'] : 100;
      $rows[] = $roll;
    }
    // Items alone still roll (item modules add a 1d1 row).
    if (!$rows && empty($submitted['items'])) {
      return [];
    }
    if (Character::isStoryteller($character)) {
      $commands = [];
      foreach ($rows as $roll) {
        $stat = (int) trim((string) ($roll['stat'] ?? 0));
        $skill = (int) trim((string) ($roll['skill'] ?? 0));
        $commands[] = [
          'command' => $roll['number'] . 'd' . $roll['size'] . '+' . ($stat + $skill),
          'note' => 'stat (' . $stat . ') + skill (' . $skill . ')',
        ];
      }
      return $commands;
    }
    $prepared = [];
    foreach ($rows as $delta => $roll) {
      $prepared[$delta] = self::prepare($roll, $character);
    }
    $context = ['submitted_rolls' => $submitted, 'character' => $character];
    \Drupal::moduleHandler()->alter('vaxia_dice_commands', $prepared, $context);
    $commands = [];
    foreach ($prepared as $roll) {
      $stat = $roll['stat'] > 0 ? (int) $roll['stat'] : 0;
      $skill = $roll['skill'] > 0 ? (int) $roll['skill'] : 0;
      $commands[] = [
        'command' => $roll['number'] . 'd' . $roll['size'] . '+' . ($stat + $skill),
        'note' => !empty($roll['note']) ? implode(' + ', $roll['note']) : $roll['number'] . 'd' . $roll['size'],
      ];
    }
    return $commands;
  }

  /**
   * A character row with its stat and skill values and notes (D7
   * _vaxia_dice_roller_prep_rolls()).
   */
  public static function prepare(array $roll, NodeInterface $character): array {
    $roll['number'] = !empty($roll['number']) ? $roll['number'] : 1;
    $roll['size'] = !empty($roll['size']) ? $roll['size'] : 100;
    $stat_key = (isset($roll['stat']) && $roll['stat'] != -1 && $roll['stat'] !== '') ? $roll['stat'] : 'any';
    $skill_id = (isset($roll['skill']) && $roll['skill'] != -1) ? (int) $roll['skill'] : 0;
    $roll['stat_name'] = $stat_key;
    $roll['skill_nid'] = $skill_id;
    $roll['note'] = $roll['note'] ?? [];
    $attributes = [];
    $label = self::smallLabel($stat_key);
    $roll['stat'] = isset(Character::STATS[$stat_key]) && $character->hasField('field_' . $stat_key) ? (int) $character->get('field_' . $stat_key)->value : 0;
    if (isset(Character::DERIVED[$stat_key])) {
      [, $one, $two] = Character::DERIVED[$stat_key];
      $roll['stat'] = intdiv((int) $character->get('field_' . $one)->value + (int) $character->get('field_' . $two)->value, 2);
    }
    if ($roll['stat']) {
      $roll['note'][] = $label . ' (' . $roll['stat'] . ')';
    }
    $attributes['stat_name'] = $label;
    $attributes['stat'] = $roll['stat'];
    $roll['skill'] = 0;
    $skill = $skill_id ? Paragraph::load($skill_id) : NULL;
    // Only the character's own skills count.
    $owned = $skill && in_array($skill_id, array_map(fn($s) => (int) $s->id(), Character::skills($character)), TRUE);
    if ($owned && !empty($skill->get('field_skill_value')->value)) {
      $value = $skill->get('field_skill_value')->value;
      $name = $skill->get('field_skill_name')->value ?: t('Unk. skill');
      $desc = (string) ($skill->hasField('field_skill_desc') ? $skill->get('field_skill_desc')->value : '');
      $roll['skill'] = $value;
      $attributes['skill'] = $value;
      $aspects = [];
      foreach (['field_aspect_one', 'field_aspect_two', 'field_aspect_three'] as $field) {
        if ($skill->hasField($field) && $skill->get($field)->entity) {
          $aspects[] = strtolower($skill->get($field)->entity->label());
        }
      }
      if ($aspects) {
        $desc .= ' (' . implode(', ', $aspects) . ')';
        $attributes['aspects'] = implode(' ', $aspects);
      }
      if ($skill->hasField('field_tech_languages')) {
        $techs = [];
        foreach ($skill->get('field_tech_languages') as $item) {
          $tech = $item->entity;
          if ($tech && $tech->get('field_tech_language')->entity) {
            $techs[] = $tech->get('field_tech_language')->entity->label() . ': ' . strtolower($tech->get('field_tech_proficiency')->value ?: 'Unfamiliar');
          }
        }
        if ($techs) {
          $desc .= ' (' . implode(', ', $techs) . ')';
        }
      }
      $context = '';
      foreach ($attributes as $key => $val) {
        $context .= ' ' . $key . '="' . Html::escape((string) $val) . '"';
      }
      $roll['note'][] = '<a title="' . Html::escape($desc) . '" ' . $context . ' href="#">' . Html::escape($name) . '</a> (' . $value . ')';
    }
    return $roll;
  }

  /**
   * "strength" => "str" (D7 _vaxia_dice_roller_small_label()).
   */
  public static function smallLabel(string $name): string {
    $label = substr(strtolower($name), 0, 3);
    return $label === 'ref' ? 'fin' : $label;
  }

}
