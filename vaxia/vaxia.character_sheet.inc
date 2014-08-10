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
    $base_value = 1;
    while ($current < $value) {
      $current += $step;
      $total += $base_value * $cost_modifier;
      // Increment the value on the value *after* the ten's number.
      // The formula is one-off from a div by ten, min 1.
      if ($current > 10 && ($current % 10) == 0) {
        $base_value++;
      }
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
      ),
    ),
    'field_is_npc' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    'field_is_open_npc' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    'field_is_storyteller' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
    //field_background
    //field_personality
    //field_occupation
    //field_description
    //field_artwork_image
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
//      'costs' => array(
//        'xp_type' => 'xp',
//        'cost' => variable_get('vaxia_economy_cost', 1),
//        'step' => 1,
//        'value_callback' => '_vaxia_calculate_attribute_field_value',
//      ),
    ),
    'field_items' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET, // Disabled for all, we're using the CNR module to handle this.
        'other' => DISABLE_SHEET, // So we don't want this value getting meddled with.
      ),
    ),
    'field_languages' => array(
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
    // Temporary hide.
    'field_skill_review' => array(
      'permissions' => array(
        'owner' => DISABLE_SHEET,
      ),
    ),
  );
}

/**
 * Implelents node_form_alter for nodes.
 * Character sheet, field level controls for players and moderators according to character sheet defs.
 */
