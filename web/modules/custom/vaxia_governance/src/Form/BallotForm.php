<?php

namespace Drupal\vaxia_governance\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\Role;

/**
 * A proposal's ballot builder (D7 node/%/webform/components on proposals):
 * questions, who may vote, and the launch button.
 */
class BallotForm extends FormBase {

  public function getFormId() {
    return 'vaxia_governance_ballot_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?NodeInterface $node = NULL) {
    $form_state->set('nid', $node->id());
    $webform = vaxia_governance_webform($node);
    $open = vaxia_governance_is_open($node);
    $voted = vaxia_governance_vote_count($node);
    $rows = [];
    foreach ($webform->getElementsDecoded() as $key => $element) {
      if (($element['#type'] ?? '') === 'webform_actions') {
        continue;
      }
      $options = isset($element['#options']) ? ' (' . implode(', ', $element['#options']) . ')' : '';
      $rows[] = [(string) ($element['#title'] ?? ($element['#markup'] ?? $key)) . $options, !empty($element['#required']) ? $this->t('Yes') : $this->t('No'), $element['#type']];
    }
    $form['elements'] = ['#type' => 'table', '#header' => [$this->t('Element'), $this->t('Mandatory'), $this->t('Type')], '#rows' => $rows, '#empty' => $this->t('No Components, add a component below.')];
    $form['add'] = [
      '#type' => 'details',
      '#title' => $this->t('Add a new form element'),
      '#open' => TRUE,
      '#description' => $this->t('Mandatory form components are questions that must be answered before a submission can be made. Available form elements are: text to display, radio buttons or a dropdown for choosing between set values, a multi-line text area, and a single-line text field.'),
    ];
    $form['add']['name'] = ['#type' => 'textfield', '#title' => $this->t('Label for new element'), '#default_value' => $this->t('New element')];
    $form['add']['type'] = ['#type' => 'select', '#title' => $this->t('Type of new element'), '#options' => ['select' => $this->t('Radio, checkboxes, or dropdown'), 'textarea' => $this->t('Textarea'), 'textfield' => $this->t('Textfield'), 'markup' => $this->t('Markup')]];
    $form['add']['items'] = ['#type' => 'textarea', '#title' => $this->t('Options'), '#description' => $this->t('Enter one option per line.'), '#states' => ['visible' => [':input[name="type"]' => ['value' => 'select']]]];
    $form['add']['mandatory'] = ['#type' => 'checkbox', '#title' => $this->t('Required?')];
    $form['add']['add'] = ['#type' => 'submit', '#value' => $this->t('Add new element'), '#submit' => ['::addElement']];

    $roles = [];
    foreach (Role::loadMultiple() as $role) {
      if ($role->hasPermission('vote on proposal') && !in_array($role->id(), ['anonymous', 'authenticated'], TRUE)) {
        $roles[$role->id()] = $role->label();
      }
    }
    $rules = $webform->getAccessRules();
    $form['role_control'] = [
      '#tree' => TRUE,
      '#type' => 'details',
      '#title' => $this->t('Submission access'),
      '#open' => TRUE,
      '#description' => $this->t('These permissions affect which roles can submit this webform. It does not prevent access to the webform page to view results.'),
    ];
    $form['role_control']['roles'] = ['#type' => 'checkboxes', '#title' => $this->t('Roles that can submit this webform'), '#options' => $roles, '#default_value' => array_intersect($rules['create']['roles'] ?? [], array_keys($roles)), '#description' => $this->t('Voters are users who have posted on the site in the last %t.', ['%t' => vaxia_governance_setting('proposal_voter_timeframe')])];
    $form['role_control']['disallowed_roles'] = ['#type' => 'checkboxes', '#title' => $this->t('Roles that CANNOT submit this webform'), '#options' => $roles, '#default_value' => (array) \Drupal::state()->get('vaxia_governance.proposal_barred.' . $node->id(), []), '#description' => $this->t('Sometimes a proposal needs to disallow a specific set of users from voting due to conflict of interest. Even if a user is in a role that can vote normally, if they have one of these roles they are barred from voting.')];
    $form['role_control']['submit_roles'] = ['#type' => 'submit', '#value' => $this->t('Save voting roles'), '#submit' => ['::saveRoles'], '#tree' => FALSE];

    if (!$open && $voted === 0) {
      $form['launch'] = ['#type' => 'details', '#title' => $this->t('Launch proposal'), '#open' => TRUE, '#description' => $this->t('When you are ready to make this proposal live so that others may vote on it, click this button.')];
      $form['launch']['launch_submit'] = ['#type' => 'submit', '#value' => $this->t('Click here to launch the proposal'), '#submit' => ['::launch']];
    }
    elseif (!$open) {
      $this->messenger()->addError($this->t('This proposal has already been voted on and closed. It cannot be modified after closing.'));
      vaxia_governance_disable($form);
    }
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
  }

  private function node(FormStateInterface $form_state): NodeInterface {
    return \Drupal\node\Entity\Node::load($form_state->get('nid'));
  }

  public function addElement(array &$form, FormStateInterface $form_state) {
    $webform = vaxia_governance_webform($this->node($form_state));
    $type = $form_state->getValue('type');
    $label = trim((string) $form_state->getValue('name'));
    $element = $type === 'markup' ? ['#type' => 'webform_markup', '#markup' => $label] : ['#type' => $type, '#title' => $label];
    if ($type === 'select') {
      $options = [];
      foreach (preg_split('/\R/', (string) $form_state->getValue('items')) as $line) {
        if (trim($line) !== '') {
          // D7 keyed options by md5 of their text.
          $options[md5(trim($line))] = trim($line);
        }
      }
      $element = ['#type' => 'radios', '#title' => $label, '#options' => $options];
    }
    if ($form_state->getValue('mandatory')) {
      $element['#required'] = TRUE;
    }
    $elements = $webform->getElementsDecoded();
    // New questions go above the closing comments box.
    $comments = $elements['additional_comments'] ?? NULL;
    unset($elements['additional_comments']);
    $elements['proposal_' . uniqid()] = $element;
    if ($comments) {
      $elements['additional_comments'] = $comments;
    }
    $webform->setElements($elements)->save();
    $this->messenger()->addStatus($this->t('New element %name added.', ['%name' => $label]));
  }

  public function saveRoles(array &$form, FormStateInterface $form_state) {
    $node = $this->node($form_state);
    $roles = array_values(array_filter((array) $form_state->getValue(['role_control', 'roles'])));
    $barred = array_values(array_filter((array) $form_state->getValue(['role_control', 'disallowed_roles'])));
    $webform = vaxia_governance_webform($node);
    $rules = $webform->getAccessRules();
    $rules['create']['roles'] = $roles;
    $webform->setAccessRules($rules)->save();
    \Drupal::state()->set('vaxia_governance.proposal_barred.' . $node->id(), $barred);
    vaxia_governance_set_eligible($node, $roles, $barred);
    $this->messenger()->addStatus($this->t('The form settings have been updated.'));
  }

  public function launch(array &$form, FormStateInterface $form_state) {
    $node = $this->node($form_state);
    vaxia_governance_set_open($node, TRUE);
    $this->messenger()->addStatus($this->t('This proposal has been opened for voting.'));
    $form_state->setRedirectUrl($node->toUrl());
  }

}
