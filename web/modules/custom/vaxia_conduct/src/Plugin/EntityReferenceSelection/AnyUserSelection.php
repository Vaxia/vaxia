<?php

namespace Drupal\vaxia_conduct\Plugin\EntityReferenceSelection;

use Drupal\Core\Entity\Attribute\EntityReferenceSelection;
use Drupal\Core\Entity\Plugin\EntityReferenceSelection\DefaultSelection;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\user\Plugin\EntityReferenceSelection\UserSelection;

/**
 * Users, blocked ones included: staff note and strike suspended players too
 * (core hides blocked accounts from anyone without "administer users").
 */
#[EntityReferenceSelection(
  id: 'vaxia_conduct:user',
  label: new TranslatableMarkup('Any user, blocked included'),
  entity_types: ['user'],
  group: 'vaxia_conduct',
  weight: 1
)]
class AnyUserSelection extends UserSelection {

  protected function buildEntityQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = DefaultSelection::buildEntityQuery($match, $match_operator);
    $configuration = $this->getConfiguration();
    if (!$configuration['include_anonymous']) {
      $query->condition('uid', 0, '<>');
    }
    if (isset($match)) {
      $query->condition('name', $match, $match_operator);
    }
    if (!empty($configuration['filter']['role'])) {
      $query->condition('roles', $configuration['filter']['role'], 'IN');
    }
    return $query;
  }

}
