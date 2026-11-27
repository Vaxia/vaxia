<?php

namespace Vaxia\Tests\Ui\DisplayPrefs;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Display preferences: production's header blocks that each user could turn
 * on or off from their account form (D7 block visibility "users may
 * choose"), each adding a few lines of CSS. The scrollable chat box is on
 * unless turned off; the rest are off unless turned on.
 *
 * The blocks are production content the D7 test site doesn't have, so these
 * run on the port only.
 *
 * @group display_prefs
 */
class DisplayPrefsTest extends VaxiaTestCase {

  const SCROLL = '#rpg-chat{height:900px;overflow-y:scroll;}';
  const NO_DICE = '.dice_rule{display:none;}';
  const LARGER = 'body{font-size:110%;}';

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s display blocks.');
    }
  }

  public function testDefaultsForNewPlayer() {
    $this->login($this->site->createUser(array('player')));
    $this->get('node');
    $this->assertRaw(self::SCROLL);
    $this->assertNoRaw(self::NO_DICE);
    $this->assertNoRaw(self::LARGER);
  }

  public function testAnonymousGetsDefaults() {
    $this->anonymous();
    $this->get('user/login');
    $this->assertRaw(self::SCROLL);
    $this->assertNoRaw(self::NO_DICE);
  }

  public function testAccountFormListsEveryChoice() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('user/' . $player->uid . '/edit');
    foreach (array('Display chat posts in scrollable box', 'Hide dice assistant buttons', 'Display using larger font (10% larger)', 'Display using larger font (20% larger)', 'Use smaller character images in chat', 'Override text colors in chatrooms w/ grey - high contrast', 'Override text colors in chatrooms w/ white - high contrast', 'Use black background - high contrast', 'Use blue background - high contrast', 'Use white background - high contrast') as $label) {
      $this->assertRaw($label);
    }
  }

  public function testTurningChoicesOnAndOff() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->submitForm('user/' . $player->uid . '/edit', array('block[20]' => TRUE, 'block[4]' => TRUE, 'block[14]' => FALSE), 'Save');
    $this->get('node');
    $this->assertRaw(self::NO_DICE);
    $this->assertRaw(self::LARGER);
    $this->assertNoRaw(self::SCROLL);

    $this->submitForm('user/' . $player->uid . '/edit', array('block[20]' => FALSE, 'block[4]' => FALSE, 'block[14]' => TRUE), 'Save');
    $this->get('node');
    $this->assertNoRaw(self::NO_DICE);
    $this->assertNoRaw(self::LARGER);
    $this->assertRaw(self::SCROLL);
  }

  public function testChoicesStayWithTheirOwner() {
    // Cached pages must not carry one player's choices to another.
    $chooser = $this->site->createUser(array('player'));
    $other = $this->site->createUser(array('player'));
    $this->login($chooser);
    $this->submitForm('user/' . $chooser->uid . '/edit', array('block[20]' => TRUE), 'Save');
    $this->get('node');
    $this->assertRaw(self::NO_DICE);

    $this->login($other);
    $this->get('node');
    $this->assertNoRaw(self::NO_DICE);

    $this->login($chooser);
    $this->get('node');
    $this->assertRaw(self::NO_DICE);
  }

  public function testFormShowsSavedChoices() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->submitForm('user/' . $player->uid . '/edit', array('block[28]' => TRUE, 'block[14]' => FALSE), 'Save');
    $this->get('user/' . $player->uid . '/edit');
    $this->assertSame(1, $this->xpath()->query('//input[@name="block[28]" and @checked]')->length);
    $this->assertSame(0, $this->xpath()->query('//input[@name="block[14]" and @checked]')->length);
  }

}
