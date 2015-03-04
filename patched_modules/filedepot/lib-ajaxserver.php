<?php

/**
 * @file
 * lib-ajaxserver.php
 * Library functions for the ajax_server
 */

function filedepotAjaxServer_getfilelisting() {
  global $user;
  $filedepot = filedepot_filedepot();

  if (empty($filedepot->activeview)) {
    $filedepot->ajaxBackgroundMode = TRUE;
  }
  elseif ($filedepot->cid == 0 AND !in_array($filedepot->activeview, $filedepot->validReportingModes)) {
    $filedepot->activeview = 'latestfiles';
    $filedepot->ajaxBackgroundMode = FALSE;
  }

  if (db_query("SELECT COUNT(*) FROM {filedepot_categories} WHERE cid=:cid",
      array(':cid' => $filedepot->cid))->fetchField() == 0
  ) {
    $filedepot->cid = 0;
  }

  if ($filedepot->activeview == 'notifications') {
    $data['cid'] = $filedepot->cid;
    $data['retcode'] = 200;
    $data['cid'] = $filedepot->cid;
    $data['activefolder'] = theme('filedepot_activefolder');
    $data['displayhtml'] = theme('filedepot_notifications');
    $data['header'] = theme('filedepot_header');
    $data['moreactions'] = filedepotAjaxServer_getMoreActions($filedepot->activeview);
  }
  elseif ($filedepot->cid > 0 AND $filedepot->checkPermission($filedepot->cid, 'view')) {
    $data['retcode'] = 200;
    $data['cid'] = $filedepot->cid;
    $foldercount = db_query("SELECT count(cid) FROM {filedepot_categories} WHERE cid=:cid",
      array(':cid' => $filedepot->cid))->fetchField();
    if (user_is_logged_in() AND $filedepot->cid > 0 AND $foldercount == 1) {
      $pid = db_query("SELECT pid FROM {filedepot_categories} WHERE cid=:cid",
        array(':cid' => $filedepot->cid))->fetchField();
      if ($pid > 0) {
        $count1 = db_query("SELECT count(cid) FROM {filedepot_recentfolders} WHERE uid=:uid",
          array(':uid' => $user->uid))->fetchField();
        if ($count1 > 4) {
          // Note DELETE not allowed in db_query_range - PDO issue with Postgres support
          $oldest = db_query_range("SELECT id FROM {filedepot_recentfolders} WHERE uid=:uid ORDER BY id ASC", 0, 1,
            array(':uid' => $user->uid))->fetchField();
          db_query("DELETE FROM {filedepot_recentfolders} WHERE id=:id ", array(':id' => $oldest));
        }
        $count2 = db_query("SELECT count(cid) FROM {filedepot_recentfolders} WHERE uid=:uid AND cid=:cid",
          array(
            ':uid' => $user->uid,
            ':cid' => $filedepot->cid,
          ))->fetchField();
        if ($count2 == 0) {
          db_query("INSERT INTO {filedepot_recentfolders} (uid,cid) VALUES (:uid,:cid)",
            array(
              ':uid' => $user->uid,
              ':cid' => $filedepot->cid,
            ));
        }
      }
    }

    $data['displayhtml'] = filedepot_displayFolderListing($filedepot->cid);
    if (is_array($filedepot->lastRenderedFiles) AND count($filedepot->lastRenderedFiles) > 0) {
      $data['lastrenderedfiles'] = json_encode($filedepot->lastRenderedFiles);
    }
    firelogmsg("Completed generating FileListing");
    $data['activefolder'] = theme('filedepot_activefolder', array('token' => drupal_get_token(FILEDEPOT_TOKEN_FOLDERMGMT)));
    $data['moreactions'] = filedepotAjaxServer_getMoreActions($filedepot->activeview);
    $data['header'] = theme('filedepot_header', array('token' => drupal_get_token(FILEDEPOT_TOKEN_LISTING)));

  }
  elseif ($filedepot->cid == 0) {
    $data['retcode'] = 200;
    $data['cid'] = $filedepot->cid;
    $data['displayhtml'] = filedepot_displayFolderListing($filedepot->cid);
    $data['activefolder'] = theme('filedepot_activefolder', array('token' => drupal_get_token(FILEDEPOT_TOKEN_FOLDERMGMT)));
    $data['moreactions'] = filedepotAjaxServer_getMoreActions($filedepot->activeview);
    $data['header'] = theme('filedepot_header', array('token' => drupal_get_token(FILEDEPOT_TOKEN_LISTING)));

  }
  else {
    $data['retcode'] = 401;
    $data['error'] = 'Error: No Access to Folder';
    db_query("DELETE FROM {filedepot_recentfolders} WHERE uid=:uid AND cid=:cid ", array(
      ':uid' => $user->uid,
      ':cid' => $filedepot->cid
    ));
  }

  //firelogmsg("Completed generating Header return data");
  return $data;

}


/**
 * Generate Left Side Navigation code which is used to create the YUI menu's in
 * the AJAX handler javascript.
 */
function filedepotAjaxServer_generateLeftSideNavigation($data = '') {
  global $user;
  $filedepot = filedepot_filedepot();
  $display_recent_folders = variable_get('filedepot_show_recent_folders', 1);

  if (empty($data)) {
    $data = array('retcode' => 200);
  }

  $approvals = filedepot_getSubmissionCnt();

  $data['reports'] = array();
  $data['recentfolders'] = array();
  $data['topfolders'] = array();
  $data['reports'][] = array(
    'label' => t('Latest Files'),
    'report' => 'latestfiles',
    'parent' => 'allitems',
    'icon' => 'icon-filelisting',
  );
  if (user_is_logged_in()) {
    $data['reports'][] = array(
      'label' => t('Notifications'),
      'report' => 'notifications',
      'parent' => 'allitems',
      'icon' => 'icon-fileowned',
    );
    $data['reports'][] = array(
      'label' => t('Owned by me'),
      'report' => 'myfiles',
      'parent' => 'allitems',
      'icon' => 'icon-fileowned',
    );
    $data['reports'][] = array(
      'label' => t('Downloaded by me'),
      'report' => 'downloads',
      'parent' => 'allitems',
      'icon' => 'icon-fileowned',
    );
    $data['reports'][] = array(
      'label' => t('Unread Files'),
      'report' => 'unread',
      'parent' => 'allitems',
      'icon' => 'icon-fileowned',
    );
    $data['reports'][] = array(
      'label' => t('Locked by me'),
      'report' => 'lockedfiles',
      'parent' => 'allitems',
      'icon' => 'icon-filelocked',
    );
    $data['reports'][] = array(
      'label' => t('Flagged by me'),
      'report' => 'flaggedfiles',
      'parent' => 'allitems',
      'icon' => 'icon-fileflagged',
    );
  }
  if ($approvals > 0) {
    $approvals = "&nbsp;($approvals)";
    $data['reports'][] = array(
      'label' => t('Waiting approval') . "$approvals",
      'report' => 'approvals',
      'parent' => 'allitems',
      'icon' => 'icon-fileowned',
    );
  }

  if (user_is_logged_in()) {
    if (user_access('administer filedepot', $user)) {
      $res = db_query("SELECT COUNT(id) as incoming FROM {filedepot_import_queue}");
    }
    else {
      $res = db_query("SELECT COUNT(id) as incoming FROM {filedepot_import_queue} WHERE uid=:uid",
        array(':uid' => $user->uid));
    }
    $A = $res->fetchAssoc();

    if ($A['incoming'] > 0) {
      $incoming_msg = "&nbsp;({$A['incoming']})";
      $data['reports'][] = array(
        'label' => t('Incoming Files') . "$incoming_msg",
        'report' => 'incoming',
        'parent' => 'allitems',
        'icon' => 'icon-fileowned',
      );
    }
  }

  // Setup the Most Recent folders for this user
  if ((user_is_logged_in()) && ($display_recent_folders != 0)) {
    $sql = "SELECT a.id,a.cid,b.name FROM {filedepot_recentfolders} a ";
    $sql .= "LEFT JOIN {filedepot_categories} b ON b.cid=a.cid ";
    if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
      $sql .= "WHERE a.cid in ({$filedepot->allowableGroupViewFoldersSql}) AND a.cid != {$filedepot->ogrootfolder} AND b.pid != {$filedepot->ogrootfolder} ";
    }
    else {
      $sql .= "WHERE 1=1 ";
    }
    $sql .= "AND uid=:uid ORDER BY id";
    $res = db_query($sql, array(':uid' => $user->uid));
    while ($A = $res->fetchAssoc()) {
      $data['recentfolders'][] = array(
        'label' => filter_xss($A['name']),
        'cid' => $A['cid'],
        'icon' => 'icon-allfolders',
      );
    }

  }

  $display_order = "ORDER BY " . (variable_get('filedepot_override_folderorder', 0) ? 'name ASC,' : '') . ' folderorder';
  $sql = "SELECT cid,pid,name,description from {filedepot_categories} ";
  if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
    $sql .= "WHERE pid = :cid {$display_order}";
    $res = db_query($sql, array(':cid' => $filedepot->ogrootfolder));
  }
  else {
    $sql .= "WHERE pid=0 {$display_order}";
    $res = db_query($sql);
  }

  while ($A = $res->fetchAssoc()) {
    if ($filedepot->checkPermission($A['cid'], 'view')) {
      $data['topfolders'][] = array(
        'label' => filter_xss($A['name']),
        'cid' => $A['cid'],
        'parent' => 'allfolders',
        'icon' => 'icon-allfolders',
      );
    }
  }

  if (function_exists('filedepot_customLeftsideNavigation')) {
    $data = filedepot_customLeftsideNavigation($data);
  }

  if ($display_recent_folders == 0) {
    $data['recentfolders'] = NULL;
  }

  return $data;

}


