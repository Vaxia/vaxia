<?php

namespace Drupal\vaxia_core\Plugin\WebformHandler;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Quiz;
use Drupal\webform\Plugin\WebformHandlerBase;
use Drupal\webform\WebformSubmissionInterface;

/**
 * Scores a quiz as D7 quiz 7.x-4.0-rc3 did and records the attempt.
 *
 * Settings:
 * - pass_rate: percent needed to pass (0: everyone who is graded passes).
 * - questions: element key => scoring, as D7 kept it per question revision:
 *   - max_score: the question's points in this quiz (D7 quiz_node_relationship
 *     max_score), which weights it;
 *   - question_max: the question's own maximum (D7 quiz_question_properties);
 *   - multiple choice: multi (several options may be chosen), boolean (all or
 *     nothing) and scores: option key => [score if chosen, score if not
 *     chosen];
 *   - free text: free_text TRUE (D7 long/short answers, graded by hand on
 *     node/NID/results/SID; grades kept by Quiz::grades()).
 * - answers: element key => the one right option, for quizzes without
 *   "questions" (scored as one point for it) and for the result page marks.
 * - randomize: shuffle the questions on each take (D7 randomization 1).
 * - summary_pass / summary_default (+ _format): D7's result texts.
 * - takes: D7 allowed attempts (0: any); the "already submitted" notice is
 *   shown only when limited.
 *
 * Per question D7 scored a multiple-choice answer as the sum of each option's
 * "chosen" or "not chosen" score (all or nothing, worth 1, for boolean
 * questions or questions without a positive score), an unanswered single
 * choice as 0 (skipped), and a free-text answer by its grade (0 until
 * graded). Points are
 * round(score * max_score / question_max); the quiz score is
 * round(100 * points / possible points) (100 with nothing to score). An
 * attempt with free-text answers isn't graded yet: it isn't passed.
 *
 * @WebformHandler(
 *   id = "vaxia_quiz_score",
 *   label = @Translation("Quiz score"),
 *   category = @Translation("VAXIA"),
 *   description = @Translation("Scores the submission as a quiz (D7 quiz module)."),
 *   cardinality = \Drupal\webform\Plugin\WebformHandlerInterface::CARDINALITY_SINGLE,
 *   results = \Drupal\webform\Plugin\WebformHandlerInterface::RESULTS_PROCESSED,
 *   submission = \Drupal\webform\Plugin\WebformHandlerInterface::SUBMISSION_OPTIONAL,
 * )
 */
class QuizScoreHandler extends WebformHandlerBase {

