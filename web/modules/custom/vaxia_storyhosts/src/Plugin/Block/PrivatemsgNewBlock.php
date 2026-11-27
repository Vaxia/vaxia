<?php

namespace Drupal\vaxia_storyhosts\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\Core\Url;

/**
 * "New message indication" (D7 privatemsg 7.x-1.4 block privatemsg-new): a
 * link to the oldest unread thread when the user has unread threads. Production
 * left "Display block when there are no new messages" off, so it's empty
 * otherwise. privatemsg 2.x for D11 has no such block (its privatemsg_block
 * is the links menu).
 */
#[Block(id: 'privatemsg-new', admin_label: new TranslatableMarkup('New message indication'))]
class PrivatemsgNewBlock extends BlockBase {

  protected function blockAccess(AccountInterface $account) {
    // D7 read privatemsg; privatemsg 2.x has one permission for reading and
    // writing. Never anonymous.
    return AccessResult::allowedIf($account->isAuthenticated() && $account->hasPermission('privatemsg write messages'))
      ->cachePerPermissions()
      ->cachePerUser();
  }

  public function build() {
    $uid = (int) \Drupal::currentUser()->id();
    $unread = vaxia_storyhosts_unread_pm($uid);
    $count = count($unread);
    if (!$count) {
      return [];
    }
    // Open the oldest unread thread itself (decided 2026-11-25): the inbox
    // lists threads by latest activity, so an old unread one could sit pages
    // down and the notice seemed not to clear.
    asort($unread);
    $thread = \Drupal::database()->query('SELECT id FROM {pm_index} WHERE owner = :uid AND `group` = :group AND deleted IS NULL', [':uid' => $uid, ':group' => array_key_first($unread)])->fetchField();
    return [
      '#type' => 'link',
      '#title' => $this->formatPlural($count, 'You have a new message! Click here to read it.', 'You have @count new messages! Click here to read them.'),
      '#url' => $thread ? Url::fromRoute('entity.privatemsg_thread.canonical', ['privatemsg_thread' => $thread]) : Url::fromUserInput('/messages'),
      '#attributes' => ['id' => 'privatemsg-new-link'],
    ];
  }

  public function getCacheContexts() {
    return array_merge(parent::getCacheContexts(), ['user']);
  }

  /**
   * New messages arrive without a cache tag to tell; D7 never cached it
   * either (DRUPAL_NO_CACHE).
   */
  public function getCacheMaxAge() {
    return 0;
  }

}
