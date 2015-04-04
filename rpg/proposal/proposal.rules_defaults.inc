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
  $items['rpg_site_staff_update_voter_on_create_comment'] = entity_import('rules_config', '{ "rpg_site_staff_update_voter_on_create_comment" : {
      "LABEL" : "Proposal - Update voter on create comment",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "rules", "comment" ],
      "ON" : { "comment_insert" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "8" : "8", "19" : "19" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : {
            "data" : [ "comment:author:field-voting-expire" ],
            "value" : "+6 months"
          }
        }
      ]
    }
  }');
  $items['rpg_site_staff_update_voter_on_create_node'] = entity_import('rules_config', '{ "rpg_site_staff_update_voter_on_create_node" : {
      "LABEL" : "Proposal - Update voter on create node",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "node:author" ],
            "roles" : { "value" : { "8" : "8", "19" : "19" } }
          }
        }
      ],
      "DO" : [
        { "data_set" : { "data" : [ "node:author:field-voting-expire" ], "value" : "+6 months" } }
      ]
    }
  }');
  $items['rules_add_voting_player'] = entity_import('rules_config', '{ "rules_add_voting_player" : {
      "LABEL" : "Proposal - Add voter on create comment",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "rules", "comment" ],
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
        { "data_set" : { "data" : [ "comment:author:field-became-voter" ], "value" : "now" } },
        { "data_set" : {
            "data" : [ "comment:author:field-voting-expire" ],
            "value" : "+6 months"
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
  $items['rules_remove_non_voting_player'] = entity_import('rules_config', '{ "rules_remove_non_voting_player" : {
      "LABEL" : "Proposal - Remove voter on expire",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "19" : "19" } } } },
        { "data_is" : {
            "data" : [ "user:field-voting-expire" ],
            "op" : "\\u003C",
            "value" : "now"
          }
        }
      ],
      "DO" : [
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "19" : "19" } } } }
      ]
    }
  }');
  $items['proposal_add_voting_player_node'] = entity_import('rules_config', '{ "proposal_add_voting_player_node" : {
      "LABEL" : "Proposal - Add voter on create node",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Proposal" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "8" : "8" } } } }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "19" : "19" } } } },
        { "data_set" : { "data" : [ "node:author:field-voting-expire" ], "value" : "+6 months" } }
      ]
    }
  }');
  return $items;
}
