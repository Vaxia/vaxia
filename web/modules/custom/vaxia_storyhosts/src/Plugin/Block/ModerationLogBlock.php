<?php

namespace Drupal\vaxia_storyhosts\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * On a character sheet's pages, for staff who may edit it: its latest
 * moderation log lines, daily silver left out (D7 view vaxia_moderation_log,
 * display moderation_log_block).
 */
#[Block(id: 'vaxia_moderation_log-moderation_log_block', admin_label: new TranslatableMarkup('Moderation log'))]
class ModerationLogBlock extends ViewBlockBase {

  const VIEW = 'vaxia_moderation_log';
  const DISPLAY = 'moderation_log_block';
  const TITLE = 'Character sheet moderation log';

  protected function blockAccess(AccountInterface $account) {
    return $this->roleAccess($account, ['administrator', 'lead', 'storyteller', 'storyteller_assistant', 'storyteller_candidate']);
  }

  public function build() {
    // The log table carries no cache tags.
    $cache = ['contexts' => ['route', 'user'], 'max-age' => 0];
    $node = \Drupal::routeMatch()->getParameter('node');
    if (is_numeric($node)) {
      $node = \Drupal::entityTypeManager()->getStorage('node')->load($node);
    }
    // D7: the node id from arg(1), valid only for a character sheet the user
    // may edit.
    if (!$node instanceof NodeInterface || $node->bundle() !== 'character_sheet' || !$node->access('update')) {
      return ['#cache' => $cache];
    }
    $query = \Drupal::database()->select('character_sheet_log', 'l')
      ->fields('l', ['uid', 'type', 'message', 'timestamp'])
      ->condition('l.nid', $node->id())
      // D7 "does not contain daily_silver".
      ->condition('l.type', '%daily\_silver%', 'NOT LIKE')
      ->orderBy('l.timestamp', 'DESC')
      ->orderBy('l.lid', 'DESC')
      // D7 paged 10 at a time; the block shows the first page.
      ->range(0, 10);
    $rows = [];
    $formatter = \Drupal::service('date.formatter');
    foreach ($query->execute() as $line) {
      $user = User::load($line->uid);
      $rows[] = [
        $line->type,
        // Lines logged by the system (cron) name the anonymous user, unlinked.
        $user && $user->id() ? ['data' => $user->toLink($user->getAccountName())->toRenderable()] : ($user ? $user->getDisplayName() : ''),
        $formatter->format($line->timestamp, 'short'),
        $line->message,
      ];
    }
    if (!$rows) {
      return ['#cache' => $cache];
    }
    return $this->view([
      'content' => [
        '#type' => 'table',
        '#header' => [$this->t('Log type'), $this->t('Logged user'), $this->t('Timestamp'), $this->t('Message')],
        '#rows' => $rows,
      ],
    ], $cache);
  }

}
