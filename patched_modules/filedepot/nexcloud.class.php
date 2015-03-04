<?php

/**
 * @file
 * nexcloud.class.php
 * Tag Cloud class for the filedepot module
 */

class nexcloud {
  public $_tagwords;
  public $_tagitems;
  public $_tagmetrics;
  public $_filter;
  public $_newtags;
  public $_activetags; // Active search tags - don't show in tag cloud

  public $_type;
  public $_fontmultiplier = 160; // Used as a multiplier in displaycloud() function - Increase to see a wider range of font sizes

  public $_maxclouditems = 200;
  public $_allusers = 1; // Role Id that includes all users (including anonymous)


  function __construct() {
    global $user;

    if (db_query("SELECT COUNT(id) FROM {nextag_words}")->fetchField() < 30) {
      $this->_fontmultiplier = 100;
    }

    if ($user->uid > 0) {
      $this->_uid = $user->uid;
    }
    else {
      $this->_uid = 0;
    }
  }

  /* Function added so I could isolate out the filtering logic.
   * May not need now for Drupal - but keeping it for now
   * Returns filtered tagword in lowercase
   */
  private function filtertag($tag, $dbmode = FALSE) {
    if ($dbmode) {
      return drupal_strtolower(strip_tags($tag));
    }
    else {
      return drupal_strtolower(strip_tags($tag));
    }
  }


  /* This function can be over-loaded or extended in a module specific class to return the item perms
   * We ony are concerned about view access
   * We only show tags with their relative metric (popularity) depending on users access - so tags in restricted folders do not appear.
   * If item is restricted to a group - then we need the assigned group id returned
   *
   * @param string $itemid    - id that identifies the plugin item, example fid for a file
   * @param boolean $sitewide - if set TRUE, then the call to perms will check site wide vs just for the user
   * @return array            - Return needs to be an associative array with the 3 permission related values
   *                            $A['group_id','perm_members','perm_anon');
   */
  function get_itemperms($fid, $sitewide = FALSE) {
    global $user;

    $perms = array(
      'groups' => array(),
      'roles' => array(),
    );
    $cid = db_query("SELECT cid FROM {filedepot_files} WHERE fid=:fid", array(':fid' => $fid))->fetchField();
    if ($cid > 0) {
      if (module_exists('og') AND module_exists('og_access')) {
        $groups = filedepot_og_get_user_groups();
        if (!empty($groups)) {
          $groupids = implode(',', array_values($groups));
          if (!empty($groupids) OR $sitewide === TRUE) {
            $sql = "SELECT permid from {filedepot_access} WHERE catid=:cid AND permtype='group' AND view = 1 AND permid > 0 ";
            $sql .= "AND permid in ($groupids) ";
            $query = db_query($sql, array(':cid' => $cid));
            if ($query) {
              while ($A = $query->fetchAssoc()) {
                $perms['groups'][] = $A['permid'];
              }
            }
          }
        }
      }
      // Determine all the roles the active user has and test for view permission.

      $roleids = implode(',', array_keys($user->roles));
      if (!empty($roleids) OR $sitewide === TRUE) {
        $sql = "SELECT permid from {filedepot_access} WHERE catid=:cid AND permtype='role' AND view = 1 AND permid > 0 ";
        if (!$sitewide) {
          $sql .= "AND permid in ($roleids) ";
        }
        $query = db_query($sql, array(':cid' => $cid));
        if ($query) {
          while ($A = $query->fetchAssoc()) {
            $perms['roles'][] = $A['permid'];
          }
        }
      }
    }
    return $perms;
  }

  // Return an array of tagids for the passed in comma separated list of tagwords


