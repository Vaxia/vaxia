<?php
/**
 * @file
 * vaxia.character_sheet.inc - Provides Vaxian character sheets for Vaxia.
 */

/* 
 * Implements hook_character_sheet_register_systems().
 * Given format of array of system information organized as follows:
  return array(
    'system_name' => array(
    'name' => 'Vaxia character sheet', // Human name.
    'description' => 'Vaxia system featuring a XP tracker for attribute and skill fields.',
    'admin_callback' => '_vaxia_admin_settings', // Admin function.
    'xp' => array(
      'types' => '', // Registered types of XP.
      'add_callback' => '_vaxia_add_xp',
    ),
  );
 */
function vaxia_character_sheet_register_systems() {
  $xp_types = array('xp', 'econ');
  foreach ($xp_types as $key => $type) {
    $xp_types[$key] = trim($type);
  }
  $system['vaxia'] = array(
    'name' => 'Vaxia character sheet',
    'description' => 'Vaxia system featuring a XP tracker for attribute and skill fields.',
    'admin_callback' => '_vaxia_admin_settings',
    'xp' => array(
      'types' => $xp_types,
      'add_callback' => '_vaxia_add_xp',
    ),
  );
  return $system;
}

/**
 * Implements hook_character_sheet_field_info().
 * Given format of array of field information organized as follows:
  return array(
    'field_name' => array(
      'permissions' => array( // Settings for permission driven changes by character sheet.
        'owner' => LIMIT_SHEET, // For owner.
      ),
      permissions_callback => '', // Function to make permission driven changes to the field instead of character sheet's system.
      validation_callback => '', // Function to validate this field, instead of character sheet's system.
      submit_callback => '', // Function to append to submission, to handle this field instead of character sheet's system.
      'costs' => array(
        'xp_type' => 'xp', // Type of xp to spend for cost.
         // Type of xp to track spending.
        'cost' => variable_get('vaxia_skill_cost', 0.75) * 0.1, // Cost to increase.
        'step' => 1, // How many steps per value. Default 1.
         value_callback => '', // Function to handle value calculation for current value, instead of character sheet's own.
         costs_callback => '', // Function to handle cost per next step(s), instead of character sheet's own.
      ),
    ),
  );
 */
function vaxia_character_sheet_field_info() {
  return array(
    'title' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
        'other' => DISABLE_SHEET,
      ),
    ),
    'field_npc' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    //field_background
    //field_personality
    //field_occupation
    //field_description
    //field_artwork
    'field_realm' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    'field_sex' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    'field_race' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    //field_age_real
    //field_age_app
    'field_life' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
        'step' => 1,
      ),
    ),
    'field_endurance' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
        'step' => 1,
      ),
    ),
    'field_strength' => array(
       'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
        'step' => 1,
      ),
    ),
    'field_dexterity' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
        'step' => 1,
      ),
    ),
    'field_intelligence' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
        'step' => 1,
      ),
    ),
    'field_spirituality' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
        'step' => 1,
      ),
    ),
    'field_charisma' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
        'step' => 1,
      ),
    ),
    //field_health
    //field_constitution
    //field_agility
    //field_reflexes
    //field_awareness
    //field_presence
    //field_appearance
    'field_skill_value' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',        
        'cost' => variable_get('vaxia_skill_cost', 0.75) * 0.1,
        'step' => 1,
      ),
    ),
    'field_items' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    //field_property
    'field_npcs' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    'field_sh_notes' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    //field_player_notes
  );
}

/*
 * Callback for admin settings page for character sheets.
 */
