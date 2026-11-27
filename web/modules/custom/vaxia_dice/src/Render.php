<?php

namespace Drupal\vaxia_dice;

use Drupal\Component\Utility\Xss;

/**
 * Reads stored rolls and renders them (D7 dice_ruler _dice_ruler_read_dice()
 * and the theme's dice-roller-results.tpl.php), markup unchanged:
 * "str (5) + Items (2) => Might: 7 Roll: 79".
 */
class Render {

  /**
   * Breaks rolls into notes, stats, skills, aspects, mights and rolls.
   */
  public static function read(array $dice_rolls): array {
    // dice_ruler's reader (dice_ruler.inc), kept as D7 wrote it.
    if (function_exists('_dice_ruler_read_dice')) {
      return _dice_ruler_read_dice($dice_rolls);
    }
    $read = ['commands' => [], 'results' => [], 'aspects' => [], 'stat_name' => [], 'stat' => [], 'skill' => [], 'mights' => [], 'rolls' => [], 'notes' => []];
    foreach ($dice_rolls as $roll) {
      $aspects = [];
      $stat_name = '';
      $stat = $skill = 0;
      $command = $roll['roll_command'] ?? '';
      $results = $roll['roll_result'] ?? '';
      $note = (string) ($roll['roll_notes'] ?? '');
      $note = str_replace('1 : roll(1d100)', '1d100', $note);
      if (strpos($note, '<a title="') !== FALSE) {
        if (preg_match('/aspects="(.+?)"/', $note, $m)) {
          $aspects = explode(' ', $m[1]);
        }
        if (preg_match('/stat_name="(.+?)"/', $note, $m)) {
          $stat_name = $m[1];
        }
        if (preg_match('/stat="(.+?)"/', $note, $m) && $stat_name !== 'any') {
          $stat = (int) $m[1];
        }
        if (preg_match('/skill="(.+?)"/', $note, $m)) {
          $skill = (int) $m[1];
        }
        if (!$aspects && preg_match('/title=".+?\((.+?)\)"/', $note, $m)) {
          $aspects = explode(', ', $m[1]);
        }
        if (!$skill && preg_match('/title=".+?".+?\((.+?)\)/', $note, $m)) {
          $skill = $m[1];
        }
        $aspects = array_map('trim', $aspects);
      }
      if ($stat_name === '' && preg_match('/(...) \(.+?\)/', $note, $m)) {
        $stat_name = trim($m[1]);
      }
      if (!$stat && preg_match('/... \((.+?)\)/', $note, $m)) {
        $stat = (int) $m[1];
      }
      $values = explode(',', explode('=', $results)[0]);
      $might = 0;
      if (count($values) > 1 && ($stat || $skill)) {
        $might = trim(array_pop($values));
      }
      $read['commands'][] = $command;
      $read['results'][] = $results;
      $read['aspects'][] = $aspects;
      $read['stat_name'][] = $stat_name !== 'any' ? $stat_name : '';
      $read['stat'][] = $stat;
      $read['skill'][] = $skill;
      $read['mights'][] = $might ?: ($stat + $skill);
      $read['rolls'][] = trim(implode(', ', $values));
      $read['notes'][] = $note;
    }
    return $read;
  }

  /**
   * The rolls' markup.
   */
  public static function markup(array $dice_rolls): string {
    $out = '';
    $ruled = 'dice_not_ruled';
    $found = [];
    if ($dice_rolls) {
      $read = self::read($dice_rolls);
      $count = count($dice_rolls);
      foreach ($read['notes'] as $row => $note) {
        // Notes are stored markup (skill links); keep it, minus scripts.
        $note = Xss::filterAdmin(str_replace(['<br>', '1 : roll(1d100)'], '', $note));
        $stat = $read['stat'][$row] ?: 0;
        $stat_name = $read['stat_name'][$row] ?: 0;
        $might = $read['mights'][$row] ?: 0;
        $roll = $read['rolls'][$row] ?: 0;
        $classes = 'dice' . ($row == 0 ? ' first ' : '') . ($row == $count - 1 ? ' last ' : '') . ($row % 2 == 0 ? ' even ' : ' odd ');
        $out .= '<div class="' . $classes . '" dice_row="' . $row . '">';
        if ($might) {
          $note = str_replace(['1d100  + ', '1d100 + ', '1d100  ', '1d100 '], '', $note);
          $ruled = 'dice_ruled';
          $out .= '<span class="dice_render">' . $note . ' => <b>' . t('Might') . ':</b> ' . $might . ' <b>' . t('Roll') . ':</b> ' . $roll . '</span>';
          $found[$row] = ['might' => $might, 'roll' => $roll, 'stat_name' => $stat_name, 'stat' => $stat];
        }
        else {
          $out .= '<span class="dice_render">' . $note . ' => <b>' . t('Roll') . ':</b> ' . $roll . '</span>';
        }
        $out .= '</div>';
      }
      if ($found) {
        $out .= '<span class="dice_sets" style="display:none;">' . "\n";
        foreach ($found as $row => $r) {
          $out .= '  <span class="dice_set dice_set_' . $row . '" set="' . $row . '">' . "\n";
          $out .= '    <span class="dice_roll dice_roll_' . $r['stat_name'] . ' dice_row_' . $row . '" stat="' . $r['stat'] . '" roll="' . $r['roll'] . '" might="' . $r['might'] . '"></span>' . "\n";
          $out .= '  </span>' . "\n";
        }
        $out .= '</span>' . "\n";
      }
    }
    return '<div class="vaxia-theme dice_rolls ' . $ruled . '">' . $out . '</div>';
  }

}
