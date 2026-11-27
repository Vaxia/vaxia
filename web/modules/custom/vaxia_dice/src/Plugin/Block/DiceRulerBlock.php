<?php

namespace Drupal\vaxia_dice\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;

/**
 * The dice ruler (D7 block dice_ruler/dice_ruler), for "use dice ruler".
 */
#[Block(id: 'dice_ruler', admin_label: new TranslatableMarkup('Dice ruler'))]
class DiceRulerBlock extends BlockBase {

  public function build() {
    return \Drupal::formBuilder()->getForm('\Drupal\vaxia_dice\Form\DiceRulerForm');
  }

  protected function blockAccess(AccountInterface $account) {
    return AccessResult::allowedIfHasPermission($account, 'use dice ruler');
  }

  public function getCacheMaxAge() {
    return 0;
  }

}
