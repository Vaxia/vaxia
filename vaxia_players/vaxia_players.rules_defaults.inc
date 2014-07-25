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
      "TAGS" : [ "SH Test" ],
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
  return $items;
}
