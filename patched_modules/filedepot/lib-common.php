<?php

/**
 * @file
 * lib-common.php
 * Common library of functions for the applications
 */



function firelogmsg($message) {
  global $firephp, $mytimer;
  $exectime = timer_read('filedepot_timer');
  if (function_exists('dfb')) {  // Calls the firephp fb() function to log message to the firebug console
    dfb("$message - time:$exectime");
  }
}


/**
 * Returns a formatted listbox of categories user has access
 * First checks for View access so that delegated admin can be just for sub-categories
 * @param        string|array        $perms        Single perm 'admin' or array of permissions as required by $filedepot->checkPermission()
 * @param        int                 $selected     Will make this item the selected item in the listbox
 * @param        string              $id           Parent category to start at and then recursively check
 * @param        string              $level        Used by this function as it calls itself to control the indent formatting
 * @return       string                            Return an array of folder options
 */
function filedepot_recursiveAccessArray($perms, $id = 0, $level = 1) {
  $filedepot = filedepot_filedepot();
  $options_tree = array();
  if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
    if ($id == 0) {
      $id = $filedepot->ogrootfolder;
    }
  }
  $query = db_query("SELECT cid,pid,name FROM {filedepot_categories} WHERE pid=:pid ORDER BY cid",
    array(':pid' => $id));
  while ($A = $query->fetchAssoc()) {
    list($cid, $pid, $name) = array_values($A);
    $indent = ' ';
    // Check if user has access to this category
    if ($filedepot->checkPermission($cid, 'view')) {
      // Check and see if this category has any sub categories - where a category record has this cid as it's parent
      $tempcid = db_query("SELECT cid FROM {filedepot_categories} WHERE pid=:cid", array(':cid' => $cid))->fetchField();
      if ($tempcid > 0) {
        if ($level > 1) {
          for ($i = 2; $i <= $level; $i++) {
            $indent .= "--";
          }
          $indent .= ' ';
        }
        if ($filedepot->checkPermission($cid, $perms)) {
          if ($indent != '') {
            $name = " $name";
          }
          $options_tree[$cid] = $indent . $name;
          $options_tree += filedepot_recursiveAccessArray($perms, $cid, $level + 1);
        }
        else {
          // Need to check for any folders with admin even subfolders of parents that user does not have access
          $options_tree += filedepot_recursiveAccessArray($perms, $cid, $level + 1);
        }
      }
      else {
        if ($level > 1) {
          for ($i = 2; $i <= $level; $i++) {
            $indent .= "--";
          }
          $indent .= ' ';
        }
        if ($filedepot->checkPermission($cid, $perms)) {
          if ($indent != '') {
            $name = " $name";
          }
          $options_tree[$cid] = $indent . $name;
        }
      }
    }
  }
  return $options_tree;
}


/**
 * Returns a formatted listbox of categories user has access
 * First checks for View access so that delegated admin can be just for sub-categories
 *
 * @param        string|array        $perms        Single perm 'admin' or array of permissions as required by $filedepot->checkPermission()
 * @param        int                 $selected     Will make this item the selected item in the listbox
 * @param        string              $id           Parent category to start at and then recursively check
 * @param        string              $level        Used by this function as it calls itself to control the indent formatting
 * @param        boolean             $addRootOpt   Add the 'Top Level Folder' option, when appropriate.  Defaults to @c TRUE.
 * @return       string                            Return a formatted HTML Select listbox of categories
 */
