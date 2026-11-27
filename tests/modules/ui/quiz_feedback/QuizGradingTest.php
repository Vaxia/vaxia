<?php

namespace Vaxia\Tests\Ui\QuizFeedback;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Grading free-text quiz answers (D7 quiz long answers): graders ("score any
 * quiz") find the answers waiting at admin/quiz/reports/score-long-answer and
 * score each one with optional feedback on the attempt's results page. The
 * attempt counts only once every answer is scored; then it is rescored and
 * passes from the pass rate on, which files role applications etc. (once).
 * The quiz taker sees each answer's score and the feedback.
 *
 * Runs on the port only (D7 takes quizzes one question per page).
 *
 * @group quiz_scoring
 */
class QuizGradingTest extends VaxiaTestCase {

  const LIST_PATH = 'admin/quiz/reports/score-long-answer';

  /** @var object */
  private $quiz;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('Checks the D11 quiz grading (D7 takes quizzes one question per page).');
    }
    // 2 points for the choice, 4 for each essay (scored out of 10 as
    // questions): 10 possible, 50% to pass.
    $this->quiz = $this->site->createScoredQuiz($this->site->uniqueName('Essay quiz'), array(
      'pick' => array('title' => 'Pick one', 'options' => array('Right' => array(1, 0), 'Wrong' => array(0, 0)), 'max_score' => 2),
      'essay_one' => array('title' => 'First essay', 'type' => 'textarea', 'question_max' => 10, 'max_score' => 4),
      'essay_two' => array('title' => 'Second essay', 'type' => 'textarea', 'question_max' => 10, 'max_score' => 4),
    ), array('pass_rate' => 50));
  }

  private function grader() {
    return $this->site->createUser(array(), array('score any quiz'));
  }

  /**
   * $account takes the quiz, choosing "Right"; returns the attempt's sid.
   */
  private function take($account) {
    $this->login($account);
    $this->get('node/' . $this->quiz->nid . '/take');
    $right = $this->xpath()->query('//input[@name="pick" and @id = //label[normalize-space(.)="Right"]/@for]')->item(0);
    $this->submitForm(NULL, array('pick' => $right->getAttribute('value'), 'essay_one' => 'My first answer.', 'essay_two' => 'My second answer.'), 'Finish');
    $this->assertStatus(200);
    $this->assertText('Parts of this quiz have not been evaluated yet.');
    $this->assertSame(1, preg_match('#/quiz-result/(\d+)#', $this->url, $m), 'Landed on the result page');
    return (int) $m[1];
  }

  /**
   * This test's quiz's rows in the pending list, as cell texts (the database
   * is restored per class, so earlier tests' attempts are listed too).
   */
  private function pending() {
    $this->get(self::LIST_PATH);
    $this->assertStatus(200);
    $rows = array();
    $xpath = $this->xpath();
    foreach ($xpath->query('//table//tbody/tr[td[4]]') as $tr) {
      $row = array_map(fn($td) => trim($td->textContent), iterator_to_array($xpath->query('./td', $tr)));
      if ($row[0] === $this->quiz->title) {
        $rows[] = $row;
      }
    }
    return $rows;
  }

  private function grade($sid, array $scores) {
    $edit = array();
    foreach ($scores as $key => $score) {
      $edit['answers[' . $key . '][score]'] = $score[0];
      $edit['answers[' . $key . '][feedback][value]'] = $score[1] ?? '';
    }
    $this->submitForm('node/' . $this->quiz->nid . '/results/' . $sid, $edit, 'Save score');
    $this->assertStatus(200);
  }

  public function testGraderSeesAnswersWaiting() {
    $sid = $this->take($this->site->createUser(array('player')));
    $this->login($this->grader());
    $rows = $this->pending();
    $this->assertCount(2, $rows);
    $this->assertSame(array($this->quiz->title, 'First essay'), array_slice($rows[0], 0, 2));
    $this->assertSame(array($this->quiz->title, 'Second essay'), array_slice($rows[1], 0, 2));
    $this->assertSame('Score this response', $rows[0][3]);
    $this->assertSame(2, $this->xpath()->query('//a[contains(@href, "/node/' . $this->quiz->nid . '/results/' . $sid . '")]')->length);
  }

  public function testPartialGradingLeavesAttemptPending() {
    $sid = $this->take($this->site->createUser(array('player')));
    $this->login($this->grader());
    $this->grade($sid, array('essay_one' => array(4)));
    $this->assertText('not final until every answer is scored');
    $this->assertSame(array(array('Second essay')), array_map(fn($r) => array($r[1]), $this->pending()));
    $attempts = $this->site->quizAttempts($this->quiz);
    $this->assertSame(0, $attempts[0]['passed'], 'Not passed while an answer waits');
  }

  public function testFullGradingScoresAndPassesAtTheBoundary() {
    $sid = $this->take($this->site->createUser(array('player')));
    $this->login($this->grader());
    // 2 (choice) + 3 + 0 = 5 of 10: exactly the pass rate.
    $this->grade($sid, array('essay_one' => array(3), 'essay_two' => array(0)));
    $this->assertText('The score has been updated: 50%.');
    // Nothing of this quiz waits (other tests' attempts may still be listed,
    // so the page-wide empty text isn't checked here).
    $this->assertSame(array(), $this->pending());
    $attempts = $this->site->quizAttempts($this->quiz);
    $this->assertSame(50.0, $attempts[0]['score']);
    $this->assertSame(1, $attempts[0]['passed']);
    // One point less fails.
    $sid2 = $this->take($this->site->createUser(array('player')));
    $this->login($this->grader());
    $this->grade($sid2, array('essay_one' => array(2), 'essay_two' => array(0)));
    $attempts = $this->site->quizAttempts($this->quiz);
    $this->assertSame(40.0, $attempts[1]['score']);
    $this->assertSame(0, $attempts[1]['passed']);
  }

  public function testScoreAboveTheMaximumIsRefused() {
    $sid = $this->take($this->site->createUser(array('player')));
    $this->login($this->grader());
    $this->get('node/' . $this->quiz->nid . '/results/' . $sid);
    $this->assertText('Score (0 to 4)');
    $this->grade($sid, array('essay_one' => array(5)));
    $this->assertText('The score needs to be a number between 0 and 4');
    $this->assertCount(2, $this->pending());
  }

  public function testPassingOnGradingFilesOneApplication() {
    $approver = $this->site->createRole();
    $this->inSession($this->superuser(), function () use ($approver) {
      $this->get('admin/rpg/role_app');
      $applied = $this->optionValue('role_app_roles[]', 'storyteller-candidate');
      $this->submitForm(NULL, array('role_app_roles[]' => array($applied)), 'Save configuration');
      $this->get('admin/rpg/role_app');
      $this->submitForm(NULL, array(
        'role_app_quiz_' . $applied . '[]' => array($this->quiz->nid),
        'role_app_eligible_' . $applied => $this->optionValue('role_app_eligible_' . $applied, 'eligible for SH'),
        'role_app_approve_' . $applied . '[]' => array($this->optionValue('role_app_approve_' . $applied . '[]', $approver)),
        'role_app_view_' . $applied . '[]' => array($this->optionValue('role_app_view_' . $applied . '[]', 'lead')),
      ), 'Save configuration');
    });
    $applicant = $this->site->createUser(array('eligible for SH'));
    $sid = $this->take($applicant);
    $this->assertSame(0, $this->applications($applicant), 'No application while the essays wait');
    $grader = $this->grader();
    $this->login($grader);
    $this->grade($sid, array('essay_one' => array(4), 'essay_two' => array(4)));
    $this->assertSame(1, $this->applications($applicant), 'Passing on grading files the application');
    // Scoring the passed attempt again doesn't pass it again.
    $this->login($grader);
    $this->grade($sid, array('essay_one' => array(3), 'essay_two' => array(4)));
    $this->assertSame(1, $this->applications($applicant));
  }

  private function applications($applicant) {
    return $this->inSession($this->superuser(), function () use ($applicant) {
      $this->get('admin/content', array('type' => 'application'));
      return $this->xpath()->query('//a[contains(text(), "Application (' . $applicant->name . ')")]')->length;
    });
  }

  public function testNonGradersAreRefused() {
    $sid = $this->take($this->site->createUser(array('player')));
    $this->login($this->site->createUser(array('player')));
    $this->get(self::LIST_PATH);
    $this->assertStatus(403);
    $this->get('node/' . $this->quiz->nid . '/results/' . $sid);
    $this->assertStatus(403);
  }

  public function testTakerSeesScoresAndFeedback() {
    $player = $this->site->createUser(array('player'));
    $sid = $this->take($player);
    $this->assertText('Score: ? of 4');
    $this->login($this->grader());
    $this->grade($sid, array('essay_one' => array(3, 'Good point about the river.'), 'essay_two' => array(1)));
    $this->login($player);
    $this->get('node/' . $this->quiz->nid . '/quiz-result/' . $sid);
    $this->assertNoText('Parts of this quiz have not been evaluated yet.');
    $this->assertText('Score: 3 of 4');
    $this->assertText('Good point about the river.');
    $this->assertText('Score: 1 of 4');
    $this->assertText('Your score: 60%');
  }

}
