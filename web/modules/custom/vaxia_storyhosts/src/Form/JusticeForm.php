<?php

namespace Drupal\vaxia_storyhosts\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Justice calculator (D7 justice_form): works out the punishment left after a
 * player's three rolls against the court.
 */
class JusticeForm extends FormBase {

  public function getFormId() {
    return '_justice_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_storyhosts.settings');
    $tiers = range(1, 5);
    $form['setup'] = ['#type' => 'fieldset', '#title' => $this->t('Setup values')];
    $form['setup']['tier'] = ['#type' => 'select', '#title' => $this->t('Hostility level'), '#options' => array_combine($tiers, $tiers), '#description' => $this->t('Set the hostility of the court.'), '#required' => TRUE];
    $form['setup']['pp'] = ['#type' => 'textfield', '#title' => $this->t('PP Required'), '#description' => $this->t('Set the PP according to the Punishment table.'), '#required' => TRUE];
    $form['setup']['rep'] = ['#type' => 'textfield', '#title' => $this->t('Player reputation'), '#description' => $this->t('Reputation score with the local government.'), '#required' => TRUE];
    $form['rolls'] = ['#type' => 'fieldset', '#title' => $this->t('Player rolls')];
    for ($i = 1; $i < 4; $i++) {
      $form['rolls']['roll_' . $i] = ['#type' => 'fieldset', '#title' => $this->t('Roll @i', ['@i' => $i])];
      foreach (['might' => 'Might', 'roll' => 'Roll'] as $key => $label) {
        foreach ([1, 2] as $j) {
          $form['rolls']['roll_' . $i][$key . '_' . $i . '_' . $j] = ['#type' => 'textfield', '#title' => $this->t($label . ' @j', ['@j' => $j]), '#required' => TRUE];
        }
      }
    }
    $form['submit'] = ['#type' => 'submit', '#value' => $this->t('Calculate results')];
    $form['punishment'] = ['#type' => 'item', '#title' => $this->t('Punishment Table'), '#markup' => (string) $config->get('justice_form_punishments')];
    $form['reputation'] = ['#type' => 'item', '#title' => $this->t('Reputation Table'), '#markup' => (string) $config->get('justice_form_reputations')];
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_storyhosts.settings');
    $values = $form_state->getValues();
    $tier = (int) $values['tier'];
    $pp = (float) $values['pp'];
    $rep = (float) $values['rep'];
    $lines = [$this->t('The Tier level is Tier @tier.', ['@tier' => $tier])];
    if ($rep < -16) {
      $tier++;
      $lines[] = $this->t('A bad reputation raised the Tier level to @tier.', ['@tier' => $tier]);
    }
    if ($rep > 16) {
      $tier--;
      $lines[] = $this->t('A good reputation lowered the Tier level to @tier.', ['@tier' => $tier]);
    }
    $diff = (float) $config->get('justice_form_base_pp') * $tier;
    $lines[] = $this->t('Rolls at Tier @tier, @diff difficulty to reduce @pp PP to zero.', ['@tier' => $tier, '@diff' => $diff, '@pp' => $pp]);
    for ($i = 1; $i < 4; $i++) {
      $ruling = _dice_ruler_make_two_trait_ruling($values['might_' . $i . '_1'], $diff, 0, $values['roll_' . $i . '_1'], $values['might_' . $i . '_2'], $diff, 0, $values['roll_' . $i . '_2']);
      // D7 read $ruling[7]; the ruler returns named keys, 'pp' formatted.
      $gained = str_replace(',', '', (string) ($ruling['pp'] ?? 0));
      $gained = is_numeric($gained) ? max(0, (float) $gained) : 0;
      $lines[] = $this->t('Roll #@i generated @this_pp towards reducing the crime PP.', ['@i' => $i, '@this_pp' => $gained]);
      $pp -= $gained;
    }
    $pp = max(0, $pp);
    $lines[] = $this->t('Crime PP leftover: @pp', ['@pp' => $pp]);
    $unit = trim((string) ($config->get('justice_form_timeunit_' . $tier) ?: 'days'));
    $lines[] = $this->t('Punishment due: @silver silver OR @service service RP OR @time @unit.', [
      '@silver' => $tier * (float) ($config->get('justice_form_silver_' . $tier) ?? 10) * $pp,
      '@service' => $tier * 0.1 * $pp,
      '@time' => (float) ($config->get('justice_form_time_' . $tier) ?? 1) * $pp,
      '@unit' => $unit,
    ]);
    $this->messenger()->addStatus(['#markup' => implode('<br/>', array_map('strval', $lines))]);
    $form_state->setRebuild();
  }

}
