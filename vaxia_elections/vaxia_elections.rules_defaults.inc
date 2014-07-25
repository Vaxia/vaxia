<?php
/**
 * @file
 * vaxia_elections.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_elections_default_rules_configuration() {
  $items = array();
  $items['rules_add_starvation_role_to_ash_on_session'] = entity_import('rules_config', '{ "rules_add_starvation_role_to_ash_on_session" : {
      "LABEL" : "Add starvation role to ASH on session",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Starvation mode" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert--session_report" : { "bundle" : "session_report" } },
      "IF" : [
        { "NOT user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "25" : "25" } } } }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "25" : "25" } } } }
      ]
    }
  }');
  return $items;
}