function _vaxia_admin_settings() {
  $form['settings'] = array(
    '#title' => 'Settings',
    '#type' => 'fieldset',
  );
  $form['settings']['vaxia_starting_points'] = array(
    '#title' => 'Starting points',
    '#description' => 'The starting points for a new character.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_starting_points', 300),
  );
  $form['settings']['vaxia_starting_econ'] = array(
    '#title' => 'Starting Economy level',
    '#description' => 'The starting Economy level for a new character.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_starting_econ', 25),
  );
  $form['settings']['vaxia_kxp_ratio'] = array(
    '#title' => 'Posted XP ratio',
    '#description' => 'The ratio of kxp to xp per comment word count.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_kxp_ratio', 70),
  );
  $form['settings']['vaxia_xp_types'] = array(
    '#title' => 'XP types',
    '#description' => t('Available XP sub-types. The following types used by the system and will ' .
      'be ignored if added here: @types',
      array('@types' => implode(', ' , _vaxia_meaningful_xp_types()))),
    '#type' => 'textarea',
    '#default_value' => variable_get('vaxia_xp_types',
      "good\nevil\nsmart\nclever\nwarrior\nmage\nrogue"),
  );
  $form['point_cost'] = array(
    '#title' => 'Point costs',
    '#type' => 'fieldset',
  );
  $form['point_cost']['vaxia_attribute_cost'] = array(
    '#title' => 'Attribute cost multiplier',
    '#description' => 'Attribute cost is calculated as (current/10) * multiplier.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_attribute_cost', 1),
  );
  $form['point_cost']['vaxia_skill_cost'] = array(
    '#title' => 'Skill cost multiplier',
    '#description' => 'Skill cost is calculated as (current/10) * multiplier.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_skill_cost', 0.75),
  );
  $form['point_cost']['vaxia_transfer_percentage'] = array(
    '#title' => 'Percent of XP',
    '#description' => 'Percent of XP when transferred that goes to XP, the rest is banked.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_transfer_percentage', 0.75),
  );
  $form['workflows'] = array(
    '#title' => 'Workflow states',
    '#type' => 'fieldset',
  );
  $workflow_options = array();
  $workflows = workflow_get_workflows();
  foreach ($workflows as $workflow) {
    $states = workflow_get_workflow_states_by_wid($workflow->wid);
    foreach ($states as $state) {
      $workflow_options[$state->sid] = $workflow->name . ':' . $state->state;
    }
  }
  $form['workflows']['vaxia_transfer_workflow'] = array(
    '#title' => 'Transfer workflow',
    '#description' => 'Workflow state characters who are transferred from end up in.',
    '#type' => 'select',
    '#options' => $workflow_options,
    '#default_value' => variable_get('vaxia_transfer_workflow', ''),
  );
  $form['workflows']['vaxia_draft_workflow'] = array(
    '#title' => 'Draft workflow',
    '#description' => 'Workflow state for characters in creation.',
    '#type' => 'select',
    '#options' => $workflow_options,
    '#default_value' => variable_get('vaxia_draft_workflow', ''),
  );
  return $form;
}

/**
 * Implements hook_form_FORMID_alter().
 */
