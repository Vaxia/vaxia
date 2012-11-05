<?php
/**
 * @file
 * vaxia.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function vaxia_user_default_roles() {
  $roles = array();

  // Exported role: administrator-vaxia.
  $roles['administrator-vaxia'] = array(
    'name' => 'administrator-vaxia',
    'weight' => '12',
  );

  // Exported role: art-evaluator.
  $roles['art-evaluator'] = array(
    'name' => 'art-evaluator',
    'weight' => '11',
  );

  // Exported role: char-evaluator.
  $roles['char-evaluator'] = array(
    'name' => 'char-evaluator',
    'weight' => '9',
  );

  // Exported role: item-evaluator.
  $roles['item-evaluator'] = array(
    'name' => 'item-evaluator',
    'weight' => '10',
  );

  // Exported role: player.
  $roles['player'] = array(
    'name' => 'player',
    'weight' => '3',
  );

  // Exported role: player-mature.
  $roles['player-mature'] = array(
    'name' => 'player-mature',
    'weight' => '4',
  );

  // Exported role: storyteller.
  $roles['storyteller'] = array(
    'name' => 'storyteller',
    'weight' => '7',
  );

  // Exported role: storyteller-assistant.
  $roles['storyteller-assistant'] = array(
    'name' => 'storyteller-assistant',
    'weight' => '8',
  );

  // Exported role: wiki-editor.
  $roles['wiki-editor'] = array(
    'name' => 'wiki-editor',
    'weight' => '5',
  );

  // Exported role: wiki-master.
  $roles['wiki-master'] = array(
    'name' => 'wiki-master',
    'weight' => '6',
  );

  return $roles;
}
