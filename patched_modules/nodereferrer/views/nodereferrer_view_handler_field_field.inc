<?php
/**
 * @file
 * nodereferrer.module Views integration
 */

/**
 * Views handler to get fields by nodereferrer fields
 */
class nodereferrer_view_handler_field_field extends nodereferrer_view_handler_field {

  function get_options_title() {
    return t('Using field');
  }

  function get_options() {
    return nodereferrer_nodereference_field_options();
  }

  function get_item_list($nid) {
    if (empty($this->options['fields'])) {
      $fields = array_keys(nodereferrer_nodereference_field_options());
    }
    else {
      $fields = array_keys(array_filter($this->options['fields']));
    }

    return $this->referrers($nid, $fields);
  }
}