function vaxia_form_node_form_alter(&$form, &$form_state, $form_id) {
  global $user;
  $node = $form['#node'];
  $lang = $form['language']['#value'];
  // Only on a character sheet that isn't yet enabled.
  if (_character_sheet_is_enabled($node) && !_character_sheet_is_playable($node)) {
    // Hide these fields for new characters.
    $disabled = array('field_economy', 'field_npcs', 'field_items', 'field_property',
      'field_sh_notes', 'field_sh_private_notes');
    foreach ($disabled as $field_name) {
      $form[$field_name]['#access'] = FALSE;
      $form[$field_name][$lang][0]['#access'] = FALSE;
      if (isset($form[$field_name][$lang][0]['#default_value'])) {
        $form[$field_name][$lang][0]['#value'] = $form[$field_name][$lang][0]['#default_value'];
      }
      if (isset($form[$field_name][$lang]['add_more'])) {
        $form[$field_name][$lang]['add_more']['#access'] = FALSE;
      }
    }
    // If the owner, and new, and user isn't an SH sort.
    if ($user->uid == $node->uid && !user_access('create NPCs') && !user_access('create open NPCs')) {
      // Stat option changes.
      $reduced = array('field_life', 'field_endurance', 'field_strength', 'field_dexterity',
        'field_intelligence', 'field_spirituality', 'field_charisma');
      $low = variable_get('vaxia_pc_stat_min', 10);
      $high = variable_get('vaxia_pc_stat_max', 40);
      $options = array_combine(range($low, $high), range($low, $high));
      foreach ($reduced as $field_name) {
        if (isset($form[$field_name][$lang][0]['value']['#type'])
          && $form[$field_name][$lang][0]['value']['#type'] == 'textfield') {
          $form[$field_name][$lang][0]['value']['#type'] = 'select';
          $form[$field_name][$lang][0]['value']['#size'] = 1;
          $form[$field_name][$lang][0]['value']['#options'] = $options;
        }
      }
      // Skill option changes.
      $low = variable_get('vaxia_pc_skill_min', 5);
      $high = variable_get('vaxia_pc_skill_max', 40);
      $vals = range($low, $high);
      array_unshift($vals, 0);
      $options = array_combine($vals, $vals);
      foreach ($form['field_skill'][$lang] as $delta => $field) {
        if (is_array($field) && isset($form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#type'])
          && $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#type'] == 'textfield') {
          $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#type'] = 'select';
          $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#size'] = 1;
          $form['field_skill'][$lang][$delta]['field_skill_value'][$lang][0]['value']['#options'] = $options;
        }
      }
      // Condition change.
      $min_hxp = variable_get('vaxia_other_min_hxp', 1);
      $user = user_load($user->uid);
      $hxp = $user->field_xp_over_time['und'][0]['value'];
      if ($hxp < $min_hxp) {
          $options = $form['field_race'][$lang]['#options'];
          unset($form['field_race'][$lang]['#options']['other']);
          $form['field_conditions']['#disabled'] = TRUE;
      }
    }

    // If in submit / review mode then really remove access.
    if ($user->uid == $node->uid && isset($node->workflow_state_name) && $node->workflow_state_name == 'submitted') {
      $form['#disabled'] = TRUE;
    }

    // If has xp, and in draft - then disable. We only want to see you submit this character, it's been pulled back from cold storage.
    if (!empty($node->nid)) {
      $has_xp = _character_sheet_get_xp($node->nid, 'xp');
      if ($user->uid == $node->uid && isset($node->workflow_state_name) && $node->workflow_state_name == 'draft' && $has_xp != 0) {
        $form['#disabled'] = TRUE;
      }
    }
  }

  // Only do this if this is a character sheet in play.
  if (_character_sheet_is_enabled($node) && _character_sheet_is_playable($node)) {
    // Remove skill's remove button and add more button IIF owner viewing playable.
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
    // Disable the private note if you can't edit it. Can't edit your own.
    if (!($user->uid == $node->uid || user_access('edit SH notes'))) {
      $form['field_sh_notes'][$lang][0]['#disabled'] = TRUE;
      if (isset($form['field_sh_notes'][$lang][0]['#default_value'])) {
        $form['field_sh_notes'][$lang][0]['#value'] = $form['field_sh_notes'][$lang][0]['#default_value'];
      }
    }
    // Hide the private note if you can't edit it. Can't edit your own.
    if (!($user->uid != $node->uid && user_access('edit private SH notes'))) {
      $form['field_sh_private_notes'][$lang][0]['#access'] = FALSE;
      if (isset($form['field_sh_private_notes'][$lang][0]['#default_value'])) {
        $form['field_sh_private_notes'][$lang][0]['#value'] = $form['field_sh_private_notes'][$lang][0]['#default_value'];
      }
    }
  }

  // Do this for all forms.
  // Remove the NPC / ONPC buttons if the user is not permissioned.
  $sh_char = _vaxia_get_SH_characters($node->uid);
  if (!user_access('create SHs') || !empty($sh_char)) {
    $form['field_is_storyteller'][$lang]['#access'] = FALSE;
  }
  if (!user_access('create NPCs')) {
    $form['field_is_npc'][$lang]['#access'] = FALSE;
    $form['field_is_npc'][$lang]['#value'] =
      isset($form['field_is_npc'][$lang]['#default_value']) ? $form['field_is_npc'][$lang]['#default_value'] : FALSE;
  }
  if (!user_access('create open NPCs')) {
    $form['field_is_open_npc'][$lang]['#access'] = FALSE;
    $form['field_is_open_npc'][$lang]['#value'] =
      isset($form['field_is_open_npc'][$lang]['#default_value']) ? $form['field_is_open_npc'][$lang]['#default_value'] : FALSE;
  }
}

/**
 * Implements hook_character_sheet_presave_sheet().
 */
function vaxia_character_sheet_presave_sheet($node) {
  if ($node->is_new || !empty($node->revision)) {
    if (empty($node->field_economy[$node->language][0]['value'])) {
      $node->field_economy[$node->language][0]['value'] = variable_get('vaxia_starting_econ', 25);
    }
  }
}

/**
 * Implements hook_character_sheet_validate_sheet().
 */
function vaxia_character_sheet_validate_sheet($node, $old_node, $form, $form_state) {
  // Don't check this if the sheet is marked as an NPC.
  if (isset($node->field_is_npc[$node->language][0]['value']) && $node->field_is_npc[$node->language][0]['value'] == TRUE) {
    return FALSE;
  }
  if (isset($node->field_is_open_npc[$node->language][0]['value']) && $node->field_is_open_npc[$node->language][0]['value'] == TRUE) {
    return FALSE;
  }
  // Run this only for draft nodes, to cap out at 300 per field.
  // Check if the sheet has been finalized.
  $has_xp = _character_sheet_get_xp($node->nid, 'xp');
  if ( !isset($old_node->workflow) || // Brand new node.
    !isset($node->workflow) || // Current node has no workflow.
    (isset($node->workflow) && ($node->workflow == variable_get('vaxia_draft_workflow', '')) && $has_xp == 0) // Created node but still in draft, with no XP spent on it.
  ) {
    // Total the value of points against starting points flat.
    $available_xp = variable_get('vaxia_starting_points', 300);
    $stat_max = variable_get('vaxia_pc_stat_spend_cap', 225);
    $max_max = variable_get('vaxia_pc_stat_high_cap', 4);
    $zero_max = variable_get('vaxia_pc_stat_zero_cap', 1);
    $high_skill = variable_get('vaxia_pc_skill_max', 40);
    $high_stat = variable_get('vaxia_pc_stat_max', 40);
    // Field info.
    $fields_info = _character_sheet_get_field_info($node);
    // Gather data on spending.
    $spent_xp = array(); // XP by type.
    $spent_on_stats = 0; // Amount spent on stats.
    $spent_on_skills = 0; // Amount spent on skills.
    $maxed_values = 0; // Number of maxed values.
    $zero_skills = 0; // Number of zeroth sklls.
    // Loop through field data.
    $node_data = (array) $node;
    foreach ($fields_info as $field_key => $field_info) {
      if (isset($field_info['costs']['xp_type'])) {
        // Calculate cost difference.
        $totals = _vaxia_get_field_value_flat($node, $field_key, $fields_info);
        $cost = $totals['total'];
        $xp_type = $field_info['costs']['xp_type'];
        // Track for stat spending.
        if ($field_key != 'field_skill_value') {
          $spent_on_stats += $cost;
          if ($cost >= $high_stat) {
            $maxed_values++;
          }
        }
        else {
          $spent_on_skills += $cost;
          foreach(element_children($node_data) as $this_field) {
            if ($this_field == 'field_skill') {
              $lang = $node->language;
              $skill_data = $node->$this_field;
              foreach ($skill_data as $skills) {
                foreach ($skills as $delta => $skill_data) {
                  // Max sills.
                  if ($skill_data['field_skill_value'][$lang][0]['value'] >= $high_skill) {
                    $maxed_values++;
                  }
                  // Zero skills.
                  if (is_array($skill_data) && $skill_data['field_skill_value'][$lang][0]['value'] == 0) {
                    $zero_skills++;
                  }
                }
              }
            }
          }
        }
        $spent_xp[$xp_type] = isset($spent_xp[$xp_type]) ? $spent_xp[$xp_type] + $cost : $cost;
        // Over spent XP.
        if ($spent_xp[$xp_type] > $available_xp) {
          return $error = array('field' => '', 'message' => 
            t('You have spent %spent_xp points out of %available_xp available points. Please revise your sheet.',
              array('%spent_xp' => $spent_xp[$xp_type], '%available_xp' => $available_xp)));
        }
      }
    }
    // Under spent XP.
    if ($spent_xp[$xp_type] < $available_xp) {
      return $error = array('field' => '', 'message' => 
        t('You have spent %spent_xp points out of %available_xp available points. You have more points to spend. Please revise your sheet.',
          array('%spent_xp' => $spent_xp[$xp_type], '%available_xp' => $available_xp)));    
    }
    // Focused too much on stats.
    if ($spent_on_stats > $stat_max) {
      return $error = array('field' => '', 'message' => 
        t('You have spent %spent_on_stats points on stats. You cannot spend more than %stat_max on stats. Please revise your sheet.',
          array('%spent_on_stats' => $spent_on_stats, '%stat_max' => $stat_max)));    
    }
    // Too many maxes.
    if ($maxed_values > $max_max) {
      return $error = array('field' => '', 'message' => 
        t('You have %maxed_values maximized values. You cannot have more than %max_max. Please revise your sheet.',
          array('%maxed_values' => $maxed_values, '%max_max' => $max_max)));    
    }
    // Too many zero skills.
    if ($zero_skills > $zero_max) {
      return $error = array('field' => '', 'message' => 
        t('You have %zero_skills skills at zero. You can only have one skill at zero. Please revise your sheet.',
          array('%zero_skills' => $zero_skills, '%zero_max' => $zero_max)));    
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
  $form['settings']['vaxia_clean_dblog'] = array(
    '#title' => 'Clear watchdog',
    '#description' => 'Clear non-essential watchdog logs (notice and below) on every cron run.',
    '#type' => 'checkbox',
    '#default_value' => variable_get('vaxia_clean_dblog', TRUE),
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
      "good\nevil\nsmart\ncharismatic\nwarrior\nmage\nrogue"),
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
    '#multiple' => TRUE,
    '#size' => 15,
    '#default_value' => variable_get('vaxia_limbo_room', array()),
  );
  $form['settings']['vaxia_display_dice'] = array(
    '#title' => 'Dice rows to show',
    '#description' => 'The number of dicerows to show in dice interface.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_display_dice', 6),
  );
  $form['settings']['vaxia_dice_roller_stacking'] = array(
    '#title' => 'Allow items to stack for dice rolling.',
    '#type' => 'checkbox',
    '#default_value' => variable_get('vaxia_dice_roller_stacking', TRUE),
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
    '#description' => 'Percent of XP when transferred that goes to XP.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_transfer_percentage', 0.75),
  );
  $form['point_cost']['vaxia_transfer_cap_on'] = array(
    '#title' => 'Cap XP to be transferred?',
    '#type' => 'checkbox',
    '#default_value' => variable_get('vaxia_transfer_cap_on', TRUE),
  );
  $form['point_cost']['vaxia_transfer_cap'] = array(
    '#title' => 'Cap of XP',
    '#description' => 'Amount of total XP that can be transferred, anything more is lost.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_transfer_cap', 300),
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
  $form['char_rules'] = array(
    '#title' => 'Character creation rules',
    '#type' => 'fieldset',
  );
  $form['char_rules']['warn'] = array(
    '#title' => 'Warn creation',
    '#type' => 'fieldset',
  );
  $form['char_rules']['warn']['vaxia_pc_skill_warn'] = array(
    '#title' => 'Skill min warn',
    '#description' => 'Warning on min skill value for a PC.',
    '#type' => 'textfield',
    '#default_value' =>  variable_get('vaxia_pc_skill_warn', 10),
  );
  $form['char_rules']['warn']['vaxia_pc_stat_spend_warn'] = array(
    '#title' => 'Stat spending warn',
    '#description' => 'Warning spent on stats for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_stat_spend_warn', 210),
  );
  $form['char_rules']['warn']['vaxia_pc_stat_high_warn'] = array(
    '#title' => 'Max caps warn',
    '#description' => 'Warning on number of allowed max values for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_stat_high_warn', 3),
  );
  $form['char_rules']['warn']['vaxia_pc_stat_min_warn'] = array(
    '#title' => 'Max mins warn',
    '#description' => 'Warning on number of allowed min values for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_stat_min_warn', 2),
  );
  $form['char_rules']['warn']['vaxia_pc_skill_ave_warn'] = array(
    '#title' => 'Skill average warn',
    '#description' => 'Warning on average skill values for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_skill_ave_warn', 20),
  );
  $form['char_rules']['block'] = array(
    '#title' => 'Block creation',
    '#type' => 'fieldset',
  );
  $form['char_rules']['block']['vaxia_pc_skill_min'] = array(
    '#title' => 'Skill min',
    '#description' => 'Minimum allowed skill value for a PC.',
    '#type' => 'textfield',
    '#default_value' =>  variable_get('vaxia_pc_skill_min', 5),
  );
  $form['char_rules']['block']['vaxia_pc_skill_max'] = array(
    '#title' => 'Skill min',
    '#description' => 'Minimum allowed skill value for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_skill_max', 40),
  );
  $form['char_rules']['block']['vaxia_pc_stat_min'] = array(
    '#title' => 'Stat min',
    '#description' => 'Minimum allowed stat value for a PC.',
    '#type' => 'textfield',
    '#default_value' =>  variable_get('vaxia_pc_stat_min', 10),
  );
  $form['char_rules']['block']['vaxia_pc_stat_max'] = array(
    '#title' => 'Stat min',
    '#description' => 'Minimum allowed stat value for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_stat_max', 40),
  );
  $form['char_rules']['block']['vaxia_pc_stat_spend_cap'] = array(
    '#title' => 'Stat spending cap',
    '#description' => 'Maximum allowed to be spent on stats for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_stat_spend_cap', 225),
  );
  $form['char_rules']['block']['vaxia_pc_stat_high_cap'] = array(
    '#title' => 'Max caps',
    '#description' => 'Maximum number of allowed max values for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_stat_high_cap', 4),
  );
  $form['char_rules']['block']['vaxia_pc_stat_zero_cap'] = array(
    '#title' => 'Zero caps',
    '#description' => 'Maximum number of zero skills for a PC.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_pc_stat_zero_cap', 1),
  );
  $form['char_rules']['block']['vaxia_other_min_hxp'] = array(
    '#title' => 'Min HXP for others',
    '#description' => 'Min HXP to be able to select Other and Conditions.',
    '#type' => 'textfield',
    '#default_value' => variable_get('vaxia_other_min_hxp', 1),
  );
  return $form;
}

