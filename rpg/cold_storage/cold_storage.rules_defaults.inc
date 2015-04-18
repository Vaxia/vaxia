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
  $items['rules_returning_player_request_access'] = entity_import('rules_config', '{ "rules_returning_player_request_access" : {
      "LABEL" : "Cold Storage - Returning player request access",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Cold Storage" ],
      "REQUIRES" : [ "rules", "php", "views_rules", "webform_rules" ],
      "ON" : { "webform_rules_submit" : [] },
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "11863" } } ],
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "text", "value" : "[data:account_email-value]" },
            "PROVIDE" : { "variable_added" : { "account_email" : "Account email" } }
          }
        },
        { "data_set" : {
            "data" : [ "account-email" ],
            "value" : "\\u003C?php echo trim($account_email); ?\\u003E"
          }
        },
        { "drupal_message" : { "message" : "Looking up accounts for the email address \\u0022[data:account_email-value]\\u0022 ...." } },
        { "VIEW LOOP" : {
            "VIEW" : "cold_storage",
            "DISPLAY" : "cold_storage_retrieve",
            "USING" : { "mail" : [ "account-email" ] },
            "ROW VARIABLES" : { "uid" : { "uid" : "Uid" } },
            "DO" : [
              { "entity_query" : {
                  "USING" : {
                    "type" : "user",
                    "property" : "uid",
                    "value" : [ "uid" ],
                    "limit" : "1"
                  },
                  "PROVIDE" : { "entity_fetched" : { "found_account" : "Found Account" } }
                }
              },
              { "LOOP" : {
                  "USING" : { "list" : [ "found-account" ] },
                  "ITEM" : { "account" : "Account" },
                  "DO" : [
                    { "drupal_message" : { "message" : "Found account \\u0022[account:name]\\u0022 ... The account has been re-activated and a welcome email sent to the last email address associated with the account. Please check your spam and junk folders if you don\\u0027t see that email within the next 30 minutes! If you no longer have access to the old email address or can\\u0027t remember what the address was - please contact us directly." } },
                    { "user_unblock" : { "account" : [ "account" ] } }
                  ]
                }
              }
            ]
          }
        },
        { "drupal_message" : { "message" : "Done searching. If your account was not found please make sure you\\u0027re using the right email address for it or contact us and we\\u0027ll go hunting for it." } }
      ]
    }
  }');
  $items['rules_set_characters_active_if_moved_from_cold_storage'] = entity_import('rules_config', '{ "rules_set_characters_active_if_moved_from_cold_storage" : {
      "LABEL" : "Cold storage - Move unlocked players from cold storage",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Cold storage" ],
      "REQUIRES" : [ "rules", "cold_storage", "rules_block_user" ],
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
        { "cold_storage_rules_pub_nodes" : {
            "user" : [ "account" ],
            "node_type" : { "value" : { "character_sheet" : "character_sheet" } },
            "current_state" : { "value" : { "_NA" : "_NA" } },
            "workflow_state" : { "value" : { "_NA" : "_NA" } }
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
      "REQUIRES" : [ "rules", "cold_storage", "rules_block_user" ],
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
        { "cold_storage_rules_unpub_nodes" : {
            "user" : [ "account" ],
            "node_type" : { "value" : { "character_sheet" : "character_sheet" } },
            "current_state" : { "value" : { "4" : "4" } },
            "workflow_state" : { "value" : { "2" : "2" } },
            "workflow_comment" : "Set to draft on player moving to storage."
          }
        }
      ]
    }
  }');
  return $items;
}
