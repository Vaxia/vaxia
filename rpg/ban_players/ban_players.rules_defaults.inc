<?php
/**
 * @file
 * ban_players.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function ban_players_default_rules_configuration() {
  $items = array();
  $items['rules_add_ip_for_banned_user'] = entity_import('rules_config', '{ "rules_add_ip_for_banned_user" : {
      "LABEL" : "Bans - Add IP for banned user",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Bans" ],
      "REQUIRES" : [ "rules", "php" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "31" : "31" } }
          }
        }
      ],
      "DO" : [
        { "list_add" : {
            "list" : [ "site:current-user:field-banned-ip" ],
            "item" : { "select" : "site:name", "php" : { "code" : "return ip_address();" } },
            "unique" : 1
          }
        },
        { "block_ip" : { "ip_address" : [ "" ] } },
        { "redirect" : { "url" : "user\\/logout" } }
      ]
    }
  }');
  $items['rules_set_player_last_ip'] = entity_import('rules_config', '{ "rules_set_player_last_ip" : {
      "LABEL" : "Bans - Set player last IP",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Bans" ],
      "REQUIRES" : [ "rules", "php", "user_stats" ],
      "ON" : { "user_stats_ip_address_insert" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "30" : "30", "8" : "8" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "text", "value" : "\\u003C?php echo $statistic_value; ?\\u003E" },
            "PROVIDE" : { "variable_added" : { "new_ip" : "New IP" } }
          }
        },
        { "data_set" : { "data" : [ "user:field-last-ip" ], "value" : [ "new-ip" ] } },
        { "data_set" : {
            "data" : [ "user:field-latitude" ],
            "value" : "\\u003C?php\\r\\n$loc_data = smart_ip_get_location([statistic-value:value]);\\r\\necho !empty($loc_data[\\u0027latitude\\u0027]) ? $loc_data[\\u0027latitude\\u0027] : 0; \\r\\n?\\u003E"
          }
        },
        { "data_set" : {
            "data" : [ "user:field-longitude" ],
            "value" : "\\u003C?php\\r\\n$loc_data = smart_ip_get_location([statistic-value:value]);\\r\\necho !empty($loc_data[\\u0027longitude\\u0027]) ? $loc_data[\\u0027longitude\\u0027] : 0; \\r\\n?\\u003E"
          }
        }
      ]
    }
  }');
  $items['rules_set_player_last_ip_on_login_access'] = entity_import('rules_config', '{ "rules_set_player_last_ip_on_login_access" : {
      "LABEL" : "Set player last IP - on login\\/access",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Players" ],
      "REQUIRES" : [ "rules", "php" ],
      "ON" : { "user_login" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account" ],
            "roles" : { "value" : { "30" : "30", "8" : "8" } },
            "operation" : "OR"
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "account:field-last-ip" ],
            "value" : "\\u003C?php\\r\\necho ip_address();\\r\\n?\\u003E"
          }
        },
        { "data_set" : {
            "data" : [ "account:field-latitude" ],
            "value" : "\\u003C?php\\r\\n$loc_data = smart_ip_get_location( ip_address() );\\r\\necho !empty($loc_data[\\u0027latitude\\u0027]) ? $loc_data[\\u0027latitude\\u0027] : 0; \\r\\n?\\u003E"
          }
        },
        { "data_set" : {
            "data" : [ "account:field-longitude" ],
            "value" : "\\u003C?php\\r\\n$loc_data = smart_ip_get_location( ip_address() );\\r\\necho !empty($loc_data[\\u0027longitude\\u0027]) ? $loc_data[\\u0027longitude\\u0027] : 0; \\r\\n?\\u003E"
          }
        }
      ]
    }
  }');
  return $items;
}
