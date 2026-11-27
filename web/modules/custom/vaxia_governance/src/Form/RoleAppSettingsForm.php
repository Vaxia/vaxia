<?php

namespace Drupal\vaxia_governance\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\user\Entity\Role;

/**
 * Application settings (D7 admin/rpg/role_app).
 */
class RoleAppSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_role_app_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_governance.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    $roles = [];
    foreach (Role::loadMultiple() as $role) {
      if ($role->id() !== 'anonymous') {
        $roles[$role->id()] = $role->label();
      }
    }
    $quizzes = [];
    foreach (\Drupal::entityTypeManager()->getStorage('node')->loadByProperties(['type' => 'quiz']) as $quiz) {
      $quizzes[$quiz->id()] = $quiz->label();
    }
    $form['role_app_roles'] = ['#type' => 'select', '#multiple' => TRUE, '#title' => $this->t('Application roles'), '#description' => $this->t('Select all roles that role_apps apply to.'), '#options' => $roles, '#default_value' => (array) $c->get('role_app_roles')];
    // D7 declared this as '#type' => 'text', so it never showed.
    $form['application_approval_min'] = ['#type' => 'textfield', '#title' => $this->t('Approval percentage'), '#description' => $this->t('Greater than or equal to percentage of possible approvers required to approve.'), '#default_value' => $c->get('application_approval_min')];
    foreach ((array) $c->get('role_app_roles') as $rid) {
      if (!isset($roles[$rid])) {
        continue;
      }
      $settings = vaxia_governance_role_app($rid);
      $form['role_app_settings'][$rid] = ['#type' => 'details', '#title' => $this->t('Settings for @role', ['@role' => $roles[$rid]]), '#description' => $this->t('Configure the settings for each role role_app.')];
      $form['role_app_settings'][$rid]['role_app_quiz_' . $rid] = ['#type' => 'select', '#multiple' => TRUE, '#title' => $this->t('Application quiz'), '#options' => $quizzes, '#default_value' => $settings['quiz']];
      $form['role_app_settings'][$rid]['role_app_eligible_' . $rid] = ['#type' => 'select', '#title' => $this->t('Application eligable role'), '#options' => $roles, '#empty_value' => '', '#default_value' => $settings['eligible']];
      $form['role_app_settings'][$rid]['role_app_view_' . $rid] = ['#type' => 'select', '#multiple' => TRUE, '#title' => $this->t('Application viewing role'), '#options' => $roles, '#default_value' => $settings['view']];
      $form['role_app_settings'][$rid]['role_app_approve_' . $rid] = ['#type' => 'select', '#multiple' => TRUE, '#title' => $this->t('Application approving role'), '#options' => $roles, '#default_value' => $settings['approve']];
    }
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    $all = (array) $c->get('role_app_settings');
    foreach ((array) $c->get('role_app_roles') as $rid) {
      if ($form_state->hasValue('role_app_eligible_' . $rid)) {
        $all[$rid] = [
          'quiz' => array_values(array_map('intval', array_filter((array) $form_state->getValue('role_app_quiz_' . $rid)))),
          'eligible' => (string) $form_state->getValue('role_app_eligible_' . $rid),
          'view' => array_values(array_filter((array) $form_state->getValue('role_app_view_' . $rid))),
          'approve' => array_values(array_filter((array) $form_state->getValue('role_app_approve_' . $rid))),
        ];
      }
    }
    $c->set('role_app_roles', array_values(array_filter((array) $form_state->getValue('role_app_roles'))))
      ->set('application_approval_min', (string) $form_state->getValue('application_approval_min'))
      ->set('role_app_settings', $all)
      ->save();
    parent::submitForm($form, $form_state);
  }

}