/* Recursive Function to display folder listing */
function filedepot_displayFolderListing($id = 0, $level = 0, $folderprefix = '', $rowid = 1) {
  global $user;

  $filedepot = filedepot_filedepot();

  $file_first_display = (variable_get('filedepot_displayorder_filesfirst', 1) == 1) ? TRUE : FALSE;

  $retval = '';
  if ($id > 0 AND !in_array($id, $filedepot->allowableViewFolders)) {
    watchdog('filedepot', 'No view access to category @id', array('id' => $id));
    return;
  }

  if ($id > 0) {
    $folderprefix = db_query("SELECT folderprefix FROM {filedepot_folderindex} WHERE cid=:cid AND uid=:uid",
      array(':cid' => $id, ':uid' => $user->uid))->fetchField();
  }
  else {
    $folderprefix = '';
  }

  $level++;

  if ($file_first_display == TRUE) {
    if ($level == 1) {
      $retval .= nexdocsrv_generateFileListing($id, $level, $folderprefix);
    }
  }

  $sql = '';
  if (function_exists('filedepot_customReportFoldersSQL')) {
    $reportmode = check_plain($_POST['reportmode']);
    $sql = trim(filedepot_customReportFoldersSQL($id, $reportmode));
  }

  if ($id > 0 OR !empty($sql)) {
    // Show any subfolders and check and see if this is a custom report
    $override_order = "ORDER BY " . (variable_get('filedepot_override_folderorder', 0) ? 'name ASC,' : '') . ' folderorder';
    $max_orderby = "ORDER BY " . (variable_get('filedepot_override_folderorder', 0) ? 'name DESC,' : '') . ' folderorder DESC';

    if (empty($sql)) {
      $sql = "SELECT DISTINCT cid,pid,name,description,folderorder,last_modified_date FROM {filedepot_categories} WHERE pid=:pid ";
      if (!empty($filedepot->allowableViewFoldersSql)) {
        $sql .= "AND cid in ({$filedepot->allowableViewFoldersSql}) ";
      }

      $sql .= $override_order;
    }

    $qfolders = db_query($sql, array(':pid' => $id));
    $output = '';
    $i = $rowid;
    $maxfolderorder = db_query_range("SELECT folderorder FROM {filedepot_categories} WHERE pid=:pid " . $max_orderby,
      0, 1, array(':pid' => $id))->fetchField();
    while ($A = $qfolders->fetchAssoc()) {
      $subfolderlisting = '';
      if (empty($folderprefix)) {
        $formatted_foldernumber = $i;
      }
      else {
        $formatted_foldernumber = "{$folderprefix}.{$i}";
      }

      $subfolder_count = db_query("SELECT count(cid) FROM {filedepot_categories} WHERE pid=:pid",
        array(':pid' => $A['cid']))->fetchField();
      $tmpsubfolderlisting = '';
      if ($subfolder_count > 0) {
        // Show any sub-subfolders - calling this function again recursively
        $tmpsubfolderlisting = filedepot_displayFolderListing($A['cid'], $level, $formatted_foldernumber, $rowid);
      }

      $tmpfilelisting = nexdocsrv_generateFileListing($A['cid'], $level, $formatted_foldernumber);

      if ($file_first_display == TRUE) {
        $subfolderlisting .= $tmpfilelisting;
        $subfolderlisting .= $tmpsubfolderlisting;
      }
      else {
        $subfolderlisting .= $tmpsubfolderlisting;
        $subfolderlisting .= $tmpfilelisting;
      }

      $retval .= theme('filedepot_folderlisting', array(
        'folderrec' => $A,
        'folderprefix' => $formatted_foldernumber,
        'level' => $level,
        'subfoldercontent' => $subfolderlisting,
        'maxorder' => $maxfolderorder,
      ));
      $i++;
    }
    if (empty($output) AND $level == 1) {
      if (isset($filedepot->lastRenderedFiles[0][0])) {
        $retval .= "<div id=\"subfolder{$filedepot->lastRenderedFiles[0][0]}_rec{$filedepot->lastRenderedFiles[0][1]}_bottom\">";
      }
    }
  }

  if ($file_first_display == FALSE) {
    if ($level == 1) {
      $retval .= nexdocsrv_generateFileListing($id, $level - 1, $folderprefix);
    }
  }

  return $retval;
}


