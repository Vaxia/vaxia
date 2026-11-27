<?php

namespace Drupal\vaxia_chat\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Which room fields a personal room's player may edit or see (D7
 * _personal_rooms_settings()).
 */
class PersonalRoomsSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_personal_rooms_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_chat.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_chat.settings');
    $form['personal_rooms_fields_edit'] = [
      '#title' => 'Edit room fields',
      '#description' => $this->t('Room fields that can be edited by the player of the owning character.'),
      '#type' => 'textarea',
      '#default_value' => $config->get('personal_rooms_fields_edit'),
    ];
    $form['personal_rooms_fields_view'] = [
      '#title' => 'View room fields',
      '#description' => $this->t('Room fields that can be viewed in edit by the player of the owning character.'),
      '#type' => 'textarea',
      '#default_value' => $config->get('personal_rooms_fields_view'),
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('vaxia_chat.settings')
      ->set('personal_rooms_fields_edit', str_replace("\r", '', (string) $form_state->getValue('personal_rooms_fields_edit')))
      ->set('personal_rooms_fields_view', str_replace("\r", '', (string) $form_state->getValue('personal_rooms_fields_view')))
      ->save();
    parent::submitForm($form, $form_state);
  }

}
