<?php

namespace Vaxia\Tests\Vaxia\VaxiaCharacters;

use Vaxia\Tests\DiceTrait;
use Vaxia\Tests\VaxiaTestCase;

/**
 * The character form shows the species and languages fields of the chosen
 * realm (vaxia_core.settings realm_fields: Fantasy, Science Fiction and
 * Horror in the release; D7's conditional_fields 76-79 did this for Vaxia and
 * Sirian); "other" in the shown species list shows Other species (40-41).
 * The browser applies them (Drupal #states); these check the rules the form
 * sends. The chosen realm's languages are required, and Other species when
 * the species is "other" (only while shown, as D7).
 *
 * The personal room (and items) autocompletes offer the chosen realm's
 * content, Storyhost-realm and realm-less content (user, 2026-11-25), and
 * "Currently wearing" lists the character's own approved items (D7 view
 * character_items_ref).
 *
 *
 * @group vaxia_characters
 */
class CharacterRealmFieldsTest extends VaxiaTestCase {

  use DiceTrait;

  const REALM_FIELDS = array(
    'Fantasy' => array('field_fantasy_species', 'field_fantasy_languages'),
    'Science Fiction' => array('field_scifi_species', 'field_scifi_languages'),
    'Horror' => array('field_horror_species', 'field_horror_languages'),
  );

  /**
   * The #states rules on a field's wrapper, decoded.
   */
  private function statesOf($xpath, $field) {
    $selector = 'edit-' . str_replace('_', '-', $field) . '-wrapper';
    $el = $xpath->query('//*[@data-drupal-selector="' . $selector . '"][@data-drupal-states]')->item(0);
    $this->assertNotNull($el, $field . ' has visibility rules');
    return json_decode($el->getAttribute('data-drupal-states'), TRUE);
  }

  public function testCurrentlyWearingListsOwnApprovedItems() {
    $owner = $this->site->createUser(array('player'));
    $character = $this->rollingCharacter($owner);
    $mine = $this->characterItem($character, array('title' => $this->site->uniqueName('Mine')));
    $pending = $this->characterItem($character, array('title' => $this->site->uniqueName('Pending')), 'submitted');
    $other = $this->characterItem($this->rollingCharacter($owner), array('title' => $this->site->uniqueName('Other')));
    $this->login($this->superuser());
    $this->get('node/' . $character->nid . '/edit');
    $xpath = $this->xpath();
    $values = array();
    foreach ($xpath->query('//input[starts-with(@name, "field_currently_wearing")]') as $input) {
      $values[] = (int) $input->getAttribute('value');
    }
    $this->assertContains($mine->nid, $values);
    $this->assertNotContains($pending->nid, $values, 'Unapproved item not offered');
    $this->assertNotContains($other->nid, $values, 'Another character\'s item not offered');
  }

  /**
   * Personal room suggestions for the room name $q, as the form offers them.
   */
  private function roomSuggestions($q) {
    $path = $this->xpath()->query('//input[@name="field_personal_room[target_id]"]')->item(0)->getAttribute('data-autocomplete-path');
    $this->get(ltrim(html_entity_decode($path), '/') . '&q=' . rawurlencode($q));
    return array_map(function ($s) {
      return html_entity_decode(strip_tags($s['label']));
    }, json_decode($this->content, TRUE));
  }

  public function testPersonalRoomSuggestionsFollowTheRealm() {
    $word = $this->site->uniqueName('Hall');
    $vaxian = $this->site->createRoom(array('title' => $word . ' Vaxian', 'field_tag_realm' => $this->site->term('realm', 'Vaxia')));
    $sirian = $this->site->createRoom(array('title' => $word . ' Sirian', 'field_tag_realm' => $this->site->term('realm', 'Sirian')));
    $character = $this->site->createCharacter($this->site->createUser(array('player')));
    $this->login($this->superuser());
    $this->get('node/' . $character->nid . '/edit');
    $offered = $this->roomSuggestions($word);
    $this->assertContains($word . ' Vaxian', $offered);
    $this->assertNotContains($word . ' Sirian', $offered);
  }

  public function testSpeciesAndLanguagesFollowTheRealm() {
    $tids = array();
    foreach (array_keys(self::REALM_FIELDS) as $name) {
      $tids[$name] = (string) $this->site->term('realm', $name);
    }
    $character = $this->site->createCharacter($this->site->createUser(array('player')));
    $this->login($this->superuser());
    $this->get('node/' . $character->nid . '/edit');
    $this->assertStatus(200);
    $xpath = $this->xpath();
    $realm = ':input[name="field_tag_realm"]';
    $other = array();
    foreach (self::REALM_FIELDS as $name => list($species, $languages)) {
      $shown = array($realm => array('value' => $tids[$name]));
      $this->assertSame($shown, $this->statesOf($xpath, $species)['visible'], $species);
      $this->assertSame($shown, $this->statesOf($xpath, $languages)['visible'], $languages);
      $this->assertSame($shown, $this->statesOf($xpath, $languages)['required'], $languages . ' required while shown');
      if ($other) {
        $other[] = 'or';
      }
      $other[] = $shown + array(':input[name="' . $species . '"]' => array('value' => 'other'));
    }
    $states = $this->statesOf($xpath, 'field_other_species');
    $this->assertSame($other, $states['visible']);
    $this->assertSame($other, $states['required']);
  }

