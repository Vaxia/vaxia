<?php
/**
 * @file
 * Controller class for comment_revisions.
 *
 * This extends the CommentController class, adding required
 * revision handling for comment objects.
 */
class CommentRevisionController extends CommentController {
  function attachLoad(&$comments, $revision_id = FALSE) {
    parent::attachLoad($comments, $revision_id);
    foreach ($comments as $key => $record) {
      $comments[$key]->revision = 1;
    }
  }

  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    $query = parent::buildQuery($ids, $conditions, $revision_id);
    $fields =& $query->getFields();
    unset($fields['timestamp']);
    $query->addField('revision', 'timestamp', 'revision_timestamp');
    $query->addField('revision', 'authorid', 'revision_authorid');
    $fields['cid']['table'] = 'base';
    return $query;
  }
}
