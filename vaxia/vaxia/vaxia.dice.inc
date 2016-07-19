<?php
/**
 * @file
 * vaxia.dice.inc - Provides Vaxian dice rolling for Vaxia.
 */

/**
 * Implementation of hook_dice_roller_form(), which returns a form for handling basic style rolls.
 */
function vaxia_dice_roller_form($form, $form_state, $form_id) {
  // Get the character to work with for items
  // Set the available characters for the form dropdown.
  $character = _vaxia_dice_roller_get_selected_character($form, $form_state);
  // No character? Return blank.
  $form['vaxia_rolls'] = array(
    '#type' => 'fieldset',
    '#theme' => 'vaxia_dice_roller',
    '#prefix' => '<div id="vaxia-dice-character" class="vaxia-dice-roller vaxia-dice-roller-hidden">',
    '#suffix' => '</div>',
    '#weight' => 10,
    '#tree' => TRUE, // This is important, this allows the deltas below to function.
  );
  // If you have the character set the dice form.
  if ($character && $character != '_none' ) {
    $form['vaxia_rolls'] = _vaxia_dice_roller_dice_form($character, 'character');
    drupal_alter('vaxia_dice_form', $form, $form_state, $form_id);
  }
  return $form;
}

/**
 * Element creation form for dice roller configurations.
 */
function _vaxia_dice_roller_dice_form($node, $type = 'character') {
  // Create roller fieldset.
  $element = array(
    '#type' => 'fieldset',
    '#theme' => 'vaxia_dice_roller',
    '#prefix' => '<div id="vaxia-dice-' . $type . '" class="vaxia-dice-roller">',
    '#suffix' => '</div>',
    '#weight' => 10,
    '#tree' => TRUE, // This is important, this allows the deltas below to function.
  );
  // If we have a character, figure out the rest of the display for it.
  if ($node && $node != '_none' ) {
    $node = node_load($node);
    // Only add a form if the character really exists.
    if (isset($node->title)) {
      // Mark character you're on.
      $element['#description'] = '<div class="vaxia-dice-roller-node">' .
        t('On ' . $type . ' !name', array('!name' => l($node->title, 'node/' . $node->nid))) .
        '</div>';
      // Get the items to display if we have any.
      $display_dice = variable_get('vaxia_display_dice', 6);
      for ($i = 0; $i < $display_dice; $i++) {
        $element['dice'][$i] = _vaxia_dice_roller_node_row($node);
      }
    }
  }
  // Return element.
  return $element;
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
  if (!empty($content->vaxia_rolls)) {
    $lang = $content->language;
    $character = isset($content->field_comment_character[$lang][0]['nid']) ?
      node_load($content->field_comment_character[$lang][0]['nid']) : '';
    global $user;
    if (!empty($character)) {
      variable_set('vaxia_last_character_' . $user->uid, $character->nid);
    }
    // Get the command array.
    $commands = _vaxia_dice_roller_submit_rolls($content->vaxia_rolls, $character, $lang);
    // Apply the set of rolls.
    if (!empty($commands)) {
      // Make the call.
      module_load_include('inc', 'dice_roller', 'dice_roller.roller');
      foreach ($commands as $command) {
        if (!empty($command['command'])) {
          $command['note'] = !empty($command['note']) ? $command['note'] : $command['command'];
          _dice_roller_parse_roll($command['command'], $command['note'], $content->cid, 'c');
        }
      }
    }
  }
}

/**
 * Get commands from the submitted rolls.
 */
function _vaxia_dice_roller_submit_rolls($submitted_rolls, $character, $lang) {
  if (!empty($submitted_rolls['dice']) && is_array($submitted_rolls['dice'])) {
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
    // Get initial rolls.
    $rolls = _vaxia_dice_roller_prep_rolls($rolls, $character, $lang);
    // Allow other modules to affect the rolls.
    $context = array(
      'submitted_rolls' => $submitted_rolls,
      'character' => $character,
      'lang' => $lang,
    );
    drupal_alter('vaxia_dice_commands', $rolls, $context);
    // Figure out the exact command set.
    $commands = array();
    foreach ($rolls as $delta => $roll) {
      // Double check the roll info.s
      $roll['stat'] = ($roll['stat'] > 0) ? (int) trim($roll['stat']) : 0;
      $roll['skill'] = ($roll['skill'] > 0) ? (int) trim($roll['skill']) : 0;
      // Set roll.
      $roll['might'] = $roll['stat'] + $roll['skill'];
      // Make the command.
      $this_note = !empty($roll['note']) ? implode(' + ', $roll['note']) : $roll['number'] . 'd' . $roll['size'];
      $commands[] = array(
        'command' => $roll['number'] . 'd' . $roll['size'] . '+' . $roll['might'],
        'note' => $this_note,
      );
    }
    return $commands;
  }
  return array();
}

/**
 * Helper function, prepare rolls.
 */