function filedepot_recursiveAccessOptions($perms, $selected = '', $id = '0', $level = '1', $addRootOpt = TRUE) {
  $filedepot = filedepot_filedepot();
  $selectlist = '';
  if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
    if ($id == 0) {
      $id = $filedepot->ogrootfolder;
    }
    if ($addRootOpt AND $level == 1 AND user_access('administer filedepot')) {
      $selectlist = '<option value="'.$filedepot->ogrootfolder.'">' . t('Top Level Folder') . '</option>' . LB;
    }
  } else {
    if ($addRootOpt AND $level == 1 AND user_access('administer filedepot')) {
      $selectlist = '<option value="0">' . t('Top Level Folder') . '</option>' . LB;
    }
  }
  $query = db_query("SELECT cid,pid,name FROM {filedepot_categories} WHERE pid=:cid ORDER BY cid", array(':cid' => $id));
  while ($A = $query->fetchAssoc()) {
    list($cid, $pid, $name) = array_values($A);
    $name = filter_xss($name);
    $indent = ' ';
    // Check if user has access to this category


    if ($filedepot->checkPermission($cid, 'view')) {
      // Check and see if this category has any sub categories - where a category record has this cid as it's parent


      $tempcid = db_query("SELECT cid FROM {filedepot_categories} WHERE pid=:cid", array(':cid' => $cid))->fetchField();
      if ($tempcid > 0) {
        if ($level > 1) {
          for ($i = 2; $i <= $level; $i++) {
            $indent .= "--";
          }
          $indent .= ' ';
        }
        if ($filedepot->checkPermission($cid, $perms)) {
          if ($indent != '') {
            $name = " $name";
          }
          $selectlist .= '<option value="' . $cid;
          if ($cid == $selected) {
            $selectlist .= '" selected="selected">' . $indent . $name . '</option>' . LB;
          }
          else {
            $selectlist .= '">' . $indent . $name . '</option>' . LB;
          }
          $selectlist .= filedepot_recursiveAccessOptions($perms, $selected, $cid, $level + 1, $addRootOpt);
        }
        else {
          // Need to check for any folders with admin even subfolders of parents that user does not have access


          $selectlist .= filedepot_recursiveAccessOptions($perms, $selected, $cid, $level + 1, $addRootOpt);
        }

      }
      else {
        if ($level > 1) {
          for ($i = 2; $i <= $level; $i++) {
            $indent .= "--";
          }
          $indent .= ' ';
        }

        if ($filedepot->checkPermission($cid, $perms)) {
          if ($indent != '') {
            $name = " $name";
          }
          $selectlist .= '<option value="' . $cid;
          if ($cid == $selected) {
            $selectlist .= '" selected="selected">' . $indent . $name . '</option>' . LB;
          }
          else {
            $selectlist .= '">' . $indent . $name . '</option>' . LB;
          }
        }
      }
    }
  }
  return $selectlist;
}


/* Recursive Function to navigate down folder structure
 * and determine most recent file data and set last_modified_date for each subfolder
 * Called after a file is added or moved to keep folder data in sync.
 */