function vaxia_form__character_sheet_xp_form_alter(&$form, $form_state) {
  // Load the node.
  $node = node_load($form['add_xp']['nid']['#value']);

  // Add additional options for xp.
  $xp_types = array();
  foreach(explode("\n", variable_get('vaxia_xp_types', "good\nevil\nsmart\nclever\nwarrior\nmage\nrogue")) as $type) {
    $type = trim($type);
    $xp_types[$type] = $type;
  }
  $form['add_xp']['type']['#options'] = array_merge($xp_types, $form['add_xp']['type']['#options']);

  // Get specific info for vaxia to display in markup.
  $this_xp = _character_sheet_get_xp($node->nid, 'spent_xp');
  $this_xp = isset($this_xp) ? $this_xp : 0;
  $xps[] = t('Spent %spent out of %xp xp.', array('%spent' => $this_xp, '%xp' => $node->xp['xp']));
  $this_xp = _character_sheet_get_xp($node->nid, 'bxp');
  $this_xp = isset($this_xp) ? $this_xp : 0;
  $xps[] = t('Banked %banked bxp.', array('%banked' => $this_xp));
  $this_xp = _character_sheet_get_xp($node->nid, 'rxp');
  $this_xp = isset($this_xp) ? $this_xp : 0;
  $xps[] = t('Roleplayed %roleplayed rxp.', array('%roleplayed' => $this_xp));
  $this_xp = _character_sheet_get_xp($node->nid, 'kxp');
  $this_xp = isset($this_xp) ? $this_xp : 0;
  $xps[] = t('Posted %posted kxp.', array('%posted' => $this_xp));
  $form['node_info']['xp_details'] = array(
    '#type' => 'item',
    '#title' => t('XP details'),
    '#markup' => theme('item_list', array('items' => $xps)),
  );

  // And add the transfer options.
  $setting = variable_get('vaxia_transfer_workflow', '');
  if (!empty($setting)) { // TODO check permissions to transfer.
    $form['transfer'] = array(
      '#type' => 'fieldset',
      '#title' => 'Transfer XP',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['transfer']['alert'] = array(
      '#type' => 'fieldset',
      '#title' => t('NOTICE'),
    );
    $form['transfer']['alert']['alert'] = array(
      '#type' => 'item',
      '#markup' => t('IF YOU TRANSFER YOUR CHARACTER XP: <br/> YOUR CHARACTER WILL NOT BE PLAYABLE AFTERWARDS.'),
    );
    $form['transfer']['alert']['details'] = array(
      '#type' => 'item',
      '#markup' => t('When transferred, %percent percent of the XP will be placed on the selected character sheet immediately and the rest will be '.
        'added to the character bank. Banked and other XP for this sheet will not be transferred. At that time, the character will be flagged '.
         'as inactive and will no longer be available to post. This cannot be undone without intervention by an administrator so be sure you '.
         'mean to do this.', array('%percent' => variable_get('vaxia_transfer_percentage', 0.75))),
    );
    $available_characters = array();
    $view = views_get_view('character_sheet_moderation');
    if (is_object($view)) {
      $view->set_display('character_sheets_refs');
      $view->pre_execute();
      $view->execute();
      $characters = $view->result;
    }
    else {
      $characters = array();
    }
    foreach ($characters as $character) {
      if ($character->nid != $node->nid) {
        $available_characters[$character->nid] = $character->node_title;
      }
    }
    $form['transfer']['confirm']['character'] = array(
      '#type' => 'hidden',
      '#value' => $node->nid,
    );
    $form['transfer']['confirm']['target'] = array(
      '#type' => 'select',
      '#title' => t('Transfer to'),
      '#options' => $available_characters,
      '#description' => t('Transfer XP to this character sheet.'),
    );
    $form['transfer']['confirm']['confirm'] = array(
      '#type' => 'textfield',
      '#title' => t('Confirm'),
      '#description' => t('If you really mean to do this, please type "TRANSFER" '.
        'into this textbox, in all caps.'),
    );
    $form['transfer']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Transfer character XP'),
      '#validate' => array('_vaxia_transfer_xp_transfer_validate'),
      '#submit' => array('_vaxia_transfer_xp_transfer_submit'),
    );
  }
  return $form;
}

/**
 * Validate callback for _vaxia_transfer_xp_form().
 */
function _vaxia_transfer_xp_transfer_validate(&$form, &$form_state) {
  if ($form_state['values']['confirm'] != 'TRANSFER') {
    form_set_error('confirm', t('Transfer was not confirmed.'));
  }
}

/**
 * Submit callback for _vaxia_transfer_xp_form().
 */
