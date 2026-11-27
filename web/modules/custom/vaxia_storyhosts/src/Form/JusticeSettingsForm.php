<?php

namespace Drupal\vaxia_storyhosts\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Justice Form Settings (D7 admin/vaxia/justice_form).
 */
class JusticeSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_justice_form_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_storyhosts.settings'];
  }

  private function keys(): array {
    $keys = ['justice_form_base_pp', 'justice_form_punishments', 'justice_form_reputations'];
    foreach (range(1, 5) as $tier) {
      array_push($keys, 'justice_form_silver_' . $tier, 'justice_form_time_' . $tier, 'justice_form_timeunit_' . $tier);
    }
    return $keys;
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_storyhosts.settings');
    $form['tiers'] = ['#type' => 'details', '#title' => $this->t('Tier Settings'), '#open' => FALSE];
    $form['justice_form_base_pp'] = ['#type' => 'textfield', '#title' => $this->t('Base PP'), '#required' => TRUE, '#default_value' => $config->get('justice_form_base_pp')];
    foreach (range(1, 5) as $tier) {
      $form['tiers']['justice_form_silver_' . $tier] = ['#type' => 'textfield', '#title' => $this->t('Silver for Tier @tier', ['@tier' => $tier]), '#default_value' => $config->get('justice_form_silver_' . $tier)];
      $form['tiers']['justice_form_time_' . $tier] = ['#type' => 'textfield', '#title' => $this->t('Time amount for Tier @tier', ['@tier' => $tier]), '#default_value' => $config->get('justice_form_time_' . $tier)];
      $form['tiers']['justice_form_timeunit_' . $tier] = ['#type' => 'select', '#title' => $this->t('Time unit for Tier @tier', ['@tier' => $tier]), '#options' => ['days' => $this->t('Days'), 'weeks' => $this->t('Weeks'), 'months' => $this->t('Months')], '#default_value' => $config->get('justice_form_timeunit_' . $tier)];
    }
    $form['justice_form_punishments'] = ['#type' => 'textarea', '#title' => $this->t('Punishment table'), '#required' => TRUE, '#default_value' => $config->get('justice_form_punishments')];
    $form['justice_form_reputations'] = ['#type' => 'textarea', '#title' => $this->t('Reputation table'), '#required' => TRUE, '#default_value' => $config->get('justice_form_reputations')];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_storyhosts.settings');
    foreach ($this->keys() as $key) {
      $config->set($key, $form_state->getValue($key));
    }
    $config->save();
    parent::submitForm($form, $form_state);
  }

}
