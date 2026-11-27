<?php

namespace Drupal\vaxia_core\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\vaxia_core\Relaunch;

/**
 * Relaunch settings: the current terms version and the terms page.
 */
class RelaunchSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return 'vaxia_core_relaunch_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_core.relaunch'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $form['terms'] = ['#type' => 'details', '#title' => 'Terms and Conditions', '#open' => TRUE];
    $form['terms']['terms_version'] = [
      '#type' => 'textfield',
      '#title' => 'Current terms version',
      '#description' => 'A date or number. Change it whenever the terms change: every logged-in user who has not accepted this version is sent to the acceptance page until they accept. Leave empty to ask nobody.',
      '#default_value' => Relaunch::termsVersion(),
      '#maxlength' => 64,
    ];
    $form['terms']['terms_path'] = [
      '#type' => 'textfield',
      '#title' => 'Terms page',
      '#description' => 'Path of the page holding the terms, linked from the acceptance page and open to users who have not accepted yet.',
      '#default_value' => Relaunch::setting('terms_path'),
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_core.relaunch');
    foreach (['terms_path'] as $key) {
      $config->set($key, (string) $form_state->getValue($key));
    }
    $config->save();
    $version = trim((string) $form_state->getValue('terms_version'));
    if ($version !== Relaunch::termsVersion()) {
      \Drupal::state()->set('vaxia_core.terms_version', $version);
      $this->logger('vaxia_core')->notice('Terms version set to "@version".', ['@version' => $version]);
    }
    parent::submitForm($form, $form_state);
  }

}
