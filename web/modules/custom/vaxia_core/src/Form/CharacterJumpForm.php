<?php

namespace Drupal\vaxia_core\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;

/**
 * Jump menu to a character sheet (D7 ctools jump menu of the "My Characters"
 * block): picking a character goes to it.
 */
class CharacterJumpForm extends FormBase {

  public function getFormId() {
    return 'vaxia_core_character_jump';
  }

  /**
   * @param array $options
   *   Group label => [nid => title].
   */
  public function buildForm(array $form, FormStateInterface $form_state, array $options = []) {
    $form['jump'] = [
      '#type' => 'select',
      '#title' => $this->t('Character'),
      '#title_display' => 'invisible',
      '#options' => $options,
      '#empty_option' => $this->t('- Chose a Character -'),
      '#attributes' => ['onchange' => 'if (this.value) { this.form.submit(); }'],
    ];
    $form['go'] = ['#type' => 'submit', '#value' => $this->t('Go')];
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    if ($nid = $form_state->getValue('jump')) {
      $form_state->setRedirectUrl(Url::fromRoute('entity.node.canonical', ['node' => $nid]));
    }
  }

}
