<?php

namespace Drupal\vaxia_chat\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\StringTranslation\TranslatableMarkup;

/**
 * Changes the page background by time of day (D7 background_changer).
 */
#[Block(id: 'background_changer', admin_label: new TranslatableMarkup('Change backgound image based on time'))]
class BackgroundChangerBlock extends BlockBase {

  public function build() {
    $build = ['#markup' => '&nbsp;', '#cache' => ['contexts' => ['user.permissions']]];
    if (\Drupal::currentUser()->hasPermission('use background changer')) {
      $path = \Drupal::service('extension.list.module')->getPath('vaxia_chat');
      $build['#attached']['library'][] = 'vaxia_chat/background_changer';
      $build['#attached']['drupalSettings']['background_changer']['path'] = \Drupal::request()->getSchemeAndHttpHost() . base_path() . $path . '/background_changer/images/';
    }
    return $build;
  }

}
