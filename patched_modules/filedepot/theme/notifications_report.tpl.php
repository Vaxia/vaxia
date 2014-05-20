<?php
/**
  * @file
  * notifications_report.tpl.php
  */
?>

<div id="reportlisting_container" style="margin-right:5px;display:none;">
  <div id="notification_report" class="yui-navset">
    <ul class="yui-nav">
      <li class="selected"><a href="#tab1"><em><div style="width:75px;"><?php print $LANG_files_menuitem ?></div></em></a></li>
      <li><a href="#tab2"><em><div style="width:75px;"><?php print $LANG_folder_menuitem ?></div></em></a></li>
      <li><a href="#tab3"><em><div style="width:135px;"><?php print $LANG_history_menuitem ?></div></em></a></li>
      <li><a href="#tab4"><em><div style="width:75px;"><?php print $LANG_settings_menuitem ?></div></em></a></li>
    </ul>
    <div class="yui-content">
      <div id="tab1">
        <div class="filedepotheading">
          <div class="floatleft"><input id="chkallfiles" type="checkbox" value="all" onclick="toggleCheckedNotificationItems(this);"></div>
          <div class="floatleft" style="width:80%;padding-left:5px;">
            <div class="floatleft" style="width:35%"><?php print $LANG_filename ?></div>
            <div class="floatleft" style="width:23%"><?php print $LANG_folder ?></div>
            <div class="floatright" style="padding-right:140px;"><?php print $LANG_dateadded ?></div>
          </div>
          <div class="floatright" style="width:10%;padding-right:10px;"><?php print $LANG_action ?></div>
        </div>
        <?php print $file_records ?>
      </div>
      <div id="tab2">
        <div class="filedepotheading">
          <div class="floatleft"><input id="chkallfolders" type="checkbox" value="all" onclick="toggleCheckedNotificationItems(this);"></div>
          <div class="floatleft" style="width:70%;padding-left:5px;">
            <div class="floatleft" style="width:50%"> <?php print $LANG_folder ?> </div>
            <div class="floatleft" style="width:15%;white-space:nowrap;"><?php print $LANG_dateadded ?></div>
            <div class="floatright" style="width:15%;padding-left:5px;padding-right:5px;white-space:nowrap;"><?php print $LANG_changes ?></div>
            <div class="floatright" style="width:15%;white-space:nowrap;"><?php print $LANG_newfiles ?></div>
          </div>
          <div class="floatright" style="width:10%;padding-right:10px;"><?php print $LANG_action ?></div>
        </div>
        <?php print $folder_records ?>
      </div>
      <div id="tab3">
        <div class="filedepotheading">
          <div id="notificationlog_report">
            <div style="padding:5px;"><a id="clearnotificationhistory" href="#"><?php print $LANG_clearhistory ?></a><span style="padding-left:20px;"><?php print $LANG_historymsg ?></span></div>
            <table width="100%" class="plugin">
              <tr>
                <th width="15%"><?php print $LANG_date ?></th>
                <th width="15%"><?php print $LANG_type ?></th>
                <th width="15%"><?php print $LANG_submitter ?></th>
                <th width="25%"><?php print $LANG_file ?></th>
                <th width="20%"><?php print $LANG_folder ?></th>
              </tr>
              <?php print $history_records ?>
            </table>
          </div>
          <div id="notificationlog_norecords" style="display:none;padding:10px;"><?php print t('No notification history on file'); ?></div>
        </div>

      </div>
      <div id="tab4">
        <div><?php print $LANG_settingheading ?>
          <ul>
            <li><?php print $LANG_settingline1 ?></li>
            <li><?php print $LANG_settingline2 ?></li>
            <li><?php print $LANG_settingline3 ?></li>
            <li><?php print $LANG_settingline4 ?></li>
          </ul>
        </div>

        <table class="plugin" cellpadding="0" cellspacing="1" width="100%">
          <tr>
            <th><?php print $LANG_personalsettings ?></th>
            <th><?php print $LANG_default ?></th>
          </tr>
          <tr>
            <td><?php print $LANG_newfilesadded ?></td>
            <td>
              <div style="float:left;width:60px;"> <?php print t('Yes'); ?> <input type="radio" name="fileadded_notify" value="1" <?php print $chk_fileadded_on ?>></div>
              <?php print t('No'); ?><input type="radio" name="fileadded_notify" value="0" <?php print $chk_fileadded_off ?>></td>
          </tr>
          <tr>
            <td><?php print $LANG_filesupdated ?></td>
            <td>
              <div style="float:left;width:60px;"><?php print t('Yes'); ?><input type="radio" name="fileupdated_notify" value="1" <?php print $chk_filechanged_on ?>></div>
              <?php print t('No'); ?><input type="radio" name="fileupdated_notify" value="0" <?php print $chk_filechanged_off ?>></td>
          </tr>
          <tr>
            <td><?php print $LANG_allowadminbroadcasts ?></td>
            <td>
              <div style="float:left;width:60px;"><?php print t('Yes'); ?><input type="radio" name="admin_broadcasts" value="1" <?php print $chk_broadcasts_on ?>></div>
              <?php print t('No'); ?><input type="radio" name="admin_broadcasts" value="0" <?php print $chk_broadcasts_off ?>></td>
          </tr>
          <tr>
            <td colspan="2" class="aligncenter" style="padding:5px;font-size:12pt;">
              <input type="submit" class="form-submit" value="<?php print $LANG_savesettings ?>" onClick="doAJAXUpdateNotificationSettings(this.form);return false;">
            </td>
          </tr>
        </table>
      </div>
    </div>
  </div>
</div>
