<?php

namespace Drupal\vaxia_items\Form;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\NodeInterface;
use Drupal\vaxia_characters\Silver;
use Drupal\vaxia_core\Character;
use Drupal\vaxia_items\Shop;

/**
 * A character's Item shop tab: tick items, "Purchase items" (D7 view
 * item_shop with its add-item form field).
 */
class ShopForm extends FormBase {

  public function getFormId() {
    return 'views_form_item_shop_item_shop';
  }

  public static function access(NodeInterface $node, AccountInterface $account) {
    // A sheet the visitor may view (review, 2026-11-26: any sheet id, even
    // unpublished, showed its shop).
    return AccessResult::allowedIf(Character::isSheet($node))->addCacheableDependency($node)->andIf($node->access('view', $account, TRUE));
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?NodeInterface $node = NULL) {
    $form_state->set('character', (int) $node->id());
    $rows = [];
    foreach (Shop::stock($node) as $nid => $item) {
      $rows[$nid] = [
        'item_shop' => ['data' => ['#type' => 'checkbox', '#return_value' => 1, '#parents' => ['buy', $nid], '#name' => 'buy[' . $nid . ']']],
        'title' => ['data' => $item->toLink()->toRenderable(), 'class' => ['views-field', 'views-field-title']],
        'field_item_type' => ['data' => ['#plain_text' => $item->get('field_item_type')->entity ? $item->get('field_item_type')->entity->label() : ''], 'class' => ['views-field', 'views-field-field-item-type']],
        'field_value' => ['data' => ['#plain_text' => (string) $item->get('field_value')->value], 'class' => ['views-field', 'views-field-field-value']],
      ];
    }
    $form['items'] = [
      '#type' => 'table',
      '#header' => ['', $this->t('Title'), $this->t('Type'), $this->t('Value')],
      '#rows' => $rows,
      '#empty' => $this->t('No items available.'),
    ];
    // The row checkboxes are read from the raw input on submit.
    $own = $node->getOwnerId() == $this->currentUser()->id();
    $form['actions'] = ['#type' => 'actions'];
    $form['actions']['submit'] = [
      '#type' => 'submit',
      '#value' => $own ? $this->t('Purchase items') : $this->t('This is not your character sheet'),
      '#disabled' => !$own,
    ];
    $footer = '<b>Customization:</b> If you would like a modified version of any of these items, please ask an SH on the forums to help you out.';
    if ($own) {
      $available = Silver::available((int) $node->id());
      $worth = Silver::worth(Silver::economy((int) $node->id()));
      $footer .= '</br>' . $this->t('You have :silver available to spend.', [':silver' => number_format($available, 2)])
        . '</br>' . $this->t('If you cash in your economy levels, you have an additional :worth silver available to spend.', [':worth' => number_format($worth, 2)])
        . '</br>' . $this->t('Your total worth including silver and economy is :total_worth silver.', [':total_worth' => number_format($available + $worth, 2)]);
    }
    $form['footer'] = ['#markup' => $footer];
    $form['#cache']['max-age'] = 0;
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $messenger = $this->messenger();
    $character = \Drupal\node\Entity\Node::load($form_state->get('character'));
    if (!$character || $character->getOwnerId() != $this->currentUser()->id()) {
      return;
    }
    $messenger->addStatus($this->t('Processing your purchase...'));
    $input = $form_state->getUserInput()['buy'] ?? [];
    $stock = Shop::stock($character);
    $buying = array_intersect_key($stock, array_filter((array) $input));
    $total = array_sum(array_map([Shop::class, 'price'], $buying));
    if (Shop::totalWorth($character) < $total) {
      $messenger->addStatus($this->t('You do not have enough silver for all of these items. Please update your selected items list. No silver charged.'));
      return;
    }
    $xp = \Drupal::service('vaxia_core.xp');
    foreach ($buying as $item) {
      $price = Shop::price($item);
      Silver::remove((int) $character->id(), -$price);
      $xp->log((int) $character->id(), 'item_shop', (string) $this->t('@xp silver removed by System to purchase @title by @user.', ['@xp' => $price, '@title' => $item->label(), '@user' => $this->currentUser()->getAccountName()]));
      $copy = Shop::copy($item, (int) $character->getOwnerId(), (int) $character->id(), FALSE, (string) $this->t('This item has been added and approved from the Item Shop.'));
      $messenger->addStatus($this->t(':title has been added to this character. :silver silver charged.', [':title' => $copy->label(), ':silver' => $price]));
    }
    $messenger->addStatus($this->t('Purchase complete.'));
  }

}
