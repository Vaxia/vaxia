<?php

namespace Drupal\vaxia_dice\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Dice ruler settings (D7 _dice_ruler_admin_settings()); keys are the D7
 * variable names.
 */
class DiceRulerSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_dice_ruler_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_dice.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_dice.settings');
    $form['dice_ruler_default_diff'] = [
      '#title' => $this->t('Default difficulty'),
      '#description' => $this->t('Use diffulty on most rolls.'),
      '#type' => 'textfield',
      '#default_value' => $config->get('dice_ruler_default_diff'),
    ];
    $form['help_texts'] = ['#type' => 'details', '#title' => $this->t('Help texts for displays')];
    $form['help_texts']['dice_ruler_help_general'] = [
      '#title' => $this->t('General Help'),
      '#description' => $this->t('Help information to show for all selections.'),
      '#type' => 'textarea',
      '#default_value' => $config->get('dice_ruler_help_general'),
    ];
    foreach (['two_trait', 'two_trait_vs', 'combat', 'magic'] as $type) {
      $form['help_texts']['dice_ruler_help_' . $type] = [
        '#title' => ucwords(str_replace('_', ' ', $type)) . ' ' . $this->t('Help'),
        '#description' => $this->t('Help information to show when action is selected.'),
        '#type' => 'textarea',
        '#default_value' => $config->get('dice_ruler_help_' . $type),
      ];
    }
    $form['extra_actions'] = ['#type' => 'details', '#title' => $this->t('Extra actions')];
    $form['extra_actions']['dice_ruler_extra_action_penalty'] = [
      '#title' => $this->t('Extra action penalty'),
      '#description' => $this->t('The default modifier per extra action taken applied to all actions.'),
      '#type' => 'textfield',
      '#default_value' => $config->get('dice_ruler_extra_action_penalty'),
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_dice.settings');
    foreach (['dice_ruler_default_diff', 'dice_ruler_extra_action_penalty'] as $key) {
      $config->set($key, (int) $form_state->getValue($key));
    }
    foreach (['general', 'two_trait', 'two_trait_vs', 'combat', 'magic'] as $type) {
      $config->set('dice_ruler_help_' . $type, (string) $form_state->getValue('dice_ruler_help_' . $type));
    }
    $config->save();
    parent::submitForm($form, $form_state);
  }

}
