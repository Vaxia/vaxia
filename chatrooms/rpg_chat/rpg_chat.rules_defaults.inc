<?php
/**
 * @file
 * rpg_chat.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rpg_chat_default_rules_configuration() {
  $items = array();
  $items['rules_locked_room_redirect'] = entity_import('rules_config', '{ "rules_locked_room_redirect" : {
      "LABEL" : "Locked Room Redirect",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Chat Rooms" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_view--rpg_chatroom" : { "bundle" : "rpg_chatroom" } },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "rpg_chatroom" : "rpg_chatroom" } }
          }
        },
        { "data_is" : { "data" : [ "node:field-locked-to-users" ], "value" : 1 } },
        { "NOT data_is" : {
            "data" : [ "site:current-user" ],
            "op" : "IN",
            "value" : [ "node:field-users" ]
          }
        }
      ],
      "DO" : [
        { "redirect" : { "url" : "http:\\/\\/vaxia.org" } },
        { "drupal_message" : {
            "message" : "The room you have tried to access is locked to specific players. You will not be able to access it.",
            "type" : "warning"
          }
        }
      ]
    }
  }');
  $items['rules_mature_room_redirect'] = entity_import('rules_config', '{ "rules_mature_room_redirect" : {
      "LABEL" : "Mature Room Redirect",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Chat Rooms" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_view--rpg_chatroom" : { "bundle" : "rpg_chatroom" } },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "rpg_chatroom" : "rpg_chatroom" } }
          }
        },
        { "data_is" : { "data" : [ "site:current-user:field-mature" ], "value" : 0 } },
        { "data_is" : { "data" : [ "node:field-mature-posting" ], "value" : 1 } }
      ],
      "DO" : [
        { "redirect" : { "url" : "http:\\/\\/vaxia.org" } },
        { "drupal_message" : {
            "message" : "The room you have tried to access is limited to players 18 yrs and older.",
            "type" : "warning"
          }
        }
      ]
    }
  }');
  return $items;
}
