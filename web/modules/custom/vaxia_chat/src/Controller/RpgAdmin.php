<?php

namespace Drupal\vaxia_chat\Controller;

use Drupal\Core\Controller\ControllerBase;

/**
 * The RPG admin section (admin/rpg): the RPG modules' settings pages.
 *
 * Not core's admin block page: that one denies access until a child page is
 * visible, and the RPG settings pages arrive with their modules.
 */
class RpgAdmin extends ControllerBase {

  public function page() {
    return \Drupal::service('system.manager')->getBlockContents() ?: ['#markup' => $this->t('You do not have any administrative items.')];
  }

}
