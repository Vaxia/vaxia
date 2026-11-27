<?php

namespace Drupal\vaxia_forum_access\EventSubscriber;

use Drupal\Core\Cache\CacheableResponseInterface;
use Drupal\Core\Routing\RouteMatchInterface;
use Drupal\vaxia_forum_access\ForumAccess;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use Symfony\Component\HttpKernel\Event\ResponseEvent;
use Symfony\Component\HttpKernel\KernelEvents;

/**
 * The forum index and forum pages depend on the viewer's grants.
 *
 * The forum module drops the cacheability of its term access checks and topic
 * queries, so without this the dynamic page cache could show one role's
 * forum list to another role with the same permissions.
 */
class ForumPageCache implements EventSubscriberInterface {

  public function __construct(protected RouteMatchInterface $routeMatch) {}

  public static function getSubscribedEvents(): array {
    // Before DynamicPageCacheSubscriber::onResponse (100) stores the response.
    return [KernelEvents::RESPONSE => [['onResponse', 110]]];
  }

  public function onResponse(ResponseEvent $event) {
    $response = $event->getResponse();
    if (!$response instanceof CacheableResponseInterface || !in_array($this->routeMatch->getRouteName(), ['forum.index', 'forum.page'], TRUE)) {
      return;
    }
    $response->getCacheableMetadata()
      ->addCacheContexts(['user.roles', 'user.node_grants:view'])
      ->addCacheTags([ForumAccess::CACHE_TAG]);
  }

}
