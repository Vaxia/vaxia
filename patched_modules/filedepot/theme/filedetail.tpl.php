<?php
/**
  * @file
  * filedetail.tpl.php
  */
?>

<!-- File Details Panel content - used by the AJAX server.php function nexdocsrv_filedetails() -->
<div class="clearboth">
<table class="plugin" width="100%" border="0" cellspacing="0" cellpadding="2" style="padding-bottom:10px;">
    <tr>
        <td class="aligntop" style="padding:15px 10px 5px 10px;">
          <?php if ($reportmode == 'approvals') { ?>
            <div style="padding-bottom:10px;"><img src="<?php print $fileicon; ?>">&nbsp;<strong><?php print $filetitle ?></strong>&nbsp;<span style="font-size:8pt;"><?php print $current_version ?></span></div>
          <?php } else { ?>
            <div style="padding-bottom:10px;"><img src="<?php print $fileicon; ?>">&nbsp;<a href="<?php print url('filedepot', array('query' => array('cid' => $cid, 'fid' => $fid), 'absolute' => true)); ?>" title="<?php print t('Direct link to file'); ?>" <?php print $disable_download ?>><strong><?php print $filetitle ?></strong></a>&nbsp;<span style="font-size:8pt;"><?php print $current_version ?></span></div>
          <?php } ?>
            <div class="floatleft" style="width:100px;"><strong><?php print t('File Name'); ?>:</strong></div>
            <div class="floatleft"><?php print $real_filename ?></div>
            <div class="clearboth"></div>
            <div class="floatleft" style="width:100px;"><strong><?php print t('Folder'); ?>:</strong></div>
            <div class="floatleft"><?php print $foldername ?></div>
            <div class="clearboth"></div>
            <div class="floatleft" style="width:100px;"><strong><?php print t('Author'); ?>:</strong></div>
            <div class="floatleft" style="font-size:9pt;"><?php print $author ?></div>
            <div class="clearboth"></div>
            <div class="clearboth floatleft" style="width:100px;"><strong><?php print t('Tags'); ?>:</strong></div>
            <div class="floatleft" style="font-size:9pt;"><?php print $tags ?></div>
        </td>
        <td width="15%" class="alignright" style="padding:15px 10px 5px 10px;font-size:8pt;" nowrap><?php print $shortdate ?><br /><strong><?php print t('Size'); ?>:</strong>&nbsp;<?php print $size ?>
        <div id="lockedalertmsg" class="pluginAlert" style="display:<?php print $show_statusmsg ?>;"><?php print $statusmessage ?></div>
        </td>
    </tr>
    <tr>
        <td colspan="2" style="padding:5px 10px;"><strong><?php print t('Description'); ?>:</strong><br /><?php print $description ?></td>
    </tr>
    <tr>
        <td colspan="2" style="padding:5px 10px;"><strong><?php print t('Version Note'); ?>:</strong><br /><?php print $current_ver_note ?></td>
    </tr>
</table>
<?php print $version_records ?>
</div>