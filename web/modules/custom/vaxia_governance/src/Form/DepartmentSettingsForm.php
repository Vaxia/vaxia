<?php

namespace Drupal\vaxia_governance\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\user\Entity\Role;

/**
 * Department settings (D7 admin/rpg/departments).
 */
class DepartmentSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_department_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_governance.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    $departments = vaxia_governance_departments();
    $form['departments'] = ['#type' => 'textarea', '#title' => $this->t('Available departments'), '#description' => $this->t('List departments available on the site, one per row.'), '#default_value' => implode("\n", $departments)];
    $form['roles'] = ['#type' => 'fieldset', '#title' => $this->t('Department associated role.'), '#description' => $this->t('Each department has a role that grants access to voting and other memebership features.')];
    $roles = [];
    foreach (Role::loadMultiple() as $role) {
      if (!in_array($role->id(), ['anonymous', 'authenticated'], TRUE)) {
        $roles[$role->id()] = $role->label();
      }
    }
    $current = vaxia_governance_department_roles();
    foreach ($departments as $key => $name) {
      // D7 had no "none" here and defaulted to anonymous: saving gave
      // anonymous visitors the vote.
      $form['roles']['department_roles_' . $key] = ['#type' => 'select', '#title' => $this->t('Department role: @d', ['@d' => $name]), '#options' => $roles, '#empty_value' => '', '#default_value' => $current[$key] ?? ''];
    }
    $form['department_vacation_cutoff'] = ['#type' => 'textarea', '#title' => $this->t('Vacation substitute cutoff'), '#description' => $this->t('Vacation length at which a second in command is required.'), '#default_value' => $c->get('department_vacation_cutoff')];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    $names = array_filter(array_map('trim', preg_split('/\R/', (string) $form_state->getValue('departments'))));
    $c->set('departments', implode("\n", $names));
    $roles = vaxia_governance_department_roles();
    foreach (array_keys(vaxia_governance_departments()) as $key) {
      if ($form_state->hasValue('department_roles_' . $key)) {
        $roles[$key] = (string) $form_state->getValue('department_roles_' . $key);
      }
    }
    $roles = array_filter($roles);
    $c->set('department_roles', $roles);
    $c->set('department_vacation_cutoff', (string) $form_state->getValue('department_vacation_cutoff'));
    // Elections are for the departments' leads.
    $c->set('election_positions', implode("\n", array_map(fn($n) => $n . ' ' . $this->t('Lead'), $names)));
    $c->save();
    foreach ($roles as $rid) {
      user_role_grant_permissions($rid, ['vote on proposal']);
    }
    parent::submitForm($form, $form_state);
  }

}
