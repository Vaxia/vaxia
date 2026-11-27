<?php

namespace Drupal\vaxia_chat\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\node\NodeInterface;

/**
 * "I'm bored." on a room: posts a play suggestion (D7 play_suggestions).
 */
#[Block(id: 'play_suggestions', admin_label: new TranslatableMarkup('Play suggestions'))]
class PlaySuggestionsBlock extends BlockBase {

  public function build() {
    $node = \Drupal::routeMatch()->getParameter('node');
    if (!$node instanceof NodeInterface) {
      return ['#markup' => $this->t('Err: not a node.')];
    }
    return \Drupal::formBuilder()->getForm('\Drupal\vaxia_chat\Form\PlaySuggestionForm', $node);
  }

  public function getCacheMaxAge() {
    return 0;
  }

}
