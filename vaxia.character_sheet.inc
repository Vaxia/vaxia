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
    'description' => 'Vaxia system featuring a points tracker for attribute and skill fields.',
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
    '#default_value' => variable_get('vaxia_starting_xp', 250),
  );
  $form['settings']['vaxia_starting_econ'] = array(
    '#title' => 'Starting Economy level',
    '#description' => 'The starting Economy level for a new character.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_starting_econ', 25),
  );
  $form['settings']['vaxia_xp_types'] = array(
    '#title' => 'XP types',
    '#description' => 'Available XP sub-types',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_xp_types', "good\nevil\smart\nclever\nwarrior\nmage\nrogue"),
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
  return $form;
}

/**
 * Experience can be tracked as userpoints or as here as a table of it's own. Really just about any way you want to.
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
  _vaxia_insert_xp($node->nid, variable_get('vaxia_starting_xp', 250));
}

/**
 * Implements hook_character_sheet_load_sheet().
 */
function vaxia_character_sheet_load_sheet($node) {
  $xp = _vaxia_get_xp($node->nid);
  if (!$xp) {
    $xp = 0;
  }
  $spent_xp = _vaxia_total_sheet($node);
  $unspent_xp = $xp - $spent_xp;
  $node->vaxia_xp = $xp;
  $node->vaxia_spent_xp = $spent_xp;
  $node->vaxia_unspent_xp = $unspent_xp;
}

/**
 * Implements hook_character_sheet_validate_sheet().
 */
function vaxia_character_sheet_validate_sheet($node, $old_node, $form, $form_state) {
  $xp = _vaxia_get_xp($node->nid);
  if (!$old_node || !$xp) {
    $xp = variable_get('vaxia_starting_xp', 250);
  }
  $spent_xp = _vaxia_total_sheet($node);
  if ($spent_xp <= $xp) {
    return FALSE;
  }
  return $error = array('field' => '', 'message' => 
    t('You have spent %spent_xp xp out of %xp available xp. Please revise your sheet.', array('%spent_xp' => $spent_xp, '%xp' => $xp)));
}

/**
 * Implements hook_form_FORMID_alter().
 */
function vaxia_form_character_sheet_node_form_alter(&$form, &$form_state) {
  $workflow_state = workflow_get_workflow_states_by_sid($form['#node']->workflow);
  if ($workflow_state->state != 'draft' && !user_access('evaluate character')) {
    // User is not allowed to modify certain elements on their sheet. Such as: name, skill names and skill descriptions.
    $lang = $form['language']['#value'];
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
    $form['field_sh_notes'][$lang][0]['value']['#value'] = $form['field_sh_notes'][$lang][0]['value']['#default_value'];
  }
  if (!user_access('add item to character sheet')) {
    $form['field_character_sheet_artwork'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_character_sheet_artwork'][$lang][0]['value']['#value'] = 
      $form['field_character_sheet_artwork'][$lang][0]['value']['#default_value'];
  }
  if (!user_access('add property to character sheet')) {
    $form['field_npcs'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_npcs'][$lang][0]['value']['#value'] = $form['field_npcs'][$lang][0]['value']['#default_value'];
  }
  if (!user_access('add item to character sheet')) {
    $form['field_property'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_property'][$lang][0]['value']['#value'] = 
      $form['field_property'][$lang][0]['value']['#default_value'];
  }
  if (!user_access('add art to character sheet')) {
    $form['field_items'][$lang][0]['value']['#disabled'] = TRUE;
    $form['field_items'][$lang][0]['value']['#value'] = $form['field_items'][$lang][0]['value']['#default_value'];
  }
}

/**
 * Implements hook_character_sheet_view_sheet().
 */
function vaxia_character_sheet_view_sheet($node) {
  $node->content['vaxia'] = array(
    '#type' => 'fieldset',
  );
  $node->content['vaxia']['xp'] = array(
    '#type' => 'item',
    '#title' => 'XP Total',
    '#markup' => t('Spent %spent out of %xp.',
      array('%spent' =>$node->vaxia_spent_xp, '%xp' => $node->vaxia_xp)),
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
    if (!$old_xp) {
      _vaxia_update_xp($node->nid, variable_get('vaxia_starting_xp', 250), '');
    }
    vaxia_insert_xp($node->nid, $values['xp'], $values['type']);
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
        // If field is a field_collection call this function on the entity. (could be done with user ref, node ref)
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

/**
 * DB function, insert XP count to the database.
 */
function _vaxia_insert_xp($nid, $xp, $type) {
  _vaxia_update_log($nid, $type, ':xp was awarded.', array(':xp' => $xp));
  // TODO: CALCULATE ALL CHANGES TO XP, KXP, BXP, RXP.
  // NEGATIVE NUMBERS ARE ALLOWED (as XP goes from one pool to the next.)
  // DO NOT REPEAT THE LOG.
  db_query('INSERT INTO {vaxia_xp} (xp, nid, type) VALUES (:xp, :nid, :type)', 
    array(':xp' => $xp, ':nid' => $nid, ':type' => $type));
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
function _vaxia_get_xp($nid, $type = 'all') {
  // Allows for negative XP values to remove XP from a sheet.
  $total = FALSE;
  $type_query = '';
  $args = array(':nid' => $nid);
  if ($type != 'all') {
    $type_query = ' AND type=:type';
    $args[':type'] = $type;
  }
  $results = db_query('SELECT SUM(xp) FROM {vaxia_xp} WHERE nid = :nid', $args);
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
  $results = db_query("SELECT * FROM {vaxia_xp_log} WHERE nid = :nid ORDER BY timestamp DESC", array(':nid' => $nid));
  foreach ($results as $result) {
    $result = (array) $result;
    $message = '[' .format_date($result['timestamp']). ' ] ' . $result['message'];
    $logs[] = $message;
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
function _vaxia_update_log($nid, $type, $message, $variables = array()) {
  global $user;
  $record = array(
    'uid' => $user->uid,
    'type' => $type,
    'message' => t($message, $variables),
    'nid' => $nid,
    'hostname' => ip_address(),
    'timestamp' => REQUEST_TIME,
  );
  drupal_write_record('vaxia_xp_log', $record);
}