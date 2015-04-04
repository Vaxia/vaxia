<?php
/**
 * @file
 * strikes.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function strikes_default_rules_configuration() {
  $items = array();
  $items['rules_create_red_strike'] = entity_import('rules_config', '{ "rules_create_red_strike" : {
      "LABEL" : "Strikes - Create red strike",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Strikes" ],
      "REQUIRES" : [ "rules_list_conditions", "rules" ],
      "ON" : { "node_insert--notation" : { "bundle" : "notation" } },
      "IF" : [
        { "ANY" : {
            "USING" : { "list" : [ "node:field-results" ] },
            "ITEM" : { "result" : "Result" },
            "ANY" : [ { "data_is" : { "data" : [ "result" ], "value" : "red" } } ]
          }
        }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-red-strike" ] },
            "ITEM" : { "strike_user" : "Strike user" },
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "node",
                    "param_type" : "strike",
                    "param_title" : "Red Strike - [strike-user:name]",
                    "param_author" : [ "node:author" ]
                  },
                  "PROVIDE" : { "entity_created" : { "strike" : "Strike" } }
                }
              },
              { "data_set" : { "data" : [ "strike:field-severity" ], "value" : "Red" } },
              { "data_set" : { "data" : [ "strike:field-associated-record" ], "value" : [ "node" ] } },
              { "data_set" : {
                  "data" : [ "strike:field-date-of-incident" ],
                  "value" : [ "node:field-date-of-incident" ]
                }
              },
              { "data_set" : { "data" : [ "strike:field-struck-player" ], "value" : [ "strike-user" ] } },
              { "entity_save" : { "data" : [ "strike" ] } },
              { "drupal_message" : { "message" : "A red strike has been issued to [strike-user:name]." } }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_create_yellow_strike'] = entity_import('rules_config', '{ "rules_create_yellow_strike" : {
      "LABEL" : "Strikes - Create yellow strike",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Strikes" ],
      "REQUIRES" : [ "rules_list_conditions", "rules" ],
      "ON" : { "node_insert--notation" : { "bundle" : "notation" } },
      "IF" : [
        { "ANY" : {
            "USING" : { "list" : [ "node:field-results" ] },
            "ITEM" : { "result" : "Result" },
            "ANY" : [ { "data_is" : { "data" : [ "result" ], "value" : "yellow" } } ]
          }
        }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-yellow-strike" ] },
            "ITEM" : { "strike_user" : "Strike user" },
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "node",
                    "param_type" : "strike",
                    "param_title" : "Yellow Strike - [strike-user:name] ",
                    "param_author" : [ "node:author" ]
                  },
                  "PROVIDE" : { "entity_created" : { "strike" : "Strike" } }
                }
              },
              { "data_set" : { "data" : [ "strike:field-severity" ], "value" : "Yellow" } },
              { "data_set" : { "data" : [ "strike:field-associated-record" ], "value" : [ "node" ] } },
              { "data_set" : {
                  "data" : [ "strike:field-date-of-incident" ],
                  "value" : [ "node:field-date-of-incident" ]
                }
              },
              { "data_set" : { "data" : [ "strike:field-struck-player" ], "value" : [ "strike-user" ] } },
              { "entity_save" : { "data" : [ "strike" ], "immediate" : 1 } },
              { "drupal_message" : { "message" : "A yellow strike has been issued to [strike-user:name]." } }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}
