<?php
/**
 * @file
 * shadowban2.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function shadowban2_default_rules_configuration() {
  $items = array();
  $items['rules_shadowban_ban_user'] = entity_import('rules_config', '{ "rules_shadowban_ban_user" : {
      "LABEL" : "Shadowban - Ban user",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Shadowban" ],
      "REQUIRES" : [ "shadowban", "rules" ],
      "ON" : { "user_update" : [] },
      "IF" : [
        { "NOT shadowban_rules_is_banned" : { "user" : [ "account-unchanged" ] } },
        { "shadowban_rules_is_banned" : { "user" : [ "account" ] } }
      ],
      "DO" : [
        { "drupal_message" : { "message" : "This user has been shadowbanned. Their nodes have been updated with the new access." } },
        { "shadowban_rules_update_nodes" : { "user" : [ "account" ] } }
      ]
    }
  }');
  $items['rules_shadowban_unban_user'] = entity_import('rules_config', '{ "rules_shadowban_unban_user" : {
      "LABEL" : "Shadowban - Unban user",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Shadowban" ],
      "REQUIRES" : [ "shadowban", "rules" ],
      "ON" : { "user_update" : [] },
      "IF" : [
        { "shadowban_rules_is_banned" : { "user" : [ "account-unchanged" ] } },
        { "NOT shadowban_rules_is_banned" : { "user" : [ "account" ] } }
      ],
      "DO" : [
        { "drupal_message" : { "message" : "This user has been UN-shadowbanned. Their nodes have been updated with the new access." } },
        { "shadowban_rules_update_nodes" : { "user" : [ "account" ] } }
      ]
    }
  }');
  return $items;
}
