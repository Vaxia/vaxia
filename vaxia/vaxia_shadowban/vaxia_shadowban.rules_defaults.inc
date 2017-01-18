<?php
/**
 * @file
 * vaxia_shadowban.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_shadowban_default_rules_configuration() {
  $items = array();
  $items['rules_shadowban_auto_approve_character'] = entity_import('rules_config', '{ "rules_shadowban_auto_approve_character" : {
      "LABEL" : "Vaxia Shadowban - Auto approve character",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Shadowban" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : { "workflow_state_changed" : [] },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "node" ],
            "type" : "node",
            "bundle" : { "value" : { "character_sheet" : "character_sheet" } }
          }
        },
        { "shadowban_rules_is_banned" : [] },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "1" : "1", "2" : "2" } },
            "new_state" : { "value" : { "3" : "3" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "node:field-sh-private-notes:value" ],
            "value" : "THE PLAYER HAS A HISTORY OF USING ALTS TO CIRCUMVENT BANS - THEREFORE HE WAS SHADOWBANNED TO OCCUPY HIM. THIS CHARACTER WAS APPROVED WHILE UNDER SHADOWBAN. IT IS NOT APPROVED. - admin, [site:current-date]"
          }
        },
        { "workflow_rules_set_state" : { "node" : [ "node" ], "workflow_state" : { "value" : { "4" : "4" } } } }
      ]
    }
  }');
  $items['rules_vaxia_shadowban_auto_approve_item'] = entity_import('rules_config', '{ "rules_vaxia_shadowban_auto_approve_item" : {
      "LABEL" : "Vaxia Shadowban - Auto approve item",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Shadowban" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : { "workflow_state_changed" : [] },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "node" ],
            "type" : "node",
            "bundle" : { "value" : { "items" : "items" } }
          }
        },
        { "shadowban_rules_is_banned" : [] },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "12" : "12", "13" : "13" } },
            "new_state" : { "value" : { "14" : "14" } }
          }
        }
      ],
      "DO" : [
        { "workflow_rules_set_state" : { "node" : [ "node" ], "workflow_state" : { "value" : { "15" : "15" } } } }
      ]
    }
  }');
  $items['rules_vaxia_shadowban_auto_approve_scene'] = entity_import('rules_config', '{ "rules_vaxia_shadowban_auto_approve_scene" : {
      "LABEL" : "Vaxia Shadowban - Auto approve scene",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Shadowban" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : { "workflow_state_changed" : [] },
      "IF" : [
        { "entity_is_of_bundle" : {
            "entity" : [ "node" ],
            "type" : "node",
            "bundle" : { "value" : { "scene" : "scene" } }
          }
        },
        { "shadowban_rules_is_banned" : [] },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "26" : "26", "27" : "27" } },
            "new_state" : { "value" : { "28" : "28" } }
          }
        }
      ],
      "DO" : [
        { "workflow_rules_set_state" : { "node" : [ "node" ], "workflow_state" : { "value" : { "29" : "29" } } } }
      ]
    }
  }');
  $items['rules_vaxia_shadowban_unban_user'] = entity_import('rules_config', '{ "rules_vaxia_shadowban_unban_user" : {
      "LABEL" : "Vaxia Shadowban Unban User",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Shadowban" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_update" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "account-unchanged" ],
            "roles" : { "value" : { "33" : "33" } }
          }
        },
        { "NOT user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "33" : "33" } } } }
      ],
      "DO" : [
        { "shadowban_rules_update_nodes" : [] },
        { "drupal_message" : { "message" : "[account:name] has been UN-shadowbanned." } }
      ]
    }
  }');
  $items['rules_vaxia_shadowban_user'] = entity_import('rules_config', '{ "rules_vaxia_shadowban_user" : {
      "LABEL" : "Vaxia Shadowban User",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Shadowban" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_update" : [] },
      "IF" : [
        { "NOT user_has_role" : {
            "account" : [ "account-unchanged" ],
            "roles" : { "value" : { "33" : "33" } }
          }
        },
        { "user_has_role" : { "account" : [ "account" ], "roles" : { "value" : { "33" : "33" } } } }
      ],
      "DO" : [
        { "shadowban_rules_update_nodes" : [] },
        { "drupal_message" : { "message" : "[account:name] has been shadowbanned." } }
      ]
    }
  }');
  return $items;
}