  /**
   * Saves the sheet as staff with $edit; the error messages.
   */
  private function saveAsStaff($character, array $edit) {
    $this->login($this->superuser());
    $this->submitForm('node/' . $character->nid . '/edit', $edit, 'Save');
    return $this->messages('error');
  }

  public function testOnlyTheChosenRealmsLanguagesAreRequired() {
    $character = $this->site->createCharacter($this->site->createUser(array('player')), array('field_tag_realm' => $this->site->term('realm', 'Fantasy')));
    $errors = $this->saveAsStaff($character, array());
    $this->assertContains('Fantasy languages field is required.', $errors);
    $this->assertNotContains('Science Fiction languages field is required.', $errors);
    $this->assertNotContains('Other Species field is required.', $errors);

    $errors = $this->saveAsStaff($character, array('field_fantasy_languages[common_nat]' => TRUE));
    $this->assertNotContains('Fantasy languages field is required.', $errors);
  }

  public function testOtherSpeciesIsRequiredOnlyForOther() {
    // "other" is offered to players with 40 HXP or more.
    $owner = $this->site->createUser(array('player'), array(), array('field_xp_over_time' => 50));
    $character = $this->site->createCharacter($owner, array('field_tag_realm' => $this->site->term('realm', 'Horror'), 'field_horror_languages' => 'common_nat'));
    $this->assertNotContains('Other Species field is required.', $this->saveAsStaff($character, array('field_horror_species' => 'ghost')));
    $errors = $this->saveAsStaff($character, array('field_horror_species' => 'other'));
    $this->assertContains('Other Species field is required.', $errors);
  }

  public function testSheetsCarryTheirRealmAsAClass() {
    // The realm's name in lower case with dashes (user, 2026-11-26).
    $character = $this->site->createCharacter($this->site->createUser(array('player')), array('field_tag_realm' => $this->site->term('realm', 'Science Fiction')));
    $this->login($this->superuser());
    $this->get('node/' . $character->nid);
    $this->assertSame(1, $this->xpath()->query('//*[contains(concat(" ", @class, " "), " realm-science-fiction ")]')->length);
  }

  public function testSheetsShowTheDerivedStats() {
    // D7's computed fields, read-only averages of two stats (user,
    // 2026-11-26). Other players see the public ones only (Presence and
    // Appearance, as on D7).
    $owner = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($owner, array(
      'field_life' => 20, 'field_endurance' => 25, 'field_strength' => 25, 'field_dexterity' => 32,
      'field_intelligence' => 22, 'field_spirituality' => 20, 'field_charisma' => 19,
    ));
    $derived = array('Health' => '22.5', 'Constitution' => '25', 'Agility' => '28.5', 'Finesse' => '27', 'Awareness' => '21', 'Presence' => '19.5', 'Appearance' => '19.5');
    $this->login($owner);
    $this->get('node/' . $character->nid);
    $text = preg_replace('/\s+/', ' ', $this->pageText());
    foreach ($derived as $label => $value) {
      $this->assertMatchesRegularExpression('/' . $label . ' :? ?' . preg_quote($value, '/') . '\b/', $text, $label);
    }
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $character->nid);
    $text = preg_replace('/\s+/', ' ', $this->pageText());
    $this->assertMatchesRegularExpression('/Presence :? ?19\.5/', $text);
    $this->assertMatchesRegularExpression('/Appearance :? ?19\.5/', $text);
    foreach (array('Health', 'Constitution', 'Agility', 'Finesse', 'Awareness') as $label) {
      $this->assertStringNotContainsString($label, $text);
    }
  }

  public function testOtherPlayersSeeTheSpecies() {
    // The realms' species fields are public (the setting still named the
    // original site's species field; review, 2026-11-26).
    $character = $this->site->createCharacter($this->site->createUser(array('player')), array(
      'field_tag_realm' => $this->site->term('realm', 'Fantasy'),
      'field_fantasy_species' => 'elf',
    ));
    $this->approveCharacter($character);
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $character->nid);
    $this->assertText('Elf');
  }

  public function testPlayersCannotMarkASheetAnOpenNpc() {
    // An open NPC skips evaluation: players don't get the box (review,
    // 2026-11-26). Storyhosts do.
    $this->login($this->site->createUser(array('player')));
    $this->get('node/add/character_sheet');
    $this->assertSame(0, $this->xpath()->query('//input[@name="field_is_open_npc[value]"]')->length);
    $this->login($this->site->createUser(array('player', 'storyteller')));
    $this->get('node/add/character_sheet');
    $this->assertSame(1, $this->xpath()->query('//input[@name="field_is_open_npc[value]"]')->length);
  }

}
