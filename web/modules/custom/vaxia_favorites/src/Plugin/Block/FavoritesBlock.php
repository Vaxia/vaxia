<?php

namespace Drupal\vaxia_favorites\Plugin\Block;

use Drupal\Component\Utility\Html;
use Drupal\Core\Access\AccessResult;
use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\Block\BlockBase;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\Core\Url;
use Drupal\vaxia_favorites\Form\AddFavoriteForm;

/**
 * "My Favorites" (D7 favorites 7.x-2.1, block favorites/0): the user's saved
 * links in their order, an "x" to remove each, drag to reorder, and "add this
 * page". For "manage own favorites"; anonymous visitors never had it, so D7's
 * cookie storage for them isn't ported.
 */
#[Block(id: 'favorites', admin_label: new TranslatableMarkup('User Favorites'))]
class FavoritesBlock extends BlockBase {

  protected function blockAccess(AccountInterface $account) {
    return AccessResult::allowedIf($account->isAuthenticated() && $account->hasPermission('manage own favorites'))
      ->cachePerPermissions()
      ->cachePerUser();
  }

  public function build() {
    $uid = (int) \Drupal::currentUser()->id();
    $csrf = \Drupal::csrfToken();
    $destination = \Drupal::destination()->get();
    $rows = \Drupal::database()->select('favorites', 'f')
      ->fields('f')
      ->condition('uid', $uid)
      ->orderBy('weight', 'ASC')
      ->orderBy('timestamp', 'DESC')
      ->execute();
    $items = [];
    foreach ($rows as $row) {
      $query = @unserialize($row->query, ['allowed_classes' => FALSE]);
      $items[] = [
        'link' => [
          '#type' => 'link',
          '#title' => $row->title,
          '#url' => Url::fromUserInput('/' . ltrim($row->path, '/'), ['query' => is_array($query) ? $query : []]),
        ],
        'space' => ['#plain_text' => ' '],
        'remove' => [
          '#type' => 'link',
          '#title' => 'x',
          '#url' => Url::fromRoute('vaxia_favorites.remove', ['fid' => $row->fid], [
            'query' => [
              'token' => $csrf->get($row->timestamp . $row->uid . $row->path),
              'destination' => $destination,
            ],
            'attributes' => [
              'class' => ['favorites-remove'],
              'title' => $this->t('delete this item'),
              'fid' => $row->fid,
            ],
          ]),
        ],
      ];
    }

    // The page title is the default title for "add this page".
    $request = \Drupal::request();
    $title = '';
    $route = \Drupal::routeMatch()->getRouteObject();
    if ($route) {
      $title = \Drupal::service('title_resolver')->getTitle($request, $route);
      if (is_array($title)) {
        $title = \Drupal::service('renderer')->renderInIsolation($title);
      }
      $title = Html::decodeEntities(strip_tags((string) $title));
    }
    if ($title === '') {
      $title = (string) (\Drupal::config('system.site')->get('name') ?: 'Home');
    }

    return [
      'list' => [
        '#type' => 'container',
        '#attributes' => ['id' => 'favorites-list'],
        'items' => ['#theme' => 'item_list', '#items' => $items, '#list_type' => 'ul'],
      ],
      'form' => \Drupal::formBuilder()->getForm(AddFavoriteForm::class, $title),
      '#attached' => [
        'library' => ['vaxia_favorites/favorites'],
        'drupalSettings' => [
          'favorites' => [
            // Not the routed URL: its token would be a markup placeholder, which
            // drupalSettings doesn't replace.
            'orderPath' => Url::fromUri('base:favorites/js/order', ['query' => ['token' => $csrf->get('favorites/js/order')]])->toString(),
          ],
        ],
      ],
    ];
  }

  public function getCacheContexts() {
    return array_merge(parent::getCacheContexts(), ['user', 'url']);
  }

  /**
   * D7 never cached it (DRUPAL_NO_CACHE); the remove links and the reorder
   * path carry session tokens.
   */
  public function getCacheMaxAge() {
    return 0;
  }

}
