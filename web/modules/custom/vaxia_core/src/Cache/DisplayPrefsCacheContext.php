<?php

namespace Drupal\vaxia_core\Cache;

use Drupal\Core\Cache\CacheableMetadata;
use Drupal\Core\Cache\Context\CacheContextInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\vaxia_core\DisplayPrefs;

/**
 * Pages vary by the set of display preferences, not by user, so cached pages
 * are shared by everyone with the same choices.
 *
 * Cache context ID: 'vaxia_display_prefs'.
 */
class DisplayPrefsCacheContext implements CacheContextInterface {

  public function __construct(protected AccountInterface $user) {}

  public static function getLabel() {
    return t('Display preferences');
  }

  public function getContext() {
    return implode('.', DisplayPrefs::enabled($this->user));
  }

  public function getCacheableMetadata() {
    return new CacheableMetadata();
  }

}
