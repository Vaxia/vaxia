<?php
/**
   * @file
   * A Views' field handler for the player item shop.
   */
class item_shop_field_handler_add_item extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Render a Views form item placeholder.
    // This causes Views to wrap the View in a form.
    // Render a Views form item placeholder.
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Add to and alter the form created by Views.
   */
  function views_form(&$form, &$form_state) {
    // Create a container for our replacements
    $form[$this->options['id']] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    // Iterate over the result and add our replacement fields to the form.
    foreach($this->view->result as $row_index => $row) {
      // Add a checkbox field to the form.
      $form[$this->options['id']][$row_index] = array(
        '#type' => 'checkbox',
        '#default_value' => 0,
        '#return_value' => 1,
      );
    }
  }

  /**
   * Form submit method.
   */
  function views_form_submit($form, &$form_state) {
    // Display message.
    drupal_set_message(t('Processing your purchase...'));
    // Determine which nodes we need to update.
    $buy = array();
    // Iterate over the view result.
    foreach($this->view->result as $row_index => $row) {
      // Grab the correspondingly submitted form value.
      $value = $form_state['values'][$this->options['id']][$row_index];
      // If the submitted value is true, we're buying this one.
      if ($value) {
        $buy[] = $row->{$this->aliases['nid']};
      }
    }
    // Get node vlaues form path.
    $node = node_load(arg(1));
    $lang = $node->language;
    // Get char from argument.
    $char = isset($node->nid) ? $node->nid : '';
    // Check values are ok.
    if (empty($char)) {
      drupal_set_message(t('No character found for this purchase.'));
      return;
    }

    // Get character's owner.
    $user = user_load($node->uid);
    // Check values are ok.
    if (empty($user)) {
      drupal_set_message(t('No player found for this purchase.'));
      return;
    }

    // Get realm from argument.
    $realm = isset($node->field_realm[$lang][0]['value']) ? $node->field_realm[$lang][0]['value'] : 'Vaxia';
    // Check values are ok.
    if (empty($realm)) {
      drupal_set_message(t('No realm found for this purchase.'));
      return;
    }

    // Get available silver from character.
    $silver = _character_sheet_get_xp($node->nid, 'silver');
    $silver_spent = _character_sheet_get_xp($node->nid, 'spent_silver');
    $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
    $available_silver = $silver - ($silver_spent + $silver_invested);
    // Available economy.
    $economy = isset($node->field_economy[$lang][0]['value']) ? $node->field_economy[$lang][0]['value'] : 0;
    $worth = _vaxia_calculate_worth($economy);
    // Grand total available.
    $total_worth = $available_silver + $worth;
    $silver = isset($total_worth) ? $total_worth : 0;
    // Check values are ok.
    if (empty($silver)) {
      drupal_set_message(t('No silver found for this purchase.'));
      return;
    }

    // Grab the nodes we need to update and update them.
    $nodes = node_load_multiple($buy);
    // Purchasing nodes.
    $total_cost = 0;
    foreach($nodes as $nid => $node) {
      $lang = $node->language;
      $this_value = trim($node->field_value[$lang][0]['value']);
      $this_value = str_replace(',', '', $this_value);
      $this_value = str_replace('.', '', $this_value);
      $total_cost += $this_value;
    }
    if ($silver < $total_cost) {
      $error = 1;
      drupal_set_message(t('You do not have enough silver for all of these items. ' .
        'Please update your selected items list. No silver charged.'));
      return;
    }

    // Still good? Add items.
    foreach($nodes as $nid => $node) {
      $lang = $node->language;
      if (!empty($char)) {
        $this_realm = $node->field_realm[$lang][0]['value'];
        $this_value = $node->field_value[$lang][0]['value'];
        $this_value = str_replace(',', '', $this_value);
        $this_value = str_replace('.', '', $this_value);
        $error = 0;
        if ($realm != $this_realm) {
          $error = 1;
          drupal_set_message(t('This item is not the same realm as the character. ' .
            ':title has NOT been added to this character. No silver charged.',
            array(':title' => $node->title)));
        }
        if (!is_numeric($this_value) ) {
          $error = 1;
          drupal_set_message(t('This item does not have a material cost. ' .
            ':title has NOT been added to this character. No silver charged.',
            array(':title' => $node->title)));
        }
        if ($silver < $this_value) {
          $error = 1;
          drupal_set_message(t('You do not have enough silver for this item. ' .
            ':title has NOT been added to this character. No silver charged.',
            array(':title' => $node->title)));
        }

        if(!$error) {
          // Charge silver.
          _add_silver_submit_form_remove_silver($char, -1 * $this_value);
          $message = t('@xp silver removed by System to purchase @title by @user.',
            array('@xp' => -1 * $this_value, '@title' => $node->title, '@user' => $user->name));
          _character_sheet_update_log($nid, 'silver', $message, $variables = array());
          // Create new node for the player, and character.
          $new_node = clone $node;
          unset($new_node->nid);
          unset($new_node->vid);
          $new_node->uid = $user->uid;
          $new_node->field_in_shop[$lang][0]['value'] = 0;
          $new_node->field_character_owner[$lang][0]['nid'] = $char;
          node_save($new_node);
          // Flush Entity Cache record.
          if (module_exists('entitycache')) {
            cache_clear_all('*', 'cache_entity_node', TRUE);
          }
          // Display message.
          drupal_set_message(t(':title has been added to this character. :silver silver charged.',
            array(':title' => $new_node->title, ':silver' => $this_value)));
          drupal_set_message(t('It can take a few minutes for our cache systems to catch up with this change, ' .
          	'do NOT purchase a copy of the item if it doesn\'t immediately appear on the sheet. (In general, ' .
          	'you only need to purchase one of any item in the shop. You can always come back and pick up another one later.'));
          // Flush Entity Cache record.
          if (module_exists('entitycache')) {
            cache_clear_all('*', 'cache_entity_node', TRUE);
          }
        }
      }
    }
    drupal_set_message(t('Purchase complete.'));
  }

}