<?php

namespace Drupal\vaxia_chat\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\vaxia_chat\Archives;

/**
 * Archive settings and "Archive All Posts" (D7 archives_admin_form()).
 */
class ArchivesSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return 'archives_admin_form';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_chat.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_chat.settings');
    $form['archives_flat_file_path'] = [
      '#title' => $this->t('Location'),
      '#description' => $this->t('Flat file archive location, the directory the archives are stored in. Be sure to end the path with a /.'),
      '#type' => 'textfield',
      '#default_value' => $config->get('archives_flat_file_path'),
    ];
    $form['archives_date'] = [
      '#title' => $this->t('Timeframe'),
      '#description' => $this->t('Timeframe to keep in the database before moving to archives. Use the same strings as date().'),
      '#type' => 'textfield',
      '#default_value' => $config->get('archives_date'),
    ];
    $form['archives_cron'] = [
      '#title' => $this->t('Enable archive cron'),
      '#type' => 'checkbox',
      '#default_value' => $config->get('archives_cron'),
    ];
    $form['archive_date'] = ['#markup' => '<div>' . $this->t('Archives last run') . ': ' . \Drupal::state()->get('vaxia_chat.archives_last_run', 'never') . '</div>'];
    $form['archive'] = [
      '#type' => 'submit',
      '#value' => $this->t('Archive All Posts'),
      '#submit' => ['::archive'],
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('vaxia_chat.settings')
      ->set('archives_flat_file_path', (string) $form_state->getValue('archives_flat_file_path'))
      ->set('archives_date', (string) $form_state->getValue('archives_date'))
      ->set('archives_cron', (bool) $form_state->getValue('archives_cron'))
      ->save();
    parent::submitForm($form, $form_state);
  }

  public function archive(array &$form, FormStateInterface $form_state) {
    Archives::archiveAll();
  }

}
