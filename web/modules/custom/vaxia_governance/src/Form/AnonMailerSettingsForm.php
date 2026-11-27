<?php

namespace Drupal\vaxia_governance\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Anon Mailer settings (D7 admin/vaxia/anon_mailer).
 */
class AnonMailerSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_anon_mailer_settings_form';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_governance.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_governance.settings');
    $nodes = \Drupal::entityQuery('node')->accessCheck(FALSE)->exists('webform')->sort('title')->execute();
    $webforms = [];
    foreach (\Drupal\node\Entity\Node::loadMultiple($nodes) as $node) {
      $webforms[$node->id()] = $node->label();
    }
    $forums = [];
    foreach (\Drupal::entityTypeManager()->getStorage('taxonomy_term')->loadTree('forums') as $term) {
      $forums[$term->tid] = $term->name;
    }
    $form['anon_mailer_webform'] = ['#type' => 'select', '#title' => $this->t('Mailer Webform'), '#description' => $this->t('Select the webform to associate with the anon. mailer feature.'), '#options' => $webforms, '#default_value' => $c->get('anon_mailer_webform')];
    $form['anon_mailer_form'] = ['#type' => 'select', '#title' => $this->t('Mailer Forum'), '#description' => $this->t('Select the forum to post mailer posts to'), '#options' => $forums, '#empty_value' => 0, '#default_value' => $c->get('anon_mailer_form')];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('vaxia_governance.settings')
      ->set('anon_mailer_webform', (int) $form_state->getValue('anon_mailer_webform'))
      ->set('anon_mailer_form', (int) $form_state->getValue('anon_mailer_form'))
      ->save();
    parent::submitForm($form, $form_state);
  }

}
