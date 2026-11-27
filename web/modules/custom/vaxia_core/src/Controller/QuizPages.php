<?php

namespace Drupal\vaxia_core\Controller;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Quiz;
use Drupal\webform\WebformSubmissionInterface;

/**
 * Taking a quiz and seeing the result (D7 node/%/take and quiz results).
 */
class QuizPages extends ControllerBase {

  public function take(NodeInterface $node) {
    $webform = $node->get('webform')->entity;
    if (!$webform) {
      return ['#markup' => $this->t('This quiz has no questions yet.')];
    }
    return $webform->getSubmissionForm(['entity_type' => 'node', 'entity_id' => $node->id()]);
  }

  public static function takeAccess(AccountInterface $account, NodeInterface $node) {
    return AccessResult::allowedIf($node->bundle() === 'quiz' && $node->access('view', $account) && $node->hasField('webform'))->addCacheableDependency($node);
  }

  /**
   * The player's score, D7's result text and their answers, marked right or
   * wrong. As D7 quiz_feedback did, the right answer to a question they got
   * wrong isn't revealed. Chosen options are marked as D7 did: right when
   * choosing it scores more than leaving it ("almost" on a single choice with
   * a better-scoring option), else wrong.
   */
  public function result(NodeInterface $node, WebformSubmissionInterface $webform_submission) {
    $handler = Quiz::handler($node);
    $elements = $webform_submission->getWebform()->getElementsDecodedAndFlattened();
    $build = ['#cache' => ['max-age' => 0]];
    $tally = $handler ? $handler->tally($webform_submission) : ['points' => 0, 'possible' => 0, 'percentage' => 0, 'evaluated' => TRUE];
    if (!$tally['evaluated']) {
      $build['not_evaluated'] = ['#markup' => '<p>' . $this->t('Parts of this quiz have not been evaluated yet. The score below is not final.') . '</p>'];
    }
    if ($tally['possible']) {
      $build['points'] = ['#markup' => '<p>' . $this->t('You got %points of %possible possible points.', ['%points' => $tally['points'], '%possible' => $tally['possible']]) . '</p>'];
    }
    $build['score'] = ['#markup' => '<p>' . $this->t('Your score: @score%', ['@score' => $tally['percentage']]) . '</p>'];
    if ($handler && ($summary = $handler->summary((float) $tally['percentage']))) {
      $build['summary'] = ['#type' => 'container', '#attributes' => ['id' => 'quiz_summary'], 'text' => $summary];
    }
    $grades = Quiz::grades((int) $webform_submission->id());
    foreach ($handler ? $handler->questions() : [] as $key => $question) {
      $element = $elements[$key] ?? NULL;
      if (!$element) {
        continue;
      }
      $value = $webform_submission->getElementData($key);
      if (!empty($question['free_text'])) {
        // As D7 long_answer: the score ("?" until scored) and, once scored,
        // the grader's feedback.
        $grade = $grades[$key] ?? NULL;
        $max = (float) ($question['question_max'] ?? 10);
        $weight = $handler->weight($question, $max);
        $build[$key] = [
          'question' => ['#markup' => '<h3>' . htmlspecialchars((string) ($element['#title'] ?? $key)) . '</h3>'],
          'answer' => ['#markup' => '<p>' . nl2br(htmlspecialchars((string) $value)) . '</p>'],
          'score' => ['#markup' => '<p class="quiz-answer-score">' . $this->t('Score: @score of @max', ['@score' => $grade === NULL ? '?' : round((float) $grade['score'] * $weight), '@max' => round($max * $weight)]) . '</p>'],
        ];
        if ($grade !== NULL && trim((string) $grade['feedback']) !== '') {
          $build[$key]['feedback'] = [
            '#type' => 'container',
            '#attributes' => ['class' => ['quiz-answer-feedback']],
            'title' => ['#markup' => '<strong>' . $this->t('Feedback') . '</strong>'],
            'text' => ['#type' => 'processed_text', '#text' => $grade['feedback'], '#format' => $grade['format'] ?? NULL],
          ];
        }
        continue;
      }
      $chosen = array_map('strval', array_filter((array) $value, fn($v) => $v !== NULL && $v !== ''));
      $scores = (array) ($question['scores'] ?? []);
      $best = NULL;
      foreach ($scores as $pair) {
        $best = $best === NULL ? (float) $pair[0] : max($best, (float) $pair[0]);
      }
      $rows = [];
      foreach ($element['#options'] ?? [] as $option => $label) {
        $icon = '';
        if (in_array((string) $option, $chosen, TRUE)) {
          $pair = $scores[$option] ?? [0, 0];
          $icon = '<span class="multichoice-icon wrong" title="' . $this->t('This option is wrong.') . '"></span>';
          if ((float) $pair[0] > (float) $pair[1]) {
            $icon = empty($question['multi']) && empty($question['boolean']) && (float) $pair[0] < $best
              ? '<span class="multichoice-icon almost" title="' . $this->t('This option is correct, but there is another that gives a higher score.') . '"></span>'
              : '<span class="multichoice-icon correct" title="' . $this->t('This option is correct.') . '"></span>';
          }
        }
        $rows[] = [['data' => ['#markup' => $icon], 'class' => ['selector-td', 'multichoice-icon-cell']], $label];
      }
      $build[$key] = [
        'question' => ['#markup' => '<h3>' . htmlspecialchars((string) ($element['#title'] ?? $key)) . '</h3>'],
        'answers' => ['#type' => 'table', '#rows' => $rows],
      ];
    }
    return $build;
  }

