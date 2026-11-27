<?php

namespace Drupal\vaxia_items;

use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Character;

/**
 * Facts about items (D7 items.module helpers).
 */
class Item {

  const STATS = ['life', 'endurance', 'strength', 'dexterity', 'intelligence', 'spirituality', 'charisma'];

  public static function isItem(?NodeInterface $node): bool {
    return $node && $node->bundle() === 'items';
  }

  public static function isPlayable(NodeInterface $item): bool {
    return Character::state($item) === \Drupal::config('vaxia_items.settings')->get('items_approved_workflow');
  }

  private static function number(NodeInterface $node, string $field): float {
    return $node->hasField($field) && !$node->get($field)->isEmpty() ? (float) $node->get($field)->value : 0;
  }

  /**
   * Silver value from bonuses, overlaps, powers, defense and companion build
   * points (D7 _items_get_item_value()), with a status line per part.
   */
  public static function value(NodeInterface $node): float {
    $config = \Drupal::config('vaxia_items.settings');
    $messenger = \Drupal::messenger();
    $title = $node->label();
    $primary = 0;
    foreach (self::STATS as $stat) {
      $primary += self::number($node, 'field_item_' . $stat);
    }
    $bonus = $primary * (float) $config->get('items_primary_value') + self::number($node, 'field_item_skill') * (float) $config->get('items_skills_value');
    $bonus_value = 0;
    if ($bonus > 0) {
      $per_point = (float) $config->get('items_bonus_value');
      // Each point costs double for every five points past the first.
      for ($i = 0; $i < floor(($bonus - 1) / 5); $i++) {
        $per_point *= 2;
      }
      $bonus_value = $bonus * $per_point;
      $messenger->addStatus(t('Value of stat modifiers for %title calculated at @value silver.', ['%title' => $title, '@value' => $bonus_value]));
    }
    // Overlaps. The intelligence one is spelled field_item_over_intellegence
    // (D7 read a field that doesn't exist: known bug).
    $overlap = self::number($node, 'field_item_over_skill');
    foreach (self::STATS as $stat) {
      $overlap += self::number($node, 'field_item_over_' . ($stat === 'intelligence' ? 'intellegence' : $stat));
    }
    $overlap_value = 0;
    if ($overlap > 0) {
      $overlap_value = $overlap * (float) $config->get('items_overlap_value');
      $messenger->addStatus(t('Value of overlapping stats for %title calculated at @value silver.', ['%title' => $title, '@value' => $overlap_value]));
    }
    $pp_value = 0;
    $pp = self::number($node, 'field_pp_created');
    if ($pp > 0) {
      $uses = $node->hasField('field_item_use_day') && !$node->get('field_item_use_day')->isEmpty() ? $node->get('field_item_use_day')->value : 'at_will';
      // No charges (0) counts as one. D7 multiplied by the stored 0, so every
      // at-will power item (uses hidden, stored 0) was worth 0 for its power;
      // the user chose the at-will rate instead (2026-11-25).
      $charges = self::number($node, 'field_item_uses') ?: 1;
      $per_pp = ['once' => 'items_once_value', 'per_day' => 'items_daily_value'][$uses] ?? 'items_at_will_value';
      // Harder powers cost more, relative to the base difficulty 25 (D7 read
      // it from an unset variable, so 25).
      $difficulty = max(self::number($node, 'field_item_difficulty') ?: 25, 25) / 25;
      $pp_value = $pp * (float) $config->get($per_pp) * $difficulty * $charges;
      $messenger->addStatus(t('Value of PP base effect for %title calculated at @value silver.', ['%title' => $title, '@value' => $pp_value]));
    }
    $def_value = 0;
    $absorbs = self::number($node, 'field_armor_absorbs');
    if ($absorbs > 0) {
      $type = $node->hasField('field_absorb_type') && !$node->get('field_absorb_type')->isEmpty() ? $node->get('field_absorb_type')->value : 'armor';
      $def_value = $absorbs * (float) $config->get($type === 'shield' ? 'items_shield_value' : 'items_absorb_value');
      $messenger->addStatus(t('Value of defense for %title calculated at @value silver.', ['%title' => $title, '@value' => $def_value]));
    }
    $comp_value = 0;
    $build = self::number($node, 'field_build_points');
    if ($build > 0) {
      $comp_value = $build * (float) $config->get('items_companion_value');
      $messenger->addStatus(t('Value of companion build points for %title calculated at @value silver.', ['%title' => $title, '@value' => $comp_value]));
    }
    $total = $bonus_value + $overlap_value + $pp_value + $def_value + $comp_value;
    $messenger->addStatus(t('Overall value of %title calculated at @value silver.', ['%title' => $title, '@value' => $total]));
    return $total;
  }

}
