<?php
/**
 * @file
 * vaxia.dice.inc - Provides Vaxian dice rolling for Vaxia.
 */

/**
 * Implementation of hook_dice_roller_form(), which returns a form for handling basic style rolls.
 */
function vaxia_dice_roller_form($parent_form, $parent_form_state, $parent_form_id) {
  $form['vaxia_rolls'] = array(
    '#type' => 'fieldset',
    '#theme' => 'vaxia_dice_roller',
    '#prefix' => '<div id="vaxia-dice-roller">',
    '#suffix' => '</div>',
    '#weight' => 10,
    '#tree' => TRUE, // This is important, this allows the deltas below to function.
  );
  // Pass additional data from user profile configuration.
  global $user;
  $this_player = user_load($user->uid);
  $character = '_none';
  $lang = $parent_form['language']['#value'];

  if (isset($parent_form_state['values'])) {
    if(isset($parent_form_state['values']['field_comment_character'][$lang][0]['nid'])) {
      $character = $parent_form_state['values']['field_comment_character'][$lang][0]['nid'];
    }
  }
  elseif(!empty($this_player->field_current_character[$lang][0]['nid'])) {
    $character = $this_player->field_current_character[$lang][0]['nid'];
  }
  elseif(!empty($form['field_comment_character'][$lang]['#default_value'])) {
    $character = $form['field_comment_character'][$lang]['#default_value'];
  }
  // Last ditch chance to grab the character.
  $limbo_room = variable_get('vaxia_limbo_room', array());
  if (!in_array($parent_form['#node']->nid, $limbo_room) && $character == '_none') {
    $lang = $parent_form['language']['#value'];
    // If none was the default, set it to the first available.
    if ($parent_form['field_comment_character'][$lang]['#default_value'] == '_none'
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
      $skills = array(-1 => '<none>');
      if (isset($character->field_skill[$character->language])) {
        foreach ($character->field_skill[$character->language] as $delta => $skill) {
          $this_skill = field_collection_item_load($skill['value']);
          $skills[$skill['value']] = $this_skill->field_skill_name[$character->language][0]['value'];  
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
            && (isset($this_item->workflow_state_name) && $this_item->workflow_state_name == t('approved') || $this_item->workflow == 4)) {
            $mods = array();
            $item_label = $this_item->title;
            $modifies = $percentages = $overlaps = $penalties = array();
            $qualifies = TRUE;
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
              $small_label = substr($label, 0, 3);
              if ($small_label == 'ref') {
                $small_label = 'fin';
              }
              $modifies_label[] = $small_label . ':' . number_format($value) . '%';
            }
            // And set the labels for overlap mods.
            foreach ($overlaps as $label => $value) {
              $small_label = substr($label, 0, 3);
              if ($small_label == 'ref') {
                $small_label = 'fin';
              }
              $modifies_label[] = $small_label . ':' . number_format($value) . ' flat';
            }
            // And consolidate into one label.
            if (!empty($modifies_label)) {
              $item_label .= ' (' . implode(', ', $modifies_label) . ')';
            }
            $items[$this_item->nid] = $item_label;
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
  switch ($origin) {
    case 'comment':
      if (!empty($content->vaxia_rolls)) {
        $command = '';
        $note = $content->vaxia_rolls['notes'] . '<br>';
        $lang = $content->language;
        $stat_set = array('life', 'health', 'endurance', 'constitution', 'strength', 'agility', 'dexterity',
          'reflexes', 'intelligence', 'awareness', 'spirituality', 'presence', 'charisma', 'appearance', 'skill',
        );

        // Calculate mods and mod note for stats, skills, etc.
        foreach ($content->vaxia_rolls['dice'] as $delta => $roll) {
          if ($roll['number'] > 0) {
            $this_command = $roll['number'] . 'd' . $roll['size'];
            // Check items: For This Stat Only.
            $this_stat = substr($roll['stat'], 6); // Trim "field_" off the top.
            // Mods for *this* item.
            $item_notes = '';
            $item_mod = 0;
            $overlap_mod = '';
            // Check items against stats.
            if (isset($content->vaxia_rolls['items'])) {
              // If character set, get the stats and items.
              if (isset($content->field_comment_character[$lang][0]['nid'])) {
                // Grab the character and items.
                $character = node_load($content->field_comment_character[$lang][0]['nid']);
                $char_lang = $character->language;
                // Gather each item and mods.
                foreach ($content->vaxia_rolls['items'] as $this_item) {
                  $this_item = node_load($this_item);
                  if (isset($this_item->nid)) {
                    $item_lang = $this_item->language;
                    $modifies = $percent_message = $overlaps = $penalties = array();
                    $qualifies = TRUE;
                    // Loop for each stat.
                    foreach ($stat_set as $stat_name) {
                      // Get character's stat value.
                      $field_name = 'field_' . $stat_name;
                      $stat = isset($character->{$field_name}[$char_lang][0]['value']) ?
                        $character->{$field_name}[$char_lang][0]['value'] : 0;
                      if ($stat_name == 'skill') {
                        $stat = 0;
                        if (isset($roll['skill']) && $roll['skill'] != -1) {
                          $skill = field_collection_item_load($roll['skill']);
                          $skill_lang = isset($skill->language) ? $skill->language : $char_lang;
                          $stat = isset($skill->field_skill_value[$skill_lang][0]['value']) ?
                            $skill->field_skill_value[$skill_lang][0]['value'] : 0;
                        }
                      }

                      // Get item modified
                      $field_name = 'field_item_' . $stat_name;
                      $modify = isset($this_item->{$field_name}[$item_lang][0]['value']) ?
                        $this_item->{$field_name}[$item_lang][0]['value'] : 0;

                      // Get item percentage
                      $field_name = 'field_item_perc_' . $stat_name;
                      $percentage = isset($this_item->{$field_name}[$item_lang][0]['value']) ?
                        $this_item->{$field_name}[$item_lang][0]['value'] : 0;

                      // Get item overlap
                      $field_name = 'field_item_over_' . $stat_name;
                      $overlap = isset($this_item->{$field_name}[$item_lang][0]['value']) ?
                        $this_item->{$field_name}[$item_lang][0]['value'] : 0;

                      // Get item required
                      $field_name = 'field_item_required_' . $stat_name;
                      $required = isset($this_item->{$field_name}[$item_lang][0]['value']) ?
                        $this_item->{$field_name}[$item_lang][0]['value'] : 0;

                      // Get item penalty
                      $field_name = 'field_item_pen_' . $stat_name;
                      $penalty = isset($this_item->{$field_name}[$item_lang][0]['value']) ?
                        $this_item->{$field_name}[$item_lang][0]['value'] : 0;

                      // Set modifier from the item stat and stat affected.
                      if ($overlap != 0) {
                        $overlaps[$stat_name] = $overlap;
                        $stat = $overlap;
                      }
                      if ($modify != 0) {
                        $modifies[$stat_name] = $modify;
                      }
                      if ($percentage != 0) {
                        $this_modifier = isset($modifies[$stat_name]) ? $modifies[$stat_name] : 0;
                        $modifies[$stat_name] = $this_modifier + (($percentage/100) * $stat);
                        $percent_message[$stat_name] = $percentage . '%';
                      }
                      if ($penalty != 0) {
                        $penalties[$stat_name] = $penalty;
                      }
                      if ($stat < $required) {
                        $qualifies = FALSE;
                      }
                    } // End loop for each stat for this item.

                    // Once gathered, make sure we go through all relevant penalities from items based on mods.
                    if (!$qualifies) {
                      // This item triggers a penalty! So we need to go through the loop again.
                      // Add all penalities for this item to the modifiers.
                      foreach ($stat_set as $stat_name) {
                        if (isset($penalties[$stat_name])) {
                          $this_modifier = isset($modifies[$stat_name]) ? $modifies[$stat_name] : 0;
                          $modifies[$stat_name] = $this_modifier + $penalties[$stat_name];
                        }
                      }
                    }
                    // Condense the skill mods to the stat, based on if a skill was selected for this item use.
                    if (isset($modifies['skill']) && $modifies['skill'] != 0 && isset($roll['skill']) && $roll['skill'] != -1) {
                      $skill_mod = isset($modifies[$this_stat]) ? $modifies[$this_stat] : 0;
                      $modifies[$this_stat] = $skill_mod + $modifies['skill'];
                    }

                    // Set descriptive text, this item affects the selected stat so we'll need this.
                    if (isset($modifies[$this_stat]) && $modifies[$this_stat] != 0 ) {
                      if (!empty($percent_message[$this_stat])) {
                        $item_notes .= ' + ' . $this_item->title . ' (' . $percent_message[$this_stat] . ' : ' . $modifies[$this_stat] . ')';
                      }
                      else {
                        $item_notes .= ' + ' . $this_item->title . ' (' . $modifies[$this_stat] . ')';
                      }
                      $item_mod += $modifies[$this_stat];
                    }
                    // Add percentage descriptions to mod_note. TXT percentages that need to be removed over time.
                    if (isset($this_item->field_percents[$item_lang][0]['value'])
                      && !empty($this_item->field_percents[$item_lang][0]['value'])) {
                      $item_notes .= ' + ' . $this_item->title . ' (' .
                        $this_item->field_percents[$item_lang][0]['value'] . ')';
                    }
                    // Adjust for overlap.
                    if (isset($overlaps[$this_stat])) {
                      $overlap_mod = $overlaps[$this_stat];
                    }
                  } // End if item exists.
                } // End loop per item.
              } // End if character set.
            } // End if items set.
            // End item information gathered for this stat.

            // Build up the command for the given stat based on character and gathered item information.
            $mod_note = '';
            $mod = 0;

            // If stat is selected, grab the basic stat OR the overlap if it's been set.
            if (isset($roll['stat']) && $roll['stat'] != -1) {
              // Check for overlap from an item.
              if (isset($overlap_mod) && !empty($overlap_mod)) {
                $small_label = substr($this_stat, 0, 3);
                if ($small_label == 'ref') {
                  $small_label = 'fin';
                }
                $mod_note .= ' + item ' . $small_label . ' (' . $overlap_mod . ')';
                $mod += $overlap_mod;
              }
              // No overlap, pull from the character sheet.
              elseif (isset($content->field_comment_character[$lang][0]['nid'])) {
                $character = node_load($content->field_comment_character[$lang][0]['nid']);
                $stat = isset($character->{$roll['stat']}[$lang][0]['value']) ? $character->{$roll['stat']}[$lang][0]['value'] : 0;
                $small_label = substr($this_stat, 0, 3);
                if ($small_label == 'ref') {
                  $small_label = 'fin';
                }
                $mod_note .= ' + ' . $small_label . ' (' . $stat . ')';
                $mod += $stat;
              }
            }

            // If skill selected, grab the skill for mods.
            if (isset($roll['skill']) && $roll['skill'] != -1) {
              $skill = field_collection_item_load($roll['skill']);
              $skill_value = isset($skill->field_skill_value[$lang][0]['value']) ? $skill->field_skill_value[$lang][0]['value'] : 0;
              $skill_name = isset($skill->field_skill_name[$lang][0]['value']) ? $skill->field_skill_name[$lang][0]['value'] : 'skill';
              $skill_desc = isset($skill->field_skill_desc[$lang][0]['value']) ? $skill->field_skill_desc[$lang][0]['value'] : '';
              $mod_note .= ' +  <a title="' . $skill_desc . '" href="#">' . $skill_name . '</a> (' . $skill_value . ')';
              $mod += $skill_value;
            }

            // Add item mods.
            if (isset($item_notes) && !empty($item_notes)) {
              $this_mod = isset($item_mod) ? $item_mod : 'n/a';
              $mod_note .= ' +  <a title="' . $item_notes . '" href="#">Items</a> (' . $this_mod . ')';
            }
            if (isset($item_mod) && !empty($item_mod)) {
              $mod += $item_mod;
            }

            // And add the command to the roll set.
            if ($mod != 0) {
              $note .= 'Roll '. $delta + 1 .' : roll(' . $this_command . ')' . $mod_note . '<br>';
              $command .= $this_command . '+' . $mod .';';
            }
            else {
              $note .= 'Roll '. $delta + 1 .' : roll(' . $this_command . ')<br>';
              $command .= $this_command .';';
            }

          } // End Combine item info with this dice info.
        } // End Roll for each dice selected.
      } // End check for empty rolls

      // Apply the set of rolls.
      if (!empty($command)) {
        module_load_include('inc', 'dice_roller', 'dice_roller.roller');
        $dids = _dice_roller_parse_roll($command, $note, $content->cid, 'c');
      }
      break;
  }
}