<?php

namespace Drupal\vaxia_storyhosts\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Storyhost course settings (D7 admin/vaxia/sh_test).
 */
class ShTestSettingsForm extends ConfigFormBase {

  const KEYS = [
    'sh_test_starvation_mode', 'sh_test_approval_min', 'sh_test_approval_min_starvation',
    'sh_test_session_party_size', 'sh_test_session_party_size_min', 'sh_test_session_party_size_npcs',
    'sh_test_session_party_size_ashs', 'sh_test_session_party_size_own_pcs', 'sh_test_session_expires',
    'sh_test_session_expires_timeframe',
  ];

  public function getFormId() {
    return '_sh_test_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_storyhosts.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_storyhosts.settings');
    $form['sh_test_starvation_mode'] = ['#type' => 'textfield', '#title' => $this->t('Minimum Storyhosts'), '#description' => $this->t('Below this number of storyhosts we are in starvation mode for ASH 3.'), '#default_value' => $c->get('sh_test_starvation_mode')];
    $form['sh_test_approval_min'] = ['#type' => 'textfield', '#title' => $this->t('Minimum approval percent'), '#description' => $this->t('At least these many approvals are required to pass ASH 3.'), '#default_value' => $c->get('sh_test_approval_min')];
    $form['sh_test_approval_min_starvation'] = ['#type' => 'textfield', '#title' => $this->t('Minimum starvation approval percent'), '#description' => $this->t('At least these many approvals are required to pass ASH 3 in starvation mode.'), '#default_value' => $c->get('sh_test_approval_min_starvation')];
    // D7 misspelled '#collapsed', so this never started closed.
    $form['session_requires'] = ['#type' => 'details', '#title' => $this->t('Session requirements'), '#open' => FALSE];
    $form['session_requires']['sh_test_session_party_size'] = ['#type' => 'checkbox', '#title' => $this->t('Check participants'), '#description' => $this->t('Use participants to check if session counts towards system.'), '#default_value' => $c->get('sh_test_session_party_size')];
    $form['session_requires']['sh_test_session_party_size_min'] = ['#type' => 'textfield', '#title' => $this->t('Participant requirements'), '#description' => $this->t('At least these many participants are required for this session to count in the system.'), '#default_value' => $c->get('sh_test_session_party_size_min')];
    $form['session_requires']['sh_test_session_party_size_npcs'] = ['#type' => 'checkbox', '#title' => $this->t('Count NPCs'), '#description' => $this->t('Count NPCs as participants.'), '#default_value' => $c->get('sh_test_session_party_size_npcs')];
    $form['session_requires']['sh_test_session_party_size_ashs'] = ['#type' => 'checkbox', '#title' => $this->t('Count A/SHs'), '#description' => $this->t('Count A/SHs as participants.'), '#default_value' => $c->get('sh_test_session_party_size_ashs')];
    $form['session_requires']['sh_test_session_party_size_own_pcs'] = ['#type' => 'checkbox', '#title' => $this->t('Count own PCs'), '#description' => $this->t('Count your own PCs as participants.'), '#default_value' => $c->get('sh_test_session_party_size_own_pcs')];
    $form['session_requires']['sh_test_session_expires'] = ['#type' => 'checkbox', '#title' => $this->t('Check dates held'), '#description' => $this->t('Use the date held to check if session counts towards system.'), '#default_value' => $c->get('sh_test_session_expires')];
    $form['session_requires']['sh_test_session_expires_timeframe'] = ['#type' => 'textfield', '#title' => $this->t('Expiration timeframe'), '#description' => $this->t('The session must have been held with this timeframe for it to count in the system.'), '#default_value' => $c->get('sh_test_session_expires_timeframe')];
    $form['sh_test_recalc_xp'] = ['#type' => 'submit', '#value' => $this->t('Recalculate Earned XP'), '#submit' => ['::recalculateXp'], '#weight' => 90];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_storyhosts.settings');
    foreach (self::KEYS as $key) {
      $value = $form_state->getValue($key);
      $config->set($key, in_array($key, ['sh_test_session_party_size', 'sh_test_session_party_size_npcs', 'sh_test_session_party_size_ashs', 'sh_test_session_party_size_own_pcs', 'sh_test_session_expires'], TRUE) ? (bool) $value : ($key === 'sh_test_session_party_size_min' ? (int) $value : (string) $value));
    }
    $config->save();
    parent::submitForm($form, $form_state);
  }

  /**
   * Sets every player's XP earned over time to the XP their characters earned.
   */
  public function recalculateXp(array &$form, FormStateInterface $form_state) {
    $totals = \Drupal::database()->query("SELECT n.uid, SUM(xp.xp) AS total FROM {node_field_data} n INNER JOIN {character_sheet_xp} xp ON xp.nid = n.nid WHERE n.type = 'character_sheet' AND xp.type = 'xp' GROUP BY n.uid")->fetchAllKeyed();
    $storage = \Drupal::entityTypeManager()->getStorage('user');
    foreach ($storage->loadMultiple(array_keys($totals)) as $account) {
      $account->set('field_xp_over_time', $totals[$account->id()]);
      $account->save();
    }
    $this->messenger()->addStatus($this->t('SH totals for all active players have been reset.'));
  }

}