/**
 * Implements hook_form_FORMID_alter().
 */
function vaxia_form__character_sheet_xp_form_alter(&$form, $form_state) {
  // Load the node.
  $node = node_load($form['node_info']['nid']['#value']);
  $xp = _character_sheet_get_xp($node->nid, 'xp');
  $spent_xp = _character_sheet_get_xp($node->nid, 'spent_xp');
  $xp_totals = array(t('You have %xp xp available to spend.',
    array('%xp' => $xp - $spent_xp )));
  $form['node_info']['xp'] = array(
    '#type' => 'item',
    '#title' => t('XP totals'),
    '#markup' => theme('item_list', array('items' => $xp_totals)),
  );
  $form['node_info']['xp_details'] = array(
    '#type' => 'item',
    '#title' => t('XP details'),
    '#markup' => theme('item_list', array('items' => _vaxia_xp_display($node))),
  );

  $form['node_info']['xp_types'] = array(
    '#type' => 'item',
    '#title' => t('XP types'),
    '#markup' => theme('item_list', array('items' => _vaxia_xp_display_types($node))),
  );

  // Add the economy form for owned forms.
  global $user;
  $level = isset($node->field_economy[$node->language][0]['value']) ? $node->field_economy[$node->language][0]['value'] : 0;
  $econ[] = t('Your current economy level is %level.', array('%level' => $level));
  $income = (float) _vaxia_get_month_income($level);
  $econ[] = t('Your monthly disposable income is %income.', array('%income' => number_format($income, 2)));
  $pay = _vaxia_get_daily_pay($level);
  $econ[] = t('You get paid %pay a day.', array('%pay' => number_format($pay, 2)));
  $form['node_info']['economy'] = array(
    '#type' => 'item',
    '#title' => t('Economy'),
    '#markup' => theme('item_list', array('items' => $econ)),
  );
  $silver = _character_sheet_get_xp($node->nid, 'silver');
  $silver_spent = _character_sheet_get_xp($node->nid, 'spent_silver');
  $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
  $available_silver = $silver - ($silver_spent + $silver_invested);
  $cost = _vaxia_invest_calculate_cost_to_improve($level);
  $invest = $cost - $silver_invested;
  // Invested silver is spent_silver, adjusted to show the difference between spent and the cost of the current economy level.
  $desc = t('You have %available silver available to invest. You need to invest %invest more silver before you will reach %level economy. Invested so far %so_far.',
    array('%available' => floor($available_silver), '%invest' => ceil($invest), '%level' => $level + 1, '%so_far' => $silver_invested));

  global $user;
  if ($user->uid == $node->uid) {
    if ($available_silver >= 1) {
      $available_silver = floor($available_silver);
      $invest = 0;
      // Figure out how many given current silver you can add.
      $form['node_info']['invest_silver'] = array(
        '#title' => t('Invest silver'),
        '#type' => 'textfield',
        '#description' => $desc,
        '#size' => 15,
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
  }

  // Add additional options for xp IIF allowed.
  if (isset($form['add_xp']) && $node->field_is_storyteller[$node->language][0]['value'] == 0 
    && $node->field_is_npc[$node->language][0]['value'] == 0 && $node->field_is_open_npc[$node->language][0]['value'] == 0) {
    $form['add_xp']['xp']['#options'] = range(0, 500);
    $xp_types = array();
    foreach(explode("\n", variable_get('vaxia_xp_types', "good\nevil\nsmart\nclever\nwarrior\nmage\nrogue")) as $type) {
      $type = trim($type);
      $xp_types[$type] = $type;
    }
    $form['add_xp']['type']['#options'] = array_merge($xp_types, $form['add_xp']['type']['#options']);
    $form['add_xp']['type']['#options'] = array_diff($form['add_xp']['type']['#options'], array('silver'));
    $form['add_xp']['type']['#default_value'] = 'xp';
    // Add silver as it's own thing.
    $form['add_xp']['add_silver'] = array(
      '#type' => 'textfield',
      '#title' => t('Add silver'),
      '#size' => 6,
      '#description' => t('Adding a negative value will subtract silver. First from available silver, then from the economy level, reducing economy.'),
    );
    $form['add_xp']['add_silver_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Add silver'),
      '#validate' => array('add_silver_validation_form'),
      '#submit' => array('add_silver_submit_form'),
    );
  }
  else {
    $form['add_xp']['#access'] = FALSE;
  }

  // And add the transfer options. But only if this sheet is a storyteller sheet.
  $rxp = _character_sheet_get_xp($node->nid, 'rxp');
  if ($rxp > 0 &&
    ($node->field_is_storyteller[$node->language][0]['value'] == 1 || $node->field_is_npc[$node->language][0]['value'] == 1 ||
    $node->field_is_open_npc[$node->language][0]['value'] == 1) &&
    ($node->uid == $user->uid && user_access('transfer rxp') || user_access('transfer others sheets'))
  ) {
    $form['transfer_rxp'] = array(
      '#type' => 'fieldset',
      '#title' => 'Transfer RXP only',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['transfer_rxp']['rxp_character'] = array(
      '#type' => 'hidden',
      '#value' => $node->nid,
    );
    $form['transfer_rxp']['rxp_target'] = array(
      '#type' => 'textfield',
      '#title' => t('Transfer to'),
      '#description' => t('Transfer RXP to this character sheet.'),
      '#autocomplete_path' => 'vaxia/transfer_xp',
    );
    $form['transfer_rxp']['rxp_amount'] = array(
      '#type' => 'select',
      '#title' => t('Transfer amount'),
      '#options' => array_combine(range(0, $rxp), range(0, $rxp)),
      '#description' => t('Transfer this much RXP to the selected character sheet. This will not disable this character.'),
    );
    $form['transfer_rxp']['rxp_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Transfer character RXP'),
      '#submit' => array('_vaxia_transfer_rxp_submit'),
    );
  }

  // And add the transfer options.
  $setting = variable_get('vaxia_transfer_workflow', '');
  if (!empty($setting) && ($node->uid == $user->uid || user_access('transfer others sheets')) ) {
    $form['transfer'] = array(
      '#type' => 'fieldset',
      '#title' => 'Transfer all HXP',
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
    $percent = variable_get('vaxia_transfer_percentage', 0.75);
    $xp_amount = _character_sheet_get_xp($node->nid, 'spent_xp');
    // Amount to transfer.
    $new_xp = floor($xp_amount * variable_get('vaxia_transfer_percentage', 0.75));
    $max = '';
    // Calc amounts.
    if (variable_get('vaxia_transfer_cap_on', TRUE)) {
      $cap = variable_get('vaxia_transfer_cap', 300);
      $max = ' max ' . $cap;
      if ($new_xp > $cap) {
        $new_xp = $cap;
      }
    }
    $percent = $percent * 100;
    $form['transfer']['alert']['details'] = array(
      '#type' => 'item',
      '#markup' => t('When transferred, %percent of the XP (%new_xp out of %xp%max) will be placed on the selected character sheet. If the character is new, ' .
        'the XP will be immediately spendable, otherwise it will be added to the character bank. Banked and other XP for this sheet will not be transferred. ' .
        'Once transferred, this character will be flagged as inactive and will no longer be available to post. This cannot be undone without intervention by an ' .
        'administrator so be sure you mean to do this.',
        array('%percent' => $percent . '%', '%new_xp' => floor($new_xp), '%xp' => floor($xp_amount), '%max' => $max)),
    );
    $form['transfer']['confirm']['character'] = array(
      '#type' => 'hidden',
      '#value' => $node->nid,
    );
    $form['transfer']['confirm']['target'] = array(
      '#type' => 'textfield',
      '#title' => t('Transfer to'),
      '#description' => t('Transfer HXP to this character sheet.'),
      '#autocomplete_path' => 'vaxia/transfer_xp',
    );
    $xp = _character_sheet_get_xp($node->nid, 'spent_xp');
    $form['transfer']['confirm']['amount'] = array(
      '#type' => 'hidden',
      '#value' => $xp,
      '#description' => t('Up to ' . $xp . ' HXP is available to be transferred.'),
    );
    $form['transfer']['confirm']['confirm'] = array(
      '#type' => 'textfield',
      '#title' => t('Confirm'),
      '#description' => t('If you really mean to do this, please type "TRANSFER" '.
        'into this textbox, in all caps.'),
    );
    $form['transfer']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Transfer ALL character HXP'),
      '#validate' => array('_vaxia_transfer_xp_validate'),
      '#submit' => array('_vaxia_transfer_xp_submit'),
    );
  }
}

/**
 * Menu callback handler for autocomplete.
 */
function _vaxia_transfer_xp_autocomplete($character_name = '') {
  $matches = array();
  global $user;
  $results = db_query('SELECT n.nid, n.title, u.name FROM {node} n ' .
    'LEFT JOIN {users} u ON n.uid = u.uid ' .
    'WHERE n.type LIKE \'character_sheet\' AND n.status = 1 ' .
    'AND n.uid = :uid AND n.title LIKE :name', array(':uid' => $user->uid, ':name' => $character_name . '%'));
  foreach ($results as $character) {
    $character_label = $character->title . ' (' . $character->name. ') [nid:' . $character->nid . ']';
    $matches[ check_plain($character_label) ] = check_plain($character_label);
  }
  drupal_json_output($matches);
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
  if (isset($form['owner_name'])) {
     $form['change_ownership']['owner_name'] = $form['owner_name'];
    unset($form['owner_name']);
  }
  if (isset($form['submit'])) {
    $form['change_ownership']['submit'] = $form['submit'];
    unset($form['submit']);
  }
  if (isset($form['character_sheet_log'])) {
    unset($form['character_sheet_log']);
  }
}

/**
 * Helper function, gather xp for display.
 */
function _vaxia_xp_display($node) {
  $historical = _character_sheet_get_xp($node->nid, 'xp');
  $spent = _character_sheet_get_xp($node->nid, 'spent_xp');
  $xps['historical_xp'] = t('Earned %historical historical xp.', array('%historical' => $spent));
  $spent = _character_sheet_get_xp($node->nid, 'spent_xp');
  $xps['spent_xp'] = t('Spent %spent out of %xp xp.', array('%spent' => $spent, '%xp' => $historical));
  $banked = _character_sheet_get_xp($node->nid, 'bxp');
  $xps['bxp'] = t('Banked %banked bxp.', array('%banked' => $banked));
  $roleplayed = _character_sheet_get_xp($node->nid, 'rxp');
  $xps['rxp'] = t('Roleplayed %roleplayed rxp.', array('%roleplayed' => $roleplayed));
  $posted = _character_sheet_get_xp($node->nid, 'kxp');
  $xps['kxp'] = t('Posted %posted kxp.', array('%posted' => $posted));
  // Figure out silver math.
  $silver = _character_sheet_get_xp($node->nid, 'silver');
  $silver_spent = _character_sheet_get_xp($node->nid, 'spent_silver');
  $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
  $available_silver = $silver - ($silver_spent + $silver_invested);
  $xps['available_silver'] = t('Have %posted silver.', array('%posted' => number_format($available_silver, 2)));
  return $xps;
}

/**
 * Helper function, gather xp type records for display.
 */
function _vaxia_xp_display_types($node) {
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
 * Helper function, figure out how much it costs to remove economy by one.
 */
function _vaxia_invest_calculate_cost_to_remove($economy) {
  // Given an economy value, figure out how much silver it would take to increase it by one.
  $total_worth = _vaxia_calculate_worth($economy);
  $economy--;
  $new_worth = _vaxia_calculate_worth($economy);
  return $total_worth - $new_worth;
}

/**
 * Helepr function, given an economy level, figure out how much income in a month.
 */
function _vaxia_get_month_income($economy) {
  $total_worth = _vaxia_calculate_worth($economy);
  $income = (($total_worth) * (3.5 / 100)) + (0.05);
  return $income;
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
  $node = node_load($nid);
  $silver = _character_sheet_get_xp($node->nid, 'silver');
  $silver_spent = _character_sheet_get_xp($node->nid, 'spent_silver');
  $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
  $available_silver = $silver - ($silver_spent + $silver_invested);
  // Act.
  if (is_numeric($invest) && $invest > 0 && $invest <= $available_silver) {
    // Figure out how many to add based on added silver.
    $cost = _vaxia_invest_calculate_cost_to_improve($node->field_economy[$node->language][0]['value']);
    $silver_invested = _character_sheet_get_xp($node->nid, 'invest_silver');
    $silver_to_invest = 0;
    while ($invest > 0) {
      $invest--;
      $silver_invested++;
      $silver_to_invest++;
      if ($silver_invested >= $cost ) {
        // Pay the investment.
        $node->field_economy[$node->language][0]['value']++;
        _character_sheet_update_xp($node->nid, $silver_invested, 'spent_silver');
        // Remove silver invested - back to zero.
        $current_invest = _character_sheet_get_xp($node->nid, 'invest_silver');
        _character_sheet_update_xp($node->nid, $current_invest * -1, 'invest_silver');
        $silver_to_invest = 0;
        $silver_invested = 0;
      }
    }
    _character_sheet_update_xp($node->nid, $silver_to_invest, 'invest_silver');
    node_save($node);
  }
}

/**
 * Form validation handler for silver.
 */
function add_silver_validation_form(&$form, &$form_state) {
  $add_silver = $form_state['values']['add_silver'];
  if (empty($add_silver)) {
    form_set_error('add_silver', t('Empty value given for silver.'));
  }
  if (!is_numeric($add_silver)) {
    form_set_error('add_silver', t('Silver given is not a numeric value'));
  }
}

/**
 * Form submit handler for silver.
 */
function add_silver_submit_form(&$form, &$form_state) {
  global $user;
  $nid = $form_state['values']['nid'];
  $character = node_load($nid);
  $add_silver = $form_state['values']['add_silver'];
  // If positive, add silver as per standard XP.
  if ($add_silver > 0) {
    _character_sheet_update_xp($nid, $add_silver, 'silver');
    drupal_set_message(t('%add_silver silver has been added to this sheet.', array('%add_silver' => $add_silver)));
	$message = t('@xp silver added to @title by @user.',
		array('@xp' => $add_silver, '@title' => $character->title, '@user' => $user->name));
	_character_sheet_update_log($nid, 'silver', $message);
    return;
  }
  // If negative. First check if we can just subtract off of given XP.
  if ($add_silver < 0) {
    _add_silver_submit_form_remove_silver($nid, $add_silver);
    drupal_set_message(t('%add_silver silver has been removed from this sheet.',
      array('%add_silver' => $add_silver)));
	$message = t('@xp silver removed from @title by @user.',
		array('@xp' => $add_silver, '@title' => $character->title, '@user' => $user->name));
	_character_sheet_update_log($character, 'silver', $message);
  }
}

/**
 * Recursive call to convert silver down.
 */
function _add_silver_submit_form_remove_silver($nid, $add_silver) {
  $silver = _character_sheet_get_xp($nid, 'silver');
  $silver_spent = _character_sheet_get_xp($nid, 'spent_silver');
  $silver_invested = _character_sheet_get_xp($nid, 'invest_silver');
  $available_silver = $silver - ($silver_spent + $silver_invested);
  //If yes, do so.
  if ($available_silver >= -1 * $add_silver) {
    _character_sheet_update_xp($nid, $add_silver, 'silver');
    return;
  }
  // If not, subtract to zero and start digging into the economy.
  if ($available_silver <  -1 * $add_silver) {
    _character_sheet_update_xp($nid, $available_silver * -1, 'silver');
    // Subtract the silver you've accounted for already.
    $add_silver += $available_silver;
    // Then convert one economy level and subtract until all silver is removed.
    if ($add_silver < 0) {
      // Convert a level, and add to silver.
      $node = node_load($nid);
      $economy = isset($node->field_economy[$node->language][0]['value']) ?
        $node->field_economy[$node->language][0]['value'] : 0;
      $converted_silver = _vaxia_invest_calculate_cost_to_remove($economy);
      _character_sheet_update_xp($nid, $converted_silver, 'silver');
      $message = t('Converted economy from level %economy to %less_economy, for %converted silver.',
        array('%economy' => $economy, '%less_economy' => $economy - 1,
        '%converted' => number_format($converted_silver, 2)));
      _character_sheet_update_log($node->nid, 'silver', $message);
      drupal_set_message($message);
      // Remove econmy.
      $economy--;
      $node->field_economy[$node->language][0]['value'] = $economy;
      node_save($node);
      // And repeat for the next debt.
      // NOTE: RECURSION.
      _add_silver_submit_form_remove_silver($nid, $add_silver);
    }
  }
}


/**
 * Validate callback for transfering XP().
 */
function _vaxia_transfer_xp_validate(&$form, &$form_state) {
  if ($form_state['values']['confirm'] != 'TRANSFER') {
    form_set_error('confirm', t('Transfer was not confirmed.'));
  }
  // Sanity check
  if (!is_numeric(trim($form_state['values']['amount'])) || $form_state['values']['amount'] < 1) {
    form_set_error('amount', t('Amount to transfer makes no sense.'));
  }
}

/**
 * Submit callback for transfering XP().
 */
function _vaxia_transfer_xp_submit(&$form, &$form_state) {
  $target_nid = _vaxia_transfer_xp_autocomplete_get_nid($form_state['values']['target']);
  if ($target_nid != FALSE) {
    $old_character = node_load($form_state['values']['character']);
    $new_character = node_load($target_nid);
    $workflow = variable_get('vaxia_transfer_workflow', '');
    if (!empty($old_character) && !empty($new_character) && !empty($workflow) && $old_character->nid != $new_character->nid) {
      // Grab XP for this character.
      $full_xp = _character_sheet_get_xp($old_character->nid, 'spent_xp');
      $xp = $form_state['values']['amount'];
      // Check in case user entered more than they have.
      if ($xp > $full_xp) {
        $xp = $full_xp;
      }
      // Amount to transfer.
      $new_xp = floor($xp * variable_get('vaxia_transfer_percentage', 0.75));
      $max = '';
      // Now add XP to the new character.
      if (variable_get('vaxia_transfer_cap_on', TRUE)) {
        $cap = variable_get('vaxia_transfer_cap', 300);
        $max = ' max ' . $cap;
        if ($new_xp > $cap) {
          $new_xp = $cap;
        }
      }
      // Record transfer
      watchdog('vaxia', 'Transfering %new_xp XP out of %xp%max from #@nid @title to #@nid2 @title2',
        array('%new_xp' => $new_xp, '%xp' => $xp, '%max' => $max, '@nid' => $old_character->nid,
        '@title' => $old_character->title, '@nid2' => $new_character->nid, '@title2' => $new_character->title));
      drupal_set_message(t('Transfering %new_xp XP out of %xp%max from #@nid @title to #@nid2 @title2',
        array('%new_xp' => $new_xp, '%xp' => $xp, '%max' => $max, '@nid' => $old_character->nid,
        '@title' => $old_character->title, '@nid2' => $new_character->nid, '@title2' => $new_character->title)));
      // New character?
      $new_spent = _character_sheet_get_xp($new_character->nid, 'spent_xp');
      if ($new_spent > 5) { // 5 or under for spent xp limits for transfer.
        _character_sheet_update_xp($new_character->nid, $new_xp, 'bxp');
      }
      else {
        _character_sheet_update_xp($new_character->nid, $new_xp, 'xp');
      }
      // Remove existing XP amount from both xp and spent xp (to keep balances).
      _character_sheet_update_xp($old_character->nid, $xp, 'spent_xp');
      _character_sheet_update_xp($old_character->nid, $xp, 'xp');
      // Logs and cleanup.
      $message = t('@xp transfer xp added to @title by system.',
        array('@xp' => $new_xp, '@title' => $new_character->title));
      _character_sheet_update_log($new_character->nid, 'transfer', $message);
      if ($full_xp <= $xp) {
        // Clear records for the XP recorded for the old character.
        _character_sheet_delete_xp($old_character->nid);
        _character_sheet_delete_log($old_character->nid);
        _vaxia_delete_type_tracking($old_character->nid);
        // And update the original character status.
        workflow_execute_transition($old_character, $workflow, t('Transferred all XP to a new character'), TRUE);
      }
    }
  }
  else {
    watchdog('vaxia', 'Unable to locate selected character. Transfer not completed.');
    drupal_set_message('Unable to locate selected character. Transfer not completed.', 'error');
  }
}

/**
 * Submit callback for transfering RXP().
 */
function _vaxia_transfer_rxp_submit(&$form, &$form_state) {
  $target_nid = _vaxia_transfer_xp_autocomplete_get_nid($form_state['values']['rxp_target']);
  if ($target_nid != FALSE) {
    $old_character = node_load($form_state['values']['rxp_character']);
    $new_character = node_load($target_nid);
    if (!empty($old_character) && !empty($new_character) && $old_character->nid != $new_character->nid) {
      $amount = $form_state['values']['rxp_amount'];
      // Transfer RXP.
      watchdog('vaxia', 'Transfering RXP (@amount) from #@nid @title to #@nid2 @title2',
        array('@amount' => $amount, '@nid' => $old_character->nid, '@title' => $old_character->title,
        '@nid2' => $new_character->nid, '@title2' => $new_character->title));
      drupal_set_message(t('Transfering RXP (@amount) from #@nid @title to #@nid2 @title2',
        array('@amount' => $amount, '@nid' => $old_character->nid, '@title' => $old_character->title,
        '@nid2' => $new_character->nid, '@title2' => $new_character->title)));
      _character_sheet_update_xp($old_character->nid, $amount * -1, 'rxp');
      _character_sheet_update_xp($new_character->nid, $amount, 'rxp'); // Transfer as RXP to be unlocked.
      // Transfer normal XP.
      watchdog('vaxia', 'Transfering XP (@amount) from #@nid @title to #@nid2 @title2',
        array('@amount' => $amount, '@nid' => $old_character->nid, '@title' => $old_character->title,
        '@nid2' => $new_character->nid, '@title2' => $new_character->title));
      drupal_set_message(t('Transfering XP (@amount) from #@nid @title to #@nid2 @title2',
        array('@amount' => $amount, '@nid' => $old_character->nid, '@title' => $old_character->title,
        '@nid2' => $new_character->nid, '@title2' => $new_character->title)));
      _character_sheet_update_xp($old_character->nid, $amount * -1, 'xp');
      _character_sheet_update_xp($new_character->nid, $amount, 'xp'); // Transfer as regular XP to be unlocked.
      // Transfer any lingering XP that's not already spent...
      global $user;
      $current_xp = _character_sheet_get_xp($old_character->nid, 'xp');
      $spent_xp = _character_sheet_get_xp($old_character->nid, 'spent_xp');
      if (($current_xp - $spent_xp) > 0) {
        $one_time = $current_xp - $spent_xp;
        // One time transfer of the current XP as well.
        watchdog('vaxia', 'One time transfering XP (@amount) from #@nid @title to #@nid2 @title2',
          array('@amount' => $one_time, '@nid' => $old_character->nid, '@title' => $old_character->title,
          '@nid2' => $new_character->nid, '@title2' => $new_character->title));
        drupal_set_message(t('One time transfering XP (@amount) from #@nid @title to #@nid2 @title2',
          array('@amount' => $one_time, '@nid' => $old_character->nid, '@title' => $old_character->title,
          '@nid2' => $new_character->nid, '@title2' => $new_character->title)));
        _character_sheet_update_xp($old_character->nid, $one_time * -1, 'xp');
        _character_sheet_update_xp($new_character->nid, $one_time, 'xp'); // Transfer as regular XP to be unlocked.
        $message = t('@xp @type xp added to @title by @user.',
          array('@xp' => $amount, '@type' => 'one time SH-XP', '@title' => $new_character->title, '@user' => $user->name));
        _character_sheet_update_log($new_character->nid, 'SH-XP', $message);
      }
      // Save log.
      $message = t('@xp @type xp added to @title by @user.',
        array('@xp' => $amount, '@type' => 'SH-RXP', '@title' => $new_character->title, '@user' => $user->name));
      _character_sheet_update_log($new_character->nid, 'SH-RXP', $message);
      $message = t('@xp @type xp added to @title by @user.',
        array('@xp' => $amount, '@type' => 'SH-XP', '@title' => $new_character->title, '@user' => $user->name));
      _character_sheet_update_log($new_character->nid, 'SH-XP', $message);
    }
  }
  else {
    drupal_set_message('Unable to locate selected character. Transfer not completed.', 'error');
  }
}

/**
 * Helper function, determine how many points a sheet has spent.
 * ALERT: RECURSIVE. BE CAREFUL HERE.
 * Based on _character_sheet_get_field_value().
 */
function _vaxia_get_field_value_flat($node, $field_key, $field_info, $nodes_seen = array()) {
  $nodes_seen[] = isset($node->nid) ? $node->nid : ''; // No nid to be found.
  // Given a node, find the key and patch to the information for that key.
  $values = array(
    'total' => 0,
    'number' => 0,
  );
  $language = isset($node->language) ? $node->language : 'und';
  $walkable_node = (array) $node;
  if (isset($walkable_node[$field_key]) && is_array($walkable_node[$field_key][$language])) {
    // Found it on the top level.
    // Grab all deltas and total them up.
    foreach ($walkable_node[$field_key][$language] as $delta => $delta_field) {
      if (isset($delta_field['value']) && is_numeric($delta_field['value'])) {
        // Otherwise, let's build up the value point by point according to the given cost.
        $values['total'] += $delta_field['value']; // CHANGE: FLAT VALUE TOTAL HERE.
        $values['number']++;
      }
    }
  }
  else {
    // We didn't find it on the top level. So it may be in a child somewhere.
    // Go looking.
    foreach (element_children($walkable_node) as $this_child) {
      $child = !empty($walkable_node[$this_child]) ? $walkable_node[$this_child] : array();
      if (is_array($child) && !empty($child[$language]) && is_array($child[$language])) {
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
              else {
                // The sub node hasn't been saved yet either.
                $sub_node = isset($delta_field['entity']) ? $delta_field['entity'] : '';
              }
            }
          }
          // If we found a subnode or field colelction at this this child, check it.
          if (!empty($sub_node)) {
            // NOTE: THIS DOES NOT HANDLE RECUSIVE LOOPS WELL - SO DON'T RECOUNT THINGS COUNTED.
            if ( (!isset($sub_node->nid)) || (isset($sub_node->nid) && !in_array($sub_node->nid, $nodes_seen)) ) {
              $new_totals = _vaxia_get_field_value_flat($sub_node, $field_key, $field_info, $nodes_seen);
              $values['total'] += $new_totals['total'];
              $values['number'] += $new_totals['number'];
            }
          }
        }
      }
    }
  }
  return $values;
}

