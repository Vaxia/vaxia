<?php

// $Id:

/**
 * @file
 * filedepot.views.inc
 * Contains all Views related API calls and default view creation information
 */


function filedepot_views_data() {

  $data = array();
  $data['filedepot_categories']['table']['group']  = t('Filedepot Folders');
  $data['filedepot_categories']['table']['base'] = array(
    'field' => 'cid',
    'title' => t('Filedepot Folders'),
    'help' => t("Filedepot Folder Structure."),
  );

  $data['filedepot_files']['table']['group']  = t('Filedepot Files');
  $data['filedepot_files']['table']['join'] = array(
    'filedepot_categories' => array(
      'type' => 'LEFT',
      'left_field' => 'cid',
      'field' => 'cid',
    ),
  );

   // *** Field definitions ***
  $data['filedepot_categories']['name'] = array(
    'title' => t('Folder Name'),
    'help' => t('Name of the Folder'),
    'field' => array(
      'click sortable' => TRUE,
    ),
  );
  
 

  // INT fields here:
  $data['filedepot_categories']['cid'] = array (
    'title' => t('Folder ID'),
    'help'  => t('Unique Folder Identifier.'),
    'field' => array (
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['filedepot_categories']['pid'] = array (
    'title' => t('Parent Folder'),
    'help'  => t('Folder Parent Identifier.'),
    'field' => array (
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['filedepot_categories']['nid'] = array (
    'title' => t('Node ID'),
    'help'  => t('Drupal Node Identifier.'),
    'field' => array (
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  $data['filedepot_files']['fid'] = array (
    'title' => t('File ID'),
    'help'  => t('Unique File Identifier.'),
    'field' => array (
      'handler' => 'views_handler_field_numeric',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    )
  );

  $data['filedepot_files']['fname'] = array(
    'title' => t('File Name'),
    'help' => t('Real Name of the File'),
    'field' => array(
      'click sortable' => TRUE,
    ),
  );

  $data['filedepot_files']['title'] = array(
    'title' => t('File Title'),
    'help' => t('Display Name of the File'),
    'field' => array(
      'click sortable' => TRUE,
    ),
  );
  
  $data['filedepot_files']['size'] = array(
    'title' => t('File Size'),
    'help' => t('Size of the File (Bytes)'),
    'field' => array(
      'click sortable' => TRUE,
    ),
  );

  return $data;

}


function filedepot_views_pre_execute(&$view) {
  if ($view->name == 'filedepot') {
    $extra_sql = '';
    
    $query = db_select('');
    $query = $view->build_info['query'];
    $where = array();
    
    // Check if user has entered the argument into the view directly -- ie in the view UI
    // If so, we need will use that folder id to remove it and add our own including sub folders.
    if ((count($view->args) > 0) && ($view->args[0] > 0)) {
      $folder_id = $view->args[0];
      /*
      //str_replace("WHERE filedepot_categories.cid = '%s'\n", '', $view->build_info['query']);
      $wherepos = strpos($view->build_info['query'],"WHERE filedepot_categories.cid = '");
      if ($wherepos > 0) {
        $wherepos_end = strpos($view->build_info['query'],"'\n",$wherepos);
        if ($wherepos_end > $wherepos) {
          $view->build_info['query'] = substr($view->build_info['query'],0,$wherepos) . substr($view->build_info['query'],$wherepos_end+3);
        }
      }*/
    }
    else {
      // Check if a folder id is being passed in to the view from a URL argument
      $folder_id = intval(arg(1));
    }

    $filedepot = filedepot_filedepot();

    // if we have a valid folder id determine all viewable subfolders and only generate view for these folders
    if ($folder_id > 0 AND $filedepot->checkPermission($folder_id, 'view')) {
      module_load_include('php','filedepot','lib-common');
      $array_folders = array();
      $array_folders[] = $folder_id;
      $filedepot->getRecursiveCatIDs($array_folders, $folder_id, 'view');
      if (!empty($array_folders) AND count($array_folders) > 0) {
        $folderlist = implode(',',$array_folders);
        $where[] = "filedepot_categories.cid in ($folderlist)";
      }
    }
    else {
      if (!empty($filedepot->allowableViewFoldersSql)) {        
        $where[] = "filedepot_categories.cid in ({$filedepot->allowableViewFoldersSql})";
        
      }
      else {  // User does not have any viewable folders
        $where[] = "1 = 2";
      }
    }
    
    if (count($where) > 0) {
      $where_str = implode(" AND ", $where);
      $view->build_info['query']->where($where_str);
    }

    
    $str = $view->build_info['query']->__toString();
    $s1 = 1;
  }
}

?>