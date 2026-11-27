<?php

namespace Vaxia\Tests\Ui\DiceRuler;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Dice ruler: a block (production shows it on every page, to those allowed to
 * use it) where storyhosts enter up to four actions' mights and rolls and get
 * the ruling: roll needed, success, wins-by, points. Extra actions lower every
 * might by a penalty. Admins set the default difficulty and the penalty.
 *
 * @group dice_ruler
 */
class DiceRulerTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    $this->site->placeBlock('dice_ruler', 'dice_ruler', 'content');
  }

  private function ruler() {
    return $this->site->createUser(array('storyteller'), array('use dice ruler'));
  }

  /**
   * Rules a single action of two traits; returns the results text.
   */
  private function ruleTwoTrait(array $a, array $b, $actions = 1) {
    $this->login($this->ruler());
    $this->get('node');
    $this->submitForm(NULL, array(
      'actions' => (string) $actions,
      'roll_type_1' => 'two_trait',
      'two_trait_might_a_1' => $a[0], 'two_trait_rolled_a_1' => $a[1],
      'two_trait_might_b_1' => $b[0], 'two_trait_rolled_b_1' => $b[1],
    ), 'Rule roll');
    $result = $this->xpath()->query('//div[@id="dice_ruler_dice_results"]')->item(0);
    $this->assertNotNull($result, 'Results shown');
    return trim(preg_replace('/\s+/', ' ', $result->textContent));
  }

  /**
   * Cells of the first action's result row, by column heading.
   */
  private function firstRuling() {
    $xpath = $this->xpath();
    $labels = array();
    foreach ($xpath->query('(//div[@id="dice_ruler_dice_results"]//table)[1]//thead//th') as $th) {
      $labels[] = trim($th->textContent);
    }
    $row = array();
    foreach ($xpath->query('(//div[@id="dice_ruler_dice_results"]//table)[1]//tbody/tr[1]/td') as $i => $td) {
      $row[$labels[$i]] = trim(preg_replace('/\s+/', ' ', $td->textContent));
    }
    return $row;
  }

  public function testSettingsNeedPermission() {
    $this->login($this->ruler());
    $this->get('admin/vaxia/dice_ruler');
    $this->assertStatus(403);
  }

  public function testSettingsDefaults() {
    $this->login($this->site->createUser(array(), array('administer dice ruler')));
    $this->get('admin/vaxia/dice_ruler');
    $this->assertSame('25', $this->xpath()->query('//input[@name="dice_ruler_default_diff"]')->item(0)->getAttribute('value'));
    $this->assertSame('20', $this->xpath()->query('//input[@name="dice_ruler_extra_action_penalty"]')->item(0)->getAttribute('value'));
  }

  public function testBlockShowsForThoseAllowed() {
    $this->login($this->ruler());
    $this->get('node');
    $this->assertSame(1, $this->xpath()->query('//input[@value="Rule roll"]')->length);
  }

  public function testBlockHiddenFromOthers() {
    $this->withoutPermission('use dice ruler', array('anonymous user', 'authenticated user', 'player'), function () {
      $this->login($this->site->createUser(array('player')));
      $this->get('node');
      $this->assertSame(0, $this->xpath()->query('//input[@value="Rule roll"]')->length);
    });
  }

  public function testTwoTraitRuling() {
    // Might 50 vs difficulty 25 needs 19.4; rolls of 80 win by 61 for 30 points.
    $this->assertStringContainsString('Action #1', $this->ruleTwoTrait(array('50', '80'), array('50', '80')));
    $ruling = $this->firstRuling();
    $this->assertSame('yes', $ruling['Success']);
    $this->assertSame('61', $ruling['Winsby']);
    $this->assertSame('30', $ruling['Points']);
  }

  public function testFailedTwoTraitRuling() {
    // Losing by more than 50 is a mishap.
    $this->ruleTwoTrait(array('10', '5'), array('10', '5'));
    $ruling = $this->firstRuling();
    $this->assertSame('mishap', $ruling['Success']);
    $this->assertSame('0', $ruling['Points']);
  }

  public function testMissingValuesAreReported() {
    $this->assertStringContainsString('Not all required values given, or values are zero.', $this->ruleTwoTrait(array('50', ''), array('50', '80')));
  }

  public function testNonNumericValuesAreReported() {
    $this->assertStringContainsString('Not all values given were numeric. We need numbers for math.', $this->ruleTwoTrait(array('50', 'high'), array('50', '80')));
  }

  public function testExtraActionsLowerMight() {
    // Two actions: -20 might, so 50 becomes 30.
    $this->ruleTwoTrait(array('50', '80'), array('50', '80'), 2);
    $this->assertStringStartsWith('30', $this->firstRuling()['Might']);
  }

  public function testDefaultDifficultyIsConfigurable() {
    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/vaxia/dice_ruler', array('dice_ruler_default_diff' => '40'), 'Save configuration');
    });
    $this->login($this->ruler());
    $this->get('node');
    $value = $this->xpath()->query('//input[@name="two_trait_diff_a_1"]')->item(0)->getAttribute('value');
    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/vaxia/dice_ruler', array('dice_ruler_default_diff' => '25'), 'Save configuration');
    });
    $this->assertSame('40', $value);
  }

  /**
   * @group knownbug
   * The rulings read "add diff" values (two_trait_add_diff_a_N, combat_add_diff_*,
   * ...) that the form never has: every ruling prints undefined-index notices.
   */
  public function testRulingHasNoPhpErrors() {
    $this->ruleTwoTrait(array('50', '80'), array('50', '80'));
    $this->assertSame(array(), $this->pageErrorsFrom('dice_ruler.module'));
  }

}
