<?php

namespace Drupal\vaxia_core\EventSubscriber;

use Drupal\Core\Routing\LocalRedirectResponse;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Url;
use Drupal\vaxia_core\Relaunch;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use Symfony\Component\HttpKernel\Event\RequestEvent;
use Symfony\Component\HttpKernel\KernelEvents;

/**
 * Sends a logged-in user who hasn't accepted the current terms version to the
 * acceptance page, from any page but the few they need meanwhile: logging
 * out, the terms themselves, the one-time login / password reset flow and
 * setting a new password on their own account form.
 */
class TermsGate implements EventSubscriberInterface {

  /**
   * Routes open to everyone (assets, errors, login and reset flow).
   */
  const OPEN_ROUTES = [
    'vaxia_core.terms_accept',
    'user.logout',
    'user.logout.confirm',
    'user.login',
    'user.pass',
    'user.reset',
    'user.reset.form',
    'user.reset.login',
    'system.403',
    'system.404',
    'system.4xx',
    'system.css_asset',
    'system.js_asset',
    'system.files',
    'system.private_file_download',
    'system.temporary',
    'system.cron',
    'system.csrftoken',
    'image.style_public',
    'image.style_private',
    'masquerade.unmasquerade',
  ];

  public function __construct(protected AccountInterface $currentUser) {
  }

  public static function getSubscribedEvents(): array {
    // After routing and access (30), before the dynamic page cache (27).
    return [KernelEvents::REQUEST => ['check', 28]];
  }

  public function check(RequestEvent $event) {
    if (!$event->isMainRequest() || $this->currentUser->isAnonymous() || Relaunch::hasAcceptedTerms($this->currentUser)) {
      return;
    }
    $request = $event->getRequest();
    $route = (string) $request->attributes->get('_route');
    if (in_array($route, self::OPEN_ROUTES, TRUE)) {
      return;
    }
    $user = $request->attributes->get('user');
    if ($route === 'entity.user.edit_form' && $user && (int) (is_object($user) ? $user->id() : $user) === (int) $this->currentUser->id()) {
      return;
    }
    $terms = Relaunch::setting('terms_path');
    if ($terms !== '') {
      $terms = '/' . ltrim($terms, '/');
      $current = \Drupal::service('path.current')->getPath($request);
      if ($current === $terms || $current === \Drupal::service('path_alias.manager')->getPathByAlias($terms)) {
        return;
      }
    }
    $url = Url::fromRoute('vaxia_core.terms_accept', [], ['query' => \Drupal::destination()->getAsArray()])->toString();
    $event->setResponse(new LocalRedirectResponse($url));
  }

}
