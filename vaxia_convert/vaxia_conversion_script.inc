<?php
/**
 * This script converts Vaxia formatted files into CSV files for import.
 * Users, art, items, npcs.
 *
 * Doing it this way allows us to generate data, check it, and then update
 * via feeds into the new system.
 */

function vaxia_convert_main($ROOT) {
  set_time_limit(60);
  $VAXIA_CHAR_PATH = $ROOT.'/vaxia/characters/active';
  $VAXIA_OPENNPC_PATH = $ROOT.'/vaxia/world';
  $VAXIA_ROOM_PATH = $ROOT.'/vaxia/enter';
  $art_file = './vaxia_convert/art.xml';
  if (!is_dir('./vaxia_convert')) {
    mkdir('./vaxia_convert', 0777, TRUE);
  }
  write_record_open_file($art_file);
  get_users($ROOT, $VAXIA_CHAR_PATH, $art_file);
  set_time_limit(240);
  get_characters($ROOT, $VAXIA_CHAR_PATH, $art_file);
  set_time_limit(60);
  get_items($ROOT, $VAXIA_CHAR_PATH);
  set_time_limit(60);
  get_npcs($ROOT, $VAXIA_CHAR_PATH, $VAXIA_OPENNPC_PATH);
  set_time_limit(180);
  get_rooms($ROOT, $VAXIA_ROOM_PATH, $art_file);
  write_record_close_file($art_file);
  return TRUE;
}

function get_directories($path) {
  if (is_dir($path) && is_readable($path)) {
    $results = array();
    $children = scandir($path);
    foreach ($children as $child) {
      if (is_dir($path .'/'. $child) && is_readable($path .'/'. $child) && $child != '.' && $child != '..') {
        $results[] = $child;
      }
    }
    return $results;
  }
  return FALSE;
}

function write_record_open_file($path) {
  $fp = fopen($path, 'a'); // Append, write one per row.
  fwrite($fp, "<?xml version='1.0' encoding='ISO-8859-1'?>\n");
  fwrite($fp, "<records>\n");
  fclose($fp);
}

function write_record_close_file($path) {
  $fp = fopen($path, 'a'); // Append, write one per row.
  fwrite($fp, "</records>");
  fclose($fp);
}

function write_record_to_file($path, $record) {
  $fp = fopen($path, 'a'); // Append, write one per row.
  fwrite($fp, "  <record>\n");
  $xml = _write_record_to_file($record);
  fwrite($fp, $xml);  
  fwrite($fp, "  </record>\n");
  fclose($fp);
}

// RECURSIVE.
function _write_record_to_file($record, $depth = '    ') {
  $xml = '';
  foreach ($record as $key => $field) {
    if (isset($key) && !empty($key) && strpos($key, ' ') === FALSE && strpos($key, '^') === FALSE && !empty($field)) {
      if (!is_array($field)) {
        $xml .= $depth."<".trim($key)."><![CDATA[".trim($field)."]]></".trim($key).">\n";
      }
      else {
        $xml .= $depth."<".trim($key).">\n";
        $xml .= _write_record_to_file($field, $depth . '  ');
        $xml .= $depth."</".trim($key).">\n";
      }
    }
  }
  return $xml;
}

function vaxia_file_scan_directory($dir, $mask) {
  $nomask = array('.', '..');
  $files = array();
  if (is_dir($dir) && $handle = opendir($dir)) {
    while (FALSE !== ($file = readdir($handle))) {
      if (!in_array($file, $nomask) && $file[0] != '.') {
        if (@ereg($mask, $file)) {
          $files[] = basename($file);
        }
      }
    }
    closedir($handle);
  }
  return $files;
}

