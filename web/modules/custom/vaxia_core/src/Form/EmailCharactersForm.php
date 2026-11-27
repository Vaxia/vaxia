<?php

namespace Drupal\vaxia_core\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\user\Entity\User;
use Drupal\user\UserInterface;

/**
 * Mails a player a summary of their characters (D7 vaxia_players).
 */
class EmailCharactersForm extends FormBase {

  public function getFormId() {
    return '_vaxia_players_email_characters';
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?UserInterface $account = NULL) {
    $names = array_map(fn($n) => $n->label(), vaxia_core_player_characters((int) $account->id()));
    $form_state->set('uid', $account->id());
    $form['vaxia_players_email'] = ['#type' => 'details', '#title' => $this->t('Email Characters')];
    $form['vaxia_players_email']['help'] = ['#markup' => '<div class="vaxia_players_email">' . $this->t('Clicking this button will send an email summary of the following characters to the email associated with this account: %pcs', ['%pcs' => implode(', ', $names)]) . '</div>'];
    $own = (int) $this->currentUser()->id() === (int) $account->id();
    $form['vaxia_players_email']['send'] = ['#type' => 'submit', '#value' => $own ? $this->t('Send Me My Characters') : $this->t('Send @name Their Characters', ['@name' => $account->getAccountName()])];
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $account = User::load($form_state->get('uid'));
    $own = (int) $this->currentUser()->id() === (int) $account->id();
    if (!$own && !$this->currentUser()->hasPermission('email others characters')) {
      return;
    }
    if (!$account->getEmail() || !\Drupal::service('email.validator')->isValid($account->getEmail())) {
      $this->messenger()->addStatus($this->t('Unable to send email - invalid email found for this account.'));
      return;
    }
    vaxia_core_send_characters($account);
  }

}
