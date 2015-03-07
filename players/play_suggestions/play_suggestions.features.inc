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
      'title_label' => t('Event'),
      'help' => t('<b>An event is a small prompt to give suggestions for an evening of casual role-play.</b>Please keep them small enough that they don\'t automatically mean you need to call for an SH. For example: A broken coffee maker, drinks half off at the inn, or other random happenings that would be good for casual (no A/SH needed) interactions. Things that require an A/SH such as a bar-fight don\'t really work for this. Some events may be in a grey-area where it could call for an A/SH or it couldn\'t - if most of the time it would play out where it wouldn\'t need an A/SH then the suggestion should be ok.
</br></br>
<b>The suggestion will be displayed in the format: "In Realm, Event at Location."</b> We recommend using the formation "there is ... " in order to fit best with this format. The location will be randomly selected when the button is hit from a short list of the most recently played areas. Examples: "there is a broken coffee machine" "there is a poker game" "there is live music". 
</br></br>
<b>Your suggestion may not be perfect for all locations. That\'s OK!</b> For example a "sale on tuna" in a landlocked location, no one said it was very  <i>good</i> tuna. Or live-music in a quiet space-lab area could mean the janitor is whistling off-tune while he works. Think of it like tossing a challenge out at the other players to figure out a way to \'make it work\' for the random location. Sometimes it doesn\'t work - and that\'s ok - you can always hit the button again.'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
