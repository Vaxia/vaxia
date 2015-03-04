<?php

/**
 * @file
 * lib-theme.php
 * Theme support functions for the module
 */



function template_preprocess_filedepot_toolbar_form(&$variables) {
  global $base_url;

  $variables['report_option'] = 'latestfiles';
  $variables['base_url']       = $base_url;
  if (!isset($_GET['cid'])) {
    $_GET['cid'] = 0;
  }
  if (intval($_GET['cid']) > 0) {
    $variables['current_category'] = intval($_GET['cid']);
  }

}

function template_preprocess_filedepot_header(&$variables) {
  $filedepot = filedepot_filedepot();
  $variables['show_mainheader']     = '';
  $variables['show_incomingheader'] = 'none';
  $variables['LANG_filename'] = t('Filename');
  $variables['LANG_showfiledetails'] = t('Show File Details');
  $variables['LANG_expandfolders'] = t('Expand Folders');
  $variables['LANG_date'] = t('Date');
  $variables['LANG_folder'] = t('Folder');
  $variables['LANG_submitted'] = t('Submitted');
  $variables['LANG_owner'] = t('Owner');
  $variables['rightpadding'] = '35'; // Need to tweek the right most padding of the far right heading column for the approvals report view.



  if ($filedepot->activeview == 'incoming') {
    $variables['show_incomingheader'] = '';
    $variables['show_mainheader'] = 'none';
  }

  if ($filedepot->cid > 0) {
    $variables['rightpadding'] = '35';
    $variables['show_folder'] = 'none';
    $variables['show_folderexpandlink'] = '';
  }
  elseif ($filedepot->activeview == 'approvals') {
    $variables['rightpadding'] = '10';
    $variables['show_folder'] = '';
    $variables['show_folderexpandlink'] = 'none';
  }
  else {
    $variables['show_folder'] = '';
    $variables['show_folderexpandlink'] = 'none';
  }

  if ($filedepot->activeview == 'approvals') {
    $variables['LANG_action'] = t('Submitter');
  }
  else {
    $variables['LANG_action'] = t('Action');
  }

  if ($filedepot->activeview == 'incoming' AND user_access('administer filedepot', $user)) {
    $variables['show_owner'] = '';
  }
  else {
    $variables['show_owner'] = 'none';
  }
}

