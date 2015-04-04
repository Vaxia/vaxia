<?php
/**
 * @file
 * personal_rooms.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function personal_rooms_default_rules_configuration() {
  $items = array();
  $items['rules_characters_create_personal_room'] = entity_import('rules_config', '{ "rules_characters_create_personal_room" : {
      "LABEL" : "Personal rooms - create personal room",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Personal rooms" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : { "workflow_state_changed" : [] },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "character_sheet" : "character_sheet" } }
          }
        },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "3" : "3" } },
            "new_state" : { "value" : { "4" : "4" } }
          }
        },
        { "data_is_empty" : { "data" : [ "node:field-personal-room" ] } },
        { "data_is" : { "data" : [ "node:field-is-open-npc" ], "value" : "0" } },
        { "data_is" : { "data" : [ "node:field-is-npc" ], "value" : "0" } },
        { "data_is" : { "data" : [ "node:field-is-storyteller" ], "value" : "0" } }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "rpg_chatroom",
              "param_title" : "[node:title] Personal Room",
              "param_author" : [ "node:author" ]
            },
            "PROVIDE" : { "entity_created" : { "personal_room" : "Personal room" } }
          }
        },
        { "data_set" : {
            "data" : [ "personal-room:field-realm" ],
            "value" : [ "node:field-realm" ]
          }
        },
        { "data_set" : { "data" : [ "personal-room:field-private-room" ], "value" : "0" } },
        { "data_set" : { "data" : [ "personal-room:field-mature-posting" ], "value" : "1" } },
        { "data_set" : { "data" : [ "personal-room:field-is-personal-room" ], "value" : "1" } },
        { "data_set" : {
            "data" : [ "personal-room:field-room-description" ],
            "value" : { "value" : {
                "value" : "A personal room is a room owned by a specific character. It does not show up on searches and posts here will not be automatically archived. Use this room for personal posts, crafting or other RP you want to keep to yourself. Be careful not to lock yourself out of the room if you set the users able to access the room.",
                "format" : "wiki"
              }
            }
          }
        },
        { "entity_save" : { "data" : [ "personal-room" ], "immediate" : "1" } },
        { "data_set" : { "data" : [ "node:field-personal-room" ], "value" : [ "personal-room" ] } }
      ]
    }
  }');
  return $items;
}
