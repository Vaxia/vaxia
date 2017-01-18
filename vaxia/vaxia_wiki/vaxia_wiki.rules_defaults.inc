<?php
/**
 * @file
 * vaxia_wiki.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function vaxia_wiki_default_rules_configuration() {
  $items = array();
  $items['rules_vaxia_wiki_add_to_book_when_page_created'] = entity_import('rules_config', '{ "rules_vaxia_wiki_add_to_book_when_page_created" : {
      "LABEL" : "Vaxia Wiki - Add to book when page created",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "OWNER" : "rules",
      "TAGS" : [ "Vaxia Wiki" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_presave--wiki_page" : { "bundle" : "wiki_page" } },
      "DO" : []
    }
  }');
  $items['rules_wiki_page_flagged_to_be_deleted'] = entity_import('rules_config', '{ "rules_wiki_page_flagged_to_be_deleted" : {
      "LABEL" : "Wiki page flagged to be deleted",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Vaxia Wiki" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : { "flag_flagged_delete" : [] },
      "IF" : [ { "node_is_published" : { "node" : [ "flagged-node" ] } } ],
      "DO" : [ { "node_unpublish" : { "node" : [ "flagged-node" ] } } ]
    }
  }');
  $items['rules_wiki_page_unflagged_to_be_deleted'] = entity_import('rules_config', '{ "rules_wiki_page_unflagged_to_be_deleted" : {
      "LABEL" : "Wiki page unflagged to be deleted",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Vaxia Wiki" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : { "flag_unflagged_delete" : [] },
      "IF" : [ { "NOT node_is_published" : { "node" : [ "flagged-node" ] } } ],
      "DO" : [ { "node_publish" : { "node" : [ "flagged-node" ] } } ]
    }
  }');
  return $items;
}
