<?php
/**
 * @file
 * vaxia_accounts.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vaxia_accounts_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function vaxia_accounts_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function vaxia_accounts_node_info() {
  $items = array(
    'proposal' => array(
      'name' => t('Proposal'),
      'base' => 'node_content',
      'description' => t('A proposal is used by any voting member of the site to bring a new proposal to the site for a vote.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'strike' => array(
      'name' => t('Strike'),
      'base' => 'node_content',
      'description' => t('A strike is a disciplinary action taken against a player for a documented on-site offense.'),
      'has_title' => '1',
      'title_label' => t('Offense'),
      'help' => t('STOP. If you are building a strike against a player on this site, make very very sure what you are doing qualifies under a strike condition. Abuse of strikes is a strike itself.'),
    ),
  );
  return $items;
}
