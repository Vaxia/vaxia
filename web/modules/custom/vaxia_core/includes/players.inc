<?php

/**
 * @file
 * Players (D7 players, vaxia_players, vaxia_rpg): registration, profiles,
 * mailing players their characters, XP over time, and the site's
 * notification mails.
 */

use Drupal\Core\Form\FormStateInterface;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\SiteTexts;
use Drupal\vaxia_core\DisplayPrefs;

/**
 * A D7 rule's text with its tokens filled in.
 */
function vaxia_core_rule_text(string $key, string $part, array $tokens = []): string {
  $tokens += ['[site:name]' => (string) \Drupal::config('system.site')->get('name')];
  return strtr(SiteTexts::TEXTS[$key][$part], $tokens);
}

/**
 * Mails a rule's text to an address.
 */
function vaxia_core_rule_mail(string $key, string $to, array $tokens = []): void {
  \Drupal::service('plugin.manager.mail')->mail('vaxia_core', 'text', $to, 'en', [
    'subject' => vaxia_core_rule_text($key, 'subject', $tokens),
    'body' => vaxia_core_rule_text($key, 'message', $tokens),
  ]);
}

/**
 * Mails a rule's text to every active user with any of the roles.
 */
function vaxia_core_rule_mail_roles(string $key, array $roles, array $tokens = []): void {
  $ids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', $roles, 'IN')->execute();
  foreach (User::loadMultiple($ids) as $account) {
    vaxia_core_rule_mail($key, $account->getEmail(), $tokens);
  }
}

/**
 * Implements hook_mail().
 */
function vaxia_core_mail($key, &$message, $params) {
  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];
}

/**
 * Implements hook_vaxia_gain_xp(): spendable XP a character gains adds to
 * its player's "XP over time" (D7 started no total for a player who had none).
 */
function vaxia_core_vaxia_gain_xp(UserInterface $player, NodeInterface $character, $amount, string $type) {
  if ($type !== 'xp' || $amount <= 0 || !$player->hasField('field_xp_over_time')) {
    return;
  }
  $player->set('field_xp_over_time', (float) $player->get('field_xp_over_time')->value + $amount);
  $player->save();
}

/**
 * Registration: under-age accounts are blocked (D7 "User is under minimal
 * age"; its mail lost the account name).
 */
function vaxia_core_player_presave(UserInterface $account): void {
  if ($account->isNew() && $account->hasField('field_age_requirement') && !$account->get('field_age_requirement')->isEmpty() && !$account->get('field_age_requirement')->value) {
    $account->removeRole('new_account');
    $account->block();
    $account->vaxiaUnderage = TRUE;
  }
}

/**
 * Implements hook_ENTITY_TYPE_insert() for users.
 */
function vaxia_core_user_insert(UserInterface $account) {
  if (!empty($account->vaxiaUnderage)) {
    vaxia_core_rule_mail('rules_user_is_under_minimal_age.mail.0', $account->getEmail(), ['[user:name]' => $account->getAccountName(), '[account:name]' => $account->getAccountName()]);
  }
}

/**
 * Implements hook_ENTITY_TYPE_update() for users: cold storage mails (D7
 * vaxia_rpg; D7 sent the deactivation mail twice).
 */
function vaxia_core_user_update(UserInterface $account) {
  $original = $account->original ?? NULL;
  if (!$original || $original->isBlocked() === $account->isBlocked()) {
    return;
  }
  $tokens = ['[account:name]' => $account->getAccountName()];
  if ($account->isBlocked() && $account->hasRole('cold_storage')) {
    vaxia_core_rule_mail('cold_storage_vaxia_email_blocked_players_to_cold_storage.mail.0', $account->getEmail(), $tokens);
  }
  if (!$account->isBlocked() && $account->hasRole('player')) {
    // The mail points to the password reset page for players whose D7
    // password no longer works.
    vaxia_core_rule_mail('players_vaxia_returning_player_request_access.mail.0', $account->getEmail(), $tokens + ['[site:url]' => \Drupal\Core\Url::fromRoute('<front>', [], ['absolute' => TRUE])->toString()]);
    vaxia_core_rule_mail_roles('players_vaxia_returning_player_request_access.mail_to_users_of_role.1', ['lead'], $tokens);
  }
}

