<?php

namespace Vaxia\Tests\Ui\DiceRuler;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Dice ruler rulings beyond the plain two-trait roll: opposed rolls (two trait
 * vs two trait), combat (attack / block / parry / dodge both ways, weapon
 * damage percentages and multi-action modifiers), magic (with the target's
 * resistance), and the player advisor block ("Should I try a multi-attack?").
 *
 * Numbers: might 50 against 50 needs 50.5, so a roll of 80 wins by 29.5 ("30")
 * for 14.75 points ("15"); a roll of 20 fails by 30.5.
 *
 * @group dice_ruler
 */
class DiceRulerRulingsTest extends VaxiaTestCase {

  /** @var array Production's weapon classes: name => tid. */
  private static $classes = array();

  protected function setUp(): void {
    parent::setUp();
    $this->site->placeBlock('dice_ruler', 'dice_ruler', 'content');
    if (!self::$classes) {
      foreach (array(
        array('Unarmed (0% / 0)', 0, 0),
        array('Heavy (75% / 10)', 75, 10),
        array('Medium (45% / 0)', 45, 0),
        array('Light (15% / -10)', 15, -10),
      ) as $weight => $class) {
        self::$classes[strtok($class[0], ' ')] = $this->site->createWeaponClass($class[0], $class[1], $class[2], $weight);
      }
    }
  }

  public static function tearDownAfterClass(): void {
    self::$classes = array();
    parent::tearDownAfterClass();
  }

  /**
   * Rules one action of $type with $fields (field name without the "_1"
   * suffix => value); returns the results as text.
   */
  private function rule($type, array $fields, $actions = 1) {
    $this->login($this->site->createUser(array('storyteller'), array('use dice ruler')));
    $this->get('node');
    $edit = array('actions' => (string) $actions, 'roll_type_1' => $type);
    foreach ($fields as $name => $value) {
      $edit[$name . '_1'] = (string) $value;
    }
    $this->submitForm(NULL, $edit, 'Rule roll');
    return trim(preg_replace('/\s+/', ' ', $this->xpath()->query('//div[@id="dice_ruler_dice_results"]')->item(0)->textContent));
  }

  /**
   * Result tables of the first action: one array of rows per table, each row
   * column heading => cell text.
   */
  private function tables() {
    $xpath = $this->xpath();
    $out = array();
    foreach ($xpath->query('//div[@id="dice_ruler_dice_results"]//table') as $table) {
      $labels = array();
      foreach ($xpath->query('.//thead//th', $table) as $th) {
        $labels[] = trim($th->textContent);
      }
      $rows = array();
      foreach ($xpath->query('.//tbody/tr', $table) as $tr) {
        $row = array();
        foreach ($xpath->query('./td', $tr) as $i => $td) {
          $row[$labels[$i] ?? $i] = trim(preg_replace('/\s+/', ' ', $td->textContent));
        }
        $rows[] = $row;
      }
      $out[] = $rows;
    }
    return $out;
  }

  /**
   * Combat rows keyed by action ("A Attacks B:").
   */
  private function combatRows() {
    $rows = array();
    foreach ($this->tables()[0] as $row) {
      $rows[$row['Action']] = $row;
    }
    return $rows;
  }

  private function combatant($side, $might, $rolled) {
    $fields = array();
    foreach (array('dex', 'str', 'end') as $stat) {
      $fields['combat_might_' . $side . '_' . $stat] = $might;
      $fields['combat_rolled_' . $side . '_' . $stat] = $rolled;
    }
    return $fields;
  }

  // ---------------------------------------------------------------------------
  // Two trait vs two trait.
  // ---------------------------------------------------------------------------

  public function testOpposedRollsRuleBothSides() {
    $text = $this->rule('two_trait_vs', array(
      'two_trait_vs_might_aa' => 50, 'two_trait_vs_rolled_aa' => 80,
      'two_trait_vs_might_ab' => 50, 'two_trait_vs_rolled_ab' => 80,
      'two_trait_vs_might_ba' => 50, 'two_trait_vs_rolled_ba' => 20,
      'two_trait_vs_might_bb' => 50, 'two_trait_vs_rolled_bb' => 20,
    ));
    $this->assertStringContainsString("First character's results.", $text);
    $this->assertStringContainsString("Second character's results.", $text);
    list($first, $second) = $this->tables();
    $this->assertSame(array('yes', '30', '15'), array($first[0]['Success'], $first[0]['Winsby'], $first[0]['Points']));
    $this->assertSame(array('no', '0'), array($second[0]['Success'], $second[0]['Points']));
  }

  public function testOpposedRollsNeedEveryValue() {
    $text = $this->rule('two_trait_vs', array(
      'two_trait_vs_might_aa' => 50, 'two_trait_vs_rolled_aa' => 80,
      'two_trait_vs_might_ab' => 50, 'two_trait_vs_rolled_ab' => 80,
    ));
    $this->assertStringContainsString('Not all required values given, or values are zero.', $text);
  }

