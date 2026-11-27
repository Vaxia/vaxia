<?php

namespace Vaxia\Tests\Ui\SelectImage;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * Select image: a select-list widget that shows image thumbnails (a jQuery
 * dropdown). Production uses it for the "Artwork" picker on chat posts, listing
 * the poster's approved art.
 *
 * @group select_image
 */
class SelectImageTest extends VaxiaTestCase {

  use DiceTrait;

  private function approvedArt($owner) {
    $art = $this->site->createNode('artwork', array('uid' => $owner->uid, 'title' => $this->site->uniqueName('Portrait')));
    $this->transition($art, 'approved');
    return $art;
  }

  public function testArtworkPickerUsesTheImageSelect() {
    $player = $this->roller();
    $art = $this->approvedArt($player);
    $this->login($player);
    $this->get('node/' . $this->site->createRoom()->nid);

    $select = $this->xpath()->query('//select[@name="' . $this->fieldName('field_artwork[und]') . '"]')->item(0);
    $this->assertNotNull($select, 'Artwork picker');
    $this->assertStringContainsString('form-select-image', $select->getAttribute('class'));
    $this->assertSame(1, $this->xpath()->query('//select[@name="' . $this->fieldName('field_artwork[und]') . '"]/option[@value="' . $art->nid . '"]')->length, 'Own approved art offered');
    $this->assertRaw('select_image/js/select_image.js');
    $this->assertRaw('select_image/js/jquery.dd.min.js');
  }

  public function testOthersArtIsNotOffered() {
    $player = $this->roller();
    $theirs = $this->approvedArt($this->site->createUser(array('player')));
    $this->login($player);
    $this->get('node/' . $this->site->createRoom()->nid);
    $this->assertSame(0, $this->xpath()->query('//select[@name="' . $this->fieldName('field_artwork[und]') . '"]/option[@value="' . $theirs->nid . '"]')->length);
  }

  public function testPostKeepsTheChosenArt() {
    $player = $this->roller();
    $character = $this->rollingCharacter($player);
    $art = $this->approvedArt($player);
    $room = $this->site->createRoom();
    $text = $this->site->uniqueName('post');

    $this->postRoll($player, $room, $character, array(), array(), $text, array('field_artwork[und]' => (string) $art->nid));

    $this->assertStatus(200);
    $this->assertStringNotContainsString('An illegal choice', implode(' ', $this->messages('error')));
    $this->get('node/' . $room->nid);
    $this->assertText($text);
  }

  public function testThePickedArtShowsBesideThePostBox() {
    // dice_helper.js shows the picked art's thumbnail: the room sends each
    // offered art's thumbnail URL (user, 2026-11-26).
    $player = $this->roller();
    $art = $this->site->createNode('artwork', array('uid' => $player->uid, 'title' => $this->site->uniqueName('Portrait'), 'field_artwork_image' => $this->site->createFile($this->site->uniqueName('portrait') . '.png')));
    $this->transition($art, 'approved');
    $bare = $this->approvedArt($player);
    $this->login($player);
    $this->get('node/' . $this->site->createRoom()->nid);
    $json = $this->xpath()->query('//script[@data-drupal-selector="drupal-settings-json"]')->item(0);
    $this->assertNotNull($json);
    $settings = json_decode($json->textContent, TRUE);
    $this->assertStringContainsString('/styles/thumbnail/public/', $settings['vaxiaArtwork'][$art->nid] ?? '');
    $this->assertArrayNotHasKey($bare->nid, $settings['vaxiaArtwork']);
  }

}
