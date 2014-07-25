<?php
/**
 * @file
 * storyhost_duties.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function storyhost_duties_default_rules_configuration() {
  $items = array();
  $items['rules_update_expire_on_eval'] = entity_import('rules_config', '{ "rules_update_expire_on_eval" : {
      "LABEL" : "SH Duties - Update power expire on eval char",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Duties" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : { "workflow_state_changed" : [] },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "character_sheet" : "character_sheet" } }
          }
        },
        { "NOT data_is" : { "data" : [ "node:author:uid" ], "value" : [ "site:current-user:uid" ] } },
        { "data_is" : { "data" : [ "node:created" ], "op" : "\\u003E", "value" : "-7 days" } },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "3" : "3" } },
            "new_state" : { "value" : { "4" : "4" } }
          }
        },
        { "data_is" : {
            "data" : [ "site:current-user:field-powers-expire" ],
            "op" : "\\u003C",
            "value" : "+6 months"
          }
        },
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "9" : "9" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "site:current-user:field-powers-expire" ],
            "value" : "+6 months"
          }
        }
      ]
    }
  }');
  $items['rules_update_expire_on_session_creation'] = entity_import('rules_config', '{ "rules_update_expire_on_session_creation" : {
      "LABEL" : "SH Duties - Update power expire on session creation",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Duties" ],
      "REQUIRES" : [ "rules", "php" ],
      "ON" : {
        "node_update--session_report" : { "bundle" : "session_report" },
        "node_insert--session_report" : { "bundle" : "session_report" }
      },
      "IF" : [
        { "data_is" : {
            "data" : [ "node:author:field-powers-expire" ],
            "op" : "\\u003C",
            "value" : "+3 months"
          }
        },
        { "data_is" : { "data" : [ "node:created" ], "op" : "\\u003E", "value" : "-3 days" } },
        { "data_is" : { "data" : [ "node:author:uid" ], "value" : [ "site:current-user:uid" ] } },
        { "php_eval" : { "code" : "return sh_test_session_counts($node);" } },
        { "user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "9" : "9" } } } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "node:author:field-powers-expire" ], "value" : "+3 months" } }
      ]
    }
  }');
  $items['rules_update_expire_on_session_update'] = entity_import('rules_config', '{ "rules_update_expire_on_session_update" : {
      "LABEL" : "SH Duties - Update power expire on session update",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Duties" ],
      "REQUIRES" : [ "rules", "php" ],
      "ON" : { "node_update--session_report" : { "bundle" : "session_report" } },
      "IF" : [
        { "data_is" : {
            "data" : [ "node:author:field-powers-expire" ],
            "op" : "\\u003C",
            "value" : "+3 months"
          }
        },
        { "data_is" : { "data" : [ "node:created" ], "op" : "\\u003E", "value" : "-3 days" } },
        { "data_is" : { "data" : [ "node:author:uid" ], "value" : [ "site:current-user:uid" ] } },
        { "php_eval" : { "code" : "return sh_test_session_counts($node);" } },
        { "user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "9" : "9" } } } }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "node:author:field-powers-expire" ], "value" : "+3 months" } }
      ]
    }
  }');
  $items['sh_test_update_expire_on_eval_item'] = entity_import('rules_config', '{ "sh_test_update_expire_on_eval_item" : {
      "LABEL" : "SH Duties - Update power expire on eval item",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Duties" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : { "workflow_state_changed" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "items" : "items" } } } },
        { "NOT data_is" : { "data" : [ "node:author:uid" ], "value" : [ "site:current-user:uid" ] } },
        { "data_is" : { "data" : [ "node:created" ], "op" : "\\u003E", "value" : "-7 days" } },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "14" : "14" } },
            "new_state" : { "value" : { "15" : "15" } }
          }
        },
        { "data_is" : {
            "data" : [ "site:current-user:field-powers-expire" ],
            "op" : "\\u003C",
            "value" : "+6 months"
          }
        },
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "9" : "9" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "site:current-user:field-powers-expire" ],
            "value" : "+6 months"
          }
        }
      ]
    }
  }');
  return $items;
}
