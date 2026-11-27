<?php

namespace Drupal\vaxia_items\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\vaxia_core\Cron;

/**
 * Item uses settings (D7 _item_uses_admin_settings()).
 */
class ItemUsesForm extends ConfigFormBase {

  public function getFormId() {
    return '_item_uses_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_items.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_items.settings');
    $form['item_uses_active'] = ['#title' => $this->t('Track item uses'), '#type' => 'checkbox', '#default_value' => $config->get('item_uses_active')];
    $form['item_uses_once_workflow'] = [
      '#title' => 'Item used up',
      '#description' => 'Workflow state for items that need otbe recharged before they can be used again.',
      '#type' => 'select',
      '#options' => Cron::stateOptions(),
      '#default_value' => $config->get('item_uses_once_workflow'),
    ];
    $form['item_uses_daily_workflow'] = [
      '#title' => 'Item used for day',
      '#description' => 'Workflow state for items that need a reset day before they can be used again.',
      '#type' => 'select',
      '#options' => Cron::stateOptions(),
      '#default_value' => $config->get('item_uses_daily_workflow'),
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('vaxia_items.settings')
      ->set('item_uses_active', (bool) $form_state->getValue('item_uses_active'))
      ->set('item_uses_once_workflow', (string) $form_state->getValue('item_uses_once_workflow'))
      ->set('item_uses_daily_workflow', (string) $form_state->getValue('item_uses_daily_workflow'))
      ->save();
    parent::submitForm($form, $form_state);
  }

}
