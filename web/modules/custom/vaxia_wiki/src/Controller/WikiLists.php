<?php

namespace Drupal\vaxia_wiki\Controller;

use Drupal\Core\Cache\CacheableMetadata;
use Drupal\Core\Cache\CacheableResponse;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Url;
use Symfony\Component\HttpFoundation\Request;

/**
 * Wiki lists (D7 vaxia_wiki and site_wiki views).
 */
class WikiLists extends ControllerBase {

  private function table(array $header, array $rows, string $empty): array {
    return [
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows, '#empty' => $empty],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

  private function date($timestamp): string {
    return \Drupal::service('date.formatter')->format($timestamp, 'short');
  }

  public function recent() {
    $storage = $this->entityTypeManager()->getStorage('node');
    $rows = [];
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(TRUE)->condition('type', 'wiki_page')->condition('status', 1)->sort('changed', 'DESC')->pager(50)->execute()) as $node) {
      $rows[] = [$node->toLink(), ($node->getOwner() ? $node->getOwner()->getDisplayName() : ''), $this->date($node->getChangedTime()), $node->getRevisionUser() ? $node->getRevisionUser()->getDisplayName() : ''];
    }
    return $this->table([$this->t('Title'), $this->t('Author'), $this->t('Updated'), $this->t('Last edited by')], $rows, $this->t('No wiki pages.'));
  }

  /**
   * Pages flagged delete/rewrite/expand, published or not (D7 listed only
   * published pages, so flagged-for-deletion pages, which it unpublished,
   * never showed).
   */
  public function flagged(string $flag) {
    $nids = \Drupal::database()->query("SELECT entity_id FROM {flagging} WHERE flag_id = :flag AND entity_type = 'node'", [':flag' => $flag])->fetchCol();
    $rows = [];
    foreach ($this->entityTypeManager()->getStorage('node')->loadMultiple($nids) as $node) {
      $rows[] = [$node->toLink(), $this->date($node->getChangedTime()), $node->isPublished() ? $this->t('Published') : $this->t('Unpublished'), $node->toLink($this->t('edit'), 'edit-form')];
    }
    return $this->table([$this->t('Title'), $this->t('Updated'), $this->t('Status'), $this->t('Edit')], $rows, $this->t('No flagged pages.'));
  }

  public function content() {
    $storage = $this->entityTypeManager()->getStorage('node');
    $rows = [];
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(FALSE)->condition('type', 'wiki_page')->sort('title')->pager(50)->execute()) as $node) {
      $rows[] = [$node->toLink(), $this->date($node->getChangedTime()), $node->toLink($this->t('edit'), 'edit-form'), $node->toLink($this->t('delete'), 'delete-form')];
    }
    return $this->table([$this->t('Title'), $this->t('Updated'), $this->t('Edit'), $this->t('Delete')], $rows, $this->t('No wiki pages.'));
  }

  public function npcs() {
    $storage = $this->entityTypeManager()->getStorage('node');
    $rows = [];
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(TRUE)->condition('type', 'character_sheet')->condition('status', 1)->condition('field_is_npc', 1)->sort('title')->pager(50)->execute()) as $node) {
      $realm = $node->hasField('field_tag_realm') ? $node->get('field_tag_realm')->entity : NULL;
      $rows[] = [$node->hasField('field_titles') ? (string) $node->get('field_titles')->value : '', $node->toLink(), ($node->getOwner() ? $node->getOwner()->getDisplayName() : ''), $realm ? $realm->label() : ''];
    }
    return $this->table([$this->t('Titles'), $this->t('Name'), $this->t('Player'), $this->t('Realm')], $rows, $this->t('No NPCs.'));
  }

  public function organizations() {
    $storage = $this->entityTypeManager()->getStorage('node');
    $rows = [];
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(TRUE)->condition('type', 'wiki_page')->condition('status', 1)->exists('field_organization_type')->sort('title')->pager(50)->execute()) as $node) {
      $type = $node->get('field_organization_type')->entity;
      $rows[] = [$node->toLink(), $type ? $type->label() : ''];
    }
    return $this->table([$this->t('Organization'), $this->t('Type')], $rows, $this->t('No organizations.'));
  }

  public function governance() {
    $storage = $this->entityTypeManager()->getStorage('node');
    $rows = [];
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(TRUE)->condition('type', 'proposal')->condition('status', 1)->condition('field_proposal_type', 'amendment')->sort('changed', 'DESC')->pager(50)->execute()) as $node) {
      $rows[] = [$node->toLink(), $this->date($node->getChangedTime())];
    }
    return $this->table([$this->t('Amendment'), $this->t('Updated')], $rows, $this->t('No amendments.'));
  }

  /**
   * One realm's published session reports, latest first (D7 had a list
   * for each of its two realms; this serves every realm).
   */
  public function sessions(string $realm) {
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'session_report')->condition('status', 1)->sort('created', 'DESC')->pager(50);
    // The realm whose name, lower case with dashes, is the path's.
    $tids = [];
    foreach ($this->entityTypeManager()->getStorage('taxonomy_term')->loadByProperties(['vid' => 'realm']) as $term) {
      if (trim(preg_replace('/[^a-z0-9]+/', '-', strtolower($term->label())), '-') === $realm) {
        $tids[] = (int) $term->id();
        $realm = $term->label();
      }
    }
    if (!$tids) {
      throw new \Symfony\Component\HttpKernel\Exception\NotFoundHttpException();
    }
    $query->condition('field_tag_realm', $tids, 'IN');
    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      $saga = $node->hasField('field_saga') ? $node->get('field_saga')->entity : NULL;
      $rows[] = [$node->toLink(), ($node->getOwner() ? $node->getOwner()->getDisplayName() : ''), $saga ? $saga->label() : '', $this->date($node->getCreatedTime())];
    }
    return ['#title' => $this->t('@realm Sessions', ['@realm' => $realm])] + $this->table([$this->t('Session'), $this->t('Storyhost'), $this->t('Saga'), $this->t('Held')], $rows, $this->t('No sessions.'));
  }

  /**
   * Wiki search: titles and text.
   */
  public function search(Request $request) {
    $keys = trim((string) $request->query->get('keys'));
    $build['form'] = [
      '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
      'keys' => ['#type' => 'textfield', '#title' => $this->t('Search the wiki'), '#name' => 'keys', '#value' => $keys],
      'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Search'), '#attributes' => ['type' => 'submit']],
    ];
    $rows = [];
    if ($keys !== '') {
      $storage = $this->entityTypeManager()->getStorage('node');
      $query = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'wiki_page')->condition('status', 1)->sort('title')->pager(50);
      $or = $query->orConditionGroup()->condition('title', $keys, 'CONTAINS')->condition('body', $keys, 'CONTAINS');
      foreach ($storage->loadMultiple($query->condition($or)->execute()) as $node) {
        $rows[] = [$node->toLink(), mb_substr(trim(strip_tags((string) $node->get('body')->value)), 0, 200)];
      }
    }
    return $build + $this->table([$this->t('Page'), $this->t('Excerpt')], $rows, $keys === '' ? $this->t('Enter words to search for.') : $this->t('No wiki pages match.'));
  }

  /**
   * One realm's published session reports as an iCalendar feed, latest
   * session first (D7 wiki_display_sessions feed_1/feed_2, date_ical).
   * Reports without a "session held" date have no event, as on D7.
   * D7's event descriptions (the rendered report) are left out.
   */
  public function sessionCalendar(string $realm) {
    $storage = $this->entityTypeManager()->getStorage('node');
    // The realm whose name, lower case with dashes, is the path's.
    $tids = [];
    foreach ($this->entityTypeManager()->getStorage('taxonomy_term')->loadByProperties(['vid' => 'realm']) as $term) {
      if (trim(preg_replace('/[^a-z0-9]+/', '-', strtolower($term->label())), '-') === $realm) {
        $tids[] = (int) $term->id();
        $realm = $term->label();
      }
    }
    if (!$tids) {
      throw new \Symfony\Component\HttpKernel\Exception\NotFoundHttpException();
    }
    $ids = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'session_report')->condition('status', 1)->condition('field_tag_realm', $tids ?: [0], 'IN')->sort('field_session_held.value', 'DESC')->execute();
    $lines = ['BEGIN:VCALENDAR', 'VERSION:2.0', 'PRODID:-//VAXIA//Sessions//EN', 'METHOD:PUBLISH', 'X-WR-CALNAME:' . $realm . ' Sessions'];
    foreach ($storage->loadMultiple($ids) as $node) {
      $held = $node->get('field_session_held')->value;
      if (!$held) {
        continue;
      }
      $lines[] = 'BEGIN:VEVENT';
      // Unique per site and stable whatever host name the feed is fetched by:
      // the site's UUID.
      $lines[] = 'UID:session-' . $node->id() . '@' . \Drupal::config('system.site')->get('uuid');
      $lines[] = 'DTSTAMP:' . gmdate('Ymd\THis\Z');
      $lines[] = 'SUMMARY:' . addcslashes($node->label(), ',;');
      // Day granularity (as on D7): all-day events.
      $lines[] = 'DTSTART;VALUE=DATE:' . str_replace('-', '', substr($held, 0, 10));
      $lines[] = 'URL:' . $node->toUrl('canonical', ['absolute' => TRUE])->toString();
      $lines[] = 'END:VEVENT';
    }
    $lines[] = 'END:VCALENDAR';
    $response = new CacheableResponse(implode("\r\n", $lines) . "\r\n", 200, ['Content-Type' => 'text/calendar; charset=utf-8']);
    $response->addCacheableDependency((new CacheableMetadata())->setCacheTags(['node_list', 'taxonomy_term_list'])->setCacheContexts(['user.permissions', 'user.node_grants:view']));
    return $response;
  }

  /**
   * Admin list of wiki pages with exposed filters on publishing and on each
   * wiki flag (D7 site_wiki_admin, display site_wiki_flagged_admin). With the
   * flag filters on "- Any -" every wiki page is listed.
   */
  public function flaggedAdmin(Request $request) {
    $q = $request->query;
    $status = (string) $q->get('status', '1');
    $title = trim((string) $q->get('title'));
    $flags = ['expand' => [$this->t('Flagged to be expanded'), 'flagged_expand'], 'rewrite' => [$this->t('Flagged for rewrite'), 'flagged_rewrite'], 'delete' => [$this->t('Flagged for delete'), 'flagged_delete']];
    $any = ['All' => $this->t('- Any -')];
    $build['filters'] = [
      '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
      'status' => ['#type' => 'select', '#title' => $this->t('Published'), '#name' => 'status', '#options' => $any + ['1' => $this->t('Yes'), '0' => $this->t('No')], '#value' => $status],
      'title' => ['#type' => 'textfield', '#title' => $this->t('Title'), '#name' => 'title', '#value' => $title],
    ];
    $storage = $this->entityTypeManager()->getStorage('node');
    $header = [
      'title' => ['data' => $this->t('Title'), 'field' => 'title', 'specifier' => 'title'],
      'changed' => ['data' => $this->t('Last updated'), 'field' => 'changed', 'specifier' => 'changed', 'sort' => 'desc'],
      'operations' => $this->t('Operations'),
    ];
    $query = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'wiki_page')->tableSort($header)->pager(35);
    if ($status === '1' || $status === '0') {
      $query->condition('status', (int) $status);
    }
    if ($title !== '') {
      $query->condition('title', $title, 'CONTAINS');
    }
    foreach ($flags as $flag => [$label, $key]) {
      $value = (string) $q->get($key, 'All');
      $build['filters'][$key] = ['#type' => 'select', '#title' => $label, '#name' => $key, '#options' => $any + ['1' => $this->t('Flagged'), '0' => $this->t('Not flagged')], '#value' => $value];
      if ($value === '1' || $value === '0') {
        $flagged = \Drupal::database()->query("SELECT entity_id FROM {flagging} WHERE flag_id = :flag AND entity_type = 'node'", [':flag' => $flag])->fetchCol();
        $query->condition('nid', $flagged ?: [0], $value === '1' ? 'IN' : 'NOT IN');
      }
    }
    $build['filters']['submit'] = ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']];
    $rows = [];
    $formatter = \Drupal::service('date.formatter');
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      $rows[] = [
        $node->toLink(),
        $formatter->format($node->getChangedTime(), 'medium'),
        ['data' => [
          'edit' => ['#type' => 'link', '#title' => $this->t('edit'), '#url' => $node->toUrl('edit-form'), '#suffix' => ' '],
          'delete' => ['#type' => 'link', '#title' => $this->t('delete'), '#url' => $node->toUrl('delete-form')],
        ]],
      ];
    }
    $build['table'] = ['#type' => 'table', '#header' => $header, '#rows' => $rows, '#empty' => $this->t('No articles flagged for deletion.')];
    $build['pager'] = ['#type' => 'pager'];
    $build['#cache'] = ['contexts' => ['url.query_args', 'user.roles'], 'tags' => ['node_list', 'flagging_list']];
    return $build;
  }

}
