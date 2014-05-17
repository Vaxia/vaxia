<?php
/**
  * @file
  * folderperms.tpl.php
  */
?>

<div style="margin:2px;border:1px solid #CCC">
<form name="frmFolderPerms" method="POST">
<input type="hidden" name="op" value="">
<div class="form-layout-default" style="width:720px;margin:0px;height:200px;">
  <div style="float:left;width:45%;">
    <div style="float:left;width:50%;">
      <div style="padding-left:10px;background-color:#BBBECE;"><strong><?php print t('Select Users'); ?></strong></div>
      <div style="padding-top:10px;padding-left:5px;">
        <select name="selusers[]" multiple size=10 class="form-select" style="width:100%;"><?php print $user_options ?></select>
      </div>
    </div>
    <div style="float:left;width:50%;">
      <div style="padding-left:15px;background-color:#BBBECE;"><strong><?php print t('Select Roles'); ?></strong></div>
      <div style="padding-left:5px;padding-top:10px;">
        <select name="selroles[]" multiple size=10 class="form-select" style="width:100%;"><?php print $role_options ?></select>
      </div>
    </div>
  </div>
  <div style="float:left;width:55%;">
    <div style="text-align:center;background-color:#BBBECE;"><strong><?php print t('Access Rights'); ?></strong></div>
      <div style="width:365px;float:right;padding-leftt:10px;font-size:90%;">
        <div>
          <div class="form-item form-option filedepot_perms_leftoption">
            <span style="float:left;"><label for="feature1"><?php print $LANG_viewcategory ?></label></span>
            <span style="float:right;padding-right:5px;"><input type="checkbox" name="cb_access[]" value="view"  id="feature1"></span>
          </div>
          <div class="form-item form-option filedepot_perms_rightoption">
            <span style="float:left;"><label for="feature2"><?php print $LANG_uploadapproval ?></label></span>
            <span style="float:right;padding-right:5px;"><input type="checkbox" name="cb_access[]" value="upload"  id="feature2"></span>
          </div>
        </div>
        <div style="background-color:#FFFFFF;">
          <div class="form-item form-option filedepot_perms_leftoption">
            <span style="float:left;"><label for="feature3"><?php print $LANG_uploadadmin ?></label></span>
            <span style="float:right;padding-right:5px;"><input type="checkbox" name="cb_access[]" value="approval"  id="feature3"></span>
          </div>
          <div class="form-item form-option filedepot_perms_rightoption">
            <span style="float:left;"><label for="feature4"><?php print $LANG_uploaddirect ?></label></span>
            <span style="float:right;padding-right:5px;"><input type="checkbox" name="cb_access[]" value="upload_dir"  id="feature4"></span>
          </div>
        </div>
        <div>
          <div class="form-item form-option filedepot_perms_leftoption">
            <span style="float:left;"><label for="feature5"><?php print $LANG_categoryadmin ?></label></span>
            <span style="float:right;padding-right:5px;"><input type="checkbox" name="cb_access[]" value="admin"  id="feature5"></span>
          </div>
          <div class="form-item form-option filedepot_perms_rightoption">
            <span style="float:left;"><label for="feature6"><?php print $LANG_uploadversions ?></label></span>
            <span style="float:right;padding-right:5px;"><input type="checkbox" name="cb_access[]" value="upload_ver"  id="feature6"></span>
          </div>
        </div>
       </div>
      <div style="clear:both;text-align:center;">
            <input type="button" name="submit" class="form-submit" value="<?php print t('Submit'); ?>" onclick="makeAJAXUpdateFolderPerms(this.form);">
            <span style="padding-left:10px;"><input id="folderperms_cancel" type="button" class="form-submit" value="<?php print t('Close'); ?> "></span>
            <input type="hidden" name="catid" value="<?php print $catid ?>">
            <input type="hidden" id="folderpermstoken" name="token" value="<?php print $token ?>"></td>
      </div>
  </div>
</div>
<div class="clearboth"></div>

<table border="0" cellpadding="5" cellspacing="1" width="100%" style="margin-top:10px;">
  <tr>
    <td colspan="9" width="100%" style="font-weight:bold;background-color:#BBBECE;font-size:2;vertical-align:top;padding:2px;"><?php print t('User Access Records'); ?></td>
  </tr>
  <tr style="font-size:90%;font-weight:bold;background-color:#ECE9D8;text-align:center;vertical-align:top;">
    <td align="left"><?php print t('User'); ?></td>
    <td><?php print $LANG_view ?></td>
    <td><?php print $LANG_uploadwithapproval ?></td>
    <td><?php print $LANG_directupload ?></td>
    <td><?php print $LANG_uploadversions ?></td>
    <td><?php print $LANG_uploadadmin ?></td>
    <td><?php print $LANG_admin ?></td>
    <td style="padding-left:10px;"><?php print $LANG_action ?></td>
  </tr>
  <?php print $user_perm_records ?>
</table>
<table border="0" cellpadding="5" cellspacing="1" width="100%" style="margin-top:20px;margin-bottom:10px;">
  <tr>
    <td colspan="9" width="100%" style="font-weight:bold;background-color:#BBBECE;font-size:2;vertical-align:top;padding:2px;"><?php print t('Role Access Records'); ?></td>
  </tr>
  <tr style="font-size:95%;font-weight:bold;background-color:#ECE9D8;text-align:center;vertical-align:top;">
    <td align="left"><?php print t('Role'); ?></td>
    <td><?php print $LANG_view ?></td>
    <td><?php print $LANG_uploadwithapproval ?></td>
    <td><?php print $LANG_directupload ?></td>
    <td><?php print $LANG_uploadversions ?></td>
    <td><?php print $LANG_uploadadmin ?></td>
    <td><?php print $LANG_admin ?></td>
    <td style="padding-left:10px;"><?php print $LANG_action ?></td>
  </tr>
  <?php print $role_perm_records ?>
</table>
</form>
</div>
