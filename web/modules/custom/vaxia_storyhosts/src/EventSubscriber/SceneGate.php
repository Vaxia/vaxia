<?php

namespace Drupal\vaxia_storyhosts\EventSubscriber;

use Drupal\Core\Routing\RouteMatch;
use Drupal\Core\Url;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Symfony\Component\HttpKernel\Event\RequestEvent;
use Symfony\Component\HttpKernel\KernelEvents;

/**
 * Sends away anyone who may not see a scene (D7 scene_record_node_view).
 */
class SceneGate implements EventSubscriberInterface {

  public static function getSubscribedEvents(): array {
    return [KernelEvents::REQUEST => ['check', 30]];
  }

  public function check(RequestEvent $event) {
    $route = RouteMatch::createFromRequest($event->getRequest());
    $node = $route->getRouteName() === 'entity.node.canonical' ? $route->getParameter('node') : NULL;
    if ($node && $node->bundle() === 'scene' && !vaxia_storyhosts_scene_viewable($node)) {
      \Drupal::messenger()->addStatus(t("This is not your scene or you are not able to see other's scenes."));
      $event->setResponse(new RedirectResponse(Url::fromRoute('<front>')->toString()));
    }
  }

}
