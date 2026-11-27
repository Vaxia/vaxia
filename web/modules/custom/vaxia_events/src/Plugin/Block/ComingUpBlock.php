<?php

namespace Drupal\vaxia_events\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * The next five scheduled session announcements, from today on (D7 view
 * calendar_block, display coming_up).
 */
#[Block(id: 'calendar_block-coming_up', admin_label: new TranslatableMarkup('Calendar: Coming up'))]
class ComingUpBlock extends ViewBlockBase {

  const VIEW = 'calendar_block';
  const DISPLAY = 'coming_up';
  const TITLE = 'Coming up';

  /**
   * The "Session announcements" forum (production term id).
   */
  const FORUM = 879;

  protected function blockAccess(AccountInterface $account) {
    return AccessResult::allowedIfHasPermission($account, 'access content');
  }

  public function build() {
    // D7 compared by day: events starting any time today (site time) or later.
    $timezone = new \DateTimeZone(\Drupal::config('system.date')->get('timezone.default') ?: 'UTC');
    $today = new \DateTime('today', $timezone);
    $tomorrow = new \DateTime('tomorrow', $timezone);
    $from = (clone $today)->setTimezone(new \DateTimeZone('UTC'))->format('Y-m-d\TH:i:s');
    $cache = [
      'tags' => ['node_list'],
      'contexts' => ['timezone'],
      'max-age' => max(0, $tomorrow->getTimestamp() - \Drupal::time()->getRequestTime()),
    ];
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(TRUE)
      ->condition('type', 'event')
      ->condition('status', 1)
      ->condition('taxonomy_forums', self::FORUM)
      ->condition('field_event_status', 'event_status_scheduled')
      ->condition('field_when.value', $from, '>=')
      ->sort('field_when.value')
      ->range(0, 5)
      ->execute();
    if (!$ids) {
      return ['#cache' => $cache];
    }
    $formatter = \Drupal::service('date.formatter');
    $items = [];
    foreach ($storage->loadMultiple($ids) as $event) {
      $start = $event->get('field_when')->value;
      $items[] = [
        'title' => $event->toLink()->toRenderable(),
        // D7 "very short" date format, start only.
        'when' => ['#markup' => ' ' . $formatter->format(strtotime($start . ' UTC'), 'custom', 'm/d/y h:ia')],
      ];
    }
    return $this->view(['content' => ['#theme' => 'item_list', '#items' => $items]], $cache);
  }

}
