<?php
/**
 * @file
 * artwork.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function artwork_default_rules_configuration() {
  $items = array();
  $items['rules_remove_art_from_game'] = entity_import('rules_config', '{ "rules_remove_art_from_game" : {
      "LABEL" : "Remove art from game",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Artwork" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : { "workflow_state_changed" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "artwork" : "artwork" } } } },
        { "workflow_check_state" : {
            "node" : [ "node" ],
            "workflow_state" : { "value" : { "11" : "11", "17" : "17" } }
          }
        }
      ],
      "DO" : [ { "entity_save" : { "data" : [ "node" ] } } ]
    }
  }');
  return $items;
}
