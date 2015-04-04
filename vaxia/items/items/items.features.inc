<?php
/**
 * @file
 * items.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function items_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "corresponding_node_references" && $api == "default_corresponding_node_references_presets") {
    return array("version" => "1");
  }
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function items_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function items_node_info() {
  $items = array(
    'items' => array(
      'name' => t('Item'),
      'base' => 'node_content',
      'description' => t('Character held items.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => t('Step 1. Check the wiki. Vaxia and Sirian have complete templates with item requirements, bonuses, value, as well as descriptions listed there.
Vaxia items are here: <a href="http://www.vaxia.org/wiki/items-and-equipment">http://www.vaxia.org/wiki/items-and-equipment</a>
Sirian items are here: <a href="http://www.vaxia.org/wiki/sirian-items-technology-and-equipment">http://www.vaxia.org/wiki/sirian-items-technology-and-equipment</a>
An outline on material, magical and mastercraft bonuses/reductions are here: <a href="http://www.vaxia.org/wiki/smithing">http://www.vaxia.org/wiki/smithing</a>
<br/><br/>
Step 2. Make sure to add all the specifics of the item. Remember if the str says \'fixed\' it will need to be added in the overlap field.
<br/><br/>
Step 3. In the SH notes, please add which template you are basing the item off of. A simple note like \'mastercraft medium armor\' will do. As well as a more complex note like "mastercraft medium armor with snail skin backer, for extra end" so that the items approval team knows where to look to make sure the item is within the parameters.
<br/><br/>
Step 4. Change workflow to submitted and and click \'save.\'
<br/><br/>
<b>Are you evaluating an item created by a player?</b>
<br/>
Don\'t forget to look over the settings in the Item Build Assistant. Make sure the RP bonus is set properly and each roll is interpreted correctly at the right difficulty. When in doubt - you can always ask the player about the intent for a particular post.'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function items_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Item evaluation
  $workflows['Item evaluation'] = array(
    'name' => 'Item evaluation',
    'tab_roles' => 'author,3,9',
    'options' => 'a:4:{s:16:"comment_log_node";s:1:"1";s:15:"comment_log_tab";s:1:"1";s:13:"name_as_title";s:1:"1";s:12:"watchdog_log";s:1:"1";}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => -50,
        'sysid' => 1,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      1 => array(
        'state' => 'draft',
        'weight' => -19,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      2 => array(
        'state' => 'submitted',
        'weight' => -18,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      3 => array(
        'state' => 'approved',
        'weight' => -17,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      4 => array(
        'state' => 'disapproved',
        'weight' => -16,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      5 => array(
        'state' => 'needs recharge',
        'weight' => -15,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      6 => array(
        'state' => 'needs daily refresh',
        'weight' => -14,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
      7 => array(
        'state' => 'removed from game',
        'weight' => -13,
        'sysid' => 0,
        'status' => 1,
        'name' => 'Item evaluation',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'draft',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'submitted',
      ),
      2 => array(
        'roles' => 'workflow_features_author_name,administrator,evaluator',
        'state' => 'draft',
        'target_state' => 'submitted',
      ),
      3 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'approved',
      ),
      4 => array(
        'roles' => 'administrator',
        'state' => 'draft',
        'target_state' => 'disapproved',
      ),
      5 => array(
        'roles' => 'administrator,evaluator',
        'state' => 'submitted',
        'target_state' => 'draft',
      ),
      6 => array(
        'roles' => 'administrator,evaluator',
        'state' => 'submitted',
        'target_state' => 'approved',
      ),
      7 => array(
        'roles' => 'administrator,evaluator',
        'state' => 'submitted',
        'target_state' => 'disapproved',
      ),
      8 => array(
        'roles' => 'administrator,evaluator',
        'state' => 'submitted',
        'target_state' => 'needs recharge',
      ),
      9 => array(
        'roles' => 'administrator',
        'state' => 'submitted',
        'target_state' => 'removed from game',
      ),
      10 => array(
        'roles' => 'administrator,evaluator',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      11 => array(
        'roles' => 'administrator,storyteller,evaluator',
        'state' => 'approved',
        'target_state' => 'submitted',
      ),
      12 => array(
        'roles' => 'administrator,evaluator',
        'state' => 'approved',
        'target_state' => 'disapproved',
      ),
      13 => array(
        'roles' => 'administrator,storyteller,evaluator',
        'state' => 'approved',
        'target_state' => 'needs recharge',
      ),
      14 => array(
        'roles' => 'administrator,evaluator',
        'state' => 'approved',
        'target_state' => 'removed from game',
      ),
      15 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'draft',
      ),
      16 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'submitted',
      ),
      17 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'approved',
      ),
      18 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'needs recharge',
      ),
      19 => array(
        'roles' => 'administrator',
        'state' => 'disapproved',
        'target_state' => 'removed from game',
      ),
      20 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'draft',
      ),
      21 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'submitted',
      ),
      22 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'approved',
      ),
      23 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'disapproved',
      ),
      24 => array(
        'roles' => 'administrator',
        'state' => 'needs recharge',
        'target_state' => 'removed from game',
      ),
      25 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'draft',
      ),
      26 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'submitted',
      ),
      27 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'approved',
      ),
      28 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'disapproved',
      ),
      29 => array(
        'roles' => 'administrator',
        'state' => 'removed from game',
        'target_state' => 'needs recharge',
      ),
    ),
    'node_types' => array(
      0 => 'items',
    ),
  );

  return $workflows;
}
