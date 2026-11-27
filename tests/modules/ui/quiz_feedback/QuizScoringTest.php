<?php

namespace Vaxia\Tests\Ui\QuizFeedback;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Quiz scoring as D7 quiz 7.x-4.0-rc3 scored it: a multiple-choice answer
 * scores each option's "if chosen" / "if not chosen" points (partial and
 * negative points included), each question is weighted by its points in the
 * quiz, the score is the rounded percentage of possible points, and a quiz
 * with a pass rate shows its pass text from that score on (else its default
 * text). Free-text answers wait for grading (not passed meanwhile). Quizzes
 * may shuffle their questions, and may be retaken without an "already
 * submitted" notice.
 *
 * D7 took a quiz one question per page; the port takes it as one webform, so
 * these run on the port only.
 *
 * @group quiz_scoring
 */
class QuizScoringTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('Checks the D11 quiz scorer (D7 takes quizzes one question per page).');
    }
  }

  /**
   * Takes the quiz as $account: $answers is question key => option label (or
   * labels, or text). Leaves the browser on the result page.
   */
  private function take($quiz, $account, array $answers) {
    $this->login($account);
    $this->get('node/' . $quiz->nid . '/take');
    $edit = array();
    foreach ($answers as $key => $answer) {
      if (!is_array($answer) && $this->xpath()->query('//textarea[@name="' . $key . '"]')->length) {
        $edit[$key] = $answer;
        continue;
      }
      foreach ((array) $answer as $label) {
        $input = $this->xpath()->query('//input[(@name="' . $key . '" or starts-with(@name, "' . $key . '[")) and @id = //label[normalize-space(.)="' . $label . '"]/@for]')->item(0);
        $this->assertNotNull($input, "Option '$label' offered for $key");
        $edit[$input->getAttribute('name')] = $input->getAttribute('value');
      }
    }
    $this->submitForm(NULL, $edit, 'Finish');
    $this->assertStatus(200);
  }

  private function weightedQuiz(array $settings = array()) {
    return $this->site->createScoredQuiz($this->site->uniqueName('Weighted quiz'), array(
      'big' => array('title' => 'Worth three', 'options' => array('Right' => array(1, 0), 'Wrong' => array(0, 0)), 'max_score' => 3),
      'small' => array('title' => 'Worth one', 'options' => array('Yes' => array(1, 0), 'No' => array(0, 0)), 'max_score' => 1),
    ), $settings);
  }

  public function testMultipleAnswersScoreEachOption() {
    $quiz = $this->site->createScoredQuiz($this->site->uniqueName('Multi quiz'), array(
      'q' => array('title' => 'Check the true ones', 'type' => 'checkboxes', 'options' => array(
        'True one' => array(1, 0),
        'True two' => array(1, 0),
        'Trap' => array(0, 1),
        'Neutral' => array(0, 0),
      )),
    ));
    $player = $this->site->createUser(array('player'));
    // Each option scores: chosen right +1, unchosen right 0, chosen trap 0
    // (leaving it would have scored 1): 1 of 3.
    $this->take($quiz, $player, array('q' => array('True one', 'Trap')));
    $this->assertText('You got 1 of 3 possible points.');
    $this->assertText('Your score: 33%');
    $this->assertSame(1, $this->xpath()->query('//span[contains(@class, "multichoice-icon") and contains(@class, "correct")]')->length);
    $this->assertSame(1, $this->xpath()->query('//span[contains(@class, "multichoice-icon") and contains(@class, "wrong")]')->length);
    $this->take($quiz, $player, array('q' => array('True one', 'True two')));
    $this->assertText('Your score: 100%');
  }

  public function testPartialAndNegativePoints() {
    $quiz = $this->site->createScoredQuiz($this->site->uniqueName('Partial quiz'), array(
      'q' => array('title' => 'Pick the best', 'options' => array(
        'Best' => array(4, 0),
        'Partly' => array(2, 0),
        'Harmful' => array(-1, 0),
        'Nothing' => array(0, 0),
      )),
    ));
    $player = $this->site->createUser(array('player'));
    $this->take($quiz, $player, array('q' => 'Partly'));
    $this->assertText('Your score: 50%');
    $this->assertSame(1, $this->xpath()->query('//span[contains(@class, "multichoice-icon") and contains(@class, "almost")]')->length);
    $this->take($quiz, $player, array('q' => 'Harmful'));
    $this->assertText('Your score: -25%');
    $this->assertSame(array(50.0, -25.0), array_column($this->site->quizAttempts($quiz), 'score'));
  }

  public function testQuestionsAreWeightedByTheirPoints() {
    $quiz = $this->weightedQuiz();
    $player = $this->site->createUser(array('player'));
    $this->take($quiz, $player, array('big' => 'Right', 'small' => 'No'));
    $this->assertText('You got 3 of 4 possible points.');
    $this->assertText('Your score: 75%');
    $this->take($quiz, $player, array('big' => 'Wrong', 'small' => 'Yes'));
    $this->assertText('Your score: 25%');
  }

  public function testPassRateBoundaryAndResultTexts() {
    $quiz = $this->weightedQuiz(array('pass_rate' => 75, 'summary_pass' => 'Well done, you passed.', 'summary_default' => 'Not this time.'));
    $player = $this->site->createUser(array('player'));
    $this->take($quiz, $player, array('big' => 'Right', 'small' => 'No'));
    $this->assertText('Your score: 75%');
    $this->assertText('Well done, you passed.');
    $this->assertNoText('Not this time.');
    $this->take($quiz, $player, array('big' => 'Wrong', 'small' => 'Yes'));
    $this->assertText('Not this time.');
    $this->assertNoText('Well done, you passed.');
    $this->assertSame(array(1, 0), array_column($this->site->quizAttempts($quiz), 'passed'));
  }

  public function testFreeTextAnswersWaitForGrading() {
    $quiz = $this->site->createScoredQuiz($this->site->uniqueName('Essay quiz'), array(
      'pick' => array('title' => 'Pick', 'options' => array('Right' => array(1, 0), 'Wrong' => array(0, 0)), 'max_score' => 1),
      'essay' => array('title' => 'Explain', 'type' => 'textarea', 'max_score' => 1, 'question_max' => 10),
    ), array('pass_rate' => 50));
    $player = $this->site->createUser(array('player'));
    $this->take($quiz, $player, array('pick' => 'Right', 'essay' => 'Because.'));
    $this->assertText('Parts of this quiz have not been evaluated yet. The score below is not final.');
    $this->assertText('Your score: 50%');
    $this->assertSame(array(array('uid' => (int) $player->uid, 'score' => 50.0, 'passed' => 0)), $this->site->quizAttempts($quiz));
  }

  /**
   * Question keys in the order the take page shows them.
   */
  private function questionOrder($quiz) {
    $this->get('node/' . $quiz->nid . '/take');
    $order = array();
    foreach ($this->xpath()->query('//form//input[@type="radio"]') as $input) {
      $order[$input->getAttribute('name')] = TRUE;
    }
    return implode(',', array_keys($order));
  }

  public function testRandomQuestionOrder() {
    $questions = array();
    foreach (range(1, 6) as $i) {
      $questions['q' . $i] = array('title' => 'Question ' . $i, 'options' => array('A' . $i => array(1, 0), 'B' . $i => array(0, 0)));
    }
    $fixed = $this->site->createScoredQuiz($this->site->uniqueName('Fixed quiz'), $questions);
    $shuffled = $this->site->createScoredQuiz($this->site->uniqueName('Shuffled quiz'), $questions, array('randomize' => TRUE));
    $this->login($this->site->createUser(array('player')));
    $this->assertSame('q1,q2,q3,q4,q5,q6', $this->questionOrder($fixed));
    $this->assertSame('q1,q2,q3,q4,q5,q6', $this->questionOrder($fixed));
    $orders = array();
    for ($i = 0; $i < 15 && count($orders) < 2; $i++) {
      $order = $this->questionOrder($shuffled);
      $this->assertSame(array('q1', 'q2', 'q3', 'q4', 'q5', 'q6'), array_values(array_intersect(array('q1', 'q2', 'q3', 'q4', 'q5', 'q6'), explode(',', $order))), 'Every question is shown');
      $orders[$order] = TRUE;
    }
    $this->assertGreaterThan(1, count($orders), 'The shuffled quiz showed its questions in more than one order');
  }

  public function testQuizCanBeRetakenWithoutNotice() {
    $quiz = $this->weightedQuiz();
    // Someone who may see their own submissions would get Webform's notice.
    $player = $this->site->createUser(array('player'), array('view own webform submission'));
    $this->take($quiz, $player, array('big' => 'Wrong', 'small' => 'No'));
    $this->get('node/' . $quiz->nid . '/take');
    $this->assertNoText('You have already submitted this webform');
    $this->assertSame(1, $this->xpath()->query($this->webformFormXpath())->length);
    $this->take($quiz, $player, array('big' => 'Right', 'small' => 'Yes'));
    $this->assertText('Your score: 100%');
    $this->assertCount(2, $this->site->quizAttempts($quiz));
  }

}
