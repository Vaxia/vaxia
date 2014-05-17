<?php
/**
  * @file
  * activefolder_admin.tpl.php
  */
?>

<div id="activefolder"><a href="#" onclick="edit_activefolder();" TITLE="<?php print $LANG_click_adminmsg ?>"><?php print $active_folder_name ?></a></div>

<div id="edit_activefolder">
  <form name="frm_activefolder" method="post" action="<?php print $ajax_server_url ?>">
    <input type="hidden" name="op" value="updatefolder">
    <input type="hidden" name="cid" value="<?php print $active_category_id ?>">
    <div style="float:left;width:68%;">
      <table class="plugin">
        <tr>
          <td width="100">Folder Name:</td>
          <td><input type="text" name="categoryname" class="form-text" value="<?php print $active_folder_name ?>" style="width:270px"></td>
        </tr>
        <tr>
          <td><?php print t('Parent Folder'); ?></td>
          <td><select id="folder_parent" name="catpid" class="form-select" style="width:270px">
              <?php print $folder_parent_options ?>
            </select>
          </td>
        </tr>
        <tr>
          <td width="100"><?php print $LANG_description ?>:</td>
          <td><textarea name="catdesc" class="form-textarea" rows="3" style="width:265px;font-size:10pt;"><?php print $folder_description ?></textarea></td>
        </tr>
        <?php if(variable_get('filedepot_override_folderorder', 0) == 0) { ?>
        <tr>
          <td width="100"><?php print $LANG_folderorder ?>:</td>
          <td><input type="text" name="folderorder" class="form-text" value="<?php print $folderorder ?>" size="5"><span class="pluginTinyText" style="padding-left:10px;"><?php print $LANG_folderordermsg ?></span></td>
        </tr>
        <?php } ?>
        <tr>
          <td colspan="2">
            <div style="float:left;width:200px;"><?php print $LANG_newfiles ?></div>
            <div style="float:left;padding-left:10px;"><input type="checkbox" name="fileadded_notify" value="1" <?php print $chk_fileadded ?>></div>
          </td>
        </tr>
        <tr>
          <td colspan="2">
            <div style="float:left;width:200px;"><?php print $LANG_filechanges ?></div>
            <div style="float:left;padding-left:10px;"><input type="checkbox" name="filechanged_notify" value="1" <?php print $chk_filechanged ?>></div>
          </td>
        </tr>
        <tr>
          <td colspan="2" style="padding-top:10px;padding-bottom:10px;white-space:nowrap">
            <input type="button" value="<?php print t('Update'); ?>" class="form-submit" onClick="makeAJAXUpdateFolderDetails(this.form)"/>
            <span style="padding-left:5px;">
              <input type="button" value="<?php print t('Close'); ?>" class="form-submit" onClick="toggleElements('edit_activefolder','activefolder');">
            </span>
            <span class="deletebuttonborder">
              <input type="button" value="<?php print t('Delete'); ?>" class="form-submit" onclick="delete_activefolder(this.form);">
            </span>
            <div style="display:inline;margin-left:15px;padding:5px 1px;">
              <input type="button" value="<?php print t('Permissions'); ?>" class="form-submit" onClick="makeAJAXShowFolderPerms(this.form);">
            </div>
          </td>
        </tr>
      </table>
    </div>
    <div style="float:left;margin-left:2px;width:30%">
      <table class="plugin" style="margin-bottom:10px;">
        <tr>
          <td colspan="2" class="pluginReportTitle pluginMediumText"><?php print $LANG_statsmsg ?></td>
        </tr>
        <tr>
          <td width="100" class="pluginRow2"><?php print $LANG_foldercount ?> :</td><td class="pluginRow1"><?php print $folder_count ?></td>
        </tr>
        <tr>
          <td width="100" class="pluginRow2"><?php print $LANG_filecount ?> :</td><td class="pluginRow1"><?php print $file_count ?></td>
        </tr>
        <tr>
          <td width="100" class="pluginRow1"><?php print $LANG_totalsize ?> :</td><td class="pluginRow1"><?php print $total_size ?></td>
        </tr>
      </table>
    </div>
    <div class="clearboth" style="padding-bottom:10px;"></div>
    <input type="hidden" name="token" value="<?php print $token ?>" />
  </form>
</div>