<?php

namespace Drupal\vaxia_core\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\vaxia_core\Character;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\Request;

/**
 * Transfer targets: the current player's playable, non-NPC, non-storyhost
 * characters whose title starts with the typed text.
 *
 * Answers both the D7 shape (GET /vaxia/transfer_xp/Zed: {label: label}) and
 * Drupal's autocomplete widget (?q=Zed: [{value, label}]).
 */
class TransferAutocomplete extends ControllerBase {

  public function matches(Request $request, $name = '') {
    $typed = $request->query->has('q') ? (string) $request->query->get('q') : (string) $name;
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(FALSE)
      ->condition('type', 'character_sheet')
      ->condition('status', 1)
      ->condition('uid', $this->currentUser()->id())
      ->condition('field_is_npc', 0)
      ->condition('field_is_storyteller', 0)
      ->condition('title', \Drupal::database()->escapeLike($typed) . '%', 'LIKE')
      ->sort('title');
    $labels = [];
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      if (Character::isPlayable($node)) {
        $label = Character::transferLabel($node);
        $labels[$label] = $label;
      }
    }
    if ($request->query->has('q')) {
      return new JsonResponse(array_map(fn($l) => ['value' => $l, 'label' => $l], array_values($labels)));
    }
    return new JsonResponse((object) $labels);
  }

}
