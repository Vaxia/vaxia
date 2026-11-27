<?php

namespace Drupal\vaxia_governance\Form;

use Drupal\Core\Entity\Entity\EntityFormDisplay;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\Entity\Node;

/**
 * Organization goals as an editable table, with the "add" form underneath
 * (D7 view organization_goals, an editableviews table): every published goal
 * by organization then priority, each row's fields edited in place and saved
 * with one button.
 */
class OrganizationGoalsForm extends FormBase {

  /**
   * Editable columns: field => header (D7 left two headers blank).
   */
  const COLUMNS = [
    'field_organization' => 'Organization',
    'field_priority' => 'Priority',
    'field_first_stat' => 'Using Stats',
    'field_second_stat' => '',
    'field_org_goal' => 'Goal',
    'field_target' => 'Target Organization',
    'field_target_government' => 'Target Government',
    'field_target_stat' => '',
    'title' => 'IC Methods',
  ];

  public function getFormId() {
    return 'vaxia_governance_organization_goals';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'organization_goal')->condition('status', 1);
    $query->sort('field_organization.target_id', 'ASC')->sort('field_priority', 'ASC')->sort('nid', 'ASC');
    $nids = array_values($query->execute());
    $form_state->set('nids', $nids);
    // D7's header style: labels and descriptions hidden in the rows.
    $form['style'] = ['#type' => 'html_tag', '#tag' => 'style', '#value' => '.organization-goals td label{display:none;} .organization-goals td .description{display:none;} .organization-goals td select{width:100px;} .organization-goals td .form-text{width:400px;}'];
    $header = [];
    foreach (self::COLUMNS as $label) {
      $header[] = $label === '' ? '' : $this->t($label);
    }
    $header[] = $this->t('Current PP');
    $header[] = $this->t('Actions');
    $form['goals'] = ['#type' => 'table', '#header' => $header, '#attributes' => ['class' => ['organization-goals']], '#tree' => TRUE];
    foreach ($storage->loadMultiple($nids) as $nid => $node) {
      $display = EntityFormDisplay::collectRenderDisplay($node, 'default');
      $parents = ['#parents' => ['goals', $nid]];
      $row = [];
      foreach (array_keys(self::COLUMNS) as $field) {
        $widget = $display->getRenderer($field);
        $row[$field] = $widget ? $widget->form($node->get($field), $parents, $form_state) : [];
      }
      $row['pp'] = ['#markup' => (string) $node->get('field_current_pp')->value];
      $row['delete'] = $node->access('delete') ? ['#type' => 'link', '#title' => $this->t('delete'), '#url' => $node->toUrl('delete-form', ['query' => ['destination' => '/setting/organization-goals']])] : [];
      $form['goals'][$nid] = $row;
    }
    if ($nids) {
      $form['actions'] = ['#type' => 'actions', 'submit' => ['#type' => 'submit', '#value' => $this->t('Save')]];
    }
    $form['#cache'] = ['tags' => ['node_list'], 'contexts' => ['user.permissions']];
    return $form;
  }

  public function validateForm(array &$form, FormStateInterface $form_state) {
    foreach ($form_state->get('nids') ?? [] as $nid) {
      $node = Node::load($nid);
      $display = EntityFormDisplay::collectRenderDisplay($node, 'default');
      foreach (array_keys(self::COLUMNS) as $field) {
        $widget = $display->getRenderer($field);
        if ($widget) {
          $widget->extractFormValues($node->get($field), ['#parents' => ['goals', $nid]], $form_state);
        }
      }
      foreach ($node->validate()->filterByFields(array_diff(array_keys($node->getFieldDefinitions()), array_keys(self::COLUMNS))) as $violation) {
        $form_state->setErrorByName('goals][' . $nid . '][' . strtok($violation->getPropertyPath(), '.'), $violation->getMessage());
      }
      $form_state->set(['changed', $nid], $node);
    }
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    foreach ($form_state->get('changed') ?? [] as $node) {
      if ($node->access('update')) {
        $node->save();
        $this->messenger()->addStatus($this->t('Saved %label.', ['%label' => $node->label()]));
      }
    }
  }

}