function read_vaxia_file($ROOT, $file) {
  $file = str_replace('//', '/', $file);
  $record = array();
  if (is_file($file)) {
    $fp = fopen($file, 'r');
    $path = '';
    if ($fp) {
      while (($line = fgets($fp, 4096)) !== false) {
        // Check for values.
        $pattern = '/^\[(.*)\](.*)/';
        preg_match($pattern, $line, $matches);
        if (count($matches) >= 3) {
          $matches[1] = trim($matches[1]);
          $matches[2] = trim($matches[2]);
          if (strpos($matches[2], '^') === 0) { // If string starts with caret remove it.
            $matches[2] = substr($matches[2], 1);
          }
          $record[ $matches[1] ] = $matches[2];
        }
        // Check for PATH: and read and merge the values.
        $pattern = '/^PATH:(.*)/';
        $matches = array();
        preg_match($pattern, $line, $matches);
        if (count($matches) >= 2) {
          $path = trim($matches[1]);
        }
      }
      if (!feof($fp)) {
        echo "Error: unexpected fgets() fail\n";
      }
      fclose($fp);
    }
    if (!empty($path)) {
      $new_record = read_vaxia_file($ROOT, $ROOT . '/' . $path);
      // Add values.
      $record = array_merge($new_record, $record);
    }
  }
  return $record;
}

function move_vaxia_art_file($path, $subpath) {
  $VAXIA_ART_PATH = './vaxia_convert/vaxia_art';
  $new_path = $path;
  if (is_file($path)) {
    if (!is_dir($VAXIA_ART_PATH.'/'.$subpath)) {
      mkdir($VAXIA_ART_PATH.'/'.$subpath, 0777, TRUE);
    }
    $new_path = $VAXIA_ART_PATH.'/'.$subpath.'/'.basename($path);
    copy($path, $new_path);
  }
  return $new_path;
}

function get_users($ROOT, $path, $art_file) {
  $file = './vaxia_convert/users.xml';
  write_record_open_file($file);
  $results = 0;
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Get user art.
      $files = vaxia_file_scan_directory($path . '/' . $letter_dir . '/' . $user_dir, '.*.item');
      foreach ($files as $more_data) {
        if (!(strpos($more_data, 'player') === 0) ) {
          // Read art in the user dir.
          $art = array();
          $art['player'] = ucfirst($user_dir);
          $art['character'] = $art['room'] = '';
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($ROOT, $path . '/' . $letter_dir . '/' . $user_dir . '/' . $more_data);
          if (isset($data['name'])) {
            $data['path'] = move_vaxia_art_file($path . '/' . $letter_dir . '/' . $user_dir . '/' . $data['name'], 'users');
            $data['name'] = basename($data['path']);
            $art = array_merge($art, $data);
            // Now that we have a user, determine the rest of the settings and permissions.
            $record = $art;
            write_record_to_file($art_file, $record);
          }
        }
      }
      // Read each dir in dir
      $user = array();
      $user['name'] = $user_dir; // User name.
      $more_data = vaxia_file_scan_directory($path . '/' . $letter_dir . '/' . $user_dir, 'player-.*.item');
      if (count($more_data) == 1) {
        // Given permissions and email in dir / dir / name-pass.item
        $data = read_vaxia_file($ROOT, $path . '/' . $letter_dir . '/' . $user_dir . '/' . $more_data[0]);
        // No 'currently set' images show up.
        $user = array_merge($user, $data);
      }
      // Now that we have a user, determine the rest of the settings and permissions.
      $record = $user;
      write_record_to_file($file, $record);
      $results++;
    }
  }
  drupal_set_message($results . ' users converted.');
  write_record_close_file($file);
}