/* Called to generate the filelisting (up to 3 times per folder)
 * Depending on the Performance Settings
 * Initially called to display pass1 number of files
 * Called a 2nd time to display pass2 number of files
 * Optionally called on click by the user to render the rest of the files for the selected folder
*/
function nexdocsrv_generateFileListing($cid, $level = 1, $folderprefix = '') {
  global $user;

  $filedepot = filedepot_filedepot();

  $filedepot->selectedTopLevelFolder = $cid;
  $files = array();
  $limit_start = 0;
  $limit_end = FALSE;

  $sql = filedepot_getFileListingSQL($cid, $limit_start, $limit_end);

  if ($limit_end === FALSE) {
    $file_query = db_query($sql);
  }
  else {
    $file_query = db_query_range($sql, $limit_start, $limit_end);
  }

  $output = '';
  $break = FALSE;

  if (empty($folderprefix)) {
    $folderprefix = db_query("SELECT folderprefix FROM {filedepot_folderindex} WHERE cid=:cid AND uid=:uid",
      array(':cid' => $cid, ':uid' => $user->uid))->fetchField();
  }

  // Adjust variable used to count displayed files during this pass as well as to create the displayed file index.
  $i = 0;
  if ($filedepot->activeview == 'getmoredata') {
    $i = $filedepot->recordCountPass1;
  }
  elseif ($filedepot->activeview == 'getmorefolderdata') {
    $i = $filedepot->recordCountPass2 + $filedepot->recordCountPass1;
  }

  while ($A = $file_query->fetchAssoc()) {
    if ($filedepot->activeview == 'approvals') {
      $A['fid'] = $A['id'];
    }

    if (empty($fid) or empty($files) OR !in_array($fid, $files)) {
      $i++;

      // Existing folders where fileorder has not yet been set
      if ($filedepot->activeview != 'latestfiles' AND $A['fileorder'] != $i * 10) {
        $A['fileorder'] = $i * 10;
        db_query("UPDATE {filedepot_files} set fileorder=:order WHERE fid=:fid", array(
          ':order' => $A['fileorder'],
          ':fid' => $A['fid'],
        ));
      }

      $more_records_message = '';
      if ($filedepot->ajaxBackgroundMode == TRUE AND $i >= $filedepot->recordCountPass1) {
        $break = TRUE;
        $filedepot->lastRenderedFiles[] = array($cid, $A['fid'], $folderprefix, $level);
        $more_records_message = 'moredata_msg';
      }
      elseif ($filedepot->activeview == 'getmoredata' AND $i >= ($filedepot->recordCountPass1 + $filedepot->recordCountPass2)) {
        $break = TRUE;
        // Check if there are more records - the SQL LIMIT statement allowed for one more record
        // If there are more - show the AJAX link to load more data - pass 2
        if ($file_query->fetchAssoc()) {
          $more_records_message = 'loadfolder_msg';
        }
      }
      if ($break) {
        $output .= theme('filedepot_filelisting', array(
            'listingrec' => $A,
            'foldernumber' => $folderprefix,
            'level' => $level,
            'morerecords' => $more_records_message,
          )
        );
        break;
      }
      else {
        $output .= theme('filedepot_filelisting',
          array(
            'listingrec' => $A,
            'foldernumber' => $folderprefix,
            'level' => $level,
          ));
        $files[] = $A['fid'];
      }
    }
  }
  return $output;
}

/* If searching on multiple query terms then it will return matches that
 * matches that have all entered terms first, followed by
 * records that have any of the entered terms
*/
function filedepot_displaySearchListing($query_terms) {
  $output = '';
  if (!empty($query_terms)) {
    $filedepot = filedepot_filedepot();

    $args = array();
    $fields = array('a.title', 'a.description', 'b.name', 'b.description');
    $keywords = explode(' ', $query_terms);

    $sql = '';
    $select = "SELECT a.fid, a.cid, a.title, a.fname, a.date, a.version, a.submitter, "
      . "a.status, a.description, a.size, b.pid, b.nid, b.name as foldername, b.description as folderdesc "
      . "FROM filedepot_files a JOIN filedepot_categories b ON b.cid=a.cid ";

    if (!empty($filedepot->allowableViewFoldersSql)) {
      $where = 'WHERE a.cid IN (' . $filedepot->allowableViewFoldersSql . ') AND ';
    }
    else {
      $where = 'WHERE ';
    }

    /* Build the AND clause of the SQL Query
     * Find files that match all the search terms in any of the defined fields
     * So for each field name, description folder, that field text must have
     * all the keywords
    */
    $field_id = 1;
    $where_and = '';
    foreach ($fields as $field) {
      $term_id = 1;
      $where_and .= ($field_id == 1) ? '(' : '1=1) OR (';
      foreach ($keywords as $keyword) {
        $where_and .= "{$field} like :afield{$field_id}_{$term_id} AND ";
        $args = array_merge($args, array(":afield{$field_id}_{$term_id}" => "%{$keyword}%"));
        $term_id++;
      }
      $field_id++;
    }

    $where_and .= (empty($where_and) ? '' : '1=1) ');
    $sql .= "{$select} {$where} {$where_and} UNION {$select} ";

    /* Build the second query - UNION used so that we return first the
     * files that match all the keywords
     * Second query will find files that match ANY of the search terms in any of the defined fields
    */
    if (!empty($filedepot->allowableViewFoldersSql)) {
      $sql .= ' WHERE a.cid IN (' . $filedepot->allowableViewFoldersSql . ') AND (';
    }
    else {
      $sql .= ' WHERE (';
    }

    /* Build the second query OR clause - loop over the fields and terms */
    $field_id = 1;
    $where_or = '';
    foreach ($fields as $field) {
      $term_id = 1;
      $where_or .= ($field_id > 1) ? '  OR ' : '';
      foreach ($keywords as $keyword) {
        $where_or .= ($term_id > 1) ? '  OR ' : '';
        $where_or .= "{$field} like :bfield{$field_id}_{$term_id}";
        $args = array_merge($args, array(":bfield{$field_id}_{$term_id}" => "%{$keyword}%"));
        $term_id++;
      }
      $field_id++;
    }
    $where_or .= (!empty($where_or)) ? ') ' : ' ';
    $sql .= $where_or;
    $result = db_query($sql, $args);

    $time = timer_read('get_time');
    drupal_set_message($time);

    $filedepot->recordCount = $result->rowCount();

    while ($A = $result->fetchAssoc()) {
      $output .= theme('filedepot_filelisting', array('listingrec' => $A));
    }
  }
  return $output;

}

function filedepot_displayTagSearchListing($query) {
  $filedepot = filedepot_filedepot();
  $nexcloud = filedepot_nexcloud();

  $sql = "SELECT file.fid as fid,file.cid,file.title,file.fname,file.date,file.version,file.submitter,file.status,";
  $sql .= "file.description,category.name as foldername,category.pid,category.nid ";
  $sql .= "FROM {filedepot_files} file ";
  $sql .= "LEFT JOIN {filedepot_categories} category ON file.cid=category.cid ";
  $sql .= "WHERE 1=1 ";
  if (!empty($filedepot->allowableViewFoldersSql)) {
    $sql .= "AND file.cid in ($filedepot->allowableViewFoldersSql) ";
  }

  $itemids = $nexcloud->search($query);
  if ($itemids !== FALSE) {
    $itemids = implode(',', $itemids);
  }
  if (!empty($itemids)) {
    $sql .= "AND file.fid in ($itemids) ";
  }
  else {
    $sql .= "AND 1 = 2 "; // No tags match query - return 0 records
  }
  $sql .= "ORDER BY file.date DESC ";

  $search_query = db_query($sql);
  $output = '';
  while ($A = $search_query->fetchAssoc()) {
    $output .= theme('filedepot_filelisting', array('listingrec' => $A));
  }
  return $output;

}

