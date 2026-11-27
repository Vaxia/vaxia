<?php

namespace Drupal\vaxia_core\EventSubscriber;

use Drupal\vaxia_core\Character;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use Symfony\Component\HttpKernel\KernelEvents;

/**
 * Runs workflow moves queued with Character::forceStateLater() before the
 * response goes out (and at the latest when the request ends).
 */
class DeferredStates implements EventSubscriberInterface {

  public static function getSubscribedEvents(): array {
    return [
      KernelEvents::RESPONSE => ['run', 1000],
      KernelEvents::TERMINATE => ['run', 1000],
    ];
  }

  public function run() {
    Character::runLater();
  }

}
