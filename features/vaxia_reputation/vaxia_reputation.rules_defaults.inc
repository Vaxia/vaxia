<?php
/**
 * @file
 * vaxia_reputation.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_reputation_default_rules_configuration() {
  $items = array();
  $items['rules_add_v_rep_for_being_in_a_session'] = entity_import('rules_config', '{ "rules_add_v_rep_for_being_in_a_session" : {
      "LABEL" : "Give v-rep for being in a session",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Vaxia Reputation" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "session_report" : "session_report" } }
          }
        }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-characters-involved" ] },
            "ITEM" : { "characters_involved" : "Characters involved" },
            "DO" : [ { "userpoints_action_grant_points" : [] } ]
          }
        }
      ]
    }
  }');
  $items['rules_give_v_rep_for_approving_character_sheet'] = entity_import('rules_config', '{ "rules_give_v_rep_for_approving_character_sheet" : {
      "LABEL" : "Give v-rep for Approving sheet w\\/in 3 days",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Vaxia Reputation" ],
      "REQUIRES" : [ "rules", "workflow_rules", "php" ],
      "ON" : [ "workflow_state_changed" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "character_sheet" : "character_sheet" } }
          }
        },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "3" : "3" } },
            "new_state" : { "value" : { "2" : "2", "4" : "4", "5" : "5" } }
          }
        },
        { "php_eval" : { "code" : "\\/\\/ Check against when node was submitted into submit state. If within 1 week, allow.\\r\\n$history = workflow_get_workflow_node_history_by_nid($node-\\u003Enid);\\r\\n$latest = $history[0];\\r\\n$next = $history[1];\\r\\n$diff = $latest-\\u003Estamp - $next-\\u003Estamp; \\/\\/ In seconds\\r\\n$week = 60 * 60 * 24 * 3;\\r\\n\\r\\nreturn $week \\u003E $diff;" } }
      ],
      "DO" : [ { "userpoints_action_grant_points" : [] } ]
    }
  }');
  $items['rules_give_v_rep_when_character_approved'] = entity_import('rules_config', '{ "rules_give_v_rep_when_character_approved" : {
      "LABEL" : "Give v-rep when character approved",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Vaxia Reputation" ],
      "REQUIRES" : [ "rules", "workflow_rules" ],
      "ON" : [ "workflow_state_changed" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "character_sheet" : "character_sheet" } }
          }
        },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "ANY" : "ANY" } },
            "new_state" : { "value" : { "4" : "4" } }
          }
        }
      ],
      "DO" : [
        { "userpoints_action_grant_points" : [] },
        { "userpoints_action_grant_points" : [] }
      ]
    }
  }');
  return $items;
}
