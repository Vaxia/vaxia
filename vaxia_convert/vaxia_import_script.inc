<?php
/**
 * This script converts Vaxia formatted files into CSV files for import.
 * Users, art, items, npcs.
 *
 * Doing it this way allows us to generate data, check it, and then update
 * via feeds into the new system.
 */

function vaxia_import_main() {
  // Now we have all the converted data in .xml form.
  // Make users from .users.xml
  if (file_exists('./vaxia_convert/users.xml')) {
    vaxia_import_users('./vaxia_convert/users.xml');
    drupal_set_message('Users have been converted.');
  }
  // Make art from art.xml
  if (file_exists('./vaxia_convert/art.xml')) {
    //vaxia_import_art('./vaxia_convert/art.xml');
    drupal_set_message('Art has been converted.');
  }
  // Make rooms from vaxia-rooms.xml
  if (file_exists('./vaxia_convert/vaxia-rooms.xml')) {
    //vaxia_import_rooms('./vaxia_convert/vaxia-rooms.xml');
    drupal_set_message('Vaxia rooms have been converted.');
  }
  if (file_exists('./vaxia_convert/sirian-rooms.xml')) {
    //vaxia_import_rooms('./vaxia_convert/sirian-rooms.xml');
    drupal_set_message('Sirian rooms have been converted.');
  }
  drupal_set_message('TASK: Create a central Limbo room.');
  // Make characters from characters.xml
  if (file_exists('./vaxia_convert/characters.xml')) {
    //vaxia_import_characters('./vaxia_convert/characters.xml');
    drupal_set_message('Characters have been converted.');
  }
  // Make npcs from npcs.xml
  if (file_exists('./vaxia_convert/npcs.xml')) {
    //vaxia_import_npcs('./vaxia_convert/npcs.xml');
    drupal_set_message('NPCs have been converted.');
  }
  // Make items from items.xml
  if (file_exists('./vaxia_convert/items.xml')) {
    //vaxia_import_items('./vaxia_convert/items.xml');
  }
  drupal_set_message('TASK: All has been converted. TURN THIS MODULE OFF.');
}

function vaxia_import_users($file) {
  $records = simplexml_load_file($file);
  $role_set = array_flip(user_roles(TRUE));
  foreach ($records->record as $record) {
    $roles = array();
    // Check player, player mature.
    if ($record['fullFledgedPlayerCheck'] != 'No') {
      $roles[] = $role_set['player'];
    }
    if ($record['matureRP'] != 'No') {
      $roles[] = $role_set['player-mature'];
    }
    // Check evals.
    drupal_set_message('TASK: Art, character, item evaulators should be reactived by hand.');
    drupal_set_message('TASK: Assistant storytellers and storytellers should be reactived by hand.');
    
    // Determine user roles by XLM values.
    $user = array(
      'name' => $record['playerName'],
      'pass' => user_password(), // Random password.
      'mail' => $record['email'],
      'init' => $record['email'],
      'status' => 1,
      'access' => REQUEST_TIME,
      'roles' => $roles,
    );
    $account = user_save(null, $user);
  }
}

function vaxia_import_art($file) {
  $records = simplexml_load_file($file);
  foreach ($records->record as $record) {
    $node = new stdClass();
    $node->type = 'artwork';
    node_object_prepare($node);
    $node->title = $record['name'];
    $user = user_load_by_name($record['player']);
    $node->uid = $user->uid;
    $node->language = LANGUAGE_NONE;
    $node->field_artwork_credit[$node->language][0]['value'] = $record['copyright'];
    $node->field_website[$node->language][0]['value'] = $record['url'];
    $image = file_get_contents($record['path']);
    $file = file_save_data($image, 'public://' . $record['name'], FILE_EXISTS_RENAME);
    $node->field_artwork_image[$node->language][0] = (array) $file;
    $node = node_save($node);
  }
}

function vaxia_import_rooms($file) {
  $records = simplexml_load_file($file);
  foreach ($records->record as $record) {
    $node = new stdClass();
    $node->type = 'rpg-chatroom';
    node_object_prepare($node);
    $node->title = $record['name'];
    $node->uid = 1;
    $node->language = LANGUAGE_NONE;
    $node->body[$node->language][0]['value'] = $record['locDescription'];
    $node->body[$node->language][0]['summary'] = text_summary($record['locDescription']);
    $node->body[$node->language][0]['format']  = 'filtered_html';
    $node->field_parent[$node->language][0]['nid'] = vaxia_convert_get_nid($record['parent']);
    $node->field_artwork[$node->language][0]['nid'] = vaxia_convert_get_nid($record['image']);
    $node = node_save($node);
  }
}

