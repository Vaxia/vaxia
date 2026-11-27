<?php

namespace Drupal\vaxia_storyhosts\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Vacation configurations (D7 admin/rpg/vacation).
 */
class VacationSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return 'vacation_admin_form';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_storyhosts.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $form['vacation_description'] = [
      '#title' => $this->t('Vacation descriptions'),
      '#type' => 'textarea',
      '#default_value' => $this->config('vaxia_storyhosts.settings')->get('vacation_description'),
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('vaxia_storyhosts.settings')->set('vacation_description', $form_state->getValue('vacation_description'))->save();
    parent::submitForm($form, $form_state);
  }

}
