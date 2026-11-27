<?php

namespace Drupal\vaxia_core\Form;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;
use Drupal\Core\Url;
use Drupal\node\NodeInterface;
use Drupal\vaxia_core\Quiz;
use Drupal\webform\WebformSubmissionInterface;

/**
 * Scoring a quiz attempt's free-text answers (D7 quiz node/%/results/% for
 * "score any quiz"): per answer, points from 0 to the question's points in
 * the quiz, and feedback for the player. Once every answer is scored the
 * attempt is evaluated: its score is recomputed and it passes if it reaches
 * the pass rate (Quiz::regrade()).
 */
class QuizGradeForm extends FormBase {

  public function getFormId() {
    return 'vaxia_core_quiz_grade_form';
  }

  public static function access(NodeInterface $node, WebformSubmissionInterface $webform_submission) {
    $handler = Quiz::handler($node);
    $free_text = $handler ? array_filter($handler->questions(), fn($q) => !empty($q['free_text'])) : [];
    return AccessResult::allowedIf($node->bundle() === 'quiz' && $free_text
      && $webform_submission->getSourceEntity() && $webform_submission->getSourceEntity()->getEntityTypeId() === 'node'
      && (int) $webform_submission->getSourceEntity()->id() === (int) $node->id())
      ->addCacheableDependency($node)->addCacheableDependency($webform_submission);
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?NodeInterface $node = NULL, ?WebformSubmissionInterface $webform_submission = NULL) {
    $form_state->set('quiz', $node);
    $form_state->set('submission', $webform_submission);
    $handler = Quiz::handler($node);
    $tally = $handler->tally($webform_submission);
    $elements = $webform_submission->getWebform()->getElementsDecodedAndFlattened();
    $grades = Quiz::grades((int) $webform_submission->id());
    $owner = $webform_submission->getOwner();
    $form['#title'] = $this->t('Score quiz answers');
    $form['intro'] = ['#markup' => '<p>' . $this->t('@quiz, taken by @name on @date. Current score: @score% (@points of @possible points)@pending.', [
      '@quiz' => $node->label(),
      '@name' => $owner ? $owner->getDisplayName() : $this->t('Anonymous'),
      '@date' => \Drupal::service('date.formatter')->format((int) $webform_submission->getCompletedTime(), 'custom', 'Y-m-d H:i'),
      '@score' => $tally['percentage'],
      '@points' => $tally['points'],
      '@possible' => $tally['possible'],
      '@pending' => $tally['evaluated'] ? '' : $this->t(', not final until every answer is scored'),
    ]) . '</p>'];
    $form['answers'] = ['#tree' => TRUE];
    foreach ($handler->questions() as $key => $question) {
      if (empty($question['free_text'])) {
        continue;
      }
      $max = (float) ($question['question_max'] ?? 10);
      $weight = $handler->weight($question, $max);
      $points_max = (int) round($max * $weight);
      $grade = $grades[$key] ?? NULL;
      $form['answers'][$key] = [
        '#type' => 'fieldset',
        '#title' => (string) ($elements[$key]['#title'] ?? $key),
        'answer' => ['#markup' => '<div class="quiz-long-answer">' . nl2br(htmlspecialchars((string) $webform_submission->getElementData($key))) . '</div>'],
        'score' => [
          // A text box, as D7's: its range is checked with D7's message.
          '#type' => 'textfield',
          '#title' => $this->t('Score (0 to @max)', ['@max' => $points_max]),
          '#size' => 3,
          '#maxlength' => 3,
          '#default_value' => $grade === NULL ? '' : round((float) $grade['score'] * $weight),
          '#description' => $grade === NULL ? $this->t('Not scored yet.') : '',
        ],
        'feedback' => [
          '#type' => 'text_format',
          '#title' => $this->t('Enter feedback'),
          '#default_value' => $grade['feedback'] ?? '',
          '#format' => $grade['format'] ?? NULL,
        ],
        '#points_max' => $points_max,
        '#weight_factor' => $weight,
      ];
    }
    $form['actions'] = ['#type' => 'actions', 'submit' => ['#type' => 'submit', '#value' => $this->t('Save score')]];
    return $form;
  }

  public function validateForm(array &$form, FormStateInterface $form_state) {
    foreach (Element::children($form['answers']) as $key) {
      $value = $form_state->getValue(['answers', $key, 'score']);
      if ($value !== '' && $value !== NULL && (!is_numeric($value) || (float) $value != (int) $value || $value < 0 || $value > $form['answers'][$key]['#points_max'])) {
        $form_state->setErrorByName('answers][' . $key . '][score', $this->t('The score needs to be a number between @min and @max', ['@min' => 0, '@max' => $form['answers'][$key]['#points_max']]));
      }
    }
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $quiz = $form_state->get('quiz');
    $submission = $form_state->get('submission');
    $grades = Quiz::grades((int) $submission->id());
    foreach (Element::children($form['answers']) as $key) {
      $value = $form_state->getValue(['answers', $key, 'score']);
      $feedback = (array) $form_state->getValue(['answers', $key, 'feedback']);
      if ($value === '' || $value === NULL) {
        unset($grades[$key]);
        continue;
      }
      $weight = (float) $form['answers'][$key]['#weight_factor'];
      // Kept as D7 kept it: the question's own score, before weighting.
      $grades[$key] = [
        'score' => $weight ? (int) $value / $weight : 0,
        'feedback' => (string) ($feedback['value'] ?? ''),
        'format' => $feedback['format'] ?? NULL,
      ];
    }
    Quiz::setGrades((int) $submission->id(), $grades);
    $tally = Quiz::regrade($quiz, $submission);
    $this->messenger()->addStatus($tally['evaluated']
      ? $this->t('The score has been updated: @score%.', ['@score' => $tally['percentage']])
      : $this->t('The scores have been saved. The attempt is not final until every answer is scored.'));
    $form_state->setRedirectUrl(Url::fromRoute('vaxia_core.quiz_unscored'));
  }

}
