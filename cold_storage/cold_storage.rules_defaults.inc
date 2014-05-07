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
      "LABEL" : "Set characters active if moved from cold storage",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Cold storage" ],
      "REQUIRES" : [ "rules", "views_bulk_operations", "workflow_rules", "rules_block_user" ],
      "ON" : { "user_was_unblocked" : [] },
      "IF" : [
        { "NOT user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "22" : "22" } } } }
      ],
      "DO" : [
        { "views_bulk_operations_action_load_list" : {
            "USING" : {
              "view" : "character_sheet_moderation|character_sheets_mine",
              "args" : "[account:uid]"
            },
            "PROVIDE" : { "entity_list" : { "entity_list" : "A list of entities" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "entity-list" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "node_publish" : { "node" : [ "list-item" ] } },
              { "workflow_rules_set_state" : {
                  "node" : [ "list-item" ],
                  "workflow_state" : { "value" : { "3" : "3" } },
                  "workflow_comment" : "Resubmitted for approval after player moved from cold storage."
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_set_characters_inactive_if_moved_to_cold_storage'] = entity_import('rules_config', '{ "rules_set_characters_inactive_if_moved_to_cold_storage" : {
      "LABEL" : "Set characters inactive if moved to cold storage",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Cold storage" ],
      "REQUIRES" : [ "rules", "views_bulk_operations", "workflow_rules", "rules_block_user" ],
      "ON" : { "user_was_blocked" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "22" : "22" } } } }
      ],
      "DO" : [
        { "views_bulk_operations_action_load_list" : {
            "USING" : {
              "view" : "character_sheet_moderation|character_sheets_mine",
              "args" : "[account:uid]"
            },
            "PROVIDE" : { "entity_list" : { "entity_list" : "A list of entities" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "entity-list" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "node_unpublish" : { "node" : [ "list-item" ] } },
              { "workflow_rules_set_state" : {
                  "node" : [ "list-item" ],
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
