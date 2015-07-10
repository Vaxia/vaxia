<?php
/**
 * @file
 * item_shop.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function item_shop_default_rules_configuration() {
  $items = array();
  $items['rules_item_shop_clone_approved_item'] = entity_import('rules_config', '{ "rules_item_shop_clone_approved_item" : {
      "LABEL" : "Item shop - Clone owned item for shop",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Item shop" ],
      "REQUIRES" : [ "rules", "workflow_rules", "clone" ],
      "ON" : { "workflow_state_changed" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "items" : "items" } } } },
        { "workflow_check_transition" : {
            "node" : [ "node" ],
            "old_state" : { "value" : { "14" : "14" } },
            "new_state" : { "value" : { "15" : "15" } }
          }
        },
        { "data_is" : { "data" : [ "node:field-in-shop" ], "value" : 1 } },
        { "NOT data_is" : { "data" : [ "site:current-user:uid" ], "value" : "1" } },
        { "NOT data_is_empty" : { "data" : [ "node:field-character-owner" ] } }
      ],
      "DO" : [
        { "clone_action_node_clone" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "clone" : { "shop_item" : "Shop item" } }
          }
        },
        { "data_set" : { "data" : [ "shop-item:title" ], "value" : [ "node:title" ] } },
        { "entity_save" : { "data" : [ "shop-item" ], "immediate" : "1" } },
        { "workflow_rules_set_state" : {
            "node" : [ "shop-item" ],
            "workflow_state" : { "value" : { "15" : "15" } },
            "workflow_comment" : "Setting to approve after cloning from item ID #[node:nid]."
          }
        },
        { "data_set" : { "data" : [ "node:field-in-shop" ], "value" : 0 } },
        { "entity_save" : { "data" : [ "node" ] } },
        { "drupal_message" : { "message" : "A new item [shop-item:title] with item ID #[shop-item:nid] has been created from your item ID #[node:nid] and added to the item shop." } }
      ]
    }
  }');
  $items['rules_item_shop_created_item_not_for_resell'] = entity_import('rules_config', '{ "rules_item_shop_created_item_not_for_resell" : {
      "LABEL" : "REM Item shop - Created item not for resell",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Item shop" ],
      "REQUIRES" : [ "workflow_rules" ],
      "ON" : { "workflow_state_changed" : [] },
      "DO" : []
    }
  }');
  return $items;
}
