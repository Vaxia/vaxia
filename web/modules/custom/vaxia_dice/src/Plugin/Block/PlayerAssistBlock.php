<?php

namespace Drupal\vaxia_dice\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\StringTranslation\TranslatableMarkup;

/**
 * The player multi-attack advisor (D7 block dice_ruler/dice_ruler_player_assist).
 */
#[Block(id: 'dice_ruler_player_assist', admin_label: new TranslatableMarkup('Dice ruler player assistant'))]
class PlayerAssistBlock extends BlockBase {

  public function build() {
    return \Drupal::formBuilder()->getForm('\Drupal\vaxia_dice\Form\PlayerAssistForm');
  }

  public function getCacheMaxAge() {
    return 0;
  }

}
