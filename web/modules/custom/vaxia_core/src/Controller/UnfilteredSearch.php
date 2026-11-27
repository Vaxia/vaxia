<?php

namespace Drupal\vaxia_core\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Database\Query\PagerSelectExtender;
use Drupal\Core\Link;
use Symfony\Component\HttpFoundation\Request;

/**
 * Leads' unfiltered power search (D7 view tolex_power_search, display
 * search_unfiltered), laid out like the ported search/search page.
 */
class UnfilteredSearch extends ControllerBase {

  /**
   * Chat posts in every chat room, private, mature and locked ones included
   * (Limbo left out, as on D7), newest first, 35 a page.
   */
  public function search(Request $request) {
    $words = trim((string) $request->query->get('comment_body_value'));
    $build['header'] = ['#type' => 'processed_text', '#text' => 'This search page is the UNFILTERED lead only search so we can monitor all posts.', '#format' => 'wiki'];
    $build['form'] = [
      '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
      'keys' => ['#type' => 'textfield', '#title' => $this->t('Post text'), '#name' => 'comment_body_value', '#value' => $words],
      'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Search'), '#attributes' => ['type' => 'submit']],
    ];
    $rows = [];
    if ($words !== '') {
      $query = \Drupal::database()->select('comment_field_data', 'c')->extend(PagerSelectExtender::class);
      $query->join('comment__comment_body', 'b', 'b.entity_id = c.cid');
      $query->join('node_field_data', 'n', "n.nid = c.entity_id AND c.entity_type = 'node'");
      $query->leftJoin('comment__field_comment_character', 'cc', 'cc.entity_id = c.cid');
      $query->fields('c', ['cid', 'subject', 'uid', 'created'])->fields('n', ['nid', 'title'])->fields('cc', ['field_comment_character_target_id']);
      $query->condition('c.status', 1)->condition('n.status', 1)->condition('n.type', 'rpg_chatroom')->condition('n.title', 'Limbo', '<>')
        ->condition('b.comment_body_value', '%' . \Drupal::database()->escapeLike($words) . '%', 'LIKE')
        ->orderBy('c.created', 'DESC')->orderBy('c.cid', 'DESC')->limit(35);
      $formatter = \Drupal::service('date.formatter');
      foreach ($query->execute() as $row) {
        $character = $row->field_comment_character_target_id ? $this->entityTypeManager()->getStorage('node')->load($row->field_comment_character_target_id) : NULL;
        $author = $this->entityTypeManager()->getStorage('user')->load($row->uid);
        $rows[] = [
          Link::createFromRoute($row->title, 'entity.node.canonical', ['node' => $row->nid]),
          $character ? $character->label() : '',
          $author ? $author->getDisplayName() : '',
          $row->subject,
          $formatter->format($row->created, 'short'),
        ];
      }
    }
    $build['table'] = ['#type' => 'table', '#header' => [$this->t('Room'), $this->t('Character'), $this->t('Player'), $this->t('Post'), $this->t('Posted')], '#rows' => $rows, '#empty' => $words === '' ? $this->t('Enter words to search for.') : $this->t('No posts match.')];
    $build['pager'] = ['#type' => 'pager'];
    $build['#cache'] = ['contexts' => ['url.query_args', 'user.roles'], 'tags' => ['comment_list', 'node_list', 'user_list']];
    return $build;
  }

}
