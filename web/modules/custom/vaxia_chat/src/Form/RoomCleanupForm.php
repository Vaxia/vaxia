<?php

namespace Drupal\vaxia_chat\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\vaxia_chat\RoomCleanup;

/**
 * Room cleanup settings and "Cleanup Rooms" (D7 room_cleanup_admin_form()).
 */
class RoomCleanupForm extends ConfigFormBase {

  public function getFormId() {
    return 'room_cleanup_admin_form';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_chat.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_chat.settings');
    $form['room_cleanup_date'] = [
      '#title' => $this->t('Timeframe'),
      '#description' => $this->t('Timeframe to keep in the database before cleaning up rooms. Use the same strings as date().'),
      '#type' => 'textfield',
      '#default_value' => $config->get('room_cleanup_date'),
    ];
    $form['room_cleanup_cron'] = [
      '#title' => $this->t('Enable room cleanup cron'),
      '#type' => 'checkbox',
      '#default_value' => $config->get('room_cleanup_cron'),
    ];
    // D7 printed the on/off setting as a date (known bug: 01/01/1970).
    $form['room_cleanup_last_run'] = ['#markup' => '<div>' . $this->t('Room cleanup last run') . ': ' . \Drupal::state()->get('vaxia_chat.room_cleanup_last_run', 'never') . '</div>'];
    $form['cleanup'] = [
      '#type' => 'submit',
      '#value' => $this->t('Cleanup Rooms'),
      '#submit' => ['::cleanup'],
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('vaxia_chat.settings')
      ->set('room_cleanup_date', (string) $form_state->getValue('room_cleanup_date'))
      ->set('room_cleanup_cron', (bool) $form_state->getValue('room_cleanup_cron'))
      ->save();
    parent::submitForm($form, $form_state);
  }

  public function cleanup(array &$form, FormStateInterface $form_state) {
    RoomCleanup::run();
  }

}
