<?php
/**
 * @file
 * vaxia.character_sheet.inc - Provides Vaxian character sheets for Vaxia.
 */

/* 
 * Implements hook_character_sheet_register_systems(). 
 */
function vaxia_character_sheet_register_systems() {
  $system['vaxia'] = array(
    'name' => 'Vaxia character sheet',
    'description' => 'Vaxia system featuring a XP tracker for attribute and skill fields.',
    'admin_callback' => '_vaxia_admin_settings',
  );
  return $system;
}

/**
 * Implements hook_character_sheet_field_info().
 */
function vaxia_character_sheet_field_info() {
  return array(
    'field_skill_value' => array(
      'cost' => variable_get('vaxia_skill_cost', 0.75) * 0.1,
    ),
    'field_life' => array(
      'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
    ),
    'field_endurance' => array(
      'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
    ),
    'field_strength' => array(
      'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
    ),
    'field_dexterity' => array(
      'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
    ),
    'field_intelligence' => array(
      'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
    ),
    'field_spirituality' => array(
      'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
    ),
    'field_charisma' => array(
      'cost' => variable_get('vaxia_attribute_cost', 1) * 0.1,
    ),
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
    '#description' => t('Available XP sub-types. The following types used by the system and will be ignored if added here: @types',
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
  $workflow_options = array();
  $workflows = workflow_get_workflows();
  foreach ($workflows as $workflow) {
    $states = workflow_get_workflow_states_by_wid($workflow->wid);
    foreach ($states as $state) {
      $workflow_options[$state->sid] = $workflow->name . ':' . $state->state;
    }
  }
  $form['point_cost']['vaxia_transfer_workflow'] = array(
    '#title' => 'Transfer workflow',
    '#description' => 'Workflow state characters who are transferred from end up in.',
    '#type' => 'select',
    '#options' => $workflow_options,
    '#default_value' => variable_get('vaxia_transfer_workflow', ''),
  );
  $form['point_cost']['vaxia_draft_workflow'] = array(
    '#title' => 'Draft workflow',
    '#description' => 'Workflow state for characters in creation.',
    '#type' => 'select',
    '#options' => $workflow_options,
    '#default_value' => variable_get('vaxia_draft_workflow', ''),
  );
  $form['point_cost']['vaxia_eval_workflow'] = array(
    '#title' => 'Evaluation workflow',
    '#description' => 'Workflow state for characters in evaluation.',
    '#type' => 'select',
    '#options' => $workflow_options,
    '#default_value' => variable_get('vaxia_eval_workflow', ''),
  );
  $form['point_cost']['vaxia_live_workflow'] = array(
    '#title' => 'Playable workflow',
    '#description' => 'Workflow state for playable characters. Character sheets in this state may be improved with XP.',
    '#type' => 'select',
    '#options' => $workflow_options,
    '#default_value' => variable_get('vaxia_live_workflow', ''),
  );
  return $form;
}

/**
 * Menu callback, manage character sheet XP.
 */
function _vaxia_manage_xp($node) {
  return drupal_get_form('_vaxia_manage_xp_form', $node);
}

/**
 * Form callback for XP form.
 */
function _vaxia_manage_xp_form($form, &$form_state, $node) {
  $form['node_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('XP information'),
  );
  $total_xp = $node->vaxia_spent_xp + $node->vaxia_unspent_xp;
  $form['node_info']['xp'] = array(
    '#type' => 'item',
    '#title' => t('Total XP'),
    '#markup' => $total_xp,
  );
  $xps[] = t('Spent %spent out of %xp xp.',
    array('%spent' =>$node->vaxia_spent_xp, '%xp' => $node->vaxia_xp));
  $xps[] = t('Banked %banked bxp.', array('%banked' =>$node->vaxia_bxp));
  $xps[] = t('Roleplayed %roleplayed rxp.', array('%roleplayed' => $node->vaxia_rxp));
  $xps[] = t('Posted %posted kxp.', array('%posted' => $node->vaxia_kxp));
  $form['node_info']['xp_details'] = array(
    '#type' => 'item',
    '#title' => t('XP details'),
    '#markup' => theme('item_list', array('items' => $xps)),
  );
  $form['node_info']['xp_log'] = array(
    '#type' => 'fieldset',
    '#title' => t('XP history'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['node_info']['xp_log']['log'] = array(
    '#type' => 'item',
    '#markup' => !empty($node->vaxia_type_xp) ? $node->vaxia_type_xp : t('No XP awarded.'),
  );
  if (TRUE) { // TODO check permissions to add XP.
    $form['add_xp'] = array(
      '#type' => 'fieldset',
      '#title' => 'Add XP',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['add_xp']['xp'] = array(
      '#type' => 'textfield',
      '#title' => t('Add XP'),
      '#default_value' => 0,
      '#description' => t('Add XP to this character sheet.'),
    );
    $types = variable_get('vaxia_xp_types', "good\nevil\nsmart\nclever\nwarrior\nmage\nrogue");
    $types = explode("\n", $types);
    $types = array_diff($types, _vaxia_meaningful_xp_types());
    $options = array_combine($types, $types);
    $options = array_merge(array('generic' => '- generic -'), $options);
    $form['add_xp']['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#default_value' => 'generic',
      '#options' => $options,
      '#description' => t('Add XP to this character sheet.'),
    );
    $form['add_xp']['nid'] = array(
      '#type' => 'hidden',
      '#value' => $node->nid,
    );
    $form['add_xp']['submit_xp'] = array(
      '#type' => 'submit',
      '#value' => t('Add XP'),
      '#submit' => array('_vaxia_add_xp_form_submit'),
    );
  }
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
      '#markup' => t('IF YOU TRANSFER YOUR CHARACTER XP: YOUR CHARACTER WILL NOT BE PLAYABLE AFTERWARDS.'),
    );
    $form['transfer']['alert']['details'] = array(
      '#type' => 'item',
      '#markup' => t('When transferred, %percent percent of the XP will be placed on the selected character sheet immediately and the rest will be '.
        'added to the character bank. Banked and other XP for this sheet will not be trasnferred. At that time, the character will be flagged '.
         'as inactive and will not longer be available to post. This cannot be undone without intervention by an administrator, so be sure you '.
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
      '#validate' => array('_vaxia_transfer_xp_form_validate'),
      '#submit' => array('_vaxia_transfer_xp_form_submit'),
    );
  }
  return $form;
}

/*
 * Submit callback submission for _vaxia_add_xp_form.
 */
function _vaxia_add_xp_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $node = node_load($values['nid']);
  global $user;
  if (!empty($node) && user_access('add xp to character sheet')) {
    $old_xp = _vaxia_get_xp($node->nid);
    // Add xp from Session.
    _vaxia_add_xp($node->nid, $values['xp'], $values['type']);
  }
}

/**
 * Validate callback for _vaxia_transfer_xp_form().
 */
function _vaxia_transfer_xp_form_validate(&$form, &$form_state) {
  if ($form_state['values']['confirm'] != 'TRANSFER') {
    form_set_error('confirm', t('Transfer was not confirmed.'));
  }
}

/**
 * Submit callback for _vaxia_transfer_xp_form().
 */
function _vaxia_transfer_xp_form_submit(&$form, &$form_state) {
  $old_character = node_load($form_state['values']['character']);
  $new_character = node_load($form_state['values']['target']);
  $workflow = variable_get('vaxia_transfer_workflow', '');
  if ($old_character && $new_character && !empty($workflow) && $old_character->nid != $new_character->nid) {
    // Grab XP for this character.
    $xp = $old_character->vaxia_unspent_xp + $old_character->vaxia_spent_xp;
    // Update the XP recorded for this character.
    _vaxia_delete_xp($old_character->nid);
    _vaxia_delete_log($old_character->nid);
    // And update the original character status.
    workflow_execute_transition($old_character, $workflow, t('Transferring XP to a new character'), TRUE);
    // Now add XP to the new character.
    $new_xp = floor($xp * variable_get('vaxia_transfer_percentage', 0.75));
    $bank_xp = floor($xp - $new_xp); 
    _vaxia_update_xp($new_character->nid, $new_xp, 'xp');
    _vaxia_update_xp($new_character->nid, $bank_xp, 'bxp');
  }
}

/**
 * Implements hook_character_sheet_delete_sheet().
 */
function vaxia_character_sheet_delete_sheet($node) {
  _vaxia_delete_xp($node->nid);
  _vaxia_delete_log($node->nid);
}

/**
 * Implements hook_character_sheet_insert_sheet().
 */
function vaxia_character_sheet_insert_sheet($node) {
}

/**
 * Implements hook_character_sheet_update_sheet().
 */
function vaxia_character_sheet_update_sheet($node, $old_node) {
  // Check if the sheet has been finalized.
  if ($node->workflow == variable_get('vaxia_live_workflow', '') && vaxia_character_sheet_no_xp_cost($node)) {
    // Total the value of points by *change* against available xp weighted.
    $old_total = _vaxia_total_sheet($old_node);
    $new_total = _vaxia_total_sheet($node);
    $spent_xp = $new_total - $old_total;
    // Charge the spent xp tp the spent_xp pool.
    _vaxia_update_xp($nid, $spent_xp, 'spent_xp');
  }
}

/**
 * Implements hook_character_sheet_load_sheet().
 */
function vaxia_character_sheet_load_sheet($node) {
  $xp = _vaxia_get_xp($node->nid, 'xp');
  if (!$xp) {
    $xp = 0;
  }
  $spent_xp = _vaxia_get_xp($node->nid, 'spent_xp');
  if (!$spent_xp) {
    $spent_xp = 0;
  }
  $bxp = _vaxia_get_xp($node->nid, 'bxp');
  if (!$bxp) {
    $bxp = 0;
  }
  $kxp = _vaxia_get_xp($node->nid, 'kxp');
  if (!$kxp) {
    $kxp = 0;
  }
  $rxp = _vaxia_get_xp($node->nid, 'rxp');
  if (!$rxp) {
    $rxp = 0;
  }
  $econ = _vaxia_get_xp($node->nid, 'econ');
  if (!$econ) {
    $econ = 0;
  }
  $unspent_xp = $xp - $spent_xp;
  $node->vaxia_xp = $xp;
  $node->vaxia_bxp = $bxp;
  $node->vaxia_kxp = $kxp;
  $node->vaxia_rxp = $rxp;
  $node->vaxia_econ = $econ;
  $node->vaxia_spent_xp = $spent_xp;
  $node->vaxia_unspent_xp = $unspent_xp;
  $logs = _vaxia_get_log_groups($node->nid);
  $message_log = array();
  foreach ($logs as $log) {
    $message_log[] = t('%xp %type xp awarded.',
      array('%xp' => $log->xp, '%type' => $log->type));
  }
  if (!empty($message_log)) {
    $node->vaxia_type_xp = theme('item_list', array('items' => $message_log));
  }
  else {
   $node->vaxia_type_xp = '';
  }
}

/**
 * Implements hook_character_sheet_validate_sheet().
 */
function vaxia_character_sheet_validate_sheet($node, $old_node, $form, $form_state) {
  // Check if the sheet has been finalized.
  if (isset($node->workflow)
    && ($node->workflow == variable_get('vaxia_live_workflow', '') && isset($old_node->nid))) {
    // Total the value of points by *change* against available xp weighted.
    $xp = _vaxia_get_xp($node->nid);
    $spent_xp = _vaxia_get_xp($node->nid, 'spent_xp');
    $available_xp = $xp - $spent_xp; // Available xp for spending.
    $old_total = _vaxia_total_sheet($old_node);
    $new_total = _vaxia_total_sheet($node);
    $spent_xp = $new_total - $old_total;
    // Calculate the difference in cost. The spare 300 won't matter in this diff.
  }
  else {
    // Total the value of points against starting points flat.
    if (!isset($old_node->nid) || !isset($xp)) {
      $available_xp = variable_get('vaxia_starting_points', 300);
    }
    $spent_xp = _vaxia_total_sheet($node, 'flat');
  }
  if ($spent_xp <= $available_xp || vaxia_character_sheet_no_xp_cost($node)) {
    drupal_set_message(t('You have spent %spent_xp xp out of %available_xp available xp.',
      array('%spent_xp' => $spent_xp, '%available_xp' => $available_xp)));
    return FALSE;
  }
  return $error = array('field' => '', 'message' => 
    t('You have spent %spent_xp xp out of %available_xp available xp. Please revise your sheet.',
      array('%spent_xp' => $spent_xp, '%available_xp' => $available_xp)));
}

/**
 * Helper function, determine how many points a sheet has spent.
 * ALERT: RECURSIVE. BE CAREFUL HERE.
 */
function _vaxia_total_sheet($node, $mode = 'weighted') {
  $total = 0;
  $entity = (array) $node;
  // Look at all elements of the node.
  foreach (element_children($entity) as $field_name) {
    if (strpos($field_name, 'field_') === 0) {
      $field = $entity[$field_name];
      if(isset($field[$entity['language']]) && is_array($field[$entity['language']])) {
        // If field is a field_collection call this function on the entity.
        // (could be done with user ref, node ref)
        // If field is a normal / plain field then try to grab the value and map cost.
        foreach ($field[$entity['language']] as $delta => $delta_field) {
          $bundle_type = 'generic';
          if (isset($entity['content'][$field_name]['#field_type'])) {
            $bundle_type = $entity['content'][$field_name]['#field_type'];
          }
          switch ($bundle_type) {
            case 'field_collection':
              $sub_entity = field_collection_item_load($delta_field['value']);
              $sub_entity->language = $entity['language'];
              $total += _vaxia_total_sheet($sub_entity);
              break;
            default:
              if (isset($delta_field['value']) && is_numeric($delta_field['value'])) {
                $total += _vaxia_field_calculate_value($field_name, $delta_field['value'], $mode);
              }
              break;
          }
        }
      }
    }
  }
  return $total;
}

/**
 * Helper function, assign values to field names.
 */
function _vaxia_field_calculate_value($field_name, $current_value, $mode = 'weighted') {
  // If in flat mode, we're just counting the value - not weighing each point of it as assigned.
  if ($mode == 'flat') {
    return $current_value;
  }
  // Else we calculate the total value, point by point starting at zero.
  $total = 0;
  $field_names = array(
    'field_skill_value',
    'field_life',
    'field_endurance',
    'field_strength',
    'field_dexterity',
    'field_intelligence',
    'field_spirituality',
    'field_charisma',
  );
  if (in_array($field_name, $field_names)) {
    for ($i=1;$i<=$current_value;$i++) {
      if ($field_name == 'field_skill_value') {
        $total += ($current_value / 10) * variable_get('vaxia_skill_cost', 0.75);
      }
      else {
        $total += ($current_value / 10) * variable_get('vaxia_attribute_cost', 1);
      }
    }
  }
  return $total;
}

/**
 * Implements hook_form_FORMID_alter().
 */
function vaxia_form_character_sheet_node_form_alter(&$form, &$form_state) {
  // If a field is in draft state, just let it go through unhindered.
  if (isset($form['#node']->workflow)) {
    $draft_state = variable_get('vaxia_draft_workflow', '');
    if ($form['#node']->workflow == $draft_state) {
      return $form;
    }
  }
  else {
    // New character sheets don't have a workflow, by default are in draft.
    return $form;
  }
  
  // Else, the field is in moderation or playable states.
  $lang = $form['language']['#value'];
  if (empty($lang)) {
    $lang = 'und';
  }
  $edit_status = _vaxia_field_edit_access($form['#node']);

  // Can't change character name or realm in principle.
  $form['title']['#disabled'] = TRUE;
  $form['title']['#value'] = $form['title']['#default_value'];
  $form['field_realm'][$lang]['#disabled'] = TRUE;
  $form['field_realm'][$lang]['#value'] = $form['field_realm'][$lang]['#default_value'];

  // Check for race and sex? TODO

  // Check skills.
  foreach (element_children($form['field_skill'][$lang]) as $delta) {
    switch($edit_status) {
      case 'free':
        // Edit freely. Do nothing.
      break;
      case 'edit':
        // Remove the option to add more skills.
        if (isset($form['field_skill'][$lang][$delta]['#name']) 
          && ($form['field_skill'][$lang][$delta]['#name'] == 'field_skill_add_more')) {
            unset($form['field_skill'][$lang][$delta]);
        }
        else {
          // Disable the skill name.
          $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#disabled'] = TRUE;
          $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#value'] =
            $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#default_value'];
          // Disable the skill desc.
          $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#disabled'] = TRUE;
          $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#value'] = 
            $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#default_value'];
          // MODIFY the skill value.
          $base_value = $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#default_value'];
          $options = range($base_value, $base_value + 10);
          $options = array_combine($options, $options);
          $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#type'] = 'select';
          $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#size'] = 1;
          $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#options'] = $options;
          // Hide the skill if the value is zero.
          if (empty($form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#value'])) {
            unset($form['field_skill'][$lang][$delta]);
          }
          // Disable the button to remove the skill.
          unset($form['field_skill'][$lang][$delta]['remove_button']);
        }
      break;
      case 'lock':
      default:
        // Remove the option to add more skills.
        if (isset($form['field_skill'][$lang][$delta]['#name']) 
          && ($form['field_skill'][$lang][$delta]['#name'] == 'field_skill_add_more')) {
            unset($form['field_skill'][$lang][$delta]);
        }
        else {
          // Disable the skill name.
          $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#disabled'] = TRUE;
          $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#value'] =
            $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#default_value'];
          // Disable the skill desc.
          $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#disabled'] = TRUE;
          $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#value'] = 
            $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#default_value'];
          // DISABLE the skill value.
          $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#disabled'] = TRUE;
          $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#value'] = 
            $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#default_value'];
          // Hide the skill if the value is zero.
          if (empty($form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#value'])) {
            unset($form['field_skill'][$lang][$delta]);
          }
          // Disable the button to remove the skill.
          unset($form['field_skill'][$lang][$delta]['remove_button']);
        }
      break;
    }
  }

  // Check attributes.
  $attributes = array(
    'field_life',
    'field_endurance',
    'field_strength',
    'field_dexterity',
    'field_intelligence',
    'field_spirituality',
    'field_charisma',
  );
  foreach ($attributes as $attribute_name) {
    switch($edit_status) {
      case 'free':
        // Edit freely. Do nothing.
      break;
      case 'edit':
        // MODIFY the attribute field.
        $base_value = $form[$attribute_name][$lang][0]['value']['#default_value'];
        $options = range($base_value, $base_value + 10);
        $options = array_combine($options, $options);
        $form[$attribute_name][$lang][0]['value']['#type'] = 'select';
        $form[$attribute_name][$lang][0]['value']['#size'] = 1;
        $form[$attribute_name][$lang][0]['value']['#options'] = $options;
      break;
      case 'lock':
      default:
        // Disable the attribute field.
        $form[$attribute_name][$lang][0]['value']['#disabled'] = TRUE;
        $form[$attribute_name][$lang][0]['value']['#value'] =
          $form[$attribute_name][$lang][0]['value']['#default_value'];
      break;
    }
  }
  
  global $user;
  // Misc info and resources for a character sheet.
  if (!user_access('post notes on character') || $user->uid == $form['#node']->uid) {
    $form['field_sh_notes'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_sh_notes'][$lang][0]['value']['#value'] =
      isset($form['field_sh_notes'][$lang][0]['value']['#default_value']) ? $form['field_sh_notes'][$lang][0]['value']['#default_value'] : '';
  }
  if (!user_access('add npcs to character sheet') || $user->uid == $form['#node']->uid) {
    $form['field_npcs'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_npcs'][$lang][0]['value']['#value'] = 
      isset($form['field_npcs'][$lang][0]['value']['#default_value']) ? $form['field_npcs'][$lang][0]['value']['#default_value'] : '';
   // Also remoe option to add more.
  }
  if (!user_access('add item to character sheet') || $user->uid == $form['#node']->uid) {
    $form['field_property'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_property'][$lang][0]['value']['#value'] = 
     isset($form['field_property'][$lang][0]['value']['#default_value']) ? $form['field_property'][$lang][0]['value']['#default_value'] : '';
    $form['field_items'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_items'][$lang][0]['value']['#value'] = 
      isset($form['field_items'][$lang][0]['value']['#default_value']) ? $form['field_artwork'][$lang][0]['value']['#default_value'] : '';
  }
  if (!user_access('add art to character sheet') || $user->uid == $form['#node']->uid) {
    $form['field_artwork'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_artwork'][$lang][0]['value']['#value'] = 
      isset($form['field_artwork'][$lang][0]['value']['#default_value']) ? $form['field_items'][$lang][0]['value']['#default_value'] : '';
  }
}

/**
 * Determine access based on node state and user permissions.
 */
function _vaxia_field_edit_access($node) {
  global $user;
  $draft_state = variable_get('vaxia_draft_workflow', '');
  $sub_state = variable_get('vaxia_eval_workflow', '');
  $live_state = variable_get('vaxia_live_workflow', '');
  // New nodes don't have workflows, by default are in draft.
  $workflow = isset($node->workflow) ? $node->workflow : $draft_state;

  if ($workflow == $draft_state) {
    if ($node->uid == $user->uid) {
      return 'free';
    }
  }
  elseif ($workflow == $sub_state) {
    if ($node->uid == $user->uid) {
      return 'edit';
    }
    if ($node->uid != $user->uid && user_access('evaluate character')) {
      return 'free';
    }
  }
  elseif ($workflow == $live_state) {
    if ($node->uid == $user->uid) {
      return 'edit';
    }
    if (user_access('edit live sheets')) {
      return 'free';
    }
  }
  return 'lock';
}

function _vaxia_meaningful_xp_types() {
  return array('xp', 'spent_xp', 'bxp', 'rxp', 'kxp', 'econ');
}

function _vaxia_add_xp($nid, $xp, $type) {
  $meaningful_types = _vaxia_meaningful_xp_types();
  if (!in_array($type, $meaningful_types)) {
    // Add a historical log for 'flavored' rxp given.
    // Flavor xp is basically xp from a GM, tagged according to type.
    if ($type != 'generic') {
      _vaxia_update_log($nid, $type, $xp);
    }
    $type = 'xp';
  }
  switch ($type) {
    case 'kxp':
      // Add wordcount xp.
      _vaxia_update_xp($nid, $xp, 'kxp');
      $current_kxp = _vaxia_get_xp($nid, 'kxp');
      // If we have enough to convert to other xp, do it.
      if ($current_kxp > variable_get('vaxia_kxp_ratio', 70)) {
        // Remove from kxp, add 1 to rxp.
        _vaxia_update_xp($nid, variable_get('vaxia_kxp_ratio', 70) * -1, 'kxp');
        _vaxia_update_xp($nid, 1, 'rxp');
        // If we have banked xp, remove one from bxp and add spendable xp.
        $current_bxp = _vaxia_get_xp($nid, 'bxp');
        if ($current_bpx > 0 ) {
          _vaxia_update_xp($nid, -1, 'bxp');
          _vaxia_update_xp($nid, 1, 'xp');
        }
      }
    break;
    case 'bxp':
      // If bxp is added directly, just add it to the bxp score.
      _vaxia_update_xp($nid, $xp, 'bxp');
    break;
    case 'rxp':
      // If rxp is added directly, just add it to the rxp score.
      _vaxia_update_xp($nid, $xp, 'rxp');
    break;
    case 'xp':
    default:
      while ($xp > 0 ) {
        $current_rxp = _vaxia_get_xp($nid, 'rxp');
        if ($current_rxp > 0 ) {
          _vaxia_update_xp($nid, -1, 'rxp');
          _vaxia_update_xp($nid, 1, 'xp');
        }
        else {
          _vaxia_update_xp($nid, 1, 'bxp');
        }
        $xp--;
      }
    break;
    case 'econ':
      // Economy is just added directly.
      _vaxia_update_xp($nid, 1, 'econ');
    break;
  }
}

/**
 * Helper function, determine access to modify without costing XP.
 */
function vaxia_character_sheet_no_xp_cost($node) {
  global $user;
  if(user_access('ignore cost on own character') && $node->uid == $user->uid) {
    return FALSE;
  }
  if(user_access('ignore cost on others character') && $node->uid != $user->uid) {
    return FALSE;
  }
  return TRUE;
}

/**
 * DB function, insert XP count to the database.
 */
function _vaxia_update_xp($nid, $xp, $type = 'xp') {
  $current_xp = _vaxia_get_xp($nid, $type);
  $record = array(
    'xp' => $xp,
    'nid' => $nid,
    'type' => $type,
  );
  if ($current_xp !== FALSE) {
    $record['xp'] += $current_xp; // Update current xp total to track.
    drupal_write_record('vaxia_xp', $record, array('nid', 'type'));
  }
  else {
    drupal_write_record('vaxia_xp', $record);
  }
}

/**
 * DB function, delete XP count to the database.
 */
function _vaxia_delete_xp($nid) {
  db_query('DELETE FROM {vaxia_xp} WHERE nid = :nid', array(':nid' => $nid));
}

/**
 * DB function, get XP count to the database.
 */
function _vaxia_get_xp($nid, $type = 'xp') {
  $type_query = 'SELECT xp FROM {vaxia_xp} WHERE nid = :nid';
  $args = array(':nid' => $nid);
  // Filter for the type.
  if ($type != 'all') {
    $type_query .= ' AND type=:type';
    $args[':type'] = $type;
  }
  // Make the query.
  $total = 0;
  $results = db_query($type_query, $args);
  foreach ($results as $result) {
    if (!empty($result->xp)) {
      $total = $result->xp;
    }
  }
  return $total;
}

/*
 * Database helper function, get status matching a given nid.
 */
function _vaxia_get_log($nid) {
  $logs = array();
  $results = db_query('SELECT * FROM {vaxia_xp_log} WHERE nid = :nid ORDER BY timestamp DESC',
    array(':nid' => $nid));
  foreach ($results as $result) {
    $logs[] = $result;
  }
  return $logs;
}

/*
 * Database helper function, get status matching a given nid.
 */
function _vaxia_get_log_groups($nid) {
  $logs = array();
  $results = db_query('SELECT *, SUM(xp) AS xp FROM {vaxia_xp_log} WHERE nid = :nid GROUP BY type',
    array(':nid' => $nid));
  foreach ($results as $result) {
    $logs[] = $result;
  }
  return $logs;
}

/*
 * Database helper function, delete all logs matching a given nid.
 */
function _vaxia_delete_log($nid) {
  db_delete('vaxia_xp_log')
  ->condition('nid', $nid)
  ->execute();
}

/*
 * Database helper function, insert a new log entry.
 */
function _vaxia_update_log($nid, $type, $xp) {
  global $user;
  $record = array(
    'uid' => $user->uid,
    'type' => $type,
    'xp' => $xp,
    'nid' => $nid,
    'hostname' => ip_address(),
    'timestamp' => REQUEST_TIME,
  );
  drupal_write_record('vaxia_xp_log', $record);
}
