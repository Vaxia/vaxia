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
  $items['rules_close_open_proposals'] = entity_import('rules_config', '{ "rules_close_open_proposals" : {
      "LABEL" : "Close Open Proposals",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Vaxia" ],
      "REQUIRES" : [ "rules", "views_bulk_operations", "webform_rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [ { "data_is" : { "data" : [ "user:uid" ], "value" : "1" } } ],
      "DO" : [
        { "views_bulk_operations_action_load_id_list" : {
            "USING" : { "view" : "proposals|closable_proposals" },
            "PROVIDE" : { "entity_id_list" : { "entity_id_list" : "A list of entity ids" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "entity-id-list" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "entity_fetch" : {
                  "USING" : { "type" : "node", "id" : [ "list-item" ], "revision_id" : [ "" ] },
                  "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
                }
              },
              { "webform_close" : { "entity" : [ "entity-fetched" ], "selected_webform" : { "value" : [] } } },
              { "mail" : {
                  "to" : [ "entity-fetched:author:mail" ],
                  "subject" : "[Vaxia] Proposal closed: [entity-fetched:title]",
                  "message" : "Your proposal has been open for two-weeks and is now closed. Please check for the results at:\\r\\n\\r\\n[entity-fetched:title]\\r\\n[entity-fetched:url]\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will\\r\\nnot be received by a human.\\r\\n~~~~~~",
                  "language" : [ "" ]
                }
              }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}