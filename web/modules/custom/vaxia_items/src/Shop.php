<?php

namespace Drupal\vaxia_items;

use Drupal\node\NodeInterface;
use Drupal\vaxia_characters\Silver;
use Drupal\vaxia_core\Character;

/**
 * The item shop (D7 item_shop): approved items marked "in shop", bought with
 * silver (and economy levels) by characters of the same realm.
 */
class Shop {

  /**
   * A price as a number: "1,200.50" is 1200.5 (D7 also stripped the decimal
   * point, so 12.50 cost 1250: known bug). NULL when not a price.
   */
  public static function price(NodeInterface $item): ?float {
    $value = str_replace(',', '', trim((string) $item->get('field_value')->value));
    return is_numeric($value) ? (float) $value : NULL;
  }

  public static function realm(NodeInterface $node): string {
    return $node->hasField('field_tag_realm') && $node->get('field_tag_realm')->entity ? strtolower($node->get('field_tag_realm')->entity->label()) : 'storyhost';
  }

  /**
   * Silver plus what the character's economy levels are worth.
   */
  public static function totalWorth(NodeInterface $character): float {
    return Silver::available((int) $character->id()) + Silver::worth(Silver::economy((int) $character->id()));
  }

  /**
   * Items a character may buy: in the shop, approved, same realm, affordable.
   * A character not in play sees none.
   *
   * @return \Drupal\node\NodeInterface[]
   */
  public static function stock(NodeInterface $character): array {
    if (!Character::isPlayable($character)) {
      return [];
    }
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(FALSE)
      ->condition('type', 'items')->condition('status', 1)->condition('field_in_shop', 1)
      ->condition('field_item_evaluation', \Drupal::config('vaxia_items.settings')->get('items_approved_workflow'))
      ->execute();
    $worth = self::totalWorth($character);
    $realm = self::realm($character);
    $stock = [];
    foreach ($storage->loadMultiple($ids) as $item) {
      $price = self::price($item);
      if ($price !== NULL && $price <= $worth && self::realm($item) === $realm) {
        $stock[$item->id()] = $item;
      }
    }
    uasort($stock, fn($a, $b) => [self::price($a), $a->label()] <=> [self::price($b), $b->label()]);
    return $stock;
  }

  /**
   * A new approved copy of an item, owned by $uid for $character_nid (or the
   * shop when NULL).
   */
  public static function copy(NodeInterface $item, int $uid, ?int $character_nid, bool $in_shop, string $comment): NodeInterface {
    // A fresh node with the item's field values (createDuplicate() trips
    // workflow 2.2.2, which works out a default state before the copy has an
    // owner).
    $values = [];
    foreach ($item->getFields(FALSE) as $name => $field) {
      if (strpos($name, 'field_') === 0 || in_array($name, ['title', 'body', 'status'], TRUE)) {
        $values[$name] = $field->getValue();
      }
    }
    $values['type'] = $item->bundle();
    $values['uid'] = $uid;
    $values['field_in_shop'] = $in_shop ? 1 : 0;
    $values['field_character_owner'] = $character_nid;
    $values['field_item_evaluation'] = 'item_evaluation_creation';
    $copy = \Drupal\node\Entity\Node::create($values);
    $copy->save();
    Character::forceState($copy, \Drupal::config('vaxia_items.settings')->get('items_approved_workflow'), $comment);
    return $copy;
  }

  public static function sellbackAllowed(NodeInterface $item): bool {
    $config = \Drupal::config('vaxia_items.settings');
    $account = \Drupal::currentUser();
    if (!$config->get('item_shop_sellback') || !$account->hasPermission('sellback item shop') || !Item::isItem($item)) {
      return FALSE;
    }
    $value = self::price($item);
    $character = $item->get('field_character_owner')->entity;
    return $item->get('field_sellback')->value && $value !== NULL && $value >= 1 && $character && $character->getOwnerId() == $account->id();
  }

  public static function sellbackValue(NodeInterface $item): float {
    return self::price($item) * ((float) \Drupal::config('vaxia_items.settings')->get('item_shop_sellback_percentage') / 100);
  }

}
