<?php

namespace Drupal\vaxia_wiki\Plugin\Block;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\Core\Url;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Wiki page search by published status, title and category, most recently
 * updated first, with the start of each page's text (D7 view site_wiki_admin,
 * display site_wiki_search; its "input required" form showed no pages until a
 * search was made).
 */
#[Block(id: 'site_wiki_admin-site_wiki_search', admin_label: new TranslatableMarkup('Site wiki search'))]
class WikiSearchBlock extends ViewBlockBase {

  const VIEW = 'site_wiki_admin';
  const DISPLAY = 'site_wiki_search';
  const TITLE = 'Site wiki search';

  /**
   * Category operators (D7 taxonomy filter operators).
   */
  const OPERATORS = ['or' => 'Is one of', 'and' => 'Is all of', 'not' => 'Is none of'];

  protected function blockAccess(AccountInterface $account) {
    return AccessResult::allowedIfHasPermission($account, 'access content');
  }

  public function build() {
    $query_args = \Drupal::request()->query;
    $status = (string) $query_args->get('status', '1');
    // Security (D7 let anyone list unpublished pages, e.g. those flagged for
    // deletion): only users who may view unpublished wiki pages (D7's
    // view_unpublished permission: administrators and leads) search them.
    $staff = \Drupal::currentUser()->hasPermission('view any unpublished wiki_page content');
    if (!$staff) {
      $status = '1';
    }
    $title = trim((string) $query_args->get('title', ''));
    $category = trim((string) $query_args->get('field_additional_tagging_tid', ''));
    $operator = (string) $query_args->get('field_additional_tagging_tid_op', 'or');
    if (!isset(self::OPERATORS[$operator])) {
      $operator = 'or';
    }
    $cache = [
      'tags' => ['node_list', 'taxonomy_term_list'],
      'contexts' => ['user.permissions', 'url.path', 'url.query_args:status', 'url.query_args:title', 'url.query_args:field_additional_tagging_tid', 'url.query_args:field_additional_tagging_tid_op'],
    ];

    $form = [
      '#type' => 'html_tag',
      '#tag' => 'form',
      '#attributes' => ['method' => 'get', 'action' => Url::fromRoute('<current>')->toString(), 'class' => ['views-exposed-form']],
      'status' => [
        '#type' => 'select',
        '#title' => $this->t('Published'),
        '#name' => 'status',
        '#id' => 'edit-status-site-wiki-search',
        '#options' => ['All' => $this->t('- Any -'), '1' => $this->t('Yes'), '0' => $this->t('No')],
        '#value' => $status,
        '#access' => $staff,
      ],
      'title' => [
        '#type' => 'textfield',
        '#title' => $this->t('Title'),
        '#name' => 'title',
        '#id' => 'edit-title-site-wiki-search',
        '#value' => $title,
        '#size' => 30,
      ],
      'operator' => [
        '#type' => 'select',
        '#title' => $this->t('Category'),
        '#name' => 'field_additional_tagging_tid_op',
        '#id' => 'edit-field-additional-tagging-tid-op-site-wiki-search',
        '#options' => self::OPERATORS,
        '#value' => $operator,
      ],
      'category' => [
        '#type' => 'textfield',
        '#title' => $this->t('Category'),
        '#title_display' => 'invisible',
        '#description' => $this->t('Category names, separated by commas.'),
        '#name' => 'field_additional_tagging_tid',
        '#id' => 'edit-field-additional-tagging-tid-site-wiki-search',
        '#value' => $category,
        '#size' => 30,
      ],
      'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => 'Apply', '#attributes' => ['type' => 'submit', 'value' => 'Apply']],
    ];

    $items = [];
    $searched = $query_args->has('status') || $query_args->has('title') || $query_args->has('field_additional_tagging_tid');
    if ($searched) {
      $storage = \Drupal::entityTypeManager()->getStorage('node');
      // Node grants hide unpublished nodes from queries: users who may view
      // unpublished wiki pages query wiki pages without them.
      $query = $storage->getQuery()->accessCheck(!$staff)
        ->condition('type', 'wiki_page')
        ->sort('changed', 'DESC')
        // D7 paged 35 at a time; the block shows the first page.
        ->range(0, 35);
      if ($status === '1' || $status === '0') {
        $query->condition('status', (int) $status);
      }
      if ($title !== '') {
        $query->condition('title', $title, 'CONTAINS');
      }
      if ($category !== '') {
        $tids = [];
        foreach (array_filter(array_map('trim', explode(',', $category))) as $term_name) {
          $found = \Drupal::entityQuery('taxonomy_term')->accessCheck(FALSE)->condition('vid', 'wiki_tags')->condition('name', $term_name)->execute();
          // A name that is no category matches no page.
          $tids[] = $found ? (int) reset($found) : 0;
        }
        if ($operator === 'or') {
          $query->condition('field_additional_tagging', $tids, 'IN');
        }
        elseif ($operator === 'and') {
          foreach ($tids as $tid) {
            $query->condition($query->andConditionGroup()->condition('field_additional_tagging', $tid));
          }
        }
        else {
          $tagged = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'wiki_page')->condition('field_additional_tagging', $tids, 'IN')->execute();
          if ($tagged) {
            $query->condition('nid', $tagged, 'NOT IN');
          }
        }
      }
      $renderer = \Drupal::service('renderer');
      foreach ($storage->loadMultiple($query->execute()) as $node) {
        $body = $node->get('body')->first();
        $text = '';
        if ($body && $body->value !== '') {
          $processed = ['#type' => 'processed_text', '#text' => $body->value, '#format' => $body->format];
          $text = trim(html_entity_decode(strip_tags((string) $renderer->renderInIsolation($processed)), ENT_QUOTES));
          // D7 trim: 140 characters, at a word boundary, with an ellipsis.
          if (mb_strlen($text) > 140) {
            $text = mb_substr($text, 0, 140);
            $text = preg_replace('/\s+\S*$/u', '', $text) . '...';
          }
        }
        $items[] = [
          'title' => $node->toLink()->toRenderable(),
          'separator' => ['#markup' => ' - '],
          'label' => ['#markup' => '<span class="views-label views-label-body">' . $this->t('Type') . '</span> '],
          'body' => ['#markup' => htmlspecialchars($text)],
        ];
      }
    }
    return $this->view([
      'header' => [
        'style' => ['#type' => 'html_tag', '#tag' => 'style', '#value' => '#block-views-wiki-admin-content-wiki-search .view-wiki-admin-content{border:1px solid;padding:0.5em} #views-exposed-form-wiki-admin-content-wiki-search .views-exposed-widgets .views-exposed-widget{float:left;} #views-exposed-form-wiki-admin-content-wiki-search .views-exposed-widgets .views-exposed-widget input.form-text{width:80px;}'],
        'text' => ['#markup' => $this->t('Search for a wiki article by tag(s).')],
      ],
      'exposed' => $form,
      'content' => $items ? ['#theme' => 'item_list', '#items' => $items] : NULL,
      'empty' => !$items && $searched ? $this->t('No wiki pages found matching, or no search performed.') : NULL,
    ], $cache);
  }

}
