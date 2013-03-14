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
        $note = '';
        if (isset($notes[$index + 1])) {
          $note = substr($notes[$index + 1], 8);
        }
        $results = explode(',', $dice_rolls[$index]['roll_result']);
        $mod = array_pop($results);
        $mod = explode('=', $mod);
        $mod = trim($mod[0]);
        foreach ($results as $result_index => $result) {
          $results[$result_index] += $mod;
        }
        $result = implode(', ', $results);
        $str_rolls .= '<div class="dice">' .
          t('!note (Might: @might) => @results',
            array('!note' => $note, '@command' => $dice_rolls[$index]['roll_command'],
            '@might' => $mod,
            '@results' => $result)
          ) .
          '</div>'."\n";
      }
    }
  }
  print $str_rolls;
?>
</div>