  public function testOpposedRollsNeedNumbers() {
    $text = $this->rule('two_trait_vs', array(
      'two_trait_vs_might_aa' => 50, 'two_trait_vs_rolled_aa' => 'high',
      'two_trait_vs_might_ab' => 50, 'two_trait_vs_rolled_ab' => 80,
      'two_trait_vs_might_ba' => 50, 'two_trait_vs_rolled_ba' => 20,
      'two_trait_vs_might_bb' => 50, 'two_trait_vs_rolled_bb' => 20,
    ));
    $this->assertStringContainsString('Not all values given were numeric. We need numbers for math.', $text);
  }

  /**
   * @group knownbug
   * Mights get the multi-action penalty subtracted before the numeric check,
   * so text typed as a might becomes 0, then 1, and is ruled ("A non-numeric
   * value encountered" notice) instead of reported.
   */
  public function testTextMightIsReportedNotRuled() {
    $text = $this->rule('two_trait_vs', array(
      'two_trait_vs_might_aa' => 'lots', 'two_trait_vs_rolled_aa' => 80,
      'two_trait_vs_might_ab' => 50, 'two_trait_vs_rolled_ab' => 80,
      'two_trait_vs_might_ba' => 50, 'two_trait_vs_rolled_ba' => 20,
      'two_trait_vs_might_bb' => 50, 'two_trait_vs_rolled_bb' => 20,
    ));
    $this->assertStringContainsString('Not all values given were numeric. We need numbers for math.', $text);
  }

  // ---------------------------------------------------------------------------
  // Combat.
  // ---------------------------------------------------------------------------

  public function testCombatRulesEveryExchange() {
    $this->rule('combat', $this->combatant('a', 50, 80) + $this->combatant('b', 50, 20) + array(
      'combat_weapon_a' => self::$classes['Unarmed'],
      'combat_weapon_b' => self::$classes['Unarmed'],
    ));
    $rows = $this->combatRows();
    $this->assertSame(array('A Attacks B:', 'B Attacks A:', 'A Blocks B:', 'B Blocks A:', 'A Parrys B:', 'B Parrys A:', 'A Dodges B:', 'B Dodges A:'), array_keys($rows));
    $this->assertSame('yes', $rows['A Attacks B:']['Success']);
    $this->assertSame('15 to B, w/ weapon', $rows['A Attacks B:']['After defense']);
    $this->assertSame('no', $rows['B Attacks A:']['Success']);
    // B's failed block lets A's 15 through; A's block stops B's nothing.
    $this->assertSame('15 to B, w/ weapon', $rows['B Blocks A:']['After defense']);
    $this->assertSame('0 to A', $rows['A Blocks B:']['After defense']);
  }

  public function testWeaponPercentageAddsDamage() {
    $text = $this->rule('combat', $this->combatant('a', 50, 80) + $this->combatant('b', 50, 20) + array(
      'combat_weapon_a' => self::$classes['Heavy'],
      'combat_weapon_b' => self::$classes['Unarmed'],
    ));
    $this->assertStringContainsString('Character A, additional damage percent: 75%', $text);
    // 15 points plus 75%.
    $this->assertSame('26.25 to B, w/ weapon', $this->combatRows()['A Attacks B:']['After defense']);
  }

  public function testMultipleActionsUseTheWeaponModifier() {
    $text = $this->rule('combat', $this->combatant('a', 50, 80) + $this->combatant('b', 50, 20) + array(
      'combat_weapon_a' => self::$classes['Heavy'],
      'combat_weapon_b' => self::$classes['Light'],
      'number_actions_b' => 3,
    ), 2);
    // Heavy: 20 + 10 per extra action; Light: 20 - 10.
    $this->assertStringContainsString('Character A taking 2 actions is at multi action penalties for all actions: 30', $text);
    $this->assertStringContainsString('Character B taking 3 actions is at multi action penalties for all actions: 10 / 20', $text);
  }

  public function testSingleActionHasNoPenalty() {
    $text = $this->rule('combat', $this->combatant('a', 50, 80) + $this->combatant('b', 50, 20) + array(
      'combat_weapon_a' => self::$classes['Medium'],
      'combat_weapon_b' => self::$classes['Medium'],
    ));
    $this->assertStringContainsString('Character A taking 1 actions and is at no penalty.', $text);
    $this->assertStringContainsString('Character B taking 1 actions and is at no penalty.', $text);
  }

  public function testCombatNeedsNumbers() {
    $text = $this->rule('combat', array('combat_rolled_a_dex' => 'high') + $this->combatant('b', 50, 20) + array(
      'combat_might_a_dex' => 50, 'combat_might_a_str' => 50, 'combat_might_a_end' => 50,
      'combat_weapon_a' => self::$classes['Medium'], 'combat_weapon_b' => self::$classes['Medium'],
    ));
    $this->assertStringContainsString('Not all values given were numeric. We need numbers for math.', $text);
  }

