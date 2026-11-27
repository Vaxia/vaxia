<?php

namespace Drupal\vaxia_core\Form;

use Drupal\Component\Utility\Html;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\Relaunch;

/**
 * Staff account recovery (user/{user}/recover): after confirming who a player
 * is, staff move the account to a new email address and can send a one-time
 * login link there. Every change is logged with who, when and how identity
 * was confirmed, and listed on this tab.
 */
class RecoverAccountForm extends FormBase {

  public function getFormId() {
    return 'vaxia_core_recover_account';
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?UserInterface $user = NULL) {
    $form_state->set('uid', (int) $user->id());
    $form['current'] = [
      '#type' => 'item',
      '#title' => 'Current email address',
      '#markup' => Html::escape((string) $user->getEmail()) . ($user->isBlocked() ? ' (account blocked)' : ''),
    ];
    $form['mail'] = [
      '#type' => 'email',
      '#title' => 'New email address',
      '#required' => TRUE,
    ];
    $form['note'] = [
      '#type' => 'textarea',
      '#title' => 'How was their identity confirmed?',
      '#description' => 'Required. Kept with the change for staff to read later.',
      '#required' => TRUE,
      '#rows' => 3,
    ];
    $form['send_link'] = [
      '#type' => 'checkbox',
      '#title' => 'Send a one-time login link to the new address',
      '#default_value' => TRUE,
    ];
    $form['actions'] = ['#type' => 'actions'];
    $form['actions']['submit'] = ['#type' => 'submit', '#value' => 'Change email address'];

    $rows = [];
    $storage = \Drupal::entityTypeManager()->getStorage('user');
    foreach (array_reverse(Relaunch::recoveryLog((int) $user->id())) as $entry) {
      $staff = $storage->load($entry['staff']);
      $rows[] = [
        \Drupal::service('date.formatter')->format($entry['time'], 'short'),
        $staff ? $staff->getDisplayName() : 'uid ' . $entry['staff'],
        $entry['old'],
        $entry['new'],
        ['data' => ['#markup' => nl2br(Html::escape($entry['note']))]],
        $entry['link'] ? 'Yes' : 'No',
      ];
    }
    $form['history'] = [
      '#type' => 'details',
      '#title' => 'Email changes by staff',
      '#open' => TRUE,
      '#weight' => 100,
      'table' => [
        '#type' => 'table',
        '#header' => ['When', 'By', 'Old address', 'New address', 'Identity confirmed', 'Link sent'],
        '#rows' => $rows,
        '#empty' => 'No changes yet.',
        '#attributes' => ['class' => ['vaxia-recovery-log']],
      ],
    ];
    return $form;
  }

  public function validateForm(array &$form, FormStateInterface $form_state) {
    $mail = trim((string) $form_state->getValue('mail'));
    $uid = $form_state->get('uid');
    $account = \Drupal::entityTypeManager()->getStorage('user')->load($uid);
    if ($mail !== '' && mb_strtolower($mail) === mb_strtolower((string) $account->getEmail())) {
      $form_state->setErrorByName('mail', 'That is already the account\'s email address.');
      return;
    }
    $taken = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('mail', $mail)->condition('uid', $uid, '<>')->range(0, 1)->execute();
    if ($mail !== '' && $taken) {
      $form_state->setErrorByName('mail', 'Another account already uses that email address.');
    }
    if (trim((string) $form_state->getValue('note')) === '') {
      $form_state->setErrorByName('note', 'Say how their identity was confirmed.');
    }
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $uid = $form_state->get('uid');
    $account = \Drupal::entityTypeManager()->getStorage('user')->load($uid);
    $old = (string) $account->getEmail();
    $new = trim((string) $form_state->getValue('mail'));
    $note = trim((string) $form_state->getValue('note'));
    $account->setEmail($new);
    $account->save();

    $link = FALSE;
    if ($form_state->getValue('send_link')) {
      if ($account->isBlocked()) {
        $this->messenger()->addWarning('The account is blocked, so no login link was sent: a one-time link does not work for a blocked account. Unblock it first, then send a link from here or have the player use "Reset your password".');
      }
      else {
        $link = (bool) _user_mail_notify('password_reset', $account);
        if (!$link) {
          $this->messenger()->addError('The login link could not be sent.');
        }
      }
    }
    Relaunch::addRecoveryLog($uid, [
      'time' => \Drupal::time()->getCurrentTime(),
      'staff' => (int) $this->currentUser()->id(),
      'old' => $old,
      'new' => $new,
      'note' => $note,
      'link' => $link,
    ]);
    $this->logger('vaxia_core')->notice('Account recovery: @staff changed the email of @name (uid @uid) from @old to @new; login link @link. Identity confirmed: @note', [
      '@staff' => $this->currentUser()->getAccountName(),
      '@name' => $account->getAccountName(),
      '@uid' => $uid,
      '@old' => $old,
      '@new' => $new,
      '@link' => $link ? 'sent' : 'not sent',
      '@note' => $note,
    ]);
    $this->messenger()->addStatus('The email address was changed' . ($link ? ' and a one-time login link was sent to the new address.' : '.'));
  }

}