function vaxia_import_characters($file) {
  $records = simplexml_load_file($file);
  foreach ($records->record as $record) {
    $node = new stdClass();
    $node->type = 'character_sheet';
    node_object_prepare($node);
    $node->title = $record['character'];
    $user = user_load_by_name($record['player']);
    $node->uid = $user->uid;
    $node->field_artwork[$node->language][0]['nid'] = vaxia_convert_get_nid($record['image']);

    $node->field_age_app[$node->language][0]['value'] = $record['charAppAge'];
    $node->field_age_real[$node->language][0]['value'] = $record['charAge'];
    $node->field_economy[$node->language][0]['value'] = $record['ecolevel'];
    $node->field_race[$node->language][0]['value'] = $record['charRace'];
    $realm = 'vaxia';
    if (isset($record['sirianCert']) && $record['sirianCert'] == 'Yes') {
      $realm = 'sirian';
    }
    $node->field_realm[$node->language][0]['value'] = $realm;
    $node->field_sex[$node->language][0]['value'] = $record['charSex'];

    $node->field_background[$node->language][0]['value'] = $record['background'];
    $node->field_description[$node->language][0]['value'] = $record['description'];
    $node->field_occupation[$node->language][0]['value'] = $record['ocupation'];
    $node->field_personality[$node->language][0]['value'] = $record['personality'];

    $node->field_player_notes[$node->language][0]['value'] = $record['playerNotes'];
    $node->field_sh_notes[$node->language][0]['value'] = $record['shNotes'];

    $node->field_property[$node->language][0]['value'] = $record['playerProperty'];
    //$node->field_items[$node->language][0]['value'] = $record['KEYNAME'];
    // Leave items empty, players will have to reassign them to characters.

    $node->field_agility[$node->language][0]['value'] = $record['agi'];
    $node->field_appearance[$node->language][0]['value'] = $record['bty'];
    $node->field_awareness[$node->language][0]['value'] = $record['awa'];
    $node->field_charisma[$node->language][0]['value'] = $record['cha'];
    $node->field_constitution[$node->language][0]['value'] = $record['con'];
    $node->field_dexterity[$node->language][0]['value'] = $record['dex'];
    $node->field_endurance[$node->language][0]['value'] = $record['end'];
    $node->field_health[$node->language][0]['value'] = $record['hlt'];
    $node->field_intelligence[$node->language][0]['value'] = $record['int'];
    $node->field_life[$node->language][0]['value'] = $record['lfe'];
    $node->field_presence[$node->language][0]['value'] = $record['pre'];
    $node->field_reflexes[$node->language][0]['value'] = $record['roa'];
    $node->field_spirituality[$node->language][0]['value'] = $record['spi'];
    $node->field_strength[$node->language][0]['value'] = $record['str'];

    // And save the character.
    $node = node_save($node);

    // Add skills, values all are keyed as : skDELTAx = amount, skDELTAn = name , skDELTAx = d = description.
    $skill_fields = array();
    foreach ($record as $key => $value) {
      if (strpos($key, 'sk') !== FALSE && strpos($key, 'sk') === 0) {
        // Get the delta and set.
        $setting = substr($key, -1);
        $delta = substr($key, 2, -1);
        $skill_fields[$delta][$setting] = $value;
      }
    }
    // And create and add the skills to the node.
    // Now we have a mapped array of skill values.
    foreach ($skill_fields as $delta => $values) {
      if (count($values) == 3) {
        $skill = entity_create('field_collection_item', array('field_name' => 'field_skill'));
        $skill->setHostEntity('node', $node);
        $skill->field_skill_desc[$node->language][0]['value'] = $values['x'];
        $skill->field_skill_name[$node->language][0]['value'] = $values['n'];
        $skill->field_skill_value[$node->language][0]['value'] = $values['d'];
        $skill->save();
        $node->field_skill[$node->language][]['value'] = $skill->item_id;
      }
    }
    // Save updated skills.
    $node = node_save($node);
    // Save XP.
    $flavoured_xp = array(
      'charGxps' => 'good',
      'charExps' => 'evil',
      'charSxps' => 'smart',
      'charCxps' => 'clever',
      'charWxps' => 'warrior',
      'charMxps' => 'mage',
      'charTxps' => 'rogue',
    );
    // Mark the historical record of flavored XP.
    foreach ($flavoured_xp as $old_type => $new_type) {
      if (isset($record[$old_type]) && $record[$old_type] > 0) {
        $xp = $record[$old_type];
        $message = t('%xp %type XP added to %title.', array('%xp' => $xp, '%type' => $type, '%title' => $node->title));
        _character_sheet_update_log($node->nid, 'xp', $message, $variables = array());
      }
    }
    _character_sheet_update_xp($node->nid, $record['charXps'], 'xp'); // Available unlocked xp.
    // Spent = zero, so it doesn't matter if they have prior XP spent, this is what's available.
    _character_sheet_update_xp($node->nid, $record['charBxps'], 'bxp');
    _character_sheet_update_xp($node->nid, $record['charRxps'], 'rxp');
    _character_sheet_update_xp($node->nid, $record['charKxps'], 'kxp');
    // And this is what is in the system - flat - for their calculations henceforth.
    _character_sheet_update_xp($node->nid, $record['invest'], 'invest_silver');
    _character_sheet_update_xp($node->nid, $record['silver'], 'silver');
  }
}

