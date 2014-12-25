<?php
/**
 * @file
 * vaxia.dice.inc - Provides Vaxian dice rolling for Vaxia.
 */

/**
 * Implementation of hook_dice_roller_form(), which returns a form for handling basic style rolls.
 */
function vaxia_dice_roller_form($form, $form_state, $form_id) {
  // Pass additional data from user profile configuration.
  // Create roller fieldset.
  $form['vaxia_rolls'] = array(
    '#type' => 'fieldset',
    '#theme' => 'vaxia_dice_roller',
    '#prefix' => '<div id="vaxia-dice-roller">',
    '#suffix' => '</div>',
    '#weight' => 10,
    '#tree' => TRUE, // This is important, this allows the deltas below to function.
  );
  // Get the character to work with for items
  // Set the available characters for the form dropdown.
  $character = _vaxia_dice_roller_get_selected_character($form, $form_state);
  // If we have a character, figure out the rest of the display for it.
  if ($character && $character != '_none' ) {
    $character = node_load($character);
    // Only add a form if the character really exists.
    if (isset($character->title) && ($character->type == 'character_sheet')) {
      // Mark character you're on.
      $form['vaxia_rolls']['#description'] = '<div class="vaxia-dice-roller-character">' .
        t('On character !name', array('!name' => l($character->title, 'node/' . $character->nid))) .
        '</div>';
      // Get the items to display if we have any.
      $skills = _vaxia_dice_roller_get_skills($character);
      $display_dice = variable_get('vaxia_display_dice', 6);
      for ($i = 0; $i < $display_dice; $i++) {
        $form['vaxia_rolls']['dice'][$i] = _vaxia_dice_roller_character_row($skills);
      }
      $form = module_invoke_all('vaxia_dice_form', $form, $form_state, $form_id);
      return $form;
    }
  }
  // No character? Return blank.
  $form['vaxia_rolls'] = array(
    '#type' => 'item',
    '#theme' => 'vaxia_dice_roller',
    '#markup' => '',
    '#prefix' => '<div id="vaxia-dice-roller">',
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Implementation of hook_dice_roller_form_submit(), process the rolls.
 */
function vaxia_dice_roller_form_submit($content, $origin = 'node') {
  // Bail out for a non-comment origin.
  if ($origin != 'comment') {
    return;
  }
  // Get details about this submission.
  $lang = $content->language;
  $character = isset($content->field_comment_character[$lang][0]['nid']) ?
    node_load($content->field_comment_character[$lang][0]['nid']) : '';
  global $user;
  variable_set('vaxia_last_character_' . $user->uid, $character->nid);
  // Get the command array.
  $roll_commands = _vaxia_dice_roller_submit_rolls($content->vaxia_rolls, $character, $lang);
  // Apply the set of rolls.
  if (!empty($roll_commands)) {
    // Get the notes and commands.
    $roll_commands['commands'] = !empty($roll_commands['commands']) ? $roll_commands['commands'] : array();
    $commands = implode(';', $roll_commands['commands']);
    $roll_commands['notes'] = !empty($roll_commands['notes']) ? $roll_commands['notes'] : array();
    $notes = '<br>' . implode('<br>', $roll_commands['notes']) . '<br>';
    // Make the call.
    module_load_include('inc', 'dice_roller', 'dice_roller.roller');
    $dids = _dice_roller_parse_roll($commands, $notes, $content->cid, 'c');
  }
}

/**
 * Get commands from the submitted rolls.
 */
function _vaxia_dice_roller_submit_rolls($submitted_rolls, $character, $lang) {
  // Check for roll formatting.
  $rolls = array();
  foreach ($submitted_rolls['dice'] as $delta => $roll) {
    $number = !empty($roll['number']) ? $roll['number'] : 0;
    if ($number > 0) {
      $roll['size'] = !empty($roll['size']) ? $roll['size'] : 1;
      $roll['stat'] = ($roll['stat'] != -1) ? $roll['stat'] : 'any';
      $roll['skill'] = ($roll['skill'] != -1) ? $roll['skill'] : 0;
      $rolls[] = $roll;
    }
  }
  $commands = module_invoke_all('vaxia_dice_commands', $rolls, $submitted_rolls, $character, $lang);
  return $commands;
}

/**
 * Helper function, list all stats.
 */
function _vaxia_dice_roller_get_selected_character(&$form, &$form_state) {
  // Get user info.
  global $user;
  $this_player = user_load($user->uid);
  $character = '_none';
  $stored_character = variable_get('vaxia_last_character_' . $user->uid, -1);
  $lang = $form['language']['#value'];
  // Check character selected from form.
  if (isset($form_state['values'])) {
    if(isset($form_state['values']['field_comment_character'][$lang][0]['nid'])) {
      $character = $form_state['values']['field_comment_character'][$lang][0]['nid'];
    }
  }
  elseif ($stored_character != -1) {
    $character = $stored_character;
  }
  elseif(!empty($form['field_comment_character'][$lang]['#default_value'])) {
    $character = $form['field_comment_character'][$lang]['#default_value'];
  }
  // Last ditch chance to grab the character if this is NOT an OOC room.
  if (_vaxia_dice_roller_is_ic($form['#node']) && $character == '_none') {
    // If none was the default, set it to the first available.
    if ((isset($form['field_comment_character'][$lang]['#default_value'])
      && $form['field_comment_character'][$lang]['#default_value'] == '_none')
      || empty($form['field_comment_character'][$lang]['#default_value'])) {
      $keys = array();
      if (empty($keys) && isset($form['field_comment_character'][$lang]['#options']['Characters'])) {
        $keys = array_keys($form['field_comment_character'][$lang]['#options']['Characters']);
      }
      if (empty($keys) && isset($form['field_comment_character'][$lang]['#options']['Personal NPCs'])) {
        $keys = array_keys($form['field_comment_character'][$lang]['#options']['Personal NPCs']);
      }
      if (empty($keys) && isset($form['field_comment_character'][$lang]['#options']['Open NPCs'])) {
        $keys = array_keys($form['field_comment_character'][$lang]['#options']['Open NPCs']);
      }
      // Set the available options.
      $form['field_comment_character'][$lang]['#default_value'] = array_shift($keys);
      // Get the character to return.
      $character = $form['field_comment_character'][$lang]['#default_value'];
    } // None was default, set get diff character
  }
  return $character;
}

/**
 * Helper function, create the row of dice rolling options.
 * For regular characters only, not SH characters.
 */
function _vaxia_dice_roller_character_row($skills) {
  $dice_size = array(100,77,50,30,20,16,12,10,8,6,4,2);
  // Make sure the stat keys are set by field.
  $stats = _vaxia_dice_roller_get_stats();
  // Set the dropdowns.
  $stats = _vaxia_dice_roller_get_dropdown($stats);
  $skills = _vaxia_dice_roller_get_dropdown($skills);
  // Dice number.
  $form['number'] = array(
    '#type' => 'select',
    '#options' => range(0, 10),
  );
  $form['number']['#attributes']['class'][] = 'dice-number';
  // Dice size.
  $form['size'] = array(
    '#type' => 'select',
    '#options' => array_combine($dice_size, $dice_size),
  );
  $form['size']['#attributes']['class'][] = 'dice-size';
  // Stat.
  $form['stat'] = array(
    '#type' => 'select',
    '#options' => $stats,
  );
  $form['stat']['#attributes']['class'][] = 'dice-stat';
  // Skill.
  $form['skill'] = array(
    '#type' => 'select',
    '#options' => $skills,
  );
  $form['skill']['#attributes']['class'][] = 'dice-skill';
  return $form;
}

/**
 * Helper function, list all stats.
 */
function _vaxia_dice_roller_get_dropdown($option_set) {
  $options = array('-1' => '<none>');
  foreach ($option_set as $option_key => $option_value) {
    $options[$option_key] = $option_value;
  }
  return $options;
}

/**
 * Helper function, list all stats.
 */
function _vaxia_dice_roller_get_stats($mode = 'all') {
  // Return the primary vs secondary relationship.
  if ($mode == 'relations') {
    return array(
      'life' => array('appearance', 'health'), 
      'endurance' => array('health', 'constitution'),
      'strength' => array('constitution', 'agility'), 
      'dexterity' => array('agility', 'reflexes'),
      'intelligence' => array('reflexes', 'awareness'),
      'spirituality' => array('awareness', 'presence'),
      'charisma' => array('presence', 'appearance'),
    );
  }
  // Return the list.
  return array(
    'life' => 'Life', 
    'health' => 'Health', 
    'endurance' => 'Endurance', 
    'constitution' => 'Constitution', 
    'strength' => 'Strength', 
    'agility' => 'Agility', 
    'dexterity' => 'Dexterity', 
    'reflexes' => 'Finesse', 
    'intelligence' => 'Intelligence', 
    'awareness' => 'Awareness', 
    'spirituality' => 'Spirituality', 
    'presence' => 'Presence', 
    'charisma' => 'Charisma', 
    'appearance' => 'Appearance',
  );
}

/**
 * Helper function, list all skills.
 */
function _vaxia_dice_roller_get_skills($character) {
  $skills = array();
  $lang = !empty($character->language) ? $character->language : 'und';
  if (isset($character->field_skill[$lang])) {
    foreach ($character->field_skill[$lang] as $delta => $skill) {
      $this_skill = field_collection_item_load($skill['value']);
      if ($this_skill) {
        $id = $skill['value'];
        $name = !empty($this_skill->field_skill_name[$lang][0]['value'])
          ? $this_skill->field_skill_name[$lang][0]['value'] : t('Unk. skill');
        $value = !empty($this_skill->field_skill_value[$lang][0]['value'])
          ? $this_skill->field_skill_value[$lang][0]['value'] : '';
        $skills[$id] = t('@name (@value)', array('@name' => $name, '@value' => $value));
      }
    }
  }
  return $skills;
}

/**
 * Helper function, the room is IC.
 */
function _vaxia_dice_roller_is_ic($room) {
  $ooc_rooms = variable_get('vaxia_limbo_room', array());
  if (!empty($room->nid) && !in_array($room->nid, $ooc_rooms)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Helper function, get the small label.
 */
function _vaxia_dice_roller_small_label($string) {
  $small_label = strtolower($string);
  $small_label = substr($small_label, 0, 3);
  if ($small_label == 'ref') {
    $small_label = 'fin';
  }
  return $small_label;
}