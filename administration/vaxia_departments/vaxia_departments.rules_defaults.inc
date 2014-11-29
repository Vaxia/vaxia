<?php
/**
 * @file
 * vaxia_departments.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_departments_default_rules_configuration() {
  $items = array();
  $items['rules_mark_passed_system_walkthrough'] = entity_import('rules_config', '{ "rules_mark_passed_system_walkthrough" : {
      "LABEL" : "Mark passed System Walkthrough",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Walkthrough" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : { "quiz_rules_quiz_passed" : [] },
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "9781" } } ],
      "DO" : [
        { "drupal_message" : { "message" : "Congratulations! You\\u0027re up to date on how our system here works. If you have any questions feel free to ask one of our A\\/SHs for clarification. If there\\u0027s anything in this walkthrough that we didn\\u0027t cover please let us know." } },
        { "data_set" : { "data" : [ "user:field-took-system-walkthrough" ], "value" : "now" } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "24" : "24" } } } }
      ]
    }
  }');
  $items['vaxia_staff_mark_passed_newbie_helper'] = entity_import('rules_config', '{ "vaxia_staff_mark_passed_newbie_helper" : {
      "LABEL" : "Mark passed Newbie Helper",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Newbie Helper" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : { "quiz_rules_quiz_passed" : [] },
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "10564" } } ],
      "DO" : [
        { "drupal_message" : { "message" : "Congratulations! You\\u0027ve passed the newbie helper training - welcome aboard! If you have any questions feel free to ask one of our A\\/SHs for clarification. If there\\u0027s anything in this walkthrough that we didn\\u0027t cover please let us know." } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "28" : "28" } } } }
      ]
    }
  }');
  $items['vaxia_staff_mark_passed_setting_walkthrough'] = entity_import('rules_config', '{ "vaxia_staff_mark_passed_setting_walkthrough" : {
      "LABEL" : "Mark passed Setting Walkthrough",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Walkthrough" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : { "quiz_rules_quiz_passed" : [] },
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "10344" } } ],
      "DO" : [
        { "drupal_message" : { "message" : "Congratulations! You\\u0027re up to date on our settings. If you have any questions feel free to ask one of our A\\/SHs for clarification. If there\\u0027s anything in this walkthrough that we didn\\u0027t cover please let us know." } },
        { "data_set" : { "data" : [ "user:field-took-setting-walkthrough" ], "value" : "now" } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "27" : "27" } } } }
      ]
    }
  }');
  $items['vaxia_staff_mark_passed_social_walkthrough'] = entity_import('rules_config', '{ "vaxia_staff_mark_passed_social_walkthrough" : {
      "LABEL" : "Mark passed Social Walkthrough",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Walkthrough" ],
      "REQUIRES" : [ "rules", "quiz_rules" ],
      "ON" : { "quiz_rules_quiz_passed" : [] },
      "IF" : [ { "data_is" : { "data" : [ "node:nid" ], "value" : "10231" } } ],
      "DO" : [
        { "drupal_message" : { "message" : "Congratulations! You\\u0027re up to date on our social expectations here. If you have any questions feel free to ask one of our A\\/SHs for clarification. If there\\u0027s anything in this walkthrough that we didn\\u0027t cover please let us know." } },
        { "data_set" : { "data" : [ "user:field-took-social-walkthrough" ], "value" : "now" } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "26" : "26" } } } }
      ]
    }
  }');
  return $items;
}
