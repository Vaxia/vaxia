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
      "LABEL" : "Cold storage - Move unlocked players from cold storage",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Cold storage" ],
      "REQUIRES" : [ "rules", "views_rules", "workflow_rules", "rules_block_user" ],
      "ON" : { "user_was_unblocked" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "22" : "22" } } } },
        { "NOT user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "31" : "31", "32" : "32" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } },
        { "user_remove_role" : { "account" : [ "account" ], "roles" : { "value" : { "22" : "22" } } } },
        { "entity_save" : { "data" : [ "account" ], "immediate" : 1 } },
        { "VIEW LOOP" : {
            "VIEW" : "vaxia_character_sheets",
            "DISPLAY" : "rules_chars_by_user",
            "USING" : { "uid" : [ "account:uid" ] },
            "ROW VARIABLES" : { "nid" : { "nid" : "Nid" } },
            "DO" : [
              { "entity_fetch" : {
                  "USING" : { "type" : "node", "id" : [ "nid" ] },
                  "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
                }
              },
              { "node_publish" : { "node" : [ "entity-fetched" ] } },
              { "workflow_rules_set_state" : {
                  "node" : [ "entity-fetched" ],
                  "workflow_state" : { "value" : { "2" : "2" } },
                  "workflow_comment" : "Characters from cold storage set to draft."
                }
              },
              { "entity_save" : { "data" : [ "entity-fetched" ] } }
            ]
          }
        },
        { "VIEW LOOP" : {
            "VIEW" : "vaxia_character_sheets",
            "DISPLAY" : "views_rules_1",
            "USING" : { "uid" : [ "account:uid" ] },
            "ROW VARIABLES" : { "nid" : { "nid" : "Nid" } },
            "DO" : [
              { "entity_fetch" : {
                  "USING" : { "type" : "node", "id" : [ "nid" ] },
                  "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
                }
              },
              { "workflow_rules_set_state" : {
                  "node" : [ "entity-fetched" ],
                  "workflow_state" : { "value" : { "3" : "3" } },
                  "workflow_comment" : "Re-submission of previously approved character by system. This is so that current eval may review characters from previous versions of the system to bring them up to date."
                }
              },
              { "entity_save" : { "data" : [ "entity-fetched" ] } }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_set_characters_inactive_if_moved_to_cold_storage'] = entity_import('rules_config', '{ "rules_set_characters_inactive_if_moved_to_cold_storage" : {
      "LABEL" : "Cold storage - Set blocked players to cold storage",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Cold storage" ],
      "REQUIRES" : [ "rules", "views_rules", "workflow_rules", "rules_block_user" ],
      "ON" : { "user_was_blocked" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "8" : "8" } } } },
        { "NOT user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "31" : "31", "32" : "32" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "22" : "22" } } } },
        { "user_remove_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : {
                "8" : "8",
                "24" : "24",
                "26" : "26",
                "27" : "27",
                "19" : "19",
                "28" : "28",
                "29" : "29",
                "12" : "12",
                "25" : "25",
                "13" : "13",
                "10" : "10",
                "17" : "17",
                "15" : "15",
                "9" : "9",
                "20" : "20",
                "14" : "14",
                "21" : "21"
              }
            }
          }
        },
        { "entity_save" : { "data" : [ "account" ], "immediate" : 1 } },
        { "VIEW LOOP" : {
            "VIEW" : "vaxia_character_sheets",
            "DISPLAY" : "rules_chars_by_user",
            "USING" : { "uid" : [ "account:uid" ] },
            "ROW VARIABLES" : { "nid" : { "nid" : "Nid" } },
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
                  "workflow_comment" : "Set to draft after player moved to cold storage."
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
