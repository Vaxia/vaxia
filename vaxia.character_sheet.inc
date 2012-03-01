<?php
/**
 * @file
 * vaxia.dice.inc - Provides Vaxian character sheets for Vaxia.
 */

/* 
 * Implementation of hook_character_sheet_register_systems(). 
 */
function vaxia_character_sheet_register_systems() {
  $system['vaxia'] = array(
    'name' => 'Vaxia character sheet',
    'description' => 'Vaxia system featuring a points tracker for '.
      'attribute and skill fields.',
    'admin_callback' => '_vaxia_admin_settings',
  );
  return $system;
}

/*
 * Callback for admin settings page for character sheets.
 */
function _vaxia_admin_settings() {
  $form['settings'] = array(
    '#title' => 'Settings',
    '#type' => 'fieldset',
  );
  $form['settings']['vaxia_starting_xp'] = array(
    '#title' => 'Starting XP',
    '#description' => 'The starting XP points for a new character.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_starting_xp', 300),
  );
  $form['settings']['vaxia_starting_econ'] = array(
    '#title' => 'Starting Economy level',
    '#description' => 'The starting Economy level for a new character.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_starting_econ', 25),
  );
  $form['settings']['vaxia_kxp_level'] = array(
    '#title' => 'Posted XP ratio',
    '#description' => 'The ratio of kxp to xp per comment word count.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_kxp_level', 70),
  );
  $form['settings']['vaxia_xp_types'] = array(
    '#title' => 'XP types',
    '#description' => 'Available XP sub-types',
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
  return $form;
}


/**
 * Menu callback, transfer XP page.
 */
function _vaxia_transfer_xp($node) {
  return drupal_get_form('_vaxia_transfer_xp_form', $node);
}

/**
 * Transfer XP form.
 */
