<?php
/**
 * @file
 * Provide views data and handlers for comment_revisions.module
 */

/**
 * Implements hook_views_data().
 */
function comment_revisions_views_data() {

  // Join comment table.
  $data['comment']['table']['join'] = array(
    'comment_revision' => array(
      'left_field' => 'cid',
      'field' => 'cid',
      'type' => 'INNER',
    ),
  );

  $data['comment_revision']['table']['group'] = t('Comment revision');
  $data['comment_revision']['table']['base'] = array(
    'field' => 'vid',
    'title' => t('Comment revision'),
    'help' => t('Comment revision is the history of comment changes.'),
  );

  // Describe how we join comment revisions.
  $data['comment_revision']['table']['join'] = array(
    'comment' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  // User reference to the author of comment revision.
  $data['comment_revision']['authorid'] = array(
    'title' => t('Revision author'),
    'help' => t('Relate an comment revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('Revision author'),
    ),
  );

  // Revision ID of comment.
  $data['comment_revision']['vid'] = array(
    'title' => t('Vid'),
    'help' => t('The revision ID of the comment revision.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a vid as an argument.
    'argument' => array(
      'handler' => 'views_handler_argument_comment_vid',
      'click sortable' => TRUE,
      'numeric' => TRUE,
    ),
    // Information for accepting a uid as a filter.
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a vid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'comment',
      'base field' => 'vid',
      'title' => t('Comment current version'),
      'label' => t('Get the current version of comment.'),
    ),
  );

  // Comment ID field.
  $data['comment_revision']['cid'] = array(
    'title' => t('Cid'),
    'help' => t('The comment ID field from the comment revisions table.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'comment',
      'base field' => 'cid',
      'title' => t('Comment'),
      'label' => t('Get all revisions of the comment.'),
    ),
  );

  // Subject of comment revision.
  $data['comment_revision']['subject'] = array(
    'title' => t('Subject'),
    'help' => t('The subject of comment.'),
    // Information for displaying a title as a field.
    'field' => array(
      'field' => 'subject',
      'handler' => 'views_handler_field_comment_revision',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Log message of comment revision.
  $data['comment_revision']['log'] = array(
    'title' => t('Log message'),
    'help' => t('The log message entered when the revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // Revision timestamp.
  $data['comment_revision']['timestamp'] = array(
    'title' => t('Created'),
    'help' => t('The date when the comment revision was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Link to revert revision.
  $data['comment_revision']['revert_revision'] = array(
    'field' => array(
      'title' => t('Revert link'),
      'help' => t('Provide a simple link to revert to the revision.'),
      'handler' => 'views_handler_field_comment_revisions_link_revert',
    ),
  );

  // Link to delete revision.
  $data['comment_revision']['delete_revision'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the comment revision.'),
      'handler' => 'views_handler_field_comment_revisions_link_delete',
    ),
  );

  return $data;
}
