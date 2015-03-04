<?php

/**
 * @file
 * ajaxserver.php
 * Implementation of filedepot_ajax() - main ajax handler for the module
 */


ob_start();

// Definitions of allowed token values
define('FILEDEPOT_TOKEN_FOLDERPERMS', 'filedepot_token_folderperms');
define('FILEDEPOT_TOKEN_FOLDERMGMT', 'filedepot_token_foldermgmt');
define('FILEDEPOT_TOKEN_FILEDETAILS', 'filedepot_token_filedetails');
define('FILEDEPOT_TOKEN_LISTING', 'filedepot_token_listing');

function filedepot_dispatcher($action) {
  global $user;

  $filedepot = filedepot_filedepot();
  $nexcloud = filedepot_nexcloud();
  module_load_include('php', 'filedepot', 'lib-theme');
  module_load_include('php', 'filedepot', 'lib-ajaxserver');
  module_load_include('php', 'filedepot', 'lib-common');

  if (function_exists('timer_start')) {
    timer_start('filedepot_timer');
  }
  firelogmsg("AJAX Server code executing - action: $action");

  switch ($action) {
    case 'archive':
      if ((isset($_GET['checked_files'])) && (isset($_GET['checked_folders']))) {
         module_load_include('php', 'filedepot', 'filedepot_archiver.class');
        $checked_files = json_decode($_GET['checked_files'], TRUE);
        $checked_folders = json_decode($_GET['checked_folders'], TRUE);
        //print_r($checked_files);
        //die(1);
        $fa = new filedepot_archiver();
        $fa->createAndCleanArchiveDirectory();
        $fa->addCheckedObjectArrays($checked_files, $checked_folders);
        $fa->createArchive();
        $fa->close();
        $fa->download();
        return;
      }
      else {
        echo "Invalid Parameters";
        return;
      }
      break;
    case 'getfilelisting':
      $cid = intval($_POST['cid']);
      if ($cid > 0) {
        if (db_query("SELECT count(*) FROM {filedepot_categories} WHERE cid=:cid", array(
          ':cid' => $cid,
        ))->fetchField() == 1) {
          $filedepot->ajaxBackgroundMode = TRUE;
        }
      }
      $reportmode = check_plain($_POST['reportmode']);
      $filedepot->activeview = $reportmode;
      $filedepot->cid = $cid;
      ctools_include('object-cache');
      $cache = ctools_object_cache_set('filedepot', 'folder', $cid);
      $data = filedepotAjaxServer_getfilelisting();
      break;

    case 'getfolderlisting':
      $filedepot->ajaxBackgroundMode = TRUE;
      $cid = intval($_POST['cid']);
      $reportmode = check_plain($_POST['reportmode']);
      if ($cid > 0) {
        ctools_include('object-cache');
        $cache = ctools_object_cache_set('filedepot', 'folder', $cid);
        $filedepot->cid = $cid;
        $filedepot->activeview = $reportmode;
        $data = filedepotAjaxServer_getfilelisting();
        firelogmsg("Completed generating FileListing");
      }
      else {
        $data = array('retcode' => 500);
      }
      break;

    case 'getleftnavigation':
      $data = filedepotAjaxServer_generateLeftSideNavigation();
      break;

    case 'getmorefiledata':
      /** Need to use XML instead of JSON format for return data.
       * It's taking up to 1500ms to interpret (eval) the JSON data into an object in the client code
       * Parsing the XML is about 10ms
       */

      $cid = intval($_POST['cid']);
      $level = intval($_POST['level']);
      $foldernumber = check_plain($_POST['foldernumber']);
      $filedepot->activeview = 'getmoredata';
      $filedepot->cid = $cid;
      $filedepot->lastRenderedFolder = $cid;
      $retval = '<result>';
      $retval .= '<retcode>200</retcode>';
      $retval .= '<displayhtml>' . htmlspecialchars(nexdocsrv_generateFileListing($cid, $level, $foldernumber), ENT_QUOTES, 'utf-8') . '</displayhtml>';
      $retval .= '</result>';
      firelogmsg("Completed generating AJAX return data - cid: {$cid}");
      break;

    case 'getmorefolderdata':
      /* Need to use XML instead of JSON format for return data.
       It's taking up to 1500ms to interpret (eval) the JSON data into an object in the client code
       Parsing the XML is about 10ms
       */

      $cid = intval($_POST['cid']);
      $level = intval($_POST['level']);
      // Need to remove the last part of the passed in foldernumber as it's the incremental file number
      // Which we recalculate in template_preprocess_filelisting()
      $x = explode('.', check_plain($_POST['foldernumber']));
      $x2 = array_pop($x);
      $foldernumber = implode('.', $x);
      $filedepot->activeview = 'getmorefolderdata';
      $filedepot->cid = $cid;
      $filedepot->lastRenderedFolder = $cid;
      $retval = '<result>';
      $retval .= '<retcode>200</retcode>';
      $retval .= '<displayhtml>' . htmlspecialchars(nexdocsrv_generateFileListing($cid, $level, $foldernumber), ENT_QUOTES, 'utf-8') . '</displayhtml>';
      $retval .= '</result>';
      firelogmsg("Completed generating AJAX return data - cid: {$cid}");
      break;

    case 'rendernewfilefolderoptions':
      $cid = intval($_POST['cid']);
      $data['displayhtml'] = theme('filedepot_newfiledialog_folderoptions', array('cid' => $cid));
      break;

    case 'rendernewfolderform':
      $cid = intval($_POST['cid']);
      $data['displayhtml'] = theme('filedepot_newfolderdialog', array('cid' => $cid));
      break;

    case 'createfolder':
      $node = (object) array(
        'uid' => $user->uid,
        'name' => $user->name,
        'type' => 'filedepot_folder',
        'title' => $_POST['catname'],
        'parentfolder' => intval($_POST['catparent']),
        'folderdesc' => $_POST['catdesc'],
        'inherit' => intval($_POST['catinherit']),
      );

      if ($node->parentfolder == 0 AND !user_access('administer filedepot')) {
        $data['errmsg'] = t('Error creating Folder - invalid parent folder');
        $data['retcode'] = 500;
      }
      else {
        node_save($node);
        if ($node->nid) {
          $data['displaycid'] = $filedepot->cid;
          $data['retcode'] = 200;
        }
        else {
          $data['errmsg'] = t('Error creating Folder');
          $data['retcode'] = 500;
        }
      }
      break;

    case 'deletefolder':
      $data = array();
      $cid = intval($_POST['cid']);
      $token = isset($_POST['token']) ? $_POST['token'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FOLDERMGMT))) {
        $data['retcode'] = 403; // Forbidden
      }
      else {
        $query = db_query("SELECT cid,pid,nid FROM {filedepot_categories} WHERE cid=:cid",
        array(':cid' => $cid));
        $A = $query->fetchAssoc();
        if ($cid > 0 AND $A['cid'] = $cid) {
          if ($filedepot->checkPermission($cid, 'admin')) {
            node_delete($A['nid']);
            $filedepot->cid = $A['pid'];
            // Set the new active directory to the parent folder
            $data['retcode'] = 200;
            $data['activefolder'] = theme('filedepot_activefolder');
            $data['displayhtml'] = filedepot_displayFolderListing($filedepot->cid);
            $data = filedepotAjaxServer_generateLeftSideNavigation($data);
          }
          else {
            $data['retcode'] = 403; // Forbidden
          }
        }
        else {
          $data['retcode'] = 404; // Not Found
        }
      }
      break;

    case 'updatefolder':
      $token = isset($_POST['token']) ? $_POST['token'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FOLDERMGMT))) {
        $data['retcode'] = 403; // Forbidden
      }
      else {
        $data = filedepotAjaxServer_updateFolder();
      }
      break;

    case 'setfolderorder':
      $cid = intval($_POST['cid']);
      $filedepot->cid = intval($_POST['listingcid']);
      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
        $data['retcode'] = 403; // Forbidden
      }
      else {
        if ($filedepot->checkPermission($cid, 'admin')) {
          // Check and see if any subfolders don't yet have a order value - if so correct
          $maxorder = 0;
          $pid = db_query("SELECT pid FROM {filedepot_categories} WHERE cid=:cid",
          array(':cid' => $cid))->fetchField();
          $maxquery = db_query_range("SELECT folderorder FROM {filedepot_categories} WHERE pid=:pid ORDER BY folderorder ASC", 0, 1,
          array(':pid' => $pid))->fetchField();
          $next_folderorder = $maxorder + 10;
          $query = db_query("SELECT cid FROM {filedepot_categories} WHERE pid=:pid AND folderorder = 0",
          array(':pid' => $pid));
          while ($B = $query->fetchAssoc()) {
            db_query("UPDATE {filedepot_categories} SET folderorder=:folderorder WHERE cid=:cid",
            array(
              ':folderorder' => $next_folderorder,
              ':cid' => $B['cid'],
            ));
            $next_folderorder += 10;
          }
          $itemquery = db_query("SELECT * FROM {filedepot_categories} WHERE cid=:cid", array(
            ':cid' => $cid,
          ));
          $retval = 0;
          while ($A = $itemquery->fetchAssoc()) {
            if ($_POST['direction'] == 'down') {
              $sql  = "SELECT folderorder FROM {filedepot_categories} WHERE pid=:pid ";
              $sql .= "AND folderorder > :folderorder ORDER BY folderorder ASC ";

              $nextorder = db_query_range($sql, 0, 1, array(':pid' => $A['pid'], ':folderorder' => $A['folderorder']))->fetchField();
              if ($nextorder > $A['folderorder']) {
                $folderorder = $nextorder + 5;
              }
              else {
                $folderorder = $A['folderorder'];
              }
              db_query("UPDATE {filedepot_categories} SET folderorder=:folderorder WHERE cid=:cid", array(
                ':folderorder' => $folderorder,
                ':cid' => $cid,
              ));
            }
            elseif ($_POST['direction'] == 'up') {
              $sql  = "SELECT folderorder FROM {filedepot_categories} WHERE pid=:pid ";
              $sql .= "AND folderorder < :folderorder ORDER BY folderorder DESC ";
              $nextorder = db_query_range($sql, 0, 1, array(
                ':pid' => $A['pid'],
                ':folderorder' => $A['folderorder'],
              ))->fetchField();
              $folderorder = $nextorder - 5;
              if ($folderorder <= 0) {
                $folderorder = 0;
              }
              db_query("UPDATE {filedepot_categories} SET folderorder=:folderorder WHERE cid=:cid", array(
                ':folderorder' => $folderorder,
                ':cid' => $cid,
              ));
            }
          }

          /* Re-order any folders that may have just been moved */
          $query = db_query("SELECT cid,folderorder from {filedepot_categories} WHERE pid=:pid ORDER BY folderorder",
          array(':pid' => $pid));
          $folderorder = 10;
          $stepnumber = 10;
          while ($A = $query->fetchAssoc()) {
            if ($folderorder != $A['folderOrder']) {
              db_query("UPDATE {filedepot_categories} SET folderorder=:folderorder WHERE cid=:cid", array(
                ':folderorder' => $folderorder,
                ':cid' => $A['cid'],
              ));
            }
            $folderorder += $stepnumber;
          }
          $data['retcode'] = 200;
          $data['displayhtml'] = filedepot_displayFolderListing($filedepot->cid);
        }
        else {
          $data['retcode'] = 400;
        }
      }
      break;

    case 'updatefoldersettings':
      $cid = intval($_POST['cid']);
      $notifyadd = intval($_POST['fileadded_notify']);
      $notifychange = intval($_POST['filechanged_notify']);
      if ($user->uid > 0 AND $cid >= 1) {
        // Update the personal folder notifications for user
        if (db_query("SELECT count(*) FROM {filedepot_notifications} WHERE cid=:cid AND uid=:uid", array(
          ':cid' => $cid,
          ':uid' => $user->uid,
        ))->fetchField() == 0) {
          $sql  = "INSERT INTO {filedepot_notifications} (cid,cid_newfiles,cid_changes,uid,date) ";
          $sql .= "VALUES (:cid,:notifyadd,:notifychange,:uid,:time)";
          db_query($sql, array(
            ':cid' => $cid,
            ':notifyadd' => $notifyadd,
            ':notifychange' => $notifychange,
            ':uid' => $user->uid,
            ':time' => time(),
          ));
        }
        else {
          $sql  = "UPDATE {filedepot_notifications} set cid_newfiles=:notifyadd, ";
          $sql .= "cid_changes=:notifychange, date=:time ";
          $sql .= "WHERE uid=:uid and cid=:cid";
          db_query($sql, array(
            ':notifyadd' => $notifyadd,
            ':notifychange' => $notifychange,
            ':time' => time(),
            ':uid' => $user->uid,
            ':cid' => $cid,
          ));
        }
        $data['retcode'] = 200;
        $data['displayhtml'] = filedepot_displayFolderListing($filedepot->cid);
      }
      else {
        $data['retcode'] = 500;
      }
      break;

    case 'loadfiledetails':
      $data = filedepotAjaxServer_loadFileDetails();
      break;

    case 'refreshfiledetails':
      $reportmode = check_plain($_POST['reportmode']);
      $fid = intval($_POST['id']);
      $cid = db_query("SELECT cid FROM {filedepot_files} WHERE fid=:fid", array(
        ':fid' => $fid,
      ))->fetchField();
      if ($filedepot->checkPermission($cid, 'view')) {
        $data['retcode'] = 200;
        $data['fid'] = $fid;
        $data['displayhtml'] = theme('filedepot_filedetail', array('fid' => $fid, 'reportmode' => $reportmode));
      }
      else {
        $data['retcode'] = 400;
        $data['error'] = t('Invalid access');
      }
      break;

    case 'updatenote':
      $fid = intval($_POST['fid']);
      $version = intval($_POST['version']);
      $note = check_plain($_POST['note']);
      $reportmode = check_plain($_POST['reportmode']);
      $token = isset($_POST['ftoken']) ? $_POST['ftoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FILEDETAILS))) {
        $data['retcode'] = 403; // forbidden
      }
      elseif ($fid > 0) {
        db_query("UPDATE {filedepot_fileversions} SET notes=:notes WHERE fid=:fid and version=:version",
        array(
          ':notes' => $note,
          ':fid' => $fid,
          ':version' => $version,
        ));
        $data['retcode'] = 200;
        $data['fid'] = $fid;
        $data['displayhtml'] = theme('filedepot_filedetail', array('fid' => $fid, 'reportmode' => $reportmode));
      }
      else {
        $data['retcode'] = 400;
      }
      break;

    case 'getfolderperms':
      $cid = intval($_POST['cid']);
      if ($cid > 0) {
        if ($filedepot->ogenabled) {
          $data['html'] = theme('filedepot_folderperms_ogenabled', array('cid' => $cid, 'token' => drupal_get_token(FILEDEPOT_TOKEN_FOLDERPERMS)));
        }
        else {
          $data['html'] = theme('filedepot_folderperms', array('cid' => $cid, 'token' => drupal_get_token(FILEDEPOT_TOKEN_FOLDERPERMS)));
        }
        $data['retcode'] = 200;
      }
      else {
        $data['retcode'] = 404;
      }
      break;

    case 'delfolderperms':
      $id = intval($_POST['id']);
      $token = isset($_POST['token']) ? $_POST['token'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FOLDERPERMS))) {
        $data['retcode'] = 403; // forbidden
      }
      elseif ($id > 0) {
        $query = db_query("SELECT catid, permtype, permid FROM  {filedepot_access} WHERE accid=:accid", array(
          ':accid' => $id,
        ));
        $A = $query->fetchAssoc();
        if ($filedepot->checkPermission($A['catid'], 'admin')) {
          db_delete('filedepot_access')
            ->condition('accid', $id)
            ->execute();
          db_update('filedepot_usersettings')
            ->fields(array('allowable_view_folders' => ''))
            ->execute();

          // For this folder - I need to update the access metrics now that a permission has been removed
          $nexcloud->update_accessmetrics($A['catid']);
          if ($filedepot->ogenabled) {
            $data['html'] = theme('filedepot_folderperms_ogenabled', array('cid' => $A['catid'], 'token' => drupal_get_token(FILEDEPOT_TOKEN_FOLDERPERMS)));
          }
          else {
            $data['html'] = theme('filedepot_folderperms', array('cid' => $A['catid'], 'token' => drupal_get_token(FILEDEPOT_TOKEN_FOLDERPERMS)));
          }
          $data['retcode'] = 200;
        }
        else {
          $data['retcode'] = 403; // Forbidden
        }
      }
      else {
        $data['retcode'] = 404; // Not Found
      }
      break;

    case 'addfolderperm':
      $cid = intval($_POST['catid']);
      $token = isset($_POST['token']) ? $_POST['token'] : NULL;

      if (!isset($_POST['cb_access'])) {
        $data['retcode'] = 204; // No permission options selected - return 'No content' statuscode
      }
      elseif (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FOLDERPERMS))) {
        $data['retcode'] = 403; // forbidden
      }
      elseif ($filedepot->updatePerms(
        $cid,                          // Category ID
        $_POST['cb_access'],           // Array of permissions checked by user
        $_POST['selusers'],            // Array of site members
        $_POST['selgroups'],           // Array of group members
        $_POST['selroles'])            // Array of roles
        ) {
        if (is_array($_POST['selroles']) AND count($_POST['selroles']) > 0) {
          foreach ($_POST['selroles'] as $roleid) {
            $roleid = intval($roleid);
            if ($roleid > 0) {
              $nexcloud->update_accessmetrics($cid);
            }
          }
        }
        if ($filedepot->ogenabled) {
          if (is_array($_POST['selgroups']) AND count($_POST['selgroups']) > 0) {
            foreach ($_POST['selgroups'] as $groupid) {
              $groupid = intval($groupid);
              if ($groupid > 0) {
                $nexcloud->update_accessmetrics($cid);
              }
            }
          }
          $data['html'] = theme('filedepot_folderperms_ogenabled', array('cid' => $cid, 'token' => drupal_get_token(FILEDEPOT_TOKEN_FOLDERPERMS)));
        }
        else {
          $data['html'] = theme('filedepot_folderperms', array('cid' => $cid, 'token' => drupal_get_token(FILEDEPOT_TOKEN_FOLDERPERMS)));
        }
        $data['retcode'] = 200;
      }
      else {
        $data['retcode'] = 403; // Forbidden
      }
      break;

    case 'updatefile':
      $fid = intval($_POST['id']);
      $folder_id = intval($_POST['folder']);
      $version = intval($_POST['version']);
      $filetitle  = $_POST['filetitle'];
      $description  = $_POST['description'];
      $vernote  = $_POST['version_note'];
      $approved  = check_plain($_POST['approved']);
      $tags  = $_POST['tags'];
      $data = array();
      $data['tagerror'] = '';
      $data['errmsg'] = '';
      $token = isset($_POST['ftoken']) ? $_POST['ftoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FILEDETAILS))) {
        $data['retcode'] = 403; // forbidden
        $data['errmsg'] = t('Invalid request');
      }
      elseif ($_POST['cid'] == 'incoming' AND $fid > 0) {
        $filemoved = FALSE;
        $sql = "UPDATE {filedepot_import_queue} SET orig_filename=:filename, description=:description,";
        $sql .= "version_note=:notes WHERE id=:fid";
        db_query($sql, array(
          ':filename' => $filetitle,
          ':description' => $description,
          ':notes' => $vernote,
          ':fid' => $fid,
        ));
        $data['retcode'] = 200;
        if ($folder_id > 0 AND $filedepot->moveIncomingFile($fid, $folder_id)) {
          $filemoved = TRUE;
          $filedepot->activeview = 'incoming';
          $data = filedepotAjaxServer_generateLeftSideNavigation($data);
          $data['displayhtml'] = filedepot_displayFolderListing();
        }
      }
      elseif ($fid > 0) {
        $filemoved = FALSE;
        if ($approved == 0) {
          $sql = "UPDATE {filedepot_filesubmissions} SET title=:title, description=:description,";
          $sql .= "version_note=:notes, cid=:cid, tags=:tags WHERE id=:fid;";
          db_query($sql, array(
            ':title' => $filetitle,
            ':description' => $description,
            ':notes' => $vernote,
            ':cid' => $folder_id,
            ':tags' => $tags,
            ':fid' => $fid,
          ));
          $data['cid'] = $folder_id;
          $data['tags'] = '';
        }
        else {
          $query = db_query("SELECT fname,cid,version,submitter FROM {filedepot_files} WHERE fid=:fid", array(
            ':fid' => $fid,
          ));
          list($fname, $cid, $current_version, $submitter) = array_values($query->fetchAssoc());

          // Allow updating the category, title, description and image for the current version and primary file record
          if ($version == $current_version) {
            db_query("UPDATE {filedepot_files} SET title=:title,description=:desc,date=:time WHERE fid=:fid", array(
              ':title' => $filetitle,
              ':desc' => $description,
              ':time' => time(),
              ':fid' => $fid,
            ));

            // Test if user has selected a different directory and if they have perms then move else return FALSE;
            if ($folder_id > 0) {
              $newcid = $folder_id;
              if ($cid != $newcid) {
                $filemoved = $filedepot->moveFile($fid, $newcid);
                if ($filemoved == FALSE) {
                  $data['errmsg'] = t('Error moving file');
                }
              }
              $data['cid'] = $newcid;
            }
            else {
              $data['cid'] = $cid;
            }
            unset($_POST['tags']); // Format tags will check this to format tags in case we are doing a search which we are not in this case.

            $data['tags'] = filedepot_formatfiletags($tags);
          }

          db_query("UPDATE {filedepot_fileversions} SET notes=:notes WHERE fid=:fid and version=:version", array(
            ':notes' => $vernote,
            ':fid' => $fid,
            ':version' => $version,
          ));

          // Update the file tags if role or group permission set -- we don't support tag access perms at the user level.
          if ($filedepot->checkPermission($folder_id, 'view', 0, FALSE)) {
            if ($filedepot->checkPermission($folder_id, 'admin', 0, FALSE) OR $user->uid == $submitter) {
              $admin = TRUE;
            }
            else {
              $admin = FALSE;
            }
            if (!$nexcloud->update_tags($fid, $tags, $admin)) {
              $data['tagerror'] = t('Tags not added - Group or Role assigned view perms required');
              $data['tags'] = '';
            }
          }
          else {
            $data['tagerror'] = t('Problem adding or updating tags');
            $data['tags'] = '';
          }
        }
        $data['retcode'] = 200;
        $data['tagcloud'] = theme('filedepot_tagcloud');
      }
      else {
        $data['retcode'] = 500;
        $data['errmsg'] = t('Invalid File');
      }
      $data['description'] = nl2br(filter_xss($description));
      $data['fid'] = $fid;
      $data['filename'] = filter_xss($filetitle);
      $data['filemoved'] = $filemoved;
      break;

    case 'deletefile':
      $fid = intval($_POST['fid']);
      $token = isset($_POST['ftoken']) ? $_POST['ftoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FILEDETAILS))) {
        $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0 AND $fid > 0) {
        $data = filedepotAjaxServer_deleteFile($fid);
      }
      else {
        $data['retcode'] = 500;
      }
      break;

    case 'deletecheckedfiles':
      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0) {
        $data = filedepotAjaxServer_deleteCheckedFiles();
      }
      else {
        $data['retcode'] = 500;
      }
      break;

    case 'deleteversion':
      $fid = intval($_POST['fid']);
      $version = intval($_POST['version']);
      $reportmode = check_plain($_POST['reportmode']);
      $token = isset($_POST['ftoken']) ? $_POST['ftoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FILEDETAILS))) {
        $data['retcode'] = 403; // forbidden
      }
      elseif ($fid > 0 AND $version > 0) {
        if ($filedepot->deleteVersion($fid, $version)) {
          $data['retcode'] = 200;
          $data['fid'] = $fid;
          $data['displayhtml'] = theme('filedepot_filedetail', array('fid' => $fid, 'reportmode' => $reportmode));
        }
        else {
          $data['retcode'] = 400;
        }
      }
      else {
        $data['retcode'] = 400;
      }
      break;

    case 'togglefavorite':
      $id = intval($_POST['id']);
      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
        $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0 AND $id >= 1) {
        if (db_query("SELECT count(fid) FROM {filedepot_favorites} WHERE uid=:uid AND fid=:fid", array(
          ':uid' => $user->uid,
          ':fid' => $id,
        ))->fetchField() > 0) {
          $data['favimgsrc'] = base_path() . drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('favorite-off');
          db_query("DELETE FROM {filedepot_favorites} WHERE uid=:uid AND fid=:fid", array(
            ':uid' => $user->uid,
            ':fid' => $id,
          ));
        }
        else {
          $data['favimgsrc'] = base_path() . drupal_get_path('module', 'filedepot') . '/css/images/' . $filedepot->getFileIcon('favorite-on');
          db_query("INSERT INTO {filedepot_favorites} (uid,fid) VALUES (:uid,:fid)", array(
            ':uid' => $user->uid,
            ':fid' => $id,
          ));
        }
        $data['retcode'] = 200;
      }
      else {
        $data['retcode'] = 400;
      }
      break;

    case 'markfavorite':

      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0 ) {
        $cid = intval($_POST['cid']);
        $reportmode = check_plain($_POST['reportmode']);
        $fileitems = check_plain($_POST['checkeditems']);
        $files = explode(',', $fileitems);
        $filedepot->cid = $cid;
        $filedepot->activeview = $reportmode;
        foreach ($files as $id) {
          if ($id > 0 AND db_query("SELECT COUNT(*) FROM {filedepot_favorites} WHERE uid=:uid AND fid=:fid", array(
            ':uid' => $user->uid,
            ':fid' => $id,
          ))->fetchField() == 0) {
            db_query("INSERT INTO {filedepot_favorites} (uid,fid) VALUES (:uid,:fid)", array(
              ':uid' => $user->uid,
              'fid' => $id,
            ));
          }
        }

        $data['retcode'] = 200;
        $data['displayhtml'] = filedepot_displayFolderListing($cid);
      }
      break;

    case 'clearfavorite':
      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0 ) {
        $cid = intval($_POST['cid']);
        $reportmode = check_plain($_POST['reportmode']);
        $fileitems = check_plain($_POST['checkeditems']);
        $files = explode(',', $fileitems);
        $filedepot->cid = $cid;
        $filedepot->activeview = $reportmode;
        foreach ($files as $id) {
          if ($id > 0 AND db_query("SELECT COUNT(*) FROM {filedepot_favorites} WHERE uid=:uid AND fid=:fid", array(
            ':uid' => $user->uid,
            ':fid' => $id,
          ))->fetchField() == 1) {
            db_query("DELETE FROM {filedepot_favorites} WHERE uid=:uid AND fid=:fid", array(
              ':uid' => $user->uid,
              ':fid' => $id,
            ));
          }
        }
        $data['retcode'] = 200;
        $data['displayhtml'] = filedepot_displayFolderListing($cid);
      }
      break;

    case 'togglelock':
      $fid = intval($_POST['fid']);
      $token = isset($_POST['ftoken']) ? $_POST['ftoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FILEDETAILS))) {
       $data['error'] = t('Error locking file');
      }
      else {
        $data['error'] = '';
        $data['fid'] = $fid;
        $query = db_query("SELECT status FROM {filedepot_files} WHERE fid=:fid", array(':fid' => $fid));
        if ($query) {
          list($status) = array_values($query->fetchAssoc());
          if ($status == 1) {
            db_query("UPDATE {filedepot_files} SET status='2', status_changedby_uid=:uid WHERE fid=:fid", array(
              ':uid' => $user->uid,
              ':fid' => $fid,
            ));
            $stat_user = db_query("SELECT name FROM {users} WHERE uid=:uid", array(
              ':uid' => $user->uid,
            ))->fetchField();
            $data['message'] = 'File Locked successfully';
            $data['locked_message'] = '* ' . t('Locked by %name', array('%name' => $stat_user));
            $data['locked'] = TRUE;
          }
          else {
            db_query("UPDATE {filedepot_files} SET status='1', status_changedby_uid=:uid WHERE fid=:fid", array(
              ':uid' => $user->uid,
              ':fid' => $fid,
            ));
            $data['message'] = 'File Un-Locked successfully';
            $data['locked'] = FALSE;
          }
        }
        else {
          $data['error'] = t('Error locking file');
        }
      }
      break;

    case 'movecheckedfiles':
      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0) {
        $data = filedepotAjaxServer_moveCheckedFiles();
      }
      else {
        $data['retcode'] = 500;
      }
      break;


    case 'rendermoveform':
      $data['displayhtml'] = theme('filedepot_movefiles_form');
      break;

    case 'rendermoveincoming':
      $data['displayhtml'] = theme('filedepot_moveincoming_form');
      break;

    case 'togglesubscribe':
      $fid = intval($_POST['fid']);
      $cid = intval($_POST['cid']);

      $token = isset($_POST['ftoken']) ? $_POST['ftoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FILEDETAILS))) {
       $data['error'] = t('Error subscribing');
      }
      else {
        global $base_url;
        $data['error'] = '';
        $data['fid'] = $fid;
        $ret = filedepotAjaxServer_updateFileSubscription($fid, 'toggle');
        // @TODO: Notifyicon does not appear to be implemented
        if ($ret['retcode'] === TRUE) {
          $data['retcode'] = 200;
          if ($ret['subscribed'] === TRUE) {
            $data['subscribed'] = TRUE;
            $data['message'] = 'You will be notified of any new versions of this file';
            $path = drupal_get_path('module', 'filedepot') . '/css/images/email-green.gif';
            $data['notifyicon'] = $base_url . '/' . $path;
            $data['notifymsg'] = 'Notification Enabled - Click to change';
          }
          elseif ($ret['subscribed'] === FALSE) {
            $data['subscribed'] = FALSE;
            $data['message'] = 'You will not be notified of any new versions of this file';
            $path = drupal_get_path('module', 'filedepot') . '/css/images/email-regular.gif';
            $data['notifyicon'] = $base_url . '/' . $path;
            $data['notifymsg'] = 'Notification Disabled - Click to change';
          }
        }
        else {
          $data['error'] = t('Error accessing file record');
          $data['retcode'] = 404;
        }
      }
      break;

    case 'updatenotificationsettings':
      if ($user->uid > 0) {
        if (db_query("SELECT count(uid) FROM {filedepot_usersettings} WHERE uid=:uid", array(':uid' => $user->uid))->fetchField() == 0) {
          db_query("INSERT INTO {filedepot_usersettings} (uid) VALUES ( :uid )", array(':uid' => $user->uid));
        }
        $sql = "UPDATE {filedepot_usersettings} SET notify_newfile=:newfile,notify_changedfile=:changefile,allow_broadcasts=:broadcast WHERE uid=:uid";
        db_query($sql, array(
          ':newfile' => $_POST['fileadded_notify'],
          ':changefile' => $_POST['fileupdated_notify'],
          ':broadcast' => $_POST['admin_broadcasts'],
          ':uid' => $user->uid,
        ));
        $data['retcode'] = 200;
        $data['displayhtml'] = theme('filedepot_notifications');
      }
      else {
        $data['retcode'] = 500;
      }
      break;

    case 'deletenotification':
      $id = intval($_POST['id']);
      if ($user->uid > 0 AND $id > 0) {
        db_query("DELETE FROM {filedepot_notifications} WHERE id=:id AND uid=:uid", array(
          ':id' => $id,
          ':uid' => $user->uid,
        ));
        $data['retcode'] = 200;
        $data['displayhtml'] = theme('filedepot_notifications');
      }
      else {
        $data['retcode'] = 500;
      }
      break;

    case 'clearnotificationlog':
      db_query("DELETE FROM {filedepot_notificationlog} WHERE target_uid=:uid", array(
        ':uid' => $user->uid,
      ));
      $data['retcode'] = 200;
      break;

    case 'multisubscribe':

      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0 ) {
        $reportmode = check_plain($_POST['reportmode']);
        $fileitems = check_plain($_POST['checkeditems']);
        $folderitems = check_plain($_POST['checkedfolders']);
        $filedepot->cid = intval($_POST['cid']);
        $filedepot->activeview = check_plain($_POST['reportmode']);

        if (!empty($fileitems)) {
          $files = explode(',', $fileitems);
          foreach ($files as $fid) {
            filedepotAjaxServer_updateFileSubscription($fid, 'add');
          }
        }

        if (!empty($folderitems)) {
          $folders = explode(',', $folderitems);
          foreach ($folders as $cid) {
            if (db_query("SELECT count(id) FROM {filedepot_notifications} WHERE cid=:cid AND uid=:uid", array(
              ':cid' => $cid,
              ':uid' => $user->uid,
            ))->fetchField() == 0) {
              $sql  = "INSERT INTO {filedepot_notifications} (cid,cid_newfiles,cid_changes,uid,date) ";
              $sql .= "VALUES (:cid,1,1,:uid,:time)";
              db_query($sql, array(
                ':cid' => $cid,
                ':uid' => $user->uid,
                ':time' => time(),
              ));
            }
          }
        }

        $data['retcode'] = 200;
        $data = filedepotAjaxServer_generateLeftSideNavigation($data);
        $data['displayhtml'] = filedepot_displayFolderListing($filedepot->cid);
      }
      else {
        $data['retcode'] = 500;
      }
      break;

    case 'autocompletetag':
      $matches = $nexcloud->get_matchingtags($_GET['query']);
      $retval = implode("\n", $matches);
      break;

    case 'refreshtagcloud':
      $data['retcode'] = 200;
      $data['tagcloud'] = theme('filedepot_tagcloud');
      break;

    case 'search':
      $query = $_POST['query'];
      if (!empty($query)) {
        $filedepot->activeview = 'search';
        $filedepot->cid = 0;
        $data['retcode'] = 200;
        $data['displayhtml'] = filedepot_displaySearchListing($query);
        $data['header'] = theme('filedepot_header');
        $data['activefolder'] = theme('filedepot_activefolder');
      }
      else {
        $data['retcode'] = 400;
      }
      break;

    case 'searchtags':
      if (isset($_POST['tags'])) {
        $tags = stripslashes($_POST['tags']);
      }
      else {
        $tags = '';
      }
      if (isset($_POST['removetag'])) {
        $removetag = stripslashes($_POST['removetag']);
      }
      else {
        $removetag = '';
      }
      $current_search_tags = '';
      $filedepot->activeview = 'searchtags';
      $filedepot->cid = 0;
      if (!empty($tags)) {
        if (!empty($removetag)) {
          $removetag = stripslashes($removetag);
          $atags = explode(',', $tags);
          $key = array_search($removetag, $atags);
          if ($key !== FALSE) {
            unset($atags[$key]);
          }
          $tags = implode(',', $atags);
          $_POST['tags'] = $tags;
        }
        else {
          $removetag = '';
        }
        if (!empty($tags)) {
          $data['searchtags'] = stripslashes($tags);
          $atags = explode(',', $tags);
          if (count($atags) >= 1) {
            foreach ($atags as $tag) {
              $tag = trim($tag); // added to handle extra space thats added when removing a tag - thats between 2 other tags

              if (!empty($tag)) {
                $current_search_tags .= theme('filedepot_searchtag', array('searchtag' => addslashes($tag), 'label' => check_plain($tag)));
              }
            }
          }
          $data['retcode'] = 200;
          $data['currentsearchtags'] = $current_search_tags;
          $data['displayhtml'] = filedepot_displayTagSearchListing($tags);
          $data['tagcloud'] = theme('filedepot_tagcloud');
          $data['header'] = theme('filedepot_header');
          $data['activefolder'] = theme('filedepot_activefolder');
        }
        else {
          unset($_POST['tags']);
          $filedepot->activeview = 'latestfiles';
          $data['retcode'] = 200;
          $data['currentsearchtags'] = '';
          $data['tagcloud'] = theme('filedepot_tagcloud');
          $data['displayhtml'] = filedepot_displayFolderListing($filedepot->cid);
          $data['header'] = theme('filedepot_header');
          $data['activefolder'] = theme('filedepot_activefolder');
        }
      }
      else {
        $data['tagcloud'] = theme('filedepot_tagcloud');
        $data['retcode'] = 203; // Partial Information

      }
      break;

    case 'approvefile':
      $id = intval($_POST['id']);
      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0 AND $filedepot->approveFileSubmission($id)) {
        $filedepot->cid = 0;
        $filedepot->activeview = 'approvals';
        $data = filedepotAjaxServer_getfilelisting();
        $data = filedepotAjaxServer_generateLeftSideNavigation($data);
        $data['retcode'] = 200;
      }
      else {
        $data['retcode'] = 400;
      }
      break;

    case 'approvesubmissions':
      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0 ) {
        $reportmode = check_plain($_POST['reportmode']);
        $fileitems = check_plain($_POST['checkeditems']);
        $files = explode(',', $fileitems);
        $approved_files = 0;
        $filedepot->activeview = 'approvals';
        foreach ($files as $id) {
          // Check if this is a valid submission record
          if ($id > 0 AND db_query("SELECT COUNT(*) FROM {filedepot_filesubmissions} WHERE id=:id", array(':id' => $id))->fetchField() == 1) {
            // Verify that user has Admin Access to approve this file
            $cid = db_query("SELECT cid FROM {filedepot_filesubmissions} WHERE id=:id", array(':id' => $id))->fetchField();
            if ($cid > 0 AND $filedepot->checkPermission($cid, array('admin', 'approval'), 0, FALSE)) {
              if ($filedepot->approveFileSubmission($id)) {
                $approved_files++;
              }
            }
          }
        }
        if ($approved_files > 0) {
          $data['retcode'] = 200;
          $data = filedepotAjaxServer_generateLeftSideNavigation($data);
          $data['displayhtml'] = filedepot_displayFolderListing();
        }
        else {
          $data['retcode'] = 400;
        }
      }
      break;

    case 'deletesubmissions':
      $token = isset($_POST['ltoken']) ? $_POST['ltoken'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_LISTING))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($user->uid > 0 ) {
        $reportmode = check_plain($_POST['reportmode']);
        $fileitems = check_plain($_POST['checkeditems']);
        $files = explode(',', $fileitems);
        $deleted_files = 0;
        $filedepot->activeview = 'approvals';
        foreach ($files as $id) {
          // Check if this is a valid submission record
          if ($id > 0 AND db_query("SELECT COUNT(*) FROM {filedepot_filesubmissions} WHERE id=:id", array(':id' => $id))->fetchField() == 1) {
            // Verify that user has Admin Access to approve this file
            $cid = db_query("SELECT cid FROM {filedepot_filesubmissions} WHERE id=:id", array(':id' => $id))->fetchField();
            if ($cid > 0 AND $filedepot->checkPermission($cid, array('admin', 'approval'), 0, FALSE)) {
              if ($filedepot->deleteSubmission($id)) {
                $deleted_files++;
              }
            }
          }
        }
        if ($deleted_files > 0) {
          $data['retcode'] = 200;
          $data = filedepotAjaxServer_generateLeftSideNavigation($data);
          $data['displayhtml'] = filedepot_displayFolderListing();
        }
        else {
          $data['retcode'] = 400;
        }
      }
      break;

    case 'deleteincomingfile':
      $id = intval($_POST['id']);
      $message = '';

      $token = isset($_POST['token']) ? $_POST['token'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FOLDERMGMT))) {
       $data['retcode'] = 403; // forbidden
      }
      else {
        $fid = db_query("SELECT drupal_fid FROM {filedepot_import_queue} WHERE id=:id", array(':id' => $id))->fetchField();
        if ($fid > 0) {
          $filepath = db_query("SELECT filepath FROM {files} WHERE fid=:fid", array(':fid' => $fid))->fetchField();
          if (!empty($filepath) AND file_exists($filepath)) {
            @unlink($filepath);
          }
          db_query("DELETE FROM {files} WHERE fid=:fid", array(':fid' => $fid));
          db_query("DELETE FROM {filedepot_import_queue} WHERE id=:id", array(':id' => $id));
          $data['retcode'] = 200;
          $filedepot->activeview = 'incoming';
          $data = filedepotAjaxServer_generateLeftSideNavigation($data);
          $data['displayhtml'] = filedepot_displayFolderListing();
        }
        else {
          $data['retcode'] = 500;
        }

        $retval = json_encode($data);
      }
      break;

    case 'moveincomingfile': //FILEDEPOT_TOKEN_FOLDERMGMT
      $newcid = intval($_POST['newcid']);
      $id = intval($_POST['id']);
      $filedepot->activeview = 'incoming';
      $data = array();
      $token = isset($_POST['token']) ? $_POST['token'] : NULL;

      if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FOLDERMGMT))) {
       $data['retcode'] = 403; // forbidden
      }
      elseif ($newcid > 0 AND $id > 0 AND $filedepot->moveIncomingFile($id, $newcid)) {
        // Send out email notifications of new file added to all users subscribed  -  Get fileid for the new file record
        $fid = db_query("SELECT fid FROM {filedepot_files} WHERE cid=:cid AND submitter=:uid ORDER BY fid DESC",
          array(
          ':cid' => $newcid,
          ':uid' => $user->uid,
        ), 0, 1)->fetchField();
        filedepot_sendNotification($fid, FILEDEPOT_NOTIFY_NEWFILE);
        $data['retcode'] = 200;
        $data = filedepotAjaxServer_generateLeftSideNavigation($data);
        $data['displayhtml'] = filedepot_displayFolderListing();
      }
      else {
        $data['retcode'] = 500;
      }
      break;

    case 'broadcastalert':
      $data = array();
      if (variable_get('filedepot_default_allow_broadcasts', 1) == 0) {
        $data['retcode'] = 204;
      }
      else {
        $fid = intval($_POST['fid']);

        $message = check_plain($_POST['message']);
        $token = isset($_POST['ftoken']) ? $_POST['ftoken'] : NULL;

        if (($token == NULL) || (!drupal_valid_token($token, FILEDEPOT_TOKEN_FILEDETAILS))) {
          $data['retcode'] = 403;
        }
        elseif (!empty($message) AND $fid > 0) {
          $data = filedepotAjaxServer_broadcastAlert($fid, $message);
        }
        else {
          $data['retcode'] = 500;
        }
      }
      break;

  }

  ob_clean();
  if ($action != 'autocompletetag') {
    if ($action != 'getmorefiledata' AND $action != 'getmorefolderdata') {
      $retval = json_encode($data);
    }
    header('Cache-Control: no-store, no-cache, must-revalidate');
    header('content-type: application/xml', TRUE);
    echo "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
  }
  echo $retval;

}
ob_end_flush();
