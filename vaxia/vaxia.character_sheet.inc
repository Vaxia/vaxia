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
  $xp_types = array('xp', 'silver');
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
 * Value callback for determining costs. Modified from _character_sheet_calculate_field_value().
 */
function _vaxia_calculate_attribute_field_value($field_info, $value) {
  if (!isset($value)) { return 0; }
  if (isset($field_info['costs']['cost_mode']) && $field_info['costs']['cost_mode'] == 'flat') {
    $total = $value * $field_info['costs']['cost'];
  }
  else {
    $total = 0;
    $current = 0;
    $cost_modifier = isset($field_info['costs']['cost']) ? $field_info['costs']['cost'] : 1;
    $step = isset($field_info['costs']['step']) ? $field_info['costs']['step'] : 1;
    while ($current < $value) {
      $current += $step;
      $total += floor($current * 0.1) * $cost_modifier;
    }
  }
  return $total;
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
        'cost_mode' => 'flat', // Cost is flat per point added not multipled of the current cost. (Default is multipled.)
        'step' => 1, // How many steps per value. Default 1.
         value_callback => '', // Function to handle value calculation for current value, instead of character sheet's own.
         costs_callback => '', // Function to handle cost per next step(s), instead of character sheet's own.
      ),
    ),
  );
 */

/**
 * Implements hook_character_sheet_field_info().
 */
function vaxia_character_sheet_field_info() {
  return array(
    'title' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
        'other' => DISABLE_SHEET,
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
        'cost' => variable_get('vaxia_attribute_cost', 1),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
      ),
    ),
    'field_endurance' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
      ),
    ),
    'field_strength' => array(
       'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
      ),
    ),
    'field_dexterity' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
      ),
    ),
    'field_intelligence' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
      ),
    ),
    'field_spirituality' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
      ),
    ),
    'field_charisma' => array(
      'permissions' => array(
        'owner' => LIMIT_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_attribute_cost', 1),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
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
        'cost' => variable_get('vaxia_skill_cost', 0.75),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
      ),
    ),
    'field_skill_name' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    'field_skill_desc' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    'field_economy' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
      'costs' => array(
        'xp_type' => 'xp',
        'cost' => variable_get('vaxia_economy_cost', 1),
        'step' => 1,
        'value_callback' => '_vaxia_calculate_attribute_field_value',
      ),
    ),
    'field_items' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    'field_property' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
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
    'field_sh_private_notes' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    //field_player_notes
  );
}

/**
 * Implelents node_form_alter for nodes.
 * Character sheet, field level controls for players and moderators according to character sheet defs.
 */
function vaxia_form_node_form_alter(&$form, &$form_state, $form_id) {
  $node = $form['#node'];

  // Only on a character sheet that isn't yet set.
  if (_character_sheet_is_enabled($node) && !_character_sheet_is_playable($node)) {
    // Remove skill's remove button and add more button IIF owner viewing playable.
    $lang = $form['language']['#value'];
    // Disable the economy field.
    $form['field_economy'][$lang][0]['#disabled'] = TRUE;
    if (isset($form['field_economy'][$lang][0]['#default_value'])) {
      $form['field_economy'][$lang][0]['#value'] = $form['field_economy'][$lang][0]['#default_value'];
    }
    // Disable these fields.
    $disabled = array('field_property', 'field_sh_notes');
    foreach ($disabled as $field_name) {
      $form[$field_name][$lang][0]['#disabled'] = TRUE;
      if (isset($form[$field_name][$lang][0]['#default_value'])) {
        $form[$field_name][$lang][0]['#value'] = $form[$field_name][$lang][0]['#default_value'];
      }
      if (isset($form[$field_name][$lang]['add_more'])) {
        $form[$field_name][$lang]['add_more']['#access'] = FALSE;
      }
    }
  }

  // Only do this if this is a character sheet in play.
  if (_character_sheet_is_enabled($node) && _character_sheet_is_playable($node)) {
    // Remove skill's remove button and add more button IIF owner viewing playable.
    global $user;
    $lang = $form['language']['#value'];
    if ($node->uid == $user->uid) {
      unset($form['field_skill'][$lang]['add_more']);
      foreach ($form['field_skill'][$lang] as $delta => $field) {
        if (is_array($field) && isset($form['field_skill'][$lang][$delta]['remove_button'])) {
          unset($form['field_skill'][$lang][$delta]['remove_button']);
        }
        if (is_array($field) && !isset($form['field_skill'][$lang][$delta]['field_skill_name'][$lang][0]['value']['#default_value'])) {
          unset($form['field_skill'][$lang][$delta]);
        } 
      }
    }
  }
}