function filedepot_getFileListingSQL($cid, &$out_limit_start, &$out_limit_end) {
  global $user;
  $filedepot = filedepot_filedepot();

  $out_limit_start = 0;
  $display_orderby = variable_get('filedepot_override_folderorder', 0) ? 'file.title ASC, file.date DESC' : 'file.date DESC';
  $sql = '';
  // Check and see if this is a custom report
  if (function_exists('filedepot_customReportFilesSQL')) {
    $sql = trim(filedepot_customReportFilesSQL($cid, $filedepot->activeview));
    if (!empty($sql)) {
      return $sql;
    }
  }

  $sql = "SELECT file.fid as fid,file.cid,file.title,file.fname,file.date,file.version,file.submitter,file.status,file.fileorder,folderindex.folderprefix,";
  $sql .= "file.description,category.name as foldername,category.pid,category.nid,category.last_modified_date,status_changedby_uid as changedby_uid, size ";
  $sql .= "FROM {filedepot_files} file ";
  $sql .= "LEFT JOIN {filedepot_categories} category ON file.cid=category.cid ";
  $sql .= "LEFT JOIN {filedepot_folderindex} folderindex ON file.cid=folderindex.cid AND folderindex.uid = {$user->uid} ";
  $limit = FALSE;
  if ($filedepot->activeview == 'lockedfiles') {
    $sql .= "WHERE file.status=2 AND status_changedby_uid={$user->uid} ";
    if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
      $sql .= "AND file.cid in ({$filedepot->allowableGroupViewFoldersSql}) ";
    }

    $sql .= "ORDER BY {$display_orderby} ";
    $limit = $filedepot->maxDefaultRecords;
  }
  elseif ($filedepot->activeview == 'downloads') {
    // Will return multiple records for same file as we capture download records each time a user downloads it
    $sql .= "LEFT JOIN {filedepot_downloads} downloads on downloads.fid=file.fid ";
    $sql .= "WHERE downloads.uid={$user->uid} ";
    if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
      $sql .= "AND file.cid in ({$filedepot->allowableGroupViewFoldersSql}) ";
    }
    $sql .= "ORDER BY {$display_orderby} ";
    $limit = $filedepot->maxDefaultRecords;
  }
  elseif ($filedepot->activeview == 'unread') {
    $sql .= "LEFT OUTER JOIN {filedepot_downloads} downloads on downloads.fid=file.fid ";
    $sql .= "WHERE downloads.fid IS NULL ";
    if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
      $sql .= "AND file.cid in ({$filedepot->allowableGroupViewFoldersSql}) ";
    }
    elseif (empty($filedepot->allowableViewFoldersSql)) {
      $sql .= "AND file.cid is NULL ";
    }
    else {
      $sql .= "AND file.cid in ({$filedepot->allowableViewFoldersSql}) ";
    }
    $sql .= "ORDER BY {$display_orderby} ";
    $limit = $filedepot->maxDefaultRecords;

  }
  elseif ($filedepot->activeview == 'incoming') {
    $sql = "SELECT id as fid, 0 as cid, orig_filename as title,  queue_filename as fname, timestamp as date, 0 as version, ";
    $sql .= "uid as submitter, 0 as status, 'N/A' as description, 'Incoming Files' as name, 0 as pid, 0 as changedby_uid, size ";
    $sql .= "FROM {filedepot_import_queue} ";
    if (!user_access('administer filedepot', $user)) {
      $sql .= "WHERE uid={$user->uid} ";
    }
    $sql .= "ORDER BY {$display_orderby} ";

  }
  elseif ($filedepot->activeview == 'flaggedfiles') {
    $sql .= "LEFT JOIN {filedepot_favorites} favorites on favorites.fid=file.fid ";
    $sql .= "WHERE favorites.uid={$user->uid} ";
    if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
      $sql .= "AND file.cid in ({$filedepot->allowableGroupViewFoldersSql}) ";
    }

    $sql .= "ORDER BY {$display_orderby} ";
    $limit = $filedepot->maxDefaultRecords;
  }
  elseif ($filedepot->activeview == 'myfiles') {
    $sql .= "WHERE file.submitter={$user->uid} ";
    if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
      $sql .= "AND file.cid in ({$filedepot->allowableGroupViewFoldersSql}) ";
    }
    $sql .= "ORDER BY {$display_orderby} ";
    $limit = $filedepot->maxDefaultRecords;

  }
  elseif ($filedepot->activeview == 'approvals') {
    // Determine if this user has any submitted files that they can approve
    $sql = "SELECT file.id,file.cid,file.title,file.fname,file.date,file.version,file.submitter,file.status,";
    $sql .= "file.description,category.name as foldername,category.pid,0 as changedby_uid, size ";
    $sql .= "FROM {filedepot_filesubmissions} file ";
    $sql .= "LEFT JOIN {filedepot_categories} category ON file.cid=category.cid ";
    if (!user_access('administer filedepot', $user)) {
      $categories = $filedepot->getAllowableCategories(array('approval', 'admin'));
      if (empty($categories)) {
        $sql .= "WHERE file.cid is NULL ";
      }
      else {
        $sql .= "WHERE file.cid in ($categories) ";
      }
      if ($filedepot->ogmode_enabled AND !empty($filedepot->allowableGroupViewFoldersSql)) {
        $sql .= "AND file.cid in ({$filedepot->allowableGroupViewFoldersSql}) ";
      }
    }
    $sql .= "ORDER BY {$display_orderby} ";

  }
  elseif ($cid > 0) {
    $sql .= "WHERE file.cid={$cid} ORDER BY {$display_orderby}, file.fid DESC ";
    if ($filedepot->activeview == 'getmorefolderdata') {
      if (isset($_POST['pass2']) AND $_POST['pass2'] == 1) {
        $limit = 100000;
        $out_limit_start = $filedepot->recordCountPass1;
      }
      else {
        $recordoffset = $filedepot->recordCountPass2 + $filedepot->recordCountPass1;
        $filedepot->folder_filenumoffset = $recordoffset;
        $limit = 100000;
        $out_limit_start = $recordoffset;
      }
    }
    elseif ($filedepot->activeview != 'getallfiles') {
      // Set SQL query options for amount of data to return - used by the AJAX routine getmorefiledata to populate display in the background
      if ($filedepot->lastRenderedFolder == $cid) {
        $filedepot->folder_filenumoffset = $filedepot->recordCountPass1;
        // Add 1 to allow $file_query->fetchAssoc() to know there are more records - used in nexdocsrv_generateFileListing()
        $folder_filelimit = $filedepot->recordCountPass2 + 1;
        $limit = $folder_filelimit;
        $out_limit_start = $filedepot->recordCountPass1;


      }
      else {

        $limit = $filedepot->recordCountPass1;
        $out_limit_start = 0;

      }
    }

  }
  else {
    // Default view - latest files
    if (!empty($filedepot->allowableViewFoldersSql)) {
      if ($filedepot->ogmode_enabled) {
        if (!empty($filedepot->allowableGroupViewFoldersSql)) {
          $sql .= "WHERE file.cid in ({$filedepot->allowableGroupViewFoldersSql}) ";
        }
        else {
          $sql .= "WHERE file.cid in ({$filedepot->allowableViewFoldersSql}) ";
        }
      }
      elseif (!user_access('administer filedepot', $user)) {
        if (empty($filedepot->allowableViewFoldersSql)) {
          $sql .= "WHERE file.cid is NULL ";
        }
        else {
          $sql .= "WHERE file.cid in ({$filedepot->allowableViewFoldersSql}) ";
        }
      }
    }

    if ($filedepot->activeview == 'latestfiles') {
      $sql .= "ORDER BY file.date DESC ";
      $limit = $filedepot->maxDefaultRecords;
    }
    else {
      $sql .= "ORDER BY {$display_orderby} ";
      $limit = $filedepot->maxDefaultRecords;
    }
  }

  $out_limit_end = $limit;
  return $sql;

}


