<?php
/**
 * @file
 * vaxia_accounts.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_accounts_default_rules_configuration() {
  $items = array();
  $items['rules_rem_eval_role_if_absentee_6mo_cloned_'] = entity_import('rules_config', '{ "rules_rem_eval_role_if_absentee_6mo_cloned_" : {
      "LABEL" : "Rem Eval role if Absentee 3mo",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : [ "user_stats_day_older" ],
      "IF" : [
        { "user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "7" : "7", "5" : "5" } },
            "operation" : "OR"
          }
        },
        { "data_is" : {
            "data" : [ "user:last-login" ],
            "op" : "\\u003C",
            "value" : { "select" : "site:current-date", "date_offset" : { "value" : -7776000 } }
          }
        }
      ],
      "DO" : [
        { "user_remove_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "7" : "7", "5" : "5" } }
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "7" : "7", "5" : "5", "4" : "4" } },
            "subject" : "[Vaxia] Permissions have been removed on an absentee account",
            "message" : "A user has failed to login to the site for 6 months. They had extra powers and have had those abilities removed from the account. This is not to prevent the user from regaining their rank on their return, but rather to keep an active progression of leadership on the site. Please review the account and determine if a replacement is needed to fill this spot.\\r\\n\\r\\nAccount: [user:name]"
          }
        }
      ]
    }
  }');
  $items['rules_rem_sh_role_if_absentee_6mo'] = entity_import('rules_config', '{ "rules_rem_sh_role_if_absentee_6mo" : {
      "LABEL" : "Rem SH role if Absentee 6mo",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : [ "user_stats_day_older" ],
      "IF" : [
        { "user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "9" : "9", "10" : "10" } },
            "operation" : "OR"
          }
        },
        { "data_is" : {
            "data" : [ "user:last-login" ],
            "op" : "\\u003C",
            "value" : {
              "select" : "site:current-date",
              "date_offset" : { "value" : -15552000 }
            }
          }
        }
      ],
      "DO" : [
        { "user_remove_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "9" : "9", "10" : "10" } }
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "4" : "4" } },
            "subject" : "[Vaxia] Permissions have been removed on an absentee account",
            "message" : "A user has failed to login to the site for 6 months. They had extra powers and have had those abilities removed from the account. This is not to prevent the user from regaining their rank on their return, but rather to keep an active progression of leadership on the site. Please review the account and determine if a replacement is needed to fill this spot.\\r\\n\\r\\nAccount: [user:name]"
          }
        }
      ]
    }
  }');
  $items['rules_retire_blocked_user_characters'] = entity_import('rules_config', '{ "rules_retire_blocked_user_characters" : {
      "LABEL" : "Retire blocked user characters",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Retire" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_update" ],
      "IF" : [ { "user_is_blocked" : { "account" : [ "account" ] } } ],
      "DO" : [
        { "entity_query" : {
            "USING" : { "type" : "node", "property" : "author", "value" : [ "account" ] },
            "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "entity-fetched" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [ { "node_unpublish" : { "node" : [ "list-item" ] } } ]
          }
        }
      ]
    }
  }');
  $items['rules_retire_inactive_user_accounr'] = entity_import('rules_config', '{ "rules_retire_inactive_user_accounr" : {
      "LABEL" : "Retire inactive user account",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Retire" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : [ "user_stats_day_older" ],
      "IF" : [
        { "data_is" : { "data" : [ "user:status" ], "value" : "1" } },
        { "data_is" : {
            "data" : [ "user:created" ],
            "op" : "\\u003C",
            "value" : { "select" : "site:current-date", "date_offset" : { "value" : -259200 } }
          }
        },
        { "data_is" : {
            "data" : [ "user:last-login" ],
            "op" : "\\u003C",
            "value" : {
              "select" : "site:current-date",
              "date_offset" : { "value" : -31104000 }
            }
          }
        }
      ],
      "DO" : [ { "data_set" : { "data" : [ "user:status" ], "value" : "0" } } ]
    }
  }');
  return $items;
}