  private function get_tagids($tagwords) {

    if (!empty($tagwords)) {
      $tagwords = explode(',', $tagwords);
      // Build a comma separated list of tagwords that we can use in a SQL statements below

      $allTagWords = array();
      foreach ($tagwords as $word) {
        $tag = "'" . addslashes($word) . "'";
        $allTagWords[] = $tag;
      }
      $tagwords = implode(',', $allTagWords); // build a comma separated list of words


      if (!empty($tagwords)) {
        $query = db_query("SELECT id FROM {nextag_words} where tagword in ($tagwords)");
        $tagids = array();
        while ($A = $query->fetchAssoc()) {
          $tagids[] = $A['id'];
        }
        return $tagids;
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }

  /*
   * New item being tagged or new tag being added to an item.
   * The usage count for this tagword will be incremented by 1 - regardless of the number of roles or groups that have access to item.
   * @param string $itemid  - Item id, used to get the access permissions
   * @param array $tagids   - array of tag id's
   */
  private function add_accessmetrics($itemid, $tagids) {

    // Test that a valid array of tag id's is passed in

    if (is_array($tagids) AND count($tagids) > 0) {
      // Test that a valid item record exist
      if (db_query("SELECT count(itemid) FROM {nextag_items} WHERE type=:type AND itemid=:iid", array(':type' => $this->_type, ':iid' => $itemid))->fetchField() > 0) {
        // Get item permissions to determine what rights to use for tag metrics record
        $perms = $this->get_itemperms($itemid, true);

        // Add any new tags
        foreach ($tagids as $id) {
          if (!empty($id)) {
            // For each role or group with view access to this item - create or update the access metric record count.
            $haveGroupsToUpdate = count($perms['groups']) > 0;
            $haveRolesToUpdate = count($perms['roles']) > 0;
            if ($haveGroupsToUpdate OR $haveRolesToUpdate) {
              db_query("UPDATE {nextag_words} SET metric=metric+1 WHERE id=:id", array(':id' => $id));
              // use an array to handle the logic of whether to process groups, roles, or both
              // use the key to track the field to update and the value to track the values
              $permAccessMetric = array();
              if ($haveGroupsToUpdate) {
                $permAccessMetric['groupid'] = $perms['groups'];
              }
              if ($haveRolesToUpdate) {
                $permAccessMetric['roleid'] = $perms['roles'];
              }
              foreach ($permAccessMetric as $permKey => $permValue) {
                foreach ($permValue as $permid) {
                  if ($permid > 0) {
                    $sql = "SELECT count(tagid) FROM {nextag_metrics} WHERE tagid = :tid AND type = :type AND " . $permKey . " = :permkey";
                    if (db_query($sql, array(':tid' => $id, ':type' => $this->_type, ':permkey' => $permid))->fetchField() > 0) {
                      $sql  = "UPDATE {nextag_metrics} SET metric=metric+1, last_updated=:updated "
                                . "WHERE tagid=:tid AND type=:type AND " . $permKey . "=:permkey";
                      db_query($sql, array('updated' => time(), ':tid' => $id, ':type' => $this->_type, ':permkey' => $permid));
                    }
                    else {
                      $sql  = "INSERT INTO {nextag_metrics} (tagid,type," . $permKey . ",metric,last_updated) "
                                . "VALUES (:id, :type, :permkey, :alias, :time)";
                      db_query($sql, array(':id' => $id, ':type' => $this->_type, ':permkey' => $permid, ':alias' => 1, ':time' => time()));
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }


  /*
   * Method is called when removing a tag for an item.
   * Need to decrement it's metric and update related access records
   * @param string $itemid  - Item id, used to get the access permissions
   * @param array $tagids   - array of tag id's
   */
  private function remove_accessmetrics($itemid, $tagids) {

    // Test that a valid array of tag id's is passed in
    if (is_array($tagids) AND count($tagids) > 0) {
      // Test that a valid item record exist
      if (db_query("SELECT count(itemid) FROM {nextag_items} WHERE type=:type AND itemid=:item", array(':type' => $this->_type, ':item' => $itemid))->fetchField() > 0) {
        // Get item permissions to determine what rights to use for tag metrics record
        $perms = $this->get_itemperms($itemid, true);
        // Remove if required unused tag related records for this item
        foreach ($tagids as $id) {
          if (!empty($id)) {
            // For each role or group with view access to this item - decrement and if need delete the access metric record count.
            $haveGroupsToRemove = count($perms['groups']) > 0;
            $haveRolesToRemove = count($perms['roles']) > 0;
            if ($haveGroupsToRemove OR $haveRolesToRemove) {
              db_query("UPDATE {nextag_words} SET metric=metric-1 WHERE id=:id", array(':id' => $id));
              db_query("DELETE FROM {nextag_words} WHERE id=:id and metric=1", array(':id' => $id));
              // use an array to handle the logic of whether to process groups, roles, or both
              // use the key to track the field to update and the value to track the values
              $permAccessMetric = array();
              if ($haveGroupsToRemove) {
                $permAccessMetric['groupid'] = $perms['groups'];
              }
              if ($haveRolesToRemove) {
                $permAccessMetric['roleid'] = $perms['roles'];
              }
              foreach ($permAccessMetric as $permKey => $permValue) {
                foreach ($permValue as $permid) {
                  // Delete the tag metric access record if metric = 1 else decrement the metric count
                  db_query("DELETE FROM {nextag_metrics} WHERE tagid=:tid AND type=:type AND " . $permKey . "=:permkey AND metric=1", array(':tid' => $id, ':type' => $this->_type, ':permkey' => $permid));
                  $sql  = "UPDATE {nextag_metrics} SET metric=metric-1, last_updated=:time "
                            . "WHERE tagid=:tid AND type=:type AND " . $permKey . "=:permkey";
                  db_query($sql, array(':time' => time(), ':tid' => $id, ':type' => $this->_type, ':permkey' => $permid));
                }
              }
            }
          }
        }
      }
    }
  }


  /*
   * Used when item permissions are changed and we need to possibly remove or add some tag_metrics records
   * Each file identified by tagid of type 'filedepot' will have a record per permission assigned
   * So if the folder has 10 files with tags, 2 role permissions and a group permission - there will be 3 record per file
   * @param string $itemid  - file id, used to get the access permissions
   */
  function update_accessmetrics($itemid, $tagids = '') {

    if (empty($tagids)) { // Retrieve the tags field and convert into an array
      $tags = db_query("SELECT tags FROM {nextag_items} WHERE type=:type AND itemid=:item", array(':type' => $this->_type, ':item' => $itemid))->fetchField();
      if (!empty($tags)) {
        $tagids = explode(',', $tags);
      }
      else {
        $tagids = array();
      }
    }

    // Test that we now have a valid array of tag id's
    if (is_array($tagids) AND count($tagids) > 0) {
      // Test that a valid item record exist
      if (db_query("SELECT count(itemid) FROM {nextag_items} WHERE type=:type AND itemid=:item", array(':type' => $this->_type, ':item' => $itemid))->fetchField() > 0) {

        // Get item permissions to determine what rights to use for tag metrics record
        $perms = $this->get_itemperms($itemid, true);

        // For each role or group with view access to this item - create or update the access metric record count.
        $haveGroupsToUpdate = count($perms['groups']) > 0;
        $haveRolesToUpdate = count($perms['roles']) > 0;

        // For each tag word - we need to update (add new or remove any un-used tag metric associated with this permission and tagword
        $metricRecords = array(); // Will contain the processed metric records

        foreach ($tagids as $id) {
          if (!empty($id)) {
            if ($haveGroupsToUpdate OR $haveRolesToUpdate) {

              // An array to handle the logic of whether to process groups, roles, or both
              // Use the key to track the field to update and the value to track the values
              $permAccessMetric = array();
              if ($haveGroupsToUpdate) {
                $permAccessMetric['groupid'] = $perms['groups'];
              }
              if ($haveRolesToUpdate) {
                $permAccessMetric['roleid'] = $perms['roles'];
              }
              foreach ($permAccessMetric as $permKey => $permValue) {
                foreach ($permValue as $permid) {
                  if ($permid > 0) {
                    $result = db_query("SELECT id FROM {nextag_metrics} WHERE tagid = :tid AND type = :type AND :permkey = :permid", array(':tid' => $id, ':type' => $this->_type, ':permkey' => $permKey, 'permid' => $permid));
                    $numrecs = 0;
                    if ($result) {
                      foreach ($result as $rec) {
                        $numrecs++;
                        if (!in_array($rec->id, $metricRecords)) {
                          $metricRecords[] = $rec->id;
                        }
                      }
                    }
                    if ($numrecs == 0) {
                      $metric = db_query("SELECT metric FROM {nextag_words} WHERE id=:id",
                        array(':id' => $id))->fetchField();
                      $query = db_insert('nextag_metrics');
                      $query->fields(array('tagid', 'type', $permKey, 'metric', 'last_updated'));
                      $query->values(array(
                        'tagid' => $id,
                        'type' => $this->_type,
                        $permKey => $permid,
                        'metric' => $metric,
                        'last_updated' => time(),
                      ));
                      $newrecid = $query->execute();
                      $metricRecords[] = $newrecid;
                    }
                  }
                }
              }
            }
          }

          /* Now we should delete any metric records that are not in the processed $metricRecords array
           * They would be records for access permissions that were removed
           */
          if (count($metricRecords) > 0) {
            $recids = implode(',', $metricRecords);
            $query = db_delete('nextag_metrics')
              ->condition('tagid', $id, '=')
              ->condition('id', $metricRecords, 'NOT IN')
              ->execute();
          }
          else {
            db_query("DELETE FROM {nextag_metrics} WHERE tagid = :tid ", array(':tid' => $id));
          }

          // Delete any tagword records that are no longer used
          $result = db_query("SELECT id FROM {nextag_words} WHERE metric = 0");
          if ($result) {
            // Let's do one more test and make sure no items are using this tagword
            while ($A = $result->fetchAssoc()) {
              /* REGEX - search for id that is the first id or has a leading comma
               *  must then have a trailing , or be the end of the field
               */
              $sql = "SELECT itemid FROM {nextag_items} WHERE type='{$this->_type}' AND ";
              $sql .= "(tags LIKE '{$A['id']},%' OR tags LIKE '%,{$A['id']}' OR tags LIKE '%,{$A['id']},%' OR tags = '{$A['id']}') ";
              //$sql .= "tags REGEXP '(^|,){$A['id']}(,|$)' ";
              if (db_query($sql)->fetchField() == 0) {
                db_query("DELETE FROM {nextag_words} WHERE id = :id", array(':id' => $A['id']));
              }
            }

          }
        }

      }

    }
  }

  /* Update tag metrics for an existing item.
   * Should work for all modules - adding tags or updating tags
   * @param string $itemid       - Example file ID (fid) relates to itemid in the tagitems table
   * @param string $tagwords     - Single tagword or comma separated list of tagwords.
   *                               Tagwords can be unfilterd if passed in.
   *                               The set_newtags function will filter and prepare tags for DB insertion
   * @param boolean $sitewide    - if set TRUE, then the call to perms will check site wide vs just for the user
   */
  public function update_tags($itemid, $tagwords = '', $sitewide = FALSE) {

    if (!empty($tagwords)) {
      $this->set_newtags($tagwords);
    }

    $perms = $this->get_itemperms($itemid, $sitewide);
    if (count($perms['groups']) > 0 OR count($perms['roles']) > 0) {
      if (!empty($this->_newtags)) {
        // If item record does not yet exist - create it.

        if (db_query("SELECT count(itemid) FROM {nextag_items} WHERE type=:type AND itemid=:item", array(':type' => $this->_type, ':item' => $itemid))->fetchField() == 0) {
          db_query("INSERT INTO {nextag_items} (itemid,type) VALUES (:item,:type)", array(':item' => $itemid, ':type' => $this->_type));
        }
        // Need to build list of tagid's for these tag words and if tagword does not yet exist then add it
        $tagwords = preg_split("/[\s,]+/", $this->_newtags, NULL, PREG_SPLIT_NO_EMPTY);
        $tags = array();
        foreach ($tagwords as $word) {
          $word = trim(strip_tags($word));
          $id = db_query("SELECT id FROM {nextag_words} WHERE tagword=:word", array(':word' => $word))->fetchField();
          if (empty($id)) {
            db_query("INSERT INTO {nextag_words} (tagword,metric,last_updated) VALUES (:word,0,:time)", array(':word' => $word, ':time' => time()));
            $id = db_query("SELECT id FROM {nextag_words} WHERE tagword=:word", array(':word' => $word))->fetchField();
          }
          $tags[] = $id;
        }

        // Retrieve the current assigned tags to compare against new tags
        $currentTags = db_query("SELECT tags FROM {nextag_items} WHERE type=:type AND itemid=:item", array(':type' => $this->_type, ':item' => $itemid))->fetchField();
        $currentTags = explode(',', $currentTags);

        $unusedTags = array_diff($currentTags, $tags);
        $newTags = array_diff($tags, $currentTags);

        $this->remove_accessmetrics($itemid, $unusedTags);
        $this->add_accessmetrics($itemid, $newTags);

        $tagids = implode(',', $tags);
        if ($currentTags != $tags) {
          db_query("UPDATE {nextag_items} SET tags = :tid WHERE itemid=:item", array(':tid' => $tagids, ':item' => $itemid));
        }
        return TRUE;

      }
      else {
        $this->clear_tags($itemid);
        return TRUE;
      }
    }
    else {
      watchdog('filedepot', 'Attempted to add tags for file (@item) but no role or group based folder permission defined', array('@item' => $itemid));
      return FALSE;
    }
  }

  /* Clear the tags used for this item and update tag access metrics
   * Typically called when item is deleted
   * @param string $itemid    - Example Story ID (sid) relates to itemid in the tagitems table
   */
  public function clear_tags($itemid) {
    // Retrieve the current assigned tags - these are the tags to update
    $currentTags = db_query("SELECT tags FROM {nextag_items} WHERE type=:type AND itemid=:item", array(':type' => $this->_type, ':item' => $itemid))->fetchField();
    $currentTags = explode(',', $currentTags);
    $this->remove_accessmetrics($itemid, $currentTags);
    db_query("DELETE FROM {nextag_items} WHERE itemid = :item", array(':item' => $itemid));
  }

  public function set_newtags($newtags) {
    $newtags = $this->filtertag($newtags);
    if (!empty($newtags)) {
      $this->_newtags = str_replace(array("\n", ';'), ',', $newtags);
    }
  }

  public function get_newtags($dbmode = TRUE) {
    if ($dbmode) {
      return $this->filtertag($this->_newtags, TRUE);
    }
    else {
      return $this->_newtags;
    }
  }

  public function get_itemtags($itemid) {
    $tags = '';
    $tagids = db_query("SELECT tags FROM {nextag_items} WHERE type=:type AND itemid=:item", array(':type' => $this->_type, ':item' => $itemid))->fetchField();
    if (!empty($tagids)) {
      $query = db_query("SELECT tagword FROM {nextag_words} WHERE id IN ($tagids)");
      while ($A = $query->fetchAssoc()) {
        $tagwords[] = $A['tagword'];
      }
      if (isset($tagwords) AND count($tagwords) > 0) {
        $tags = implode(',', $tagwords);
      }
    }
    return $tags;
  }

  /* Search the defined tagwords across all modules for any tag words matching query
   * Typically would be used in a AJAX driven lookup to populate a dropdown list dynamically as user enters tags
   *
   * @param string $query  - tag words to search on. Can be a list but only the last word will be used for search
   * @return array         - Returns an array of matching tag words
   */
  public function get_matchingtags($query) {
    $matches = array();
    $query = drupal_strtolower(strip_tags($query));
    // User may be looking for more then 1 tag - pull of the last word in the query to search against
    $tags = explode(',', $query);
    $lookup = addslashes(array_pop($tags));
    $sql = "SELECT tagword FROM {nextag_words} WHERE tagword LIKE '{$lookup}%' ORDER BY metric DESC";// REGEXP '^$lookup' ORDER BY metric DESC";
    $q = db_query($sql);
    while ($A = $q->fetchAssoc()) {
      $matches[] = $A['tagword'];
    }
    return $matches;
  }

  /* Return an array of item id's matching tag query */
  public function search($query) {

    $query = addslashes($query);
    $itemids = array();
    // Get a list of Tag ID's for the tag words in the query
    $sql = "SELECT id,tagword FROM {nextag_words} ";
    $asearchtags = explode(',', stripslashes($query));
    if (count($asearchtags) > 1) {
      $sql .= "WHERE ";
      $i = 1;
      foreach ($asearchtags as $tag) {
        $tag = addslashes($tag);
        if ($i > 1) {
          $sql .= "OR tagword = '$tag' ";
        }
        else {
          $sql .= "tagword = '$tag' ";
        }
        $i++;
      }
    }
    else {
      $sql .= "WHERE tagword = '$query' ";
    }
    
    $query = db_query($sql);
    $tagids = array();
    $sql = "SELECT itemid FROM {nextag_items} WHERE type='{$this->_type}' AND ";
    $i = 1;
    while ($A = $query->fetchAssoc()) {
      $tagids[] = $A['id'];
      // REGEX - search for id that is the first id or has a leading comma must then have a trailing , or be the end of the field
      if ($i > 1) {
        $sql .= "AND (tags LIKE '{$A['id']},%' OR tags LIKE '%,{$A['id']}' OR tags LIKE '%,{$A['id']},%' OR tags = '{$A['id']}')";//"AND tags REGEXP '(^|,){$A['id']}(,|$)' ";
      }
      else {
        $sql .= "(tags LIKE '{$A['id']},%' OR tags LIKE '%,{$A['id']}' OR tags LIKE '%,{$A['id']},%' OR tags = '{$A['id']}')";//"tags REGEXP '(^|,){$A['id']}(,|$)' ";
      }
      $i++;
    }
    
    
    if (count($tagids) > 0) {
      $this->_activetags = implode(',', $tagids);
      $query = db_query($sql);
      while ($A = $query->fetchAssoc()) {
        $itemids[] = $A['itemid'];
      }
      if (count($itemids) > 0) {
        return $itemids;
      }
      else {
        return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }
}


class filedepotTagCloud extends nexcloud {

  function __construct() {
    parent::__construct();
    $this->_type = 'filedepot';
  }

  /* For each file in this folder with tagwords - update the metrics per access permission */
  function update_accessmetrics($cid, $tagids = '') {
    $query = db_select('nextag_items', 'a');
    $query->join('filedepot_files', 'b', 'b.fid = a.itemid');
    $query->fields('a', array('itemid'));
    $query->condition('b.cid', $cid, '=');
    $results = $query->execute();
    foreach ($results as $record) {
      parent::update_accessmetrics($record->itemid, $tagids = '');
    }
  }
}
