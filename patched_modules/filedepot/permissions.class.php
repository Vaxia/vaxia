<?php

/**
 * @file
 * permissions.class.php
 * Permissions management class for the Filedepot module
 */
class filedepot_permission_manager
{
  
}

/**
 * Holds information about a CIDs permissions
 */
class filedepot_permission_object
{

  private $_Cid             = 0;
  private $_PermissionArray = Array(
    'view'          => FALSE,
    'upload'        => FALSE,
    'upload_direct' => FALSE,
    'upload_ver'    => FALSE,
    'approval'      => FALSE,
    'admin'         => FALSE
  );

  const VIEW          = "view";
  const UPLOAD        = "upload";
  const UPLOAD_DIRECT = "upload_direct";
  const UPLOAD_VER    = "upload_ver";
  const APPROVAL      = "approval";
  const ADMIN         = "admin";

  public function __construct($cid) {
    $this->_Cid = $cid;
  }

  /**
   * Create and return a new instance of the filedepot_permission_object with all permissions set
   * @param type $cid
   * @return \filedepot_permission_object
   */
  public static function createFullPermissionObject($cid) {
    $obj = new filedepot_permission_object($cid);
    $obj->setPermissions(true, true, true, true, true, true);
    return $obj;
  }

  /**
   * Create and return a new instance of the filedepot_permission_object with no permissions set
   * @param type $cid
   * @return type
   */
  public static function createNoPermissionsObject($cid) {
    $obj = new filedepot_permission_object($cid);
    return $obj;
  }

  /**
   * Set a single permission by name ONLY if the value is 1 (set) (view, upload, upload_direct, upload_ver, approval, admin)
   * @param type $name
   * @param type $value
   */
  public function setTruePermission($name, $value) {
    if ($value == 1) {
      $this->_PermissionArray[$name] = TRUE;
    }
  }

  public function canUpload() {
    if (($this->hasPermission(filedepot_permission_object::UPLOAD)) || ($this->hasPermission(filedepot_permission_object::UPLOAD_DIRECT)) || ($this->hasPermission(filedepot_permission_object::ADMIN))) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  
  public function canUploadDirect() 
  {
    if (($this->hasPermission(filedepot_permission_object::UPLOAD_DIRECT)) || ($this->hasPermission(filedepot_permission_object::ADMIN))) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Set the permissions ONLY if the value is 1 (set), otherwise nothing happens
   * @param type $view               If 1, will be set to true  
   * @param type $upload             If 1, will be set to true   
   * @param type $upload_direct      If 1, will be set to true   
   * @param type $upload_ver         If 1, will be set to true  
   * @param type $approval           If 1, will be set to true  
   * @param type $admin              If 1, will be set to true  
   */
  public function setTruePermissions($view, $upload, $upload_direct, $upload_ver, $approval, $admin) {
    if ($view == 1) {
      $this->_PermissionArray['view'] = TRUE;
    }

    if ($upload == 1) {
      $this->_PermissionArray['upload'] = TRUE;
    }

    if ($upload_direct == 1) {
      $this->_PermissionArray['upload_direct'] = TRUE;
    }

    if ($upload_ver == 1) {
      $this->_PermissionArray['upload_ver'] = TRUE;
    }

    if ($approval == 1) {
      $this->_PermissionArray['approval'] = TRUE;
    }

    if ($admin == 1) {
      $this->_PermissionArray['admin'] = TRUE;
    }
  }

  /**
   * Set the permissions
   * @param type $view                 If 1, true, else 0 FALSE
   * @param type $upload               If 1, true, else 0 FALSE
   * @param type $upload_direct        If 1, true, else 0 FALSE  
   * @param type $upload_ver           If 1, true, else 0 FALSE
   * @param type $approval             If 1, true, else 0 FALSE
   * @param type $admin                If 1, true, else 0 FALSE
   */
  public function setPermissions($view, $upload, $upload_direct, $upload_ver, $approval, $admin) {
    $this->_PermissionArray['view']          = ($view == 1) ? TRUE : FALSE;
    $this->_PermissionArray['upload']        = ($upload == 1) ? TRUE : FALSE;
    $this->_PermissionArray['upload_direct'] = ($upload_direct == 1) ? TRUE : FALSE;
    $this->_PermissionArray['upload_ver']    = ($upload_ver == 1) ? TRUE : FALSE;
    $this->_PermissionArray['approval']      = ($approval == 1) ? TRUE : FALSE;
    $this->_PermissionArray['admin']         = ($admin == 1) ? TRUE : FALSE;
  }

  /**
   * Checks to see if the CID has the specified permission by name (view, upload, upload_direct, etc)
   * @param type $permission
   */
  public function hasPermission($permission) {
    if ($permission === "upload_dir") {
      $permission = "upload_direct";
    }
    
    if ((isset($this->_PermissionArray[$permission])) && ($this->_PermissionArray[$permission] === TRUE)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Returns TRUE if the user has view permissions, false otherwise
   * @return type
   */
  public function canView() {
    return $this->_PermissionArray['view'];
  }

  public function canManage() {
    return $this->_PermissionArray['admin'];
  }

  public function canDownload() {
    return $this->canView();
  }

}

?>
