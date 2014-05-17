<?php
/**
  * @file
  * notifications_file_record.tpl.php
  */
?>

<div class="listing_record" id="folder_<?php print $folder_id ?>_rec_{fid}" style="padding-left:0px">
  <div class="floatleft">
    <input id="chkfile<?php print $fid ?>" type="checkbox" name="chkfile" value="<?php print $recid ?>" onClick="updateCheckedItems(this,'file')">
  </div>
  <div class="floatleft" style="width:82%;padding-left:5px;">
    <div class="floatleft" style="width:34%"><?php print $filename ?></div>
    <div class="floatleft" style="width:23%"><a href="#" onClick="makeAJAXGetFolderListing(<?php print $folderid ?>);return false;"><?php print $foldername ?></a></div>
    <div class="floatright" style="padding-right:160px;"><?php print $date ?></div>
  </div>
  <div class="floatright" style="width:10%;padding-right:10px;"><a href="#" onclick="doAJAXDeleteNotification('file',<?php print $recid ?>);return false;"><?php print $LANG_delete ?></a></div>
</div>


