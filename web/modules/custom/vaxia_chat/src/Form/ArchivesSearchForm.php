<?php

namespace Drupal\vaxia_chat\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Markup;
use Drupal\vaxia_chat\Archives;

/**
 * Searching the archives (D7 archives_form()).
 */
class ArchivesSearchForm extends FormBase {

  public function getFormId() {
    return 'archives_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $form['search_fieldset'] = ['#type' => 'fieldset'];
    $form['search_fieldset']['archives_text'] = [
      '#title' => $this->t('Search Text'),
      '#type' => 'textfield',
      '#description' => $this->t('Enter any string to search for it in the archive text.'),
      '#size' => 15,
    ];
    $form['search_fieldset']['archives_submit'] = [
      '#type' => 'button',
      '#value' => $this->t('Search archives'),
      '#ajax' => ['callback' => '::ajax', 'wrapper' => 'archives_results'],
    ];
    $text = (string) $form_state->getValue('archives_text');
    $form['archives_results'] = [
      '#title' => $this->t('Seach results'),
      '#type' => 'item',
      // Links the search built from file paths it escaped itself.
      '#markup' => $text !== '' ? Markup::create(Archives::search($text)) : $this->t('No search performed.'),
      '#prefix' => '<div id="archives_results">',
      '#suffix' => '</div>',
    ];
    $root = Archives::root();
    if ($root !== '') {
      $form['archives_raw'] = [
        '#title' => $this->t('Raw files'),
        '#type' => 'item',
        '#markup' => '<br/><b><i><a href="/' . htmlspecialchars($root, ENT_QUOTES) . '" target="_blank">Direct link to archives</a></i></b>',
      ];
    }
    return $form;
  }

  public function ajax(array $form, FormStateInterface $form_state) {
    return $form['archives_results'];
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
  }

}
