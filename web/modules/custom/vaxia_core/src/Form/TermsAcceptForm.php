<?php

namespace Drupal\vaxia_core\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\vaxia_core\Relaunch;

/**
 * Accepting the current Terms and Conditions. Logged-in users who haven't
 * are sent here (EventSubscriber\TermsGate) until they do.
 */
class TermsAcceptForm extends FormBase {

  public function getFormId() {
    return 'vaxia_core_terms_accept';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $path = Relaunch::setting('terms_path');
    $link = $path !== '' ? Url::fromUserInput('/' . ltrim($path, '/'))->toString() : '';
    if (Relaunch::hasAcceptedTerms($this->currentUser())) {
      $form['done'] = ['#markup' => '<p>You have accepted the current Terms and Conditions.</p>'];
      return $form;
    }
    $form['intro'] = [
      '#markup' => '<p>Welcome! Before you continue, please read our current '
      . ($link !== '' ? '<a href="' . $link . '">Terms and Conditions</a>' : 'Terms and Conditions')
      . ' and accept them. You can still log out without accepting.</p>',
    ];
    $form['agree'] = [
      '#type' => 'checkbox',
      '#title' => 'I agree to the Terms and Conditions.',
      '#required' => TRUE,
    ];
    $form['actions'] = ['#type' => 'actions'];
    $form['actions']['submit'] = ['#type' => 'submit', '#value' => 'Accept'];
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    Relaunch::acceptTerms((int) $this->currentUser()->id());
    $this->messenger()->addStatus('Thank you for accepting the Terms and Conditions.');
    $form_state->setRedirect('<front>');
  }

}
