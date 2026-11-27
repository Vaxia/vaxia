<?php

namespace Drupal\vaxia_dice;

use Drupal\Core\Database\Connection;
use Drupal\Core\Messenger\MessengerInterface;

/**
 * Rolls and stores dice (D7 dice_roller: dice_roller.roller.inc and the
 * storage functions).
 *
 * A command is "NdS" plus optional +/- modifiers ("1d100+45"). A result reads
 * "79, 45 = 124": each die, then the modifier as one more value, then the
 * total. That's the format stored in production and read back by Render.
 */
class Roller {

  public function __construct(
    protected Connection $database,
    protected MessengerInterface $messenger,
  ) {}

  /**
   * Rolls a command ("1d100+45"; several separated by ";") and stores each
   * result against a post. Returns the new roll ids, or FALSE.
   */
  public function rollAndStore(string $command, string $note, int $pid, string $origin = 'c') {
    $ids = [];
    foreach (explode(';', trim($command)) as $single) {
      $result = $this->roll($single);
      if ($result !== FALSE) {
        $ids[] = $this->store($single, $result, $note, $pid, $origin);
      }
      elseif (trim($single) !== '') {
        $this->messenger->addStatus(t('There was an error in rolling your dice selection @command.', ['@command' => $single]));
      }
    }
    return $ids ?: FALSE;
  }

  /**
   * One roll's result text, or FALSE when the command can't be read.
   */
  public function roll(string $command) {
    $command = str_replace([' ', "\t", "\n"], '', trim($command));
    if ($command === '') {
      return FALSE;
    }
    $split = explode('d', $command);
    if (count($split) < 2) {
      return FALSE;
    }
    $number = array_shift($split);
    if (!is_numeric($number) || $number < 1) {
      $this->messenger->addStatus(t('Unable to parse dice roll: No number of dice to roll given.'));
      return FALSE;
    }
    $sides = implode('', $split);
    $addition = 0;
    if (!is_numeric($sides)) {
      $parts = explode('|', str_replace(['+', '-'], ['|+', '|-'], $sides));
      $sides = array_shift($parts);
      foreach ($parts as $part) {
        if (is_numeric($part)) {
          $addition += $part;
        }
      }
    }
    if (!is_numeric($sides) || $sides < 1) {
      $this->messenger->addStatus(t('Unable to parse dice roll: No number of sides for dice given.'));
      return FALSE;
    }
    $values = [];
    for ($i = 0; $i < (int) $number; $i++) {
      $values[] = mt_rand(1, (int) $sides);
    }
    // The modifier counts as one more rolled value.
    if ($addition != 0) {
      $values[] = $addition;
    }
    return implode(', ', $values) . ' = ' . array_sum($values);
  }

  public function store(string $command, string $result, string $note, int $pid, string $origin = 'c'): int {
    return (int) $this->database->insert('dice_roller')->fields([
      'pid' => $pid,
      'origin' => $origin,
      'roll_command' => $command,
      'roll_result' => $result,
      'roll_notes' => $note,
    ])->execute();
  }

  /**
   * A post's rolls, in roll order (rows as arrays).
   */
  public function rolls(int $pid, string $origin = 'c'): array {
    return $this->database->query('SELECT * FROM {dice_roller} WHERE pid = :pid AND origin = :origin ORDER BY did', [':pid' => $pid, ':origin' => $origin])->fetchAll(\PDO::FETCH_ASSOC);
  }

  public function delete(int $pid, string $origin = 'c'): void {
    $this->database->delete('dice_roller')->condition('pid', $pid)->condition('origin', $origin)->execute();
  }

}
