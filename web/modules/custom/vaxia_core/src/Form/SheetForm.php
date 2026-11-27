<?php

namespace Drupal\vaxia_core\Form;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Character;

/**
 * The Sheet tab: XP totals, adding XP, and RXP / HXP transfers (D7
 * _character_sheet_xp_form + vaxia_form__character_sheet_xp_form_alter()).
 */
class SheetForm extends FormBase {

  public function getFormId() {
    return '_character_sheet_xp_form';
  }

  /**
   * Owners, and staff who may add XP (D7 _character_sheet_management_access()).
   */
  public static function access(NodeInterface $node, AccountInterface $account) {
    if (!Character::isSheet($node)) {
      return AccessResult::forbidden()->addCacheableDependency($node);
    }
    return AccessResult::allowedIf($node->getOwnerId() == $account->id() || self::mayAddXp($node, $account))
      ->cachePerUser()->addCacheableDependency($node);
  }

  public static function mayAddXp(NodeInterface $node, AccountInterface $account): bool {
    $own = $node->getOwnerId() == $account->id();
    return (!$own && $account->hasPermission('add xp to character sheet')) || ($own && $account->hasPermission('add xp to own character sheet'));
  }

  private function xp() {
    return \Drupal::service('vaxia_core.xp');
  }

  private function settings() {
    return $this->config('vaxia_core.settings');
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?NodeInterface $node = NULL) {
    $form_state->set('nid', (int) $node->id());
    $xp = $this->xp();
    $nid = (int) $node->id();
    $earned = $xp->get($nid, 'xp');
    $spent = $xp->get($nid, 'spent_xp');

    $form['node_info'] = ['#type' => 'fieldset', '#title' => $this->t('XP information')];
    // Not "xp": that's the Add XP select's value key (the form isn't #tree).
    $form['node_info']['xp_totals'] = [
      '#type' => 'item',
      '#title' => $this->t('XP totals'),
      'list' => ['#theme' => 'item_list', '#items' => [$this->t('You have %xp xp available to spend.', ['%xp' => $earned - $spent])]],
    ];
    $form['node_info']['xp_details'] = [
      '#type' => 'item',
      '#title' => $this->t('XP details'),
      'list' => ['#theme' => 'item_list', '#items' => [
        $this->t('Earned %historical historical xp.', ['%historical' => $earned]),
        $this->t('Spent %spent out of %xp xp.', ['%spent' => $spent, '%xp' => $earned]),
        $this->t('Banked %banked bxp.', ['%banked' => $xp->get($nid, 'bxp')]),
        $this->t('Roleplayed %roleplayed rxp.', ['%roleplayed' => $xp->get($nid, 'rxp')]),
        $this->t('Posted %posted kxp.', ['%posted' => $xp->get($nid, 'kxp')]),
      ]],
    ];
    $flavours = [];
    foreach ($xp->flavours($nid) as $type => $amount) {
      $flavours[] = $this->t('Earned %xp %type XP.', ['%xp' => (float) $amount, '%type' => $type]);
    }
    $form['node_info']['xp_types'] = [
      '#type' => 'item',
      '#title' => $this->t('XP types'),
      'list' => ['#theme' => 'item_list', '#items' => $flavours],
    ];

    $account = $this->currentUser();
    $own = $node->getOwnerId() == $account->id();
    // Adding XP: staff, on player characters only.
    if (self::mayAddXp($node, $account) && !Character::isStoryteller($node) && !Character::isNpc($node)) {
      $form['add_xp'] = ['#type' => 'details', '#title' => $this->t('Add XP'), '#open' => TRUE];
      $range = range(0, 500);
      $form['add_xp']['xp'] = [
        '#type' => 'select',
        '#title' => $this->t('Add XP'),
        '#options' => array_combine($range, $range),
        '#default_value' => 0,
        '#description' => $this->t('Add XP to this character sheet.'),
      ];
      $types = [];
      foreach (explode("\n", (string) $this->settings()->get('vaxia_xp_types')) as $type) {
        $type = trim($type);
        if ($type !== '' && !in_array($type, \Drupal\vaxia_core\Xp::MEANINGFUL, TRUE)) {
          $types[$type] = $type;
        }
      }
      $form['add_xp']['type'] = [
        '#type' => 'select',
        '#title' => $this->t('Type'),
        '#options' => $types + ['xp' => 'xp'],
        '#default_value' => 'xp',
      ];
      $form['add_xp']['submit_xp'] = [
        '#type' => 'submit',
        '#value' => $this->t('Add XP'),
        '#submit' => ['::submitAddXp'],
      ];
    }

    // RXP transfer: storyhost and NPC sheets pass their roleplayed XP on.
    $rxp = $xp->get($nid, 'rxp');
    if ($rxp > 0 && (Character::isStoryteller($node) || Character::isNpc($node))
      && (($own && $account->hasPermission('transfer rxp')) || $account->hasPermission('transfer others sheets'))) {
      $form['transfer_rxp'] = ['#type' => 'details', '#title' => $this->t('Transfer RXP only')];
      $form['transfer_rxp']['rxp_target'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Transfer to'),
        '#description' => $this->t('Transfer RXP to this character sheet.'),
        '#autocomplete_route_name' => 'vaxia_core.transfer_autocomplete',
      ];
      $amounts = range(0, (int) floor($rxp));
      $form['transfer_rxp']['rxp_amount'] = [
        '#type' => 'select',
        '#title' => $this->t('Transfer amount'),
        '#options' => array_combine($amounts, $amounts),
        '#description' => $this->t('Transfer this much RXP to the selected character sheet. This will not disable this character.'),
      ];
      $form['transfer_rxp']['rxp_submit'] = [
        '#type' => 'submit',
        '#value' => $this->t('Transfer character RXP'),
        '#submit' => ['::submitRxp'],
      ];
    }

    // HXP transfer: retire this sheet, moving most of its spent XP on.
    if ($this->settings()->get('vaxia_transfer_workflow') && ($own || $account->hasPermission('transfer others sheets'))) {
      [$new_xp, $old_xp, $max] = $this->hxpAmounts($nid);
      $form['transfer'] = ['#type' => 'details', '#title' => $this->t('Transfer all HXP')];
      $form['transfer']['alert'] = ['#type' => 'fieldset', '#title' => $this->t('NOTICE')];
      $form['transfer']['alert']['alert'] = [
        '#type' => 'item',
        '#markup' => $this->t('IF YOU TRANSFER YOUR CHARACTER SPENT XP: <br/> YOUR CHARACTER WILL NOT BE PLAYABLE AFTERWARDS.'),
      ];
      $form['transfer']['alert']['details'] = [
        '#type' => 'item',
        '#markup' => $this->t('When transferred, %percent of the XP (%new_xp out of %xp%max) will be placed on the selected character sheet. If the character is new, the XP will be immediately spendable, otherwise it will be added to the character bank. Banked and other XP for this sheet will not be transferred. Once transferred, this character will be flagged as inactive and will no longer be available to post. This cannot be undone without intervention by an administrator so be sure you mean to do this.', [
          '%percent' => ($this->settings()->get('vaxia_transfer_percentage') * 100) . '%',
          '%new_xp' => $new_xp,
          '%xp' => floor($old_xp),
          '%max' => $max,
        ]),
      ];
      $form['transfer']['target'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Transfer to'),
        '#description' => $this->t('Transfer HXP to this character sheet.'),
        '#autocomplete_route_name' => 'vaxia_core.transfer_autocomplete',
      ];
      $form['transfer']['confirm'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Confirm'),
        '#description' => $this->t('If you really mean to do this, please type "TRANSFER" into this textbox, in all caps.'),
      ];
      $form['transfer']['submit'] = [
        '#type' => 'submit',
        '#value' => $this->t('Transfer ALL character HXP'),
        '#validate' => ['::validateHxp'],
        '#submit' => ['::submitHxp'],
      ];
    }

    if ($account->hasPermission('view character sheet log')) {
      $form['log'] = [
        '#type' => 'item',
        '#title' => $this->t('Moderation log'),
        'list' => ['#theme' => 'item_list', '#items' => $xp->logLines($nid)],
      ];
    }
    return $form;
  }

