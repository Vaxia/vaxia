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
    //vaxia_import_users('./vaxia_convert/users.xml');
    drupal_set_message('Users have been converted.');
  }
  set_time_limit(180);
  // Make art from art.xml
  if (file_exists('./vaxia_convert/art.xml')) {
    //vaxia_import_art('./vaxia_convert/art.xml');
    drupal_set_message('Art has been converted.');
  }
  set_time_limit(180);
  // Make rooms from vaxia-rooms.xml
  if (file_exists('./vaxia_convert/vaxia-rooms.xml')) {
    //vaxia_import_rooms('./vaxia_convert/vaxia-rooms.xml');
    drupal_set_message('Vaxia rooms have been converted.');
  }
  if (file_exists('./vaxia_convert/sirian-rooms.xml')) {
    //vaxia_import_rooms('./vaxia_convert/sirian-rooms.xml');
    drupal_set_message('Sirian rooms have been converted.');
  }
  set_time_limit(180);
  drupal_set_message('TASK: Create a central Limbo room.');
  // Make characters from characters.xml
  if (file_exists('./vaxia_convert/characters.xml')) {
    //vaxia_import_characters('./vaxia_convert/characters.xml');
    drupal_set_message('Characters have been converted.');
  }
  // Make items from items.xml
  if (file_exists('./vaxia_convert/items.xml')) {
    //vaxia_import_items('./vaxia_convert/items.xml');
    drupal_set_message('Items have been converted.');
  }
  // Make npcs from npcs.xml
  if (file_exists('./vaxia_convert/npcs.xml')) {
    vaxia_import_npcs('./vaxia_convert/npcs.xml');
    drupal_set_message('NPCs have been converted.');
  }
  drupal_set_message('TASK: All has been converted. TURN THIS MODULE OFF.');
}

function vaxia_read_xml($file) {
  $results = simplexml_load_file($file, 'SimpleXMLElement', LIBXML_NOCDATA);
  $records = array();
  foreach ($results->children() as $child) {
    $record = array();
    foreach ($child->children() as $child_two) {
      $child_value = (array) $child_two;
      $record[ $child_two->getName() ] = $child_value[0];
    }
    $records[] = $record;
  }
  return $records;
}

function vaxia_import_users($file) {
  $results = 0;
  $records = vaxia_read_xml($file);
  $role_set = array_flip(user_roles(TRUE));
  foreach ($records as $record) {
    if (isset($record['playerName']) && !empty($record['playerName'])) {
      $roles = array();
      // Check player, player mature.
      $roles[$role_set['player']] = 'player';
      if (isset($record['matureRP']) && $record['matureRP'] != 'No') {
        $roles[$role_set['player-mature']] = 'player-mature';
      }
      // Determine user roles by XLM values.
      $user = array(
        'name' => $record['playerName'],
        'pass' => user_password(), // Random password.
        'mail' => isset($record['email']) ? $record['email'] : 'vaxia+' . $record['playerName'] . '@example.com',
        'init' => isset($record['email']) ? $record['email'] : 'vaxia+' . $record['playerName'] . '@example.com',
        'status' => 1,
        'access' => REQUEST_TIME,
        'roles' => $roles,
      );
      $account = user_save((object) $user);
      $results++;
    }
  }
  drupal_set_message($results . ' users imported.');
  drupal_set_message('TASK: Art, character, item evaulators should be reactived by hand.');
  drupal_set_message('TASK: Assistant storytellers and storytellers should be reactived by hand.');
}

