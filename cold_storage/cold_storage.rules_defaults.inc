<?php
/**
 * @file
 * cold_storage.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function cold_storage_default_rules_configuration() {
  $items = array();
  $items['rules_set_characters_active_if_moved_from_cold_storage'] = entity_import('rules_config', '{ "rules_set_characters_active_if_moved_from_cold_storage" : {
      "LABEL" : "Move unlocked players from cold storage",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Cold storage" ],
      "REQUIRES" : [ "views_rules", "rules", "workflow_rules", "rules_block_user" ],
      "ON" : { "user_was_unblocked" : [] },
      "DO" : [
        { "VIEW LOOP" : {
            "VIEW" : "vaxia_character_sheets",
            "DISPLAY" : "rules_chars_by_user",
            "USING" : { "uid" : [ "account:uid" ] },
            "ROW VARIABLES" : { "nid" : { "nid" : "Nid" }, "title_1" : { "character_name" : "Name" } },
            "DO" : [
              { "entity_fetch" : {
                  "USING" : { "type" : "node", "id" : [ "nid" ] },
                  "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
                }
              },
              { "node_publish" : { "node" : [ "entity-fetched" ] } },
              { "workflow_rules_set_state" : {
                  "node" : [ "entity-fetched" ],
                  "workflow_state" : { "value" : { "3" : "3" } },
                  "workflow_comment" : "Returned to submit pending re-approval after player moved from cold storage."
                }
              }
            ]
          }
        },
        { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } },
        { "user_remove_role" : { "account" : [ "account" ], "roles" : { "value" : { "22" : "22" } } } },
        { "entity_save" : { "data" : [ "account" ], "immediate" : 1 } }
      ]
    }
  }');
  $items['rules_set_characters_inactive_if_moved_to_cold_storage'] = entity_import('rules_config', '{ "rules_set_characters_inactive_if_moved_to_cold_storage" : {
      "LABEL" : "Set blocked players to cold storage",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Cold storage" ],
      "REQUIRES" : [ "rules", "views_rules", "workflow_rules", "rules_block_user" ],
      "ON" : { "user_was_blocked" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "22" : "22" } } } },
        { "user_remove_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } },
        { "entity_save" : { "data" : [ "account" ], "immediate" : 1 } },
        { "VIEW LOOP" : {
            "VIEW" : "vaxia_character_sheets",
            "DISPLAY" : "rules_chars_by_user",
            "USING" : { "uid" : [ "account:uid" ] },
            "ROW VARIABLES" : { "nid" : { "nid" : "Nid" }, "title_1" : { "character_name" : "Name" } },
            "DO" : [
              { "entity_fetch" : {
                  "USING" : { "type" : "node", "id" : [ "nid" ] },
                  "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
                }
              },
              { "node_unpublish" : { "node" : [ "entity-fetched" ] } },
              { "workflow_rules_set_state" : {
                  "node" : [ "entity-fetched" ],
                  "workflow_state" : { "value" : { "2" : "2" } },
                  "workflow_comment" : "Set to default after player moved to cold storage."
                }
              }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}