  /**
   * [new xp, spent xp, " max N" or ""] for an HXP transfer.
   */
  private function hxpAmounts(int $nid): array {
    $spent = $this->xp()->get($nid, 'spent_xp');
    $new_xp = floor($spent * (float) $this->settings()->get('vaxia_transfer_percentage'));
    $max = '';
    if ($this->settings()->get('vaxia_transfer_cap_on')) {
      $cap = (float) $this->settings()->get('vaxia_transfer_cap');
      $max = ' max ' . $cap;
      $new_xp = min($new_xp, $cap);
    }
    return [$new_xp, $spent, $max];
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
  }


  public function submitAddXp(array &$form, FormStateInterface $form_state) {
    $amount = (float) $form_state->getValue('xp');
    if ($amount > 0) {
      $this->xp()->award($form_state->get('nid'), $amount, (string) $form_state->getValue('type'));
    }
  }

  public function submitRxp(array &$form, FormStateInterface $form_state) {
    $old = \Drupal\node\Entity\Node::load($form_state->get('nid'));
    $new = Character::fromTransferLabel((string) $form_state->getValue('rxp_target'));
    if (!$new || !$old || $old->id() == $new->id()) {
      $this->messenger()->addError($this->t('Unable to locate selected character. Transfer not completed.'));
      return;
    }
    $amount = (float) $form_state->getValue('rxp_amount');
    $args = ['@amount' => $amount, '@nid' => $old->id(), '@title' => $old->label(), '@nid2' => $new->id(), '@title2' => $new->label()];
    $xp = $this->xp();
    foreach (['rxp' => 'Transfering RXP (@amount) from #@nid @title to #@nid2 @title2', 'xp' => 'Transfering XP (@amount) from #@nid @title to #@nid2 @title2'] as $type => $text) {
      $message = (string) $this->t($text, $args);
      $this->logger('vaxia')->notice($message);
      $this->messenger()->addStatus($message);
      $xp->update((int) $old->id(), -$amount, $type);
      $xp->update((int) $new->id(), $amount, $type);
    }
    $user = $this->currentUser()->getAccountName();
    foreach (['SH-RXP', 'SH-XP'] as $type) {
      $xp->log((int) $new->id(), $type, (string) $this->t('@xp @type xp added to @title by @user.', ['@xp' => $amount, '@type' => $type, '@title' => $new->label(), '@user' => $user]));
    }
  }

