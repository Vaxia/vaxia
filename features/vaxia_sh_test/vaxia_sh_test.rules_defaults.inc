<?php
/**
 * @file
 * vaxia_sh_test.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_sh_test_default_rules_configuration() {
  $items = array();
  $items['rules_sh_test_1_response'] = entity_import('rules_config', '{ "rules_sh_test_1_response" : {
      "LABEL" : "SH Test 1 Response",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "SH Test" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : [ "quiz_rules_quiz_passed" ],
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "1" } } ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9" } },
            "subject" : "[Vaxia] SH Test 1 has been completed",
            "message" : "A user ( USER ) has passed the SH 1 test. They cannot run sessions until they pass test 2."
          }
        }
      ]
    }
  }');
  $items['rules_sh_test_2_response'] = entity_import('rules_config', '{ "rules_sh_test_2_response" : {
      "LABEL" : "SH Test 2 Response",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "SH Test" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : [ "quiz_rules_quiz_passed" ],
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "1" } } ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9" } },
            "subject" : "[Vaxia] SH Test 2 has been completed",
            "message" : "A user ( USER ) has passed the SH 2 test. They may now run sessions until they pass test 3."
          }
        },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "10" : "10" } } } }
      ]
    }
  }');
  $items['rules_sh_test_3_response_cloned_'] = entity_import('rules_config', '{ "rules_sh_test_3_response_cloned_" : {
      "LABEL" : "SH Test 3 Response",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "SH Test" ],
      "REQUIRES" : [ "rules", "webform_rules", "quiz_rules" ],
      "ON" : [ "quiz_rules_quiz_passed" ],
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "1" } } ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9" } },
            "subject" : "[Vaxia] SH Test 2 has been completed",
            "message" : "A user ( USER ) has passed the SH 2 test. They may now run sessions until they pass test 3."
          }
        },
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "webform",
              "param_title" : "SH Application",
              "param_author" : [ "user" ]
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Created entity" } }
          }
        },
        { "webform_open" : { "entity" : [ "entity-created" ], "selected_webform" : { "value" : [] } } },
        { "entity_save" : { "data" : [ "entity-created" ] } }
      ]
    }
  }');
  $items['rules_sh_test_4_response'] = entity_import('rules_config', '{ "rules_sh_test_4_response" : {
      "LABEL" : "SH Test 4 Response",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "SH Test" ],
      "REQUIRES" : [ "php", "rules", "webform_rules" ],
      "ON" : [ "webform_rules_update" ],
      "IF" : [ { "php_eval" : { "code" : "return 1;" } } ],
      "DO" : []
    }
  }');
  return $items;
}
