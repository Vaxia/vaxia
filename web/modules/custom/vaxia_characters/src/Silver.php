<?php

namespace Drupal\vaxia_characters;

use Drupal\node\Entity\Node;
use Drupal\vaxia_core\Character;

/**
 * Silver and economy (D7 silver module). Silver lives in the XP pools:
 * silver (earned), spent_silver, invest_silver (put toward the next economy
 * level). An economy level is worth silver and pays a little every day.
 */
class Silver {

  private static function xp() {
    return \Drupal::service('vaxia_core.xp');
  }

  public static function available(int $nid): float {
    $xp = self::xp();
    return $xp->get($nid, 'silver') - ($xp->get($nid, 'spent_silver') + $xp->get($nid, 'invest_silver'));
  }

  public static function economy(int $nid): int {
    $node = Node::load($nid);
    return $node && $node->hasField('field_economy') ? (int) $node->get('field_economy')->value : 0;
  }

  /**
   * What all economy levels up to this one are worth in silver.
   */
  public static function worth($economy): float {
    $power = (0.04 * $economy) + 1.853 + (-4 * (1 / ($economy + 1)));
    return pow(10, $power) + 0.5;
  }

  public static function monthlyIncome($economy): float {
    return self::worth($economy) * (3.5 / 100) + 0.05;
  }

  public static function dailyPay($economy): float {
    return self::monthlyIncome($economy) / 30;
  }

  public static function costToImprove($economy): float {
    return self::worth($economy + 1) - self::worth($economy);
  }

  public static function costToRemove($economy): float {
    return self::worth($economy) - self::worth($economy - 1);
  }

  /**
   * Takes silver (a negative amount): from the purse first, then by selling
   * economy levels one at a time (D7 _silver_add_submit_form_remove_silver()).
   */
  public static function remove(int $nid, float $amount): void {
    $xp = self::xp();
    while ($amount < 0) {
      $available = self::available($nid);
      if ($available >= -$amount) {
        $xp->update($nid, $amount, 'silver');
        return;
      }
      $xp->update($nid, -$available, 'silver');
      $amount += $available;
      $node = Node::load($nid);
      $economy = self::economy($nid);
      // No level left to sell: the worth counted for economy 0 (about half a
      // silver) comes from the purse, which goes that far below zero. D7
      // divided by zero here (worth(-1)); the shop lets a character spend
      // their whole worth, so a price within that half silver reached it.
      if ($economy <= 0) {
        $xp->update($nid, $amount, 'silver');
        return;
      }
      $converted = self::costToRemove($economy);
      $xp->update($nid, $converted, 'silver');
      $message = (string) t('Converted economy from level @economy to @less_economy, for @converted silver.', ['@economy' => $economy, '@less_economy' => $economy - 1, '@converted' => floor($converted)]);
      $xp->log($nid, 'silver', $message);
      \Drupal::messenger()->addStatus($message);
      $node->set('field_economy', $economy - 1);
      $node->save();
    }
  }

  /**
   * Daily pay: below a month's income a character earns a day's pay; above it,
   * they spend one (D7 _silver_character_daily_silver()).
   */
  public static function payDay(): void {
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(FALSE)->condition('type', Character::TYPES, 'IN')->condition('status', 1)->exists('field_economy')->execute();
    $xp = self::xp();
    foreach ($storage->loadMultiple($ids) as $node) {
      if (Character::isNpc($node) || Character::isStoryteller($node) || !Character::isPlayable($node)) {
        continue;
      }
      $level = (int) $node->get('field_economy')->value;
      $daily = self::dailyPay($level);
      $nid = (int) $node->id();
      if (self::available($nid) > self::monthlyIncome($level)) {
        $xp->update($nid, -$daily, 'silver');
        $xp->log($nid, 'daily_silver', 'Removed ' . $daily . ' to # ' . $nid . ' ' . $node->label());
      }
      else {
        $xp->update($nid, $daily, 'silver');
        $xp->log($nid, 'daily_silver', 'Added ' . $daily . ' to # ' . $nid . ' ' . $node->label());
      }
    }
  }

}