function _vaxia_transfer_xp_transfer_submit(&$form, &$form_state) {
  $old_character = node_load($form_state['values']['character']);
  $new_character = node_load($form_state['values']['target']);
  $workflow = variable_get('vaxia_transfer_workflow', '');
  if ($old_character && $new_character && !empty($workflow) && $old_character->nid != $new_character->nid) {
    // Grab XP for this character.
    $xp = $old_character->vaxia_unspent_xp + $old_character->vaxia_spent_xp;
    // Update the XP recorded for this character.
    _character_sheet_delete_xp($old_character->nid);
    _character_sheet_delete_log($old_character->nid);
    // And update the original character status.
    workflow_execute_transition($old_character, $workflow, t('Transferring XP to a new character'), TRUE);
    // Now add XP to the new character.
    $new_xp = floor($xp * variable_get('vaxia_transfer_percentage', 0.75));
    $bank_xp = floor($xp - $new_xp); 
    _character_sheet_update_xp($new_character->nid, $new_xp, 'xp');
    _character_sheet_update_xp($new_character->nid, $bank_xp, 'bxp');
  }
}

/**
 * Implements hook_character_sheet_validate_sheet().
 */
function vaxia_character_sheet_validate_sheet($node, $old_node, $form, $form_state) {
  // Run this only for draft nodes, to cap out at 300 per field.
  // Check if the sheet has been finalized.
  if (isset($node->workflow)
    && ($node->workflow == variable_get('vaxia_draft_workflow', '') )) {
    // Total the value of points against starting points flat.
    $available_xp = variable_get('vaxia_starting_points', 300);
    $fields_info = _character_sheet_get_field_info($node);
    $spent_xp = array();
    foreach ($fields_info as $field_key => $field_info) {
      if (isset($field_info['costs']['xp_type'])) {
        // Calculate cost difference.
        $old_value = _vaxia_get_field_value_flat($old_node, $field_key, $fields_info);
        $new_value = _vaxia_get_field_value_flat($node, $field_key, $fields_info);
        // Add the debt to the ongoing total for this type.
        $xp_type = $field_info['costs']['xp_type'];
        $cost = $new_value - $old_value;
        $spent_xp[$xp_type] = isset($spent_xp[$xp_type]) ? $spent_xp[$xp_type] + $cost : $cost;
        if ($spent_xp[$xp_type] > $available_xp) {
          return $error = array('field' => '', 'message' => 
            t('You have spent over %spent_xp xp out of %available_xp available xp. Please revise your sheet.',
              array('%spent_xp' => $spent_xp[$xp_type], '%available_xp' => $available_xp)));
        }
      }
    }
  }
  return FALSE;
}

/**
 * Helper function, determine how many points a sheet has spent.
 * ALERT: RECURSIVE. BE CAREFUL HERE.
 * Based on _character_sheet_get_field_value().
 */
function _vaxia_get_field_value_flat($node, $field_key, $field_info) {
  // Given a node, find the key and patch to the information for that key.
  $total = 0;
  $language = isset($node->language) ? $node->language : 'und';
  $walkable_node = (array) $node;
  if (isset($walkable_node[$field_key]) && is_array($walkable_node[$field_key][$language])) {
    // Found it on the top level.
    // Grab all deltas and total them up.
    foreach ($walkable_node[$field_key][$language] as $delta => $delta_field) {
      if (isset($delta_field['value']) && is_numeric($delta_field['value'])) {
        // Otherwise, let's build up the value point by point according to the given cost.
        $total += $delta_field['value']; // CHANGE: FLAT VALUE TOTAL HERE.
      }
    }
  }
  else {
    // We didn't find it on the top level. So it may be in a child somewhere.
    // Go looking.
    foreach (element_children($walkable_node) as $this_child) {
      $child = $walkable_node[$this_child];
      if (isset($child[$language]) && is_array($child[$language])) {
        foreach ($child[$language] as $delta => $delta_field) {
          $sub_node = '';
          if (isset($delta_field['nid'])) {
            // We have a node reference subnode.
            $sub_node = node_load($delta_field['nid']);
          }
          else {
            $child_field_info = field_info_field($this_child);
            if ($child_field_info['type'] == 'field_collection') {
              if (isset($delta_field['value'])) {
                // Checking on an already saved node (probably old_node).
                $sub_node = field_collection_item_load($delta_field['value']);
              }
              elseif(isset($delta_field['entity']->item_id)) {
                // New node, has a differnt format. Not saved, so we can't just load it.
                // We have to grab it from the form's submitted data.
                $sub_node = $delta_field['entity'];
              }
            }
          }
          // If we found a subnode at this this child, check it.
          if (!empty($sub_node)) {
             $total += _vaxia_get_field_value_flat($sub_node, $field_key, $field_info);
          }
        }
      }
    }
  }
  return $total;
}

