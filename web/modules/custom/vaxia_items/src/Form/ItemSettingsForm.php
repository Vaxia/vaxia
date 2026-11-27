<?php

namespace Drupal\vaxia_items\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\workflow\Entity\WorkflowState;

/**
 * Item settings (D7 _items_admin_settings(), which crashed: known bug).
 */
class ItemSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_items_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_items.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_items.settings');
    $states = [];
    foreach (WorkflowState::loadMultiple() as $sid => $state) {
      $states[$sid] = $state->getWorkflow()->label() . ':' . $state->label();
    }
    $form['items_approved_workflow'] = [
      '#title' => 'Item approved',
      '#description' => 'Workflow state for approved items.',
      '#type' => 'select',
      '#options' => $states,
      '#default_value' => $config->get('items_approved_workflow'),
    ];
    $form['silver'] = ['#title' => $this->t('Item values'), '#type' => 'details'];
    foreach ([
      'items_primary_value' => 'Value modifier for primary stats',
      'items_skills_value' => 'Value modifier for skills',
      'items_bonus_value' => 'Silver value per bonus',
      'items_overlap_value' => 'Silver value per overlap',
    ] as $key => $title) {
      $form['silver'][$key] = ['#title' => $this->t($title), '#type' => 'textfield', '#default_value' => $config->get($key)];
    }
    $form['silver']['items_recalculate_values'] = [
      '#title' => $this->t('Recalculate value on save'),
      '#type' => 'checkbox',
      '#default_value' => $config->get('items_recalculate_values'),
    ];
    $form['oversight'] = ['#title' => $this->t('Item oversight'), '#type' => 'details'];
    $form['oversight']['oversight_limit'] = [
      '#title' => $this->t('Stat value requiring oversight'),
      '#type' => 'textfield',
      '#default_value' => $config->get('oversight_limit'),
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_items.settings');
    $config->set('items_approved_workflow', (string) $form_state->getValue('items_approved_workflow'));
    foreach (['items_primary_value', 'items_skills_value', 'items_bonus_value', 'items_overlap_value'] as $key) {
      $config->set($key, (float) $form_state->getValue($key));
    }
    $config->set('items_recalculate_values', (bool) $form_state->getValue('items_recalculate_values'));
    $config->set('oversight_limit', (int) $form_state->getValue('oversight_limit'));
    $config->save();
    parent::submitForm($form, $form_state);
  }

}