  public function defaultConfiguration() {
    return [
      'answers' => [],
      'pass_rate' => 75,
      'questions' => [],
      'randomize' => FALSE,
      'summary_pass' => '',
      'summary_pass_format' => 'wiki',
      'summary_default' => '',
      'summary_default_format' => 'wiki',
      'takes' => 0,
    ];
  }

  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $form['pass_rate'] = ['#type' => 'number', '#title' => $this->t('Pass rate (%)'), '#default_value' => $this->configuration['pass_rate'], '#min' => 0, '#max' => 100];
    $form['answers'] = [
      '#type' => 'webform_mapping',
      '#title' => $this->t('Answer key'),
      '#description' => $this->t('For each question, the key of the correct option (questions with per-option scores from D7 are scored by those).'),
      '#source' => array_map(fn($e) => $e['#title'] ?? '', array_filter($this->getWebform()->getElementsDecodedAndFlattened(), fn($e) => in_array($e['#type'] ?? '', ['radios', 'select'], TRUE))),
      '#destination_type' => 'textfield',
      '#default_value' => $this->configuration['answers'],
    ];
    $form['randomize'] = ['#type' => 'checkbox', '#title' => $this->t('Random question order'), '#default_value' => $this->configuration['randomize']];
    $form['takes'] = ['#type' => 'number', '#title' => $this->t('Allowed attempts (0: unlimited)'), '#default_value' => $this->configuration['takes'], '#min' => 0];
    $form['summary_pass'] = ['#type' => 'textarea', '#title' => $this->t('Result text when passed'), '#default_value' => $this->configuration['summary_pass']];
    $form['summary_default'] = ['#type' => 'textarea', '#title' => $this->t('Result text otherwise'), '#default_value' => $this->configuration['summary_default']];
    return $form;
  }

  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    parent::submitConfigurationForm($form, $form_state);
    $this->configuration['pass_rate'] = (float) $form_state->getValue('pass_rate');
    $this->configuration['answers'] = array_filter((array) $form_state->getValue('answers'));
    $this->configuration['randomize'] = (bool) $form_state->getValue('randomize');
    $this->configuration['takes'] = (int) $form_state->getValue('takes');
    $this->configuration['summary_pass'] = (string) $form_state->getValue('summary_pass');
    $this->configuration['summary_default'] = (string) $form_state->getValue('summary_default');
  }

  /**
   * Scoring per question: the "questions" setting, else one point for the
   * answer-key option of each question in "answers".
   */
  public function questions(): array {
    $questions = (array) ($this->configuration['questions'] ?? []);
    foreach ((array) $this->configuration['answers'] as $key => $correct) {
      if (!isset($questions[$key])) {
        $questions[$key] = ['max_score' => 1, 'question_max' => 1, 'scores' => [(string) $correct => [1, 0]]];
      }
    }
    return $questions;
  }

  /**
   * D7's scoring of one multiple-choice answer: [score, maximum score].
   */
  public function questionScore(string $key, array $question, array $chosen): array {
    $element = $this->getWebform()->getElement($key) ?: [];
    $scores = [];
    foreach (array_keys($element['#options'] ?? []) as $option) {
      $scores[(string) $option] = [0, 0];
    }
    foreach ((array) ($question['scores'] ?? []) as $option => $pair) {
      $scores[(string) $option] = [(float) $pair[0], (float) $pair[1]];
    }
    $chosen = array_map('strval', $chosen);
    $max = 0;
    $all_wrong = TRUE;
    foreach ($scores as $pair) {
      $max = empty($question['multi']) ? max($max, $pair[0], $pair[1]) : $max + max($pair[0], $pair[1]);
      if ($pair[0] > 0 || $pair[1] > 0) {
        $all_wrong = FALSE;
      }
    }
    $max = empty($question['boolean']) ? max($max, 1) : 1;
    if (!empty($question['boolean']) || $all_wrong) {
      $score = $max;
      foreach ($scores as $option => $pair) {
        $picked = in_array((string) $option, $chosen, TRUE);
        if (($picked && $pair[0] <= $pair[1]) || (!$picked && $pair[0] > $pair[1])) {
          $score = 0;
        }
      }
    }
    else {
      $score = 0;
      foreach ($scores as $option => $pair) {
        $score += in_array((string) $option, $chosen, TRUE) ? $pair[0] : $pair[1];
      }
    }
    return [$score, $max];
  }

  /**
   * The attempt's tally: points, possible points, percentage (D7 rounding)
   * and whether it is fully graded (no free-text answers waiting).
   */
  public function tally(WebformSubmissionInterface $submission): array {
    $points = 0;
    $possible = 0;
    $evaluated = TRUE;
    $grades = $submission->id() ? Quiz::grades((int) $submission->id()) : [];
    foreach ($this->questions() as $key => $question) {
      if (!empty($question['free_text'])) {
        // Graded by hand (Quiz::grades()); 0 and not evaluated until then.
        $grade = $grades[$key] ?? NULL;
        $score = $grade === NULL ? 0 : (float) $grade['score'];
        $max = (float) ($question['question_max'] ?? 10);
        if ($grade === NULL) {
          $evaluated = FALSE;
        }
      }
      else {
        $value = $submission->getElementData($key);
        $chosen = array_values(array_filter((array) $value, fn($v) => $v !== NULL && $v !== ''));
        [$score, $max] = $this->questionScore((string) $key, $question, $chosen);
        if (!$chosen && empty($question['multi'])) {
          // A single choice left unanswered was a skipped question.
          $score = 0;
        }
      }
      $question_max = (float) ($question['question_max'] ?? $max);
      $weight = $this->weight($question, $max);
      $points += round($score * $weight);
      $possible += round($question_max * $weight);
    }
    return [
      'points' => $points,
      'possible' => $possible,
      'percentage' => $possible == 0 ? 100 : round($points * 100 / $possible),
      'evaluated' => $evaluated,
    ];
  }

  /**
   * A question's weight in the quiz: its points in the quiz per point of its
   * own maximum (D7 score_weight).
   */
  public function weight(array $question, float $max): float {
    $question_max = (float) ($question['question_max'] ?? $max);
    $weighted = (float) ($question['max_score'] ?? $question_max);
    return ($weighted == 0 || $question_max == 0) ? 0 : $weighted / $question_max;
  }

  /**
   * The quiz score in percent (D7 rounds to whole percents).
   */
  public function score(WebformSubmissionInterface $submission): float {
    return (float) $this->tally($submission)['percentage'];
  }

  /**
   * D7's result text for a score: the pass text when the quiz has a pass
   * rate and the score reaches it, else the default text.
   */
  public function summary(float $score): array {
    $config = $this->configuration;
    if ((float) $config['pass_rate'] > 0 && $score >= (float) $config['pass_rate']) {
      $text = trim((string) $config['summary_pass']) !== '' ? $config['summary_pass'] : '';
      $format = $config['summary_pass_format'];
    }
    else {
      $text = trim((string) $config['summary_default']) !== '' ? $config['summary_default'] : '';
      $format = $config['summary_default_format'];
    }
    return $text === '' ? [] : ['#type' => 'processed_text', '#text' => $text, '#format' => $format ?: 'wiki'];
  }

  /**
   * Keeps the webform in step with the allowed attempts: D7 quizzes could be
   * retaken unless "takes" limited them, so the "already submitted" notice
   * and the per-user limit are on only when attempts are limited.
   */
  public function createHandler() {
    $webform = $this->getWebform();
    $takes = (int) $this->configuration['takes'];
    if ((bool) $webform->getSetting('form_previous_submissions') !== $takes > 0 || (int) $webform->getSetting('limit_user') !== $takes) {
      $webform->setSetting('form_previous_submissions', $takes > 0);
      $webform->setSetting('limit_user', $takes ?: NULL);
      $webform->save();
    }
  }

  public function updateHandler() {
    $this->createHandler();
  }

  public function alterForm(array &$form, FormStateInterface $form_state, WebformSubmissionInterface $webform_submission) {
    if (empty($this->configuration['randomize']) || empty($form['elements'])) {
      return;
    }
    $keys = array_values(array_filter(array_keys($form['elements']), fn($key) => is_string($key) && $key[0] !== '#' && isset($this->questions()[$key])));
    $shuffled = $keys;
    shuffle($shuffled);
    $elements = [];
    foreach ($form['elements'] as $key => $element) {
      // Questions take the shuffled places; everything else stays put.
      $position = array_search($key, $keys, TRUE);
      if ($position !== FALSE) {
        $key = $shuffled[$position];
        $element = $form['elements'][$key];
      }
      $elements[$key] = $element;
    }
    $weight = 0;
    foreach ($elements as $key => &$element) {
      if (is_string($key) && $key[0] !== '#' && is_array($element)) {
        $element['#weight'] = $weight++;
      }
    }
    unset($element);
    $form['elements'] = $elements;
  }

  public function postDelete(WebformSubmissionInterface $submission) {
    Quiz::setGrades((int) $submission->id(), []);
  }

  public function postSave(WebformSubmissionInterface $submission, $update = TRUE) {
    $quiz = $submission->getSourceEntity();
    if ($update || !$quiz instanceof NodeInterface) {
      return;
    }
    $tally = $this->tally($submission);
    Quiz::record($quiz, \Drupal::currentUser(), (float) $tally['percentage'], (int) $submission->id(), $tally['evaluated']);
  }

  public function confirmForm(array &$form, FormStateInterface $form_state, WebformSubmissionInterface $submission) {
    $quiz = $submission->getSourceEntity();
    if ($quiz instanceof NodeInterface) {
      $form_state->setRedirectUrl(Url::fromRoute('vaxia_core.quiz_result', ['node' => $quiz->id(), 'webform_submission' => $submission->id()]));
    }
  }

}
