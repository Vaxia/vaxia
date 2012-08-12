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
        'field_reflexes' => 'Reflexes', 
        'field_intelligence' => 'Intelligence', 
        'field_awareness' => 'Awareness', 
        'field_spirituality' => 'Spirituality', 
        'field_presence' => 'Presence', 
        'field_charisma' => 'Charima', 
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
        foreach ($character->field_items[$character->language] as $delta => $item) {
          $this_item = node_load($item['nid']);
          $items[$this_item->nid] = $this_item->title;  
        }
      }
      for ($i=0;$i<3;$i++) {
        $form['vaxia_rolls']['dice'][$i] = _vaxia_dice_roller_row($dice_size, $stat, $skills, $items);
      }
      $form['vaxia_rolls']['notes'] = array(
        '#type' => 'textarea',
        '#title' => t('Notes'),
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
function _vaxia_dice_roller_row($dice_size, $stat, $skill, $item) {
  $form['number'] = array(
    '#type' => 'select',
    '#options' => range(0, 10),
  );
  $form['size'] = array(
    '#type' => 'select',
    '#options' => array_combine($dice_size, $dice_size),
  );
  $form['stat'] = array(
    '#type' => 'select',
    '#options' => $stat,
  );
  $form['skill'] = array(
    '#type' => 'select',
    '#options' => $skill,
  );
  $form['item'] = array(
    '#type' => 'select',
    '#options' => $item,
    '#multiple' => FALSE,
  );
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
        foreach ($content->vaxia_rolls['dice'] as $delta => $roll) {
          $lang = $content->language;
          if ($roll['number'] > 0) {
            $mod_note = '';
            $mod = 0;
            if (isset($content->field_comment_character[$lang][0]['nid'])) {
              $character = node_load($content->field_comment_character[$lang][0]['nid']);
              $stat = isset($character->{$roll['stat']}[$lang][0]['value']) ? $character->{$roll['stat']}[$lang][0]['value'] : 0;
              $mod_note .= ' + stat ('.$stat.')';
              $mod += $stat;
            }
            if (isset($roll['skill']) && $roll['skill'] != -1) {
              $skill = field_collection_item_load($roll['skill']); 
              $skill = isset($skill->field_skill_value[$lang][0]['value']) ? $skill->field_skill_value[$lang][0]['value'] : 0;
              $mod_note .= ' + skill ('.$skill.')';
              $mod += $skill;
            }
            if (isset($roll['item']) && $roll['item'] != -1) {
              foreach ($roll['item'] as $this_item) {
                $item = node_load($this_item);
                $item_stat = _vaxia_dice_map_stat_to_item($roll['stat']);
                $item += isset($item->{$item_stat}[$lang][0]['value']) ? $item->{$item_stat}[$lang][0]['value'] : 0;
                $mod_note .= ' + item ('.$item.')';
                $mod += $item;
              }
            }
            $this_command = $roll['number'] . 'd' . $roll['size'];
            $note .= 'Roll '. $delta + 1 .' : roll(' . $this_command . ')' . $mod_note . '<br>';
            $command .= $this_command . '+' . $mod .';';
          }
        }
      }
      if (!empty($command)) {
        module_load_include('inc', 'dice_roller', 'dice_roller.roller');
        $dids = _dice_roller_parse_roll($command, $note, $content->cid, 'c');
      }
      break;
  }
}

function _vaxia_dice_map_stat_to_item($stat_name) {
  $map = array(
    'field_life' => 'field_item_life', 
    'field_health' => 'field_item_health', 
    'field_endurance' => 'field_item_endurance', 
    'field_constitution' => 'field_item_constitution', 
    'field_strength' => 'field_item_strength', 
    'field_agility' => 'field_item_agility', 
    'field_dexterity' => 'field_item_dexterity', 
    'field_reflexes' => 'field_item_reflexes', 
    'field_intelligence' => 'field_intelligence', 
    'field_awareness' => 'field_item_awareness', 
    'field_spirituality' => 'field_item_spirituality', 
    'field_presence' => 'field_item_presence', 
    'field_charisma' => 'field_item_charisma', 
    'field_appearance' => 'field_item_appearance',
  );
  return isset($map[$stat_name]) ? $map[$stat_name] : $stat_name;
}