function vaxia_import_art($file) {
  $records = vaxia_read_xml($file);
  $results = 0;
  foreach ($records as $record) {
    if (isset($record['name']) && !empty($record['name']) && isset($record['path']) && !empty($record['path'])) {
      $node = new stdClass();
      $node->type = 'artwork';
      node_object_prepare($node);
      $node->title = $record['name'];
      $user = user_load_by_name($record['player']);
      $node->uid = isset($user->uid) ? $user->uid : 1;
      $node->language = LANGUAGE_NONE;
      $node->field_artwork_credit[$node->language][0]['value'] = isset($record['copyright']) ? $record['copyright'] : '';
      $node->field_website[$node->language][0]['value'] =  isset($record['url']) ? $record['url'] : '';
      if (is_file($record['path'])) {
        $image = file_get_contents($record['path']);
        $file = file_save_data($image, 'public://' . $record['name'], FILE_EXISTS_RENAME);
        $node->field_artwork_image[$node->language][0] = (array) $file;
        node_save($node);
        $results++;
      }
    }
  }
  drupal_set_message($results . ' arts imported.');
}

function vaxia_import_rooms($file) {
  $records = vaxia_read_xml($file);
  $results = 0;
  foreach ($records as $record) {
    if (isset($record['name']) && !empty($record['name'])) {
      $node = new stdClass();
      $node->type = 'rpg_chatroom';
      node_object_prepare($node);
      $node->title = $record['name'];
      $node->uid = 1;
      $node->language = LANGUAGE_NONE;
      $node->body[$node->language][0]['value'] = isset($record['locDescription']) ? $record['locDescription'] : '';
      $node->body[$node->language][0]['summary'] = text_summary(isset($record['locDescription']) ? $record['locDescription'] : '');
      $node->body[$node->language][0]['format']  = 'filtered_html';
      $parent = vaxia_convert_get_nid(isset($record['parent']) ? $record['parent'] : '');
      if ($parent) {
        $node->field_parent[$node->language][0]['nid'] = $parent;
      }
      $image = vaxia_convert_get_nid(isset($record['image']) ? $record['image'] : '');
      if ($image) {
        $node->field_artwork[$node->language][0]['nid'] = $image;
      }
      node_save($node);
      $results++;
    }
  }
  drupal_set_message($results . ' rooms imported.');
}

