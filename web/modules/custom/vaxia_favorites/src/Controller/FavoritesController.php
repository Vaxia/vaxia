<?php

namespace Drupal\vaxia_favorites\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\JsonResponse;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpKernel\Exception\NotFoundHttpException;

/**
 * Removing and reordering favorites (D7 favorites/remove/% and
 * favorites/js/order/%/%).
 */
class FavoritesController extends ControllerBase {

  /**
   * Removes one of the user's favorites; back to ?destination=.
   */
  public function remove($fid, Request $request) {
    $favorite = \Drupal::database()->query('SELECT * FROM {favorites} WHERE fid = :fid', [':fid' => $fid])->fetchObject();
    if (!$favorite) {
      throw new NotFoundHttpException();
    }
    $token = (string) $request->query->get('token', '');
    if ((int) $favorite->uid === (int) $this->currentUser()->id() && \Drupal::csrfToken()->validate($token, $favorite->timestamp . $favorite->uid . $favorite->path)) {
      \Drupal::database()->delete('favorites')->condition('fid', $favorite->fid)->execute();
    }
    else {
      $this->messenger()->addError($this->t('You do not have permission to remove this favorite.'));
    }
    // The destination in the query string wins over this.
    return $this->redirect('<front>');
  }

  /**
   * Saves the order the user dragged their favorites into: fids[] top first.
   */
  public function order(Request $request) {
    $fids = $request->request->all()['fids'] ?? [];
    if (!is_array($fids) || !$fids) {
      return new JsonResponse(['success' => FALSE, 'error' => TRUE]);
    }
    $uid = (int) $this->currentUser()->id();
    foreach (array_values($fids) as $i => $fid) {
      \Drupal::database()->update('favorites')
        ->fields(['weight' => $i + 1])
        ->condition('fid', (int) $fid)
        ->condition('uid', $uid)
        ->execute();
    }
    return new JsonResponse(['success' => TRUE]);
  }

}