function _vaxia_transfer_xp_form($form, &$form_state, $node) {
  $form['node_info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Character sheet information'),
  );
  $form['node_info']['name'] = array(
    '#type' => 'item',
    '#title' => t('Character'),
    '#markup' => $node->title,
  );
  $xp = _vaxia_get_xp($node->nid);
  $form['node_info']['xp'] = array(
    '#type' => 'item',
    '#title' => t('Current XP total'),
    '#markup' => $xp ? $xp : 0,
  );
  $logs = _vaxia_get_log_groups($node->nid);
  $message_log = array();
  foreach ($logs as $log) {
    $message_log[] = t('%xp %type xp awarded.',
      array('%xp' => $log->xp, '%type' => $log->type));
  }
  $form['node_info']['xp_log'] = array(
    '#type' => 'item',
    '#title' => t('XP history'),
    '#markup' => theme('item_list', array('items' => $message_log)),
  );
  $form['alert'] = array(
    '#type' => 'fieldset',
    '#title' => t('NOTICE'),
  );
  $form['alert']['alert'] = array(
    '#type' => 'item',
    '#markup' => t('IF YOU TRANSFER YOUR CHARACTER XP: YOUR CHARACTER WILL NOT BE PLAYABLE AFTERWARDS.'),
  );
  $form['alert']['details'] = array(
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
  $form['confirm']['character'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['confirm']['target'] = array(
    '#type' => 'select',
    '#title' => t('Transfer to'),
    '#options' => $available_characters,
    '#description' => t('Transfer XP to this character sheet.'),
  );
  $form['confirm']['confirm'] = array(
    '#type' => 'textfield',
    '#title' => t('Confirm'),
    '#description' => t('If you really mean to do this, please type "TRANSFER" into this textbox, in all caps.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Transfer character XP'),
  );
  return $form;
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
    $xp = $old_character->vaxia_xp;
    // Update the XP recorded for this character.
    _vaxia_delete_xp($old_character->nid);
    _vaxia_delete_xp_log($old_character->nid);
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
 * Experience can be tracked as userpoints or as here as a table of it's own.
 * Really just about any way you want to.
 * Userpoints ties in best with outside systems like Rules, Views, and the like.
 */

/**
 * Implements hook_character_sheet_delete_sheet().
 */
function vaxia_character_sheet_delete_sheet($node) {
  _vaxia_delete_xp($node->nid);
  _vaxia_delete_xp_log($node->nid);
}

/**
 * Implements hook_character_sheet_insert_sheet().
 */
function vaxia_character_sheet_insert_sheet($node) {
  _vaxia_update_xp($node->nid, variable_get('vaxia_starting_xp', 300), 'xp');
}

/**
 * Implements hook_character_sheet_load_sheet().
 */
function vaxia_character_sheet_load_sheet($node) {
  $xp = _vaxia_get_xp($node->nid, 'xp');
  if (!$xp) {
    $xp = 0;
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
  $spent_xp = _vaxia_total_sheet($node);
  $unspent_xp = $xp - $spent_xp;
  $node->vaxia_xp = $xp;
  $node->vaxia_bxp = $bxp;
  $node->vaxia_kxp = $kxp;
  $node->vaxia_rxp = $rxp;
  $node->vaxia_spent_xp = $spent_xp;
  $node->vaxia_unspent_xp = $unspent_xp;
  $logs = _vaxia_get_log_groups($node->nid);
  $message_log = array();
  foreach ($logs as $log) {
    $message_log[] = t('%xp %type xp awarded.',
      array('%xp' => $log->xp, '%type' => $log->type));
  }
  $node->vaxia_type_xp = theme('item_list', array('items' => $message_log));
}

/**
 * Implements hook_character_sheet_validate_sheet().
 */
function vaxia_character_sheet_validate_sheet($node, $old_node, $form, $form_state) {
  $xp = _vaxia_get_xp($node->nid);
  if (!$old_node || !$xp) {
    $xp = variable_get('vaxia_starting_xp', 300);
  }
  $spent_xp = _vaxia_total_sheet($node);
  if ($spent_xp <= $xp) {
    return FALSE;
  }
  return $error = array('field' => '', 'message' => 
    t('You have spent %spent_xp xp out of %xp available xp. Please revise your sheet.',
      array('%spent_xp' => $spent_xp, '%xp' => $xp)));
}

/**
 * Implements hook_form_FORMID_alter().
 */
function vaxia_form_character_sheet_node_form_alter(&$form, &$form_state) {
  $lang = $form['language']['#value'];
  if (empty($lang)) {
    $lang = 'und';
  }  
  if (isset($form['#node']->workflow)) {
    $workflow_state = workflow_get_workflow_states_by_sid($form['#node']->workflow);
  }
  if ((!empty($workflow_state->state) && $workflow_state->state != 'draft')
      && !user_access('evaluate character')) {
    // User is not allowed to modify certain elements on their sheet.
    // Such as: name, skill names and skill descriptions.
    $form['title']['#disabled'] = TRUE;
    $form['title']['#value'] = $form['title']['#default_value'];
    $form['field_realm'][$lang]['#disabled'] = TRUE;
    $form['field_realm'][$lang]['#value'] = $form['field_realm'][$lang]['#default_value'];
    foreach (element_children($form['field_skill'][$lang]) as $delta) {
      if (isset($form['field_skill'][$lang][$delta]['#name']) 
        && ($form['field_skill'][$lang][$delta]['#name'] == 'field_skill_add_more')) {
          unset($form['field_skill'][$lang][$delta]);
      }
      else {
        $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#disabled'] = TRUE;
        $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#value'] =
          $form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#default_value'];
        $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#disabled'] = TRUE;
        $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#value'] = 
          $form['field_skill'][$lang][$delta]['field_skill_desc'][$lang][0]['value']['#default_value'];
        if (empty($form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#value'])) {
          unset($form['field_skill'][$lang][$delta]);
        }
        unset($form['field_skill'][$lang][$delta]['remove_button']);
      }
    }
  }
  if (!user_access('post notes on character')) {
    $form['field_sh_notes'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_sh_notes'][$lang][0]['value']['#value'] =
      $form['field_sh_notes'][$lang][0]['value']['#default_value'];
  }
  if (!user_access('add item to character sheet')) {
    $form['field_artwork'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_artwork'][$lang][0]['value']['#value'] = 
      $form['field_artwork'][$lang][0]['value']['#default_value'];
  }
  if (!user_access('add property to character sheet')) {
    $form['field_npcs'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_npcs'][$lang][0]['value']['#value'] =
      $form['field_npcs'][$lang][0]['value']['#default_value'];
  }
  if (!user_access('add item to character sheet')) {
    $form['field_property'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_property'][$lang][0]['value']['#value'] = 
      $form['field_property'][$lang][0]['value']['#default_value'];
  }
  if (!user_access('add art to character sheet')) {
    $form['field_items'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_items'][$lang][0]['value']['#value'] =
      $form['field_items'][$lang][0]['value']['#default_value'];
  }
}

/**
 * Implements hook_character_sheet_view_sheet().
 */
function vaxia_character_sheet_view_sheet($node) {
  $node->content['vaxia'] = array(
    '#type' => 'fieldset',
  );
  $xps[] = t('Spent %spent out of %xp xp.',
    array('%spent' =>$node->vaxia_spent_xp, '%xp' => $node->vaxia_xp));
  $xps[] = t('Banked %banked bxp.', array('%banked' =>$node->vaxia_bxp));
  $xps[] = t('Roleplayed %roleplayed rxp.', array('%roleplayed' => $node->vaxia_rxp));
  $xps[] = t('Posted %posted kxp.', array('%posted' => $node->vaxia_kxp));
  $node->content['vaxia']['xp'] = array(
    '#type' => 'item',
    '#title' => 'XP Total',
    '#markup' => theme('item_list', array('items' => $xps)),
  );

  $node->content['vaxia']['type'] = array(
    '#type' => 'item',
    '#access' => user_access('add xp to character sheet'),
    '#markup' => drupal_render(drupal_get_form('vaxia_add_xp_form', $node)),
  );
}

/*
 * Form callback function from node view, adds XP form to node display.
 */
function vaxia_add_xp_form($form, &$form_state, $node) {
  $form['xp'] = array(
    '#type' => 'textarea',
    '#title' => t('Add XP'),
    '#default_value' => 0,
    '#description' => t('Add XP to this character sheet.'),
  );
  $types = variable_get('vaxia_xp_types', "good\nevil\smart\nclever\nwarrior\nmage\nrogue");
  $types = explode("\n", $types);
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#default_value' => 0,
    '#options' => combine_array($types, $types),
    '#description' => t('Add XP to this character sheet.'),
  );
  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add XP'),
  );
  return $form;
}

/*
 * Form callback submission for XP form.
 */
function vaxia_add_xp_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $node = node_load($values['nid']);
  global $user;
  if (!empty($node) && user_access('add xp to character sheet')) {
    $old_xp = _vaxia_get_xp($node->nid);
    // If somehow a character got this far without xp, add the baseline.
    if (!$old_xp) {
      _vaxia_update_xp($node->nid, variable_get('vaxia_starting_xp', 300), 'xp');
    }
    // Add xp from Session.
    _vaxia_add_xp($node->nid, $values['xp'], $values['type']);
  }
}

/**
 * Helper function, determine how many points a sheet has spent.
 * ALERT: RECURSIVE. BE CAREFUL HERE.
 */
function _vaxia_total_sheet($node) {
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
                $total += _vaxia_field_calculate_cost($field_name, $delta_field['value']);
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
function _vaxia_field_calculate_cost($field_name, $current_value) {
  // If a skill.
  if ($field_name == 'field_skill_value') {
    return floor($current_value / 10) * variable_get('vaxia_skill_cost', 0.75);
  }
  // If an attribute.
  $attributes = array(
    'field_life',
    'field_endurance',
    'field_strength',
    'field_dexterity',
    'field_intelligence',
    'field_spirituality',
    'field_charisma',
  );
  if (in_array($field_name, $attributes)) {
    return floor($current_value / 10) * variable_get('vaxia_attribute_cost', 1);
  }
  // If value is not found. Calculated fields, descriptions, etc.
  return 0;
}

function _vaxia_add_xp($nid, $xp, $type) {
  $meaningful_types = array('xp', 'bxp', 'rxp', 'kxp');
  if (!in_array($type, $meaningful_types)) {
    // Add a historical log for 'flavored' rxp given.
    // Flavor xp is basically xp from a GM, tagged according to type.
    _vaxia_update_log($nid, $type, $xp);
    $type = 'xp';
  }
  switch ($type) {
    case 'kxp':
      // Add wordcount xp.
      _vaxia_update_xp($nid, $xp, 'kxp');
      $current_kxp = _vaxia_get_xp($nid, 'kxp');
      // If we have enough to convert to other xp, do it.
      if ($current_kxp > variable_get('vaxia_kxp_level', 70)) {
        // Remove from kxp, add 1 to rxp.
        _vaxia_update_xp($nid, variable_get('vaxia_kxp_level', 70) * -1, 'kxp');
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
  }
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
  if ($current_xp) {
    $record['xp'] += $current_xp; // Add current xp total to track.
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
  $type_query = 'SELECT SUM(xp) AS xp FROM {vaxia_xp} WHERE nid = :nid';
  $args = array(':nid' => $nid);
  // Filter for the type.
  if ($type != 'all') {
    $type_query = ' AND type=:type';
    $args[':type'] = $type;
  }
  // Make the query.
  $total = FALSE;
  $results = db_query($type_query, $args);
  foreach ($results as $result) {
    $total = $result->xp;
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