function get_characters($ROOT, $path, $art_file) {
  $file = './vaxia_convert/characters.xml';
  write_record_open_file($file);
  $results = 0;
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Read each dir in dir
      foreach (get_directories($path . '/' . $letter_dir . '/' . $user_dir) as $char_dir) {
        // Read the art dir.
        $art_dir = $path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/art';
        $files = vaxia_file_scan_directory($art_dir, '.*.item');
        foreach ($files as $more_data) {
          $art = array();
          $art['player'] = ucfirst($user_dir);
          $art['character'] = ucfirst($char_dir);
          $art['room'] = '';
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($ROOT, $path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/art/' . $more_data);
          if (isset($data['name'])) {
            $data['path'] = move_vaxia_art_file($path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/art/' . $data['name'], 'characters');
            $data['name'] = basename($data['path']);
            $art = array_merge($art, $data);
            // Now that we have a user, determine the rest of the settings and permissions.
            $record = $art;
            write_record_to_file($art_file, $record);
          }
        }
        // Get the character data too.
        $character = array();
        $character['player'] = ucfirst($user_dir);
        $character['character'] = ucfirst($char_dir);
        $more_data = vaxia_file_scan_directory($path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir, '.*-'.$char_dir.'.item');
        if (count($more_data) == 1) {
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($ROOT, $path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir. '/' . $more_data[0]);
          if (isset($data['actualPic'])) {
            $data['image'] = basename($data['actualPic']); // Current image.
            $character = array_merge($character, $data);
          }
        }
        // Now that we have a user, determine the rest of the settings and permissions.
        $record = $character;
        // TODO map the skill set.
        write_record_to_file($file, $record);
        $results++;
      }
    }
  }
  drupal_set_message($results . ' characters converted.');
  write_record_close_file($file);
}

function get_items($ROOT, $path) {// For each dir in root.
  $file = './vaxia_convert/items.xml';
  write_record_open_file($file);
  $results = 0;
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Read each dir in dir
      foreach (get_directories($path . '/' . $letter_dir . '/' . $user_dir) as $char_dir) {
        // Read the art dir.
        $item_dir = $path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/property';
        $files = vaxia_file_scan_directory($item_dir, '.*.item');
        foreach ($files as $more_data) {
          $item = array(); //TODO map item MODS to an attribute
          $item['player'] = ucfirst($user_dir);
          $item['character'] = ucfirst($char_dir);
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($ROOT, $path . '/' . $letter_dir . '/' . $user_dir  . '/' . $char_dir . '/property/' . $more_data);
          $item = array_merge($item, $data);
          // Now that we have a user, determine the rest of the settings and permissions.
          $record = $item;
          write_record_to_file($file, $record);
          $results++;
        }
      }
    }
  }
  drupal_set_message($results . ' items converted.');
  write_record_close_file($file);
}

