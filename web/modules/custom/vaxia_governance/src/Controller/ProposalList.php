<?php

namespace Drupal\vaxia_governance\Controller;

use Drupal\Core\Controller\ControllerBase;

/**
 * Proposals and elections (D7 proposal views; both D7 pages claimed
 * /proposals, so the elections list never showed).
 */
class ProposalList extends ControllerBase {

  private function rows(array $nodes): array {
    $rows = [];
    $formatter = \Drupal::service('date.formatter');
    foreach ($nodes as $node) {
      $open = vaxia_governance_is_open($node);
      $rows[] = [
        $node->toLink(),
        $formatter->format($node->getChangedTime(), 'short'),
        $formatter->format($node->getCreatedTime(), 'short'),
        $open ? date('l, F j, Y', strtotime(vaxia_governance_setting('proposal_open_timeframe'), $node->getChangedTime())) : '',
        $open ? $this->t('Open') : $this->t('Closed'),
        $node->get('field_proposal_type')->value,
      ];
    }
    return $rows;
  }

  public function page() {
    $storage = $this->entityTypeManager()->getStorage('node');
    $header = [$this->t('Title'), $this->t('Updated'), $this->t('Created'), $this->t('Closes on'), $this->t('Status'), $this->t('Type')];
    $elections = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'proposal')->condition('status', 1)->condition('field_proposal_type', 'election')->sort('created', 'DESC')->range(0, 1)->execute();
    $proposals = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'proposal')->condition('status', 1)->sort('created', 'DESC')->pager(15)->execute();
    return [
      'elections' => [
        '#type' => 'details', '#title' => $this->t('Elections'), '#open' => TRUE,
        'table' => ['#type' => 'table', '#header' => $header, '#rows' => $this->rows($storage->loadMultiple($elections)), '#empty' => $this->t('No elections.')],
      ],
      'proposals' => ['#type' => 'table', '#caption' => $this->t('Proposals'), '#header' => $header, '#rows' => $this->rows($storage->loadMultiple($proposals)), '#empty' => $this->t('No proposals.')],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

}
