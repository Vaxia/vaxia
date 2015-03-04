<?php
/**
  * @file
  * page.tpl.php
  */
?>

<?php
  // Initialize variable id unknown to solve any PHP Notice level error messages
  if (!isset($search_query)) $search_query = 0;
?>

<!-- On-Demand loading the Module Javascript using YUI Loader -->

<script type="text/javascript">
  var useYuiLoader = true;         // Set to false if you have manually loaded all the needed YUI libraries else they will dynamically be loaded
  var pagewidth = 0;               // Integer value: Use 0 for 100% width with auto-resizing of layout, or a fixed width in pixels
                                   // Height of main display is set by the height of #filedepot div - default set in CSS
  var numGetFileThreads = 5;       // Max number of concurrent AJAX threads to spawn in the background to retrieve & render record details for subfolders
  var useModalDialogs = true;      // Default of true is preferred but there is an IE7 bug that makes them un-useable so in this case set to false

  // Do not modify any variables below
  var filedepotfolders = '';
  var filedepotdetail = '';
  var folderstack = new Array;  // will contain list of folders being processed by AJAX YAHOO.filedepot.getmorefiledata function
  var fileID;
  var initialfid = <?php print $initialfid ?>;
  var initialcid = <?php print $initialcid ?>;
  var initialop = '<?php print $initialop ?>';
  var initialparm = '<?php print $initialparm ?>';
  var siteurl = '<?php print $site_url ?>';
  var ajax_post_handler_url = '<?php print $ajax_server_url ?>';
  var actionurl_dir = '<?php print $actionurl_dir ?>';
  var imgset = '<?php print $layout_url ?>/css/images';
  var ajaxactive = false;
  var clear_ajaxactivity = false;
  var blockui = false;
  var timerArray = new Array();
  var lastfiledata = new Array();
  var expandedfolders = new Array();
  var searchprompt = '<?php print t('Keyword Search'); ?>';
  var show_upload = <?php print $show_upload ?>;
  var show_newfolder = <?php print $show_newfolder ?>;
</script>

<script type="text/javascript">
  var YUIBaseURL  = "<?php print $yui_base_url ?>";
</script>

<script type="text/javascript">
   jQuery.blockUI();
   if (useYuiLoader == true) {
    // Instantiate and configure Loader:
    var loader = new YAHOO.util.YUILoader({

      base: YUIBaseURL,
      // Identify the components you want to load.  Loader will automatically identify
      // any additional dependencies required for the specified components.
      require: ["container","layout","resize","connection","dragdrop","menu","button","tabview","treeview","element","cookie","logger","animation"],

      // Configure loader to pull in optional dependencies.  For example, animation
      // is an optional dependency for slider.
      loadOptional: true,

      // The function to call when all script/css resources have been loaded
      onSuccess: function() {
        blockui=true;
        //jQuery.blockUI();
        timeDiff.setStartTime();
        Dom = YAHOO.util.Dom;
        Event = YAHOO.util.Event;
        Event.onDOMReady(function() {
          setTimeout('init_filedepot()',1000);
        });
      },
      onFailure: function(o) {
        alert("The required javascript libraries could not be loaded.  Please refresh your page and try again.");
      },

      allowRollup: true,

      // Configure the Get utility to timeout after 10 seconds for any given node insert
      timeout: 10000,

      // Combine YUI files into a single request (per file type) by using the Yahoo! CDN combo service.
      combine: false
    });

    // Load the files using the insert() method. The insert method takes an optional
    // configuration object, and in this case we have configured everything in
    // the constructor, so we don't need to pass anything to insert().
    loader.insert();

  } else {
    blockui=true;
    jQuery.blockUI();
    timeDiff.setStartTime();
    Dom = YAHOO.util.Dom;
    Event = YAHOO.util.Event;
    Event.onDOMReady(function() {
      setTimeout('init_filedepot()',1000);
    });
  }

</script>