/**
 * Helper function, list meaningful types for Vaxia.
 */
function _vaxia_meaningful_xp_types() {
  return array('xp', 'bxp', 'rxp', 'kxp', 'silver');
}

/**
 * Helper function, given a string retrieve any nid in the format [nid:XXX].
 * Or look for the node by title.
 */
function _vaxia_transfer_xp_autocomplete_get_nid($character_selected) {
  $result = preg_match('/\[nid:([0-9]+)\]$/', $character_selected, $matches);
  if ($result > 0) {
    $nid = $matches[$result];
    $node = node_load($nid);
    if (empty($node)) {
      $nodes = node_load_multiple(array(), array('title' => $character_selected));
      foreach ($nodes as $nid => $this_node) {
        $node = $this_node;
      }
    }
    if (!empty($node) && $node->type == 'character_sheet' && $node->status == 1) {
      return $node->nid;
    } 
  }
  return FALSE;
}

/**
 * XP callback for adding XP to a node. Vaxia's wierd XP complexity.
 */
function _vaxia_add_xp($nid, $xp, $type) {
  if (!is_numeric($xp)) {
    watchdog('vaxia', 'Error assigning XP for #@nid, @xp of @type - not numeric XP.',
      array('@nid' => $nid, '@xp' => $xp, '@type' => type));
    return;
  }
  global $user;
  $node = node_load($nid);
  $meaningful_types = _vaxia_meaningful_xp_types();
  $message = t('@xp @type XP added to @title by @user.',
    array('@xp' => $xp, '@type' => $type, '@title' => $node->title, '@user' => $user->name));
  if (!in_array($type, $meaningful_types)) {
    _vaxia_update_type_tracking(array('nid' => $nid, 'xp' => $xp, 'type' => $type)); // Add to the type given.
    $message = t('@xp @type XP added to @title by @user.',
      array('@xp' => $xp, '@type' => $type, '@title' => $node->title, '@user' => $user->name));
    if (function_exists('rules_invoke_event')) {
      $player = user_load($node->uid);
      rules_invoke_event('vaxia_gain_xp', $player, $node, $xp, $type);
    }
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
        // Remove from kxp, add 1 to rxp, xp.
        _character_sheet_update_xp($nid, $kxp_ratio * -1, 'kxp');
        _character_sheet_update_xp($nid, 1, 'rxp');
        _character_sheet_update_xp($nid, 1, 'xp');
        if (function_exists('rules_invoke_event')) {
          $player = user_load($node->uid);
          rules_invoke_event('vaxia_gain_xp', $player, $node, 1, 'xp');
        }
        // If we have banked xp, remove one from bxp and add spendable xp.
        $current_bxp = _character_sheet_get_xp($nid, 'bxp');
        if ($current_bxp > 0 ) {
          _character_sheet_update_xp($nid, -1, 'bxp');
          _character_sheet_update_xp($nid, 1, 'xp');
          if (function_exists('rules_invoke_event')) {
            $player = user_load($node->uid);
            rules_invoke_event('vaxia_gain_xp', $player, $node, 1, 'xp');
          }
        }
      }
    break;
    case 'bxp':
      // If bxp is added directly, just add it to the bxp score.
      _character_sheet_update_xp($nid, $xp, 'bxp');
      if (function_exists('rules_invoke_event')) {
        $player = user_load($node->uid);
        rules_invoke_event('vaxia_gain_xp', $player, $node, $xp, $type);
      }
    break;
    case 'rxp':
      // If rxp is added directly, just add it to the rxp score.
      _character_sheet_update_xp($nid, $xp, 'rxp');
      if (function_exists('rules_invoke_event')) {
        $player = user_load($node->uid);
        rules_invoke_event('vaxia_gain_xp', $player, $node, $xp, $type);
      }
    break;
    case 'silver':
      $message = t('@xp @ype added to @title by @user.',
        array('@xp' => $xp, '@type' => $type, '@title' => $node->title, '@user' => $user->name));
      _character_sheet_update_log($nid, 'silver', $message);
      // Silver is just added directly.
      _character_sheet_update_xp($nid, $xp, 'silver');
      // Echo log message to user.
      drupal_set_message($message);
    break;
    case 'xp':
    default:
      _character_sheet_update_log($nid, 'xp', $message);
      // Add per, b/c of the way the calculations work for adding.
      $current_rxp = _character_sheet_get_xp($nid, 'rxp');
      $bxp_to_add = 0;
      $xp_to_add = 0;
      $rxp_to_charge = 0;
      while ($xp > 0 ) {
        if ( ($current_rxp - $rxp_to_charge) > 0 ) {
          $rxp_to_charge++;
          $xp_to_add++;
        }
        else {
          $bxp_to_add++;
        }
        $xp--;
      }
      _character_sheet_update_xp($nid, $rxp_to_charge * -1, 'rxp');
      _character_sheet_update_xp($nid, $xp_to_add, 'xp');
      _character_sheet_update_xp($nid, $bxp_to_add, 'bxp');
      if (function_exists('rules_invoke_event')) {
        $player = user_load($node->uid);
        rules_invoke_event('vaxia_gain_xp', $player, $node, $xp_to_add, $type);
      }
      // Echo log message to user.
      drupal_set_message($message);
    break;
  }
  // Flush Entity Cache record.
  if (module_exists('entitycache')) {
    cache_clear_all($nid, 'cache_entity_node');
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