  public function testHelpTextsShowOnTheRuler() {
    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/vaxia/dice_ruler', array(
        'dice_ruler_help_general' => 'Ask a lead when unsure.',
        'dice_ruler_help_combat' => 'Combat is three traits.',
      ), 'Save configuration');
    });
    $this->rule('two_trait', array('two_trait_might_a' => 50, 'two_trait_rolled_a' => 80, 'two_trait_might_b' => 50, 'two_trait_rolled_b' => 80));
    $this->assertText('Ask a lead when unsure.');
    $this->assertText('Combat is three traits.');
  }

  // ---------------------------------------------------------------------------
  // Magic.
  // ---------------------------------------------------------------------------

  private function spell(array $extra = array()) {
    return $extra + array(
      'magic_might_a' => 50, 'magic_rolled_a' => 80,
      'magic_might_b' => 50, 'magic_rolled_b' => 80,
    );
  }

  public function testMagicAgainstTheDefaultDifficulty() {
    $this->rule('magic', $this->spell());
    $ruling = $this->tables()[0][0];
    // Difficulty 25 needs 19.4: wins by 61 for 30 points.
    $this->assertSame(array('yes', '61', '30'), array($ruling['Success'], $ruling['Winsby'], $ruling['Points']));
  }

  public function testTargetResistanceRaisesTheDifficulty() {
    $text = $this->rule('magic', $this->spell(array('magic_target_stat_int' => 25, 'magic_target_stat_spi' => 25)));
    $this->assertStringContainsString('Resisted roll results:', $text);
    $this->assertStringContainsString("With target's resistance, the base difficulty is: 50 / 50", $text);
    $resisted = $this->tables()[1][0];
    $this->assertSame(array('yes', '30', '15'), array($resisted['Success'], $resisted['Winsby'], $resisted['Points']));
  }

  public function testUnresistedSpellsShowOneRuling() {
    $this->rule('magic', $this->spell());
    $this->assertCount(1, $this->tables());
  }

  public function testMagicNeedsNumbers() {
    $text = $this->rule('magic', $this->spell(array('magic_target_skill' => 'high')));
    $this->assertStringContainsString('Not all values given were numeric. We need numbers for math.', $text);
  }

  public function testBadlyFailedSpellIsAMishap() {
    $this->rule('magic', $this->spell(array('magic_might_a' => 5, 'magic_rolled_a' => 2, 'magic_might_b' => 5, 'magic_rolled_b' => 2)));
    $this->assertSame('mishap', $this->tables()[0][0]['Success']);
  }

  // ---------------------------------------------------------------------------
  // Player advisor.
  // ---------------------------------------------------------------------------

  private function advise(array $edit) {
    $this->site->placeBlock('dice_ruler', 'dice_ruler_player_assist', 'content');
    $this->login($this->site->createUser(array('player')));
    $this->get('node');
    $this->submitForm(NULL, $edit, 'Should I?');
    return $this->pageText();
  }

  public function testAdvisorEstimatesMultiAttacks() {
    $text = $this->advise(array(
      'multi_attack_str' => '60', 'multi_attack_dex' => '60', 'multi_attack_skill' => '20',
      'multi_attack_type' => (string) self::$classes['Medium'], 'multi_attack_target_might' => '50',
    ));
    $this->assertMatchesRegularExpression('/With your average might of 80, and an opponent of might 50 you can probably hit him \d+ time\(s\) for a total of [\d.]+\./', $text);
    $this->assertStringContainsString('This calculation assumes everyone rolls 50s.', $text);
  }

  public function testAdvisorAnswersOverAjax() {
    // Browsers press "Should I?" over AJAX: the answer must come back in the
    // results box, not only on a full page submit.
    $this->site->placeBlock('dice_ruler', 'dice_ruler_player_assist', 'content');
    $this->login($this->site->createUser(array('player')));
    $this->get('node');
    $this->triggerAjax('Should I?', array(
      'multi_attack_str' => '60', 'multi_attack_dex' => '60', 'multi_attack_skill' => '20',
      'multi_attack_type' => (string) self::$classes['Medium'], 'multi_attack_target_might' => '50',
    ), 'op');
    $box = $this->xpath()->query('//div[@id="dice_ruler_dice_assist_multi_attack_results"]')->item(0);
    $this->assertNotNull($box);
    $this->assertMatchesRegularExpression('/With your average might of 80, and an opponent of might 50 you can probably hit him \d+ time\(s\)/', preg_replace('/\s+/', ' ', $box->textContent));
  }

  public function testAdvisorWithAHopelessMatch() {
    $text = $this->advise(array(
      'multi_attack_str' => '5', 'multi_attack_dex' => '5', 'multi_attack_skill' => '1',
      'multi_attack_type' => (string) self::$classes['Light'], 'multi_attack_target_might' => '90',
    ));
    $this->assertStringContainsString('you can probably hit him 0 time(s) for a total of 0.', $text);
  }

  /**
   * @group knownbug
   * The advisor form's weapon class default reads an undefined $first, and a
   * first attack that misses leaves $last_damage undefined: PHP notices.
   */
  public function testAdvisorHasNoPhpErrors() {
    $this->advise(array(
      'multi_attack_str' => '5', 'multi_attack_dex' => '5', 'multi_attack_skill' => '1',
      'multi_attack_type' => (string) self::$classes['Light'], 'multi_attack_target_might' => '90',
    ));
    $this->assertSame(array(), $this->pageErrorsFrom('dice_ruler.module'));
  }

}