<!-- filedepot module wrapper div -->
<div id="filedepotmodule">

  <div id="filedepot">

      <div id="filedepottoolbar" class="filedepottoolbar" style="margin-right:0px;padding:5px;display:none;margin-bottom:1px;">
      <div style="float:left;width:250px;height:20px;padding-left:20px;">
      <?php if ($show_newfolder == 'true') { ?>
        <span id="newfolderlink">
          <span class="first-child">
            <a class="ctools-use-modal ctools-modal-filedepot-newfolder-dialog-style" href="<?php print url('filedepot/nojs/newfolder'); ?>"><?php echo t('New Folder'); ?></a>
          </span>
        </span>
      <?php } ?>
      <?php if ($show_upload == 'true') { ?>
        <span id="newfilelink">
          <span class="first-child">
            <a class="ctools-use-modal ctools-modal-filedepot-newfile-dialog-style" href="<?php print url('filedepot/nojs/newfile'); ?>"><?php echo t('New File'); ?></a>
          </span>
        </span>
      <?php } ?>
      </div>
      <?php print $toolbarform ?>
      <div class="filedepottoolbar_searchbox">
        <div class="filedepottoolbar_searchform">
          <form name="fsearch" onSubmit="makeAJAXSearch();return false;">
            <input type="hidden" name="tags" value="">
            <table>
              <tr>
                <td width="50%"><input type="text" size="20" name="query" id="searchquery" class="form-text" style="margin-top:-2px;padding:3px 3px 5px 3px;" value="<?php print $search_query ?>" onClick="this.value='';"></td>
                <td width="50%" style="text-align:right;"><input type="button" id="searchbutton" value="<?php print t('Search') ?>"></td>
              </tr>
            </table>
          </form>
        </div>
        <div class="tagsearchboxcontainer" style="width:10%;padding:5px;">
          <div><a id="showsearchtags" href="#"><?php echo t('Tags'); ?></a></div>
        </div>
      </div>
    </div>
    <div class="tagsearchboxcontainer">
      <div id="tagspanel" style="display:none;">
        <div class="hd"><?php print t('Search Tags'); ?></div>
        <div id="tagcloud" class="bd tagcloud">
          <?php print $tagcloud ?>
        </div>
      </div>
    </div>

    <div id="filedepot_sidecol">
      <!-- Leftside Folder Navigation generated onload by page javascript -->

      <div id="filedepotNavTreeDiv"></div>
      <div class="clearboth"></div>
    </div>
    <div id="filedepot_centercol">
      <div id="filedepot_alert" class="filedepot_alert" style="display: <?php print $show_alert ?>;overflow:hidden;">
        <div id="filedepot_alert_content" class="floatleft"><?php print $alert_message ?></div>
        <div id="cancelalert" class="floatright" style="position:relative;top:4px;padding-right:10px;">
          <a class="cancelbutton" href="#">&nbsp;</a>
        </div>
        <div class="clearboth"></div>
      </div>
      <div id="activefolder_container"></div>
      <div class="clearboth" id="showactivetags" style="display:none;">
        <div id="tagsearchbox" style="padding-bottom:5px;">Search Tags:&nbsp;<span id="activesearchtags"></span></div>
      </div>
      <div class="clearboth"></div>
      <div style="margin-right:0px;">
        <div id="filelistingheader" style="margin-bottom:10px;"></div>
        <div class="clearboth"></div>
        <form name="frmfilelisting" action="<?php print $actionurl_dir; ?>" method="post" style="margin:0px;">
          <div id="filelisting_container"></div>
        </form>
      </div>
      <div class="clearboth"></div>

    </div> <!-- end of filedepot_centercol div -->

    <div class="clearboth"></div>
  </div>   <!-- end of filedepot div -->

  <div class="clearboth"></div>

  <!-- Supporting Panels - initially hidden -->

    <div id="filedetails" style="display:none;">
      <div id="filedetails_titlebar" class="hd"><?php print t('File Details'); ?></div>
      <div id="filedetailsmenubar" class="yuimenubar" style="border:0px;">
        <div class="bd" style="margin:0px;padding:0px 2px 2px 2px;border:0px;font-size:11pt;">
          <ul class="first-of-type">
            <li id="displaymenubaritem" class="yuimenubaritem first-of-type">
              <a id="menubar_downloadlink" href="" TITLE="<?php print t('Download file'); ?>"><?php print t('Download'); ?></a>
            </li>
            <li id="editmenubaritem" class="yuimenubaritem first-of-type">
              <a id="editfiledetailslink" href="#" TITLE="<?php print t('Edit File'); ?>"> <?php print t('Edit'); ?> </a>
            </li>
            <li id="addmenubaritem" class="yuimenubaritem first-of-type">
              <a id="newversionlink" href="#" TITLE="<?php print t('Upload new version'); ?>"><?php print t('New Version'); ?></a>
            </li>
            <li id="approvemenubaritem" class="yuimenubaritem first-of-type">
              <a id="approvefiledetailslink" href="#" TITLE="<?php print t('Approve File Submission'); ?>"><?php print t('Approve'); ?></a>
            </li>
            <li id="deletemenubaritem" class="yuimenubaritem first-of-type">
              <a id="deletefiledetailslink" href="#" TITLE="<?php print t('Delete File'); ?>"><?php print t('Delete'); ?></a>
            </li>
            <li id="lockmenubaritem" class="yuimenubaritem first-of-type">
              <a id="lockfiledetailslink" href="#" TITLE="<?php print t('Lock File'); ?>"><?php print t('Lock'); ?></a>
            </li>
            <li id="notifymenubaritem" class="yuimenubaritem first-of-type">
              <a id="notifyfiledetailslink" href="#" TITLE="<?php print t('Enable email notification for any updates'); ?>"><?php print t('Subscribe'); ?></a>
            </li>
            <li id="broadcastmenubaritem" class="yuimenubaritem first-of-type">
              <a id="broadcastnotificationlink" href="#" TITLE="<?php print t('Send out a broadcast email notification'); ?>"><?php print t('Broadcast Notification'); ?></a>
            </li>
          </ul>
        </div>
      </div>
      <div id="filedetails_statusmsg" class="pluginInfo alignleft" style="display:none;"></div>
      <div id="displayfiledetails" class="alignleft" style="display:block;">

      </div>

      <div id="editfiledetails" class="alignleft" style="display:none;">
        <form id="frmFileDetails" name="frmFileDetails" method="POST">
          <input type="hidden" name="cid" value="">
          <input type="hidden" name="id" value="">
          <input type="hidden" name="version" value="">
          <input type="hidden" name="tagstore" value="">
          <input type="hidden" name="approved" value="">
          <input type="hidden" name="ftoken" id="frmFileDetails_ftoken" value="" />

          <table width="100%" style="margin:10px;">
            <tr>
              <td width="100"><label><?php print t('File Name'); ?></label></td>
              <td width="225"><input type="text" class="form-text" name="filetitle" size="29" value="" style="width:195px;" /></td>
              <td width="80"><label><?php print t('Folder'); ?></label></td>
              <td width="255" id="folderoptions"></td>
            </tr>
            <tr style="vertical-align:top;">
              <td rowspan="3"><label><?php print t('Description'); ?></label></td>
              <td rowspan="3"><textarea rows="6" cols="30" name="description" style="width:195px;"></textarea></td>
              <td><label><?php print t('Owner'); ?></label></td>
              <td><span id="disp_owner"></span></td>
            </tr>
            <tr style="vertical-align:top;">
              <td><label><?php print t('Date'); ?></label></td>
              <td><span id="disp_date"></span></td>
            </tr>
            <tr>
              <td><label><?php print t('Size'); ?></label></td>
              <td><span id="disp_size"></span></td>
            </tr>
            <tr style="vertical-align:top;">
              <td><label><?php print t('Version Notes'); ?></label></td>
              <td><textarea rows="3" cols="30" name="version_note" style="width:195px;"></textarea></td>
              <td><label><?php print t('Tags'); ?></label></td>
              <td>
                <div id="tagsfield" style="padding-bottom:15px;">
                  <input id="editfile_tags" class="form-text" name="tags" type="text" size="30" style="width:210px" />
                  <div id="editfile_autocomplete" style="width:210px;"></div>
                </div>
                <div id="tagswarning" class="pluginAlert" style="width:180px;display:none;"><?php print t('Folder Perms not set'); ?></div>
              </td>
            </tr>
            <tr>
              <td colspan="4" style="padding-top:10px;text-align:center;">
                <input type="button" value="<?php print t('Submit'); ?>" class="form-submit" onClick="makeAJAXUpdateFileDetails(this.form)"/>
                <span style="padding-left:10px;"><input id="filedetails_cancel" class="form-submit" type="button" value="<?php print t('Cancel'); ?>"></span>
              </td>
            </tr>
          </table>
        </form>
      </div>
    </div>

    <div id="folderperms" style="display:none;">
      <div class="hd">Folder Permissions</div>
      <div id="folderperms_content" class="bd alignleft"></div>
    </div>

    <div id="newfolderdialog" style="display:none;">
      <div class="hd"><?php print t('Add a new folder'); ?></div>
      <div id="newfolderdialog_form" class="bd" style="text-align:left;">

      </div>
    </div>

    <div id="moveIncomingFileDialog" style="display:none;">
      <div class="hd"><?php print t('Move Selected Files'); ?></div>
      <div class="pluginInfo alignleft" style="color:#000;font-size:90%"><?php print t('Select the destination folder'); ?></div>
      <div id="movebatchfiledialog_form" class="bd" style="text-align:left;">

      </div>
    </div>

    <div id="movebatchfilesdialog" style="display:none;">
      <div class="hd"><?php print t('Move Selected File'); ?></div>
      <div class="pluginInfo alignleft" style="color:#000;font-size:90%"><?php print t('Only selected files where you are the Owner or have Folder Admin rights will be moved to the new target folder'); ?></div>
      <div id="movebatchfilesdialog_form" class="bd" style="text-align:left;">

      </div>
    </div>

    <div id="newfiledialog" style="display:none;">
      <div id="newfiledialog_heading" class="hd"></div>
      <div class="bd" style="text-align:left;">
        <?php
          $form = drupal_get_form('filedepot_newversion_form');
          print drupal_render($form);
        ?>
      </div>
    </div>

    <div id="broadcastDialog" style="display:none;">
      <div class="hd"><?php print t('Send out a Broadcast Notification'); ?></div>
      <div class="pluginInfo alignleft" style="color:#000;font-size:90%"><?php print t('Broadcast email will be sent to all site members that have view access to this folder alerting them. A link to this file will be included'); ?></div>
      <div class="bd" style="text-align:left;">
        <form id="frmBroadcast" name="frmBroadcast" method="post">
          <input type="hidden" name="fid" value="">
          <input type="hidden" name="cid" value="">
          <table class="formtable">
            <tr>
              <td><label for="parent"><?php print t('Message'); ?>:</label>&nbsp;</td>
              <td><textarea name="message" rows="4" class="form-textarea" style="width:300px;font-size:10pt;"></textarea></td>
            </tr>
            <tr>
              <td colspan="2" style="text-align:center;padding:15px;">
                <input id="btnBroadcastSubmit" type="button" class="form-submit" value="<?php print t('Send'); ?>">
                <span style="padding-left:10px;">
                  <input id="btnBroadcastCancel" type="button" class="form-submit" value="<?php print t('cancel'); ?>">
                </span>
              </td>
            </tr>
          </table>
        </form>
      </div>
    </div>


</div>   <!-- end of filedepotmodule wrapper div -->

<div class="clearboth"></div>
