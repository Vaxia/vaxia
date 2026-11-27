<?php

namespace Drupal\vaxia_core\Plugin\EntityReferenceSelection;

use Drupal\Core\Entity\Attribute\EntityReferenceSelection;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\node\Plugin\EntityReferenceSelection\NodeSelection;

/**
 * Node selection narrowed to realms (user, 2026-11-25: a character's items
 * and personal room follow the character's realm). Settings:
 * - vaxia_realms: realm term ids; content in one of them, or in none, is
 *   offered. Empty: no narrowing.
 * - vaxia_keep: node ids always allowed (what the field already holds, so
 *   saving a sheet with an older cross-realm reference still validates).
 */
#[EntityReferenceSelection(
  id: "vaxia_realm:node",
  label: new TranslatableMarkup("Node selection by realm"),
  entity_types: ["node"],
  group: "vaxia_realm",
  weight: 5
)]
class RealmNodeSelection extends NodeSelection {

  public function defaultConfiguration() {
    return ['vaxia_realms' => [], 'vaxia_keep' => []] + parent::defaultConfiguration();
  }

  protected function buildEntityQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = parent::buildEntityQuery($match, $match_operator);
    $realms = array_values(array_filter(array_map('intval', (array) $this->getConfiguration()['vaxia_realms'])));
    if ($realms) {
      $or = $query->orConditionGroup()
        ->condition('field_tag_realm', $realms, 'IN')
        ->notExists('field_tag_realm');
      $keep = array_values(array_filter(array_map('intval', (array) $this->getConfiguration()['vaxia_keep'])));
      if ($keep) {
        $or->condition('nid', $keep, 'IN');
      }
      $query->condition($or);
    }
    return $query;
  }

}
