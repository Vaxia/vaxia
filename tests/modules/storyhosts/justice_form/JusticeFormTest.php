<?php

namespace Vaxia\Tests\Storyhosts\JusticeForm;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Justice calculator: storyhosts enter a court's hostility tier, the crime's PP,
 * the player's reputation and three two-trait rolls; the page works out the
 * difficulty, how much PP the rolls wipe out, and the punishment due (silver,
 * service RP or time, per tier settings).
 *
 * @group justice_form
 */
class JusticeFormTest extends VaxiaTestCase {

  private function storyhost() {
    return $this->site->createUser(array('storyteller'), array('use justice form'));
  }

  /**
   * Submits the calculator; every roll uses the same mights and dice.
   */
  private function calculate($tier, $pp, $rep, $might = 50, $rolled = 50) {
    $edit = array('tier' => (string) $tier, 'pp' => (string) $pp, 'rep' => (string) $rep);
    for ($i = 1; $i < 4; $i++) {
      foreach (array(1, 2) as $j) {
        $edit['might_' . $i . '_' . $j] = (string) $might;
        $edit['roll_' . $i . '_' . $j] = (string) $rolled;
      }
    }
    $this->login($this->storyhost());
    $this->submitForm('vaxia/justice', $edit, 'Calculate results');
    return implode(' ', $this->messages('status'));
  }

  public function testCalculatorNeedsPermission() {
    $this->login($this->site->createUser(array('player')));
    $this->get('vaxia/justice');
    $this->assertStatus(403);
  }

  public function testSettingsNeedPermission() {
    $this->login($this->storyhost());
    $this->get('admin/vaxia/justice_form');
    $this->assertStatus(403);
  }

  public function testTierSetsTheDifficulty() {
    $result = $this->calculate(2, 10, 0);
    $this->assertStringContainsString('The Tier level is Tier 2.', $result);
    $this->assertStringContainsString('Rolls at Tier 2, 50 difficulty to reduce 10 PP to zero.', $result);
  }

  public function testBadReputationRaisesTheTier() {
    $result = $this->calculate(2, 10, -17);
    $this->assertStringContainsString('A bad reputation raised the Tier level to 3.', $result);
    $this->assertStringContainsString('Rolls at Tier 3, 75 difficulty', $result);
  }

  public function testGoodReputationLowersTheTier() {
    $result = $this->calculate(2, 10, 17);
    $this->assertStringContainsString('Rolls at Tier 1, 25 difficulty', $result);
  }

  public function testReputationWithinSixteenChangesNothing() {
    $this->assertStringContainsString('Rolls at Tier 2,', $this->calculate(2, 10, 16));
    $this->assertStringContainsString('Rolls at Tier 2,', $this->calculate(2, 10, -16));
  }

  public function testFailedRollsLeaveTheFullPunishment() {
    // Rolls of 1 fail at any difficulty.
    $result = $this->calculate(2, 10, 0, 10, 1);
    $this->assertStringContainsString('Crime PP leftover: 10', $result);
    // Tier 2 x 10 silver x 10 PP; 2 x 0.1 x 10 service; 1 x 10 time.
    $this->assertStringContainsString('Punishment due: 200 silver OR 2 service RP OR 10', $result);
  }

  public function testTierSilverIsConfigurable() {
    $this->login($this->site->createUser(array(), array('administrate justice form')));
    $this->submitForm('admin/vaxia/justice_form', array(
      'justice_form_silver_2' => '5',
      'justice_form_punishments' => 'Theft: 10 PP',
      'justice_form_reputations' => 'Hero: +20',
    ), 'Save configuration');

    $result = $this->calculate(2, 10, 0, 10, 1);

    $this->assertStringContainsString('Punishment due: 100 silver', $result);
    $this->assertText('Theft: 10 PP');
    $this->assertText('Hero: +20');
  }

  /**
   * @group knownbug
   * The PP a roll generates is read as $ruling[7], but the dice ruler returns
   * named keys ('pp'): rolls never reduce the crime PP (and each prints an
   * "Undefined offset: 7" notice).
   */
  public function testWinningRollsReduceTheCrimePp() {
    // Might 100 against difficulty 25 with rolls of 100 wins big.
    $result = $this->calculate(1, 10, 0, 100, 100);
    $this->assertStringNotContainsString('Crime PP leftover: 10', $result);
    $this->assertSame(array(), $this->pageErrorsFrom('justice_form.module'));
  }

  /**
   * @group knownbug
   * The good-reputation message says the tier was "raised" when it was lowered.
   */
  public function testGoodReputationMessageSaysLowered() {
    $this->assertStringContainsString('A good reputation lowered the Tier level to 1.', $this->calculate(2, 10, 17));
  }

  /**
   * @group knownbug
   * The tier's time unit (days/weeks/months) is read but never printed: the
   * punishment reads "OR 10 time."
   */
  public function testTimePunishmentHasItsUnit() {
    $result = $this->calculate(2, 10, 0, 10, 1);
    $this->assertMatchesRegularExpression('/OR 10 ?(days|weeks|months)\./', $result);
  }

}