function filedepotAjaxServer_loadFileDetails() {
  global $user;

  $filedepot = filedepot_filedepot();
  $nexcloud = filedepot_nexcloud();
  $reportmode = check_plain($_POST['reportmode']);
  $retval = array();
  $retval['editperm'] = FALSE;
  $retval['token'] = drupal_get_token(FILEDEPOT_TOKEN_FILEDETAILS);
  $retval['deleteperm'] = FALSE;
  $retval['addperm'] = FALSE;
  $retval['lockperm'] = FALSE;
  $retval['notifyperm'] = FALSE;
  $retval['broadcastperm'] = FALSE;
  $retval['tags'] = '';

  $validfile = FALSE;
  if ($reportmode == 'approvals') {
    $id = intval($_POST['id']);
    if (db_query("SELECT count(*) FROM {filedepot_filesubmissions} WHERE id=:id",
        array(':id' => $id))->fetchField() == 1
    ) {
      $validfile = TRUE;
      $sql = "SELECT file.id as fid,file.cid,file.title,file.fname,file.date,file.size,file.version,file.submitter,file.tags,u.name, ";
      $sql .= "file.status,file.description,category.pid,category.name as folder,category.nid,file.version_note,tags ";
      $sql .= "FROM {filedepot_filesubmissions} file ";
      $sql .= "LEFT JOIN {filedepot_categories} category ON file.cid=category.cid ";
      $sql .= "LEFT JOIN {users} u ON u.uid=file.submitter ";
      $sql .= "WHERE file.id=:id ";
      $rec = db_query($sql, array(':id' => $id))->fetchAssoc();
      $retval = array_merge($retval, $rec);
      $retval['displayhtml'] = theme('filedepot_filedetail', array('fid' => $id, 'reportmode' => $reportmode));
      $retval['locked'] = FALSE;
      $retval['subscribed'] = FALSE;
    }

  }
  elseif ($reportmode == 'incoming') {
    $id = intval($_POST['id']);
    if (db_result(db_query("SELECT count(*) FROM {filedepot_import_queue} WHERE id=:id", array(':id' => $id))) == 1) {
      $validfile = TRUE;
      $sql = "SELECT file.id as fid,file.orig_filename as title,file.description,file.version_note,u.name ";
      $sql .= "FROM {filedepot_import_queue} file ";
      $sql .= "LEFT JOIN {users} u ON u.uid=file.uid ";
      $sql .= "WHERE file.id=:id ";
      $rec = db_query($sql, array(':id' => $id))->fetchAssoc();
      $retval = array_merge($retval, $rec);
      if (empty($retval['version_note'])) {
        $retval['version_note'] = '';
      }
      $retval['displayhtml'] = theme('filedepot_filedetail', array('fid' => $id, 'reportmode' => $reportmode));
      $retval['locked'] = FALSE;
      $retval['subscribed'] = FALSE;
      // Need to reference a valid filedepot_folder node for the filedepot_download callback to work - required for the File Details 'Download' menuitem
      $retval['nid'] = db_query_range("SELECT nid FROM {filedepot_categories} WHERE pid=0", 0, 1, array())->fetchField();
    }

  }
  else {
    // Check that record exists
    $fid = intval($_POST['id']);
    $cid = db_query("SELECT cid FROM {filedepot_files} WHERE fid=:fid", array(':fid' => $fid))->fetchField();
    if ($filedepot->checkPermission($cid, 'view') AND db_query("SELECT count(*) FROM {filedepot_files} WHERE fid=:fid",
        array(':fid' => $fid))->fetchField() == 1
    ) {
      $validfile = TRUE;

      $sql = "SELECT file.fid,file.cid,file.title,file.description,file.fname,file.date,file.size,file.version,file.submitter,u.name, ";
      $sql .= "file.status,category.pid,category.name as folder,category.nid,v.notes as version_note,file.status_changedby_uid ";
      $sql .= "FROM {filedepot_files} file ";
      $sql .= "LEFT JOIN {filedepot_categories} category ON file.cid=category.cid ";
      $sql .= "LEFT JOIN {filedepot_fileversions} v ON v.fid=file.fid ";
      $sql .= "LEFT JOIN {users} u ON u.uid=file.submitter ";
      $sql .= "WHERE file.fid=:fid ORDER BY v.version DESC";
      $rec = db_query($sql, array(':fid' => $fid))->fetchAssoc();
      $retval = array_merge($retval, $rec);

      $retval['tags'] = $nexcloud->get_itemtags($fid);
      $retval['displayhtml'] = theme('filedepot_filedetail', array('fid' => $fid, 'reportmode' => $reportmode));

      // Check if file is locked
      if (($retval['status']) == FILEDEPOT_LOCKED_STATUS) {
        $retval['locked'] = TRUE;
      }
      else {
        $retval['locked'] = FALSE;
      }

      // Check and see if user has subscribed to this file
      $direct = FALSE;
      $ignorefilechanges = FALSE;
      // Check if user has an ignore file changes record or a subscribe to changes record for this file
      $query = db_query("SELECT fid,ignore_filechanges FROM {filedepot_notifications} WHERE fid=:fid and uid=:uid",
        array(':fid' => $fid, ':uid' => $user->uid));
      $A = $query->fetchAssoc();
      if ($A['ignore_filechanges'] == 1) {
        $ignorefilechanges = TRUE;
      }
      elseif ($A['fid'] == $fid) {
        $direct = TRUE;
      }
      // Check and see if user has indirectly subscribed to file by subscribing to folder
      $sql = "SELECT count(*) FROM {filedepot_notifications} WHERE cid_changes=1 AND cid=:cid AND uid=:uid";
      $indirect = db_query($sql, array(':cid' => $rec['cid'], ':uid' => $user->uid))->fetchField();
      if (($direct or $indirect) AND !$ignorefilechanges) {
        $retval['subscribed'] = TRUE;
      }
      else {
        $retval['subscribed'] = FALSE;
      }
    }
  }

  if ($validfile) {
    $retval['error'] = '';
    $retval['retcode'] = 200;
    if ($reportmode == 'incoming') {
      $retval['downloadperm'] = TRUE;
      $retval['editperm'] = TRUE;
      $retval['deleteperm'] = TRUE;
      $retval['addperm'] = FALSE;
      $retval['lockperm'] = FALSE;
      $retval['notifyperm'] = FALSE;
      $retval['broadcastperm'] = FALSE;
      $folderoptions = filedepot_recursiveAccessOptions('admin', 0);
      $retval['folderoptions'] = '<select name="folder" style="width:220px;">' . $folderoptions . '</select>';
    }
    else {
      $retval['dispfolder'] = $retval['folder'];
      $retval['description'] = nl2br($retval['description']);
      $retval['version_note'] = nl2br($retval['version_note']);
      $retval['date'] = strftime('%b %d %Y %I:%M %p', $retval['date']);
      $retval['size'] = filedepot_formatFileSize($retval['size']);

      // Setup the folder option select HTML options
      $cid = intval($retval['cid']);
      $folderoptions = filedepot_recursiveAccessOptions('admin', $cid, 0, 1, FALSE);
      if (!empty($folderoptions) AND $filedepot->checkPermission($retval['cid'], 'admin')) {
        $retval['folderoptions'] = '<select name="folder" style="width:220px;">' . $folderoptions . '</select>';
      }
      else {
        $retval['folderoptions'] = '<input type="text" name="folder" value="' . $retval['folder'] . '" READONLY />';
      }
      if ($filedepot->checkPermission($retval['cid'], 'admin')) {
        $retval['downloadperm'] = TRUE;
        $retval['editperm'] = TRUE;
        $retval['deleteperm'] = TRUE;
        $retval['addperm'] = TRUE;
        $retval['lockperm'] = TRUE;
        $retval['notifyperm'] = TRUE;
        $retval['broadcastperm'] = TRUE;
      }
      elseif ($retval['locked']) {
        if ($retval['status_changedby_uid'] == $user->uid) {
          $retval['lockperm'] = TRUE;
          if ($filedepot->checkPermission($retval['cid'], 'upload_ver')) {
            $retval['addperm'] = TRUE;
          }
          if ($retval['submitter'] == $user->uid) {
            $retval['deleteperm'] = TRUE;
          }
        }
        elseif ($retval['status_changedby_uid'] > 0) {
          if ($retval['submitter'] == $user->uid) {
            $retval['lockperm'] = TRUE;
          }
          else {
            $retval['downloadperm'] = FALSE;
          }
        }
        $retval['notifyperm'] = TRUE;
      }
      elseif ($user->uid > 0) {
        if ($retval['submitter'] == $user->uid) {
          $retval['deleteperm'] = TRUE;
          $retval['lockperm'] = TRUE;
        }
        if ($filedepot->checkPermission($retval['cid'], 'upload_ver')) {
          $retval['addperm'] = TRUE;
        }
        $retval['notifyperm'] = TRUE;
      }

      // Changed
      if ($filedepot->checkPermission($retval['cid'], 'view', 0, TRUE)) {
        $retval['tagperms'] = TRUE; // Able to set or change tags
        if ($retval['locked']) {
          if ($retval['submitter'] == $user->uid OR $retval['status_changedby_uid'] == $user->uid) {
            $retval['downloadperm'] = TRUE;
          }
          elseif (variable_get('filedepot_locked_file_download_enabled', 0) == 1) { // Check admin config setting
            $retval['downloadperm'] = TRUE;
          }
          else {
            $retval['downloadperm'] = FALSE;
          }
        }
        else {
          $retval['downloadperm'] = TRUE;
          if ($retval['submitter'] == $user->uid) {
            $retval['editperm'] = TRUE;
          }
        }
      }
      else {
        $retval['tagperms'] = FALSE;
        $retval['downloadperm'] = FALSE;
      }

    }

  }
  else {
    $retval['retcode'] = 400;
    $retval['error'] = t('Invalid access');
  }

  return $retval;
}


