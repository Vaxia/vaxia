<?php

namespace Drupal\vaxia_core;

use Drupal\Core\Database\Connection;
use Drupal\Core\Messenger\MessengerInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\node\Entity\Node;
use Symfony\Component\HttpFoundation\RequestStack;

/**
 * Character XP pools, flavour XP and the sheet log (D7 character_sheet +
 * vaxia.character_sheet.inc).
 *
 * Pools: xp (earned), spent_xp, bxp (banked), rxp (roleplayed, unlocks banked
 * XP), kxp (posting; every vaxia_kxp_ratio becomes 1 xp + 1 rxp).
 */
class Xp {

  /**
   * The pools with a meaning of their own; any other type is flavour XP.
   */
  const MEANINGFUL = ['xp', 'bxp', 'rxp', 'kxp'];

  public function __construct(
    protected Connection $database,
    protected AccountInterface $currentUser,
    protected MessengerInterface $messenger,
    protected RequestStack $requestStack,
  ) {}

  /**
   * A pool's total (0 when unset).
   */
  public function get(int $nid, string $type = 'xp'): float {
    $xp = $this->database->query('SELECT xp FROM {character_sheet_xp} WHERE nid = :nid AND type = :type', [':nid' => $nid, ':type' => $type])->fetchField();
    return $xp === FALSE ? 0.0 : (float) $xp;
  }

  /**
   * Adds to a pool (negative to take away). D7 _character_sheet_update_xp().
   */
  public function update(int $nid, float $xp, string $type = 'xp'): void {
    if ($xp == 0) {
      return;
    }
    $this->database->merge('character_sheet_xp')
      ->keys(['nid' => $nid, 'type' => $type])
      ->fields(['xp' => $xp])
      ->expression('xp', 'xp + :xp', [':xp' => $xp])
      ->execute();
    \Drupal::entityTypeManager()->getStorage('node')->resetCache([$nid]);
  }

  public function deleteAll(int $nid): void {
    $this->database->delete('character_sheet_xp')->condition('nid', $nid)->execute();
    $this->database->delete('vaxia_xp_type_totals')->condition('nid', $nid)->execute();
  }

  /**
   * Flavour XP earned: type => amount.
   */
  public function flavours(int $nid): array {
    return $this->database->query('SELECT type, xp FROM {vaxia_xp_type_totals} WHERE nid = :nid ORDER BY type', [':nid' => $nid])->fetchAllKeyed();
  }

  public function addFlavour(int $nid, float $xp, string $type): void {
    $this->database->merge('vaxia_xp_type_totals')
      ->keys(['nid' => $nid, 'type' => $type])
      ->fields(['xp' => $xp])
      ->expression('xp', 'xp + :xp', [':xp' => $xp])
      ->execute();
  }

  public function log(int $nid, string $type, string $message): void {
    $request = $this->requestStack->getCurrentRequest();
    $this->database->insert('character_sheet_log')->fields([
      'nid' => $nid,
      'uid' => $this->currentUser->id(),
      'type' => $type,
      'message' => $message,
      'hostname' => $request ? (string) $request->getClientIp() : '',
      'timestamp' => \Drupal::time()->getRequestTime(),
    ])->execute();
  }

  /**
   * The latest log lines, newest first.
   */
  public function logLines(int $nid, int $limit = 20): array {
    $rows = $this->database->queryRange('SELECT message, timestamp FROM {character_sheet_log} WHERE nid = :nid ORDER BY timestamp DESC, lid DESC', 0, $limit, [':nid' => $nid]);
    $lines = [];
    foreach ($rows as $row) {
      $lines[] = '[' . \Drupal::service('date.formatter')->format($row->timestamp) . ' ] ' . $row->message;
    }
    return $lines;
  }

  public function deleteLog(int $nid): void {
    $this->database->delete('character_sheet_log')->condition('nid', $nid)->execute();
  }

  /**
   * Awards XP the Vaxia way (D7 _vaxia_add_xp()).
   *
   * - Flavour types also count as plain xp.
   * - kxp converts to 1 xp + 1 rxp per kxp ratio, and unlocks 1 banked xp.
   * - xp is spendable only as far as rxp covers it; the rest is banked.
   */
  public function award(int $nid, float $xp, string $type): void {
    $node = Node::load($nid);
    if (!$node) {
      return;
    }
    $message = (string) t('@xp @type XP added to @title by @user.', [
      '@xp' => $xp, '@type' => $type, '@title' => $node->label(), '@user' => $this->currentUser->getAccountName(),
    ]);
    if (!in_array($type, self::MEANINGFUL, TRUE)) {
      $this->addFlavour($nid, $xp, $type);
      $this->gained($node, $xp, $type);
      $type = 'xp';
    }
    switch ($type) {
      case 'kxp':
        $this->update($nid, $xp, 'kxp');
        $ratio = (float) \Drupal::config('vaxia_core.settings')->get('vaxia_kxp_ratio');
        if ($ratio > 0 && $this->get($nid, 'kxp') >= $ratio) {
          $this->update($nid, -$ratio, 'kxp');
          $this->update($nid, 1, 'rxp');
          $this->update($nid, 1, 'xp');
          $this->gained($node, 1, 'xp');
          if ($this->get($nid, 'bxp') > 0) {
            $this->update($nid, -1, 'bxp');
            $this->update($nid, 1, 'xp');
            $this->gained($node, 1, 'xp');
          }
        }
        break;

      case 'bxp':
      case 'rxp':
        $this->update($nid, $xp, $type);
        $this->gained($node, $xp, $type);
        break;

      default:
        $this->log($nid, 'xp', $message);
        $rxp = $this->get($nid, 'rxp');
        $spendable = (int) min(ceil(max($rxp, 0)), max($xp, 0));
        $banked = max($xp, 0) - $spendable;
        $this->update($nid, -$spendable, 'rxp');
        $this->update($nid, $spendable, 'xp');
        $this->update($nid, $banked, 'bxp');
        $this->gained($node, $spendable, 'xp');
        $this->messenger->addStatus($message);
    }
  }

  /**
   * Tells other modules a character gained XP (D7 Rules event vaxia_gain_xp):
   * hook_vaxia_gain_xp($player, $character, $amount, $type).
   */
  private function gained(Node $node, float $amount, string $type): void {
    $player = \Drupal\user\Entity\User::load($node->getOwnerId());
    if ($player) {
      \Drupal::moduleHandler()->invokeAll('vaxia_gain_xp', [$player, $node, $amount, $type]);
    }
  }

  /**
   * KXP for a post of this text (D7 _vaxia_figure_kxp()).
   */
  public function kxpFor(string $text): float {
    $kxp = 81.6 * (exp(0.00008 * strlen($text)) - 1);
    // D7 cut the number's string to 4 characters.
    $kxp = (float) substr((string) $kxp, 0, 4);
    $max = (float) \Drupal::config('vaxia_core.settings')->get('vaxia_max_kxp_post');
    return $kxp <= $max ? $kxp : $max;
  }

}
