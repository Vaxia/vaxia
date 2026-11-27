<?php

namespace Drupal\vaxia_core\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\StringTranslation\TranslatableMarkup;

/**
 * On an approved character's pages that have flavour XP: its XP pools in one
 * row (D7 view character_xp_flavors, display block_1).
 */
#[Block(id: 'character_xp_flavors-block_1', admin_label: new TranslatableMarkup('Character XP Breakdown'))]
class XpBreakdownBlock extends XpFlavorsBlock {

  const DISPLAY = 'block_1';
  const TITLE = 'Character XP Breakdown';

  public function build() {
    $cache = ['contexts' => ['route'], 'max-age' => 0];
    $node = $this->character();
    // One row, shown only when the character has a flavour XP row.
    if (!$node || !$this->flavourRows((int) $node->id(), 1)) {
      return ['#cache' => $cache];
    }
    $nid = (int) $node->id();
    $pool = 'SELECT SUM(xp) FROM {character_sheet_xp} WHERE type = :type AND nid = :nid';
    $row = [
      $this->total($pool, [':type' => 'bxp', ':nid' => $nid]),
      $this->total($pool, [':type' => 'rxp', ':nid' => $nid]),
      $this->total($pool, [':type' => 'spent_xp', ':nid' => $nid]),
      $this->total("SELECT SUM(xp) FROM {vaxia_xp_type_totals} WHERE type = 'transfer' AND nid = :nid", [':nid' => $nid]),
      // D7 summed "transfer" here too, not "site-award" (kept as it was).
      $this->total("SELECT SUM(xp) FROM {vaxia_xp_type_totals} WHERE type = 'transfer' AND nid = :nid", [':nid' => $nid]),
      $this->total("SELECT SUM(xp) FROM {vaxia_xp_type_totals} WHERE type NOT IN ('transfer', 'site-award') AND nid = :nid", [':nid' => $nid]),
    ];
    // D7 showed a negative pool as 0.
    $row = array_map(fn($xp) => (string) max(0, $xp), $row);
    return $this->view([
      'content' => [
        '#type' => 'table',
        '#header' => [$this->t('Banked XP'), $this->t('Roleplayed XP'), $this->t('Spent XP'), $this->t('Transfer XP'), $this->t('Site Award XP'), $this->t('Flavor XP')],
        '#rows' => [$row],
      ],
    ], $cache);
  }

}
