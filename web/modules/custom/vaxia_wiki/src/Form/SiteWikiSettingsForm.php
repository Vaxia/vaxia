<?php

namespace Drupal\vaxia_wiki\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Site wiki settings (D7 admin/config/system/site_wiki).
 */
class SiteWikiSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_site_wiki_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_wiki.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $c = $this->config('vaxia_wiki.settings');
    $form['site_wiki_settings'] = ['#type' => 'details', '#title' => $this->t('Site wiki settings'), '#open' => TRUE];
    $form['site_wiki_settings']['site_wiki_revision'] = ['#type' => 'checkbox', '#title' => $this->t('Require revision notes'), '#description' => $this->t('Check to make sure that revision notes are required.'), '#default_value' => $c->get('site_wiki_revision')];
    $form['site_wiki_templates'] = ['#type' => 'details', '#title' => $this->t('Site wiki templates'), '#tree' => TRUE];
    $templates = array_values(array_filter((array) $c->get('site_wiki_templates'), fn($t) => !empty($t['template_name'])));
    foreach ($templates as $index => $template) {
      $form['site_wiki_templates'][$index]['template_name'] = ['#type' => 'textfield', '#title' => $this->t('Template name'), '#default_value' => $template['template_name']];
      $form['site_wiki_templates'][$index]['template'] = ['#type' => 'textarea', '#title' => $this->t('Template'), '#default_value' => $template['template']];
    }
    // A blank row adds a template.
    $new = count($templates);
    $form['site_wiki_templates'][$new]['template_name'] = ['#type' => 'textfield', '#title' => $this->t('Template name'), '#description' => $this->t('The name the template will be listed under for the display.')];
    $form['site_wiki_templates'][$new]['template'] = ['#type' => 'textarea', '#title' => $this->t('Template'), '#description' => $this->t('The text of the template.')];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $templates = array_values(array_filter((array) $form_state->getValue('site_wiki_templates'), fn($t) => trim((string) ($t['template_name'] ?? '')) !== ''));
    $this->config('vaxia_wiki.settings')
      ->set('site_wiki_revision', (bool) $form_state->getValue('site_wiki_revision'))
      ->set('site_wiki_templates', array_map(fn($t) => ['template_name' => trim($t['template_name']), 'template' => (string) $t['template']], $templates))
      ->save();
    parent::submitForm($form, $form_state);
  }

}
