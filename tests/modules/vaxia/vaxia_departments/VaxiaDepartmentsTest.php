<?php

namespace Vaxia\Tests\Vaxia\VaxiaDepartments;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vaxia departments: passing the system, setting and social walkthrough
 * quizzes (production nodes 9781, 10344, 10231) congratulates the player and
 * records when they took it. Public pages list each department's members
 * (knows-the-system / -social / -setting), newbie helpers and mediators.
 *
 * @group vaxia_departments
 */
class VaxiaDepartmentsTest extends VaxiaTestCase {

  /** @var bool */
  private static $quizzes = FALSE;

  protected function setUp(): void {
    parent::setUp();
    if (!self::$quizzes) {
      foreach (array(9781 => 'System walkthrough', 10344 => 'Setting walkthrough', 10231 => 'Social walkthrough') as $nid => $title) {
        $this->site->createNode('quiz', array('nid' => $nid, 'is_new' => TRUE, 'title' => $title));
      }
      self::$quizzes = TRUE;
    }
  }

  public static function tearDownAfterClass(): void {
    self::$quizzes = FALSE;
    parent::tearDownAfterClass();
  }

  public function walkthroughs() {
    return array(
      'system' => array(9781, "Congratulations! You're up to date on how our system here works.", 'Took system walkthrough'),
      'setting' => array(10344, "Congratulations! You're up to date on our settings.", 'Took setting walkthrough'),
      'social' => array(10231, "Congratulations! You're up to date on our social expectations here.", 'Took social walkthrough'),
    );
  }

  /**
   * @dataProvider walkthroughs
   */
  public function testPassingAWalkthroughIsRecorded($nid, $message, $label) {
    $player = $this->site->createUser(array('player'));
    $messages = $this->site->passQuiz($player, (object) array('nid' => $nid));
    $this->assertNotEmpty(array_filter($messages, function ($m) use ($message) {
      return strpos($m, $message) === 0;
    }), 'Congratulated');
    $this->assertStringContainsString($label, $this->profileText($player), 'Date recorded on the profile');
  }

  public function testOtherQuizzesAreNotWalkthroughs() {
    $player = $this->site->createUser(array('player'));
    $messages = $this->site->passQuiz($player, $this->site->createNode('quiz'));
    $this->assertEmpty(preg_grep('/^Congratulations! You\'re up to date/', $messages));
  }

  public function departments() {
    return array(
      array('players/system-department', 'knows-the-system'),
      array('players/social-department', 'knows-the-social'),
      array('players/setting-department', 'knows-the-setting'),
      array('players/social-helpers', 'newbie-helper'),
      array('players/social-mediators', 'mediator'),
    );
  }

  /**
   * @dataProvider departments
   */
  public function testDepartmentPagesListMembers($path, $role) {
    $member = $this->site->createUser(array('player', $role));
    $other = $this->site->createUser(array('player'));
    $this->login($this->site->createUser(array('player')));
    $this->get($path);
    $this->assertStatus(200);
    $this->assertText($member->name);
    $this->assertNoText($other->name);
  }

}
