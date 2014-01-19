<?php
/**
 * @file
 * vaxia.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vaxia_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "corresponding_node_references" && $api == "default_corresponding_node_references_presets") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function vaxia_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function vaxia_image_default_styles() {
  $styles = array();

  // Exported image style: tiny-thumbnail.
  $styles['tiny-thumbnail'] = array(
    'name' => 'tiny-thumbnail',
    'label' => 'tiny-thumbnail',
    'effects' => array(
      2 => array(
        'label' => 'Resize',
        'help' => 'Resizing will make images an exact set of dimensions. This may cause images to be stretched or shrunk disproportionately.',
        'effect callback' => 'image_resize_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_resize',
        'data' => array(
          'width' => '40',
          'height' => '40',
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function vaxia_node_info() {
  $items = array(
    'artwork' => array(
      'name' => t('Artwork'),
      'base' => 'node_content',
      'description' => t('Artwork for character sheets'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('Step 1: When creating artwork we are under Fair Use policies here. In general, we also ask that you keep the art you use for your characters at the PG-13 level. Vaxia admin reserve the right to tighten down on the approval for art if the freedom is abused.
<br/><br/>
Step 2: Save, when ready to submit change the workflow to approved to activate the art for the site.'),
    ),
    'character_sheet' => array(
      'name' => t('Character sheet'),
      'base' => 'node_content',
      'description' => t('Character sheet'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => t('Step 1: Creating a new character, the guidelines can be found here: <a href="http://vaxia.org/wiki/character-creation">http://vaxia.org/wiki/character-creation</a>
<br/><br/>
Step 2: Save, when ready to submit change the workflow to submitted.
<br/><br/>
Things to remember: you get 300 points to allocate between stats and skills pre-play. Those points are flat. 30 points in a stat is 30 points from your pool, 20 points in a skill is 20 from the pool. It is only once in play, when you are spending XP, that the cost of higher values will scale.'),
    ),
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
Step 4. Change workflow to submitted and and click \'save.\''),
    ),
    'npc' => array(
      'name' => t('NPC'),
      'base' => 'node_content',
      'description' => t('NPC for character sheets.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'rpg_chatroom' => array(
      'name' => t('RPG Chatroom'),
      'base' => 'node_content',
      'description' => t('RPG chatroom'),
      'has_title' => '1',
      'title_label' => t('Room name'),
      'help' => '',
    ),
    'session_report' => array(
      'name' => t('Session report'),
      'base' => 'node_content',
      'description' => t('For SHs after a session, to include a list of users, characters and provide a record of events.'),
      'has_title' => '1',
      'title_label' => t('Session name'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_workflow_default_workflows().
 */