function get_npcs($ROOT, $path, $open_path) {
  $file = './vaxia_convert/npcs.xml';
  $sh_file = './vaxia_convert/shnpcs.xml';
  $open_file = './vaxia_convert/opennpcs.xml';
  write_record_open_file($file);
  write_record_open_file($sh_file);
  write_record_open_file($open_file);
  $results = 0;
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Read each dir in dir
      foreach (get_directories($path . '/' . $letter_dir . '/' . $user_dir) as $char_dir) {
        // Read the art dir.
        $npc_dir = $path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/dependants';
        $files = vaxia_file_scan_directory($npc_dir, '.*.item');
        foreach ($files as $more_data) {
          $npc = array();
          $npc['player'] = ucfirst($user_dir);
          $npc['character'] = ucfirst($char_dir);
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($ROOT, $path . '/' . $letter_dir . '/' . $user_dir  . '/' . $char_dir . '/dependants/' . $more_data);
          $npc = array_merge($npc, $data);
          // Now that we have a user, determine the rest of the settings and permissions.
          $record = $npc;
          write_record_to_file($file, $record);
          $results++;
        }
        // Get SH NPCS        
        foreach (get_directories($path . '/' . $letter_dir . '/' . $user_dir . '/dependants') as $shnpc_dir) {
          // Read the art dir.
          $shnpc = array();
          $shnpc['player'] = ucfirst($user_dir);
          $shnpc['character'] = ucfirst($shnpc_dir);
          $shnpc['isNPC'] = 1;
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($ROOT, $path . '/' . $letter_dir . '/' . $user_dir . '/dependants/' . $shnpc_dir . '/' . $shnpc_dir . '.item');
          $shnpc = array_merge($shnpc, $data);
          // Now that we have a user, determine the rest of the settings and permissions.
          $record = $shnpc;
          write_record_to_file($sh_file, $record);
          $results++;
        }
      }
    }
  }
  // Get open NPCs
  foreach (get_directories($open_path) as $world) {
    foreach (get_directories($open_path . '/' . $world . '/npcs/') as $letter_dir) {
      // Read each dir in dir
      foreach (get_directories($open_path . '/' . $world . '/npcs/' . $letter_dir) as $open_dir) {
        // Read the art dir.
        $opennpc = array();
        $opennpc['player'] = ucfirst('admin');
        $opennpc['character'] = ucfirst($open_dir);
        $opennpc['realm'] = ucfirst($world);
        $opennpc['isNPC'] = 1;
        $opennpc['isOpenNPC'] = 1;
        // Given permissions and email in dir / dir / name-pass.item
        $data = read_vaxia_file($open_path . '/' . $world . '/npcs/' . $letter_dir . '/' . $open_dir . '/' . $open_dir . '.item');
        $opennpc = array_merge($opennpc, $data);
        // Now that we have a user, determine the rest of the settings and permissions.
        $record = $opennpc;
        write_record_to_file($open_file, $record);
        $results++;
      }
    }
  }
  drupal_set_message($results . ' npcs converted.');
  write_record_close_file($file);
  write_record_close_file($sh_file);
  write_record_close_file($open_file);
}

function get_rooms($ROOT, $path, $art_file) {
  $worlds = array('/_vaxian_world' => 'vaxia', '/sirian_solar_system' => 'sirian');
  $results = 0;
  foreach ($worlds as $world => $category) {
    $file = './vaxia_convert/'.$category.'-rooms.xml';
    write_record_open_file($file);
    $world_path = $path.$world;
    $results = recursive_get_rooms($ROOT, $world_path, '', $category, $file, $art_file, $results);
    write_record_close_file($file);
  }
  drupal_set_message($results . ' rooms converted.');
}

// RECURSIVE EFFORT.
function recursive_get_rooms($ROOT, $path, $parent, $category, $file, $art_file, $results) {
  if (is_dir($path)) {
    foreach (get_directories($path) as $this_path) {
      // Read art files.
      $files = vaxia_file_scan_directory($path .'/'. $this_path, '.*.item');
      foreach ($files as $more_data) {
        if ($more_data != 'item.item') {
          $art = array();
          $art['player'] = $art['character'] = '';
          $art['room'] = ucwords(str_replace('_', ' ', $this_path));
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($ROOT, $path .'/'. $this_path . '/' . $more_data);
          if (isset($data['name'])) {
            $data['path'] = move_vaxia_art_file($path .'/'. $this_path . '/' . $data['name'], 'rooms');
            $data['name'] = basename($data['path']);
            $art = array_merge($art, $data);
            // Now that we have a user, determine the rest of the settings and permissions.
            $record = $art;
            write_record_to_file($art_file, $record);
          }
        }
      }
      // Read room file.
      $room['name'] = ucwords(str_replace('_', ' ', $this_path));
      $room['realm'] = $category;
      $room['parent'] = $parent;
      $data = read_vaxia_file($ROOT, $path .'/'. $this_path . '/item.item');
      if (isset($data['locImage'])) {
        $data['image'] = basename($data['locImage']); // Current image.
      }
      $record = array_merge($room, $data);
      write_record_to_file($file, $record);
      $results++;
      // And recurse down.
      $results = recursive_get_rooms($path .'/'. $this_path, $room['name'], $category, $file, $art_file, $results);
    }
  }
  return $results;
}
