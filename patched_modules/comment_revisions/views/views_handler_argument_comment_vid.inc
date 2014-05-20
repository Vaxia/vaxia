<?php
/**
 * @file
 * Provide comment revision vid argument handler.
 */

/**
 * Argument handler to accept a comment revision ID.
 */
class views_handler_argument_comment_vid extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the subject of the revision.
   */
  function title_query() {
    $subjects = array();

    $result = db_query("SELECT n.subject FROM {comment_revision} c WHERE c.cid IN (:cids)", array(':cids' => $this->value));
    foreach ($result as $comment) {
      $subjects[] = check_plain($comment->subject);
    }
    return $subjects;
  }
}

