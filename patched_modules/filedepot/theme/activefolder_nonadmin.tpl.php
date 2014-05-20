<?php
/**
  * @file
  * activefolder_nonadmin.tpl.php
  */
?>

<div class="folderlink"><a id="folderoptions_link" href="#" TITLE="Click to view folder description and notification options"><?php print $active_folder_name ?></a></div>
<div id="folder_options_container" class="clearboth" style="padding:5px 50px 5px 60px;display:none;">
    <div><?php print $folder_description ?></div>
    <fieldset style="width:300px;margin-top:10px;"><legend><?php print $LANG_folderoptions ?></legend>
    <form name="frm_foldersettings" method="post" style="padding-top:15px;">
        <input type="hidden" name="cid" value="<?php print $active_category_id ?>">
        <table cellpadding="0" cellspacing="1" width="100%">
            <tr>
                <td><?php print $LANG_newfiles ?></td>
                <td><input type="checkbox" name="fileadded_notify" value="1" <?php print $chk_fileadded ?>></td>
            </tr>
            <tr>
                <td><?php print $LANG_filechanges ?></td>
                <td><input type="checkbox" name="filechanged_notify" value="1" <?php print $chk_filechanged ?>></td>
            </tr>
            <tr>
                <td colspan="2" class="aligncenter" style="padding:5px;">
                    <input type="submit" value="Save Settings" onClick="doAJAXUpdateFolderNotificationSettings(this.form);return false;">
                    <span style="padding-left:10px;"><input type="button" class="form-submit" value="<?php print t('Close'); ?>" onClick="togglefolderoptions();"></span>
                </td>
            </tr>
        </table>
    </form>
    </fieldset>
</div>