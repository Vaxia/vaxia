<?php

namespace Drupal\vaxia_characters\EventSubscriber;

use Drupal\Core\Routing\RouteMatch;
use Drupal\Core\Url;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Symfony\Component\HttpKernel\Event\RequestEvent;
use Symfony\Component\HttpKernel\KernelEvents;

/**
 * Sends players away from sheets that are out of play (D7 vaxia_characters).
 */
class SheetGate implements EventSubscriberInterface {

  public static function getSubscribedEvents(): array {
    return [KernelEvents::REQUEST => ['check', 30]];
  }

  public function check(RequestEvent $event) {
    $route = RouteMatch::createFromRequest($event->getRequest());
    // The sheet page, and the reply form, which shows the whole sheet too
    // (review, 2026-11-26).
    $param = ['entity.node.canonical' => 'node', 'comment.reply' => 'entity'][$route->getRouteName()] ?? NULL;
    $node = $param ? $route->getParameter($param) : NULL;
    if ($node instanceof \Drupal\node\NodeInterface && ($reason = vaxia_characters_sheet_gate($node))) {
      \Drupal::messenger()->addStatus($reason);
      $event->setResponse(new RedirectResponse(Url::fromRoute('<front>')->toString()));
    }
  }

}