function vaxia_import_characters($file) {
  $records = vaxia_read_xml($file);
  $results = 0;
  foreach ($records as $record) {
    if (isset($record['character']) && !empty($record['character'])) {
      $node = new stdClass();
      $node->type = 'character_sheet';
      node_object_prepare($node);
      $node->title = $record['character'];
      $user = user_load_by_name($record['player']);
      $node->uid = isset($user->uid) ? $user->uid : 1;
      $node->language = LANGUAGE_NONE;
      $image = vaxia_convert_get_nid(isset($record['image']) ? $record['image'] : '');
      if ($image) {
        $node->field_artwork[$node->language][0]['nid'] = $image;
      }
      $node->field_age_app[$node->language][0]['value'] = isset($record['charAppAge']) ? $record['charAppAge'] : '';
      $node->field_age_real[$node->language][0]['value'] = isset($record['charAge']) ? $record['charAge'] : '';
      $node->field_economy[$node->language][0]['value'] = isset($record['ecolevel']) ? $record['ecolevel'] : 0;
      $node->field_race[$node->language][0]['value'] = isset($record['charRace']) ? $record['charRace'] : 'human';
      $realm = 'vaxia';
      if (isset($record['sirianCert']) && $record['sirianCert'] == 'Yes') {
        $realm = 'sirian';
      }
      $node->field_realm[$node->language][0]['value'] = $realm;
      $node->field_sex[$node->language][0]['value'] = isset($record['charSex']) ? $record['charSex'] : 'none';
  
      $node->field_background[$node->language][0]['value'] = isset($record['background']) ? $record['background'] : '';
      $node->field_description[$node->language][0]['value'] = isset($record['description']) ? $record['description'] : '';
      $node->field_occupation[$node->language][0]['value'] = isset($record['ocupation']) ? $record['ocupation'] : '';
      $node->field_personality[$node->language][0]['value'] = isset($record['personality']) ? $record['personality'] : '';
  
      $node->field_player_notes[$node->language][0]['value'] = isset($record['playerNotes']) ? $record['playerNotes'] : '';
      $node->field_sh_notes[$node->language][0]['value'] = isset($record['shNotes']) ? $record['shNotes'] : '';
  
      $node->field_property[$node->language][0]['value'] = isset($record['playerProperty']) ? $record['playerProperty'] : '';
      //$node->field_items[$node->language][0]['value'] = $record['KEYNAME'];
      // Leave items empty, players will have to reassign them to characters.
  
      $node->field_agility[$node->language][0]['value'] = isset($record['agi']) ? $record['agi'] : 0;
      $node->field_appearance[$node->language][0]['value'] = isset($record['bty']) ? $record['bty'] : 0;
      $node->field_awareness[$node->language][0]['value'] = isset($record['awa']) ? $record['awa'] : 0;
      $node->field_charisma[$node->language][0]['value'] = isset($record['cha']) ? $record['cha'] : 0;
      $node->field_constitution[$node->language][0]['value'] = isset($record['con']) ? $record['con'] : 0;
      $node->field_dexterity[$node->language][0]['value'] = isset($record['dex']) ? $record['dex'] : 0;
      $node->field_endurance[$node->language][0]['value'] = isset($record['end']) ? $record['end'] : 0;
      $node->field_health[$node->language][0]['value'] = isset($record['hlt']) ? $record['hlt'] : 0;
      $node->field_intelligence[$node->language][0]['value'] = isset($record['int']) ? $record['int'] : 0;
      $node->field_life[$node->language][0]['value'] = isset($record['lfe']) ? $record['lfe'] : 0;
      $node->field_presence[$node->language][0]['value'] = isset($record['pre']) ? $record['pre'] : 0;
      $node->field_reflexes[$node->language][0]['value'] = isset($record['roa']) ? $record['roa'] : 0;
      $node->field_spirituality[$node->language][0]['value'] = isset($record['spi']) ? $record['spi'] : 0;
      $node->field_strength[$node->language][0]['value'] = isset($record['str']) ? $record['str'] : 0;
  
      // And save the character.
      node_save($node);
      $results++;

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
        if (count($values) == 3) { //TODO track down the error here:
        /*
        DatabaseTransactionNoActiveException: in DatabaseConnection->rollback() (line 1016 of /Users/cg176010/Localhost/vaxia/includes/database/database.inc).
        */
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
      node_save($node);

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
      _character_sheet_update_xp($node->nid, isset($record['charXps']) ? $record['charXps'] : 0, 'xp'); // Available unlocked xp.
      // Spent = zero, so it doesn't matter if they have prior XP spent, this is what's available.
      _character_sheet_update_xp($node->nid, isset($record['charBxps']) ? $record['charBxps'] : 0, 'bxp');
      _character_sheet_update_xp($node->nid, isset($record['charRxps']) ? $record['charRxps'] : 0, 'rxp');
      _character_sheet_update_xp($node->nid, isset($record['charKxps']) ? $record['charKxps'] : 0, 'kxp');
      // And this is what is in the system - flat - for their calculations henceforth.
      _character_sheet_update_xp($node->nid, isset($record['invest']) ? $record['invest'] : 0, 'invest_silver');
      _character_sheet_update_xp($node->nid, isset($record['silver']) ? $record['silver'] : 0, 'silver');
      return;
    }
  }
  drupal_set_message($results . ' characters imported.');
}

function vaxia_import_items($file) {
  $records = vaxia_read_xml($file);
  $results = 0;
  foreach ($records as $record) {
    if (isset($record['USEitemName']) && !empty($record['USEitemName'])) {
      $node = new stdClass();
      $node->type = 'items';
      node_object_prepare($node);
      $node->title = $record['USEitemName'];
      $user = user_load_by_name($record['player']);
      $node->uid = isset($user->uid) ? $user->uid : 1;
      $node->language = LANGUAGE_NONE;
      $node->field_item_desc[$node->language][0]['value'] = isset($record['NEWitemDescription']) ? $record['NEWitemDescription'] : '';
      $node->field_item_history[$node->language][0]['value'] = '';
      $node->field_item_notes[$node->language][0]['value'] = '';
      if (isset($record['MODitemValue']) && is_numeric($record['MODitemValue'])) {
        $node->field_item_value[$node->language][0]['value'] = $record['MODitemValue'] < 500000? $record['MODitemValue'] : 500000;
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
        if (isset($record[$key]) && is_numeric($record[$key])) {
          $node->{$map}[$node->language][0]['value'] = $record[$key] < 500000? $record[$key] : 500000;
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
        if (isset($record[$key]) && is_numeric($record[$key])) {
          $node->{$map}[$node->language][0]['value'] = $record[$key] < 500000? $record[$key] : 500000;
        }
        else {
          $node->{$map}[$node->language][0]['value'] = 0;
        }
      }
      // Set skill modifier.
      if (isset($record['MODski']) && is_numeric($record['MODski'])) {
        $node->field_item_skill[$node->language][0]['value'] = $record['MODski'] < 500000? $record['MODski'] : 500000;
      }
      else {
        $node->field_item_skill[$node->language][0]['value'] = 0;
      }
      // Set skill required.
      if (isset($record['MODreqski']) && is_numeric($record['MODreqski'])) {
        $node->field_item_required_skill[$node->language][0]['value'] = $record['MODreqski'] < 500000? $record['MODreqski'] : 500000;
      }
      else {
        $node->field_item_required_skill[$node->language][0]['value'] = 0;
      }
      // Save if we have an item. (Some item records are incomplete.)
      node_save($node);
      $results++;
    }
  }
  drupal_set_message($results . ' items imported.');
}

function vaxia_import_npcs($file) {
  $records = vaxia_read_xml($file);
  $results = 0;
  foreach ($records as $record) {
    if (isset($record['name']) && !empty($record['name'])) {
      $node = new stdClass();
      $node->type = 'npc';
      node_object_prepare($node);
      $node->title = $record['name'];
      $user = user_load_by_name($record['player']);
      $node->uid = isset($user->uid) ? $user->uid : 1;
      $node->language = LANGUAGE_NONE;
      $node->body[$node->language][0]['value'] = isset($record['description']) ? $record['description'] : '';
      $node->body[$node->language][0]['summary'] = text_summary(isset($record['description']) ? $record['description'] : '');
      $node->body[$node->language][0]['format']  = 'filtered_html';
      $image = vaxia_convert_get_nid($record['image']);
      if ($image) {
        $node->field_artwork[$node->language][0]['value'] = $image;
      }
      $node->field_age_real[$node->language][0]['value'] = isset($record['age']) ? $record['age'] : '';
      $node->field_age_app[$node->language][0]['value'] = isset($record['AppAge']) ? $record['AppAge'] : '';
      $node->field_race[$node->language][0]['value'] = isset($record['race']) ? $record['race'] : '';
      $node->field_sex[$node->language][0]['value'] =  isset($record['sex']) ? $record['sex'] : '';
      $node->field_last_name[$node->language][0]['value'] = isset($record['last']) ? $record['last'] : '';
      $node->field_sex[$node->language][0]['value'] =  isset($record['titles']) ? $record['titles'] : '';
      $node->field_sh_notes[$node->language][0]['value'] = isset($record['shNotes']) ? $record['shNotes'] : '';
      node_save($node);
      $results++;
    }
  }
  drupal_set_message($results . ' npcs imported.');
}

function vaxia_convert_get_nid($title) {
  $nid = FALSE;
  if (isset($title) && !empty($title)) {
    $results = db_query('SELECT nid FROM {node} WHERE title LIKE :title', array(':title' => $title));
    foreach ($results as $result) {
      return $result->nid;
    }
  }
  return $nid;
}
