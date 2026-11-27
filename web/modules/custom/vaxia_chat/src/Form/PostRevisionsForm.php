<?php

namespace Drupal\vaxia_chat\Form;

use Drupal\comment\CommentInterface;
use Drupal\Core\Access\AccessResult;
use Drupal\Core\Session\AccountInterface;
use Drupal\node\NodeInterface;
use Drupal\vaxia_chat\Room;
use Drupal\Component\Diff\Diff;
use Drupal\Core\Diff\DiffFormatter;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * A chat post's edit history for staff: pick two versions and compare them
 * (D7 comment_revisions + comment_revisions_diff).
 */
class PostRevisionsForm extends FormBase {

  /**
   * Route access: the post's room must let the viewer in (locked, mature and
   * personal rooms: Room::denyReason()).
   */
  public static function access(CommentInterface $comment, AccountInterface $account) {
    $host = $comment->getCommentedEntity();
    $denied = $host instanceof NodeInterface && Room::isRoom($host) && Room::denyReason($host, $account) !== NULL;
    return AccessResult::allowedIf(!$denied)->addCacheableDependency($comment)->cachePerUser();
  }

  public function getFormId() {
    return 'comment_revisions_diff_comment_revisions';
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?CommentInterface $comment = NULL) {
    $form_state->set('cid', (int) $comment->id());
    $revisions = \Drupal::database()->query('SELECT r.vid, r.timestamp, u.name FROM {vaxia_chat_post_revision} r LEFT JOIN {users_field_data} u ON u.uid = r.uid WHERE r.cid = :cid ORDER BY r.vid DESC', [':cid' => $comment->id()])->fetchAll();
    $options = [];
    foreach ($revisions as $revision) {
      $options[$revision->vid] = $this->t('@date by @name', ['@date' => \Drupal::service('date.formatter')->format($revision->timestamp), '@name' => $revision->name ?? $this->t('Anonymous')]);
    }
    $form['#title'] = $this->t('Revisions for %title', ['%title' => $comment->getSubject()]);
    if (count($options) < 2) {
      $form['none'] = ['#markup' => $this->t('This post has not been edited.')];
      return $form;
    }
    $vids = array_keys($options);
    // Two radio sets named "old" and "new", as D7's form had.
    $form['old'] = ['#type' => 'radios', '#title' => $this->t('Older'), '#options' => $options, '#default_value' => $vids[1]];
    $form['new'] = ['#type' => 'radios', '#title' => $this->t('Newer'), '#options' => $options, '#default_value' => $vids[0]];
    $form['submit'] = ['#type' => 'submit', '#value' => $this->t('Compare')];
    if ($form_state->get('diff')) {
      $form['diff'] = $form_state->get('diff');
    }
    return $form;
  }

  public function validateForm(array &$form, FormStateInterface $form_state) {
    if ($form_state->getValue('old') == $form_state->getValue('new')) {
      $form_state->setErrorByName('old', $this->t('Select two different revisions to compare.'));
    }
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $db = \Drupal::database();
    $body = fn($vid) => (string) $db->query('SELECT body FROM {vaxia_chat_post_revision} WHERE vid = :vid AND cid = :cid', [':vid' => $vid, ':cid' => $form_state->get('cid')])->fetchField();
    [$old, $new] = [(int) $form_state->getValue('old'), (int) $form_state->getValue('new')];
    if ($old > $new) {
      [$old, $new] = [$new, $old];
    }
    $diff = new Diff(explode("\n", $body($old)), explode("\n", $body($new)));
    $formatter = new DiffFormatter(\Drupal::configFactory());
    $formatter->show_header = FALSE;
    $form_state->set('diff', [
      '#type' => 'table',
      '#attributes' => ['class' => ['diff']],
      '#header' => [['data' => $this->t('Older'), 'colspan' => 2], ['data' => $this->t('Newer'), 'colspan' => 2]],
      '#rows' => $formatter->format($diff),
    ]);
    $form_state->setRebuild();
  }

}
