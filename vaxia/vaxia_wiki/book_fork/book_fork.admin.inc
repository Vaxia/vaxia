<?php

/**
 * @file
 * Administration page callbacks for the Book module.
 */

/**
 * Form constructor for administering a single book's hierarchy.
 *
 * @see book_admin_edit_submit()
 *
 * @param $node
 *   The node of the top-level page in the book.
 *
 * @see book_admin_edit_validate()
 * @see book_admin_edit_submit()
 * @ingroup forms
 */
function book_fork_admin_edit($form, $form_state, $node) {
  drupal_set_title($node->title);
  $form['#node'] = $node;
  _book_fork_admin_table($node, $form);
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save book pages'),
  );

  return $form;
}

/**
 * Form validation handler for book_admin_edit().
 *
 * Checks that the book has not been changed while using the form.
 *
 * @see book_admin_edit_submit()
 */
function book_fork_admin_edit_validate($form, &$form_state) {
  if ($form_state['values']['tree_hash'] != $form_state['values']['tree_current_hash']) {
    form_set_error('', t('This book has been modified by another user, the changes could not be saved.'));
  }
}

/**
 * Form submission handler for book_admin_edit().
 *
 * This function takes care to save parent menu items before their children.
 * Saving menu items in the incorrect order can break the menu tree.
 *
 * @see book_admin_edit_validate()
 * @see menu_overview_form_submit()
 */
function book_fork_admin_edit_submit($form, &$form_state) {
  // Save elements in the same order as defined in post rather than the form.
  // This ensures parents are updated before their children, preventing orphans.
  $order = array_flip(array_keys($form_state['input']['table']));
  $form['table'] = array_merge($order, $form['table']);

  foreach (element_children($form['table']) as $key) {
    if ($form['table'][$key]['#item']) {
      $row = $form['table'][$key];
      $values = $form_state['values']['table'][$key];

      // Update menu item if moved or not.
      //if ($row['plid']['#default_value'] != $values['plid'] || $row['weight']['#default_value'] != $values['weight']) {
        $row['#item']['plid'] = $values['plid'];
        $row['#item']['weight'] = $values['weight'];
        menu_link_save($row['#item']);
      //}

      // Update the title if changed.
      if ($row['title']['#default_value'] != $values['title']) {
        $node = node_load($values['nid']);
        $langcode = LANGUAGE_NONE;
        $node->title = $values['title'];
        $node->book['link_title'] = $values['title'];
        $node->revision = 1;
        $node->log = t('Title changed from %original to %current.', array('%original' => $node->title, '%current' => $values['title']));
        node_save($node);
        watchdog('content', 'book: updated %title.', array('%title' => $node->title), WATCHDOG_NOTICE, l(t('view'), 'node/' . $node->nid));
      }

    }
  }

  drupal_set_message(t('Updated book %title.', array('%title' => $form['#node']->title)));
}

/**
 * Builds the table portion of the form for the book administration page.
 *
 * @param $node
 *   The node of the top-level page in the book.
 * @param $form
 *   The form that is being modified, passed by reference.
 *
 * @see book_admin_edit()
 */
function _book_fork_admin_table($node, &$form) {
  $form['table'] = array(
    '#theme' => 'book_admin_table',
    '#tree' => TRUE,
  );

  $tree = book_menu_subtree_data($node->book);
  $tree = array_shift($tree); // Do not include the book item itself.
  if ($tree['below']) {
    $hash = drupal_hash_base64(serialize($tree['below']));
    // Store the hash value as a hidden form element so that we can detect
    // if another user changed the book hierarchy.
    $form['tree_hash'] = array(
      '#type' => 'hidden',
      '#default_value' => $hash,
    );
    $form['tree_current_hash'] = array(
      '#type' => 'value',
      '#value' => $hash,
    );
    $count = _book_fork_admin_table_tree_count($tree['below']);
    $form['tree_count'] = array(
      '#type' => 'value',
      '#value' => $count,
    );
    _book_fork_admin_table_tree($tree['below'], $form['table']);
  }

}

/**
 * Figure out how many leaves there are in this tree.
 */
function _book_fork_admin_table_tree_count($tree) {
  $count = count($tree);
  foreach ($tree as $data) {
    if ($data['below']) {
      $count = $count + _book_fork_admin_table_tree_count();
    }
  }
  return $count;
}

/**
 * Helps build the main table in the book administration page form.
 *
 * @param $tree
 *   A subtree of the book menu hierarchy.
 * @param $form
 *   The form that is being modified, passed by reference.
 *
 * @return
 *   The modified form array.
 *
 * @see book_admin_edit()
 */
function _book_fork_admin_table_tree($tree, &$form, &$added = 0) {
  // The delta must be big enough to give each node a distinct value.
  $count = $form['tree_count']['#value'];
  $delta = ($count < 30) ? 15 : intval($count / 2) + 10;

  foreach ($tree as $data) {
    $form['book-admin-' . $data['link']['nid']] = array(
      '#item' => $data['link'],
      'nid' => array('#type' => 'value', '#value' => $data['link']['nid']),
      'depth' => array('#type' => 'value', '#value' => $data['link']['depth']),
      'href' => array('#type' => 'value', '#value' => $data['link']['href']),
      'title' => array(
        '#type' => 'textfield',
        '#default_value' => $data['link']['link_title'],
        '#maxlength' => 255,
        '#size' => 40,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#default_value' => $data['link']['weight'],
        '#delta' => max($delta, abs($data['link']['weight'])),
        '#title' => t('Weight for @title', array('@title' => $data['link']['title'])),
        '#title_display' => 'invisible',
      ),
      'plid' => array(
        '#type' => 'hidden',
        '#default_value' => $data['link']['plid'],
      ),
      'mlid' => array(
        '#type' => 'hidden',
        '#default_value' => $data['link']['mlid'],
      ),
    );
    if ($data['below']) {
      _book_fork_admin_table_tree($data['below'], $form);
    }
  }

  return $form;
}
