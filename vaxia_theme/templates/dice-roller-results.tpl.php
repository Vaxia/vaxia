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
        $might = array_pop($results);
        $might = explode('=', $might);
        $might = trim($might[0]);
        $roll = implode(', ', $results);
        $str_rolls .= '<div class="dice">' .
          t('!note => Roll: @roll, Might: @might',
            array('!note' => $note, '@command' => $dice_rolls[$index]['roll_command'],
            '@roll' => $roll,
            '@might' => $might)
          ) .
          '</div>'."\n";
      }
    }
  }
  print $str_rolls;
?>
</div>