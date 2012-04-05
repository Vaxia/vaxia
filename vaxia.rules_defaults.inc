<?php
/**
 * @file
 * vaxia.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_default_rules_configuration() {
  $items = array();
  $items['rules_notify_art_eval_re_new_art'] = entity_import('rules_config', '{ "rules_notify_art_eval_re_new_art" : {
      "LABEL" : "Notify art eval re: new art",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Vaxia" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : [ "workflow_state_changed" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "artwork" : "artwork" } } } },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "ANY" : "ANY" } },
            "new_state" : { "value" : { "3" : "3" } }
          }
        }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "5" : "5" } },
            "subject" : "[Vaxia] Art ready for evaluation",
            "message" : "Art is ready for evaluation at Vaxia.\\r\\n\\r\\nArt: [node:title]\\r\\nPlayer: [node:author]\\r\\nLink: [node:url]\\r\\n\\r\\nThis is an automated message. Please do not reply to this email. If you no longer wish to receive these notifications, please ask to be removed from the art evaluation team."
          }
        }
      ]
    }
  }');
  $items['rules_notify_char_eval_re_new_character'] = entity_import('rules_config', '{ "rules_notify_char_eval_re_new_character" : {
      "LABEL" : "Notify char eval re: new character",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Vaxia" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : [ "workflow_state_changed" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "character_sheet" : "character_sheet" } }
          }
        },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "ANY" : "ANY" } },
            "new_state" : { "value" : { "8" : "8" } }
          }
        }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "6" : "6" } },
            "subject" : "[Vaxia] Character ready for evaluation",
            "message" : "A character is ready for evaluation at Vaxia.\\r\\n\\r\\nCharacter: [node:title]\\r\\nPlayer: [node:author]\\r\\nLink: [node:url]\\r\\n\\r\\nThis is an automated message. Please do not reply to this email. If you no longer wish to receive these notifications, please ask to be removed from the character evaluation team."
          }
        }
      ]
    }
  }');
  $items['rules_notify_item_eval_re_new_item'] = entity_import('rules_config', '{ "rules_notify_item_eval_re_new_item" : {
      "LABEL" : "Notify item eval re: new item",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Vaxia" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : [ "workflow_state_changed" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "items" : "items" } } } },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "ANY" : "ANY" } },
            "new_state" : { "value" : { "14" : "14" } }
          }
        }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "7" : "7" } },
            "subject" : "[Vaxia] Item ready for evaluation",
            "message" : "An item is ready for evaluation at Vaxia.\\r\\n\\r\\nItem: [node:title]\\r\\nPlayer: [node:author]\\r\\nLink: [node:url]\\r\\n\\r\\nThis is an automated message. Please do not reply to this email. If you no longer wish to receive these notifications, please ask to be removed from the item evaluation team."
          }
        }
      ]
    }
  }');
  $items['rules_notify_player_when_char_eval'] = entity_import('rules_config', '{ "rules_notify_player_when_char_eval" : {
      "LABEL" : "Notify player when char approved",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Vaxia" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : [ "workflow_state_changed" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "character_sheet" : "character_sheet" } }
          }
        },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "8" : "8" } },
            "new_state" : { "value" : { "9" : "9" } }
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "node:author:mail" ],
            "subject" : "[Vaxia] Character ready to play",
            "message" : "Your character [node:title] has been approved for play.\\r\\n\\r\\nThis is an automated message, please do not reply to this email."
          }
        }
      ]
    }
  }');
  return $items;
}
