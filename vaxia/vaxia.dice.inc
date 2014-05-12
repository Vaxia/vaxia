<?php
/**
 * @file
 * vaxia.dice.inc - Provides Vaxian dice rolling for Vaxia.
 */

/**
 * Implementation of hook_dice_roller_form(), which returns a form for handling basic style rolls.
 */
function vaxia_dice_roller_form($parent_form, $parent_form_state, $parent_form_id) {
  // Pass additional data from user profile configuration.
  global $user;
  $this_player = user_load($user->uid);
  $character = '_none';
  $stored_character = variable_get('vaxia_last_character_' . $user->uid, -1);
  $lang = $parent_form['language']['#value'];

  if (isset($parent_form_state['values'])) {
    if(isset($parent_form_state['values']['field_comment_character'][$lang][0]['nid'])) {
      $character = $parent_form_state['values']['field_comment_character'][$lang][0]['nid'];
    }
  }
  elseif ($stored_character != -1) {
    $character = $stored_character;
  }
  elseif(!empty($form['field_comment_character'][$lang]['#default_value'])) {
    $character = $form['field_comment_character'][$lang]['#default_value'];
  }

  // Last ditch chance to grab the character.
  $limbo_room = variable_get('vaxia_limbo_room', array());
  if (!empty($parent_form['#node']->nid) && !in_array($parent_form['#node']->nid, $limbo_room) && $character == '_none') {
    $lang = $parent_form['language']['#value'];
    // If none was the default, set it to the first available.
    if ((isset($parent_form['field_comment_character'][$lang]['#default_value']) && $parent_form['field_comment_character'][$lang]['#default_value'] == '_none')
      || empty($parent_form['field_comment_character'][$lang]['#default_value'])) {
      $keys = array();
      if (empty($keys) && isset($parent_form['field_comment_character'][$lang]['#options']['Characters'])) {
        $keys = array_keys($parent_form['field_comment_character'][$lang]['#options']['Characters']);
      }
      if (empty($keys) && isset($parent_form['field_comment_character'][$lang]['#options']['Personal NPCs'])) {
        $keys = array_keys($parent_form['field_comment_character'][$lang]['#options']['Personal NPCs']);
      }
      if (empty($keys) && isset($parent_form['field_comment_character'][$lang]['#options']['Open NPCs'])) {
        $keys = array_keys($parent_form['field_comment_character'][$lang]['#options']['Open NPCs']);
      }
      $parent_form['field_comment_character'][$lang]['#default_value'] = array_shift($keys);
      $character = $parent_form['field_comment_character'][$lang]['#default_value'];
    }
  }

  $form['vaxia_rolls'] = array(
    '#type' => 'fieldset',
    '#theme' => 'vaxia_dice_roller',
    '#prefix' => '<div id="vaxia-dice-roller">',
    '#suffix' => '</div>',
    '#weight' => 10,
    '#tree' => TRUE, // This is important, this allows the deltas below to function.
  );

  if ($character && $character != '_none' ) {
    $character = node_load($character);
    if (isset($character->title) && ($character->type == 'character_sheet')) {
      $dice_size = array(100,77,50,30,20,16,12,10,8,6,4,2);
      $stat = array(
        '-1' => '<none>', 
        'field_life' => 'Life', 
        'field_health' => 'Health', 
        'field_endurance' => 'Endurance', 
        'field_constitution' => 'Constitution', 
        'field_strength' => 'Strength', 
        'field_agility' => 'Agility', 
        'field_dexterity' => 'Dexterity', 
        'field_reflexes' => 'Finesse', 
        'field_intelligence' => 'Intelligence', 
        'field_awareness' => 'Awareness', 
        'field_spirituality' => 'Spirituality', 
        'field_presence' => 'Presence', 
        'field_charisma' => 'Charisma', 
        'field_appearance' => 'Appearance'
      );
      // Add SH stats for listing. 25 to 70 as NPC max.
      $SH_character = _vaxia_get_SH_characters($user->uid);
      if (user_access('use SH dice options')
        && !empty($SH_character) && $character->nid == $SH_character->nid) {
        for ($i = 25; $i < 71; $i = $i + 5) {
          $stat['field_npc_stat_' . $i] = 'NPC: Stat @ ' . $i;
        }
      }
      $skills = array(-1 => '<none>');
      if (isset($character->field_skill[$character->language])) {
        foreach ($character->field_skill[$character->language] as $delta => $skill) {
          $this_skill = field_collection_item_load($skill['value']);
          $skills[$skill['value']] = $this_skill->field_skill_name[$character->language][0]['value'];  
        }
      }
      // Add SH stats for listing. 25 to 70 as NPC max.
      if (user_access('use SH dice options')
        && !empty($SH_character) && $character->nid == $SH_character->nid) {
        for ($i = 25; $i < 71; $i = $i + 5) {
          $skills['field_npc_skill_' . $i] = 'NPC: Skill @ ' . $i;
        }
      }
      $items = array(-1 => '<none>');
      if (isset($character->field_items[$character->language])) {
        $stat_set = array('life', 'health', 'endurance', 'constitution', 'strength', 'agility', 'dexterity',
          'reflexes', 'intelligence', 'awareness', 'spirituality', 'presence', 'charisma', 'appearance', 'skill'
        );
        foreach ($character->field_items[$character->language] as $delta => $item) {
          // Filter the item by workflow state.
          $this_item = node_load($item['nid']);
          if (isset($this_item->workflow)
            && (isset($this_item->workflow_state_name) && $this_item->workflow_state_name == t('approved'))) {
            $mods = array();
            $item_label = $this_item->title;
            $modifies = $percentages = $overlaps = $penalties = array();
            $qualifies = TRUE;
            $type = $this_item->field_type[$this_item->language][0]['value'];
            $type_percentage = variable_get('items_perc_' . $type, 0);
            foreach ($stat_set as $stat_name) {
              // Grab the stat.
              $field_name = 'field_' . $stat_name;
              $stat_stat = isset($character->{$field_name}[$character->language][0]['value']) ?
                $character->{$field_name}[$character->language][0]['value'] : 0;
              //  Get item mod.
              $field_name = 'field_item_' . $stat_name;
              $stat_mod = isset($this_item->{$field_name}[$this_item->language][0]['value']) ?
                $this_item->{$field_name}[$this_item->language][0]['value'] : 0;
              // Get item percentage.
              $field_name = 'field_item_perc_' . $stat_name;
              $stat_perc = isset($this_item->{$field_name}[$this_item->language][0]['value']) ?
                $this_item->{$field_name}[$this_item->language][0]['value'] : 0;
              if ($stat_name == 'strength') {
                $stat_perc = $stat_perc + $type_percentage;
              }
              // Get item overlap.
              $field_name = 'field_item_over_' . $stat_name;
              $stat_over = isset($this_item->{$field_name}[$this_item->language][0]['value']) ?
                $this_item->{$field_name}[$this_item->language][0]['value'] : 0;
              $field_name = 'field_item_required_' . $stat_name;
              $stat_req = isset($this_item->{$field_name}[$this_item->language][0]['value']) ? 
                $this_item->{$field_name}[$this_item->language][0]['value'] : 0;
              $field_name = 'field_item_pen_' . $stat_name;
              $stat_pen = isset($this_item->{$field_name}[$this_item->language][0]['value']) ?
                $this_item->{$field_name}[$this_item->language][0]['value'] : 0;  
              // Once collected, figure out what applies
              if ($stat_mod != 0) {
                $modifies[$stat_name] = $stat_mod;
              }
              if ($stat_perc != 0) {
                $percentages[$stat_name] = $stat_perc;
              }
              if ($stat_over != 0) {
                $overlaps[$stat_name] = $stat_over;
              }
              if ($stat_pen != 0) {
                $penalties[$stat_name] = $stat_pen;
              }
              if ($stat_stat < $stat_req && $stat_name != 'skill') {
                $qualifies = FALSE;
              }
            } // End stat loop
            // Once gathered, make sure we go through all relevant penalities for mods.   
            // Add all penalities for this item to the modifiers.   
            if (!$qualifies) {
              foreach ($stat_set as $stat_name) {
                if (isset($penalties[$stat_name])) {
                  $this_modifier = isset($modifies[$stat_name]) ? $modifies[$stat_name] : 0;
                  $modifies[$stat_name] = $this_modifier + $penalties[$stat_name];
                }
              }
            }
            // And set the labels for basic mods.
            $modifies_label = array();
            foreach ($modifies as $label => $value) {
              $small_label = substr($label, 0, 3);
              if ($small_label == 'ref') {
                $small_label = 'fin';
              }
              $modifies_label[] = $small_label . ':' . $value;
            }
            // And set the labels for percentage mods.
            foreach ($percentages as $label => $value) {
              $percentages[$label] = $value;
              $small_label = substr($label, 0, 3);
              if ($small_label == 'ref') {
                $small_label = 'fin';
              }
              $modifies_label[] = 'PP: ' . number_format($value) . '%';
            }
            // And set the labels for overlap mods.
            foreach ($overlaps as $label => $value) {
              $small_label = substr($label, 0, 3);
              if ($small_label == 'ref') {
                $small_label = 'fin';
              }
              $modifies_label[] = $small_label . ':' . number_format($value) . ' flat';
            }
            // Check for uses.
            if (module_exists('item_uses')) {
              $uses = isset($this_item->field_item_uses[$this_item->language][0]['value']) ?
                $this_item->field_item_uses[$this_item->language][0]['value'] : -1;
              if ($uses != -1) {
                $modifies_label[] = t('@uses uses', array('@uses' => $uses));
              }
            }
            // And consolidate into one label.
            if (!empty($modifies_label)) {
              $item_label .= ' (' . implode(', ', $modifies_label) . ')';
              $items[$this_item->nid] = $item_label;
            }
            else {
              // This item doesn't modify anything - we shouldn't have it in the list!
              unset($items[$this_item->nid]);
            }
          }
        } // End items loop
      }
      $display_dice = variable_get('vaxia_display_dice', 6);
      for ($i = 0; $i < $display_dice; $i++) {
        $form['vaxia_rolls']['dice'][$i] = _vaxia_dice_roller_row($dice_size, $stat, $skills);
      }
      $form['vaxia_rolls']['items'] = array(
        '#title' => t('Using items'),
        '#type' => 'select',
        '#options' => $items,
        '#size' => count($items),
        '#multiple' => TRUE,
      );
      $form['vaxia_rolls']['notes'] = array(
        '#type' => 'hidden',
      );
      $form['vaxia_rolls']['#description'] = '<div class="vaxia-dice-roller-character">On character ' .
        l($character->title, 'node/' . $character->nid) . '</div>';
      return $form;
    }
  }
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
 * Helper function, create the row of dice rolling options.
 */
function _vaxia_dice_roller_row($dice_size, $stat, $skill) {
  $form['number'] = array(
    '#type' => 'select',
    '#options' => range(0, 10),
  );
  $form['number']['#attributes']['class'][] = 'dice-number';
  $form['size'] = array(
    '#type' => 'select',
    '#options' => array_combine($dice_size, $dice_size),
  );
  $form['size']['#attributes']['class'][] = 'dice-size';
  $form['stat'] = array(
    '#type' => 'select',
    '#options' => $stat,
  );
  $form['stat']['#attributes']['class'][] = 'dice-stat';
  $form['skill'] = array(
    '#type' => 'select',
    '#options' => $skill,
  );
  $form['skill']['#attributes']['class'][] = 'dice-skill';
  return $form;
}

/**
 * Implementation of hook_dice_roller_form_submit(), which returns a form for handling basic style rolls.
 */
function vaxia_dice_roller_form_submit($content, $origin = 'node') {
  $item_stack = variable_get('vaxia_item_stack', TRUE);
  switch ($origin) {
    case 'comment':
      if (!empty($content->vaxia_rolls)) {
        $command = '';
        $note = $content->vaxia_rolls['notes'] . '<br>';
        $lang = $content->language;

        // Gather the character information if it's set.
        $character = '';
        if (isset($content->field_comment_character[$lang][0]['nid'])) {
          $character = node_load($content->field_comment_character[$lang][0]['nid']);
        }

        // Gather the item mods. Sorted by stat.
        $item_mods = array();
        // Check items against stats.
        if (isset($content->vaxia_rolls['items'])) {
          // If character set, get the stats and items.
          if (!empty($character)) {
            $item_mods = _vaxia_dice_roller_form_submit_item_mods($character, $content->vaxia_rolls['items']);
          }
        }
        /**
         *  Returns in the format:
         *  array(
         *   'stat1' => array(
         *      'modify' => X,
         *      'percentage' => X,
         *      'overlap' => X,
         *      'notes' => X,
         *    ),
         *    'stat2' => array(
         *      'modify' => X,
         *      'percentage' => X,
         *      'overlap' => X,
         *      'notes' => X,
         *    ),
         *  );
         */

        // Calculate mods and mod note for stats, skills, etc.
        foreach ($content->vaxia_rolls['dice'] as $delta => $roll) {
          // Only roll if we have at least one die to roll.
          if ($roll['number'] > 0) {
            // Base command.
            $this_command = $roll['number'] . 'd' . $roll['size'];
            // Build up the command for the given stat based on character and gathered item information.
            $this_stat = '';
            $mod_note = '';
            $mod = $item_mod = 0;
            $stat_note = $skill_note = '';

            // If stat is selected, grab the basic stat AND/OR the overlap if it's been set.
            if (!empty($roll['stat']) && $roll['stat'] != -1) {
              // Pull stat value.
              $this_stat = '';
              if (strpos($roll['stat'], 'field_npc_stat_') !== FALSE) {
                // An NPC roll pull from stat directly.
                $this_stat = 'npc';
                $stat = substr($roll['stat'], 15); // Trim "field_npc_stat_" off the top.
              }
              else {
                // Pull from character sheet.
                // Note which stat this is for.
                $this_stat = substr($roll['stat'], 6); // Trim "field_" off the top.
                // Check for overlap from an item.
                if (!empty($item_mods[$this_stat]['overlap']) && !empty($item_mods[$this_stat]['overlap'])) {
                  $stat = $item_mods[$this_stat]['overlap'];
                }
                elseif (!empty($character)) {
                  $stat = !empty($character->{$roll['stat']}[$lang][0]['value']) ? $character->{$roll['stat']}[$lang][0]['value'] : 0;
                }
              }
              // Set the stat message and label.
              $small_label = substr($this_stat, 0, 3);
              if ($small_label == 'ref') {
                $small_label = 'fin';
              }
              $mod_note .= ' + ' . $small_label . ' (' . $stat . ')';
              $mod += $stat;

              // Add item mods for stats.
              if (!empty($item_mods[$this_stat]['modify'])) {
                $item_mod += $item_mods[$this_stat]['modify'];
              }
              $stat_note .= !empty($item_mods[$this_stat]['notes']) ? $item_mods[$this_stat]['notes'] : '';
            }

            // If skill selected, grab the skill for mods.
            if (!empty($roll['skill']) && $roll['skill'] != -1) {
              // Pull skill value.
              if (strpos($roll['skill'], 'field_npc_skill_') !== FALSE) {
                // An NPC roll pull from skill directly.
                $skill = substr($roll['skill'], 16); // Trim "field_npc_skill_" off the top.
                $skill_name = 'npc skill';
                $skill_desc = t('A placeholder for an NPC skill - for use by SH staff to roll quick rolls with.');
              }
              else {
                // Pull from character sheet.
                // Check for overlap from an item.
                if (!empty($item_mods['skill']['overlap'])) {
                  $skill = $item_mods['skill']['overlap'];
                }
                elseif (!empty($character)) {
                  $skill_details = field_collection_item_load($roll['skill']);
                  $skill = !empty($skill_details->field_skill_value[$lang][0]['value']) ? $skill_details->field_skill_value[$lang][0]['value'] : 0;
                }
                $skill_name = !empty($skill_details->field_skill_name[$lang][0]['value']) ? $skill_details->field_skill_name[$lang][0]['value'] : 'skill';
                $skill_desc = !empty($skill_details->field_skill_desc[$lang][0]['value']) ? $skill_details->field_skill_desc[$lang][0]['value'] : '';
              }
              // Set the stat message and label.
              $small_label = substr($this_stat, 0, 3);
              if ($small_label == 'ref') {
                $small_label = 'fin';
              }
              $mod_note .= ' +  <a title="' . $skill_desc . '" href="#">' . $skill_name . '</a> (' . $skill . ')';
              $mod += $skill;

              // Add item mods for skills.
              if (!empty($item_mods['skill']['modify'])) {
                $item_mod += $item_mods['skill']['modify'];
              }
              $skill_note .= !empty($item_mods['skill']['notes']) ? $item_mods['skill']['notes'] : '';
            }

            // Sum up the mods for items from stats and skills, and add to the final mods.
            if (!empty($item_mod)) {
              $mod += $item_mod;
            }
            if (!empty($stat_note) || !empty($skill_note)) {
              $item_notes = $stat_note . ' ' . $skill_note;
              $item_notes = trim($item_notes);
              if (empty($item_notes)) {
                $item_notes = 'n/a';
              }
              if (empty($item_mod)) {
                $item_mod = 'see Items';
              }
              $mod_note .= ' +  <a title="' . $item_notes . '" href="#">Items</a> (' . $item_mod . ')';
            }

            // And add the command to the roll set to be processed.
            $note .= 'Roll '. $delta + 1 .' : roll(' . $this_command . ')' . $mod_note . '<br>';
            if ($mod != 0) {
              $command .= $this_command . '+' . $mod .';';
            }
            else {
              $command .= $this_command .';';
            }

          } // End building up the command for this roll.
        } // End loop for each roll selected.
      } // End check for empty rolls.

      // Apply the set of rolls.
      if (!empty($command)) {
        module_load_include('inc', 'dice_roller', 'dice_roller.roller');
        $dids = _dice_roller_parse_roll($command, $note, $content->cid, 'c');
      }
      break;
  }
}

/**
 * Helper function, gather item mods per owner character and selected set of items for a roll.
 *  Returns in the format:
 *  array(
 *   'stat1' => array(
 *      'modify' => X,
 *      'percentage' => X,
 *      'overlap' => X,
 *      'notes' => X,
 *    ),
 *    'stat2' => array(
 *      'modify' => X,
 *      'percentage' => X,
 *      'overlap' => X,
 *      'notes' => X,
 *    ),
 *  );
 */
function _vaxia_dice_roller_form_submit_item_mods($character, $items) {
  $stacking = variable_get('vaxia_dice_roller_stacking', TRUE);
  $item_mods = array();
  $item_notes = '';
  $lang = $character->language;
  $stat_set = array('life', 'health', 'endurance', 'constitution', 'strength', 'agility', 'dexterity',
    'reflexes', 'intelligence', 'awareness', 'spirituality', 'presence', 'charisma', 'appearance', 'skill',
  );

  // Loop for each item.
  foreach ($items as $item) {
    $item = node_load($item);
    // Only do something if the item exists.
    if (isset($item->nid)) {
      $item_lang = $item->language;
      // Assume all items qualify.
      $qualifies = TRUE;
      // While looping for each item, loop again for each stat.
      foreach ($stat_set as $stat_name) {
        // Get character's stat value.
        $field_name = 'field_' . $stat_name;
        $stat = isset($character->{$field_name}[$lang][0]['value']) ? $character->{$field_name}[$lang][0]['value'] : 0;
        $small_label = substr($stat_name, 0, 3);
        if ($small_label == 'ref') {
          $small_label = 'fin';
        }
        //if ($stat_name == 'skill') {
        //  $stat = 0;
        //  if (isset($roll['skill']) && $roll['skill'] != -1) {
        //    $skill = field_collection_item_load($roll['skill']);
        //    $skill_lang = isset($skill->language) ? $skill->language : $lang;
        //    $stat = isset($skill->field_skill_value[$skill_lang][0]['value']) ?
        //    $skill->field_skill_value[$skill_lang][0]['value'] : 0;
        //  }
        //}

        // Get item modify, percent, overlap, required and penalty values for this stat.
        $field_name = 'field_item_' . $stat_name;
        $modify = isset($item->{$field_name}[$item_lang][0]['value']) ? $item->{$field_name}[$item_lang][0]['value'] : 0;
        $field_name = 'field_item_perc_' . $stat_name;

        $percentage = isset($item->{$field_name}[$item_lang][0]['value']) ? $item->{$field_name}[$item_lang][0]['value'] : 0;
        $type = $item->field_type[$item->language][0]['value'];
        $type_percentage = variable_get('items_perc_' . $type, 0);
        $percentage = $percentage + $type_percentage;

        $field_name = 'field_item_over_' . $stat_name;
        $overlap = isset($item->{$field_name}[$item_lang][0]['value']) ? $item->{$field_name}[$item_lang][0]['value'] : 0;
        $field_name = 'field_item_required_' . $stat_name;
        $required = isset($item->{$field_name}[$item_lang][0]['value']) ? $item->{$field_name}[$item_lang][0]['value'] : 0;

        // Set modify value into return array. Use the largest one IFF set to not allow stacking.
        $item_mods[$stat_name]['modify'] = isset($item_mods[$stat_name]['modify']) ? $item_mods[$stat_name]['modify'] : '';
        $item_mods[$stat_name]['notes'] = isset($item_mods[$stat_name]['notes']) ? $item_mods[$stat_name]['notes'] : '';
        if ($stacking) {
          if ($modify != 0) {
            $item_mods[$stat_name]['modify'] += $modify;
            $item_mods[$stat_name]['notes'] .= ' + ' . $item->title . ' (' . $modify . ' ' . $small_label .')';
          }
        }
        else {
          if ($modify != 0) {
            if (!empty($item_mods[$stat_name]['modify'])) {
              if ($modify > 0 && $modify > $item_mods[$stat_name]['modify']) {
                $item_mods[$stat_name]['modify'] = $modify;
                $item_mods[$stat_name]['notes'] = ' + ' . $item->title . ' (' . $modify . ' ' . $small_label .')';
              }
              if ($modify < 0 && $modify < $item_mods[$stat_name]['modify']) {
                $item_mods[$stat_name]['modify'] = $modify;
                $item_mods[$stat_name]['notes'] = ' + ' . $item->title . ' (' . $modify . ' ' . $small_label .')';
              }
            }
            else {
              $item_mods[$stat_name]['modify'] = $modify;
              $item_mods[$stat_name]['notes'] = ' + ' . $item->title . ' (' . $modify . ' ' . $small_label .')';
            }
          }
        }

        // Set percentages into return array. Use the largest one.
        $item_mods[$stat_name]['percentage'] = isset($item_mods[$stat_name]['percentage']) ? $item_mods[$stat_name]['percentage'] : '';
        if ($percentage != 0) {
          if (!empty($item_mods[$stat_name]['percentage'])) {
            if ($percentage > $item_mods[$stat_name]['percentage']) {
              $item_mods[$stat_name]['percentage'] = $percentage;
              $stat_bonus = ($percentage/100) * $stat;
              $stat_bonus = number_format($stat_bonus, 2);
              $item_mods[$stat_name]['notes']  = ' + ' . $item->title . ' (' . $percentage . '% PP)';
            }
          }
          else {
            $item_mods[$stat_name]['percentage'] = $percentage;
            $stat_bonus = ($percentage/100) * $stat;
            $stat_bonus = number_format($stat_bonus, 2);
            $item_mods[$stat_name]['notes']  = ' + ' . $item->title . ' (' . $percentage . '% PP)';
          }
        }

        // Set overlap value into return array. Use the largest one.
        $item_mods[$stat_name]['overlap'] = isset($item_mods[$stat_name]['overlap']) ? $item_mods[$stat_name]['overlap'] : '';
        if ($overlap != 0) {
          if (!empty($item_mods[$stat_name]['overlap'])) {
            if ($overlap > $item_mods[$stat_name]['overlap']) {
              $item_mods[$stat_name]['overlap'] = $overlap;
              $item_mods[$stat_name]['notes'] = ' + ' . $item->title . ' (' . $overlap . ' flat ' . $small_label .')';
            }
          }
          else {
            $item_mods[$stat_name]['overlap'] = $overlap;
            $item_mods[$stat_name]['notes'] = ' + item ' . $item->title . ' (' . $overlap . ' flat ' . $small_label .')';
          }
        }

        // Make additional changes if this item triggered a problem.
        if ($stat < $required) {
          $qualifies = FALSE;
        }        
      } // End loop for each stat for this item.

      // We are still in the each item loop.
      // We now know this item's mods and if it triggered a penalty.

      // If we did trigger a penalty with this item, make the changes we need to.
      if (!$qualifies) {
        // This item triggers a penalty! So we need to go through the stat loop again.
        // Grab the penalties for this item, and apply them.
        foreach ($stat_set as $stat_name) {
          $field_name = 'field_item_pen_' . $stat_name;
          $penalty = isset($item->{$field_name}[$item_lang][0]['value']) ?$item->{$field_name}[$item_lang][0]['value'] : 0;
          if ($penalty != 0) {
            $item_mods[$stat_name]['modify'] += $penalty;
            $item_mods[$stat_name]['notes'] .= ' + ' . $item->title . ' (' . $penalty . ' ' . $small_label .')';
          }
        }
      }

    } // End if item exists.
  } // End loop per item.
  return $item_mods;
}