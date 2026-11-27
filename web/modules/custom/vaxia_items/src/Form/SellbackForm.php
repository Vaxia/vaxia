<?php

namespace Drupal\vaxia_items\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Character;
use Drupal\vaxia_items\Shop;

/**
 * "Sell back <item>" on an item the viewer's character owns (D7
 * _item_shop_sellback_form()).
 */
class SellbackForm extends FormBase {

  public function getFormId() {
    return '_item_shop_sellback_form';
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?NodeInterface $item = NULL) {
    $form_state->set('item', (int) $item->id());
    $form['item_shop_sellback_help'] = [
      '#markup' => '<div class="sellback_shop_help">' . $this->t("%title may be automatically sold for %amount silver at %percent% of it's value.", [
        '%title' => $item->label(),
        '%amount' => Shop::sellbackValue($item),
        '%percent' => (float) $this->config('vaxia_items.settings')->get('item_shop_sellback_percentage'),
      ]) . '</div>',
    ];
    $form['item_shop_sellback'] = ['#type' => 'submit', '#value' => $this->t('Sell back @title', ['@title' => $item->label()])];
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $item = Node::load($form_state->get('item'));
    if (!$item || !Shop::sellbackAllowed($item)) {
      return;
    }
    $character = $item->get('field_character_owner')->entity;
    $value = Shop::sellbackValue($item);
    $xp = \Drupal::service('vaxia_core.xp');
    $this->messenger()->addStatus('Selling back item...');
    $xp->update((int) $character->id(), $value, 'silver');
    $xp->log((int) $character->id(), 'item_shop', (string) $this->t('@xp silver added by System on selling back @title by @user.', ['@xp' => $value, '@title' => $item->label(), '@user' => $this->currentUser()->getAccountName()]));
    $this->messenger()->addStatus($this->t('%value silver added to %title.', ['%value' => $value, '%title' => $character->label()]));
    // Back to the admin, off the sheet, out of play.
    $item->setOwnerId(1);
    $item->set('field_character_owner', NULL);
    vaxia_items_system_save($item);
    Character::forceState(Node::load($item->id()), 'item_evaluation_removed_from_game', (string) $this->t('This item has been sold back to the Item Shop.'));
  }

}