function filedepotAjaxServer_getMoreActions($op) {
  $retval = '<option value="0">' . t('More Actions') . '...</option>';
  switch ($op) {
    case 'approvals':
      $retval .= '<option value="approvesubmissions">' . t('Approve selected Submissions') . '</option>';
      $retval .= '<option value="deletesubmissions">' . t('Delete selected Submissions') . '</option>';
      break;
    case 'incoming':
      $retval .= '<option value="delete">' . t('Delete selected files') . '</option>';
      $retval .= '<option value="move">' . t('Move selected files') . '</option>';
      break;
    case 'notifications':
      $retval .= '<option value="delete">' . t('Delete selected Notifications') . '</option>';
      break;
    default:
      if (!user_is_logged_in()) {
        // $retval .= '<option value="archive">'. t ('Download as an archive') . '</option>';
      }
      else {
        $retval .= '<option value="delete">' . t('Delete selected files') . '</option>';
        $retval .= '<option value="move">' . t('Move selected files') . '</option>';
        $retval .= '<option value="subscribe">' . t('Subscribe to update notifications') . '</option>';
        // $retval .= '<option value="archive">' . t('Download as an archive') . '</option>';
        $retval .= '<option value="markfavorite">' . t('Mark Favorite') . '</option>';
        $retval .= '<option value="clearfavorite">' . t('Clear Favorite') . '</option>';
        $retval .= '<option value="download">' . t('Download Archive') . '</option>';
      }
      break;

  }
  return $retval;
}

function filedepotAjaxServer_deleteCheckedFiles() {
  global $user;
  $filedepot = filedepot_filedepot();

  $retval = array();

  $cid = intval($_POST['cid']);
  $reportmode = check_plain($_POST['reportmode']);
  $fileitems = check_plain($_POST['checkeditems']);
  $files = explode(',', $fileitems);
  $delerror = FALSE;

  if (!empty($_POST['checkedfolders'])) {
    $folderitems = check_plain($_POST['checkedfolders']);
    $folders = explode(',', $folderitems);
    foreach ($folders as $id) {
      if ($reportmode == 'notifications') {
        if ($id > 0 AND db_query("SELECT uid FROM {filedepot_notifications} WHERE id=:id",
            array(':id' => $id))->fetchField() > 0
        ) {
          db_query("DELETE FROM {filedepot_notifications} WHERE id=:id",
            array(':id' => $id));
        }
      }
      elseif ($id > 0 AND $_POST['multiaction'] == 'delete' AND $filedepot->checkPermission($id, 'admin')) {
        $nid = db_query("SELECT nid FROM {filedepot_categories} WHERE cid=:cid",
          array(':cid' => $id))->fetchField();
        if ($filedepot->deleteFolder($nid)) {
          // Remove any recent folder records for this category
          db_query("DELETE FROM {filedepot_recentfolders} WHERE cid=:cid",
            array(':cid' => $id));
        }
        else {
          $delerror = TRUE;
        }
      }
    }
  }

  if ($reportmode == 'incoming') {
    foreach ($files as $id) {
      if (db_query("SELECT COUNT(*) FROM {filedepot_import_queue} WHERE id=:id",
          array(':id' => $id))->fetchField() == 1
      ) {
        $query = db_query("SELECT drupal_fid,filepath,uid FROM {filedepot_import_queue} WHERE id=:id",
          array(':id' => $id));
        $file = $query->fetchObject();
        if ($file->uid == $user->uid OR user_access('administer filedepot', $user)) {
          if (!empty($file->filepath) AND file_exists($file->filepath)) {
            @unlink($file->filepath);
          }
          db_query("DELETE FROM {files} WHERE fid=:fid",
            array('fid' => $file->drupal_fid));
          db_query("DELETE FROM {filedepot_import_queue} WHERE id=:id",
            array(':id' => $id));
        }
      }
    }
  }
  elseif ($reportmode == 'notifications') {
    foreach ($files as $id) {
      $uid = db_query("SELECT uid FROM {filedepot_notifications} WHERE id=:id",
        array(':id' => $id))->fetchField();
      if ($id > 0 AND $uid == $user->uid) {
        db_query("DELETE FROM {filedepot_notifications} WHERE id=:id",
          array(':id' => $id));
      }
    }
  }
  else {
    foreach ($files as $id) {
      if ($id > 0) {
        if ($filedepot->deleteFile($id) === FALSE) {
          $delerror = TRUE;
        }
      }
    }
  }

  if (!in_array($reportmode, $filedepot->validReportingModes)) {
    $filedepot->ajaxBackgroundMode = TRUE;
  }
  $filedepot->cid = $cid;
  $filedepot->activeview = $reportmode;
  $retval['retcode'] = 200;
  $retval['errmsg'] = '';
  if ($reportmode == 'notifications') {
    $retval['displayhtml'] = theme('filedepot_notifications');
  }
  else {
    $retval['displayhtml'] = filedepot_displayFolderListing($filedepot->cid);
  }
  $retval['activefolder'] = theme('filedepot_activefolder');
  if ($delerror == TRUE) {
    $retval['errmsg'] = t('Error deleting one or more items - invalid permissions');
  }

  if (is_array($filedepot->lastRenderedFiles) AND count($filedepot->lastRenderedFiles) > 0) {
    $retval['lastrenderedfiles'] = json_encode($filedepot->lastRenderedFiles);
  }
  return $retval;
}

