<?php

namespace Drupal\vaxia_forum_access;

use Drupal\Component\Utility\DeprecationHelper;
use Drupal\history\HistoryManager;
use Drupal\comment\CommentManagerInterface;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Database\Connection;
use Drupal\Core\Database\Query\PagerSelectExtender;
use Drupal\Core\Database\Query\TableSortExtender;
use Drupal\Core\DependencyInjection\DependencySerializationTrait;
use Drupal\Core\Entity\EntityFieldManagerInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\StringTranslation\TranslationInterface;
use Drupal\node\NodeInterface;
use Drupal\forum\ForumManager;

/**
 * Forum listings for every node type filed into a forum. Contrib forum joins
 * comment statistics on the comment_forum field only, so events, scenes,
 * session reports and proposals in a forum (D7 listed them: forum_index
 * covered every node with taxonomy_forums) vanished from topic lists, topic
 * counts and "last post". getTopics(), getLastPost() and getForumStatistics()
 * are contrib's, with the statistics join taking any node comment field and
 * the topic's own comment field used for its comment status and new replies
 * (other types get no "N new posts" link: contrib's preprocess hard-codes
 * comment_forum there).
 */
class VaxiaForumManager extends ForumManager {

  /**
   * The site's node comment field names.
   */
  protected function vaxiaCommentFields(): array {
    return array_keys($this->entityFieldManager->getFieldMapByFieldType('comment')['node'] ?? ['comment_forum' => []]);
  }

  /**
   * The comment field of a topic's node type.
   */
  protected function vaxiaCommentField(NodeInterface $node): string {
    foreach ($node->getFieldDefinitions() as $name => $definition) {
      if ($definition->getType() === 'comment') {
        return $name;
      }
    }
    return 'comment_forum';
  }

  /**
   * {@inheritdoc}
   */
  public function getTopics($tid, AccountInterface $account) {
    $config = $this->configFactory->get('forum.settings');
    $forum_per_page = $config->get('topics.page_limit');
    $sortby = $config->get('topics.order');

    $header = [
      ['data' => $this->t('Topic'), 'field' => 'f.title'],
      ['data' => $this->t('Replies'), 'field' => 'f.comment_count'],
      ['data' => $this->t('Last reply'), 'field' => 'f.last_comment_timestamp'],
    ];

    $order = $this->getTopicOrder($sortby);
    for ($i = 0; $i < count($header); $i++) {
      if ($header[$i]['field'] == $order['field']) {
        $header[$i]['sort'] = $order['sort'];
      }
    }

    $query = $this->connection->select('forum_index', 'f')
      ->extend(PagerSelectExtender::class)
      ->extend(TableSortExtender::class);
    $query->fields('f');
    $query
      ->condition('f.tid', $tid)
      ->addTag('node_access')
      ->addMetaData('base_table', 'forum_index')
      ->orderBy('f.sticky', 'DESC')
      ->orderByHeader($header)
      ->limit($forum_per_page);

    $count_query = $this->connection->select('forum_index', 'f');
    $count_query->condition('f.tid', $tid);
    $count_query->addExpression('COUNT(*)');
    $count_query->addTag('node_access');
    $count_query->addMetaData('base_table', 'forum_index');

    $query->setCountQuery($count_query);
    $result = $query->execute();
    $nids = [];
    foreach ($result as $record) {
      $nids[] = $record->nid;
    }
    if ($nids) {
      $nodes = $this->entityTypeManager->getStorage('node')->loadMultiple($nids);

      $query = $this->connection->select('node_field_data', 'n')
        ->extend(TableSortExtender::class);
      $query->fields('n', ['nid']);

      $query->join('comment_entity_statistics', 'ces', "[n].[nid] = [ces].[entity_id] AND [ces].[field_name] IN (:vaxia_comment_fields[]) AND [ces].[entity_type] = 'node'", [':vaxia_comment_fields[]' => $this->vaxiaCommentFields()]);
      $query->fields('ces', [
        'cid',
        'last_comment_uid',
        'last_comment_timestamp',
        'comment_count',
      ]);

      $query->join('forum_index', 'f', '[f].[nid] = [n].[nid]');
      $query->addField('f', 'tid', 'forum_tid');

      $query->join('users_field_data', 'u', '[n].[uid] = [u].[uid] AND [u].[default_langcode] = 1');
      $query->addField('u', 'name');

      $query->join('users_field_data', 'u2', '[ces].[last_comment_uid] = [u2].[uid] AND [u].[default_langcode] = 1');

      $query->addExpression('CASE [ces].[last_comment_uid] WHEN 0 THEN [ces].[last_comment_name] ELSE [u2].[name] END', 'last_comment_name');

      $query
        ->orderBy('f.sticky', 'DESC')
        ->orderByHeader($header)
        ->condition('n.nid', $nids, 'IN')
        // @todo This should be actually filtering on the desired node language
        //   and just fall back to the default language.
        ->condition('n.default_langcode', 1);

      $result = [];
      foreach ($query->execute() as $row) {
        $topic = $nodes[$row->nid];
        $topic->comment_mode = $topic->get($this->vaxiaCommentField($topic))->status;

        foreach ($row as $key => $value) {
          $topic->{$key} = $value;
        }
        $result[] = $topic;
      }
    }
    else {
      $result = [];
    }

    $topics = [];
    $first_new_found = FALSE;
    foreach ($result as $topic) {
      if ($account->isAuthenticated()) {
        // A forum is new if the topic is new, or if there are new comments
        // since the user's last visit.
        if ($topic->forum_tid != $tid) {
          $topic->new = 0;
        }
        else {
          $history = $this->lastVisit($topic->id(), $account);
          // @phpstan-ignore-next-line
          $topic->new_replies = DeprecationHelper::backwardsCompatibleCall(\Drupal::VERSION, '11.3.0', fn() => \Drupal::service(HistoryManager::class)->getCountNewComments($topic, $this->vaxiaCommentField($topic), $history), fn() => $this->commentManager->getCountNewComments($topic, $this->vaxiaCommentField($topic), $history));
          $topic->new = $topic->new_replies || ($topic->last_comment_timestamp > $history);
          // template_preprocess_forums() pages to the new replies through the
          // comment_forum field, which other node types lack (fatal error):
          // they stay marked new, without the "N new posts" link.
          if ($this->vaxiaCommentField($topic) !== 'comment_forum') {
            $topic->new_replies = 0;
          }
        }
      }
      else {
        // Do not track "new replies" status for topics if the user is
        // anonymous.
        $topic->new_replies = 0;
        $topic->new = 0;
      }

      // Make sure only one topic is indicated as the first new topic.
      $topic->first_new = FALSE;
      if ($topic->new != 0 && !$first_new_found) {
        $topic->first_new = TRUE;
        $first_new_found = TRUE;
      }

      if ($topic->comment_count > 0) {
        $last_reply = new \stdClass();
        $last_reply->created = $topic->last_comment_timestamp;
        $last_reply->name = $topic->last_comment_name;
        $last_reply->uid = $topic->last_comment_uid;
        $topic->last_reply = $last_reply;
      }
      $topics[$topic->id()] = $topic;
    }

    return ['topics' => $topics, 'header' => $header];

  }

