<?php
/**
 * @file
 * vaxia_staff.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_staff_default_rules_configuration() {
  $items = array();
  $items['rules_mark_passed_system_walkthrough'] = entity_import('rules_config', '{ "rules_mark_passed_system_walkthrough" : {
      "LABEL" : "Mark passed System Walkthrough",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Staff" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : { "quiz_rules_quiz_passed" : [] },
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "9781" } } ],
      "DO" : [
        { "drupal_message" : { "message" : "Congratulations! You\\u0027re up to date on how our system here works. If you have any questions feel free to ask one of our A\\/SHs for clarification. If there\\u0027s anything in this walkthrough that we didn\\u0027t cover please let us know." } },
        { "data_set" : {
            "data" : [ "user:field-took-system-walkthrough" ],
            "value" : [ "site:current-date" ]
          }
        },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "24" : "24" } } } }
      ]
    }
  }');
  return $items;
}