/**
 * The player's characters (not NPCs).
 */
function vaxia_core_player_characters(int $uid): array {
  $storage = \Drupal::entityTypeManager()->getStorage('node');
  $ids = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'character_sheet')->condition('uid', $uid)->sort('title')->execute();
  return array_filter($storage->loadMultiple($ids), fn($n) => !$n->get('field_is_npc')->value);
}

/**
 * Mails a player a copy of all their characters.
 */
function vaxia_core_send_characters(UserInterface $account): void {
  $renderer = \Drupal::service('renderer');
  $builder = \Drupal::entityTypeManager()->getViewBuilder('node');
  $parts = ["For your records the following characters are associted with your account. These characters may or may not be currently active, and their inclusion in this email is not an indication that the character will be reactived if it was deactivated for a specific reason. This email is being sent to you because you, or a site admin or lead wanted to make sure you had a copy of all of your characters on the site.\n\n---\n\n"];
  foreach (vaxia_core_player_characters((int) $account->id()) as $character) {
    $build = $builder->view($character, 'full');
    $html = (string) $renderer->renderInIsolation($build);
    // As D7's text mail did: the title upper-cased, then the sheet as text.
    $parts[] = mb_strtoupper($character->label()) . "\n" . trim(preg_replace('/\n\s*\n+/', "\n", html_entity_decode(strip_tags(str_replace(['</div>', '</p>', '<br>', '</tr>'], ["\n", "\n", "\n", "\n"], $html)), ENT_QUOTES)));
    $parts[] = "\n---\n";
  }
  $parts[] = "\n\n~~~~~~\nThis email was automatically generated by [site:name].\nYou are receiving this email as part of your membership at [site:name].\nDo not reply this this email, it is not a monitored account and messages will\nnot be received by a human.\n~~~~~~\n";
  \Drupal::service('plugin.manager.mail')->mail('vaxia_core', 'text', $account->getEmail(), $account->getPreferredLangcode(), [
    'subject' => t('[site:mail-prefix] Player Characters for @name', ['@name' => $account->getAccountName()]),
    'body' => implode("\n", $parts),
  ]);
  \Drupal::messenger()->addStatus(t('Characters have been sent to the email associated with this player account.'));
}

/**
 * Implements hook_ENTITY_TYPE_view() for users: the "mail my characters"
 * form, and only public fields for those who may not see others' accounts.
 */
