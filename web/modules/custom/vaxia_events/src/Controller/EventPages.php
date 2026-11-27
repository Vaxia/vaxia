<?php

namespace Drupal\vaxia_events\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\node\NodeInterface;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;

/**
 * Calendar pages (D7 site_events views) and the wiki timeline (D7
 * timeline_events view, TimelineJS).
 */
class EventPages extends ControllerBase {

  /**
   * Published, scheduled events from $from on.
   */
  private function events(?string $from = NULL): array {
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'event')->condition('status', 1)->condition('field_event_status', 'event_status_scheduled')->sort('field_when.value');
    if ($from) {
      $query->condition('field_when.value', $from, '>=');
    }
    return $storage->loadMultiple($query->execute());
  }

  private function when(NodeInterface $event): string {
    $value = $event->get('field_when')->value;
    return $value ? \Drupal::service('date.formatter')->format(strtotime($value . ' UTC'), 'medium') : '';
  }

  private function table(array $events): array {
    $rows = [];
    foreach ($events as $event) {
      $realm = $event->get('field_tag_realm')->entity;
      $rows[] = [$this->when($event), $event->toLink(), $realm ? $realm->label() : '', $event->getOwner()->getDisplayName()];
    }
    return ['#type' => 'table', '#header' => [$this->t('When'), $this->t('Event'), $this->t('Realm'), $this->t('Organizer')], '#rows' => $rows, '#empty' => $this->t('No events.'), '#cache' => ['max-age' => 0]];
  }

  /**
   * This month's events.
   */
  public function calendar() {
    $start = gmdate('Y-m-01\T00:00:00');
    return $this->table(array_filter($this->events($start), fn($e) => $e->get('field_when')->value < gmdate('Y-m-01\T00:00:00', strtotime('first day of next month'))));
  }

  public function upcoming() {
    return $this->table($this->events(gmdate('Y-m-d\TH:i:s')));
  }

  /**
   * Upcoming events as an iCalendar feed.
   */
  public function ical() {
    $lines = ['BEGIN:VCALENDAR', 'VERSION:2.0', 'PRODID:-//VAXIA//Events//EN'];
    foreach ($this->events(gmdate('Y-m-d\TH:i:s', strtotime('-1 month'))) as $event) {
      $when = $event->get('field_when')->first();
      $lines[] = 'BEGIN:VEVENT';
      // Unique per site and stable whatever host name the feed is fetched by:
      // the site's UUID.
      $lines[] = 'UID:event-' . $event->id() . '@' . \Drupal::config('system.site')->get('uuid');
      $lines[] = 'SUMMARY:' . addcslashes($event->label(), ',;');
      $lines[] = 'DTSTART:' . gmdate('Ymd\THis\Z', strtotime($when->value . ' UTC'));
      if ($when->end_value) {
        $lines[] = 'DTEND:' . gmdate('Ymd\THis\Z', strtotime($when->end_value . ' UTC'));
      }
      $lines[] = 'URL:' . $event->toUrl('canonical', ['absolute' => TRUE])->toString();
      $lines[] = 'END:VEVENT';
    }
    $lines[] = 'END:VCALENDAR';
    return new Response(implode("\r\n", $lines) . "\r\n", 200, ['Content-Type' => 'text/calendar; charset=utf-8']);
  }

  /**
   * Timeline of one playable realm's timeline events and session reports
   * (the first unless another realm is picked). The data is what TimelineJS reads.
   */
  public function timeline(Request $request) {
    $realms = \Drupal::entityTypeManager()->getStorage('taxonomy_term')->loadByProperties(['vid' => 'realm']);
    $options = [];
    // The playable realms: every realm but the administrative one. The
    // first, by weight, unless one is picked.
    uasort($realms, fn($a, $b) => [$a->getWeight(), $a->label()] <=> [$b->getWeight(), $b->label()]);
    foreach ($realms as $term) {
      if ($term->label() !== \Drupal\vaxia_core\Character::adminRealm()) {
        $options[$term->id()] = $term->label();
      }
    }
    $tid = (int) $request->query->get('field_tag_realm_tid');
    if (!isset($options[$tid])) {
      $tid = (int) array_key_first($options);
    }
    $storage = $this->entityTypeManager()->getStorage('node');
    $ids = $tid ? $storage->getQuery()->accessCheck(TRUE)->condition('type', ['timeline_event', 'session_report'], 'IN')->condition('status', 1)->condition('field_tag_realm', $tid)->exists('field_session_held')->sort('field_session_held.value')->execute() : [];
    $events = [];
    foreach ($storage->loadMultiple($ids) as $node) {
      $held = $node->get('field_session_held')->first();
      $events[] = [
        'start_date' => ['year' => (int) substr($held->value, 0, 4), 'month' => (int) substr($held->value, 5, 2), 'day' => (int) substr($held->value, 8, 2)],
        'text' => ['headline' => $node->label(), 'text' => mb_substr(strip_tags((string) ($node->hasField('body') ? $node->get('body')->value : '')), 0, 300)],
        'unique_id' => 'node-' . $node->id(),
      ];
    }
    $links = [];
    foreach ($options as $id => $label) {
      $links[] = ['#type' => 'link', '#title' => $label, '#url' => \Drupal\Core\Url::fromRoute('vaxia_events.timeline', [], ['query' => ['field_tag_realm_tid' => $id]])];
    }
    return [
      'realms' => ['#theme' => 'item_list', '#items' => $links],
      'timeline' => ['#type' => 'html_tag', '#tag' => 'div', '#attributes' => ['id' => 'timeline-embed', 'data-timeline' => json_encode(['events' => $events])]],
      'list' => ['#theme' => 'item_list', '#items' => array_map(fn($e) => $e['start_date']['year'] . '-' . $e['start_date']['month'] . '-' . $e['start_date']['day'] . ': ' . $e['text']['headline'], $events)],
      '#cache' => ['max-age' => 0],
    ];
  }

}
