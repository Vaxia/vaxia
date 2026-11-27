<?php

namespace Drupal\vaxia_core;

use Drupal\Core\Session\AccountInterface;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\webform\WebformSubmissionInterface;

/**
 * Quizzes (D7 quiz module, now scored webforms): results and the "quiz
 * passed" hook other modules react to (D7 Rules event quiz_rules_quiz_passed).
 */
class Quiz {

  /**
   * The score (percent) needed to pass a quiz: its scoring handler's setting,
   * else 75 (D7 quiz's default pass rate).
   */
  public static function passRate(NodeInterface $quiz): float {
    $handler = self::handler($quiz);
    return $handler ? (float) ($handler->getConfiguration()['settings']['pass_rate'] ?? 75) : 75.0;
  }

  /**
   * The quiz's scoring handler, if its webform has one.
   */
  public static function handler(NodeInterface $quiz) {
    $webform = $quiz->hasField('webform') ? $quiz->get('webform')->entity : NULL;
    if (!$webform) {
      return NULL;
    }
    foreach ($webform->getHandlers('vaxia_quiz_score') as $handler) {
      return $handler;
    }
    return NULL;
  }

  /**
   * Records an attempt; a passing one invokes hook_vaxia_quiz_passed(). As in
   * D7 (quiz_is_passed), an attempt still waiting for free-text answers to be
   * graded hasn't passed.
   */
  public static function record(NodeInterface $quiz, AccountInterface $account, float $score, ?int $sid = NULL, bool $evaluated = TRUE): bool {
    $passed = $evaluated && $score >= self::passRate($quiz);
    \Drupal::database()->insert('vaxia_quiz_result')->fields([
      'nid' => $quiz->id(),
      'uid' => $account->id(),
      'sid' => $sid,
      'score' => $score,
      'passed' => (int) $passed,
      'time_end' => \Drupal::time()->getRequestTime(),
    ])->execute();
    if ($passed && $account->id()) {
      \Drupal::moduleHandler()->invokeAll('vaxia_quiz_passed', [$quiz, User::load($account->id())]);
    }
    return $passed;
  }

  /**
   * Grades of an attempt's free-text answers (D7 quiz_long_answer_user_answers
   * score / feedback): element key => ['score' => the question's own score
   * (0 to its maximum, before weighting), 'feedback' => text, 'format' =>
   * text format].
   */
  public static function grades(int $sid): array {
    return (array) \Drupal::keyValue('vaxia_quiz_grades')->get((string) $sid, []);
  }

  public static function setGrades(int $sid, array $grades): void {
    if ($grades) {
      \Drupal::keyValue('vaxia_quiz_grades')->set((string) $sid, $grades);
    }
    else {
      \Drupal::keyValue('vaxia_quiz_grades')->delete((string) $sid);
    }
  }

  /**
   * Rescores a recorded attempt after grading: its score and pass are
   * updated; becoming passed invokes hook_vaxia_quiz_passed() (once: an
   * attempt that had passed already doesn't again). Returns the tally.
   */
  public static function regrade(NodeInterface $quiz, WebformSubmissionInterface $submission): array {
    $handler = self::handler($quiz);
    if (!$handler) {
      return [];
    }
    $tally = $handler->tally($submission);
    $db = \Drupal::database();
    $row = $db->query('SELECT rid, uid, passed FROM {vaxia_quiz_result} WHERE sid = :sid ORDER BY rid DESC', [':sid' => $submission->id()])->fetchObject();
    if (!$row) {
      return $tally;
    }
    $passed = $tally['evaluated'] && (float) $tally['percentage'] >= self::passRate($quiz);
    $db->update('vaxia_quiz_result')->fields(['score' => $tally['percentage'], 'passed' => (int) $passed])->condition('rid', $row->rid)->execute();
    if ($passed && !$row->passed && $row->uid && ($account = User::load($row->uid))) {
      \Drupal::moduleHandler()->invokeAll('vaxia_quiz_passed', [$quiz, $account]);
    }
    return $tally + ['passed' => $passed];
  }

}