/**
 * Implements hook_character_sheet_presave_sheet().
 */
function vaxia_character_sheet_presave_sheet($node) {
  if ($node->is_new || !empty($node->revision)) {
    $node->field_economy[$node->language][0]['value'] = variable_get('vaxia_starting_econ', 25);
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
        $new_value = _vaxia_get_field_value_flat($node, $field_key, $fields_info);
        // Add the debt to the ongoing total for this type.
        $xp_type = $field_info['costs']['xp_type'];
        $cost = $new_value;
        $spent_xp[$xp_type] = isset($spent_xp[$xp_type]) ? $spent_xp[$xp_type] + $cost : $cost;
        if ($spent_xp[$xp_type] > $available_xp) {
          return $error = array('field' => '', 'message' => 
            t('You have spent %spent_xp xp out of %available_xp available xp. Please revise your sheet.',
              array('%spent_xp' => $spent_xp[$xp_type], '%available_xp' => $available_xp)));
        }
      }
    }
  }
  return FALSE;
}

/**
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
    '#title' => 'KXP to XP ratio',
    '#description' => 'The ratio of kxp to xp.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_kxp_ratio', 70),
  );
  $form['settings']['vaxia_kxp_per_words'] = array(
    '#title' => 'Words to KXP ratio',
    '#description' => 'The ratio of comment word count to kxp.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_kxp_per_words', 20),
  );
  $form['settings']['vaxia_max_kxp_post'] = array(
    '#title' => 'Max KXP per post',
    '#description' => 'The maximum amount of kxp to be awarded per post.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_max_kxp_post', 10),
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
  $rooms = _rpg_chat_get_rooms();
  $options = array();
  foreach ($rooms as $room) {
    $options[$room->nid] = $room->title;
  }
  $form['settings']['vaxia_limbo_room'] = array(
    '#title' => 'Limbo room',
    '#description' => t('In this room, posts made do not gain XP.'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('vaxia_limbo_room', ''),
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
  $form['point_cost']['vaxia_economy_cost'] = array(
    '#title' => 'Economy cost',
    '#description' => 'Economy cost is calculated as (current/10) * multiplier.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_economy_cost', 1),
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
  $node = node_load($form['node_info']['nid']['#value']);

  $form['node_info']['xp_details'] = array(
    '#type' => 'item',
    '#title' => t('XP details'),
    '#markup' => theme('item_list', array('items' => _vaxia_xp_display($node))),
  );

  if (count($xp_types) > 0) {
    $form['node_info']['xp_types'] = array(
      '#type' => 'item',
      '#title' => t('XP types'),
      '#markup' => theme('item_list', array('items' => _vaxia_xp_display_type($node))),
    );
  }

  // Add the economy form for owned forms.
  global $user;
  $level = isset($node->field_economy[$node->language][0]['value']) ? $node->field_economy[$node->language][0]['value'] : 0;
  $econ[] = t('Your current economy level is %level.', array('%level' => $level));
  $income = _vaxia_get_month_income($level);
  $econ[] = t('Your monthly disposable income is %income.', array('%income' => number_format($income, 0)));
  $pay = _vaxia_get_daily_pay($level);
  $econ[] = t('You get paid %pay a day.', array('%pay' => number_format($pay, 1)));
  $form['node_info']['economy'] = array(
    '#type' => 'item',
    '#title' => t('Economy'),
    '#markup' => theme('item_list', array('items' => $econ)),
  );
  $cost = _vaxia_invest_calculate_cost_to_improve($level);
  $invest = $cost - $silver_invested;
  // Invested silver is spent_silver, adjusted to show the difference between spent and the cost of the current economy level.
  $desc = t('You need to invest %invest more silver before you will reach %level economy. Invested so far %so_far.',
    array('%invest' => number_format($invest, 0), '%level' => $level + 1, '%so_far' => $silver_invested));
  if ($available_silver >= 1 ) {
    $available_silver = floor($available_silver);
    $options = range(0, $available_silver);
    $options[0] = '-';
    $invest = 0;
    // Figure out how many given current silver you can add.
    $form['node_info']['invest_silver'] = array(
      '#title' => t('Invest silver'),
      '#type' => 'select',
      '#options' => $options,
      '#description' => $desc,
    );
    $form['node_info']['submit_economy'] = array(
      '#type' => 'submit',
      '#value' => t('Invest silver'),
      '#validate' => array(),
      '#submit' => array('_vaxia_invest_silver_submit'),
    );
  }
  else {
    $form['node_info']['invest_silver'] = array(
      '#type' => 'item',
      '#value' => t('You currectly have no silver available to invest.'),
      '#description' => $desc,
    );
  }

  // Add additional options for xp IIF allowed.
  if (isset($form['add_xp'])) {
    $form['add_xp']['xp']['#options'] = range(0, 500);
    $xp_types = array();
    foreach(explode("\n", variable_get('vaxia_xp_types', "good\nevil\nsmart\nclever\nwarrior\nmage\nrogue")) as $type) {
      $type = trim($type);
      $xp_types[$type] = $type;
    }
    $xp_types['silver'] = 'silver';
    $form['add_xp']['type']['#options'] = array_merge($xp_types, $form['add_xp']['type']['#options']);
  }

  // And add the transfer options.
  $setting = variable_get('vaxia_transfer_workflow', '');
  if (!empty($setting) && ($node->uid == $user->uid || user_access('transfer others sheets')) ) {
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
      '#validate' => array('_vaxia_transfer_xp_validate'),
      '#submit' => array('_vaxia_transfer_xp_submit'),
    );
  }
}

/**
 * Implements hook_form_FORMID_alter().
 */
