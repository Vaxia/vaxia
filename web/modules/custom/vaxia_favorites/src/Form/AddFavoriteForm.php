<?php

namespace Drupal\vaxia_favorites\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * "add this page" under the favorites list (D7 favorites_add_favorite_form):
 * saves the current page, with its query string, under the title given.
 */
class AddFavoriteForm extends FormBase {

  public function getFormId() {
    return 'favorites_add_favorite_form';
  }

  /**
   * @param string $title
   *   The page title, the default for the favorite's title.
   */
  public function buildForm(array $form, FormStateInterface $form_state, $title = '') {
    $request = $this->getRequest();
    $query = $request->query->all();
    unset($query['q']);
    $form['add'] = [
      '#type' => 'details',
      '#title' => $this->t('add this page'),
      '#open' => FALSE,
      'title' => [
        '#type' => 'textfield',
        '#title' => $this->t('Title'),
        '#title_display' => 'invisible',
        '#size' => 20,
        '#maxlength' => 255,
        '#default_value' => $title,
        '#attributes' => [
          'style' => 'width: 90%',
          'class' => ['favorites-add-textfield'],
        ],
      ],
      'submit' => [
        '#type' => 'submit',
        '#value' => $this->t('Add'),
      ],
      // D7 kept the internal (unaliased) path without its leading slash.
      'path' => [
        '#type' => 'value',
        '#value' => ltrim(\Drupal::service('path.current')->getPath($request), '/'),
      ],
      'query' => [
        '#type' => 'value',
        '#value' => serialize($query),
      ],
    ];
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $uid = (int) $this->currentUser()->id();
    $path = $form_state->getValue('path');
    $query = $form_state->getValue('query');
    $db = \Drupal::database();
    // Adding a page again moves it to the end under the new title.
    $db->delete('favorites')
      ->condition('uid', $uid)
      ->condition('path', $path)
      ->condition('query', $query)
      ->execute();
    $weight = (int) $db->query('SELECT MAX(weight) FROM {favorites} WHERE uid = :uid', [':uid' => $uid])->fetchField();
    $db->insert('favorites')
      ->fields([
        'uid' => $uid,
        'path' => $path,
        'query' => $query,
        'title' => (string) $form_state->getValue('title'),
        'timestamp' => \Drupal::time()->getRequestTime(),
        'weight' => $weight + 1,
      ])
      ->execute();
  }

}