  public function validateHxp(array &$form, FormStateInterface $form_state) {
    if ($form_state->getValue('confirm') !== 'TRANSFER') {
      $form_state->setErrorByName('confirm', $this->t('Transfer was not confirmed.'));
    }
  }

  public function submitHxp(array &$form, FormStateInterface $form_state) {
    $old = \Drupal\node\Entity\Node::load($form_state->get('nid'));
    $new = Character::fromTransferLabel((string) $form_state->getValue('target'));
    $state = (string) $this->settings()->get('vaxia_transfer_workflow');
    if (!$new || !$old || $old->id() == $new->id() || !$state) {
      $this->logger('vaxia')->notice('Unable to locate selected character. Transfer not completed.');
      $this->messenger()->addError($this->t('Unable to locate selected character. Transfer not completed.'));
      return;
    }
    $xp = $this->xp();
    [$new_xp, $old_xp, $max] = $this->hxpAmounts((int) $old->id());
    $message = $this->t('Transferring %new_xp XP out of %xp%max from #@nid @title to #@nid2 @title2.', [
      '%new_xp' => $new_xp, '%xp' => $old_xp, '%max' => $max,
      '@nid' => $old->id(), '@title' => $old->label(), '@nid2' => $new->id(), '@title2' => $new->label(),
    ]);
    $this->logger('vaxia')->notice($message);
    $this->messenger()->addStatus($message);
    $xp->log((int) $old->id(), 'transfer', strip_tags((string) $message));
    $xp->log((int) $new->id(), 'transfer', strip_tags((string) $message));
    // A new sheet (nothing spent) can spend it now; otherwise it's banked.
    $xp->update((int) $new->id(), $new_xp, $xp->get((int) $new->id(), 'spent_xp') > 0 ? 'bxp' : 'xp');
    $xp->log((int) $new->id(), 'transfer', (string) $this->t('@xp transfer xp added to @title by system.', ['@xp' => $new_xp, '@title' => $new->label()]));
    $xp->deleteAll((int) $old->id());
    $xp->log((int) $old->id(), 'transfer', 'Cleared records for XP and flavor XP. DO NOT REACTIVATE CHARACTER.');
    Character::forceState($old, $state, (string) $this->t('Transferred all XP to a new character.'));
  }

}