function vaxia_workflow_default_workflows() {
  $workflows = array();

  // Exported workflow: Art evaluation workflow
  $workflows['Art evaluation workflow'] = array(
    'name' => 'Art evaluation workflow',
    'tab_roles' => 'author,4,3',
    'options' => 'a:4:{s:16:"comment_log_node";i:1;s:15:"comment_log_tab";i:1;s:13:"name_as_title";i:1;s:12:"watchdog_log";i:1;}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => '-50',
        'sysid' => '1',
        'status' => '1',
        'name' => 'Art evaluation workflow',
      ),
      1 => array(
        'state' => 'draft',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Art evaluation workflow',
      ),
      2 => array(
        'state' => 'approved',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Art evaluation workflow',
      ),
      3 => array(
        'state' => 'removed from site',
        'weight' => '0',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Art evaluation workflow',
      ),
    ),
    'transitions' => array(
      0 => array(
        'roles' => 'workflow_features_author_name',
        'state' => '(creation)',
        'target_state' => 'approved',
      ),
      1 => array(
        'roles' => 'workflow_features_author_name,administrator-vaxia,administrator',
        'state' => 'draft',
        'target_state' => 'approved',
      ),
      2 => array(
        'roles' => 'workflow_features_author_name,administrator-vaxia,administrator',
        'state' => 'draft',
        'target_state' => 'removed from site',
      ),
      3 => array(
        'roles' => 'workflow_features_author_name,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      4 => array(
        'roles' => 'workflow_features_author_name,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'removed from site',
      ),
      5 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'removed from site',
        'target_state' => 'draft',
      ),
      6 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'removed from site',
        'target_state' => 'approved',
      ),
    ),
    'node_types' => array(
      0 => 'artwork',
    ),
  );

  // Exported workflow: Item evaluation workflow
  $workflows['Item evaluation workflow'] = array(
    'name' => 'Item evaluation workflow',
    'tab_roles' => 'author,3,9,4',
    'options' => 'a:4:{s:16:"comment_log_node";i:1;s:15:"comment_log_tab";i:1;s:13:"name_as_title";i:1;s:12:"watchdog_log";i:1;}',
    'states' => array(
      0 => array(
        'state' => '(creation)',
        'weight' => '-50',
        'sysid' => '1',
        'status' => '1',
        'name' => 'Item evaluation workflow',
      ),
      1 => array(
        'state' => 'draft',
        'weight' => '-19',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Item evaluation workflow',
      ),
      2 => array(
        'state' => 'submitted',
        'weight' => '-18',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Item evaluation workflow',
      ),
      3 => array(
        'state' => 'approved',
        'weight' => '-17',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Item evaluation workflow',
      ),
      4 => array(
        'state' => 'disapproved',
        'weight' => '-16',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Item evaluation workflow',
      ),
      5 => array(
        'state' => 'moved to property tab',
        'weight' => '-15',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Item evaluation workflow',
      ),
      6 => array(
        'state' => 'removed from game',
        'weight' => '-14',
        'sysid' => '0',
        'status' => '1',
        'name' => 'Item evaluation workflow',
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
        'roles' => 'workflow_features_author_name,storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'draft',
        'target_state' => 'submitted',
      ),
      3 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'draft',
        'target_state' => 'approved',
      ),
      4 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'draft',
        'target_state' => 'disapproved',
      ),
      5 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'submitted',
        'target_state' => 'draft',
      ),
      6 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'submitted',
        'target_state' => 'approved',
      ),
      7 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'submitted',
        'target_state' => 'disapproved',
      ),
      8 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'submitted',
        'target_state' => 'moved to property tab',
      ),
      9 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'submitted',
        'target_state' => 'removed from game',
      ),
      10 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'draft',
      ),
      11 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'submitted',
      ),
      12 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'disapproved',
      ),
      13 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'moved to property tab',
      ),
      14 => array(
        'roles' => 'storyteller,evaluator,administrator-vaxia,administrator',
        'state' => 'approved',
        'target_state' => 'removed from game',
      ),
      15 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'disapproved',
        'target_state' => 'draft',
      ),
      16 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'disapproved',
        'target_state' => 'submitted',
      ),
      17 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'disapproved',
        'target_state' => 'approved',
      ),
      18 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'disapproved',
        'target_state' => 'moved to property tab',
      ),
      19 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'disapproved',
        'target_state' => 'removed from game',
      ),
      20 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'moved to property tab',
        'target_state' => 'draft',
      ),
      21 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'moved to property tab',
        'target_state' => 'submitted',
      ),
      22 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'moved to property tab',
        'target_state' => 'approved',
      ),
      23 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'moved to property tab',
        'target_state' => 'disapproved',
      ),
      24 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'moved to property tab',
        'target_state' => 'removed from game',
      ),
      25 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'removed from game',
        'target_state' => 'draft',
      ),
      26 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'removed from game',
        'target_state' => 'submitted',
      ),
      27 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'removed from game',
        'target_state' => 'approved',
      ),
      28 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'removed from game',
        'target_state' => 'disapproved',
      ),
      29 => array(
        'roles' => 'administrator-vaxia,administrator',
        'state' => 'removed from game',
        'target_state' => 'moved to property tab',
      ),
    ),
    'node_types' => array(
      0 => 'items',
    ),
  );

  return $workflows;
}
