<?php

namespace Drupal\vaxia_items;

use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Character;
use Drupal\vaxia_dice\DiceForm;

/**
 * Items on dice rolls (D7 item_dice): the "Using items" list on the post form,
 * and item bonuses and overlaps added to the rolled stats and skills.
 *
 * Per stat (and skill): the highest bonus counts (bonuses don't stack), and
 * the highest overlap replaces the stat with a flat value.
 */
class ItemDice {

  /**
   * Stat keys items modify: the seven stats and skill.
   */
  public static function keys(): array {
    return array_keys(Character::STATS + ['skill' => 'Skill']);
  }

  /**
   * Item field for a stat's bonus or overlap.
   */
  public static function field(string $stat, bool $overlap = FALSE): string {
    if ($overlap) {
      // The intelligence overlap field is spelled "intellegence".
      return 'field_item_over_' . ($stat === 'intelligence' ? 'intellegence' : $stat);
    }
    return 'field_item_' . $stat;
  }

  public static function number(NodeInterface $item, string $field): float {
    return $item->hasField($field) && !$item->get($field)->isEmpty() ? (float) $item->get($field)->value : 0;
  }

  /**
   * A character's approved items, as list options: "Sword (str:2, dex:5 flat)".
   */
  public static function options(NodeInterface $character): array {
    $options = [];
    if (!$character->hasField('field_items')) {
      return $options;
    }
    foreach ($character->get('field_items') as $reference) {
      $item = $reference->entity;
      if (!$item || !Item::isPlayable($item)) {
        continue;
      }
      $labels = [];
      foreach (self::keys() as $stat) {
        $small = DiceForm::smallLabel($stat);
        $bonus = self::number($item, self::field($stat));
        $overlap = self::number($item, self::field($stat, TRUE));
        if ($overlap) {
          $labels[] = $small . ':' . number_format($overlap) . ' flat';
        }
        elseif ($bonus) {
          $labels[] = $small . ':' . $bonus;
        }
      }
      $labels = array_merge($labels, (array) \Drupal::moduleHandler()->invokeAll('item_dice_label', [$item]));
      if ($labels) {
        $options[$item->id()] = t('@name (@mods)', ['@name' => $item->label(), '@mods' => implode(', ', $labels)]);
      }
    }
    return $options;
  }

  /**
   * What the chosen items do per stat key: ['modifies' => [[modifies, notes]],
   * 'overlaps' => n, 'notes' => '...', 'general' => n].
   */
  public static function modifiers(array $item_ids): array {
    $mods = [];
    foreach ($item_ids as $nid) {
      $item = Node::load((int) $nid);
      if (!$item || !Item::isItem($item) || !Item::isPlayable($item)) {
        continue;
      }
      foreach (self::keys() as $stat) {
        $small = DiceForm::smallLabel($stat);
        $bonus = self::number($item, self::field($stat));
        if ($bonus) {
          $current = $mods[$stat]['modifies'][0]['modifies'] ?? 0;
          if ($bonus > $current) {
            $mods[$stat]['modifies'][0] = ['modifies' => $bonus, 'notes' => $item->label() . ' (' . $small . ' ' . $bonus . ')'];
          }
        }
        $overlap = self::number($item, self::field($stat, TRUE));
        if ($overlap && $overlap > ($mods[$stat]['overlaps'] ?? 0)) {
          $mods[$stat]['overlaps'] = $overlap;
          $mods[$stat]['notes'] = $item->label() . ' (' . $small . ' ' . $overlap . ' flat)';
        }
        $context = ['stat_key' => $stat, 'item' => $item];
        \Drupal::moduleHandler()->alter('item_dice_submit', $mods, $context);
      }
    }
    return $mods;
  }

  /**
   * Applies one stat key's item effects to a roll's stat or skill.
   */
  public static function apply(array $roll, array $items, string $target): array {
    $roll[$target] = $roll[$target] ?? 0;
    if (!empty($items['overlaps'])) {
      $roll[$target] = $items['overlaps'];
    }
    foreach ($items['modifies'] ?? [] as $modifier) {
      if (!empty($modifier['modifies'])) {
        $roll[$target] += $modifier['modifies'];
        $roll['item_mods'] += $modifier['modifies'];
        $roll['item_notes'][] = $modifier['notes'];
      }
    }
    if (!empty($items['general'])) {
      $roll[$target] += $items['general'];
      $roll['item_mods'] += $items['general'];
    }
    if (!empty($items['notes'])) {
      $roll['item_notes'][] = $items['notes'];
    }
    return $roll;
  }

}
