<?php
// $Id$

/**
 * @file
 * Implements the default result display theme
 *
 * Incoming variables:  $dice_rolls, an array of roll results.
 */

$str_rolls = '';
$ruled = 'dice_not_ruled';
if (isset($dice_rolls) && is_array($dice_rolls) && !empty($dice_rolls)) {
  $read_rolls = _dice_ruler_read_dice($dice_rolls);
  $rolls_found = array();
  $stats_rolled = '';
  if (isset($dice_rolls[0]['roll_notes'])) {
    // Notes starts as a list of all rolls and results, separated by "<br>".
    $notes = explode('<br>', $dice_rolls[0]['roll_notes']);
    // An array of each note, in the format: array('0' => "1 : roll(1d100) + agi (26)");
    foreach ($read_rolls['notes'] as $dice_row => $note) {
      $stat = '';
      $display = array();
      // Trim "1 : roll" off the note. New format: roll(1d100) + agi (26)
      if (!empty($note)) {
        $note = substr(trim($note), 8);
      }
      // Get the values from the row.
      $stat = !empty($read_rolls['stat'][$dice_row]) ? $read_rolls['stat'][$dice_row] : 0;
      $stat_name = !empty($read_rolls['stat_name'][$dice_row]) ? $read_rolls['stat_name'][$dice_row] : 0;
      $skill = !empty($read_rolls['skill'][$dice_row]) ? $read_rolls['skill'][$dice_row] : 0;
      $might = !empty($read_rolls['mights'][$dice_row]) ? $read_rolls['mights'][$dice_row] : 0;
      $roll = !empty($read_rolls['rolls'][$dice_row]) ? $read_rolls['rolls'][$dice_row] : 0;
      // Figure out the ass for the display.
      // The the first rolls are the rolls. The very last one is the might.
      // If there is no might, leave it empty. For 1-dice rolls.
      $first = ($dice_row == 0) ? ' first ' : '';
      $last = ($dice_row == (count($dice_rolls) - 1)) ? ' last ' : '';
      $even_odd = ($dice_row % 2 == 0) ? ' even ' : ' odd ';
      // Start generating the display.
      $display[] = '<div class="dice' . $first . $last . $even_odd . '" dice_row="' . $dice_row . '">';
      if (!empty($stat) || !empty($skill)) {
        $ruled = 'dice_ruled';
        $display[] = '<span class="dice_render">' . $note . ' =>' . ' <b>' . t('Might') . ':</b> ' . $might . ' <b>' . t('Roll') . ':</b> ' . $roll . '</span>';
        $rolls_found[$dice_row][$stat] = array('might' => $might, 'roll' => $roll, 'stat_name' => $stat_name);
      }
      else {
        $display[] = '<span class="dice_render">' . $note . ' => ' . '<b>' . t('Roll') . ':</b> ' . $roll . '</span>';
      } 
      $display[] = '</div>';
      $str_rolls .= implode('', $display);
    } // End loop processing this roll.
  } // End if protecting the roll set.
  if (!empty($rolls_found)) {
    $str_rolls .= '<span class="dice_sets" style="display:none;">'. "\n";
  }
  // We now know all the stats involved in this roll and have the rolls collected into a sorted array.
  foreach ($rolls_found as $index => $roll_set) {
    $str_rolls .= '  <span class="dice_set dice_set_' . $index . '" set="' . $index . '">'. "\n";
    $dice_roll = 0;
    foreach ($roll_set as $stat => $roll) {
      $str_rolls .= '    ' .
        '<span class="dice_roll dice_roll_' . $roll['stat_name'] . ' dice_row_' . $dice_roll . '" ' .
        'stat="' . $stat . '" roll="' . $roll['roll'] . '" might="' . $roll['might'] . '"></span>'. "\n";
        $dice_roll++;
    }
    $str_rolls .= '  </span>'. "\n";
  }
  if (!empty($rolls_found)) {
    $str_rolls .= '</span>'. "\n";
  }
}
// Echo the rows.
print '<div class="dice_rolls ' . $ruled . '">' . $str_rolls . '</div>';

// BP: no end.