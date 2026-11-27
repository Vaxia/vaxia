<?php

namespace Vaxia\Tests\Ui\BackgroundChanger;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Background changer: a header block (production shows it on every page) that,
 * for users allowed to use it, loads a script swapping the page background by
 * time of day from the module's images.
 *
 * @group background_changer
 */
class BackgroundChangerTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    $this->site->placeBlock('background_changer', 'background_changer', 'header');
  }

  public function testScriptLoadsForUsersWithPermission() {
    $this->login($this->site->createUser(array('player'), array('use background changer')));
    $this->get('node');
    $this->assertRaw('background_changer/js/background_changer.js');
    $this->assertRaw('background_changer\/images\/');
  }

  public function testNoScriptWithoutPermission() {
    $this->withoutPermission('use background changer', array('anonymous user', 'authenticated user', 'player'), function () {
      $this->login($this->site->createUser(array('player')));
      $this->get('node');
      $this->assertStatus(200);
      $this->assertNoRaw('background_changer.js');
    });
  }

  public function testImagesAreServed() {
    // The images live wherever the page settings say (the module's folder).
    $this->login($this->site->createUser(array('player'), array('use background changer')));
    $this->get('node');
    $this->assertMatchesRegularExpression('#"path":"([^"]+background_changer\\\\/images\\\\/)"#', $this->content);
    preg_match('#"path":"([^"]+background_changer\\\\/images\\\\/)"#', $this->content, $m);
    $this->anonymous();
    $this->get(str_replace('\\/', '/', $m[1]) . '0.jpg');
    $this->assertStatus(200);
  }

}