function vaxia_core_user_view(array &$build, UserInterface $account, $display, $view_mode) {
  if ($view_mode !== 'full') {
    return;
  }
  $current = \Drupal::currentUser();
  $own = (int) $current->id() === (int) $account->id();
  $build['#cache']['contexts'][] = 'user';
  if (($own || $current->hasPermission('email others characters')) && vaxia_core_player_characters((int) $account->id())) {
    $build['vaxia_players_email'] = \Drupal::formBuilder()->getForm('Drupal\vaxia_core\Form\EmailCharactersForm', $account) + ['#weight' => 1000];
  }
  if (!$own && !$current->hasPermission('view others accounts')) {
    $public = array_filter(array_map('trim', preg_split('/\R/', (string) \Drupal::config('vaxia_core.settings')->get('vaxia_public_user_fields'))));
    foreach (array_keys($build) as $key) {
      if (is_string($key) && $key[0] !== '#' && !in_array($key, $public, TRUE) && $key !== 'vaxia_players_email') {
        unset($build[$key]);
      }
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for the account form: owners don't
 * edit the staff-managed fields.
 */
function vaxia_core_form_user_form_alter(array &$form, FormStateInterface $form_state) {
  $account = $form_state->getFormObject()->getEntity();
  // Display preferences (D7 "Personalize blocks"): per-user CSS toggles.
  if (!$account->isNew()) {
    $enabled = DisplayPrefs::enabled($account);
    $form['block'] = [
      '#type' => 'details',
      '#title' => t('Display preferences'),
      '#description' => t('Choose how pages look for you.'),
      '#open' => TRUE,
      '#tree' => TRUE,
      '#weight' => 20,
    ];
    foreach (DisplayPrefs::OPTIONS as $bid => [$label]) {
      $form['block'][$bid] = [
        '#type' => 'checkbox',
        '#title' => t($label),
        '#default_value' => in_array($bid, $enabled, TRUE),
      ];
    }
    $form['actions']['submit']['#submit'][] = 'vaxia_core_display_prefs_submit';
  }
  // Signing up with the terms box ticked accepts the current terms version
  // (accounts staff create still accept on first login).
  if ($account->isNew() && !\Drupal::currentUser()->hasPermission('administer users')) {
    $form['actions']['submit']['#submit'][] = 'vaxia_core_register_terms_submit';
  }
  if ($account->isNew() || (int) $account->id() !== (int) \Drupal::currentUser()->id()) {
    return;
  }
  foreach (preg_split('/\R/', (string) \Drupal::config('vaxia_core.settings')->get('vaxia_disabled_user_fields')) as $field) {
    $field = trim($field);
    if ($field !== '' && isset($form[$field])) {
      $form[$field]['#access'] = FALSE;
    }
  }
}

/**
 * Registration submit: records the current terms version as accepted.
 */
function vaxia_core_register_terms_submit(array &$form, FormStateInterface $form_state) {
  $account = $form_state->getFormObject()->getEntity();
  if ($account->id() && $account->hasField('field_terms_and_conditions') && $account->get('field_terms_and_conditions')->value) {
    \Drupal\vaxia_core\Relaunch::acceptTerms((int) $account->id());
  }
}

/**
 * Saves the account form's display preferences.
 */
function vaxia_core_display_prefs_submit(array &$form, FormStateInterface $form_state) {
  $saved = [];
  foreach (array_keys(DisplayPrefs::OPTIONS) as $bid) {
    $saved[$bid] = (int) (bool) $form_state->getValue(['block', $bid]);
  }
  \Drupal::service('user.data')->set('vaxia_core', $form_state->getFormObject()->getEntity()->id(), 'display_prefs', $saved);
}

/**
 * Implements hook_page_attachments(): the display preferences' CSS.
 */
function vaxia_core_page_attachments(array &$attachments) {
  foreach (DisplayPrefs::enabled(\Drupal::currentUser()) as $bid) {
    $attachments['#attached']['html_head'][] = [[
      '#tag' => 'style',
      '#value' => \Drupal\Core\Render\Markup::create(DisplayPrefs::OPTIONS[$bid][1]),
      '#attributes' => ['class' => ['vaxia-display-pref', 'vaxia-display-pref-' . $bid]],
    ], 'vaxia_display_pref_' . $bid];
  }
  $attachments['#cache']['contexts'][] = 'vaxia_display_prefs';
}

/**
 * Daily: players away six months go to cold storage (D7's rule listened for
 * an event that never delivered a user).
 */
function vaxia_core_retire_cron(): void {
  if (!\Drupal\vaxia_core\Cron::onceADay('retire_cron')) {
    return;
  }
  $cutoff = strtotime('-6 months');
  $storage = \Drupal::entityTypeManager()->getStorage('user');
  $ids = $storage->getQuery()->accessCheck(FALSE)->condition('status', 1)->condition('roles', 'player')->condition('created', $cutoff, '<')->condition('access', $cutoff, '<')->execute();
  foreach ($storage->loadMultiple($ids) as $account) {
    foreach (['voter', 'storyteller_newbie', 'storyteller_assistant', 'storyteller_candidate', 'storyteller_expired', 'storyteller', 'lead', 'administrator'] as $role) {
      $account->removeRole($role);
    }
    // Blocking a player moves them to cold storage (vaxia_conduct).
    $account->block();
    $account->save();
    vaxia_core_rule_mail_roles('rules_retire_inactive_user_account.mail_to_users_of_role.0', ['administrator', 'lead'], ['[user:name]' => $account->getAccountName()]);
    \Drupal::logger('rules_wd')->notice('@name account deactivated for inactivity.', ['@name' => $account->getAccountName()]);
  }
}

/**
 * Implements hook_webform_submission_insert(): proposal receipts.
 */
function vaxia_core_webform_submission_insert($submission) {
  $node = $submission->getSourceEntity();
  $voter = $submission->getOwner();
  if ($node instanceof NodeInterface && $node->bundle() === 'proposal' && $voter && $voter->id() && !$voter->isBlocked()) {
    // The answers as text, "Label: value" (D7's rule printed the submission
    // with PHP code, which the port had kept as text).
    $elements = $submission->getWebform()->getElementsDecodedAndFlattened();
    $answers = [];
    foreach ($submission->getData() as $key => $value) {
      $answers[] = ($elements[$key]['#title'] ?? $key) . ': ' . implode(', ', array_map('strval', (array) $value));
    }
    vaxia_core_rule_mail('rules_proposal_reciept.mail.0', $voter->getEmail(), [
      '[node:title]' => $node->label(), '[node:url]' => $node->toUrl('canonical', ['absolute' => TRUE])->toString(),
      '[user:name]' => $voter->getAccountName(), '[data:sid]' => $submission->id(),
      '[data:submission]' => implode("\n", $answers),
    ]);
  }
}

/**
 * Notes and strikes are mailed out (D7 vaxia_rpg).
 */
function vaxia_core_conduct_mails(NodeInterface $node): void {
  $url = $node->toUrl('canonical', ['absolute' => TRUE])->toString();
  if ($node->bundle() === 'notation') {
    $players = $node->get('field_players')->referencedEntities();
    $labels = fn($field) => implode(', ', array_map(fn($i) => (string) ($i['value'] ?? ''), $node->hasField($field) ? $node->get($field)->getValue() : []));
    vaxia_core_rule_mail_roles('rules_notation_email.mail_to_users_of_role.0', ['lead', 'mediator'], [
      '[node:field-players:name]' => $players ? $players[0]->getAccountName() : '',
      '[node:field-players]' => implode(', ', array_map(fn($p) => $p->getAccountName(), $players)),
      '[node:title]' => $node->label(), '[node:url]' => $url, '[node:author]' => $node->getOwner()->getDisplayName(),
      '[node:body]' => (string) $node->get('body')->value, '[node:field-category]' => $labels('field_category'),
      '[node:field-results]' => $labels('field_results'),
      '[node:field-social-members]' => implode(', ', array_map(fn($p) => $p->getAccountName(), $node->get('field_social_members')->referencedEntities())),
    ]);
  }
  if ($node->bundle() === 'strike') {
    $player = $node->get('field_struck_player')->entity;
    if (!$player) {
      return;
    }
    $player = User::load($player->id());
    $tokens = [
      '[node:field-struck-player:name]' => $player->getAccountName(),
      '[node:field-struck-player:field-banned-until]' => (string) $player->get('field_banned_until')->value,
      '[node:title]' => $node->label(), '[node:url]' => $url, '[node:author]' => $node->getOwner()->getDisplayName(),
      '[node:body]' => (string) $node->get('field_details')->value,
    ];
    if ($node->get('field_severity')->value === 'Red') {
      vaxia_core_rule_mail_roles('rules_red_strike.mail_to_users_of_role.0', ['storyteller', 'lead', 'mediator'], $tokens);
      vaxia_core_rule_mail('rules_red_strike.mail.1', $player->getEmail(), $tokens);
      vaxia_core_send_characters($player);
    }
    else {
      vaxia_core_rule_mail_roles('rules_yellow_strike.mail_to_users_of_role.0', ['lead', 'mediator'], $tokens);
      vaxia_core_rule_mail('rules_yellow_strike.mail.1', $player->getEmail(), $tokens);
    }
  }
}

/**
 * Implements hook_user_login(): a login from a banned player's IP alerts
 * leads and admins (D7 looked up the literal text "<?php echo ip_address();
 * ?>", so it never matched).
 */
function vaxia_core_user_login(UserInterface $account) {
  if ($account->hasRole('banned_user')) {
    return;
  }
  $ip = \Drupal::request()->getClientIp();
  $ids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('roles', 'banned_user')->condition('field_banned_ip', $ip)->execute();
  if (!$ids) {
    return;
  }
  $list = '';
  foreach (User::loadMultiple($ids) as $banned) {
    $list .= "\n" . $banned->getAccountName() . ' ( ' . $banned->getEmail() . ' )';
  }
  vaxia_core_rule_mail_roles('rules_alert_user_for_banned_ip.mail_to_users_of_role.0', ['administrator', 'lead'], [
    '[matching-user-list:value]' => $list, '[site:current-user:name]' => $account->getAccountName(), '[user:name]' => $account->getAccountName(),
  ]);
}