function vaxia_import_items($file) {
  $records = simplexml_load_file($file);
  foreach ($records->record as $record) {
    $node = new stdClass();
    $node->type = 'items';
    node_object_prepare($node);
    $node->title = $record['USEitemName'];
    $user = user_load_by_name($record['player']);
    $node->uid = $user->uid;
    $node->field_item_desc[$node->language][0]['value'] = $record['NEWitemDescription'];
    $node->field_item_history[$node->language][0]['value'] = '';
    $node->field_item_notes[$node->language][0]['value'] = '';
    if (numeric($record['MODitemValue'])) {
      $node->field_item_value[$node->language][0]['value'] = $record['MODitemValue'];
    }
    else {
      $node->field_item_value[$node->language][0]['value'] = 0;
    }
    // Set stat modifications.
    $mods_map = array(
      'MODagi' => 'field_item_agility',
      'MODbty' => 'field_item_appearance',
      'MODawa' => 'field_item_awareness',
      'MODcha' => 'field_item_charisma',
      'MODcon' => 'field_item_constitution',
      'MODdex' => 'field_item_dexterity',
      'MODend' => 'field_item_endurance',
      'MODhlt' => 'field_item_health',
      'MODint' => 'field_item_intelligence',
      'MODlfe' => 'field_item_life',
      'MODpre' => 'field_item_presence',
      'MODroa' => 'field_item_reflexes',
      'MODspi' => 'field_item_spirituality',
      'MODstr' => 'field_item_strength',
    );
    foreach ($mods_map as $key => $map) {
      if (isset($record[$key])) {
        $node->{$map}[$node->language][0]['value'] = $record[$key];
      }
      else {
        $node->{$map}[$node->language][0]['value'] = 0;
      }
    }
    // Set stat requirements.
    $mods_map = array(
      'MODreqagi' => 'field_item_required_agility',
      'MODreqbty' => 'field_item_required_appearance',
      'MODreqawa' => 'field_item_required_awareness',
      'MODreqcha' => 'field_item_required_charisma',
      'MODreqcon' => 'field_item_required_constitution',
      'MODreqdex' => 'field_item_required_dexterity',
      'MODreqend' => 'field_item_required_endurance',
      'MODreqhlt' => 'field_item_required_health',
      'MODreqint' => 'field_item_required_intelligence',
      'MODreqlfe' => 'field_item_required_life',
      'MODreqpre' => 'field_item_required_presence',
      'MODreqroa' => 'field_item_required_reflexes',
      'MODreqspi' => 'field_item_required_spirituality',
      'MODreqstr' => 'field_item_required_strength',
    );
    foreach ($mods_map as $key => $map) {
      if (isset($record[$key])) {
        $node->{$map}[$node->language][0]['value'] = $record[$key];
      }
      else {
        $node->{$map}[$node->language][0]['value'] = 0;
      }
    }
    // Set skill modifier.
    if (isset($record['MODski'])) {
      $node->field_item_skill[$node->language][0]['value'] = $record['MODski'];
    }
    else {
      $node->field_item_skill[$node->language][0]['value'] = 0;
    }
    // Set skill required.
    if (isset($record['MODreqski'])) {
      $node->field_item_required_skill[$node->language][0]['value'] = $record['MODreqski'];
    }
    else {
      $node->field_item_required_skill[$node->language][0]['value'] = 0;
    }
    // Save if we have an item. (Some item records are incomplete.)
    if (isset($record['USEitemName'])) {
      $node = node_save($node);
    }
  }
  drupal_set_message('Items have been converted.');
}

function vaxia_import_npcs($file) {
  $records = simplexml_load_file($file);
  foreach ($records->record as $record) {
    $node = new stdClass();
    $node->type = 'npc';
    node_object_prepare($node);
    $node->title = $record['name'];
    $user = user_load_by_name($record['player']);
    $node->uid = $user->uid;
    $node->language = LANGUAGE_NONE;
    $node->body[$node->language][0]['value'] = $record['description'];
    $node->body[$node->language][0]['summary'] = text_summary($record['description']);
    $node->body[$node->language][0]['format']  = 'filtered_html';
    $node->field_artwork[$node->language][0]['value'] = vaxia_convert_get_nid($record['image']);
    $node->field_age_real[$node->language][0]['value'] = $record['age'];
    $node->field_age_app[$node->language][0]['value'] = $record['AppAge'];
    $node->field_race[$node->language][0]['value'] = $record['race'];
    $node->field_sex[$node->language][0]['value'] = $record['sex'];
    $node->field_last_name[$node->language][0]['value'] = $record['last'];
    $node->field_sex[$node->language][0]['value'] = $record['titles'];
    $node->field_sh_notes[$node->language][0]['value'] = $record['shNotes'];
    // Add Last name? last
    // Add SH notes? shNotes
    // Add titles? titles
    $node = node_save($node);
  }
}

function vaxia_convert_get_nid($title) {
  $nid = FALSE;
  $nid = db_query('SELECT nid FROM {node] WHERE title = :title', array(':title' => $title))->fetchCol();
  return $nid;
}
