<?php
// $Id$

/**
 * @file
 * Implements the default result display theme
 *
 * Incoming variables:  $dice_rolls, an array of roll results.
 */
?>
<div class="dice_rolls">
<?php
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
        // Trim "1 : roll" off the note. New format: roll(1d100) + agi (26)
        if (isset($notes[$index + 1])) {
          $note = substr($notes[$index + 1], 8);
          // We have a note, let's get the stat.
          $results = explode('+', $note);
          $results = explode(' ', trim($results[1]));
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
        // The the first rolls are the rolls. The very last one is the might.
        // If there is no might, leave it empty. For 1-dice rolls.
        $might = count($results) > 1 ? trim(array_pop($results)) : '';
        $rolls = trim(implode(', ', $results));
        if (!empty($might)) {
          $str_rolls .= '<div class="dice">' . $note . ' => ' .
            '<b>' . t('Roll') . ':</b> ' . $rolls . ' <b>' . t('Might') . ':</b> ' . $might . '</div>' . "\n";
            // Add the hidden div for jQuery to tie into for the ruler.
            // All hidden, so no need for text translation here.
          $rolls = explode(',', $rolls);
          foreach ($rolls as $index => $roll) {
            $rolls_found[$index][$stat] = array('might' => $might, 'roll' => trim($roll));
          }
        }
        else {
          $str_rolls .= '<div class="dice">' . $note . ' => <b>' . t('Roll') . ':</b> ' . $roll . '</div>' . "\n";
        }
      } // End loop processing this roll.
    } // End if protecting the roll set.
    // We now know all the stats involved in this roll and have the rolls collected into a sorted array.
    foreach ($rolls_found as $index => $roll_set) {
      $roll_type = 'unknown';
      if (count(array_keys($roll_set)) == 1) {
        $roll_type = 'one-trait';
      }
      if (count(array_keys($roll_set)) == 2) {
        $roll_type = 'two-trait';
        $magic_stats = array('int', 'spi');
        $diffs = array_diff(array_keys($roll_set), $magic_stats);
        if (empty($diffs)) {
          $roll_type = 'magic';
        }
      }
      if (count(array_keys($roll_set)) == 3) {
        $magic_stats = array('dex', 'str', 'end');
        $diffs = array_diff(array_keys($roll_set), $magic_stats);
        if (empty($diffs)) {
          $roll_type = 'combat';
        }
      }
      if ($roll_type != 'unknown') {
        $str_rolls .= 
          '<span class="dice_rolls">'. "\n"  .
          '  <span class="suggested_rule" style="display:none;">' . $roll_type . '</span>'. "\n";
          $count = 0;
          foreach ($roll_set as $stat => $roll) {
            $str_rolls .= 
              '  <span class="dice_roll dice_roll_' . $stat . ' dice_roll_' . $count . '" style="display:none;">' . "\n" .
              '    <span class="stat">' . $stat . '</span>' . "\n" .
              '    <span class="roll">' . $roll['roll'] . '</span>' . "\n" .
              '    <span class="might">' . $roll['might'] . '</span>' . "\n" .
              '  </span>'. "\n";
              $count++;
          }
          $str_rolls .=
          '</span>'. "\n";
      }
    }
  }
  print $str_rolls;
?>
</div>