  /**
   * Provides the last post information for the given forum tid.
   *
   * @param int $tid
   *   The forum tid.
   *
   * @return object
   *   The last post for the given forum.
   */
  protected function getLastPost($tid) {
    if (!empty($this->lastPostData[$tid])) {
      return $this->lastPostData[$tid];
    }
    // Query "Last Post" information for this forum.
    $query = $this->connection->select('node_field_data', 'n');
    $query->join('forum', 'f', '[n].[vid] = [f].[vid] AND [f].[tid] = :tid', [':tid' => $tid]);
    $query->join('comment_entity_statistics', 'ces', "[n].[nid] = [ces].[entity_id] AND [ces].[field_name] IN (:vaxia_comment_fields[]) AND [ces].[entity_type] = 'node'", [':vaxia_comment_fields[]' => $this->vaxiaCommentFields()]);
    $query->join('users_field_data', 'u', '[ces].[last_comment_uid] = [u].[uid] AND [u].[default_langcode] = 1');
    $query->addExpression('CASE [ces].[last_comment_uid] WHEN 0 THEN [ces].[last_comment_name] ELSE [u].[name] END', 'last_comment_name');

    $topic = $query
      ->fields('ces', ['last_comment_timestamp', 'last_comment_uid'])
      ->condition('n.status', 1)
      ->orderBy('last_comment_timestamp', 'DESC')
      ->range(0, 1)
      ->addTag('node_access')
      ->execute()
      ->fetchObject();

    // Build the last post information.
    $last_post = new \stdClass();
    if (!empty($topic->last_comment_timestamp)) {
      $last_post->created = $topic->last_comment_timestamp;
      $last_post->name = $topic->last_comment_name;
      $last_post->uid = $topic->last_comment_uid;
    }

    $this->lastPostData[$tid] = $last_post;
    return $last_post;
  }

  /**
   * Provides statistics for a forum.
   *
   * @param int $tid
   *   The forum tid.
   *
   * @return object|null
   *   Statistics for the given forum if statistics exist, else NULL.
   */
  protected function getForumStatistics($tid) {
    if (empty($this->forumStatistics)) {
      // Prime the statistics.
      $query = $this->connection->select('node_field_data', 'n');
      $query->join('comment_entity_statistics', 'ces', "[n].[nid] = [ces].[entity_id] AND [ces].[field_name] IN (:vaxia_comment_fields[]) AND [ces].[entity_type] = 'node'", [':vaxia_comment_fields[]' => $this->vaxiaCommentFields()]);
      $query->join('forum', 'f', '[n].[vid] = [f].[vid]');
      $query->addExpression('COUNT([n].[nid])', 'topic_count');
      $query->addExpression('SUM([ces].[comment_count])', 'comment_count');
      $this->forumStatistics = $query
        ->fields('f', ['tid'])
        ->condition('n.status', 1)
        ->condition('n.default_langcode', 1)
        ->groupBy('tid')
        ->addTag('node_access')
        ->execute()
        ->fetchAllAssoc('tid');
    }

    if (!empty($this->forumStatistics[$tid])) {
      return $this->forumStatistics[$tid];
    }
  }

}
