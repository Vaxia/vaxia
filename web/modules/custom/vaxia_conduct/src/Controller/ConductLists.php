<?php

namespace Drupal\vaxia_conduct\Controller;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\user\UserInterface;
use Symfony\Component\HttpFoundation\Request;

/**
 * Conduct lists (D7 strikes, ban_players and cold_storage views).
 */
class ConductLists extends ControllerBase {

  private function nodes(string $type, ?int $player = NULL): array {
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(FALSE)->condition('type', $type)->condition('status', 1)->sort('field_date_of_incident', 'DESC')->pager(35);
    if ($player) {
      $query->condition('field_players', $player);
    }
    return $storage->loadMultiple($query->execute());
  }

  private static function names($items): string {
    $names = [];
    foreach ($items->referencedEntities() as $account) {
      $names[] = $account->getDisplayName();
    }
    return implode(', ', $names);
  }

  private static function labels($items): string {
    $options = $items->getFieldDefinition()->getFieldStorageDefinition()->getSetting('allowed_values');
    $labels = [];
    foreach ($items as $item) {
      $labels[] = $options[$item->value] ?? $item->value;
    }
    return implode(', ', $labels);
  }

  private function notationTable(array $nodes): array {
    $rows = [];
    foreach ($nodes as $node) {
      $rows[] = [
        $node->get('field_date_of_incident')->value,
        $node->toLink(),
        self::labels($node->get('field_category')),
        self::names($node->get('field_players')),
        self::names($node->get('field_witnesses')),
        self::names($node->get('field_social_members')),
        self::labels($node->get('field_results')),
      ];
    }
    return [
      'table' => [
        '#type' => 'table',
        '#header' => [$this->t('Date of Incident'), $this->t('Title'), $this->t('Category'), $this->t('Players'), $this->t('Witnesses'), $this->t('Social members'), $this->t('Results')],
        '#rows' => $rows,
        '#empty' => $this->t('No notes.'),
      ],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

  public function notations() {
    return $this->notationTable($this->nodes('notation'));
  }

  public function userNotations(UserInterface $user) {
    return $this->notationTable($this->nodes('notation', (int) $user->id()));
  }

  /**
   * Players see their own notes; notation editors see anyone's.
   */
  public static function userNotationsAccess(AccountInterface $account, UserInterface $user) {
    return AccessResult::allowedIf((int) $account->id() === (int) $user->id() || $account->hasPermission('edit any notation content'))->cachePerUser();
  }

  public function strikes() {
    $rows = [];
    foreach ($this->nodes('strike') as $node) {
      $record = $node->get('field_associated_record')->entity;
      $rows[] = [
        $node->get('field_date_of_incident')->value,
        $record ? $record->toLink() : '',
        self::names($node->get('field_struck_player')),
        self::labels($node->get('field_severity')),
        $node->toLink(),
      ];
    }
    return [
      'table' => [
        '#type' => 'table',
        '#header' => [$this->t('Date of Incident'), $this->t('Associated record'), $this->t('Player'), $this->t('Severity'), $this->t('Title')],
        '#rows' => $rows,
        '#empty' => $this->t('No strikes.'),
      ],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

  /**
   * Users with any of the roles, filtered by the name/mail query strings.
   */
  private function users(Request $request, array $roles, ?int $status = NULL): array {
    $storage = $this->entityTypeManager()->getStorage('user');
    $query = $storage->getQuery()->accessCheck(FALSE)->condition('roles', $roles, 'IN')->sort('name')->pager(35);
    if ($status !== NULL) {
      $query->condition('status', $status);
    }
    foreach (['name', 'mail'] as $key) {
      $value = trim((string) $request->query->get($key));
      if ($value !== '') {
        $query->condition($key, $value, 'CONTAINS');
      }
    }
    return $storage->loadMultiple($query->execute());
  }

  private function filterForm(Request $request): array {
    $form = ['#type' => 'container'];
    foreach (['name' => $this->t('Name'), 'mail' => $this->t('E-mail')] as $key => $label) {
      $form[$key] = ['#type' => 'textfield', '#title' => $label, '#name' => $key, '#value' => (string) $request->query->get($key)];
    }
    return [
      '#type' => 'html_tag',
      '#tag' => 'form',
      '#attributes' => ['method' => 'get'],
      'fields' => $form,
      'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']],
    ];
  }

  private static function date($timestamp): string {
    return $timestamp ? \Drupal::service('date.formatter')->format($timestamp, 'short') : '';
  }

  public function banned(Request $request) {
    $rows = [];
    foreach ($this->users($request, ['banned_user', 'suspended_user']) as $account) {
      $ips = [];
      foreach ($account->get('field_banned_ip') as $item) {
        $ips[] = $item->value;
      }
      $until = $account->get('field_banned_until')->value;
      $rows[] = [
        $account->toLink(),
        self::date($account->getCreatedTime()),
        self::date($account->getLastLoginTime()),
        implode(', ', $ips),
        $until ?: $this->t('PERMANENT'),
        $account->get('field_reason_for_ban')->value,
      ];
    }
    return [
      'filters' => $this->filterForm($request),
      'table' => [
        '#type' => 'table',
        '#header' => [$this->t('Name'), $this->t('Member for'), $this->t('Last login'), $this->t('Banned IP'), $this->t('Banned until'), $this->t('Reason for ban')],
        '#rows' => $rows,
        '#empty' => $this->t('No banned or suspended players.'),
      ],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

  public function coldStorage(Request $request) {
    $rows = [];
    foreach ($this->users($request, ['cold_storage'], 0) as $account) {
      $rows[] = [$account->toLink(), self::date($account->getCreatedTime()), self::date($account->getLastLoginTime())];
    }
    return [
      'help' => ['#markup' => '<p>' . $this->t('These players are in "cold storage". They were imported from an earlier version of the site, or have been deactivated as players for some reason. If the player returns, use this interface to find their account to reactivate it.') . '</p>'],
      'filters' => $this->filterForm($request),
      'table' => [
        '#type' => 'table',
        '#header' => [$this->t('Name'), $this->t('Member for'), $this->t('Last login')],
        '#rows' => $rows,
        '#empty' => $this->t('No players in cold storage.'),
      ],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

}
