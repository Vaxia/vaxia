<?php

namespace Drupal\vaxia_items\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Item shop settings (D7 _item_shop_admin_settings()).
 */
class ShopSettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_item_shop_admin_settings';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_items.settings'];
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_items.settings');
    $form['item_shop_sellback'] = ['#title' => $this->t('Allow item shop sellback'), '#type' => 'checkbox', '#default_value' => $config->get('item_shop_sellback')];
    $form['item_shop_sellback_percentage'] = ['#title' => $this->t('Percent'), '#description' => $this->t('Sellback percentage value'), '#type' => 'textfield', '#default_value' => $config->get('item_shop_sellback_percentage')];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('vaxia_items.settings')
      ->set('item_shop_sellback', (bool) $form_state->getValue('item_shop_sellback'))
      ->set('item_shop_sellback_percentage', (float) $form_state->getValue('item_shop_sellback_percentage'))
      ->save();
    parent::submitForm($form, $form_state);
  }

}
