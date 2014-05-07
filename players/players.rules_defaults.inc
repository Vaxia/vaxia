<?php
/**
 * @file
 * players.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function players_default_rules_configuration() {
  $items = array();
  $items['rules_add_character_and_become_a_player'] = entity_import('rules_config', '{ "rules_add_character_and_become_a_player" : {
      "LABEL" : "Add character and become a player",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Players" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert--character_sheet" : { "bundle" : "character_sheet" } },
      "IF" : [
        { "NOT user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "8" : "8" } } } }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "8" : "8" } } } }
      ]
    }
  }');
  return $items;
}
