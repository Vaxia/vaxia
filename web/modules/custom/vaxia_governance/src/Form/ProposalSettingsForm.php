<?php

namespace Drupal\vaxia_governance\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Proposal system settings (D7 admin/rpg/proposal).
 */
class ProposalSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return 'proposal_admin_form';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_governance.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    $form['proposal_node_help'] = ['#type' => 'textarea', '#title' => $this->t('Node help'), '#description' => $this->t('Help information to display on the proposal node.'), '#default_value' => $c->get('proposal_node_help')];
    $form['proposal_open_timeframe'] = ['#type' => 'textfield', '#title' => $this->t('Open timeframe'), '#description' => $this->t('Timeframe to keep proposals open for. Use the same strings as date().'), '#default_value' => $c->get('proposal_open_timeframe')];
    $form['proposal_active_timeframe'] = ['#type' => 'textfield', '#title' => $this->t('Active timeframe'), '#description' => $this->t('Timeframe to watch for active voters in. Use the same strings as date().'), '#default_value' => $c->get('proposal_active_timeframe')];
    $form['proposal_voter_timeframe'] = ['#type' => 'textfield', '#title' => $this->t('Voter timeframe'), '#description' => $this->t('Timeframe for determining user activity as a voter. Use the same strings as date().'), '#default_value' => $c->get('proposal_voter_timeframe')];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    foreach (['proposal_node_help', 'proposal_open_timeframe', 'proposal_active_timeframe', 'proposal_voter_timeframe'] as $key) {
      $c->set($key, (string) $form_state->getValue($key));
    }
    $c->save();
    parent::submitForm($form, $form_state);
  }

}
