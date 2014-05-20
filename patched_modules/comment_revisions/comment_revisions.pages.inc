<?php
/**
 * @file
 * Comment revision pages definitions.
 */

/**
 * Generate an overview table of older revisions of a user.
 */
function comment_revisions_overview($comment) {
  drupal_set_title(t('Revisions for %title', array('%title' => $comment->subject)), PASS_THROUGH);
  $header = array(
    t('Revision'),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  $revisions = comment_revisions_list($comment);
  $revert_permission = FALSE;
  $delete_permission = FALSE;
  global $user;
  if ($user->uid == $comment->uid) {
    if (user_access('revert own comments revisions') || user_access('revert comment revisions')) {
      $revert_permission = TRUE;
    }
    if (user_access('delete own comments revisions') || user_access('delete comment revisions')) {
      $delete_permission = TRUE;
    }
  }
  else {
    if (user_access('revert comment revisions')) {
      $revert_permission = TRUE;
    }
    if (user_access('delete comment revisions')) {
      $delete_permission = TRUE;
    }
  }
  $rows = array();
  foreach ($revisions as $revision) {
    $row = array();
    $operations = array();
    if ($revision->current_vid > 0) {
      $row[] = array(
        'data' => t('!date by !username', array(
          '!date' => l(format_date($revision->timestamp, 'short'), "comment/$comment->cid"),
          '!username' => theme('username', array('account' => $revision)),
        ))
        . (($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : ''),
        'class' => array('revision-current'),
      );
      $operations[] = array(
        'data' => drupal_placeholder(t('current revision')),
        'class' => array('revision-current'),
        'colspan' => 2,
      );
    }
    else {
      $row[] = t('!date by !username', array(
          '!date' => l(format_date($revision->timestamp, 'short'), "comment/$comment->cid/revisions/$revision->vid/view"),
          '!username' => theme('username', array('account' => $revision))
        ))
        . (($revision->log != '') ? '<p class="revision-log">' . filter_xss($revision->log) . '</p>' : '');
      if ($revert_permission) {
        $operations[] = l(t('revert'), "comment/$comment->cid/revisions/$revision->vid/revert");
      }
      else {
        // Empty row for theming purposes.
        $operations[] = "";
      }
      if ($delete_permission) {
        $operations[] = l(t('delete'), "comment/$comment->cid/revisions/$revision->vid/delete");
      }
      else {
        // Empty row for theming purposes.
        $operations[] = "";
      }
    }
    $rows[] = array_merge($row, $operations);
  }

  $build['user_revisions_table'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
    '#header' => $header,
  );

  return $build;
}

/**
 * Revert a revision.
 */
function comment_revisions_revert_confirm($form, $form_state, $comment_revision) {
  $form['#comment_revision'] = $comment_revision;
  return confirm_form($form, t('Are you sure you want to revert to the revision from %revision-date?', array('%revision-date' => format_date($comment_revision->revision_timestamp))), 'comment/' . $comment_revision->cid . '/revisions', '', t('Revert'), t('Cancel'));
}

function comment_revisions_revert_confirm_submit($form, &$form_state) {
  $comment_revision = $form['#comment_revision'];
  $comment_revision->revision = 1;
  $comment_revision->log = t('Copy of the revision from %date.', array('%date' => format_date($comment_revision->revision_timestamp)));
  comment_save($comment_revision);
  watchdog('comment', 'Reverted %title revision %revision.', array(
    '%title' => $comment_revision->subject,
    '%revision' => $comment_revision->vid,
  ));
  drupal_set_message(t('%title has been reverted back to the revision from %revision-date.', array(
    '%title' => $comment_revision->subject,
    '%revision-date' => format_date($comment_revision->revision_timestamp),
  )));
  $form_state['redirect'] = 'comment/' . $comment_revision->cid . '/revisions';
}

/**
 * Delete a revision.
 */
function comment_revisions_delete_confirm($form, $form_state, $comment_revision) {
  $form['#comment_revision'] = $comment_revision;
  return confirm_form($form, t('Are you sure you want to delete the revision of this comment from %revision-date?', array('%revision-date' => format_date($comment_revision->revision_timestamp))), 'comment/' . $comment_revision->cid . '/revisions', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

function comment_revisions_delete_confirm_submit($form, &$form_state) {
  $comment_revision = $form['#comment_revision'];
  comment_revisions_delete($comment_revision);
  watchdog('comment', 'Deleted %title revision of comment.', array(
    '%title' => $comment_revision->subject,
  ));
  drupal_set_message(t('Revision from %revision-date %title has been deleted.', array(
    '%revision-date' => format_date($comment_revision->revision_timestamp),
    '%title' => $comment_revision->subject,
  )));
  if (db_query('SELECT COUNT(vid) FROM {comment_revision} WHERE cid = :cid', array(':cid' => $comment_revision->cid))->fetchField() > 1) {
    $form_state['redirect'] = 'comment/' . $comment_revision->cid . '/revisions';
  }
  else {
    $form_state['redirect'] = 'node/' . $comment_revision->nid;
  }
}