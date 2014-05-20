<?php
/**
 * @file
 * Field handler to present a link to delete a comment to a revision.
 */
class views_handler_field_comment_revisions_link_delete extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['cid'] = 'cid';
    $this->additional_fields['vid'] = 'vid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $value = $this->get_value($values, 'cid');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  function access() {
    // TODO Handle field access here! We need to access comment object somehow.
    return TRUE;
  }

  function render_link($data, $values) {
    $comment = comment_load($values->comment_revisions_cid);
    $perm = array('delete comment revisions', 'delete own comments revisions');
    // Check if user has access to delete revisions.
    if (!_comment_revisions_access($comment, $perm)) {
      return;
    }
    // Current version of comment also can't be reverted.
    if ($comment->vid == $values->vid) {
      $this->options['alter']['make_link'] = FALSE;
      return t('current version');
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "comment/" . $values->{$this->aliases['cid']} . "/revisions/" . $values->{$this->aliases['vid']} . "/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    return $text;
  }
}

