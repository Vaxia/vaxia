<?php

namespace Drupal\vaxia_chat;

use Drupal\comment\CommentInterface;
use Drupal\comment\CommentStorage;

/**
 * Comment storage whose display ordinal follows the chat rooms' newest-first
 * order (vaxia_chat_query_comment_filter_alter()). Core counts the posts
 * before a post in oldest-first order, so a permalink to a recent post opened
 * the room's oldest page. D7 comment_goodness overrode comment_permalink() for
 * the same reason.
 */
class RoomCommentStorage extends CommentStorage {

  public function getDisplayOrdinal(CommentInterface $comment, $comment_mode, $divisor = 1) {
    if (!Room::isRoom($comment->getCommentedEntity())) {
      return parent::getDisplayOrdinal($comment, $comment_mode, $divisor);
    }
    // Rooms list by cid, newest first, in flat and threaded mode alike: the
    // posts before this one are the newer ones.
    $data_table = $this->getDataTable();
    $query = $this->database->select($data_table, 'c1');
    $query->innerJoin($data_table, 'c2', '[c2].[entity_id] = [c1].[entity_id] AND [c2].[entity_type] = [c1].[entity_type] AND [c2].[field_name] = [c1].[field_name]');
    $query->addExpression('COUNT(*)', 'count');
    $query->condition('c2.cid', $comment->id());
    $query->where('[c1].[cid] > [c2].[cid]');
    if (!$this->currentUser->hasPermission('administer comments')) {
      $query->condition('c1.status', CommentInterface::PUBLISHED);
    }
    $query->condition('c1.default_langcode', 1);
    $query->condition('c2.default_langcode', 1);
    $ordinal = $query->execute()->fetchField();
    return ($divisor > 1) ? floor($ordinal / $divisor) : $ordinal;
  }

}
