<?php
/**
 * @file
 * role_app.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function role_app_default_rules_configuration() {
  $items = array();
  $items['rules_role_app_check_webform_submission'] = entity_import('rules_config', '{ "rules_role_app_check_webform_submission" : {
      "LABEL" : "Role App - Check webform submission",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Role App" ],
      "REQUIRES" : [ "rules", "role_app", "webform_rules" ],
      "ON" : { "webform_rules_submit" : [] },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "application" : "application" } }
          }
        },
        { "NOT data_is_empty" : { "data" : [ "node:field-applied-role-id" ] } },
        { "NOT data_is" : { "data" : [ "node:field-applied-role-id" ], "value" : "9" } },
        { "role_app_approved_app" : { "webform" : [ "node" ] } }
      ],
      "DO" : [
        { "role_app_set_role" : { "user" : [ "node:author" ], "rid" : "[node:field-applied-role-id]" } },
        { "webform_close" : { "entity" : [ "node" ], "selected_webform" : { "value" : [] } } }
      ]
    }
  }');
  $items['rules_role_app_create_application'] = entity_import('rules_config', '{ "rules_role_app_create_application" : {
      "LABEL" : "Role App - Create application",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Role App" ],
      "REQUIRES" : [ "role_app", "quiz_rules" ],
      "ON" : { "quiz_rules_quiz_passed" : [] },
      "IF" : [ { "role_app_is_app_quiz" : { "quiz" : [ "node" ] } } ],
      "DO" : [
        { "role_app_create_app" : { "user" : [ "user" ], "quiz" : [ "node" ] } }
      ]
    }
  }');
  return $items;
}
