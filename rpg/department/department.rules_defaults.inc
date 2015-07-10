<?php
/**
 * @file
 * department.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function department_default_rules_configuration() {
  $items = array();
  $items['rules_demote_temporary_lead'] = entity_import('rules_config', '{ "rules_demote_temporary_lead" : {
      "LABEL" : "Department - Demote temporary lead",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Department" ],
      "REQUIRES" : [ "rules", "department" ],
      "ON" : { "department_unpromote_lead" : [] },
      "DO" : [
        { "user_remove_role" : { "account" : [ "temp-lead" ], "roles" : { "value" : { "21" : "21" } } } }
      ]
    }
  }');
  $items['rules_promote_temporary_lead'] = entity_import('rules_config', '{ "rules_promote_temporary_lead" : {
      "LABEL" : "Department - Promote temporary lead",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Department" ],
      "REQUIRES" : [ "rules", "department" ],
      "ON" : { "department_promote_lead" : [] },
      "DO" : [
        { "user_add_role" : { "account" : [ "temp-lead" ], "roles" : { "value" : { "21" : "21" } } } }
      ]
    }
  }');
  return $items;
}
