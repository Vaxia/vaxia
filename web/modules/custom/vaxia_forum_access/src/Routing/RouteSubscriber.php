<?php

namespace Drupal\vaxia_forum_access\Routing;

use Drupal\Core\Routing\RouteSubscriberBase;
use Symfony\Component\Routing\RouteCollection;

/**
 * Adds the forum Post grant to the comment reply route.
 */
class RouteSubscriber extends RouteSubscriberBase {

  protected function alterRoutes(RouteCollection $collection) {
    if ($route = $collection->get('comment.reply')) {
      $route->setRequirement('_vaxia_forum_access_reply', 'TRUE');
    }
  }

}
