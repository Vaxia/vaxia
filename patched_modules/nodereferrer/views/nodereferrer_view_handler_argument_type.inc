<?php
/**
 * @file
 * nodereferrer.module Views integration
 */

/**
 * Views handler to filter nodes by nodereferer fields
 */
class nodereferrer_view_handler_argument_type extends nodereferrer_view_handler_argument {

  function get_options_title() {
    return t('From node type');
  }

  function get_options() {
    return node_get_types('names');
  }

  function get_node_list() {
    $fields = array_keys(nodereferrer_nodereference_field_options());
    $nid = (int)$this->argument;
    $node_type = array_filter($this->options['fields']);
    if (empty($node_type)) {
      $node_type = array_keys(node_get_types('names'));
    }

    return $this->referrers($nid, $fields, $node_type);
  }
}
