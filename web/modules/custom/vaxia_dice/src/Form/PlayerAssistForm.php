<?php

namespace Drupal\vaxia_dice\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * "Should I try a multi-attack?"; built by D7's _dice_ruler_player_assist_form().
 */
class PlayerAssistForm extends FormBase {

  public function getFormId() {
    return '_dice_ruler_player_assist_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    return _dice_ruler_player_assist_form($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $form_state->setRebuild();
  }

}
