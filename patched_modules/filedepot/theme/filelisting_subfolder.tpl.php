<?php
/**
  * @file
  * filelisting_subfolder.tpl.php
  */
?>  

<div id="subfolder<?php print $folder_id ?>" class="subfolder listing_record parentfolder<?php print $parent_folder_id ?>">
  <div style="padding-right:20px;">
    <div class="folder_withhover" style="width:100%;">
      <div class="floatleft">
        <input type="checkbox" name="chkfolder" value="<?php print $folder_id ?>" onclick="toggleCheckedItems(this,'<?php print $folder_files ?>');">
        <span id="subfolder_icon<?php print $folder_id ?>" class="icon-folderclosed" style="padding:0px 5px;" onClick="togglefolder(<?php print $folder_id ?>);">&nbsp;</span>
        <span style="padding-left:<?php print $folder_padding_left ?>px;"><a href="#"><img src="<?php print $layout_url ?>/css/images/allfolders-16x16.png"></a></span>
        <span style="padding-left:5px;padding-right:5px;color:#666;"><?php print $folder_number ?></span>
        <span class="folderlink"><a href="#" onClick="makeAJAXGetFolderListing(<?php print $folder_id ?>);return false;" TITLE="Folder id: <?php print $folder_id ?>  Parent:<?php print $parent_folder_id ?>"><?php print$folder_name ?></a></span>
      </div>
      <div class="floatright" style="padding-right:30px;width:125px;"><?php print $last_modified_date ?>&nbsp;</div>
      <?php print $onhover_move_options ?>
    </div>
  </div>
</div>
<div class="subfolder_container" id="subfolder<?php print $folder_id ?>_contents" style="display:none;">
  <?php print $subfoldercontent ?>
  <div id="subfolderlisting<?php print $folder_id ?>_bottom"></div>
</div> <!-- end of subfolder container -->
<div id="subfolder<?php print $folder_id ?>_bottom"></div>
