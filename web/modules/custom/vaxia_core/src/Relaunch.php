<?php

namespace Drupal\vaxia_core;

use Drupal\Core\Session\AccountInterface;

/**
 * Relaunch (Phase 6): accepting the current terms and staff account
 * recovery (no announcement mail: decided 2026-11-25). Per-account records
 * live in user.data (module vaxia_core), so no schema change is needed.
 */
class Relaunch {

  /**
   * Settings (config vaxia_core.relaunch) and their defaults when unset.
   */
  const DEFAULTS = [
    'terms_path' => '/wiki/terms-and-conditions',
  ];

  public static function setting(string $key): string {
    $value = \Drupal::config('vaxia_core.relaunch')->get($key);
    return $value === NULL ? self::DEFAULTS[$key] : (string) $value;
  }

  // ---------------------------------------------------------------------------
  // Terms.
  // ---------------------------------------------------------------------------

  /**
   * The current terms version staff set; '' means nobody is asked yet. Kept
   * in state, not config, so a config import can't undo a bump.
   */
  public static function termsVersion(): string {
    return (string) \Drupal::state()->get('vaxia_core.terms_version', '');
  }

  public static function hasAcceptedTerms(AccountInterface $account): bool {
    $version = self::termsVersion();
    if ($version === '' || $account->isAnonymous()) {
      return TRUE;
    }
    return (string) \Drupal::service('user.data')->get('vaxia_core', (int) $account->id(), 'terms_version') === $version;
  }

  /**
   * Records that the account accepted the current terms (version + time).
   */
  public static function acceptTerms(int $uid): void {
    $data = \Drupal::service('user.data');
    $data->set('vaxia_core', $uid, 'terms_version', self::termsVersion());
    $data->set('vaxia_core', $uid, 'terms_accepted', \Drupal::time()->getCurrentTime());
    \Drupal::logger('vaxia_core')->info('User @uid accepted the terms, version @version.', ['@uid' => $uid, '@version' => self::termsVersion()]);
  }

  // ---------------------------------------------------------------------------
  // Account recovery.
  // ---------------------------------------------------------------------------

  /**
   * Staff email changes on an account, oldest first.
   *
   * @return array[]
   *   Each: time, staff (uid), old, new, note, link (bool).
   */
  public static function recoveryLog(int $uid): array {
    return (array) (\Drupal::service('user.data')->get('vaxia_core', $uid, 'recovery_log') ?? []);
  }

  public static function addRecoveryLog(int $uid, array $entry): void {
    $log = self::recoveryLog($uid);
    $log[] = $entry;
    \Drupal::service('user.data')->set('vaxia_core', $uid, 'recovery_log', $log);
  }

}
