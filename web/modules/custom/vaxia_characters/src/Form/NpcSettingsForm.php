<?php

namespace Drupal\vaxia_characters\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * NPC handling (D7 _npcs_admin_settings()).
 */
class NpcSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_npcs_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_characters.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $form['npcs_stat_cap'] = [
      '#title' => 'NPC stat cap',
      '#description' => 'The max value an NPC stat can be: Default 70.',
      '#type' => 'textfield',
      '#default_value' => $this->config('vaxia_characters.settings')->get('npcs_stat_cap'),
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('vaxia_characters.settings')->set('npcs_stat_cap', (int) $form_state->getValue('npcs_stat_cap'))->save();
    parent::submitForm($form, $form_state);
  }

}