function filedepot_updateFolderLastModified($id) {
  $last_modified_parentdate = 0;
  if (db_query("SELECT cid FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $id))->fetchField() > 0) {
    $q1 = db_query("SELECT cid FROM {filedepot_categories} WHERE pid=:cid ORDER BY folderorder ASC", array(':cid' => $id));
    while ($A = $q1->fetchAssoc()) {
      $last_modified_date = 0;
      $q2 = db_query_range("SELECT date FROM {filedepot_files} WHERE cid=:cid ORDER BY date DESC",
        0, 1, array(':cid' => $A['cid']));
      $B = $q2->fetchAssoc();
      if ($B['date'] > $last_modified_date) {
        $last_modified_date = $B['date'];
      }
      if (db_query("SELECT pid FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $A['cid']))->fetchField() > 0) {
        $latestdate = filedepot_updateFolderLastModified($A['cid']);
        if ($latestdate > $last_modified_date) {
          $last_modified_date = $latestdate;
        }
      }
      db_query("UPDATE {filedepot_categories} SET last_modified_date=:time WHERE cid=:cid",
        array(':time' => $last_modified_date, ':cid' => $A['cid']));
      if ($last_modified_date > $last_modified_parentdate) {
        $last_modified_parentdate = $last_modified_date;
      }
    }
    db_query("UPDATE {filedepot_categories} SET last_modified_date=:time WHERE cid=:cid",
      array(':time' => $last_modified_parentdate, ':cid' => $id));
  }
  $q4 = db_query("SELECT date FROM {filedepot_files} WHERE cid=:cid ORDER BY date DESC", array(':cid' => $id));
  $C = $q4->fetchAssoc();
  if ($C['date'] > $last_modified_parentdate) {
    $last_modified_parentdate = $C['date'];
  }
  db_query("UPDATE {filedepot_categories} SET last_modified_date=:time WHERE cid=:cid",
    array(':time' => $last_modified_parentdate, ':cid' => $id));

  return $last_modified_parentdate;
}


/* Return the toplevel parent folder id for a subfolder */
function filedepot_getTopLevelParent($cid) {
  $pid = db_query("SELECT pid FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $cid))->fetchField();
  if ($pid == 0) {
    return $cid;
  }
  else {
    $cid = filedepot_getTopLevelParent($pid);
  }
  return $cid;
}



function filedepot_formatfiletags($tags) {
  $retval = '';
  if (!empty($tags)) {
    $atags = explode(',', $tags);
    if (isset($_POST['tags'])) {
      $asearchtags = explode(',', stripslashes($_POST['tags']));
    }
    else {
      $asearchtags = array();
    }
    foreach ($atags as $tag) {
      $tag = trim($tag); // added to handle extra space thats added when removing a tag - thats between 2 other tags

      if (!empty($tag)) {
        if (in_array($tag, $asearchtags)) {
          $retval .= theme('filedepot_taglinkoff', array('label' => check_plain($tag)));
        }
        else {
          $retval .= theme('filedepot_taglinkon', array('searchtag' => addslashes($tag), 'label' => check_plain($tag)));
        }
      }
    }
  }
  return $retval;

}



function filedepot_formatFileSize($size) {
  $size = intval($size);
  if ($size / 1000000 > 1) {
    $size = round($size / 1000000, 2) . " MB";
  }
  elseif ($size / 1000 > 1) {
    $size = round($size / 1000, 2) . " KB";
  }
  else {
    $size = round($size, 2) . " Bytes";
  }
  return $size;
}

function filedepot_getSubmissionCnt() {
  $filedepot = filedepot_filedepot();
  // Determine if this user has any submitted files that they can approve
  $query = db_query("SELECT cid from {filedepot_filesubmissions}");
  $submissions = 0;
  while ($A = $query->fetchAssoc()) {
    if ($filedepot->checkPermission($A['cid'], 'approval')) {
      $submissions++;
    }
  }
  return $submissions;
}


function filedepot_getUserOptions() {
  $retval = '';
  $query = db_query("SELECT u.uid, u.name,u.status FROM {users} u WHERE u.status = 1 ORDER BY name");
  while ($u = $query->fetchObject()) {
    $retval .= '<option value = "' . $u->uid . '">' . $u->name . '</option>';
  }
  return $retval;
}

function filedepot_getRoleOptions() {
  $retval = '';
  $query = db_query("SELECT r.rid, r.name FROM {role} r ");
  while ($r = $query->fetchObject()) {
    $retval .= '<option value = "' . $r->rid . '">' . $r->name . '</option>';
  }
  return $retval;
}

function filedepot_getGroupOptions() {
  global $user;
  $retval = '';
  $groups = filedepot_og_get_user_groups();
  foreach ($groups as $grpid) {
    $entity = filedepot_og_get_group_entity($grpid);
    $retval .= '<option value="' . $grpid . '">' . $entity->title . '</option>';
  }
  return $retval;
}

function filedepot_og_get_user_groups() {
  global $user;

  $retval = array();
  $groups = og_get_entity_groups('user', $user);
  if (is_array($groups) AND count($groups) > 0) {
    if (function_exists('og_get_group')) {
      $retval = $groups;
    } else {
      $retval = $groups['node'];
    }
  }

  return $retval;
}


function filedepot_og_get_group_entity($grpid) {
  if (function_exists('og_get_group')) {
    $entity = og_get_group('group', $grpid);
    $entity->title = $entity->label;
  } else {
    $entity = node_load($grpid);
  }

  return $entity;
}

function filedepot_get_group_entity_query($grpid=0) {
  $query = new EntityFieldQuery();
  if ($grpid > 0) {
    if (function_exists('og_get_group')) {
      $efq = $query->entityCondition('entity_type', 'group', '=')
        ->entityCondition('bundle', 'group')
        ->entityCondition('entity_id', $grpid);
    } else {
      $efq = $query->entityCondition('entity_type', 'node')
        ->entityCondition('entity_id', $grpid)
        ->fieldCondition(OG_GROUP_FIELD, 'value', 1, '=');
    }
  } else {
    if (function_exists('og_get_group')) {
      $efq = $query->entityCondition('entity_type', 'group', '=')
        ->entityCondition('bundle', 'group');
    } else {
      $efq = $query->entityCondition('entity_type', 'node')
        ->fieldCondition(OG_GROUP_FIELD, 'value', 1, '=');
    }
  }

  return $efq->execute();

}


/**
 * Send out notifications to all users that have subscribed to this file or file category
 * Will check user preferences for notification if Messenger Plugin is installed
 * @param        string      $id        Key used to retrieve details depending on message type
 * @param        string      $type      Message type ->
 *                                       (1) FILEDEPOT_NOTIFY_NEWFILE,
 *                                       (2) FILEDEPOT_NOTIFY_APPROVED,
 *                                       (3) FILEDEPOT_NOTIFY_REJECT,
 *                                       (4) FILEDEPOT_NOTIFY_ADMIN
 * @return       Boolean     Returns TRUE if atleast 1 message was sent out
 */
function filedepot_sendNotification($id, $type = 1) {
  global $user;

  /* If notifications have been disabled via the module admin settings - return TRUE */
  if (variable_get('filedepot_notifications_enabled', 1) == 0) {
    return TRUE;
  }

  if (intval($id) > 0) {
    $target_users = filedepot_build_notification_distribution($id, $type);
    if (count($target_users) > 0) {
      $values = array('fid' => $id, 'target_users' => $target_users);
      drupal_mail('filedepot', $type, $user, language_default(), $values);
    } else {
      watchdog('filedepot', "filedepot_sendNotification (@type) - no target users", array("@type" => $type));
    }
  }
}



/* Common function to generate an array of users that email notification
 * will be sent to. Return an empty array if none
 */
function filedepot_build_notification_distribution($id, $type = 1) {
  global $user;

  $filedepot = filedepot_filedepot();

  $target_users = array();
  if ($type == FILEDEPOT_NOTIFY_NEWFILE OR $type == FILEDEPOT_NOTIFY_APPROVED OR $type == FILEDEPOT_BROADCAST_MESSAGE) {
    $sql = "SELECT file.cid,file.submitter,category.name FROM {filedepot_files} file, "
      . "{filedepot_categories} category WHERE file.cid=category.cid and file.fid=:fid";
    $query = db_query($sql, array(':fid' => $id));
    list($cid, $submitter) = array_values($query->fetchAssoc());
  }
   elseif ($type == FILEDEPOT_NOTIFY_ADMIN) {
    $sql = "SELECT file.cid,file.submitter FROM {filedepot_filesubmissions} file , "
    . "{filedepot_categories} category WHERE file.cid=category.cid and file.id=:fid";
    $query = db_query($sql, array(':fid' => $id));
    list($cid, $submitter) = array_values($query->fetchAssoc());
  }
   elseif ($type == FILEDEPOT_NOTIFY_REJECT) {
     $submitter = db_query("SELECT submitter FROM {filedepot_filesubmissions} WHERE id=:fid", array(':fid' => $id))->fetchField();
   }

  // Generate the distribution
  if ($type == FILEDEPOT_NOTIFY_NEWFILE ) {
    if (variable_get('filedepot_default_notify_newfile', 0) == 1) { // Site default to notify all users on new files
      $query_users = db_query("SELECT uid FROM {users} WHERE uid > 0 AND status = 1");
      while ( $A = $query_users->fetchObject()) {
        if ($filedepot->checkPermission($cid, 'view', $A->uid)) {
          $personal_exception = FALSE;
          if (db_query("SELECT uid FROM {filedepot_usersettings} WHERE uid=:uid AND notify_newfile=0", array(':uid' => $A->uid))->fetchField() == $A->uid) {
            $personal_setting = FALSE; // User preference record exists and set to not be notified
          }
          else {
            $personal_setting = TRUE; // Either record does not exist or user preference is to be notified
          }
          // Check if user has any notification exceptions set for this folder
          if (db_query("SELECT count(*) FROM {filedepot_notifications} WHERE cid=:cid AND uid=:uid AND cid_newfiles=0", array(':cid' => $cid, ':uid' => $A->uid))->fetchField() > 0) {
            $personal_exception = TRUE;
          }
          // Only want to notify users that don't have setting disabled or exception record
          if ($personal_setting == TRUE AND $personal_exception == FALSE AND $A->uid != $submitter) {
            $target_users[] = $A->uid;
          }
        }
      }
    }
    else {
      $sql = "SELECT a.uid FROM {filedepot_usersettings} a LEFT JOIN {users} b on b.uid=a.uid WHERE a.notify_newfile = 1 and b.status=1";
      $query_users = db_query($sql);
      while ( $A = $query_users->fetchObject()) {
        if ($filedepot->checkPermission($cid, 'view', $A->uid)) {
          $personal_exception = FALSE;
          if (db_query("SELECT ignore_filechanges FROM {filedepot_notifications} WHERE fid=:fid and uid=:uid", array(':fid' => $id, ':uid' => $A->uid))->fetchField() == 1) {
            $personal_exception = TRUE;
          }
          // Only want to notify users that have notifications enabled but don't have an exception record
          if ($personal_exception === FALSE) {
            $target_users[] = $A->uid;
          }
        }
      }
      // Check the folder specific notification options as well
      $sql = "SELECT a.uid FROM {filedepot_notifications} a LEFT JOIN {users} b on b.uid=a.uid WHERE a.cid=:cid AND a.cid_newfiles = 1 and b.status=1";
      $query_users = db_query($sql, array(':cid' => $cid));
      while ( $A = $query_users->fetchObject()) {
        if (!in_array($A->uid, $target_users) AND $filedepot->checkPermission($cid, 'view', $A->uid)) {
            $target_users[] = $A->uid;
        }
      }

    }
  }
   elseif ($type == FILEDEPOT_NOTIFY_APPROVED) { // File submission being approved by admin where $id = file id. Send only to user
      $target_users[] = $submitter;
  }
   elseif ($type == FILEDEPOT_NOTIFY_REJECT) { // File submission being approved by admin where $id = file id. Send only to user
      $target_users[] = $submitter;
  }
   elseif ($type == FILEDEPOT_NOTIFY_ADMIN) {
    $query_users = db_query("SELECT uid FROM {users} WHERE uid > 0 AND status = 1");
    while ( $A = $query_users->fetchObject()) {
      if ($filedepot->checkPermission($cid, 'approval', $A->uid)) {
        $personal_exception = FALSE;
        if (db_query("SELECT uid FROM {filedepot_usersettings} WHERE uid=:uid AND notify_newfile=0", array(':uid' => $A->uid))->fetchField() == $A->uid) {
          $personal_setting = FALSE; // User preference record exists and set to not be notified
        }
        else {
          $personal_setting = TRUE; // Either record does not exist or user preference is to be notified
        }
        // Check if user has any notification exceptions set for this folder
        if (db_query("SELECT count(*) FROM {filedepot_notifications} WHERE cid=:cid AND uid=:uid AND cid_newfiles=0", array(':cid' => $cid, ':uid' => $A->uid))->fetchField() > 0) {
          $personal_exception = TRUE;
        }
        // Only want to notify users that don't have setting disabled or exception record
        if ($personal_setting == TRUE AND $personal_exception == FALSE AND $A->uid != $submitter) {
          $target_users[] = $A->uid;
        }
      }
    }
  }
   elseif ($type == FILEDEPOT_BROADCAST_MESSAGE) {
    // Nov 2014: Added check that user also has view access to the folder from where broadcast was issued from
    if (variable_get('filedepot_default_allow_broadcasts', 1) == 1) { // Site default set to allow broadcast enabled
      $uquery = db_query("SELECT uid FROM {users} WHERE uid > 0 AND status = 1");
      while ( $A = $uquery->fetchObject()) {
        if ($A->uid != $user->uid && $filedepot->checkPermission($cid, 'view', $A->uid)) {
          if (db_query("SELECT allow_broadcasts FROM {filedepot_usersettings} WHERE uid=:uid",
            array(':uid' => $A->uid))->fetchField() == 0) {
            $personal_setting = FALSE; // Found user setting to not be notified
          }
          else {
            $personal_setting = TRUE;
          }
          // Only want to notify users that don't have setting disabled or exception record and folder view access
          if ($personal_setting == TRUE AND $filedepot->checkPermission($cid, 'view', $A->uid)) {
            $target_users[] = $A->uid;
          }
        }
      }
    }
    else {
      $sql = "SELECT a.uid FROM {filedepot_usersettings} a LEFT JOIN {users} b on b.uid=a.uid "
      . "WHERE a.allow_broadcasts=1 and b.status=1";
      $uquery = db_query($sql);
      while ($B  = $uquery->fetchObject()) {
        if ($user->uid != $B->uid && $filedepot->checkPermission($cid, 'view', $B->uid)) {
          $target_users[] = $B->uid;
        }
      }
    }
  }

  if (count($target_users) > 0) {
      // Sort the array so that we can check for duplicate user notification records
      sort($target_users);
      reset($target_users);
  }

  return $target_users;

}



function filedepot_delTree($dir) {
  $files = glob( $dir . '*', GLOB_MARK );
  foreach ( $files as $file ) {
    if ( drupal_substr( $file, -1 ) == '/' ) {
      filedepot_delTree( $file );
    }
    else {
      @unlink( $file );
    }
  }
  if (is_dir($dir)) {
    @rmdir( $dir );
  }
}

