<?php

namespace Drupal\vaxia_chat\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\Core\Url;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Room search by name and realm: public, unlocked, non-mature rooms, most
 * recently active first (D7 view rpg_chats, display search_rpg_chats; its
 * "input required" form showed no rooms until a search was made).
 */
#[Block(id: 'rpg_chats-search_rpg_chats', admin_label: new TranslatableMarkup('Search RPG Chats'))]
class RoomSearchBlock extends ViewBlockBase {

  const VIEW = 'rpg_chats';
  const DISPLAY = 'search_rpg_chats';
  const TITLE = 'RPG Chats';

  /**
   * Query key of the name box.
   */
  const NAME_KEY = 'title';

  /**
   * Query key of the realm list (NULL: no realm filter).
   */
  const REALM_KEY = 'field_tag_realm_tid';

  const BUTTON = 'Apply';

  protected function blockAccess(AccountInterface $account) {
    return $this->roleAccess($account, ['authenticated']);
  }

  /**
   * Whether a search was made (D7 "input required": any filter key present,
   * even empty).
   */
  protected function searched(string $name): bool {
    $query = \Drupal::request()->query;
    return $query->has(static::NAME_KEY) || (static::REALM_KEY && $query->has(static::REALM_KEY));
  }

  /**
   * Text shown when there are no rooms (NULL: nothing).
   */
  protected function emptyText(bool $searched): ?string {
    return $searched ? (string) $this->t('No rooms found matching this name, or no search performed.') : NULL;
  }

  protected function footer(): ?string {
    return NULL;
  }

  public function build() {
    $request = \Drupal::request();
    $name = trim((string) $request->query->get(static::NAME_KEY, ''));
    $realms = [];
    if (static::REALM_KEY) {
      $realms = array_values(array_filter(array_map('intval', (array) ($request->query->all()[static::REALM_KEY] ?? []))));
    }
    $contexts = ['url.path', 'url.query_args:' . static::NAME_KEY];
    if (static::REALM_KEY) {
      $contexts[] = 'url.query_args:' . static::REALM_KEY;
    }
    $cache = ['tags' => ['node_list', 'comment_list', 'taxonomy_term_list'], 'contexts' => $contexts];

    // The search form (GET, to this page).
    $form = [
      '#type' => 'html_tag',
      '#tag' => 'form',
      '#attributes' => ['method' => 'get', 'action' => Url::fromRoute('<current>')->toString(), 'class' => ['views-exposed-form']],
      'name' => [
        '#type' => 'textfield',
        '#title' => $this->t('Room name'),
        '#title_display' => 'invisible',
        '#name' => static::NAME_KEY,
        '#id' => 'edit-' . static::NAME_KEY . '-' . str_replace('_', '-', static::DISPLAY),
        '#value' => $name,
        '#size' => 20,
      ],
    ];
    if (static::REALM_KEY) {
      $options = [];
      foreach (\Drupal::entityTypeManager()->getStorage('taxonomy_term')->loadTree('realm') as $term) {
        $options[$term->tid] = $term->name;
      }
      $form['realm'] = [
        '#type' => 'select',
        '#title' => $this->t('Realm'),
        '#name' => static::REALM_KEY . '[]',
        '#id' => 'edit-' . str_replace('_', '-', static::REALM_KEY) . '-' . str_replace('_', '-', static::DISPLAY),
        '#options' => $options,
        '#value' => array_map('strval', $realms),
        '#attributes' => ['multiple' => 'multiple'],
      ];
    }
    $form['submit'] = ['#type' => 'html_tag', '#tag' => 'button', '#value' => static::BUTTON, '#attributes' => ['type' => 'submit', 'value' => static::BUTTON]];

    $searched = $this->searched($name);
    $items = [];
    if ($searched) {
      $query = \Drupal::database()->select('node_field_data', 'n');
      $query->join('node__field_locked_to_users', 'l', 'l.entity_id = n.nid');
      $query->join('node__field_mature_posting', 'm', 'm.entity_id = n.nid');
      $query->join('node__field_private_room', 'p', 'p.entity_id = n.nid');
      $query->leftJoin('comment_entity_statistics', 's', "s.entity_id = n.nid AND s.entity_type = 'node'");
      $query->fields('n', ['nid']);
      $query->addExpression('MAX(s.last_comment_timestamp)', 'last_comment');
      $query->condition('n.type', 'rpg_chatroom')
        ->condition('n.status', 1)
        ->condition('l.field_locked_to_users_value', 0)
        ->condition('m.field_mature_posting_value', 0)
        // "Posts in this room show in Power Search" (public rooms).
        ->condition('p.field_private_room_value', 1)
        ->condition('n.title', '%' . $query->escapeLike($name) . '%', 'LIKE')
        ->groupBy('n.nid')
        ->orderBy('last_comment', 'DESC')
        ->range(0, 5)
        ->addTag('node_access');
      if ($realms) {
        $query->join('node__field_tag_realm', 'r', 'r.entity_id = n.nid');
        $query->condition('r.field_tag_realm_target_id', $realms, 'IN');
      }
      $nids = $query->execute()->fetchCol();
      foreach (\Drupal::entityTypeManager()->getStorage('node')->loadMultiple($nids) as $room) {
        $realm = implode(', ', array_map(fn($term) => $term->label(), $room->get('field_tag_realm')->referencedEntities()));
        $items[] = [
          'title' => $room->toLink()->toRenderable(),
          'realm' => ['#markup' => $realm === '' ? '' : ' (' . htmlspecialchars($realm) . ')'],
        ];
      }
    }
    return $this->view([
      'header' => $this->t('Search for a room by name.'),
      'exposed' => $form,
      'content' => $items ? ['#theme' => 'item_list', '#items' => $items] : NULL,
      'empty' => $items ? NULL : $this->emptyText($searched),
      'footer' => $this->footer(),
    ], $cache);
  }

}
