<?php

namespace Drupal\vaxia_chat\Controller;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\node\NodeInterface;
use Drupal\vaxia_chat\Room;
use Symfony\Component\HttpFoundation\Request;

/**
 * Chat room pages (D7 views rpg_chats and chat_archives).
 */
class ChatLists extends ControllerBase {

  /**
   * Public locations: published rooms that show in Power Search
   * (field_private_room = 1), not mature and not locked, by title, 50 a page.
   * Filters: title contains, realm (any of).
   */
  public function rooms(Request $request) {
    $title = trim((string) $request->query->get('title'));
    $realms = array_values(array_filter(array_map('intval', (array) ($request->query->all()['field_tag_realm_tid'] ?? []))));
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(FALSE)
      ->condition('type', 'rpg_chatroom')->condition('status', 1)
      ->condition('field_private_room', 1)->condition('field_locked_to_users', 0)->condition('field_mature_posting', 0);
    if ($title !== '') {
      $query->condition('title', $title, 'CONTAINS');
    }
    if ($realms) {
      $query->condition('field_tag_realm', $realms, 'IN');
    }
    $header = [['data' => $this->t('Chatroom'), 'field' => 'title', 'specifier' => 'title', 'sort' => 'asc']];
    $query->tableSort($header)->pager(50);
    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      $rows[] = [$node->toLink()];
    }
    // Realm choices with their hierarchy, as D7's exposed filter showed them.
    $options = [];
    foreach ($this->entityTypeManager()->getStorage('taxonomy_term')->loadTree('realm') as $term) {
      $options[$term->tid] = str_repeat('-', $term->depth) . $term->name;
    }
    $realm_select = ['#type' => 'html_tag', '#tag' => 'select', '#attributes' => ['name' => 'field_tag_realm_tid[]', 'id' => 'edit-field-tag-realm-tid', 'multiple' => 'multiple']];
    foreach ($options as $tid => $label) {
      $realm_select[$tid] = ['#type' => 'html_tag', '#tag' => 'option', '#value' => $label, '#attributes' => ['value' => $tid] + (in_array((int) $tid, $realms, TRUE) ? ['selected' => 'selected'] : [])];
    }
    return [
      'filter' => [
        '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
        'title' => ['#type' => 'textfield', '#title' => $this->t('Title'), '#name' => 'title', '#value' => $title],
        'realm_label' => ['#type' => 'html_tag', '#tag' => 'label', '#value' => $this->t('Realm'), '#attributes' => ['for' => 'edit-field-tag-realm-tid']],
        'realm' => $realm_select,
        'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']],
      ],
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['contexts' => ['url.query_args'], 'tags' => ['node_list', 'taxonomy_term_list']],
    ];
  }

  /**
   * The "Earlier Posts" tab: a room's published posts, newest first, 35 a
   * page, optionally between two dates.
   */
  public function archives(NodeInterface $node, Request $request) {
    // D7's header marked the visitor as in the room.
    Room::markPresent((int) $node->id());
    // Rooms open to anonymous visitors: the page cache ignores max-age.
    \Drupal::service('page_cache_kill_switch')->trigger();
    $created = (array) ($request->query->all()['created'] ?? []);
    $min = trim((string) ($created['min'] ?? ''));
    $max = trim((string) ($created['max'] ?? ''));
    $query = \Drupal::database()->select('comment_field_data', 'c')->fields('c', ['cid'])
      ->condition('c.entity_type', 'node')->condition('c.entity_id', $node->id())->condition('c.status', 1)
      ->orderBy('c.cid', 'DESC');
    $errors = [];
    foreach (['min' => '>=', 'max' => '<='] as $key => $op) {
      $value = $key === 'min' ? $min : $max;
      if ($value === '') {
        continue;
      }
      $time = strtotime($value);
      if ($time === FALSE) {
        $errors[] = $this->t('Invalid date format.');
        continue;
      }
      $query->condition('c.created', $time, $op);
    }
    $cids = $errors ? [] : $query->extend('Drupal\Core\Database\Query\PagerSelectExtender')->limit(35)->execute()->fetchCol();
    $builder = $this->entityTypeManager()->getViewBuilder('comment');
    // The room's own wrappers, so the chat post styles apply as in the room
    // (user, 2026-11-25: Earlier Posts should look like the room).
    $posts = [
      '#type' => 'container',
      '#attributes' => ['class' => ['rpg-chat-node']],
      '#prefix' => '<div class="rpg-chat-wrapper rpg-chat-archive"><div class="rpg-chat" id="rpg-chat">',
      '#suffix' => '</div></div>',
      '#attached' => ['library' => ['vaxia_chat/chat_styles']],
    ];
    foreach ($this->entityTypeManager()->getStorage('comment')->loadMultiple($cids) as $cid => $comment) {
      // Built now so the links (D7 showed none) can be dropped.
      $post = $builder->build($builder->view($comment));
      unset($post['links']);
      $posts[$cid] = $post;
    }
    return [
      'filter' => [
        '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
        'label' => ['#markup' => '<div class="form-item"><label>' . $this->t('Posted date') . '</label>'],
        'min' => ['#type' => 'textfield', '#title' => $this->t('Min'), '#name' => 'created[min]', '#value' => $min],
        'max' => ['#type' => 'textfield', '#title' => $this->t('Max'), '#name' => 'created[max]', '#value' => $max],
        'description' => ['#markup' => '<div class="description">' . $this->t('Show posts between given dates.') . '</div></div>'],
        'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']],
      ],
      'errors' => $errors ? ['#markup' => '<div class="messages messages--error">' . reset($errors) . '</div>'] : [],
      'posts' => $posts,
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

  /**
   * Rooms only; the visitor must be able to view the room and, unlike D7,
   * open it (mature/locked rooms; D7 showed their posts here to anyone).
   */
  public function archivesAccess(NodeInterface $node, AccountInterface $account) {
    if ($node->bundle() !== 'rpg_chatroom') {
      return AccessResult::forbidden()->addCacheableDependency($node);
    }
    return AccessResult::allowedIfHasPermission($account, 'access content')
      ->andIf($node->access('view', $account, TRUE))
      ->andIf(AccessResult::allowedIf(Room::denyReason($node, $account) === NULL))
      ->setCacheMaxAge(0);
  }

}
