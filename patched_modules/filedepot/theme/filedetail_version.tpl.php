<?php
/**
  * @file
  * filedetail_version.tpl.php
  */
?>

<table width="100%" border="0" cellspacing="2" cellpadding="0">
    <tr class="pluginRow<?php print $cssid ?>">
        <td class="aligntop" style="padding-left:10px;padding-top:10px;"><img src="<?php print $fileicon ?>">&nbsp;<a href="<?php print url('filedepot_download/' . $nid . '/' . $fid . '/' . $file_version, array('absolute' => true)); ?>" TITLE="<?php print t('Download File');?>"><strong><?php print $vname ?></strong></a>&nbsp;<span style="font-size:9pt;"><?php print $file_versionnum ?></span></td>
        <td width="15%" class="alignright" style="padding-top:10px;" nowrap><span style="font-size:8pt;"><?php print $ver_shortdate ?><br /><strong><?php print t('Size'); ?>:</strong>&nbsp;<?php print $ver_size ?></span></td>
    </tr>
    <tr class="pluginRow<?php print $cssid ?>">
        <td colspan="2" style="padding-left:10px;">
            <div><strong><?php print t('Version Note'); ?>:</strong></div>
            <div id="detaildisp<?php print $fid ?>v<?php print $file_version ?>">
                <div style="width:555px;height:63px;overflow:auto;border:0px solid #CCC;"><?php print $version_note ?></div>
            </div>
            <div id="detailedit<?php print $fid ?>v<?php print $file_version ?>" style="display:none;">
                <form method="post" style="margin:0px;padding:0px;">
                    <input type="hidden" name="op" value="">
                    <input type="hidden" name="fid" value="<?php print $fid ?>">
                    <input type="hidden" name="version" value="<?php print $file_version ?>">
                    <textarea name="note" rows="3" cols="75"><?php print $edit_version_note ?></textarea>
                    <div style="padding-top:5px;padding-left:200px;">
                      <input class="form-submit" type="button" value="<?php print t('Update'); ?>" onclick="doAJAXEditVersionNote(this.form)"><span style="padding-left:10px;">
                      <input type="button" class="form-submit" value="<?php print t('Cancel'); ?>" onClick="toggleElements('detaildisp<?php print $fid ?>v<?php print $file_version ?>','detailedit<?php print $fid ?>v<?php print $file_version ?>');return false;"></span>
                    </div>
                </form>
            </div>
        </td>
    </tr>
    <tr class="pluginRow<?php print $cssid ?>">
        <td colspan="2" style="padding-left:10px;">
        <span><?php print t('Author'); ?>:&nbsp;<?php print $ver_author ?>&nbsp;&nbsp;<a href="<?php print url('filedepot_download/' . $nid . '/' . $fid . '/' . $file_version, array('absolute' => true)); ?>"><?php print t('Download File'); ?></a></span>
        <span style="padding-left:10px;display:<?php print $show_edit_version;?>"><a href="#" onClick="toggleElements('detaildisp<?php print $fid ?>v<?php print $file_version ?>','detailedit<?php print $fid ?>v<?php print $file_version ?>');return false;"><?php print t('Edit File'); ?></a></span>
        <span style="padding-left:10px;display:<?php print $show_delete_version;?>"><a href="#" onclick="doAJAXDeleteVersion(<?php print $fid ?>,<?php print $file_version ?>)"><?php print t('Delete File'); ?></a></span>
        </td>
    </tr>
</table>