<?php

namespace Drupal\vaxia_dice\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Render\Markup;

/**
 * How the last 1000 d100 rolls spread over 1-100 (D7 dice_helper's proof page).
 */
class DiceProof extends ControllerBase {

  public function page() {
    $results = \Drupal::database()->queryRange("SELECT roll_result FROM {dice_roller} WHERE roll_command LIKE '1d100%' ORDER BY did DESC", 0, 1000)->fetchCol();
    $set = [];
    $count = 0;
    $sum = 0;
    foreach ($results as $result) {
      $roll = trim(explode(',', explode('=', $result)[0])[0]);
      if (is_numeric($roll)) {
        $count++;
        $sum += $roll;
        $set[$roll] = ($set[$roll] ?? 0) + 1;
      }
    }
    // D7 divided by the count even with no rolls (known bug).
    $average = $count ? $sum / $count : 0;
    $output = '<style>.dice_proof_label{padding-left:0.5em;}.dice_proof_average{border-right:solid 1px #111;display:block;}.dice_proof_value{background:green;display:block;}</style>';
    $output .= '<div class="dice_proof_title">Across ' . $count . ' Rolls:</div>';
    $counts = array_sum($set) / 100;
    $cut = $counts > 100 ? $counts / 2 : 0;
    $mark = $counts - $cut;
    for ($n = 1; $n < 101; $n++) {
      $width = ($set[$n] ?? 0) - $cut;
      $output .= '<div><span class="dice_proof_value" style="width:' . $width . 'em;"><span class="dice_proof_average" style="width:' . $mark . 'em;">&nbsp;<span class="dice_proof_label">' . $n . '</span></span></span></div>';
    }
    $output .= '<div class="dice_proof_title">Expected mathematical average of all rolls: 50 / 51</div>';
    $output .= '<div class="dice_proof_title">Actual mathematical average of all rolls: ' . $average . '</div>';
    // Built only from numbers.
    return ['#markup' => Markup::create($output), '#cache' => ['max-age' => 0]];
  }

}
