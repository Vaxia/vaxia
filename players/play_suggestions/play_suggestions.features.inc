<?php
/**
 * @file
 * play_suggestions.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function play_suggestions_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function play_suggestions_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function play_suggestions_node_info() {
  $items = array(
    'play_suggestions' => array(
      'name' => t('Play suggestions'),
      'base' => 'node_content',
      'description' => t('An event is a small prompt to give suggestions for an evening of casual role-play.'),
      'has_title' => '1',
      'title_label' => t('There is ...'),
      'help' => t('<b>An event is a small prompt to give suggestions for an evening of casual role-play.</b>Please keep them small enough that they don\'t automatically mean you need to call for an SH (no combats or dramatic events). Your suggestion may not be perfect for all locations. That\'s OK!
<br><br>
Think of it like tossing a challenge out at the other players to figure out a way to \'make it work\' for the random location. If it doesn\'t, then just hit the button again.
<br><br>
<b>Examples: </b> 
<ul>
<li><b>In</b> "<i>Sirian</i>" <b>There is...</b> "<i>a broken coffee machine.</i>"</li>
<li><b>In</b> "<i>Vaxia</i>" <b>There is...</b> "<i>a confused gnome looking for directions.</i>"</li>
</ul>'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
