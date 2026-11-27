<?php

/**
 * @file
 * Vacations (D7 vacation): staff park their roles (keeping player + vacation
 * mode) until they come back or the vacation ends.
 */

use Drupal\Core\Form\FormStateInterface;
use Drupal\user\Entity\User;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\Cron;

/**
 * The user's current vacation, if any.
 */
function vaxia_storyhosts_vacation(int $uid): ?object {
  $vacation = \Drupal::database()->query('SELECT * FROM {vacation} WHERE uid = :uid ORDER BY vid DESC', [':uid' => $uid])->fetchObject();
  if (!$vacation) {
    return NULL;
  }
  $vacation->positions = json_decode((string) $vacation->positions, TRUE) ?: [];
  return $vacation;
}

/**
 * Whether the account holds a role that may go on vacation.
 */
function vaxia_storyhosts_may_vacation(UserInterface $account): bool {
  foreach (\Drupal::entityTypeManager()->getStorage('user_role')->loadMultiple($account->getRoles(TRUE)) as $role) {
    if ($role->hasPermission('can go on vacation') || $role->isAdmin()) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Starts a vacation: roles parked, player + vacation mode kept. An open-ended
 * one ("longer (resign)") gives the roles up for good.
 */
function vaxia_storyhosts_vacation_start(UserInterface $account, ?int $end): void {
  if ($end) {
    \Drupal::database()->insert('vacation')->fields([
      'uid' => $account->id(),
      'start' => \Drupal::time()->getRequestTime(),
      'end' => $end,
      'positions' => json_encode(array_values($account->getRoles(TRUE))),
    ])->execute();
  }
  $account->set('roles', $end ? ['player', 'vacation_mode'] : ['player']);
  $account->save();
  \Drupal::messenger()->addStatus(t("Your vacation notice has gone out, roles have been removed and you are ready to hit the beach and/or deal with what you need to. Good luck, and we'll see you again when you're ready to return."));
  $name = $account->getDisplayName();
  vaxia_storyhosts_send_to_roles(['storyteller'], '[site:mail-prefix] Storyhost or ASH has gone on vacation',
    $name . " has gone on a long-term vacation using the vacation mode system. This email is to notify you to expect a long term vacation or absence on behalf of this player.\n\nWhile on vacation, " . $name . " will only have permissions and access as per a player. They will not have any SH or ASH powers and will not be receiving any notices relevant to those roles.\n\nOnce " . $name . " declares themselves off of vacation, the permissions will be re-enabled.\n\nIf you have any questions about " . $name . "'s absence or the policies on inactive SHs, use the site's Contact form or post to the Policies forum to get clarification.");
}

/**
 * Ends a vacation: the parked roles come back.
 */
function vaxia_storyhosts_vacation_end(object $vacation): void {
  \Drupal::database()->delete('vacation')->condition('vid', $vacation->vid)->execute();
  $account = User::load($vacation->uid);
  if (!$account) {
    return;
  }
  $account->set('roles', $vacation->positions);
  $account->save();
  \Drupal::moduleHandler()->invokeAll('vaxia_vacation_end', [$vacation]);
  \Drupal::messenger()->addStatus(t("Welcome back! Take a moment to check in with the rest of the Storyhosts so you know what you missed. If you've been gone for a long time - expect to have to do some catching up."));
  $name = $account->getDisplayName();
  vaxia_storyhosts_send_to_roles(['storyteller', 'lead', 'administrator'], '[site:mail-prefix] Storyhost or ASH has returned from vacation',
    $name . " has returned from a long-term vacation using the vacation system. This email is to notify you to welcome this player back.\n\n" . $name . "'s permissions have been re-enabled as they were before they left, and by marking the vacation button they're letting us know they are ready to fulfill those roles.\n\nIf you have any questions about " . $name . " 's absence or the policies on inactive SHs, use the site's Contact form or post to the Policies forum to get clarification.");
}

/**
 * Implements hook_form_FORM_ID_alter() for the account form: the vacation
 * fieldset. D7 ended any current vacation while building the form, so it
 * never offered "Return from vacation".
 */
function vaxia_storyhosts_form_user_form_alter(array &$form, FormStateInterface $form_state) {
  $account = $form_state->getFormObject()->getEntity();
  if ($account->isNew()) {
    return;
  }
  $vacation = vaxia_storyhosts_vacation((int) $account->id());
  if (!$vacation && !vaxia_storyhosts_may_vacation($account)) {
    return;
  }
  $form['vacation'] = [
    '#type' => 'fieldset',
    '#title' => t('Vacation mode'),
    '#description' => ['#markup' => \Drupal::config('vaxia_storyhosts.settings')->get('vacation_description')],
    '#weight' => 1000,
  ];
  if (!$vacation) {
    $form['vacation']['vacation_end'] = [
      '#type' => 'select',
      '#options' => [
        '3 days' => '3 days', '1 week' => '1 week', '2 weeks' => '2 weeks', '1 month' => '1 month',
        '2 months' => '2 months', '3 months' => '3 months', 'indefinitely' => 'longer (resign)',
      ],
      '#default_value' => '1 week',
    ];
    $form['vacation']['vacation_submit'] = [
      '#type' => 'submit',
      '#value' => t('Go on vacation'),
      '#submit' => ['vaxia_storyhosts_go_on_vacation'],
      '#limit_validation_errors' => [['vacation_end'], ['temp_replacement']],
    ];
  }
  else {
    $form['vacation']['#description'] = t('You are currently listed as on vacation. Based on the options selected when you went on vacation, you will be on vacation until: @end', ['@end' => date('m/d/Y', $vacation->end)]);
    $form['vacation']['vacation_submit'] = [
      '#type' => 'submit',
      '#value' => t('Return from vacation'),
      '#submit' => ['vaxia_storyhosts_return_from_vacation'],
      '#limit_validation_errors' => [],
    ];
  }
  // Other modules add to the fieldset (D7 hook_vacation_form).
  $context = ['account' => $account, 'vacation' => $vacation];
  \Drupal::moduleHandler()->alter('vaxia_vacation_form', $form, $form_state, $context);
}

function vaxia_storyhosts_go_on_vacation(array &$form, FormStateInterface $form_state) {
  $account = User::load($form_state->getFormObject()->getEntity()->id());
  $choice = (string) $form_state->getValue('vacation_end');
  vaxia_storyhosts_vacation_start($account, $choice === 'indefinitely' ? NULL : strtotime($choice));
  $form_state->setRedirect('<front>');
}

function vaxia_storyhosts_return_from_vacation(array &$form, FormStateInterface $form_state) {
  $vacation = vaxia_storyhosts_vacation((int) $form_state->getFormObject()->getEntity()->id());
  if ($vacation) {
    vaxia_storyhosts_vacation_end($vacation);
  }
  $form_state->setRedirect('<front>');
}

/**
 * Daily: ends vacations due by the end of today, overdue ones included (D7
 * only looked at today, so a missed cron day stranded them).
 */
function vaxia_storyhosts_vacation_cron(): void {
  if (!Cron::onceADay('vacation_cron', '00:00:01')) {
    return;
  }
  $due = \Drupal::database()->query('SELECT vid, uid FROM {vacation} WHERE "end" < :tomorrow', [':tomorrow' => strtotime('tomorrow')])->fetchAll();
  foreach ($due as $row) {
    $vacation = vaxia_storyhosts_vacation((int) $row->uid);
    if ($vacation) {
      vaxia_storyhosts_vacation_end($vacation);
    }
  }
}

/**
 * Implements hook_ENTITY_TYPE_delete() for users.
 */
function vaxia_storyhosts_user_delete(UserInterface $account) {
  \Drupal::database()->delete('vacation')->condition('uid', $account->id())->execute();
}
