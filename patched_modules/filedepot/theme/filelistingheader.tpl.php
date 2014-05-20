<?php
/**
  * @file
  * filelistingheader.tpl.php
  */
?>


<div class="filedepotheading" style="display: <?php print $show_mainheader ?>">
  <input type="hidden" name="ltoken" id="flistingltoken" value="<?php print $token ?>" />
  <div class="floatleft"><input id="headerchkall" type="checkbox" value="all" onclick="toggleCheckedItems(this);"></div>
  <div class="floatleft" style="padding-left:35px;padding-right:10px;">
    <div class="floatleft"><?php print $LANG_filename ?>
      <span id="showhidedetail" style="padding-left:20px;">
        [&nbsp;<a href="#" onClick="showhideFileDetail();"><?php print $LANG_showfiledetails ?></a>&nbsp;]
      </span>
      <span id="expandcollapsefolders" style="display: <?php print $show_folderexpandlink ?>;padding-left:5px;">
        [&nbsp;<a id="expandcollapsefolderslink" href="?op=expand" onClick="expandCollapseFolders(this);return false;"><?php print $LANG_expandfolders ?></a>&nbsp;]
      </span>
    </div>
  </div>
  <div class="floatright" style="padding-right: <?php print $rightpadding ?>px;">
    <div class="floatright"><?php print $LANG_action ?></div>
    <div class="floatright" style="padding-right:55px;"><?php print $LANG_date ?></div>
    <div class="floatright" style="display: <?php print $show_folder ?>;padding-right:25px"><?php print $LANG_folder ?></div>
  </div>
</div>
<div class="filedepotheading" style="display: <?php print $show_incomingheader ?>">
  <div class="floatleft"><input id="headerchkall" type="checkbox" value="all" onclick="toggleCheckedItems(this);"></div>
  <div class="floatleft" style="padding-left:35px;padding-right:10px;">
    <div class="floatleft"><?php print $LANG_filename ?></div>
  </div>
  <div class="floatright" style="padding-right:60px;">
    <div class="floatright"><?php print $LANG_action ?></div>
    <div class="floatright" style="padding-right:45px;"><?php print $LANG_submitted ?></div>
    <div class="floatright" style="padding-right:15px"><?php print $LANG_owner ?></div>
  </div>
</div>