function vaxia_form__character_sheet_owner_form_alter(&$form, &$form_state) {
  $form['change_ownership'] = array(
    '#type' => 'fieldset',
    '#title' => 'Change Ownership',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['change_ownership']['owner_name'] = $form['owner_name'];
  unset($form['owner_name']);
  $form['change_ownership']['submit'] = $form['submit'];
  unset($form['submit']);
  $form['change_ownership']['character_sheet_log'] = $form['character_sheet_log'];
  unset($form['character_sheet_log']);
}

/**
 * Helper function, gather xp for display.
 */
function _vaxia_xp_display($node) {
  // Get specific info for vaxia to display in markup.
  $this_xp = _character_sheet_get_xp($node->nid, 'spent_xp');
  $xps['spent_xp'] = t('Spent %spent out of %xp xp.', array('%spent' => $this_xp, '%xp' => $node->xp['xp']));
  $this_xp = _character_sheet_get_xp($node->nid, 'bxp');
  $xps['bxp'] = t('Banked %banked bxp.', array('%banked' => $this_xp));
  $this_xp = _character_sheet_get_xp($node->nid, 'rxp');
  $xps['rxp'] = t('Roleplayed %roleplayed rxp.', array('%roleplayed' => $this_xp));
  $this_xp = _character_sheet_get_xp($node->nid, 'kxp');
  $xps['kxp'] = t('Posted %posted kxp.', array('%posted' => $this_xp));
  $silver = _character_sheet_get_xp($node->nid, 'silver');
  $silver_spent = _character_sheet_get_xp($node->nid, 'spent_silver');
  $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
  $available_silver = $silver - ($silver_spent + $silver_invested);
  $xps['available_silver'] = t('Have %posted silver.', array('%posted' => $available_silver));
  return $xps;
}

/**
 * Helper function, gather xp type records for display.
 */
function _vaxia_xp_display_type($node) {
  // Get specific info for vaxia to display in markup.
  // Display types of xp.
  $xp_types = array();
  $types = _vaxia_get_type_tracking_by_nid($node->nid);
  foreach ($types as $type) {
    $xp_types[] = t('Earned %xp %type XP.', array('%xp' => $type->xp, '%type' => $type->type));
  }
  return $xp_types;
}

/**
 * Helper function, figure out how much it costs to improve economy by one.
 */
function _vaxia_invest_calculate_cost_to_improve($economy) {
  // Given an economy value, figure out how much silver it would take to increase it by one.
  $total_worth = _vaxia_calculate_worth($economy);
  $economy++;
  $new_worth = _vaxia_calculate_worth($economy);
  return $new_worth - $total_worth;
}

/**
 * Helepr function, given an economy level, figure out how much income in a month.
 */
function _vaxia_get_month_income($economy) {
  $total_worth = _vaxia_calculate_worth($economy);
  return (($total_worth) * (3.5 / 100)) + (0.05);
}

/**
 * Helepr function, given an economy level, figure out how much income in a day.
 */
function _vaxia_get_daily_pay($economy) {
  $monthly = _vaxia_get_month_income($economy);
  $daily = $monthly / 30;
  return $daily;
}

/**
 * Helper function, figure out how much you're worth in silver.
 */
function _vaxia_calculate_worth($economy) {
  $power = (0.04 * $economy) + 1.853 + (-4 * ( 1 / ($economy + 1) ));
  return pow(10, $power) + (0.5);
}

/**
 * Submit callback for investing silver.
 */
function _vaxia_invest_silver_submit($form, $form_state) {
  $nid = $form_state['values']['nid'];
  $invest = $form_state['values']['invest_silver'];
  if (is_numeric($invest) && $invest > 0 ) {
    // Figure out how many to add based on added silver.
    $node = node_load($nid);
    $cost = _vaxia_invest_calculate_cost_to_improve($node->field_economy[$node->language][0]['value']);
    $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
    while ($invest > 0) {
      $invest--;
      $silver_invested++;
      _character_sheet_update_xp($node->nid, 1, 'invest_silver');
      if ($silver_invested >= $cost ) {
        // Pay the cost.
        $node->field_economy[$node->language][0]['value']++;
        $cost = _vaxia_invest_calculate_cost_to_improve($node->field_economy[$node->language][0]['value']);
        _character_sheet_update_xp($node->nid, $cost, 'spent_silver');
        _character_sheet_update_xp($node->nid, $cost*-1, 'invest_silver');
        $silver_invested = $silver_invested - $cost;
      }
    }
    node_save($node);
  }
}

/**
 * Validate callback for transfering XP().
 */
function _vaxia_transfer_xp_validate(&$form, &$form_state) {
  if ($form_state['values']['confirm'] != 'TRANSFER') {
    form_set_error('confirm', t('Transfer was not confirmed.'));
  }
}

/**
 * Submit callback for transfering XP().
 */
function _vaxia_transfer_xp_submit(&$form, &$form_state) {
  $old_character = node_load($form_state['values']['character']);
  $new_character = node_load($form_state['values']['target']);
  $workflow = variable_get('vaxia_transfer_workflow', '');
  if ($old_character && $new_character && !empty($workflow) && $old_character->nid != $new_character->nid) {
    // Grab XP for this character.
    $xp = $old_character->vaxia_unspent_xp + $old_character->vaxia_spent_xp;
    // Update the XP recorded for this character.
    _character_sheet_delete_xp($old_character->nid);
    _character_sheet_delete_log($old_character->nid);
    _vaxia_delete_type_tracking($old_character->nid);
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
 * Helper function, calculate money for economy on cron.
 */
function vaxia_character_sheet_cron() {
  $last_run = variable_get('vaxia_character_sheet_cron', 0);
  $yesterday = strtotime('-1 day');
  if ($last_run < $yesterday) {
    $last_run = variable_set('vaxia_character_sheet_cron', strtotime('now'));
    // On cron. Go through ALL character sheets. (BATCH API or TABLE?).
    // For each sheet, calculate money to add based on the economy level.
    // Using this query to make swure we don't load anything we really don't need to.
    // So only nodes that have an economy and exist.
    $sheets = db_query('SELECT n.nid, e.field_economy_value FROM {field_data_field_economy} e ' .
      'LEFT JOIN {node} n ON e.entity_id = n.nid AND e.revision_id = n.vid');
    foreach ($sheets as $node_data) {
      $node = node_load($node_data->nid);
      if (_character_sheet_is_enabled($node) && !_character_sheet_is_playable($node)) {
        $node = node_load($node->nid);
        $level = isset($node->field_economy[$node->language][0]['value']) ? $node->field_economy[$node->language][0]['value'] : 0;
        $silver = _character_sheet_get_xp($node->nid, 'silver');
        $silver_spent = _character_sheet_get_xp($node->nid, 'spent_silver');
        $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
        $available_silver = $silver - ($silver_spent + $silver_invested);
        $income = _vaxia_get_month_income($level);
        $daily = _vaxia_get_daily_pay($level);
        // If silver on hand > income reduce it by daily pay, else add it.
        if ($available_silver > $income) {
          // Reduce silver.
          _character_sheet_update_xp($node->nid, $daily * -1, 'silver');
        }
        else {
          // Add silver.
          _character_sheet_update_xp($node->nid, $daily, 'silver');
        }
      }
    }
  }
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
  return array('xp', 'bxp', 'rxp', 'kxp', 'silver');
}

/**
 * XP callback for adding XP to a node. Vaxia's wierd XP complexity.
 */
function _vaxia_add_xp($nid, $xp, $type) {
  $node = node_load($nid);
  $meaningful_types = _vaxia_meaningful_xp_types();
  if (!in_array($type, $meaningful_types)) {
    _vaxia_update_type_tracking(array('nid' => $nid, 'xp' => $xp, 'type' => $type)); // Add to the type given.
    $type = 'xp';
  }
  switch ($type) {
    case 'kxp':
      // Add wordcount xp.
      _character_sheet_update_xp($nid, $xp, 'kxp');
      $current_kxp = _character_sheet_get_xp($nid, 'kxp');
      // If we have enough to convert to other xp, do it.
      $kxp_ratio = variable_get('vaxia_kxp_ratio', 70);
      if ($current_kxp >= $kxp_ratio) {
        // Remove from kxp, add 1 to rxp.
        _character_sheet_update_xp($nid, $kxp_ratio * -1, 'kxp');
        _character_sheet_update_xp($nid, 1, 'rxp');
        // If we have banked xp, remove one from bxp and add spendable xp.
        $current_bxp = _character_sheet_get_xp($nid, 'bxp');
        if ($current_bxp > 0 ) {
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
        if ( ($current_rxp - $rxp_to_charge) > 0 ) {
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
    case 'silver':
      $message = t('%xp %type XP added to %title.', array('%xp' => $xp, '%type' => $type, '%title' => $node->title));
      _character_sheet_update_log($nid, 'xp', $message, $variables = array());
      // Silver is just added directly.
      _character_sheet_update_xp($nid, $xp, 'silver');
    break;
  }
}

/**
 * CRUD style DB function for vaxia_xp_type_totals.
 */
function _vaxia_create_type_tracking($tracking) {
  $tracking = (array) $tracking;
  $tracking_data = FALSE;
  if (isset($tracking['nid'])) {
    $tracking_data = _vaxia_get_type_tracking_by_nid_and_type($tracking['nid'], $tracking['type']);
  }
  if (!$tracking_data) {
    drupal_write_record('vaxia_xp_type_totals', $tracking);
  }
  else {
    _vaxia_update_type_tracking($tracking);
  }
}

/**
 * CRUD style DB function for vaxia_xp_type_totals.
 */
function _vaxia_get_type_tracking_by_nid($nid) {
  return db_query('SELECT * FROM {vaxia_xp_type_totals} WHERE nid = :nid', array(':nid' => $nid));
}

/**
 * CRUD style DB function for vaxia_xp_type_totals.
 */
function _vaxia_get_type_tracking_by_nid_and_type($nid, $type) {
  return db_query('SELECT * FROM {vaxia_xp_type_totals} WHERE nid = :nid AND type = :type',
    array(':nid' => $nid, ':type' => $type))->fetchObject();
}

/**
 * CRUD style DB function for vaxia_xp_type_totals.
 */
function _vaxia_update_type_tracking($tracking) {
  $tracking = (array) $tracking;
  $tracking_data = FALSE;
  if (isset($tracking['nid'])) {
    $tracking_data = _vaxia_get_type_tracking_by_nid_and_type($tracking['nid'], $tracking['type']);
  }
  if (!$tracking_data) {
    _vaxia_create_type_tracking($tracking);
  }
  else {
    $tracking_data = (array) $tracking_data;
    $tracking['xp'] = $tracking['xp'] + $tracking_data['xp'];
    drupal_write_record('vaxia_xp_type_totals', $tracking, array('nid', 'type'));
  }
}

/**
 * CRUD style DB function for vaxia_xp_type_totals.
 */
function _vaxia_delete_type_tracking($nid) {
  db_delete('vaxia_xp_type_totals')->condition('nid', $nid)->execute();
}
