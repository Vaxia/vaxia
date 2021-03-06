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
  $stats_rolled = '';
  $rolls_found = array();
  if (isset($dice_rolls[0]['roll_notes'])) {
    $read_rolls = _dice_ruler_read_dice($dice_rolls);
    // An array of each note, in the format: array('0' => "1 : roll(1d100) + agi (26)");
    foreach ($read_rolls['notes'] as $dice_row => $note) {
      // Clip bits out of the note.
      $note = str_replace('<br>', '', $note);
      $note = str_replace('1 : roll(1d100)', '', $note);
      // Get the values from the row.
      $stat = !empty($read_rolls['stat'][$dice_row]) ? $read_rolls['stat'][$dice_row] : 0;
      $stat_name = !empty($read_rolls['stat_name'][$dice_row]) ? $read_rolls['stat_name'][$dice_row] : 0;
      $might = !empty($read_rolls['mights'][$dice_row]) ? $read_rolls['mights'][$dice_row] : 0;
      $roll = !empty($read_rolls['rolls'][$dice_row]) ? $read_rolls['rolls'][$dice_row] : 0;
      // The the first rolls are the rolls. The very last one is the might.
      // If there is no might, leave it empty. For 1-dice rolls.
      $first = ($dice_row == 0) ? ' first ' : '';
      $last = ($dice_row == (count($dice_rolls) - 1)) ? ' last ' : '';
      $even_odd = ($dice_row % 2 == 0) ? ' even ' : ' odd ';
      // Start generating the display.
      $display = array();
      $display[] = '<div class="dice' . $first . $last . $even_odd . '" dice_row="' . $dice_row . '">';
      if (!empty($might)) {
        $note = str_replace('1d100  + ', '', $note);
        $note = str_replace('1d100 + ', '', $note);
        $note = str_replace('1d100  ', '', $note);
        $note = str_replace('1d100 ', '', $note);
        $ruled = 'dice_ruled';
        $display[] = '<span class="dice_render">' . $note . ' =>' . ' <b>' .
          t('Might') . ':</b> ' . $might . ' <b>' . t('Roll') . ':</b> ' . $roll . '</span>';
        $rolls_found[$dice_row] = array('might' => $might, 'roll' => $roll, 'stat_name' => $stat_name, 'stat' => $stat);
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
    // We now know all the stats involved in this roll and have the rolls collected into a sorted array.
    foreach ($rolls_found as $dice_row => $roll) {
      $str_rolls .= '  <span class="dice_set dice_set_' . $dice_row . '" set="' . $dice_row . '">'. "\n";
      $str_rolls .= '    <span class="dice_roll dice_roll_' . $roll['stat_name'] . ' dice_row_' . $dice_row . '" ' .
        'stat="' . $roll['stat'] . '" roll="' . $roll['roll'] . '" might="' . $roll['might'] . '"></span>'. "\n";
      $str_rolls .= '  </span>'. "\n";
    }
    $str_rolls .= '</span>'. "\n";
  }
}
// Echo the rows.
print '<div class="vaxia-theme dice_rolls ' . $ruled . '">' . $str_rolls . '</div>';

// BP: no end.