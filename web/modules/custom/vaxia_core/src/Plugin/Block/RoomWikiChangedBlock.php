<?php

namespace Drupal\vaxia_core\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * In a chat room: a warning when the room's wiki description changed since
 * the user last read it (D7 view vaxia_rpg_wiki_view, block display).
 */
#[Block(id: 'vaxia_rpg_wiki_view-block', admin_label: new TranslatableMarkup('Vaxia RPG Wiki View'))]
class RoomWikiChangedBlock extends ViewBlockBase {

  const VIEW = 'vaxia_rpg_wiki_view';
  const DISPLAY = 'block';
  const TITLE = 'Vaxia RPG Wiki View';

  protected function blockAccess(AccountInterface $account) {
    return AccessResult::allowedIfHasPermission($account, 'access content');
  }

  public function build() {
    // Reading a page (history) invalidates no cache tag.
    $cache = ['contexts' => ['route', 'user'], 'max-age' => 0];
    $room = \Drupal::routeMatch()->getParameter('node');
    if (is_numeric($room)) {
      $room = \Drupal::entityTypeManager()->getStorage('node')->load($room);
    }
    if (!$room instanceof NodeInterface || $room->bundle() !== 'rpg_chatroom' || !$room->isPublished() || $room->get('field_wiki_source')->isEmpty()) {
      return ['#cache' => $cache];
    }
    $uid = (int) \Drupal::currentUser()->id();
    $limit = \Drupal::time()->getRequestTime() - 30 * 86400;
    foreach ($room->get('field_wiki_source')->referencedEntities() as $wiki) {
      // D7 "Has new content": not read since it (or a comment on it) changed;
      // never read counts when it changed in the last 30 days. Anonymous
      // users were not filtered.
      if ($uid) {
        $read = \Drupal::database()->query('SELECT timestamp FROM {history} WHERE uid = :uid AND nid = :nid', [':uid' => $uid, ':nid' => $wiki->id()])->fetchField();
        $commented = (int) \Drupal::database()->query("SELECT MAX(last_comment_timestamp) FROM {comment_entity_statistics} WHERE entity_type = 'node' AND entity_id = :nid", [':nid' => $wiki->id()])->fetchField();
        $new = $read === FALSE
          ? ($wiki->getChangedTime() > $limit || $commented > $limit)
          : ((int) $read < $wiki->getChangedTime() || (int) $read < $commented);
        if (!$new) {
          continue;
        }
      }
      // One row (D7 showed the first match).
      $text = $this->t('This room description was updated on @changed, please <a href=":url">review description changes</a>.', [
        '@changed' => \Drupal::service('date.formatter')->format($wiki->getChangedTime(), 'short'),
        ':url' => $wiki->toUrl()->toString(),
      ]);
      return $this->view(['content' => ['#markup' => $text]], $cache, ['messages--warning', 'messages', 'warning']);
    }
    return ['#cache' => $cache];
  }

}
