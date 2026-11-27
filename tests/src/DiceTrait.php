<?php

namespace Vaxia\Tests;

/**
 * Posting dice rolls in chat rooms through the Vaxia dice roller (the chat post
 * form), and reading the rendered results.
 */
trait DiceTrait {

  /**
   * A player who may chat, roll and see rolls.
   */
  protected function roller(array $extraPermissions = array()) {
    return $this->site->createUser(array('player'), array_merge(array('view rpg chat', 'view vaxia ic chat', 'use dice roller', 'view dice rolls', 'post comments'), $extraPermissions));
  }

  /**
   * An approved character for $owner.
   */
  protected function rollingCharacter($owner, array $values = array()) {
    $character = $this->site->createCharacter($owner, $values + array('title' => $this->site->uniqueName('Hero')));
    $this->approveCharacter($character);
    return $character;
  }

  /**
   * An item on $character, written by staff, approved unless $state says otherwise.
   */
  protected function characterItem($character, array $values = array(), $state = 'approved') {
    $item = $this->site->createNode('items', $values + array(
      'uid' => $this->site->createUser(array('storyteller'))->uid,
      'title' => $this->site->uniqueName('Item'),
      'field_character_owner' => $character->nid,
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ));
    $path = array('draft', 'submitted', 'approved');
    foreach ($path as $step) {
      $this->transition($item, $step);
      if ($step === $state) {
        break;
      }
    }
    return $item;
  }

  /**
   * Item choices offered on the post form ("Using items"): nid => label.
   */
  protected function itemChoices($player, $room, $character) {
    $this->login($player);
    $this->get('node/' . $room->nid);
    $options = array();
    foreach ($this->xpath()->query('//select[@name="vaxia_rolls[items][]"]/option') as $opt) {
      $options[(int) $opt->getAttribute('value')] = trim($opt->textContent);
    }
    return $options;
  }

  /**
   * Posts in a room as $character with dice rows and items.
   *
   * @param array $rows
   *   Each: array('number' => 1, 'stat' => 'strength', 'skill' => -1).
   *
   * @return string[]
   *   Rendered dice lines of the new post ("str (5) + Items (2) => Might: 7 Roll: 79").
   */
  protected function postRoll($player, $room, $character, array $rows, array $items = array(), $text = NULL, array $extra = array()) {
    $text = $text ?: $this->site->uniqueName('post');
    $this->login($player);
    $this->get('node/' . $room->nid);
    $edit = array(
      'comment_body[und][0][value]' => $text,
      'field_comment_character[und]' => $character->nid,
    );
    foreach (array_values($rows) as $i => $row) {
      $edit['vaxia_rolls[dice][' . $i . '][number]'] = (string) ($row['number'] ?? 1);
      $edit['vaxia_rolls[dice][' . $i . '][stat]'] = $row['stat'] ?? '-1';
      if (isset($row['skill'])) {
        $edit['vaxia_rolls[dice][' . $i . '][skill]'] = (string) $row['skill'];
      }
    }
    if ($items) {
      $edit['vaxia_rolls[items][]'] = array_map(function ($i) {
        return (string) $i->nid;
      }, $items);
    }
    $this->submitForm(NULL, $edit + $extra, 'Post');
    return $this->diceLinesOf($text);
  }

  /**
   * Dice lines rendered under the post containing $text.
   */
  protected function diceLinesOf($text) {
    $xpath = $this->xpath();
    $lines = array();
    foreach ($xpath->query('//*[contains(@class, "comment")][.//text()[contains(., "' . $text . '")]]//span[@class="dice_render"]') as $span) {
      $lines[] = trim(preg_replace('/\s+/', ' ', $span->textContent));
    }
    return array_values(array_unique($lines));
  }

}
