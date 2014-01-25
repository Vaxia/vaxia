<?php
/**
 * @file
 * proposal.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function proposal_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function proposal_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function proposal_node_info() {
  $items = array(
    'proposal' => array(
      'name' => t('Proposal'),
      'base' => 'node_content',
      'description' => t('A proposal is used by any voting member of the site to bring a new proposal to the site for a vote.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
