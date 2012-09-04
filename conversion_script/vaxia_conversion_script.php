<?php
/**
 * This script converts Vaxia formatted files into CSV files for import.
 * Users, art, items, npcs.
 *
 * Doing it this way allows us to generate data, check it, and then update
 * via feeds into the new system.
 */

define('VAXIA_CHAR_PATH', './vaxia_docroot/vaxia/characters/active');
define('VAXIA_ROOM_PATH', './vaxia_docroot/vaxia/enter');

// Execute.
main();

function main() {
  get_users(VAXIA_CHAR_PATH);
  get_characters(VAXIA_CHAR_PATH);
  get_art(VAXIA_CHAR_PATH);
  get_items(VAXIA_CHAR_PATH);
  get_npcs(VAXIA_CHAR_PATH);
  get_rooms(VAXIA_ROOM_PATH);
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
    if (!empty($key) && !empty($field)) {
      if (!is_array($field)) {
        $xml .= $depth."<".trim($key).">".trim($field)."</".trim($key).">\n";
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

function file_scan_directory($dir, $mask) {
  $nomask = array('.', '..');
  $key = (in_array($key, array('filename', 'basename', 'name')) ? $key : 'filename');
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

function read_vaxia_file($file) {
  $record = array();
  $fp = fopen($file, 'r');
  if ($fp) {
    while (($line = fgets($fp, 4096)) !== false) {
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
    }
    if (!feof($fp)) {
      echo "Error: unexpected fgets() fail\n";
    }
    fclose($fp);
  }
  return $record;
}

function get_users($path) {
  $file = './users.xml';
  write_record_open_file($file);
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Read each dir in dir
      $user = array();
      $user['name'] = $user_dir; // User name.
      $more_data = file_scan_directory($path . '/' . $letter_dir . '/' . $user_dir, 'player-.*.item');
      if (count($more_data) == 1) {
        // Given permissions and email in dir / dir / name-pass.item
        $data = read_vaxia_file($path . '/' . $letter_dir . '/' . $user_dir . '/' . $more_data[0]);
        $user = array_merge($user, $data);
      }
      // Now that we have a user, determine the rest of the settings and permissions.
      $record = $user;
      write_record_to_file($file, $record);
    }
  }
  write_record_close_file($file);
}

function get_characters($path) {
  $file = './characters.xml';
  write_record_open_file($file);
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Read each dir in dir
      foreach (get_directories($path . '/' . $letter_dir . '/' . $user_dir) as $char_dir) {
        $character = array();
        $character['player'] = ucfirst($user_dir);
        $character['character'] = ucfirst($char_dir);
        $more_data = file_scan_directory($path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir, '.*-'.$char_dir.'.item');
        if (count($more_data) == 1) {
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir. '/' . $more_data[0]);
          $character = array_merge($character, $data);
        }
        // Now that we have a user, determine the rest of the settings and permissions.
        $record = $character;
        // TODO map the skill set.
        write_record_to_file($file, $record);
      }
    }
  }
  write_record_close_file($file);
}

function get_art($path) {
// TODO add art along the whole way.
  $file = './art.xml';
  write_record_open_file($file);
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Read each dir in dir
      foreach (get_directories($path . '/' . $letter_dir . '/' . $user_dir) as $char_dir) {
        // Read the art dir.
        $art_dir = $path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/art';
        $files = file_scan_directory($art_dir, '.*.item');
        foreach ($files as $more_data) {
          $art = array();//TODO map art to a image file and map that to a open HTML path for feeds to pick up.
          $art['player'] = ucfirst($user_dir);
          $art['character'] = ucfirst($char_dir);
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/art/' . $more_data);
          $art = array_merge($art, $data);
          // Now that we have a user, determine the rest of the settings and permissions.
          $record = $art;
          write_record_to_file($file, $record);
        }
      }
    }
  }
  write_record_close_file($file);
}

function get_items($path) {// For each dir in root.
  $file = './items.xml';
  write_record_open_file($file);
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Read each dir in dir
      foreach (get_directories($path . '/' . $letter_dir . '/' . $user_dir) as $char_dir) {
        // Read the art dir.
        $item_dir = $path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/property';
        $files = file_scan_directory($item_dir, '.*.item');
        foreach ($files as $more_data) {
          $item = array(); //TODO map item MODS to an attribute
          $item['player'] = ucfirst($user_dir);
          $item['character'] = ucfirst($char_dir);
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($path . '/' . $letter_dir . '/' . $user_dir  . '/' . $char_dir . '/property/' . $more_data);
          $item = array_merge($item, $data);
          // Now that we have a user, determine the rest of the settings and permissions.
          $record = $item;
          write_record_to_file($file, $record);
        }
      }
    }
  }
  write_record_close_file($file);
}

function get_npcs($path) {
  $file = './npcs.xml';
  write_record_open_file($file);
  foreach (get_directories($path) as $letter_dir) {
    // We now have the alphabetical.
    foreach (get_directories($path . '/' . $letter_dir) as $user_dir) {
      // Read each dir in dir
      foreach (get_directories($path . '/' . $letter_dir . '/' . $user_dir) as $char_dir) {
        // Read the art dir.
        $npc_dir = $path . '/' . $letter_dir . '/' . $user_dir . '/' . $char_dir . '/dependants';
        $files = file_scan_directory($npc_dir, '.*.item');
        foreach ($files as $more_data) {
          $npc = array();
          $npc['player'] = ucfirst($user_dir);
          $npc['character'] = ucfirst($char_dir);
          // Given permissions and email in dir / dir / name-pass.item
          $data = read_vaxia_file($path . '/' . $letter_dir . '/' . $user_dir  . '/' . $char_dir . '/dependants/' . $more_data);
          $npc = array_merge($npc, $data);
          // Now that we have a user, determine the rest of the settings and permissions.
          $record = $npc;
          write_record_to_file($file, $record);
        }
      }
    }
  }
  write_record_close_file($file);
}

function get_rooms($path) {
  $worlds = array('/_vaxian_world' => 'vaxia', '/sirian_solar_system' => 'sirian');
  foreach ($worlds as $world => $category) {
    $file = './'.$category.'-rooms.xml';
    write_record_open_file($file);
    $path = $path.$world;
    recursive_get_rooms($path, $category, $file);
    write_record_close_file($file);
  }
}

// RECURSIVE EFFORT.
function recursive_get_rooms($path, $category, $file) {
  if (is_dir($path)) {
    foreach (get_directories($path) as $this_path) {
      $room['name'] = $this_path;
      $room['realm'] = $category;
      $data = read_vaxia_file($path .'/'. $this_path . '/item.item');
      $record = array_merge($room, $data);
      write_record_to_file($file, $record);
      // And recurse down.
      recursive_get_rooms($path .'/'. $this_path, $category, $file);
    }
  }
}
?>
