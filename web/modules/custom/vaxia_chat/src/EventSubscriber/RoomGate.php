<?php

namespace Drupal\vaxia_chat\EventSubscriber;

use Drupal\Core\Routing\RouteMatch;
use Drupal\Core\Url;
use Drupal\vaxia_chat\Room;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Symfony\Component\HttpKernel\Event\RequestEvent;
use Symfony\Component\HttpKernel\KernelEvents;

/**
 * Sends visitors who may not open a room to the front page with the reason
 * (D7 vaxia_chatrooms_node_view(): drupal_goto() with a message).
 */
class RoomGate implements EventSubscriberInterface {

  public static function getSubscribedEvents(): array {
    // After routing (32) and access checks (33 in reverse order: run later).
    return [KernelEvents::REQUEST => ['check', 30]];
  }

  public function check(RequestEvent $event) {
    $route = RouteMatch::createFromRequest($event->getRequest());
    // The room page, the reply form, which shows the whole room and its
    // posts too, and the post edit form, which marks the editor present
    // (review, 2026-11-26: they let players into locked and mature rooms).
    $param = ['entity.node.canonical' => 'node', 'comment.reply' => 'entity', 'entity.comment.edit_form' => 'comment'][$route->getRouteName()] ?? NULL;
    if ($param === NULL) {
      return;
    }
    $room = $route->getParameter($param);
    if ($room instanceof \Drupal\comment\CommentInterface) {
      $room = $room->getCommentedEntity();
    }
    if (!Room::isRoom($room)) {
      return;
    }
    $reason = Room::denyReason($room, \Drupal::currentUser());
    if ($reason === NULL) {
      return;
    }
    if ($reason !== '') {
      \Drupal::messenger()->addStatus($reason);
      \Drupal::logger('vaxia_room')->notice('Turned away from room @nid: @reason', ['@nid' => $room->id(), '@reason' => $reason]);
    }
    $event->setResponse(new RedirectResponse(Url::fromRoute('<front>')->toString()));
  }

}
