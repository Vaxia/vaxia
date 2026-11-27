<?php

namespace Drupal\vaxia_characters\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\vaxia_core\Character;

/**
 * Staff NPC lists (D7 npcs views): all NPCs, and NPCs needing oversight.
 */
class NpcLists extends ControllerBase {

  private function loadNpcs(): array {
    $storage = $this->entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'character_sheet')->condition('status', 1)->condition('field_is_npc', 1)->sort('title')->execute();
    return array_filter($storage->loadMultiple($ids), fn($n) => Character::state($n) !== 'character_evaluation_creation');
  }

  private function table(array $nodes): array {
    $rows = [];
    foreach ($nodes as $node) {
      $rows[] = [$node->toLink(), ($node->getOwner() ? $node->getOwner()->getDisplayName() : ''), Character::stateLabel($node)];
    }
    return ['#type' => 'table', '#header' => [$this->t('NPC'), $this->t('Owner'), $this->t('State')], '#rows' => $rows, '#empty' => $this->t('No NPCs.'), '#cache' => ['max-age' => 0]];
  }

  public function npcs() {
    return $this->table($this->loadNpcs());
  }

  public function oversight() {
    return $this->table(array_filter($this->loadNpcs(), 'vaxia_characters_npc_needs_oversight'));
  }

}