function template_preprocess_filedepot_folder_breadcrumb(&$variables) {
  if ($variables['cid'] > 0) {
    $foldername = db_query("SELECT name FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $variables['cid']))->fetchField();
    $variables['catid'] = $variables['cid'];
    $variables['padding_left'] = $variables['padding'];
    $variables['folder_name'] = filter_xss($foldername);
  }
}


function template_preprocess_filedepot_activefolder_nonadmin(&$variables) {
  global $user;

  $filedepot = filedepot_filedepot();
  $foldername = db_query("SELECT name FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $filedepot->cid))->fetchField();
  $variables['active_category_id'] = $filedepot->cid;
  $variables['active_folder_name'] = filter_xss($foldername);
  $variables['LANG_folderoptions'] = t('Folder Notification Options');
  $variables['LANG_newfiles'] = t('Alert me if new files are added');
  $variables['LANG_filechanges'] = t('Alert me if files are changed');

  $variables['chk_fileadded'] = '';
  $variables['chk_filechanged'] = '';

  $query = db_query("SELECT cid_newfiles,cid_changes FROM {filedepot_notifications} WHERE cid=:cid AND uid=:uid", array(':cid' => $filedepot->cid, ':uid' => $user->uid));
  if ($query) {
    $B = $query->fetchAssoc();
    if ($B['cid_newfiles'] == 1) {
      $variables['chk_fileadded'] = "CHECKED=checked";
    }
    if ($B['cid_changes'] == 1) {
      $variables['chk_filechanged'] = "CHECKED=checked";
    }
  }
}

function template_preprocess_filedepot_activefolder_admin(&$variables) {
  global $user;

  $filedepot = filedepot_filedepot();
  $variables['LANG_click_adminmsg'] = t('Click to edit folder options or administrate folder');
  $variables['LANG_description'] = t('Description');
  $variables['LANG_folderorder'] = t('Folder Order');
  $variables['LANG_folderordermsg'] = t('Displayed in increments of 10 for easy editing');
  $variables['LANG_newfiles'] = t('Alert me if new files are added');
  $variables['LANG_filechanges'] = t('Alert me if files are changed');
  $variables['LANG_statsmsg'] = t('Folder & Sub Folders Stats');
  $variables['LANG_foldercount'] = t('Folder Count');
  $variables['LANG_filecount'] = t('File Count');
  $variables['LANG_totalsize'] = t('Total Size');
  $variables['ajax_server_url'] = url('filedepot_ajax');

  // Folder Stats


  $list = array();
  array_push($list, $filedepot->cid);
  $filedepot->getRecursiveCatIDs($list, $filedepot->cid, 'view');
  $variables['folder_count'] = count($list);
  $numfiles = 0;
  $totalsize = 0;
  foreach ($list as $folderid) {
    $q = db_query("SELECT count(fid) as filecount,sum(size) as filesize FROM {filedepot_files} WHERE cid=:cid GROUP BY cid", array(':cid' => $folderid));
    $A = $q->fetchAssoc();
    $numfiles = $numfiles + $A['filecount'];
    $totalsize = $totalsize + $A['filesize'];
  }
  $variables['file_count'] = $numfiles;
  $variables['total_size'] = filedepot_formatFileSize($totalsize);

  $A = db_query("SELECT pid,name,description,folderorder FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $filedepot->cid))->fetchAssoc();
  $variables['folderorder'] = $A['folderorder'];
  $variables['active_category_id'] = $filedepot->cid;
  $variables['active_folder_name'] = filter_xss($A['name']);
  $variables['folder_description'] = filter_xss($A['description']);

  $options = filedepot_recursiveAccessOptions('admin', $A['pid']);
  $variables['folder_parent_options'] = $options;

  $variables['chk_fileadded'] = '';
  $variables['chk_filechanged'] = '';

  $query = db_query("SELECT cid_newfiles,cid_changes FROM {filedepot_notifications} WHERE cid=:cid AND uid=:uid", array(':cid' => $filedepot->cid, ':uid' => $user->uid));
  if ($query) {
    $B = $query->fetchAssoc();
    if ($B['cid_newfiles'] == 1) {
      $variables['chk_fileadded'] = "CHECKED=checked";
    }
    if ($B['cid_changes'] == 1) {
      $variables['chk_filechanged'] = "CHECKED=checked";
    }
  }

}


function template_preprocess_filedepot_activefolder(&$variables) {
  $filedepot = filedepot_filedepot();
  $variables['show_activefolder'] = 'none';
  $variables['show_reportmodeheader'] = 'none';
  $variables['show_nonadmin'] = 'none';
  $variables['show_breadcrumbs'] = 'none';
  $variables['folder_breadcrumb_links'] = '';
  $variables['report_heading'] = '';
  $variables['active_folder_admin'] = '';

  if ($filedepot->cid == 0) {
    if (in_array($filedepot->activeview, $filedepot->validReportingModes)) {
      if ($filedepot->activeview == 'search') {
        $variables['report_heading'] = t("@active returned %count records", array(
            "@active" => $filedepot->activeview,
            "%count" => $filedepot->recordCount
          ));
      }
      else {
        $variables['report_heading'] = t("@active", array(
            "@active" => $filedepot->activeview
          ));
      }
      $variables['show_reportmodeheader'] = '';
    }
  }
  else {
    $variables['show_activefolder'] = '';
    $pid = db_query("SELECT pid FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $filedepot->cid))->fetchField();
    if ($pid != 0) {
      $parent = $pid;
      $rootfolder = $filedepot->cid;
      while ($parent != 0) { // Determine the rootfolder
        $rootfolder = $parent;
        $parent = db_query("SELECT pid FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $parent))->fetchField();
      }
      $variables['folder_breadcrumb_links'] = theme('filedepot_folder_breadcrumb', array('cid' => $rootfolder, 'padding' => 0));
      if ($rootfolder != $pid) {
        $query = db_query("SELECT cid from {filedepot_categories} WHERE cid=:cid", array(':cid' => $pid));
        $A = $query->fetchAssoc();
        $variables['folder_breadcrumb_links'] .= theme('filedepot_folder_breadcrumb', array('cid' => $A['cid'], 'padding' => 5));
      }
      $variables['show_breadcrumbs'] = 'block';
    }
    if ($filedepot->checkPermission($filedepot->cid, 'admin')) {
      $variables['active_folder_admin'] = theme('filedepot_activefolder_admin', array('token' => $variables['token']));
    }
    else {
      $variables['show_nonadmin'] = '';
      $variables['active_folder_admin'] = theme('filedepot_activefolder_nonadmin');
    }
  }

  $variables['ajaxstatus'] = theme('filedepot_ajaxstatus');
  $variables['ajaxactivity'] = theme('filedepot_ajaxactivity', array('layout_url' => $variables['layout_url']));

}


function template_preprocess_filedepot_folderlisting(&$variables) {
  $filedepot = filedepot_filedepot();
  $rec = $variables['folderrec']; // cid,pid,name,description,folderorder,last_modified_date


  $level = $variables['level'];
  $variables['padding_right'] = 0;
  $variables['folder_desc_padding_left'] = 23 + (($level) * 30); // Version 3.0 - not presently used


  $variables['folder_id'] = $rec['cid'];
  $variables['parent_folder_id'] = $rec['pid'];
  $variables['folder_name'] = filter_xss($rec['name']);
  $variables['folder_description'] = filter_xss($rec['description']);

  if (variable_get('filedepot_show_index_enabled', 1) == 1) { // Check admin config setting
    $variables['folder_number'] = "{$variables['folderprefix']}.0";
  }
  else {
    $variables['folder_number'] = '';
  }
  if ($rec['last_modified_date'] > 0) {
    $variables['last_modified_date'] = strftime($filedepot->shortdate, $rec['last_modified_date']);
  }
  else {
    $variables['last_modified_date'] = '';
  }

  // For the checkall files - need to set the inline files


  // and can't be done in filedepot_displayFileListing since a folder can have subfolders


  // and template var in parent folder is being over-written


  $query_files = db_query("SELECT fid from {filedepot_files} WHERE cid=:cid", array(':cid' => $rec['cid']));
  $files = array();
  while ($A = $query_files->fetchAssoc()) {
    $files[] = $A['fid'];
  }
  $variables['folder_files'] = implode(',', $files);
  if ($filedepot->checkPermission($rec['cid'], 'admin')) {
    $variables['onhover_move_options'] = theme('filedepot_folder_moveoptions', array(
      'folderid' => $rec['cid'],
      'order' => $rec['folderorder'],
      'maxorder' => $variables['maxorder'],
    )
    );
  }
  else {
    $variables['onhover_move_options'] = '';
  }
  $variables['folder_padding_left'] = $level * $filedepot->listingpadding;
}

function template_preprocess_filedepot_folder_moveoptions(&$variables) {
  $variables['LANG_moveup'] = t('Move Folder Up');
  $variables['LANG_movedown'] = t('Move Folder Down');
  if ($variables['order'] == 10) {
    $variables['hide_moveup'] = 'none';
  }
  else {
    $variables['hide_moveup'] = '';
  }
  if ($variables['order']  < $variables['maxorder']) {
    $variables['hide_movedown'] = '';
  }
  else {
    $variables['hide_movedown'] = 'none';
  }
}


function template_preprocess_filedepot_filelisting(&$variables) {
  global $user;

  $filedepot = filedepot_filedepot();
  $nexcloud = filedepot_nexcloud();
  /* listing rec format
   file.fid as fid,file.cid,file.title,file.fname,file.date,file.version,file.submitter,file.status,
   detail.description,category.name as foldername,category.pid,category.last_modified_date,status_changedby_uid as changedby_uid, size
   */
  $rec = $variables['listingrec'];
  $level = $variables['level'];
  $variables['subfolder_id'] = $rec['cid'];
  $variables['show_submitter'] = 'none';
  $variables['padding_left'] = ($level * $filedepot->listingpadding) + $filedepot->listingpadding;
  $variables['file_desc_padding_left'] = $filedepot->filedescriptionOffset + ($level * $filedepot->listingpadding);
  $variables['locked_icon'] = base_path() . drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('locked');
  $variables['submitter'] = '';
  $variables['favorite_status_image'] = '';
  $variables['filesize'] = '';
  if ($rec['status'] == 2) {
    $variables['show_lock'] = '';
  }
  else {
    $variables['show_lock'] = 'none';
  }
  $variables['details_link_parms'] = "?fid={$rec['fid']}";
  $variables['fid'] = $rec['fid'];
  if (isset($rec['size'])) {
    $variables['filesize'] = filedepot_formatFileSize($rec['size']);
  }
  $variables['file_name'] = filter_xss($rec['title']);

  if (isset($rec['date']) AND $rec['date'] > 0) {
    $variables['modified_date'] = strftime($filedepot->shortdate, $rec['date']);
  }
  else {
    $variables['modified_date'] = '';
  }

  $variables['folder_link'] = url('filedepot', array('query' => array('cid' => $rec['cid']), 'absolute' => true));
  $variables['folder_name'] = filter_xss($rec['foldername']);
  $filenum = $rec['fileorder']/10;
  if (variable_get('filedepot_show_index_enabled', 1) == 1) { // Check admin config setting
    $variables['file_number'] = "{$rec['folderprefix']}.{$filenum}";
  }
  else {
    $variables['file_number'] = '';
  }
  $variables['file_description'] = nl2br(filter_xss($rec['description']));
  $variables['actionclass'] = 'twoactions';

  $tags = $nexcloud->get_itemtags($variables['fid']);
  $variables['tags'] = filedepot_formatfiletags($tags);

  $variables['show_favorite'] = FALSE;
  if ($rec['status'] > 0 AND user_is_logged_in()) {
    $variables['show_favorite'] = TRUE;
    if (db_query("SELECT count(fid) FROM {filedepot_favorites} WHERE uid=:uid AND fid=:fid", array(':uid' => $user->uid, ':fid' => $variables['fid']))->fetchField() > 0) {
      $variables['favorite_status_image'] = "{$variables['layout_url']}/css/images/{$filedepot->iconmap['favorite-on']}";
      $variables['LANG_favorite_status'] = t('Click to clear favorite');
    }
    else {
      $variables['favorite_status_image'] = "{$variables['layout_url']}/css/images/{$filedepot->iconmap['favorite-off']}";
      $variables['LANG_favorite_status'] = t('Click to mark item as a favorite');
    }
  }

  $variables['show_approvalsubmitter'] = 'none';
  $variables['show_foldername'] = '';
  if ($filedepot->activeview == 'approvals') {
    $variables['show_approvalsubmitter'] = '';
    $variables['show_submitter'] = 'none';
    $variables['submitter'] = db_query("SELECT name FROM {users} WHERE uid=:uid", array(':uid' => $rec['submitter']))->fetchField();
  }
  elseif ($filedepot->activeview == 'incoming') {
    $movelink = "<a class=\"moveincoming\" href=\"?fid={$rec['fid']}\" onClick=\"return false;\">" . t('Move') . '</a>';
    $deletelink = "<a class=\"deleteincoming\" href=\"?fid={$rec['fid']}\" onClick=\"return false;\">" . t('Delete') . '</a>';
    $variables['action1_link'] = $movelink;
    $variables['action2_link'] = $deletelink;
    $variables['submitter'] = db_query("SELECT name FROM {users} WHERE uid=:uid", array(':uid' => $rec['submitter']))->fetchField();
    $variables['show_submitter'] = '';
    $variables['show_foldername'] = 'none';
  }
  else {
    $folder_admin = $filedepot->checkPermission($rec['cid'], 'admin');
    if ($filedepot->cid > 0 OR empty($filedepot->activeview)) {
      $variables['show_foldername'] = 'none';
    }
    $variables['action1_link'] = '&nbsp;';
    $variables['action2_link'] = '&nbsp;';
    $variables['actionclass'] = 'noactions';
    $allowLockedFileDownloads = variable_get('filedepot_locked_file_download_enabled', 0); // Check admin config setting


    if ($rec['status'] == FILEDEPOT_LOCKED_STATUS) {
      if ($folder_admin OR $rec['changedby_uid'] == $user->uid) { // File locked and folder admin or file owner
        $path = drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('download');
        $downloadlinkimage = theme('image', array('path' => $path));
        $variables['action1_link'] =  l( $downloadlinkimage, "filedepot_download/{$rec['nid']}/{$rec['fid']}",
          array('html' => TRUE, 'attributes' => array('title' => t('Download File'))));
        if (FILEDEPOT_CLIENT_SUPPORT AND $user->uid > 0 AND $filedepot->checkPermission($rec['cid'], array('upload_dir'), $user->uid)) {
          $variables['actionclass'] = 'twoactions';
          $path = drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('editfile');
          $editlinkimage = theme('image', array('path' => $path));
          $variables['action2_link'] =  l( $editlinkimage, "filedepot_download/{$rec['nid']}/{$rec['fid']}/0/edit",
            array('html' => TRUE, 'attributes' => array('title' => t('Download for Editing'))));
        }
        else {
          $variables['action2_link'] = '';
          $variables['actionclass'] = 'oneaction';
        }
      }
      elseif ($allowLockedFileDownloads == 1) { // File locked and downloads allowed
        $path = drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('download');
        $downloadlinkimage = theme('image', array('path' => $path));
        $variables['action1_link'] =  l( $downloadlinkimage, "filedepot_download/{$rec['nid']}/{$rec['fid']}",
          array('html' => TRUE, 'attributes' => array('title' => t('Download File'))));
        $variables['action2_link'] = '';
        $variables['actionclass'] = 'oneaction';
      }
    }
    else {
      if ($folder_admin OR $rec['changedby_uid'] == $user->uid) {
        $path = drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('download');
        $downloadlinkimage = theme('image', array('path' => $path));
        $variables['action1_link'] =  l( $downloadlinkimage, "filedepot_download/{$rec['nid']}/{$rec['fid']}",
          array('html' => TRUE, 'attributes' => array('title' => t('Download File'))));
        if (FILEDEPOT_CLIENT_SUPPORT AND $user->uid > 0 AND $filedepot->checkPermission($rec['cid'], array('upload_dir'), $user->uid)) {
          $variables['actionclass'] = 'twoactions';
          $path = drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('editfile');
          $editlinkimage = theme('image', array('path' => $path));
          $variables['action2_link'] =  l( $editlinkimage, "filedepot_download/{$rec['nid']}/{$rec['fid']}/0/edit",
            array('html' => TRUE, 'attributes' => array('title' => t('Download for Editing'))));
        }
        else {
          $variables['action2_link'] = '';
          $variables['actionclass'] = 'oneaction';
        }
      }
      else {
        $path = drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('download');
        $downloadlinkimage = theme('image', array('path' => $path));
        $variables['action1_link'] =  l( $downloadlinkimage, "filedepot_download/{$rec['nid']}/{$rec['fid']}",
          array('html' => TRUE, 'attributes' => array('title' => t('Download File'))));
        $variables['action2_link'] = '';
        $variables['actionclass'] = 'oneaction';
      }
    }

  }

  $icon = $filedepot->getFileIcon($rec['fname']);
  $variables['extension_icon'] = "{$variables['layout_url']}/css/images/$icon";
  if ($variables['morerecords'] == 'loadfolder_msg') {
    $variables['more_records_message'] = theme('filedepot_filelisting_loadfolder', array(
      'cid' => $rec['cid'],
      'fid' => $variables['fid'],
      'foldernumber' => $variables['file_number'],
      'level' => $level,
    )
    );
  }
  elseif ($variables['morerecords'] != '') {
    $variables['more_records_message'] = theme('filedepot_filelisting_moredata', array(
      'cid' => $rec['cid'],
      'fid' => $variables['fid'],
      'foldernumber' => $variables['file_number'],
      'level' => $level,
    )
    );
  }
  else {
    $variables['more_records_message'] = '';
  }


}

function template_preprocess_filedepot_filelisting_moredata(&$variables) {
  $filedepot = filedepot_filedepot();
  $variables['message_padding'] = 100 + ($variables['level'] * $filedepot->paddingsize);
}

function template_preprocess_filedepot_filelisting_loadfolder(&$variables) {
  $filedepot = filedepot_filedepot();
  $variables['message_padding'] = 100 + ($variables['level'] * $filedepot->paddingsize);
}


function template_preprocess_filedepot_newfiledialog_folderoptions(&$variables) {
  $variables['folder_options'] = filedepot_recursiveAccessOptions(array('upload', 'upload_dir'), $variables['cid'], 0, 1, FALSE);
  if (empty($variables['folder_options'])) {
    // must have some option for the enclosing select


    $variables['folder_options'] = '<option value="0" disabled="disabled">' . t('Top Level Folder') . '</option>' . LB;
  }
}


function template_preprocess_filedepot_newfolderdialog(&$variables) {
  $variables['folder_options'] = filedepot_recursiveAccessOptions('admin', $variables['cid']);
  $variables['LANG_folder'] = t('Folder Name');
  $variables['LANG_description'] = t('Description');
  $variables['LANG_inherit'] = t('Inherit Parent Permissions');
  $variables['LANG_submit'] = t('Submit');
  $variables['LANG_cancel'] = t('Cancel');
}


function template_preprocess_filedepot_movefiles_form(&$variables) {
  $variables['movefolder_options'] = filedepot_recursiveAccessOptions('admin');
  $variables['LANG_newfolder'] = t('New Folder');
  $variables['LANG_submit'] = t('Submit');
  $variables['LANG_cancel'] = t('Cancel');
}

function template_preprocess_filedepot_moveincoming_form(&$variables) {
  $variables['movefolder_options'] = filedepot_recursiveAccessOptions('admin');
  $variables['LANG_newfolder'] = t('New Folder');
  $variables['LANG_submit'] = t('Submit');
  $variables['LANG_cancel'] = t('Cancel');
  $variables['token'] = drupal_get_token(FILEDEPOT_TOKEN_FOLDERMGMT);
}

function template_preprocess_filedepot_filedetail(&$variables) {
  $filedepot = filedepot_filedepot();
  $nexcloud = filedepot_nexcloud();
  $fid = $variables['fid'];
  $variables['site_url']              = base_path();
  $variables['ajax_server_url']       = url('filedepot_ajax');
  $variables['LANG_download'] = t('Download File');
  $variables['LANG_lastupated'] = t('Last Updated');
  $variables['show_statusmsg'] = 'none';
  $limit = FALSE;

  if ($variables['reportmode'] == 'approvals') {
    $sql = "SELECT file.cid,file.title,file.fname,file.date,file.version,file.size, ";
    $sql .= "file.description,file.submitter,file.status,file.version_note as notes,tags ";
    $sql .= "FROM {filedepot_filesubmissions} file ";
    $sql .= "WHERE file.id=:fid";
  }
  elseif ($variables['reportmode'] == 'incoming') {
    $sql = "SELECT 0,file.orig_filename as title,file.orig_filename as fname,file.timestamp,1,file.size, ";
    $sql .= "file.description,file.uid,9,file.version_note,'' ";
    $sql .= "FROM {filedepot_import_queue} file ";
    $sql .= "WHERE file.id=:fid";
  }
  else {
    $sql  = "SELECT file.cid, file.title, v.fname, file.date, file.version, file.size, ";
    $sql .= "file.description, file.submitter, file.status, v.notes, '' as tags ";
    $sql .= "FROM {filedepot_files} file ";
    $sql .= "LEFT JOIN {filedepot_fileversions} v ON v.fid=file.fid ";
    $sql .= "WHERE file.fid=:fid ORDER BY v.version DESC ";
    $limit = 1;
  }

  $filedetail = FALSE;
  if ($limit !== FALSE) {
    $query = db_query_range($sql, 0, 1, array(':fid' => $fid));
  }
  else {
    $query = db_query($sql, array(':fid' => $fid));
  }

  $A = $query->fetchAssoc();
  if ($A != NULL) {
    list($cid, $title, $fname, $date, $cur_version, $size, $description, $submitter, $status, $cur_notes, $tags) = array_values($A);
    $variables['cid'] = $cid;
    $variables['shortdate'] = strftime($filedepot->shortdate, $date);
    $variables['size'] = filedepot_formatFileSize($size);
    $icon = $filedepot->getFileIcon($fname);
    $variables['fileicon'] = "{$variables['layout_url']}/css/images/$icon";

    $author = db_query("SELECT name FROM {users} WHERE uid=:uid", array(':uid' => $submitter))->fetchField();
    $catname = db_query("SELECT name FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $cid))->fetchField();
    $nid = db_query("SELECT nid FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $cid))->fetchField();
    $variables['fname'] = filter_xss($fname);
    $variables['current_version'] = "(V{$cur_version})";
    $variables['filetitle'] = filter_xss($title);
    $variables['real_filename'] = filter_xss($fname);
    $variables['author']  = $author;
    $variables['description'] = nl2br(filter_xss($description));
    $variables['foldername'] = filter_xss($catname);
    $variables['current_ver_note'] = nl2br(filter_xss($cur_notes));
    $variables['tags'] = $nexcloud->get_itemtags($fid);
    $variables['disable_download'] = '';
    if ($status == FILEDEPOT_UNAPPROVED_STATUS) {
      $variables['show_statusmsg'] = '';
      $variables['status_image'] = '<img src="' . $variables['layout_url'] . '/css/images/padlock.gif">';
      $variables['statusmessage'] = '* ' .  t('File Submission to Approve');
    }
    elseif ($status == FILEDEPOT_INCOMING_STATUS) {
      $variables['show_statusmsg'] = '';
      $variables['status_image']   = '&nbsp;';
      $variables['statusmessage'] = '* ' .  t('Incoming File - needs to be moved or deleted');
      $variables['disable_download'] = 'onClick="return false;"';
    }
    elseif ($status == FILEDEPOT_LOCKED_STATUS) {
      $variables['show_statusmsg'] = '';
      $stat_userid = db_query("SELECT status_changedby_uid FROM {filedepot_files} WHERE fid=:fid", array(':fid' => $fid))->fetchField();
      $stat_user = db_query("SELECT name FROM {users} WHERE uid=:uid", array(':uid' => $stat_userid))->fetchField();
      $variables['status_image'] = '<img src="' . $variables['layout_url'] . '/css/images/padlock.gif">';
      $variables['statusmessage'] = '* ' . t('Locked by %name', array('%name' => $stat_user));
      $variables['LANG_DOWNLOAD_MESSAGE'] = t('File Locked by: %name', array('%name' => $stat_user));
      $variables['disable_download'] = 'onClick="return FALSE;"';
    }
    else {
      $variables['status_image']   = '&nbsp;';
      $variables['statusmessage']  = '&nbsp;';
    }

    if (function_exists('spaces_get_space')) {
      $space = spaces_get_space();
      if ($space && $space->type === 'og') {
        $urlprefix = '';
        switch (variable_get('purl_method_spaces_og', 'path')) {
          case 'path':
            $urlprefix = "{$space->group->purl}";
            break;
          case 'pair':
            $urlprefix = "{$key}/{$space->id}";
            break;
        }
      }
    }

    $clean_urls_on = variable_get('clean_url', 0);
    if ($clean_urls_on == 1) {
      $url_separator = "/";
    }
    else {
      $url_separator = "?q=";
    }

    if (isset($urlprefix) AND !empty($urlprefix)) {
      $variables['download_url'] = base_path() . "index.php{$url_separator}{$urlprefix}/filedepot&cid=$cid&fid=$fid";
    }
    else {
      $variables['download_url'] = base_path() . "index.php{$url_separator}filedepot&cid=$cid&fid=$fid";
    }

    // Retrieve file versions


    $sql = "SELECT fid,fname,version,notes,size,date,uid FROM {filedepot_fileversions} "
    . "WHERE fid=:fid AND version < :version ORDER by version DESC";
    $query = db_query($sql, array(':fid' => $fid, ':version' => $cur_version));
    $version_records = '';
    if ($query) {
      while ($rec = $query->fetchAssoc()) {
        $rec['nid'] = $nid;
        $version_records .= theme('filedepot_fileversion', array('versionRec' => $rec));
      }
    }
    $variables['version_records'] = $version_records;
  }

}


function template_preprocess_filedepot_fileversion(&$variables) {
  global $user;

  $filedepot = filedepot_filedepot();
  $variables['site_url']              = base_path();
  list($fid, $fname, $file_version, $ver_note, $ver_size, $ver_date, $submitter, $nid) = array_values($variables['versionRec']);
  $ver_shortdate = strftime($filedepot->shortdate, $ver_date);
  $ver_author = db_query("SELECT name from {users} WHERE uid=:uid", array(':uid' => $submitter))->fetchField();
  $cid = db_query("SELECT cid from {filedepot_files} WHERE fid=:fid", array(':fid' => $fid))->fetchField();
  $icon = $filedepot->getFileIcon($fname);
  $variables['fileicon'] = "{$variables['layout_url']}/css/images/$icon";
  $variables['fid'] = $fid;
  $variables['nid'] = $nid;
  $variables['vname'] = filter_xss($fname);
  $variables['ver_shortdate'] = $ver_shortdate;
  $variables['ver_author'] = $ver_author;
  $variables['ver_size'] = filedepot_formatFileSize($ver_size);
  $variables['ver_fileicon'] = $icon;
  $variables['file_versionnum'] = '(V' . $file_version . ')';
  $variables['file_version'] = $file_version;
  $variables['edit_version_note'] = filter_xss($ver_note);
  $variables['version_note'] = nl2br(filter_xss($ver_note));
  $variables['show_edit_version'] = 'none';
  $variables['show_delete_version'] = 'none';
  if ($user->uid == $submitter OR $filedepot->checkPermission($cid, 'admin')) {
    $variables['show_edit_version'] = '';
    $variables['show_delete_version'] = '';
  }
  $variables['cssid'] = ($variables['zebra'] == 'odd') ? 1 : 2;

}


function template_preprocess_filedepot_folderperms(&$variables) {
  $filedepot = filedepot_filedepot();
  $variables['catid'] = $variables['cid'];
  $variables['token'] = $variables['token'];
  $variables['user_options'] = filedepot_getUserOptions();
  $variables['role_options'] = filedepot_getRoleOptions();
  $variables['LANG_viewcategory'] = t('View Folder');
  $variables['LANG_uploadapproval'] = t('Upload with Approval');
  $variables['LANG_uploadadmin'] = t('Upload Admin');
  $variables['LANG_uploaddirect'] = t('Upload Direct');
  $variables['LANG_categoryadmin'] = t('Folder Admin');
  $variables['LANG_uploadversions'] = t('Upload New Versions');
  $variables['LANG_user'] = t('User');
  $variables['LANG_admin'] = t('Admin');
  $variables['LANG_action'] = t('Action');
  $variables['LANG_view'] = t('View');
  $variables['LANG_uploadadmin'] = t('Upload Admin');
  $variables['LANG_uploadversions'] = t('Upload Versions');
  $variables['LANG_directupload'] = t('Direct Upload');
  $variables['LANG_uploadwithapproval'] = t('Upload with Approval');

  $sql = "SELECT accid,permid,view,upload,upload_direct,upload_ver,approval,admin ";
  $sql .= "FROM {filedepot_access} WHERE permtype = 'user' AND permid > 0 AND catid = :cid";
  $query = db_query($sql, array(':cid' => $variables['cid']));
  $i = 0;
  $user_perm_records = '';
  while ($permrec = $query->fetchAssoc()) {
    $i++;
    $user_perm_records .= theme('filedepot_folderperm_rec', array('permRec' => $permrec, 'mode' => 'user', 'token' => $variables['token']));
  }
  if ($i > 0) {
    $variables['user_perm_records'] = $user_perm_records;
  }
  else {
    $variables['user_perm_records'] = '<tr><td width="20%">&nbsp;</td><td colspan="8">&nbsp;</td></tr>';
  }
  $sql = "SELECT accid,permid,view,upload,upload_direct,upload_ver,approval,admin ";
  $sql .= "FROM {filedepot_access} WHERE permtype = 'role' AND permid > 0 AND catid = :cid";
  $query = db_query($sql, array(':cid' => $variables['cid']));
  $i = 0;
  $role_perm_records = '';
  while ($permrec = $query->fetchAssoc()) {
    $i++;
    $role_perm_records .= theme('filedepot_folderperm_rec', array('permRec' => $permrec, 'mode' => 'role', 'token' => $variables['token']));
  }
  if ($i > 0) {
    $variables['role_perm_records'] = $role_perm_records;
  }
  else {
    $variables['role_perm_records'] = '<tr><td width="20%">&nbsp;</td><td colspan="8">&nbsp;</td></tr>';
  }
}

function template_preprocess_filedepot_folderperms_ogenabled(&$variables) {
  $filedepot = filedepot_filedepot();
  $variables['catid'] = $variables['cid'];
  $variables['user_options'] = filedepot_getUserOptions();
  $variables['role_options'] = filedepot_getRoleOptions();
  $variables['group_options'] = filedepot_getGroupOptions();
  $variables['LANG_viewcategory'] = t('View Folder');
  $variables['LANG_uploadapproval'] = t('Upload with Approval');
  $variables['LANG_uploadadmin'] = t('Upload Admin');
  $variables['LANG_uploaddirect'] = t('Upload Direct');
  $variables['LANG_categoryadmin'] = t('Folder Admin');
  $variables['LANG_uploadversions'] = t('Upload New Versions');
  $variables['LANG_user'] = t('User');
  $variables['LANG_admin'] = t('Admin');
  $variables['LANG_action'] = t('Action');
  $variables['LANG_view'] = t('View');
  $variables['LANG_uploadadmin'] = t('Upload Admin');
  $variables['LANG_uploadversions'] = t('Upload Versions');
  $variables['LANG_directupload'] = t('Direct Upload');
  $variables['LANG_uploadwithapproval'] = t('Upload with Approval');

  $sql = "SELECT accid,permid,view,upload,upload_direct,upload_ver,approval,admin ";
  $sql .= "FROM {filedepot_access} WHERE permtype = 'user' AND permid > 0 AND catid = :cid";
  $query = db_query($sql, array(':cid' => $variables['cid']));
  $i = 0;
$user_perm_records = '';
  while ($permrec = $query->fetchAssoc()) {
    $i++;
    $user_perm_records .= theme('filedepot_folderperm_rec', array('permRec' => $permrec, 'mode' => 'user', 'token' => $variables['token']));
  }
  if ($i > 0) {
    $variables['user_perm_records'] = $user_perm_records;
  }
  else {
    $variables['user_perm_records'] = '<tr><td width="20%">&nbsp;</td><td colspan="8">&nbsp;</td></tr>';
  }

  $sql = "SELECT accid,permid,view,upload,upload_direct,upload_ver,approval,admin ";
  $sql .= "FROM {filedepot_access} WHERE permtype = 'group' AND permid > 0 AND catid = :cid";
  $query = db_query($sql, array(':cid' => $variables['cid']));
  $i = 0;
  $group_perm_records = '';
  while ($permrec = $query->fetchAssoc()) {
    $i++;
    $group_perm_records .= theme('filedepot_folderperm_rec', array('permRec' => $permrec, 'mode' => 'group', 'token' => $variables['token']));
  }
  if ($i > 0) {
    $variables['group_perm_records'] = $group_perm_records;
  }
  else {
    $variables['group_perm_records'] = '<tr><td width="20%">&nbsp;</td><td colspan="8">&nbsp;</td></tr>';
  }

  $sql = "SELECT accid,permid,view,upload,upload_direct,upload_ver,approval,admin ";
  $sql .= "FROM {filedepot_access} WHERE permtype = 'role' AND permid > 0 AND catid = :cid";
  $query = db_query($sql, array(':cid' => $variables['cid']));
  $i = 0;
  $role_perm_records = '';
  while ($permrec = $query->fetchAssoc()) {
    $i++;
    $role_perm_records .= theme('filedepot_folderperm_rec', array('permRec' => $permrec, 'mode' => 'role', 'token' => $variables['token']));
  }
  if ($i > 0) {
    $variables['role_perm_records'] = $role_perm_records;
  }
  else {
    $variables['role_perm_records'] = '<tr><td width="20%">&nbsp;</td><td colspan="8">&nbsp;</td></tr>';
  }
}


function template_preprocess_filedepot_folderperm_rec(&$variables) {
  list($accid, $permid, $acc_view, $acc_upload, $acc_uploaddirect, $acc_uploadver, $acc_approval, $acc_admin) = array_values($variables['permRec']);
  if ($variables['mode'] == 'user') {
    $variables['name'] = db_query("SELECT name FROM {users} WHERE uid=:uid", array(':uid' => $permid))->fetchField();
  }
  else if ($variables['mode'] == 'group') {
    $group = filedepot_og_get_group_entity($permid);
    $variables['name'] = $group->title;
  }
  else {
    $variables['name'] = db_query("SELECT name FROM {role} WHERE rid=:uid", array(':uid' => $permid))->fetchField();
  }
  $variables['accid'] = $accid;
  $variables['view_perm'] = ($acc_view) ? t('Yes') : t('No');
  $variables['upload_perm'] = ($acc_upload) ? t('Yes') : t('No');
  $variables['uploaddir_perm'] = ($acc_uploaddirect) ? t('Yes') : t('No');
  $variables['uploadver_perm'] = ($acc_uploadver) ? t('Yes') : t('No');
  $variables['approve_perm'] = ($acc_approval) ? t('Yes') : t('No');
  $variables['admin_perm'] = ($acc_admin) ? t('Yes') : t('No');
  $variables['LANG_delete'] = t('Delete');
}


function template_preprocess_filedepot_taglinkon(&$variables) {
  if (!empty($_POST['tags'])) {
    $variables['searchtag'] = strip_tags($_POST['tags']) . ',' . strip_tags($variables['searchtag']);
  }
}


function template_preprocess_filedepot_searchtag(&$variables) {
  $variables['LANG_removetag'] = t('Remove search tag');
}


function template_preprocess_filedepot_notifications_file(&$variables) {
  $filedepot = filedepot_filedepot();
  $rec = $variables['rec'];
  $variables['recid'] = $rec['id'];
  $variables['fid'] = $rec['fid'];
  $variables['date'] = strftime($filedepot->shortdate, $rec['date']);
  $variables['LANG_delete'] = t('Delete');

  $sql = "SELECT a.title,a.cid,b.name as folder FROM {filedepot_files} a ";
  $sql .= "LEFT JOIN {filedepot_categories} b ON b.cid = a.cid WHERE a.fid={$rec['fid']} ";
  list($filename, $cid, $folder) = array_values(db_query($sql)->fetchAssoc());
  $variables['folderid'] = $cid;
  $variables['filename'] = filter_xss($filename);
  $variables['foldername'] = filter_xss($folder);
}

function template_preprocess_filedepot_notifications_folder(&$variables) {
  $filedepot = filedepot_filedepot();
  $rec = $variables['rec'];
  $variables['recid'] = $rec['id'];
  $variables['date'] = strftime($filedepot->shortdate, $rec['date']);
  $variables['folderid'] = $rec['cid'];
  $variables['LANG_delete'] = t('Delete');
  if ($rec['cid_newfiles'] == 1) {
    $variables['chk_newfiles'] = 'CHECKED=checked';
  }
  else {
    $variables['chk_newfiles'] = '';
  }
  if ($rec['cid_changes'] == 1) {
    $variables['chk_filechanges'] = 'CHECKED=checked';
  }
  else {
    $variables['chk_filechanges'] = '';
  }
  $folder = db_query("SELECT name FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $rec['cid']))->fetchField();
  $variables['foldername'] = filter_xss($folder);
}

function template_preprocess_filedepot_notifications_history(&$variables) {
  global $base_url;

  $filedepot = filedepot_filedepot();
  $rec = $variables['rec'];
  $variables['notification_type'] = t('@type', array(
    '@type' => $filedepot->notificationTypes[$rec['notification_type']],
  )
  );
  $variables['file_title'] = filter_xss($rec['title']);
  $variables['submitter_uid'] = $rec['submitter_uid'];
  $variables['submitter_name'] = $rec['name'];
  $variables['file_name'] = filter_xss($rec['fname']);
  $foldername = db_query("SELECT name FROM {filedepot_categories} WHERE cid=:cid", array(':cid' => $rec['cid']))->fetchField();
  $variables['folder_name'] = filter_xss($foldername);
  $variables['fid'] = $rec['fid'];
  $variables['cid'] = $rec['cid'];
  $variables['notification_date'] = strftime('%b %d %y, %I:%M', $rec['datetime']);
  $variables['site_url'] = $base_url;
}

function template_preprocess_filedepot_notifications(&$variables) {
  global $user;

  $filedepot = filedepot_filedepot();

  $variables['LANG_files_menuitem'] = t('Files');
  $variables['LANG_folder_menuitem'] = t('Folders');
  $variables['LANG_history_menuitem'] = t('Notification&nbsp;History');
  $variables['LANG_settings_menuitem'] = t('Settings');
  $variables['LANG_filename'] = t('File Name');
  $variables['LANG_folder'] = t('Folder');
  $variables['LANG_dateadded'] = t('Date Added');
  $variables['LANG_action'] = t('Action');
  $variables['LANG_newfiles'] = t('New Files');
  $variables['LANG_changes'] = t('Changes');
  $variables['LANG_historymsg'] = t('Log of notification emails sent - max 100');
  $variables['LANG_clearhistory'] = t('Clear History');
  $variables['LANG_date'] = t('Date');
  $variables['LANG_type'] = t('Type');
  $variables['LANG_submitter'] = t('Submitter');
  $variables['LANG_file'] = t('File');
  $variables['LANG_savesettings'] = t('Save Settings');
  $variables['LANG_settingheading'] = t('Setup your personal notification defaults. Individual folder and file notifications can also be used to over-ride these defaults.');
  $variables['LANG_settingline1'] = t('If you want to be notified of all new new files being added for all folders you have access, then you only need to enable the setting here');
  $variables['LANG_settingline2'] = t('If you only want to be notified of new files being added to selected folders, then disable the setting here and enable the notification for those selected folders only');
  $variables['LANG_settingline3'] = t('Folder Notification options are set by first selecting that folder and once the folder listing is in the main right panel, click on the folder name in the main right panel above the file listing to view/update the notification options');
  $variables['LANG_settingline4'] = t('Broadcast Notifications can be sent out by folder administrators  even if you are not subscribed to updates unless you disable broadcasts here');
  $variables['LANG_personalsettings'] = t('Personal Notification Setting');
  $variables['LANG_default'] = t('Default');
  $variables['LANG_newfilesadded'] = t('New Files being added');

  $variables['LANG_filesupdated'] = t('Files updated');
  $variables['LANG_allowadminbroadcasts'] = t('Allow Admin Broadcasts');

  $variables['history_records'] = '';
  $variables['file_records'] = '';
  $variables['folder_records'] = '';

  $sql = "SELECT a.id,a.fid,a.cid,a.date,cid_newfiles,cid_changes FROM {filedepot_notifications} a ";
  $sql .= "WHERE uid={$user->uid} AND a.ignore_filechanges = 0 ORDER BY a.date DESC";
  $query = db_query($sql);
  while ($A = $query->fetchAssoc()) {
    if ($A['fid'] != 0) {
      $variables['file_records'] .= theme('filedepot_notifications_file', array('rec' => $A));
    }
    elseif ($A['cid'] > 0) {
      $variables['folder_records'] .= theme('filedepot_notifications_folder', array('rec' => $A));
    }
  }

  if (variable_get('filedepot_default_notify_newfile', 0) == 1) {
    $variables['chk_fileadded_on'] = 'CHECKED=checked';
  }
  else {
    $variables['chk_fileadded_off'] = 'CHECKED=checked';
  }
  if (variable_get('filedepot_default_notify_filechange', 0) == 1) {
    $variables['chk_filechanged_on'] = 'CHECKED=checked';
  }
  else {
    $variables['chk_filechanged_off'] = 'CHECKED=checked';
  }
  if (variable_get('filedepot_default_allow_broadcasts', 0) == 1) {
    $variables['chk_broadcasts_on'] = 'CHECKED=checked';
  }
  else {
    $variables['chk_broadcasts_off'] = 'CHECKED=checked';
  }

  $qsettings = db_query("SELECT * FROM {filedepot_usersettings} WHERE uid=:uid", array(':uid' => $user->uid));
  if ($qsettings) {
    $A = $qsettings->fetchAssoc();
    if ($A['notify_newfile'] == 1) {
      $variables['chk_fileadded_off'] = '';
      $variables['chk_fileadded_on'] = 'CHECKED=checked';
    }
    else {
      $variables['chk_fileadded_on'] = '';
      $variables['chk_fileadded_off'] = 'CHECKED=checked';
    }
    if ($A['notify_changedfile'] == 1) {
      $variables['chk_filechanged_off'] = '';
      $variables['chk_filechanged_on'] = 'CHECKED=checked';
    }
    else {
      $variables['chk_filechanged_on'] = '';
      $variables['chk_filechanged_off'] = 'CHECKED=checked';
    }
    if ($A['allow_broadcasts'] == 1) {
      $variables['chk_broadcasts_off'] = '';
      $variables['chk_broadcasts_on'] = 'CHECKED=checked';
    }
    else {
      $variables['chk_broadcasts_on'] = '';
      $variables['chk_broadcasts_off'] = 'CHECKED=checked';
    }
  }
  // Generate the user notification history - last 100 records


  $sql = "SELECT a.submitter_uid,a.notification_type,a.fid,b.fname,b.title,a.cid,c.name,a.datetime,d.name "
  . "FROM {filedepot_notificationlog} a "
  . "LEFT JOIN {filedepot_files} b ON b.fid=a.fid "
  . "LEFT JOIN {filedepot_categories} c ON c.cid=a.cid "
  . "LEFT JOIN {users} d ON d.uid=a.submitter_uid "
  . "WHERE a.target_uid={$user->uid} "
  . "ORDER BY a.datetime DESC ";
  $query = db_query_range($sql, 0, 100);
  $cssid = 1;
  while ($A = $query->fetchAssoc()) {
    $variables['history_records'] .= theme('filedepot_notifications_history', array('rec' => $A));
  }
}

