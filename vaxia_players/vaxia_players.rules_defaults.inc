<?php
/**
 * @file
 * vaxia_players.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_players_default_rules_configuration() {
  $items = array();
  $items['rules_add_xp_to_total_user_xp'] = entity_import('rules_config', '{ "rules_add_xp_to_total_user_xp" : {
      "LABEL" : "Add XP to total User XP",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Storyhosts" ],
      "REQUIRES" : [ "rules", "vaxia" ],
      "ON" : { "vaxia_gain_xp" : [] },
      "IF" : [ { "data_is" : { "data" : [ "xp-type" ], "value" : "xp" } } ],
      "DO" : [
        { "data_calc" : {
            "USING" : {
              "input_1" : [ "user:field-xp-over-time" ],
              "op" : "+",
              "input_2" : [ "xp-amount" ]
            },
            "PROVIDE" : { "result" : { "new_total_xp" : "New total XP" } }
          }
        },
        { "data_set" : { "data" : [ "user:field-xp-over-time" ], "value" : [ "new-total-xp" ] } }
      ]
    }
  }');
  $items['rules_filedepot_add_personal_folder'] = entity_import('rules_config', '{ "rules_filedepot_add_personal_folder" : {
      "LABEL" : "Filedepot add personal folder",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Vaxia filesystem" ],
      "REQUIRES" : [ "rules", "filedepot_rules" ],
      "ON" : { "node_insert--character_sheet" : { "bundle" : "character_sheet" } },
      "IF" : [
        { "data_is_empty" : { "data" : [ "node:author:field-personal-folder" ] } }
      ],
      "DO" : [
        { "filedepot_rules_actions_create_folder" : {
            "USING" : {
              "owner" : [ "node:author" ],
              "folder_name" : "[account:name] Personal Files",
              "parent_folder" : { "value" : { "89" : "89" } },
              "description" : "Personal files for [account:name]",
              "inherit_permission" : 0,
              "set_owner" : 1
            },
            "PROVIDE" : { "folder_node" : { "folder_node" : "Created folder" } }
          }
        },
        { "data_set" : {
            "data" : [ "node:author:field-personal-folder" ],
            "value" : [ "folder-node" ]
          }
        }
      ]
    }
  }');
  $items['rules_node_redirect_after_creation'] = entity_import('rules_config', '{ "rules_node_redirect_after_creation" : {
      "LABEL" : "Node redirect after creation",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Vaxia" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [ { "entity_is_new" : { "entity" : [ "node" ] } } ],
      "DO" : [ { "redirect" : { "url" : "node\\/[node:nid]" } } ]
    }
  }');
  $items['rules_retire_inactive_user_account'] = entity_import('rules_config', '{ "rules_retire_inactive_user_account" : {
      "LABEL" : "Retire inactive user account",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Players" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "NOT user_is_blocked" : { "account" : [ "user" ] } },
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "8" : "8" } } } },
        { "data_is" : { "data" : [ "user:created" ], "op" : "\\u003C", "value" : "-1 year" } },
        { "data_is" : { "data" : [ "user:last-access" ], "op" : "\\u003C", "value" : "-1 year" } }
      ],
      "DO" : [
        { "user_remove_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : {
                "19" : "19",
                "13" : "13",
                "10" : "10",
                "17" : "17",
                "15" : "15",
                "9" : "9",
                "20" : "20",
                "14" : "14",
                "21" : "21",
                "4" : "4",
                "3" : "3",
                "11" : "11"
              }
            }
          }
        },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "22" : "22" } } } },
        { "entity_save" : { "data" : [ "user" ], "immediate" : 1 } },
        { "user_block" : { "account" : [ "user" ] } },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "4" : "4", "3" : "3" } },
            "subject" : "[Vaxia] User has been set inactive",
            "message" : "The following user account has been set inactive.\\r\\n\\r\\n[user:name]\\r\\n\\r\\nThe user created the account and a character, but the player has not logged in since. The account has been moved to cold storage."
          }
        }
      ]
    }
  }');
  $items['rules_set_new_account_role_on_new_accounts'] = entity_import('rules_config', '{ "rules_set_new_account_role_on_new_accounts" : {
      "LABEL" : "Set new account role on new accounts",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Players" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "IF" : [
        { "NOT user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "30" : "30", "8" : "8" } }
          }
        }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "account" ], "roles" : { "value" : { "30" : "30" } } } }
      ]
    }
  }');
  $items['rules_user_is_under_minimal_age'] = entity_import('rules_config', '{ "rules_user_is_under_minimal_age" : {
      "LABEL" : "User is under minimal age",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Staff" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "account:field-age-requirement" ], "value" : 0 } }
      ],
      "DO" : [
        { "user_remove_role" : { "account" : [ "account" ], "roles" : { "value" : { "30" : "30" } } } },
        { "user_block" : { "account" : [ "account" ] } },
        { "mail" : {
            "to" : "[account:mail]",
            "subject" : "Account details for [user:name] at [site:name] (NOT approved)",
            "message" : "[user:name],\\r\\n\\r\\nThank you for your interest in [site:name]. Your application for an account has been denied because you are under 13.\\r\\n\\r\\nWe have a minimum age to play on the site. Please feel free to return when you\\u0027re 13, and we\\u0027ll be glad to have you!\\r\\n\\r\\n\\r\\n--  [site:name] team",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
