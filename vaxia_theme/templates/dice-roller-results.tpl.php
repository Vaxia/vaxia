<?php
// $Id$

/**
 * @file
 * Implements the default result display theme
 *
 * Incoming variables:  $dice_rolls, an array of roll results.
 */
?>
<div class="dice_rolls"><?php
  $str_rolls = '';
  if (isset($dice_rolls) && is_array($dice_rolls)) {
    $rolls_found = array();
    $stats_rolled = '';
    if (isset($dice_rolls[0]['roll_notes'])) {
      // Notes starts as a list of all rolls and results, separated by "<br>".
      $notes = explode('<br>', $dice_rolls[0]['roll_notes']);
      // An array of each note, in the format: array('0' => "1 : roll(1d100) + agi (26)");
      foreach ($dice_rolls as $index => $dice_roll) {
        $note = '';
        $stat = '';
        // Trim "1 : roll" off the note. New format: roll(1d100) + agi (26)
        if (!empty( $notes[$index + 1] )) {
          $notes[$index + 1] = trim($notes[$index + 1]);
          $note = substr($notes[$index + 1], 8);
        }
        if (!empty($notes[$index+1]) && strpos($notes[$index+1], '+') !== FALSE) {
          // We have a note, let's get the stat.
          $results = explode('+', $note);
          if (isset($results[1])) {
            $results = explode(' ', trim($results[1]));
          }
          $stat = trim($results[0]);
        }
        // The roll and the might are pulled out of the roll results.
        // That starts as a format: 28, 26 = 54, everything after the "=" can be ignored.
        // Since that's not how the Vaxian system works.
        $results = explode('=', $dice_rolls[$index]['roll_result']);
        // Drop everything after the "=" on the floor. We don't need it.
        $results = trim($results[0]);
        // New format "66, 25, 22, 99". Where 99 is the might.
        $results =  explode(',', $results);
        foreach ($results as $index => $value) {
          $results[$index] = trim($value);
          if ($value == '0') {
            results($index[$index]);
          }
        }
        // The the first rolls are the rolls. The very last one is the might.
        // If there is no might, leave it empty. For 1-dice rolls.
        $first = ($index == 0) ? ' first ' : '';
        $last = ($index == (count($dice_rolls) - 1)) ? ' last ' : '';
        $even_odd = ($index % 2 == 0) ? ' even ' : ' odd ';
        if (!empty($stat)) {
          $might = count($results) > 1 ? trim(array_pop($results)) : '';
          // Filter out zero's on non-might rolls.
          $rolls = trim(implode(', ', $results));
          $str_rolls .= '<div class="dice' . $first . $last . $even_odd . '" dice_row="' . $index . '">' . $note . ' =>' .
            ' <b>' . t('Might') . ':</b> ' . $might .
            ' <b>' . t('Roll') . ':</b> ' . $rolls .
            '</div>' . "\n";
            // Add the hidden div for jQuery to tie into for the ruler.
            // All hidden, so no need for text translation here.
          $rolls = explode(',', $rolls);
          foreach ($rolls as $index => $roll) {
            $rolls_found[$index][$stat] = array('might' => $might, 'roll' => trim($roll));
          }
        }
        else {
          // Filter out zero's on non-might rolls.
          $rolls = trim(implode(', ', $results));
          $str_rolls .= '<div class="dice' . $first . $last . $even_odd . '">' . $note . ' => <b>' . t('Roll') . ':</b> ' . $rolls . '</div>' . "\n";
        }
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
          '<span class="dice_roll dice_roll_' . $stat . ' dice_row_' . $dice_roll . '" ' .
          'stat="' . $stat . '" roll="' . $roll['roll'] . '" might="' . $roll['might'] . '"></span>'. "\n";
          $dice_roll++;
      }
      $str_rolls .= '  </span>'. "\n";
    }
    if (!empty($rolls_found)) {
      $str_rolls .= '</span>'. "\n";
    }
  }
  print $str_rolls;
?></div>