  /**
   * Answers waiting to be scored (D7 long_answer "Score long answer
   * questions"): per answer the quiz, question, when the attempt finished and
   * a link to score it, oldest first.
   */
  public function unscored() {
    $rows = [];
    $storage = $this->entityTypeManager()->getStorage('node');
    $formatter = \Drupal::service('date.formatter');
    $db = \Drupal::database();
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(FALSE)->condition('type', 'quiz')->sort('nid')->execute()) as $quiz) {
      $handler = Quiz::handler($quiz);
      $free_text = $handler ? array_filter($handler->questions(), fn($q) => !empty($q['free_text'])) : [];
      if (!$free_text) {
        continue;
      }
      $elements = $quiz->get('webform')->entity->getElementsDecodedAndFlattened();
      foreach ($db->query('SELECT sid, time_end FROM {vaxia_quiz_result} WHERE nid = :nid AND sid IS NOT NULL ORDER BY time_end, rid', [':nid' => $quiz->id()]) as $result) {
        $grades = Quiz::grades((int) $result->sid);
        foreach (array_keys($free_text) as $key) {
          if (isset($grades[$key])) {
            continue;
          }
          $rows[] = [
            $quiz->toLink()->toString(),
            (string) ($elements[$key]['#title'] ?? $key),
            $formatter->format((int) $result->time_end, 'custom', 'Y-m-d H:i'),
            \Drupal\Core\Link::createFromRoute($this->t('Score this response'), 'vaxia_core.quiz_grade', ['node' => $quiz->id(), 'webform_submission' => $result->sid])->toString(),
          ];
        }
      }
    }
    return [
      '#type' => 'table',
      '#header' => [$this->t('Quiz'), $this->t('Question'), $this->t('Finished'), $this->t('Action')],
      '#rows' => $rows,
      '#empty' => $this->t('There are no unscored essays.'),
      '#cache' => ['max-age' => 0],
    ];
  }

  public static function resultAccess(AccountInterface $account, NodeInterface $node, WebformSubmissionInterface $webform_submission) {
    return AccessResult::allowedIf((int) $webform_submission->getOwnerId() === (int) $account->id() || $account->hasPermission('view any webform submission'))->cachePerUser();
  }

}