function filedepotAjaxServer_deleteFile($fid) {
  $filedepot = filedepot_filedepot();
  $retval = array();
  $reportmode = check_plain($_POST['reportmode']);
  $listing_folder = intval($_POST['listingcid']);
  $filedepot->cid = $listing_folder;
  $filedepot->activeview = $reportmode;

  $retval['fid'] = $fid;
  if ($reportmode == 'approvals') {
    $retval['cid'] = db_query("SELECT cid FROM {filedepot_filesubmissions} WHERE id=:fid",
      array(':fid' => $fid))->fetchField();
  }
  elseif ($reportmode == 'incoming') {
    $drupal_fid = db_query("SELECT drupal_fid FROM {filedepot_import_queue} WHERE id=:fid",
      array(':fid' => $fid))->fetchField();
  }
  else {
    $retval['cid'] = db_query("SELECT cid FROM {filedepot_files} WHERE fid=:fid",
      array(':fid' => $fid))->fetchField();
  }
  $message = '';
  if ($reportmode == 'approvals' AND $filedepot->checkPermission($retval['cid'], 'approval')) {
    if ($filedepot->deleteSubmission($fid)) {
      $retval['retcode'] = 200;
      $message = '<div class="pluginInfo aligncenter" style="width:100%;height:60px;padding-top:30px;">';
      $message .= t('File was sucessfully deleted. This message will clear in a couple seconds');
      $message .= '</div>';
      $retval['displayhtml'] = filedepot_displayFolderListing($listing_folder);
    }
    else {
      $retval['retcode'] = 500;
    }
  }
  elseif ($reportmode == 'incoming') {
    if ($drupal_fid > 0) {
      $filepath = db_query("SELECT filepath FROM {files} WHERE fid=:fid",
        array(':fid' => $drupal_fid))->fetchField();
      if (!empty($filepath) AND file_exists($filepath)) {
        @unlink($filepath);
      }
      db_query("DELETE FROM {files} WHERE fid=:fid", array(':fid' => $drupal_fid));
      db_query("DELETE FROM {filedepot_import_queue} WHERE id=:fid", array(':fid' => $fid));
      $retval['retcode'] = 200;
      $filedepot->activeview = 'incoming';
      $retval['displayhtml'] = filedepot_displayFolderListing();
      $retval = filedepotAjaxServer_generateLeftSideNavigation($retval);
    }
    else {
      $retval['retcode'] = 500;
    }
  }
  elseif ($filedepot->deleteFile($fid)) { /* Includes security tests that user can delete this file */
    if (!in_array($reportmode, $filedepot->validReportingModes)) {
      $filedepot->ajaxBackgroundMode = TRUE;
    }
    $retval['retcode'] = 200;
    $message = '<div class="pluginInfo aligncenter" style="width:100%;height:60px;padding-top:30px;">';
    $message .= t('File was sucessfully deleted. This message will clear in a couple seconds');
    $message .= '</div>';
    $retval['displayhtml'] = filedepot_displayFolderListing($listing_folder);
    if (is_array($filedepot->lastRenderedFiles) AND count($filedepot->lastRenderedFiles) > 0) {
      $retval['lastrenderedfiles'] = json_encode($filedepot->lastRenderedFiles);
    }
  }
  else {
    $retval['retcode'] = 404;
  }

  $retval['message'] = $message;
  $retval['title'] = t('Delete Confirmation');
  return $retval;

}


