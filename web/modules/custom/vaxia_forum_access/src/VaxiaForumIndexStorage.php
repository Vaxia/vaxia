<?php

namespace Drupal\vaxia_forum_access;

use Drupal\comment\CommentInterface;
use Drupal\Core\Database\Connection;
use Drupal\node\NodeInterface;
use Drupal\forum\ForumIndexStorage;

/**
 * Forum index reply counts from every node comment field, as D7 counted them
 * (contrib forum counts comment_forum only, so events, scenes and session
 * reports in a forum showed no replies). updateIndex() is contrib's with the
 * field condition widened.
 */
class VaxiaForumIndexStorage extends ForumIndexStorage {

  /**
   * {@inheritdoc}
   */
  public function updateIndex(NodeInterface $node) {
    $nid = $node->id();
    $count = $this->database->query("SELECT COUNT([cid]) FROM {comment_field_data} [c] INNER JOIN {forum_index} [i] ON [c].[entity_id] = [i].[nid] WHERE [c].[entity_id] = :nid AND [c].[field_name] IN (:fields[]) AND [c].[entity_type] = 'node' AND [c].[status] = :status AND [c].[default_langcode] = 1", [
      ':nid' => $nid,
      ':fields[]' => array_keys(\Drupal::service('entity_field.manager')->getFieldMapByFieldType('comment')['node'] ?? ['comment_forum' => []]),
      ':status' => CommentInterface::PUBLISHED,
    ])->fetchField();

    if ($count > 0) {
      // Comments exist.
      $last_reply = $this->database->queryRange("SELECT [cid], [name], [created], [uid] FROM {comment_field_data} WHERE [entity_id] = :nid AND [field_name] IN (:fields[]) AND [entity_type] = 'node' AND [status] = :status AND [default_langcode] = 1 ORDER BY [cid] DESC", 0, 1, [
        ':nid' => $nid,
      ':fields[]' => array_keys(\Drupal::service('entity_field.manager')->getFieldMapByFieldType('comment')['node'] ?? ['comment_forum' => []]),
        ':status' => CommentInterface::PUBLISHED,
      ])->fetchObject();
      $this->database->update('forum_index')
        ->fields([
          'comment_count' => $count,
          'last_comment_timestamp' => $last_reply->created,
        ])
        ->condition('nid', $nid)
        ->execute();
    }
    else {
      // Comments do not exist.
      // @todo This should be actually filtering on the desired node language
      $this->database->update('forum_index')
        ->fields([
          'comment_count' => 0,
          'last_comment_timestamp' => $node->getCreatedTime(),
        ])
        ->condition('nid', $nid)
        ->execute();
    }
  }

}
