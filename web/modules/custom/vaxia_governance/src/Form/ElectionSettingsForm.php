<?php

namespace Drupal\vaxia_governance\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\user\Entity\Role;

/**
 * RPG Election settings and actions (D7 admin/rpg/rpg_election).
 */
class ElectionSettingsForm extends ConfigFormBase {

  const KEYS = [
    'election_rate', 'election_term_limits', 'election_term_limit_limits', 'election_role',
    'election_default_forum', 'election_election_intro', 'election_min_numb', 'election_min_numb_val',
    'election_min_perc', 'election_min_perc_val', 'election_runoff',
  ];

  public function getFormId() {
    return '_rpg_election_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_governance.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    $roles = [];
    foreach (Role::loadMultiple() as $role) {
      $roles[$role->id()] = $role->label();
    }
    $form['election_day'] = ['#type' => 'textfield', '#title' => $this->t('Last rpg_election day'), '#description' => $this->t('The last-held rpg_election day for calculating further rpg_elections.'), '#disabled' => TRUE, '#value' => \Drupal::state()->get('vaxia_governance.election_day', '')];
    $positions = vaxia_governance_positions();
    $form['election_position_details'] = ['#type' => 'details', '#title' => $this->t('Election positions'), '#open' => TRUE];
    $form['election_position_details']['election_positions'] = ['#type' => 'textarea', '#title' => $this->t('Available positions'), '#description' => $this->t('List postitions available on the site, one per row.'), '#default_value' => $positions ? implode("\n", $positions) : "Setting Lead\nSystem Lead\nSocial Lead"];
    $counts = $c->get('election_position_counts') ?: [];
    $descs = $c->get('election_position_descs') ?: [];
    foreach ($positions as $key => $name) {
      $form['election_position_details']['details_' . $key] = ['#type' => 'details', '#title' => $name];
      $form['election_position_details']['details_' . $key]['election_position_count_' . $key] = ['#type' => 'select', '#title' => $this->t('How many slots in this position?'), '#options' => range(0, 10), '#default_value' => $counts[$key] ?? 2];
      $form['election_position_details']['details_' . $key]['election_position_desc_' . $key] = ['#type' => 'textarea', '#title' => $this->t('Additional description for the position.'), '#default_value' => $descs[$key] ?? ''];
    }
    $form['election_times'] = ['#type' => 'details', '#title' => $this->t('Election times')];
    $form['election_times']['election_rate'] = ['#type' => 'textfield', '#title' => $this->t('Election rate'), '#description' => $this->t('How often to hold rpg_elections. Use timeframe formats strtotime will recognize.'), '#default_value' => $c->get('election_rate')];
    $form['election_times']['election_term_limits'] = ['#type' => 'checkbox', '#title' => $this->t('Term limits'), '#default_value' => $c->get('election_term_limits')];
    $form['election_times']['election_term_limit_limits'] = ['#type' => 'textfield', '#title' => $this->t('Term limit'), '#description' => $this->t('How many consecutive (not lifetime) terms can a user serve in the same position.'), '#default_value' => $c->get('election_term_limit_limits')];
    $form['election_roles'] = ['#type' => 'details', '#title' => $this->t('Election roles')];
    $form['election_roles']['election_run_roles'] = ['#type' => 'select', '#multiple' => TRUE, '#title' => $this->t('Election run role'), '#description' => $this->t('These roles are allowed to run.'), '#options' => $roles, '#default_value' => (array) $c->get('election_run_roles')];
    $form['election_roles']['election_role'] = ['#type' => 'select', '#title' => $this->t('Elected role'), '#description' => $this->t('This role is considered the elected role. Election winners automatically recieve this role.'), '#options' => $roles, '#empty_value' => '', '#default_value' => $c->get('election_role')];
    $forums = [];
    foreach (\Drupal::entityTypeManager()->getStorage('taxonomy_term')->loadTree('forums') as $term) {
      $forums[$term->tid] = str_repeat('-', $term->depth) . $term->name;
    }
    $form['election_setup'] = ['#type' => 'details', '#title' => $this->t('Election setup')];
    $form['election_setup']['election_default_forum'] = ['#type' => 'select', '#title' => $this->t('Election forum'), '#description' => $this->t('The default forum to post the rpg_election proposal in.'), '#options' => $forums, '#empty_value' => 0, '#default_value' => $c->get('election_default_forum')];
    $form['election_setup']['election_election_intro'] = ['#type' => 'textarea', '#title' => $this->t('Election intro'), '#default_value' => $c->get('election_election_intro')];
    $form['election_counting'] = ['#type' => 'details', '#title' => $this->t('Election counting')];
    $form['election_counting']['election_min_numb'] = ['#type' => 'checkbox', '#title' => $this->t('Use a minimum number of votes for voting.'), '#default_value' => $c->get('election_min_numb')];
    $form['election_counting']['election_min_numb_val'] = ['#type' => 'textfield', '#title' => $this->t('Min votes'), '#default_value' => $c->get('election_min_numb_val')];
    $form['election_counting']['election_min_perc'] = ['#type' => 'checkbox', '#title' => $this->t('Use a minimum percentage of votes for voting.'), '#default_value' => $c->get('election_min_perc')];
    $form['election_counting']['election_min_perc_val'] = ['#type' => 'textfield', '#title' => $this->t('Min vote%'), '#default_value' => $c->get('election_min_perc_val')];
    $form['election_counting']['election_runoff'] = ['#type' => 'checkbox', '#title' => $this->t('Automatically run run-offs.'), '#default_value' => $c->get('election_runoff')];
    $form['election_actions'] = ['#type' => 'details', '#title' => $this->t('Election actions'), '#open' => TRUE, '#weight' => 100];
    $form['election_actions']['election_open_submit'] = ['#type' => 'submit', '#value' => $this->t('Hold an election off-cycle'), '#submit' => ['::hold']];
    $form['election_actions']['election_close_election'] = ['#type' => 'submit', '#value' => $this->t('Close open elections'), '#submit' => ['::close']];
    $form['election_actions']['election_check_election'] = ['#type' => 'submit', '#value' => $this->t('Check open election votes'), '#submit' => ['::check']];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    foreach (self::KEYS as $key) {
      $value = $form_state->getValue($key);
      $c->set($key, in_array($key, ['election_term_limits', 'election_min_numb', 'election_min_perc', 'election_runoff'], TRUE) ? (bool) $value : ($key === 'election_default_forum' ? (int) $value : (string) $value));
    }
    $c->set('election_run_roles', array_values(array_filter((array) $form_state->getValue('election_run_roles'))));
    $names = array_filter(array_map('trim', preg_split('/\R/', (string) $form_state->getValue('election_positions'))));
    $c->set('election_positions', implode("\n", $names));
    $counts = $c->get('election_position_counts') ?: [];
    $descs = $c->get('election_position_descs') ?: [];
    foreach (array_keys(vaxia_governance_positions()) as $key) {
      if ($form_state->hasValue('election_position_count_' . $key)) {
        $counts[$key] = (int) $form_state->getValue('election_position_count_' . $key);
        $descs[$key] = (string) $form_state->getValue('election_position_desc_' . $key);
      }
    }
    $c->set('election_position_counts', $counts)->set('election_position_descs', $descs)->save();
    parent::submitForm($form, $form_state);
    vaxia_governance_position_fields();
  }