/**
 * Helper function, list meaningful types for Vaxia.
 */
function _vaxia_meaningful_xp_types() {
  return array('xp', 'bxp', 'rxp', 'kxp', 'econ');
}

/**
 * XP callback for adding XP to a node. Vaxia's wierd XP complexity.
 */
function _vaxia_add_xp($nid, $xp, $type) {
  $node = node_load($nid);
  $meaningful_types = _vaxia_meaningful_xp_types();
  if (!in_array($type, $meaningful_types)) {
    // Log it.
    $message = t('%xp %type XP added to %title.', array('%xp' => $xp, '%type' => $type, '%title' => $node->title));
    _character_sheet_update_log($nid, 'xp', $message, $variables = array());
    $type = 'xp';
  }
  switch ($type) {
    case 'kxp':
      // Add wordcount xp.
      _character_sheet_update_xp($nid, $xp, 'kxp');
      $current_kxp = _character_sheet_get_xp($nid, 'kxp');
      // If we have enough to convert to other xp, do it.
      if ($current_kxp > variable_get('vaxia_kxp_ratio', 70)) {
        // Remove from kxp, add 1 to rxp.
        _character_sheet_update_xp($nid, variable_get('vaxia_kxp_ratio', 70) * -1, 'kxp');
        _character_sheet_update_xp($nid, 1, 'rxp');
        // If we have banked xp, remove one from bxp and add spendable xp.
        $current_bxp = _character_sheet_get_xp($nid, 'bxp');
        if ($current_bpx > 0 ) {
          _character_sheet_update_xp($nid, -1, 'bxp');
          _character_sheet_update_xp($nid, 1, 'xp');
        }
      }
    break;
    case 'bxp':
      // If bxp is added directly, just add it to the bxp score.
      _character_sheet_update_xp($nid, $xp, 'bxp');
    break;
    case 'rxp':
      // If rxp is added directly, just add it to the rxp score.
      _character_sheet_update_xp($nid, $xp, 'rxp');
    break;
    case 'xp':
    default:
      $message = t('%xp %type XP added to %title.', array('%xp' => $xp, '%type' => $type, '%title' => $node->title));
      _character_sheet_update_log($nid, 'xp', $message, $variables = array());
      // Add per, b/c of the way the calculations work for adding.
      $current_rxp = _character_sheet_get_xp($nid, 'rxp');
      $bxp_to_add = 0;
      $xp_to_add = 0;
      $rxp_to_charge = 0;
      while ($xp > 0 ) {
        if ($current_rxp > 0 ) {
          $rxp_to_charge--;
          $xp_to_add++;
        }
        else {
          $bxp_to_add++;
        }
        $xp--;
      }
      _character_sheet_update_xp($nid, $rxp_to_charge, 'rxp');
      _character_sheet_update_xp($nid, $xp_to_add, 'xp');
      _character_sheet_update_xp($nid, $bxp_to_add, 'bxp');
    break;
    case 'econ':
      $message = t('%xp %type XP added to %title.', array('%xp' => $xp, '%type' => $type, '%title' => $node->title));
      _character_sheet_update_log($nid, 'xp', $message, $variables = array());
      // Economy is just added directly.
      _character_sheet_update_xp($nid, $xp, 'econ');
    break;
  }
}
