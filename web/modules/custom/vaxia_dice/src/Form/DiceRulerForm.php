<?php

namespace Drupal\vaxia_dice\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * The dice ruler form; built by D7's dice_ruler_form() (dice_ruler.inc).
 * "Rule roll" is a plain button: it rebuilds the form, which works out the
 * rulings from the values.
 */
class DiceRulerForm extends FormBase {

  public function getFormId() {
    return 'dice_ruler_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    return dice_ruler_form($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $form_state->setRebuild();
  }

}
