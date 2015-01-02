<?php

/**
 * @file
 * Commonly reused patterns throughout the Vaxia system.
 * Such as checking for the workflow of a node or the owner or the like.
 */

/**
 * Vaxia, generic workflow helper.
 */
function _vaxia_once_a_day_cron($cron, $time = '01:00:00') {
  watchdog('vaxia_cron', 'Calling ' . $cron . ' at ' . $time);
  // Things to run just once a day - always stored in timestamp format.
  // Get only items waiting for recharge.
  $last_ran = variable_get($cron, strtotime('-10 years'));
  $yesterday = strtotime('-1 day');
  // If it's been at least 24 hrs since last run, run the cron.
  if ($last_ran < $yesterday) {
    // And set the run.
    // Prevent slippage by setting to the same time each date. Default at 1am.
    variable_set($cron, strtotime(date('m/d/Y ' . $time)));
    return TRUE;
  }
  return FALSE;
}

/**
 * Vaxia, generic workflow helper.
 */
function _vaxia_get_workflow_options() {
  $workflow_options = array();
  $workflows = workflow_get_workflows();
  foreach ($workflows as $workflow) {
    $states = workflow_get_workflow_states_by_wid($workflow->wid);
    foreach ($states as $state) {
      $workflow_options[$state->sid] = $workflow->name . ': ' . $state->state;
    }
  }
  return $workflow_options;
}

/**
 * Vaxia, generic field helper.
 */
function _vaxia_get_field_value($node, $field, $default = '', $stored = 'value') {
  // Return no-pass if wrong workflow.
  if (empty($node->nid) || empty($field)) {
    return $default;
  }
  static $node_lang = array();
  if (empty($node_lang[$node->nid])) {
    $node_lang[$node->nid] = isset($node->language) ? $node->language : 'und';
  }
  $lang = $node_lang[$node->nid];
  return isset($node->{$field}[$lang][0][$stored]) ? $node->{$field}[$lang][0][$stored] : $default;
}

/**
 * Vaxia, generic field helper.
 */
function _vaxia_get_field_node($node, $field, $default = '') {
  $nid = _vaxia_get_field_value($node, $field, $default, 'nid');
  if (!empty($nid)) {
    return node_load($nid);
  }
  return FALSE;
}

/**
 * Vaxia, generic field helper.
 */
function _vaxia_set_field_value($node, $field, $value = '', $stored = 'value') {
  // Return no-pass if wrong workflow.
  if (empty($node->nid) || empty($field)) {
    return;
  }
  static $node_lang = array();
  if (empty($node_lang[$node->nid])) {
    $node_lang[$node->nid] = isset($node->language) ? $node->language : 'und';
  }
  $lang = $node_lang[$node->nid];
  // Set the value.
  $node->{$field}[$lang][0][$stored] = $value;
}

/**
 * Vaxia, generic workflow helper.
 */
function _vaxia_get_in_workflows($types = array(), $workflows = array()) {
  // Set defaults if not given.
  if (empty($types)) {
    $types = 'character_sheet';
  }
  if (empty($workflows)) {
    $workflows = 'approved';
  }
  // Make sure values are arrays before working with them.
  $types = (array) $types;
  $workflows = (array) $workflows;
  // Set values.
  $args = array(
    ':state_names' => $workflows,
    ':state_values' => $workflows,
    ':types' => $types,
    );
  // Get all nids of type in workflows.
  $found_nodes = db_query('SELECT n.nid, n.vid, n.type, n.title, n.uid, n.status, n.created, n.changed, ' .
    'ws.sid as workflow, ws.state as workflow_state_name' .
    'FROM workflow_node wn ' .
    'LEFT JOIN node n ON n.nid = wn.nid ' .
    'LEFT JOIN workflow_states ws ON ws.sid = wn.sid ' .
    'WHERE ' .
    '(ws.state IN (:state_names) OR ws.sid IN (:state_values)) ' . // In state.
    'AND n.type IN (:types) ' . // In the right type.
    'AND n.status = 1 ', // Published.
    $args);
  // Got results? Good. Return as array.
  $results = array();
  foreach($found_nodes as $found_node) {
    $results[$found_node->nid] = $found_node;
  }
  return $results;
}

/**
 * Vaxia, generic workflow helper.
 */
function _vaxia_check_workflow_is($node, $workflows = array()) {
  // Make sure the workflows given are an array even if string.
  $workflows = (array) $workflows;
  // Return no-pass if wrong workflow.
  if (empty($node->nid) || empty($workflows)) {
    return FALSE;
  }
  // Make sure to load the item if we lack a workflow.
  if (empty($node->workflow)) {
    $node = node_load($node->nid);
  }
  // And if the workflow is a number check for value.
  if (in_array($node->workflow, $workflows)) {
    return TRUE;
  }
  // If a string use state name instead.
  if (in_array($node->workflow_state_name, $workflows)) {
    return TRUE;
  }
  // Default is to return false.
  return FALSE;
}

/**
 * Vaxia, generic workflow helper.
 */
function _vaxia_check_workflow_in($node, $workflow = '') {
  return _vaxia_check_workflow_is($node, $workflow);
}

/**
 * Check if the room is IC or OOC.
 */
function _vaxia_is_ic_room($node) {
  if (!isset($node->nid)) {
    return FALSE;
  }
  if (!isset($node->character_sheet_system) || !isset($node->workflow)) {
    $node = node_load($node->nid);
  }
  $ooc_rooms = variable_get('vaxia_limbo_room', array());
  if (in_array($node->nid, $ooc_rooms)) {
    return TRUE;
  }
  return FALSE;
}