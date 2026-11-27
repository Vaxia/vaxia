<?php

namespace Drupal\vaxia_core\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * On an approved character's pages: the share of each flavour of XP in its
 * flavour XP (D7 view character_xp_flavors, display "block").
 */
#[Block(id: 'character_xp_flavors-block', admin_label: new TranslatableMarkup('Character XP Flavors'))]
class XpFlavorsBlock extends ViewBlockBase {

  const VIEW = 'character_xp_flavors';
  const DISPLAY = 'block';
  const TITLE = 'Character XP Flavors';

  /**
   * D7 filter "type contains any of these words".
   */
  const FLAVOUR_WORDS = ['physical', 'magical', 'support', 'artist', 'rogue', 'intelligent', 'charismatic', 'good', 'evil'];

  /**
   * The published, approved character sheet whose page this is (D7: node id
   * from arg(1)), or NULL.
   */
  protected function character(): ?NodeInterface {
    $node = \Drupal::routeMatch()->getParameter('node');
    if (is_numeric($node)) {
      $node = \Drupal::entityTypeManager()->getStorage('node')->load($node);
    }
    if (!$node instanceof NodeInterface || $node->bundle() !== 'character_sheet' || !$node->isPublished()) {
      return NULL;
    }
    return $node->get('field_character_evaluation')->value === 'character_evaluation_approved' ? $node : NULL;
  }

  /**
   * The character's flavour XP rows (type => xp), by type, at most $limit.
   */
  protected function flavourRows(int $nid, int $limit): array {
    $query = \Drupal::database()->select('vaxia_xp_type_totals', 't')
      ->fields('t', ['type', 'xp'])
      ->condition('t.nid', $nid)
      ->orderBy('t.type')
      ->range(0, $limit);
    $words = $query->orConditionGroup();
    foreach (self::FLAVOUR_WORDS as $word) {
      $words->condition('t.type', '%' . $query->escapeLike($word) . '%', 'LIKE');
    }
    return $query->condition($words)->execute()->fetchAllKeyed();
  }

  /**
   * floor(SUM(xp)) over a query on one of the XP tables (D7 PHP fields).
   */
  protected function total(string $sql, array $args): float {
    return floor((float) \Drupal::database()->query($sql, $args)->fetchField());
  }

  public function build() {
    // XP tables carry no cache tags.
    $cache = ['contexts' => ['route'], 'max-age' => 0];
    $node = $this->character();
    $rows = $node ? $this->flavourRows((int) $node->id(), 9) : [];
    if (!$rows) {
      return ['#cache' => $cache];
    }
    $flavour = $this->total("SELECT SUM(xp) FROM {vaxia_xp_type_totals} WHERE type NOT IN ('transfer', 'site-award') AND nid = :nid", [':nid' => $node->id()]);
    $table = [];
    foreach ($rows as $type => $xp) {
      // D7 math expression ([xp] / [Flavor XP]) * 100, 2 decimals, "%".
      $table[] = [$flavour ? number_format($xp / $flavour * 100, 2) . '%' : '', $type];
    }
    return $this->view([
      'header' => ['#type' => 'html_tag', '#tag' => 'style', '#value' => '.view-character-xp-flavors table{width:40%;float:left;margin-left:5%;}'],
      'content' => ['#type' => 'table', '#rows' => $table],
    ], $cache);
  }

}
