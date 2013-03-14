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
    if (isset($dice_rolls[0]['roll_notes'])) {
      $notes = explode('<br>', $dice_rolls[0]['roll_notes']);
      foreach ($dice_rolls as $index => $dice_roll) {
        $note = substr($notes[$index + 1], 8);
        $results = explode(',', $dice_rolls[$index]['roll_result']);
        $mod = array_pop($results);
        foreach ($results as $index => $result) {
          $results[$index] += $mod;
        }
        $result = implode(', ', $results);
        $str_rolls .= '<div class="dice">' .
          t('@note => @results',
            array('@note' => $note, '@command' => $dice_rolls[$index]['roll_command'] . 'Might:' . $mod, '@results' => $result)
          ) .
          '</div>'."\n";
      }
    }
  }
  print $str_rolls;
?>
</div>