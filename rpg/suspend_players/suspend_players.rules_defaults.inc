<?php
/**
 * @file
 * suspend_players.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function suspend_players_default_rules_configuration() {
  $items = array();
  $items['rules_unban_a_banned_user'] = entity_import('rules_config', '{ "rules_unban_a_banned_user" : {
      "LABEL" : "Suspend - Unblock a suspended user",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Suspend Players" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "32" : "32" } } } },
        { "user_is_blocked" : { "account" : [ "user" ] } },
        { "NOT data_is_empty" : { "data" : [ "user:field-banned-until" ] } },
        { "data_is" : {
            "data" : [ "user:field-banned-until" ],
            "op" : "\\u003C",
            "value" : "now"
          }
        }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "8" : "8" } } } },
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "32" : "32" } } } },
        { "user_unblock" : { "account" : [ "user" ] } },
        { "data_set" : { "data" : [ "user:field-banned-until" ] } }
      ]
    }
  }');
  $items['strikes_strikes_third_yellow_strike'] = entity_import('rules_config', '{ "strikes_strikes_third_yellow_strike" : {
      "LABEL" : "Strikes - 3rd yellow strike ",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "5",
      "OWNER" : "rules",
      "TAGS" : [ "Strikes" ],
      "REQUIRES" : [ "rules", "views_rules" ],
      "ON" : { "node_insert--strike" : { "bundle" : "strike" } },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "strike" : "strike" } } } },
        { "data_is" : { "data" : [ "node:field-severity" ], "value" : "Yellow" } },
        { "entity_is_of_bundle" : {
            "entity" : [ "node:field-associated-record" ],
            "type" : "node",
            "bundle" : { "value" : { "notation" : "notation" } }
          }
        }
      ],
      "DO" : [
        { "entity_save" : { "data" : [ "node" ], "immediate" : 1 } },
        { "VIEW LOOP" : {
            "VIEW" : "suspensions",
            "DISPLAY" : "check_yellows",
            "USING" : { "field_struck_player_uid" : [ "node:field-struck-player:uid" ] },
            "ROW VARIABLES" : { "yellow_count" : { "yellow_count" : "Yellow count" } },
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "node",
                    "param_type" : "strike",
                    "param_title" : "Red strike - [node:field-struck-player:name]",
                    "param_author" : [ "node:author" ]
                  },
                  "PROVIDE" : { "entity_created" : { "red_strike" : "Red strike" } }
                }
              },
              { "data_set" : {
                  "data" : [ "node:field-associated-record" ],
                  "value" : [ "node:field-associated-record" ]
                }
              },
              { "data_set" : { "data" : [ "red-strike:field-date-of-incident" ], "value" : "now" } },
              { "data_set" : {
                  "data" : [ "red-strike:field-struck-player" ],
                  "value" : [ "node:field-struck-player" ]
                }
              },
              { "data_set" : { "data" : [ "red-strike:field-severity" ], "value" : "Red" } },
              { "data_set" : {
                  "data" : [ "red-strike:body" ],
                  "value" : { "value" : {
                      "value" : "This Red strike was given as the result of a 3rd (or greater) Yellow strike within a year timeframe.",
                      "format" : "wiki"
                    }
                  }
                }
              },
              { "list_add" : {
                  "list" : [ "node:field-associated-record:field-results" ],
                  "item" : "red-upgrade",
                  "unique" : "1"
                }
              },
              { "entity_save" : { "data" : [ "node:field-associated-record" ] } },
              { "entity_save" : { "data" : [ "red-strike" ] } },
              { "drupal_message" : { "message" : "This is the players third Yellow strike within the year. This Yellow has been upgraded to a Red as a result and the player\\u0027s account will be suspended." } }
            ]
          }
        }
      ]
    }
  }');
  $items['strikes_suspend_player'] = entity_import('rules_config', '{ "strikes_suspend_player" : {
      "LABEL" : "Suspend - Red Strike",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "5",
      "OWNER" : "rules",
      "TAGS" : [ "Suspend Players" ],
      "REQUIRES" : [ "rules", "views_rules", "cold_storage" ],
      "ON" : { "node_insert--strike" : { "bundle" : "strike" } },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "strike" : "strike" } } } },
        { "data_is" : { "data" : [ "node:field-severity" ], "value" : "Red" } }
      ],
      "DO" : [
        { "entity_save" : { "data" : [ "node" ], "immediate" : 1 } },
        { "VIEW LOOP" : {
            "VIEW" : "suspensions",
            "DISPLAY" : "check_reds",
            "USING" : { "field_struck_player_uid" : [ "node:field-struck-player:uid" ] },
            "ROW VARIABLES" : {
              "red_count" : { "red_count" : "Red count" },
              "new_ban_months" : { "new_ban_months" : "New ban months" },
              "new_ban" : { "new_ban" : "New Ban" }
            },
            "DO" : [
              { "cold_storage_rules_update_nodes" : {
                  "user" : [ "node:field-struck-player" ],
                  "node_type" : { "value" : { "character_sheet" : "character_sheet" } },
                  "current_state" : { "value" : { "4" : "4" } },
                  "new_state" : { "value" : { "2" : "2" } },
                  "workflow_comment" : "Characters moved to draft after suspension."
                }
              },
              { "data_set" : {
                  "data" : [ "node:field-struck-player:field-banned-until" ],
                  "value" : [ "new-ban" ]
                }
              },
              { "entity_save" : { "data" : [ "node:field-struck-player" ], "immediate" : 1 } },
              { "user_remove_role" : {
                  "account" : [ "node:field-struck-player" ],
                  "roles" : { "value" : {
                      "3" : "3",
                      "30" : "30",
                      "8" : "8",
                      "9" : "9",
                      "21" : "21",
                      "24" : "24",
                      "26" : "26",
                      "27" : "27",
                      "19" : "19",
                      "28" : "28",
                      "29" : "29",
                      "12" : "12",
                      "34" : "34",
                      "35" : "35",
                      "25" : "25",
                      "13" : "13",
                      "10" : "10",
                      "17" : "17",
                      "15" : "15",
                      "22" : "22",
                      "23" : "23",
                      "31" : "31",
                      "32" : "32"
                    }
                  }
                }
              },
              { "user_add_role" : {
                  "account" : [ "node:field-struck-player" ],
                  "roles" : { "value" : { "32" : "32" } }
                }
              },
              { "user_block" : { "account" : [ "node:field-struck-player" ] } },
              { "drupal_message" : { "message" : "This is Red strike #[red-count:value] in the last year. [node:field-struck-player] has been suspended for [new-ban-months:value] months thru [new-ban:value]." } }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}