function filedepotAjaxServer_updateFolder() {
  global $user;
  $filedepot = filedepot_filedepot();
  $cid = intval($_POST['cid']);
  $catpid = intval($_POST['catpid']);
  $folderorder = intval($_POST['folderorder']);
  $fileadded = intval($_POST['fileadded_notify']);
  $filechanged = intval($_POST['filechanged_notify']);
  $catname = check_plain($_POST['categoryname']);
  $catdesc = check_plain($_POST['catdesc']);

  $retval = array();

  if ($cid > 0 AND $filedepot->checkPermission($cid, 'admin')) {
    $retval['retcode'] = 200;
    $retval['cid'] = $cid;
    db_query("UPDATE {filedepot_categories} SET name=:catname, description=:desc WHERE cid=:cid",
      array(
        ':catname' => $catname,
        ':desc' => $catdesc,
        ':cid' => $cid,
      ));
    $nid = db_query("SELECT nid FROM {filedepot_categories} WHERE cid=:cid",
      array(':cid' => $cid))->fetchField();
    db_query("UPDATE {node} SET title=:catname WHERE nid=:nid",
      array(':catname' => $catname, ':nid' => $nid));
    db_query("UPDATE {node_revision} SET title=:catname WHERE nid=:nid",
      array(':catname' => $catname, ':nid' => $nid));
    if (db_query("SELECT folderorder FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $cid))->fetchField() != $folderorder) {
      db_query("UPDATE {filedepot_categories} SET folderorder=:folder WHERE cid=:cid",
        array(':folder' => $folderorder, ':cid' => $cid));

      /* Re-order any folders that may have just been moved */
      $query = db_query("SELECT cid,folderorder from {filedepot_categories} WHERE pid=:pid ORDER BY folderorder",
        array(':pid' => $catpid));
      $folderorder = 10;
      $stepnumber = 10;
      while ($A = $query->fetchAssoc()) {
        if ($A['folderorder'] != $folderorder) {
          db_query("UPDATE {filedepot_categories} SET folderorder=:folder WHERE cid=:cid",
            array(
              ':folder' => $folderorder,
              ':cid' => $A['cid'],
            ));
        }
        $folderorder += $stepnumber;
      }
    }

    // Update the personal folder notifications for user
    if ($filechanged == 1 OR $fileadded == 1) {
      if (db_query("SELECT count(*) FROM {filedepot_notifications} WHERE cid=:cid AND uid=:uid",
          array(
            ':cid' => $cid,
            ':uid' => $user->uid,
          ))->fetchField() == 0
      ) {
        $sql = "INSERT INTO {filedepot_notifications} (cid,cid_newfiles,cid_changes,uid,date) ";
        $sql .= "VALUES (:cid,:added,:changed,:uid,:time)";
        db_query($sql,
          array(
            ':cid' => $cid,
            ':added' => $fileadded,
            ':changed' => $filechanged,
            ':uid' => $user->uid,
            ':time' => time(),
          ));
      }
      else {
        $sql = "UPDATE {filedepot_notifications} set cid_newfiles=:added, ";
        $sql .= "cid_changes=:changed, date=:time ";
        $sql .= "WHERE uid=:uid and cid=:cid";
        db_query($sql,
          array(
            ':added' => $fileadded,
            ':changed' => $filechanged,
            ':time' => time(),
            ':uid' => $user->uid,
            ':cid' => $cid,
          ));
      }
    }
    else {
      db_query("DELETE FROM {filedepot_notifications} WHERE uid=:uid AND cid=:cid",
        array(
          ':uid' => $user->uid,
          ':cid' => $cid,
        ));
    }

    // Now test if user has requested to change the folder's parent and if they have permission to this folder
    $pid = db_query("SELECT pid FROM {filedepot_categories} WHERE cid=:cid",
      array(
        ':cid' => $cid,
      ))->fetchField();
    if (($pid != $catpid) && ($catpid != $cid)) {
      if ($filedepot->checkPermission($catpid, 'admin') OR user_access('administer filedepot')) {
        // Check if user is trying to set the folder's parent to a child folder - ERROR!
        $children = $filedepot->getFolderChildren($cid);
        if (!in_array($catpid, $children)) {
          db_query("UPDATE {filedepot_categories} SET pid=:pid WHERE cid=:cid",
            array(
              ':pid' => $catpid,
              ':cid' => $cid,
            ));
          // Need to force a reset of user accessible folders in case folder has been moved under a parent with restricted access
          db_update('filedepot_usersettings')
            ->fields(array('allowable_view_folders' => ''))
            ->execute();

          // If the folder is now a top level folder - then remove it from the recent folders list as top level don't appear.
          if (($filedepot->ogmode_enabled AND $catpid == $filedepot->ogrootfolder) OR $catpid == 0) {
            db_query("DELETE FROM {filedepot_recentfolders} WHERE cid=:cid ", array(':cid' => $cid));
          }
        }
        else {
          watchdog('filedepot', "Attempt to set the parent folder for :foldername to a current child folder", array(':foldername' => $catname));
          $retval['retcode'] = 500;
        }
      }
      else {
        $retval['retcode'] = 500;
      }
    }

  }
  else {
    $retval['retcode'] = 500;
  }
  return $retval;
}


function filedepotAjaxServer_moveCheckedFiles() {
  global $user;
  $filedepot = filedepot_filedepot();
  $message = '';
  $retval = array();
  $cid = intval($_POST['cid']);
  $newcid = intval($_POST['newcid']);
  $reportmode = check_plain($_POST['reportmode']);
  $fileitems = check_plain($_POST['checkeditems']);
  $files = explode(',', $fileitems);

  $filedepot->cid = $cid;
  $filedepot->activeview = $reportmode;

  $movedfiles = 0;
  if ($newcid > 0 AND $user->uid > 0) {
    foreach ($files as $id) {
      if ($id > 0) {
        if ($reportmode == 'incoming') {
          if ($filedepot->moveIncomingFile($id, $newcid)) {
            $movedfiles++;
          }
        }
        else {
          if ($filedepot->moveFile($id, $newcid)) {
            $movedfiles++;
          }
        }
      }
    }
  }

  if ($movedfiles > 0) {
    $message = "Successfully moved $movedfiles files to this folder.";
    if ($reportmode == 'incoming') {
      // Send out email notifications of new file added to all users subscribed  -  Get fileid for the new file record
      $args = array(
        ':cid' => $newcid,
        ':uid' => $user->uid,
      );
      $fid = db_query_range("SELECT fid FROM {filedepot_files} WHERE cid=:cid AND submitter=:uid ORDER BY fid DESC", 0, 1, $args)->fetchField();
      filedepot_sendNotification($fid, FILEDEPOT_NOTIFY_NEWFILE);
    }
    $cid = $newcid;
  }
  elseif ($newcid == 0) {
    $message = 'Unable to move any files - Invalid new folder selected';
  }
  else {
    $message = 'Unable to move any files - invalid folder or insufficient rights';
  }

  $retval['retcode'] = 200;
  $retval['cid'] = $cid;
  $retval['movedfiles'] = $movedfiles;
  $retval['message'] = $message;
  $retval['activefolder'] = theme('filedepot_activefolder');
  $retval['displayhtml'] = filedepot_displayFolderListing($cid);

  return $retval;


}

function filedepotAjaxServer_updateFileSubscription($fid, $op = 'toggle') {
  global $user;

  $retval = array(
    'retcode' => '',
    'subscribed' => '',
  );
  if ($user->uid > 0) {
    $uid = $user->uid;
  }
  else {
    $retval['retcode'] = FALSE;
    return $retval;
  }

  if (db_query("SELECT count(fid) FROM {filedepot_files} WHERE fid=:fid", array(':fid' => $fid))->fetchField() == 1) { // Valid file and user
    $cid = db_query("SELECT cid FROM {filedepot_files} WHERE fid=:fid", array(':fid' => $fid))->fetchField();
    // Check if user has an ignore file changes record or a subscribe to changes record for this file
    $direct = FALSE;
    $ignorefilechanges = FALSE;
    $query = db_query("SELECT fid,ignore_filechanges FROM {filedepot_notifications} WHERE fid=:fid and uid=:uid",
      array(':fid' => $fid, ':uid' => $uid));
    if ($A = $query->fetchAssoc()) {
      if ($A['ignore_filechanges'] == 1) {
        $ignorefilechanges = TRUE;
      }
      else {
        $direct = TRUE;
      }
    }
    $indirect = db_query("SELECT cid_changes FROM {filedepot_notifications} WHERE cid=:cid AND uid=:uid",
      array(':cid' => $cid, ':uid' => $uid))->fetchField();
    if ($indirect AND $direct) { // User may have subscribed to single file and the folder option was also set
      if ($op == 'toggle' or $op == 'remove') {
        db_query("UPDATE {filedepot_notifications} set ignore_filechanges = 1 WHERE fid=:fid AND uid=:uid",
          array(':fid' => $fid, ':uid' => $uid));
        $retval['subscribed'] = FALSE;
      }
    }
    elseif (($direct OR $indirect) AND !$ignorefilechanges) { // User is subscribed - so un-subscribe
      if ($op == 'toggle' or $op == 'remove') {
        $retval['subscribed'] = FALSE;
        if ($direct > 0) {
          db_query("DELETE FROM {filedepot_notifications} WHERE fid=:fid AND uid=:uid",
            array(':fid' => $fid, ':uid' => $uid));
        }
        elseif ($indirect > 0) {
          db_query("INSERT INTO {filedepot_notifications} (fid,ignore_filechanges,uid,date) VALUES (:fid,1,:uid,:time)",
            array(':fid' => $fid, ':uid' => $uid, ':time' => time()));
        }
      }

    }
    else { // User is not subscribed
      if ($op == 'toggle' OR $op == 'add') {
        $retval['subscribed'] = TRUE;
        if ($ignorefilechanges) {
          //delete the exception record
          db_query("UPDATE {filedepot_notifications} set ignore_filechanges = 0 WHERE fid=:fid AND uid=:uid",
            array(':fid' => $fid, ':uid' => $uid));
        }
        elseif (!$direct AND !$indirect) {
          db_query("INSERT INTO {filedepot_notifications} (fid,cid,uid,date) VALUES (:fid,:cid,:uid,:time)",
            array(':fid' => $fid, ':cid' => $cid, ':uid' => $uid, ':time' => time()));
        }
      }

    }
    $retval['retcode'] = TRUE;

  }
  else {
    $retval['retcode'] = FALSE;
  }

  return $retval;

}


function filedepotAjaxServer_broadcastAlert($fid, $comment) {
  global $user;

  $retval = '';
  $target_users = filedepot_build_notification_distribution($fid, FILEDEPOT_BROADCAST_MESSAGE);
  if (count($target_users) > 0) {
    $values = array(
      'fid' => $fid,
      'comment' => filter_xss($comment),
      'target_users' => $target_users,
    );
    $ret = drupal_mail('filedepot', FILEDEPOT_BROADCAST_MESSAGE, $user, language_default(), $values);
    if ($ret) {
      $filedepot = filedepot_filedepot();
      $retval['retcode'] = 200;
      $retval['count'] = $filedepot->message;
    }
    else {
      $retval['retcode'] = 205;
    }
  }
  else {
    $retval['retcode'] = 205;
  }
  return $retval;
}