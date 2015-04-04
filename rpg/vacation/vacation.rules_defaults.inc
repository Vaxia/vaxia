<?php
/**
 * @file
 * vacation.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vacation_default_rules_configuration() {
  $items = array();
  $items['rules_vacation_end'] = entity_import('rules_config', '{ "rules_vacation_end" : {
      "LABEL" : "Vacation - Ending vacation",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Vacation" ],
      "REQUIRES" : [ "rules", "vacation" ],
      "ON" : { "vacation_end" : [] },
      "DO" : [
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "23" : "23" } } } },
        { "redirect" : { "url" : "\\u003Cfront\\u003E" } },
        { "drupal_message" : { "message" : "Welcome back! Take a moment to check in with the rest of the Storyhosts so you know what you missed. If you\\u0027ve been gone for a long time - expect to have to do some catching up." } }
      ]
    }
  }');
  $items['rules_vacation_start'] = entity_import('rules_config', '{ "rules_vacation_start" : {
      "LABEL" : "Vacation - Starting vacation",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Vacation" ],
      "REQUIRES" : [ "rules", "vacation" ],
      "ON" : { "vacation_start" : [] },
      "DO" : [
        { "user_add_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "8" : "8", "23" : "23" } }
          }
        },
        { "redirect" : { "url" : "\\u003Cfront\\u003E" } },
        { "drupal_message" : { "message" : "Your vacation notice has gone out, roles have been removed and you are ready to hit the beach and\\/or deal with what you need to. Good luck, and we\\u0027ll see you again when you\\u0027re ready to return." } }
      ]
    }
  }');
  return $items;
}
