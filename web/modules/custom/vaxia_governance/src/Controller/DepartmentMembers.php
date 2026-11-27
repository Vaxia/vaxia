<?php

namespace Drupal\vaxia_governance\Controller;

use Drupal\Core\Controller\ControllerBase;

/**
 * Department member lists (D7 vaxia_departments views).
 */
class DepartmentMembers extends ControllerBase {

  public function members(string $role) {
    $storage = $this->entityTypeManager()->getStorage('user');
    $rows = [];
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(FALSE)->condition('status', 1)->condition('roles', $role)->sort('name')->execute()) as $account) {
      $rows[] = [$account->toLink(), $account->getLastAccessedTime() ? \Drupal::service('date.formatter')->formatTimeDiffSince($account->getLastAccessedTime()) : $this->t('Never')];
    }
    return ['#type' => 'table', '#header' => [$this->t('Name'), $this->t('Last access')], '#rows' => $rows, '#empty' => $this->t('No members.'), '#cache' => ['max-age' => 0]];
  }

}
