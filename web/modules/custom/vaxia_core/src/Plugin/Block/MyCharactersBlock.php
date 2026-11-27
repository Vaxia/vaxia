<?php

namespace Drupal\vaxia_core\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Link;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\vaxia_core\ViewBlockBase;

/**
 * Jump menu to the current user's approved characters, and to approved NPCs
 * for those who may use NPCs (D7 view vaxia_character_sheets, display
 * my_chars_block), with a link to make a character (or, for visitors, to
 * sign up) and links to the item shop of each of the player's characters.
 */
#[Block(id: 'vaxia_character_sheets-my_chars_block', admin_label: new TranslatableMarkup('My characters'))]
class MyCharactersBlock extends ViewBlockBase {

  const VIEW = 'vaxia_character_sheets';
  const DISPLAY = 'my_chars_block';
  const TITLE = 'My Characters';

  protected function blockAccess(AccountInterface $account) {
    // The view's own roles, without anonymous (user, 2026-11-26).
    return $this->roleAccess($account, ['administrator', 'new_account', 'player']);
  }

  public function build() {
    $account = \Drupal::currentUser();
    $cache = ['tags' => ['node_list'], 'contexts' => ['user', 'user.permissions']];
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    // Own approved sheets, or approved NPCs (D7: two OR'd filter groups, then
    // a PHP filter dropping NPCs the user neither owns nor may use).
    $query = $storage->getQuery()->accessCheck(TRUE)
      ->condition('type', 'character_sheet')
      ->condition('status', 1)
      ->condition('field_character_evaluation', 'character_evaluation_approved')
      ->sort('field_is_npc')
      ->sort('title');
    if ($account->hasPermission('use NPCs')) {
      $query->condition($query->orConditionGroup()->condition('uid', $account->id())->condition('field_is_npc', 1));
    }
    else {
      $query->condition('uid', $account->id());
    }
    $options = [];
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      // D7 _vaxia_get_character_ref_types().
      $npc = (bool) $node->get('field_is_npc')->value;
      $group = $npc ? ($node->get('field_is_generic_npc')->value ? 'Generic NPCs' : 'NPCs') : 'Characters';
      $options[$group][$node->id()] = $node->label();
    }
    // A way to make a character: the sheet form for those who may create
    // one.
    $footer = NULL;
    if ($account->isAuthenticated()) {
      $create = \Drupal::entityTypeManager()->getAccessControlHandler('node')->createAccess('character_sheet', $account, [], TRUE);
      $cache['tags'][] = 'config:node.type.character_sheet';
      if ($create->isAllowed()) {
        $footer = Link::createFromRoute($this->t('Create a new character'), 'node.add', ['node_type' => 'character_sheet'])->toRenderable();
      }
    }
    // The item shop of each of the player's own approved characters (the
    // shop is per character).
    if (!empty($options['Characters']) && \Drupal::moduleHandler()->moduleExists('vaxia_items') && $account->hasPermission('use item shop')) {
      $shops = [];
      foreach ($options['Characters'] as $nid => $title) {
        $shops[] = Link::createFromRoute(count($options['Characters']) > 1 ? $title : $this->t('Item shop for @name', ['@name' => $title]), 'vaxia_items.shop', ['node' => $nid])->toRenderable();
      }
      $footer = array_filter([
        'create' => $footer,
        'shop' => count($shops) > 1
          ? ['#theme' => 'item_list', '#title' => $this->t('Item shop'), '#items' => $shops, '#attributes' => ['class' => ['vaxia-item-shop-links']]]
          : ['#type' => 'container', '#attributes' => ['class' => ['vaxia-item-shop-links']], 'link' => $shops[0]],
      ]);
    }
    if (!$options) {
      return $this->view(['empty' => $this->t('No characters available.'), 'footer' => $footer], $cache);
    }
    return $this->view([
      'header' => $this->t('Select a character to view.'),
      'content' => \Drupal::formBuilder()->getForm('Drupal\vaxia_core\Form\CharacterJumpForm', $options),
      'footer' => $footer,
    ], $cache);
  }

}