  public function hold(array &$form, FormStateInterface $form_state) {
    vaxia_governance_vacate_seats();
    \Drupal::state()->set('vaxia_governance.election_day', date('m/d/Y'));
    vaxia_governance_open_election();
  }

  public function close(array &$form, FormStateInterface $form_state) {
    foreach (vaxia_governance_open_elections() as $node) {
      vaxia_governance_set_open($node, FALSE);
    }
  }

  public function check(array &$form, FormStateInterface $form_state) {
    foreach (vaxia_governance_open_elections() as $node) {
      $results = vaxia_governance_election_results($node);
      $this->messenger()->addStatus('Results:');
      $votes = [];
      foreach ($results as $positions) {
        foreach ($positions as $position => $uids) {
          foreach ($uids as $uid) {
            $votes[$position][$uid] = ($votes[$position][$uid] ?? 0) + 1;
          }
        }
      }
      foreach ($votes as $position => $users) {
        $this->messenger()->addStatus($this->t('Votes for position @position.', ['@position' => $position]));
        foreach ($users as $uid => $count) {
          $this->messenger()->addStatus($this->t('@votes votes for @user.', ['@votes' => $count, '@user' => \Drupal\user\Entity\User::load($uid)->getAccountName()]));
        }
      }
      $this->messenger()->addStatus('Winners:');
      foreach (vaxia_governance_election_winners($node, $results) as $position => $uids) {
        foreach ($uids as $uid) {
          $this->messenger()->addStatus($this->t('@user for position @position.', ['@user' => \Drupal\user\Entity\User::load($uid)->getAccountName(), '@position' => $position]));
        }
      }
    }
  }

}
