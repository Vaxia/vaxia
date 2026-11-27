<?php

namespace Drupal\vaxia_characters\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\user\UserInterface;
use Drupal\vaxia_core\Character;

/**
 * Artwork lists (D7 view "artwork"): the gallery of approved art, and a
 * player's own art still on the site.
 */
class ArtworkList extends ControllerBase {

  /**
   * Art nodes, newest first.
   *
   * @return \Drupal\node\NodeInterface[]
   */
  private function art(array $conditions): array {
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'artwork')->condition('status', 1)->sort('created', 'DESC')->range(0, 35);
    foreach ($conditions as $field => $value) {
      $query->condition($field, $value, is_array($value) ? 'IN' : '=');
    }
    return $storage->loadMultiple($query->execute());
  }

  private function grid(array $nodes): array {
    $builder = $this->entityTypeManager()->getViewBuilder('node');
    return [
      '#theme' => 'item_list',
      '#items' => array_map(fn($n) => $builder->view($n, 'teaser'), array_values($nodes)),
      '#empty' => $this->t('No artwork.'),
      '#cache' => ['max-age' => 0],
    ];
  }

  public function gallery() {
    return $this->grid($this->art(['field_art_evaluation' => 'art_evaluation_approved']));
  }

  public function mine(UserInterface $user) {
    // Live art: not removed from the site.
    $nodes = array_filter($this->art(['uid' => $user->id()]), fn($n) => Character::state($n) !== 'art_evaluation_removed_from_site');
    $build = $this->grid($nodes);
    $build['#title'] = $this->t('My artwork');
    return $build;
  }

  /**
   * The "My art" tab (D7 artwork/mine): the viewer's published art still in
   * creation or approved, by title, 35 a page.
   */
  public function mineTab() {
    $storage = $this->entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(TRUE)->condition('type', 'artwork')->condition('status', 1)
      ->condition('uid', $this->currentUser()->id())
      ->condition('field_art_evaluation', ['art_evaluation_creation', 'art_evaluation_approved'], 'IN')
      ->sort('title')->pager(35)->execute();
    $cache = ['contexts' => ['user', 'url.query_args.pagers:0'], 'tags' => ['node_list']];
    return ['list' => ['#cache' => $cache] + $this->grid($storage->loadMultiple($ids)), 'pager' => ['#type' => 'pager'], '#cache' => $cache];
  }

}
