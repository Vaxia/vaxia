<?php

namespace Vaxia\Tests\Ui\QuizFeedback;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Quiz feedback: on multiple-choice quiz results, players see whether the
 * answer they chose was right or wrong, but not which answer was correct (the
 * quiz module's "you should have chosen this" marker is removed), so results
 * can't be used as an answer key.
 *
 * @group quiz_feedback
 */
class QuizFeedbackTest extends VaxiaTestCase {

  /**
   * Takes the quiz choosing $answer; leaves the browser on the results.
   */
  private function take($quiz, $answer) {
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $quiz->nid . '/take');
    $radio = $this->xpath()->query('//tr[contains(., "' . $answer . '")]//input[@type="radio"] | //div[contains(@class, "form-item")][contains(., "' . $answer . '")]/input[@type="radio"]')->item(0);
    $this->assertNotNull($radio, "Answer '$answer' offered");
    $this->submitForm(NULL, array($radio->getAttribute('name') => $radio->getAttribute('value')), 'Finish');
    $this->assertStatus(200);
  }

  private function quiz() {
    return $this->site->createMultichoiceQuiz($this->site->uniqueName('Lore quiz'), 'What is the capital of Vaxia?', array(
      'Rivermoot' => TRUE,
      'Ashfall' => FALSE,
      'Port Vey' => FALSE,
    ));
  }

  public function testWrongChoiceIsMarkedWrong() {
    $this->take($this->quiz(), 'Ashfall');
    $this->assertSame(1, $this->xpath()->query('//span[contains(@class, "multichoice-icon") and contains(@class, "wrong")]')->length);
  }

  public function testCorrectAnswerIsNotRevealed() {
    $this->take($this->quiz(), 'Ashfall');
    $this->assertText('Rivermoot');
    $this->assertSame(0, $this->xpath()->query('//span[contains(@class, "multichoice-icon") and contains(@class, "should")]')->length);
    $this->assertNoText("This option is correct, but you didn't select it.");
  }

  public function testRightChoiceIsMarkedCorrect() {
    $this->take($this->quiz(), 'Rivermoot');
    $this->assertSame(1, $this->xpath()->query('//span[contains(@class, "multichoice-icon") and contains(@class, "correct")]')->length);
  }

}
