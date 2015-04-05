<?php
/**
 * @file
 * proposal.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function proposal_default_rules_configuration() {
  $items = array();
  $items['proposal_add_voting_player_node'] = entity_import('rules_config', '{ "proposal_add_voting_player_node" : {
      "LABEL" : "Proposal - Add voter on create node",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "rules", "role_expire" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "8" : "8" } } } }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "19" : "19" } } } },
        { "role_expire_rules_action_set_role_expire" : { "user" : [ "node:author" ], "role" : "19", "timestamp" : "+6 months" } }
      ]
    }
  }');
  $items['rpg_site_staff_update_voter_on_create_comment'] = entity_import('rules_config', '{ "rpg_site_staff_update_voter_on_create_comment" : {
      "LABEL" : "Proposal - Update voter on create comment",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "rules", "role_expire", "comment" ],
      "ON" : { "comment_insert" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "8" : "8", "19" : "19" } }
          }
        }
      ],
      "DO" : [
        { "role_expire_rules_action_set_role_expire" : {
            "user" : [ "comment:author" ],
            "role" : "19",
            "timestamp" : "+6 months"
          }
        }
      ]
    }
  }');
  $items['rules_add_voting_player'] = entity_import('rules_config', '{ "rules_add_voting_player" : {
      "LABEL" : "Proposal - Add voter on create comment",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "rules", "role_expire", "comment" ],
      "ON" : { "comment_insert" : [] },
      "IF" : [
        { "NOT user_is_blocked" : { "account" : [ "comment:author" ] } },
        { "user_has_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "8" : "8" } }
          }
        },
        { "NOT user_has_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "19" : "19" } }
          }
        }
      ],
      "DO" : [
        { "user_add_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "19" : "19" } }
          }
        },
        { "role_expire_rules_action_set_role_expire" : {
            "user" : [ "comment:author" ],
            "role" : "19",
            "timestamp" : "+6 months"
          }
        }
      ]
    }
  }');
  $items['rules_close_proposal'] = entity_import('rules_config', '{ "rules_close_proposal" : {
      "LABEL" : "Proposal - Close proposal",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "webform_rules", "proposal" ],
      "ON" : { "close_proposal" : [] },
      "IF" : [ { "node_is_webform" : { "node" : [ "node" ] } } ],
      "DO" : [
        { "webform_close" : { "entity" : [ "node" ], "selected_webform" : { "value" : [] } } }
      ]
    }
  }');
  return $items;
}
