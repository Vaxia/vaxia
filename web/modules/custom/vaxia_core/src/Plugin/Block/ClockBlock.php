<?php

namespace Drupal\vaxia_core\Plugin\Block;

use Drupal\Component\Utility\Html;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Datetime\Entity\DateFormat;
use Drupal\Core\Render\Markup;
use Drupal\Core\StringTranslation\TranslatableMarkup;

/**
 * The clock (D7 contrib clock 7.x-1.2, block clock/clock) as production set
 * it up: clock_time_zone 2 ("User time zone"), clock_date_type "long" and
 * clock_update 1 (clock.js ticks it every second).
 */
#[Block(id: 'clock', admin_label: new TranslatableMarkup('Clock'))]
class ClockBlock extends BlockBase {

  /**
   * D7 clock_date_type.
   */
  const DATE_TYPE = 'long';

  /**
   * D7 clock_update.
   */
  const UPDATE = 1;

  public function build() {
    // D7 rendered this block on every page view (DRUPAL_NO_CACHE).
    \Drupal::service('page_cache_kill_switch')->trigger();
    // "User time zone": D11 sets PHP's default zone to the user's own (or the
    // site's when the user has none), as D7's date_now(NULL) used.
    $time_zone = date_default_timezone_get();
    $format = DateFormat::load(self::DATE_TYPE);
    $pattern = $format ? $format->getPattern() : 'l, F j, Y - H:i';
    $now = new \DateTime('@' . \Drupal::time()->getRequestTime());
    $now->setTimezone(new \DateTimeZone($time_zone));
    $clock = \Drupal::service('date.formatter')->format($now->getTimestamp(), 'custom', $pattern, $time_zone);
    return [
      '#markup' => Markup::create('<div class="clock">' . Html::escape($clock) . '</div>'),
      '#attached' => [
        'library' => ['vaxia_core/clock'],
        'drupalSettings' => [
          'clock' => [
            'time_zone' => $time_zone,
            'date_format' => $pattern,
            'update' => self::UPDATE,
            'local' => 0,
            'offset_name' => $now->format('T'),
            'offset_seconds' => $now->format('Z'),
            'daylight_savings_time' => $now->format('I'),
          ],
        ],
      ],
    ];
  }

  public function getCacheContexts() {
    return array_merge(parent::getCacheContexts(), ['timezone']);
  }

  public function getCacheMaxAge() {
    return 0;
  }

}
