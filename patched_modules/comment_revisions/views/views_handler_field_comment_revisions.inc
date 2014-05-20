<?php
/**
 * @file
 * Contains the basic comment revision field handler.
 */
class views_handler_field_comment_revision extends views_handler_field_comment {
  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_comment_revision'])) {
      $this->additional_fields['vid'] = 'vid';
      $this->additional_fields['cid'] = 'cid';
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_comment'] = array('default' => FALSE);
    $options['link_to_comment_revision'] = array('default' => TRUE);
    return $options;
  }

  /**
   * Provide link to revision option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_comment_revision'] = array(
      '#title' => t('Link this field to its comment revision'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_comment_revision']),
    );
  }

  /**
   * Render whatever the data is as a link to the comment.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_comment_revision']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = "comment/" . $values->{$this->aliases['cid']} . '/revisions/' . $values->{$this->aliases['vid']} . '/view';
    }
    else {
      return parent::render_link($data, $values);
    }
    return $data;
  }
}
