<?php
/**
 * @file
 * vaxia.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function vaxia_user_default_permissions() {
  $permissions = array();

  // Exported permission: access comments.
  $permissions['access comments'] = array(
    'name' => 'access comments',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'comment',
  );

  // Exported permission: access content.
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      0 => 'administrator',
      1 => 'player',
    ),
    'module' => 'node',
  );

  // Exported permission: access private fields.
  $permissions['access private fields'] = array(
    'name' => 'access private fields',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
      3 => 'item-evaluator',
      4 => 'storyteller',
      5 => 'storyteller-assistant',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: add xp to character sheet.
  $permissions['add xp to character sheet'] = array(
    'name' => 'add xp to character sheet',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
      3 => 'storyteller',
      4 => 'storyteller-assistant',
    ),
    'module' => 'character_sheet',
  );

  // Exported permission: add xp to own character sheet.
  $permissions['add xp to own character sheet'] = array(
    'name' => 'add xp to own character sheet',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'character_sheet',
  );

  // Exported permission: administer character sheet.
  $permissions['administer character sheet'] = array(
    'name' => 'administer character sheet',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'character_sheet',
  );

  // Exported permission: administer comments.
  $permissions['administer comments'] = array(
    'name' => 'administer comments',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'comment',
  );

  // Exported permission: administer dice roller.
  $permissions['administer dice roller'] = array(
    'name' => 'administer dice roller',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'dice_roller',
  );

  // Exported permission: administer field permissions.
  $permissions['administer field permissions'] = array(
    'name' => 'administer field permissions',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: administer rpg chat.
  $permissions['administer rpg chat'] = array(
    'name' => 'administer rpg chat',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'rpg_chat',
  );

  // Exported permission: change art owner.
  $permissions['change art owner'] = array(
    'name' => 'change art owner',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'art-evaluator',
    ),
    'module' => 'vaxia',
  );

  // Exported permission: change character sheet owner.
  $permissions['change character sheet owner'] = array(
    'name' => 'change character sheet owner',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
    ),
    'module' => 'character_sheet',
  );

  // Exported permission: change own character sheet owner.
  $permissions['change own character sheet owner'] = array(
    'name' => 'change own character sheet owner',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'player',
    ),
    'module' => 'character_sheet',
  );

  // Exported permission: create NPCs.
  $permissions['create NPCs'] = array(
    'name' => 'create NPCs',
    'roles' => array(
      0 => 'administrator',
      1 => 'storyteller',
    ),
    'module' => 'vaxia',
  );

  // Exported permission: create artwork content.
  $permissions['create artwork content'] = array(
    'name' => 'create artwork content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'art-evaluator',
      3 => 'player',
    ),
    'module' => 'node',
  );

  // Exported permission: create character_sheet content.
  $permissions['create character_sheet content'] = array(
    'name' => 'create character_sheet content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
      3 => 'player',
      4 => 'storyteller',
    ),
    'module' => 'node',
  );

  // Exported permission: create field_sh_private_notes.
  $permissions['create field_sh_private_notes'] = array(
    'name' => 'create field_sh_private_notes',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
      3 => 'item-evaluator',
      4 => 'storyteller',
      5 => 'storyteller-assistant',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: create items content.
  $permissions['create items content'] = array(
    'name' => 'create items content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'item-evaluator',
      3 => 'player',
      4 => 'storyteller',
    ),
    'module' => 'node',
  );

  // Exported permission: create npc content.
  $permissions['create npc content'] = array(
    'name' => 'create npc content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'storyteller',
    ),
    'module' => 'node',
  );

  // Exported permission: create open NPCs.
  $permissions['create open NPCs'] = array(
    'name' => 'create open NPCs',
    'roles' => array(
      0 => 'administrator',
      1 => 'storyteller',
    ),
    'module' => 'vaxia',
  );

  // Exported permission: create rpg_chatroom content.
  $permissions['create rpg_chatroom content'] = array(
    'name' => 'create rpg_chatroom content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'item-evaluator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any artwork content.
  $permissions['delete any artwork content'] = array(
    'name' => 'delete any artwork content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any character_sheet content.
  $permissions['delete any character_sheet content'] = array(
    'name' => 'delete any character_sheet content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any items content.
  $permissions['delete any items content'] = array(
    'name' => 'delete any items content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any npc content.
  $permissions['delete any npc content'] = array(
    'name' => 'delete any npc content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any rpg_chatroom content.
  $permissions['delete any rpg_chatroom content'] = array(
    'name' => 'delete any rpg_chatroom content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'item-evaluator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own artwork content.
  $permissions['delete own artwork content'] = array(
    'name' => 'delete own artwork content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'player',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own character_sheet content.
  $permissions['delete own character_sheet content'] = array(
    'name' => 'delete own character_sheet content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'player',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own items content.
  $permissions['delete own items content'] = array(
    'name' => 'delete own items content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'player',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own npc content.
  $permissions['delete own npc content'] = array(
    'name' => 'delete own npc content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'storyteller',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own rpg_chatroom content.
  $permissions['delete own rpg_chatroom content'] = array(
    'name' => 'delete own rpg_chatroom content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'item-evaluator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete revisions.
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: delete terms in 2.
  $permissions['delete terms in 2'] = array(
    'name' => 'delete terms in 2',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: edit any artwork content.
  $permissions['edit any artwork content'] = array(
    'name' => 'edit any artwork content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'art-evaluator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any character_sheet content.
  $permissions['edit any character_sheet content'] = array(
    'name' => 'edit any character_sheet content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
      3 => 'storyteller',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any items content.
  $permissions['edit any items content'] = array(
    'name' => 'edit any items content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'item-evaluator',
      3 => 'storyteller',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any npc content.
  $permissions['edit any npc content'] = array(
    'name' => 'edit any npc content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any rpg_chatroom content.
  $permissions['edit any rpg_chatroom content'] = array(
    'name' => 'edit any rpg_chatroom content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'item-evaluator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit field_sh_private_notes.
  $permissions['edit field_sh_private_notes'] = array(
    'name' => 'edit field_sh_private_notes',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
      3 => 'item-evaluator',
      4 => 'storyteller',
      5 => 'storyteller-assistant',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own artwork content.
  $permissions['edit own artwork content'] = array(
    'name' => 'edit own artwork content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own character_sheet content.
  $permissions['edit own character_sheet content'] = array(
    'name' => 'edit own character_sheet content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'player',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own comments.
  $permissions['edit own comments'] = array(
    'name' => 'edit own comments',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'comment',
  );

  // Exported permission: edit own field_sh_private_notes.
  $permissions['edit own field_sh_private_notes'] = array(
    'name' => 'edit own field_sh_private_notes',
    'roles' => array(),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own items content.
  $permissions['edit own items content'] = array(
    'name' => 'edit own items content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own npc content.
  $permissions['edit own npc content'] = array(
    'name' => 'edit own npc content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'storyteller',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own rpg_chatroom content.
  $permissions['edit own rpg_chatroom content'] = array(
    'name' => 'edit own rpg_chatroom content',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'item-evaluator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit terms in 2.
  $permissions['edit terms in 2'] = array(
    'name' => 'edit terms in 2',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: post comments.
  $permissions['post comments'] = array(
    'name' => 'post comments',
    'roles' => array(
      0 => 'administrator',
      1 => 'player',
    ),
    'module' => 'comment',
  );

  // Exported permission: post in narrative format.
  $permissions['post in narrative format'] = array(
    'name' => 'post in narrative format',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'storyteller',
    ),
    'module' => 'vaxia',
  );

  // Exported permission: remove xp from character sheet.
  $permissions['remove xp from character sheet'] = array(
    'name' => 'remove xp from character sheet',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
      3 => 'storyteller',
    ),
    'module' => 'character_sheet',
  );

  // Exported permission: remove xp from own character sheet.
  $permissions['remove xp from own character sheet'] = array(
    'name' => 'remove xp from own character sheet',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'character_sheet',
  );

  // Exported permission: revert revisions.
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'node',
  );

  // Exported permission: search content.
  $permissions['search content'] = array(
    'name' => 'search content',
    'roles' => array(
      0 => 'administrator',
      1 => 'player',
    ),
    'module' => 'search',
  );

  // Exported permission: skip comment approval.
  $permissions['skip comment approval'] = array(
    'name' => 'skip comment approval',
    'roles' => array(
      0 => 'administrator',
      1 => 'player',
    ),
    'module' => 'comment',
  );

  // Exported permission: transfer others sheets.
  $permissions['transfer others sheets'] = array(
    'name' => 'transfer others sheets',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
    ),
    'module' => 'vaxia',
  );

  // Exported permission: use NPCs.
  $permissions['use NPCs'] = array(
    'name' => 'use NPCs',
    'roles' => array(
      0 => 'administrator',
      1 => 'storyteller',
    ),
    'module' => 'vaxia',
  );

  // Exported permission: use advanced search.
  $permissions['use advanced search'] = array(
    'name' => 'use advanced search',
    'roles' => array(
      0 => 'administrator',
      1 => 'player',
    ),
    'module' => 'search',
  );

  // Exported permission: use dice roller.
  $permissions['use dice roller'] = array(
    'name' => 'use dice roller',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'player',
    ),
    'module' => 'dice_roller',
  );

  // Exported permission: view character sheet log.
  $permissions['view character sheet log'] = array(
    'name' => 'view character sheet log',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
    ),
    'module' => 'character_sheet',
  );

  // Exported permission: view dice rolls.
  $permissions['view dice rolls'] = array(
    'name' => 'view dice rolls',
    'roles' => array(
      0 => 'administrator',
      1 => 'player',
    ),
    'module' => 'dice_roller',
  );

  // Exported permission: view field_sh_private_notes.
  $permissions['view field_sh_private_notes'] = array(
    'name' => 'view field_sh_private_notes',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'char-evaluator',
      3 => 'item-evaluator',
      4 => 'storyteller',
      5 => 'storyteller-assistant',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own field_sh_private_notes.
  $permissions['view own field_sh_private_notes'] = array(
    'name' => 'view own field_sh_private_notes',
    'roles' => array(),
    'module' => 'field_permissions',
  );

  // Exported permission: view own unpublished content.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      0 => 'administrator',
      1 => 'player',
    ),
    'module' => 'node',
  );

  // Exported permission: view revisions.
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(
      0 => 'administrator',
      1 => 'player',
    ),
    'module' => 'node',
  );

  // Exported permission: view rpg chat.
  $permissions['view rpg chat'] = array(
    'name' => 'view rpg chat',
    'roles' => array(
      0 => 'administrator',
      1 => 'administrator-vaxia',
      2 => 'player',
    ),
    'module' => 'rpg_chat',
  );

  // Exported permission: view rpg mature chat.
  $permissions['view rpg mature chat'] = array(
    'name' => 'view rpg mature chat',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'rpg_chat',
  );

  return $permissions;
}