function _vaxia_dice_roller_prep_rolls($sub_rolls, $character, $lang) {
  // Figure out the stat and skill.
  $rolls = array();
  foreach ($sub_rolls as $delta => $roll) {

    // Set defaults.
    $roll_attr = array();
    $roll['number'] = !empty($roll['number']) ? $roll['number'] : 1;
    $roll['size'] = !empty($roll['size']) ? $roll['size'] : 100;
    $roll['stat'] = !empty($roll['stat']) ? $roll['stat'] : 'any';
    $roll['skill'] = !empty($roll['skill']) ? $roll['skill'] : 0;
    // Get some data before we start replacing values.
    $roll['stat_name'] = $roll['stat'];
    $roll['skill_nid'] = $roll['skill'];
    // Add an initial value to the note array so we get a leading '+' on display.
    $roll['note'] = !empty($roll['note']) ? $roll['note'] : array();

    // Get stat value from the character if it exists.
    if (!empty($roll['stat'])) {
      $small_label = _vaxia_dice_roller_small_label($roll['stat_name']);
      $field_name = 'field_' . $roll['stat_name'];
      $roll['stat'] = !empty($character->{$field_name}[$lang][0]['value']) ? $character->{$field_name}[$lang][0]['value'] : 0;
      if (!empty($character->{$field_name}[$lang][0]['value'])) {
        $roll['note'][] = $small_label . ' (' . $roll['stat'] . ')';
      }
      $roll_attr['stat_name'] = $small_label;
      $roll_attr['stat'] = $roll['stat'];
    }

    // Get skill value from the character if it exists.
    if (!empty($roll['skill'])) {
      $skill_details = field_collection_item_load($roll['skill']);
      // Update the skill after trying to load it for review.
      $roll['skill'] = 0;
      // Process the skill if we got one.
      if (!empty($skill_details->field_skill_value[$lang][0]['value'])) {
        $value = !empty($skill_details->field_skill_value[$lang][0]['value']) ? $skill_details->field_skill_value[$lang][0]['value'] : 0;
        $name = !empty($skill_details->field_skill_name[$lang][0]['value']) ? $skill_details->field_skill_name[$lang][0]['value'] : t('Unk. skill');
        $desc = !empty($skill_details->field_skill_desc[$lang][0]['value']) ? $skill_details->field_skill_desc[$lang][0]['value'] : '';
        $roll['skill'] = $value;
        $roll_attr['skill'] = $value;
        // Add aspects to display.
        $aspects = array();
        if (!empty($skill_details->field_aspect_one[$lang][0]['tid'])) {
          $aspect = taxonomy_term_load($skill_details->field_aspect_one[$lang][0]['tid']);
          $aspects[] = strtolower($aspect->name);
        }
        if (!empty($skill_details->field_aspect_two[$lang][0]['tid'])) {
          $aspect = taxonomy_term_load($skill_details->field_aspect_two[$lang][0]['tid']);
          $aspects[] = strtolower($aspect->name);
        }
        if (!empty($skill_details->field_aspect_three[$lang][0]['tid'])) {
          $aspect = taxonomy_term_load($skill_details->field_aspect_three[$lang][0]['tid']);
          $aspects[] = strtolower($aspect->name);
        }
        if (!empty($aspects)) {
          $desc .= ' (' . implode(', ', $aspects) . ')';
          $roll_attr['aspects'] = implode(' ', $aspects);
        }
        // Add tech to display.
        if (!empty($skill_details->field_tech_languages[$lang]) && is_array($skill_details->field_tech_languages[$lang])) {
          $techs = array();
          foreach ($skill_details->field_tech_languages[$lang] as $tech_delta => $tech_id) {
            $tech_details = field_collection_item_load($tech_id['value']);
            if (!empty($tech_details->field_tech_language[$lang][0]['tid'])) {
              $tech = taxonomy_term_load($tech_details->field_tech_language[$lang][0]['tid']);
              $tech_desc = !empty($tech_details->field_tech_proficiency[$lang][0]['value']) ? $tech_details->field_tech_proficiency[$lang][0]['value'] : 'Unfamiliar';
              $techs[] = $tech->name . ': ' . strtolower($tech_desc);
            }
          }
          // Add any tech langs found.
          if (!empty($tech)) {
            $desc .= ' (' . implode(', ', $techs) . ')';
          }
        }
        // Set display.
        $roll_context = '';
        foreach ($roll_attr as $key => $val) {
          $roll_context .= ' ' . $key . '="' . $val . '"';
        }
        $roll['note'][] = '<a title="' . $desc . '" ' . $roll_context . ' href="#">' . $name . '</a> (' . $value . ')';
      }
    }

    // Add roll to rolls to return;
    $rolls[$delta] = $roll;
  }
  return $rolls;
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
function _vaxia_dice_roller_node_row($node) {
  $dice_size = array(100,20,12,10,8,6,4,2);
  // Make sure the stat keys are set by field.
  $stats = _vaxia_dice_roller_get_stats($node);
  $skills = _vaxia_dice_roller_get_skills($node);
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
function _vaxia_dice_roller_get_stats($node = '') {
  // Return the list.
  $list = array(
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
  // Use the character if available.
  if (!empty($node)) {
    $lang = !empty($node->language) ? $node->language : 'und';
    foreach ($list as $key => $value) {
      $field_name = 'field_' . $key;
      $char_value = !empty($node->{$field_name}[$lang][0]['value']) ? $node->{$field_name}[$lang][0]['value'] : 0;
      $list[$key] = $value . ' (' . $char_value . ')';
    }
  }
  return $list;
}

/**
 * Helper function, list all stats.
 */
function _vaxia_dice_roller_get_stat_relations() {
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


/**
 * Helper function, list all skills.
 */
function _vaxia_dice_roller_get_skills($node) {
  $skills = array();
  $lang = !empty($node->language) ? $node->language : 'und';
  if (isset($node->field_skill[$lang])) {
    foreach ($node->field_skill[$lang] as $delta => $skill) {
      $this_skill = field_collection_item_load($skill['value']);
      if (!empty($this_skill)) {
        $id = $skill['value'];
        $name = !empty($this_skill->field_skill_name[$lang][0]['value'])
          ? $this_skill->field_skill_name[$lang][0]['value'] : t('Unk. skill');
        $value = !empty($this_skill->field_skill_value[$lang][0]['value'])
          ? $this_skill->field_skill_value[$lang][0]['value'] : '';
        if (!empty($value) && $value > 0) {
          $skills[$id] = t('@name (@value)', array('@name' => $name, '@value' => $value));
        }
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