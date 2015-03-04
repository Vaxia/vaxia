/*
 * @file common.js
 * Common or main libray of javascript functions for the drupal filedepot module
 */

/**
 * Nextide filedepot class
 */
function NxFiledepot() {
  // empty
};

Global_checkedItemsDict = {
  folders: {},
  files: {}
};

/**
 * Checked items manager class
 */
NxFiledepot.checkedItemsManager = function() {

};

/**
 * Create a checked item object from the parameters passed
 *
 * @param id            ID of the folder or file
 * @param ischecked     Boolean true or false to indicate if checked or nots
 * @param pid           Parent ID
 */
NxFiledepot.checkedItemsManager.createCheckedItemObject = function(id, ischecked, pid) {
  return {
    id: id,
    checked: ischecked,
    pid : pid
  };
};

/**
 * Add a new checked folder item to the list
 *
 * @param checkedItemObj          Object with parameters: { id : "folder_id", checked : "boolean" }
 */
NxFiledepot.checkedItemsManager.setFolder = function(checkedItemObj) {
  Global_checkedItemsDict.folders[checkedItemObj.id] = checkedItemObj;
};

/**
 * Add a new checked file item to the list
 *
 * @param checkedItemObj          Object with parameters: { id : "file_id", checked : "boolean" }
 */
NxFiledepot.checkedItemsManager.setFile = function(checkedItemObj) {
  Global_checkedItemsDict.files[checkedItemObj.id] = checkedItemObj;
};

/**
 * Returns true if anything is selected, false otherwise
 */
NxFiledepot.checkedItemsManager.areSelected = function() {
  var s = NxFiledepot.checkedItemsManager.exportFolders();
  var q = NxFiledepot.checkedItemsManager.exportFiles();
  if ((s == '{}') && (q == '{}')) { // empty
    return false;
  }
  else {
    return true;
  }
};

/**
 * Export folder checked items as a string
 */
NxFiledepot.checkedItemsManager.exportFolders = function() {
  return JSON.stringify(Global_checkedItemsDict.folders);
};

NxFiledepot.checkedItemsManager.areFilesSelected = function() {
  var selected = false;

  for(var key in Global_checkedItemsDict.files) {
    if (Global_checkedItemsDict.files[key].id == undefined) {
      continue;
    }
    else if (Global_checkedItemsDict.files[key].checked == false) {
      continue;
    }
    else {
      if (Global_checkedItemsDict.files[key].pid in Global_checkedItemsDict.folders) {
        if (Global_checkedItemsDict.folders[Global_checkedItemsDict.files[key].pid].checked == true) {
          continue;
        }
      }
    }

    selected = true;
    break;
  }

  return selected;
};

/**
 * Export file checked items as a string
 */
NxFiledepot.checkedItemsManager.exportFiles = function() {
  return JSON.stringify(Global_checkedItemsDict.files);
};

/**
 * Clear folders and files
 */
NxFiledepot.checkedItemsManager.clearAll = function() {
  Global_checkedItemsDict.folders = { };
  Global_checkedItemsDict.files = { };
};

 YAHOO.namespace("filedepot");
 YAHOO.namespace("container");

function closeAlert() {
  Dom.setStyle('cancelalert', 'display', 'none'); // Hide the cancel icon
  // Note: Key to this working is having the div's overflow:auto set
  var myAnim = new YAHOO.util.Anim('filedepot_alert', {height: {to: 0}},1, YAHOO.util.Easing.easeOut);
  myAnim.animate();
  timer = setTimeout("Dom.setStyle('filedepot_alert', 'display', 'none')", 1000);
}

function showAlert(message,autoclose) {
  Dom.get('filedepot_alert_content').innerHTML = message;
  Dom.setStyle('filedepot_alert', 'display', '');
  Dom.setStyle('cancelalert', 'display', ''); // Show the cancel icon
  if (autoclose == undefined) {
    var myAnim = new YAHOO.util.Anim('filedepot_alert', {height: {to: 30}},1, YAHOO.util.Easing.easeOut);
    myAnim.animate();
  }
}

function updateAjaxStatus(message) {
  try {
    Dom.get('filedepot_ajaxStatus').innerHTML = '';
    if(message) {
      if (message == 'activity') {
        ajaxactive = true;
        Dom.setStyle('filedepot_ajaxActivity','visibility','visible');
      } else {
        Dom.get('filedepot_ajaxStatus').innerHTML = message;
        Dom.setStyle('filedepot_ajaxStatus','visibility','visible');
      }
    } else {
      ajaxactive = false;
      Dom.setStyle('filedepot_ajaxActivity','visibility','hidden');
      Dom.setStyle('filedepot_ajaxStatus','visibility','hidden');
    }
  } catch  (e) {}
}

function clearAjaxActivity() {
  clear_ajaxactivity = true;
  //YAHOO.log('Clear ' + timerArray.length + ' timers');
  for(var i=0; i< timerArray.length; i++) {
    clearTimeout(timerArray[i]);
  }
  timerArray = new Array;
}

function clearBlockUi() {
  setTimeout('jQuery.unblockUI()',200);
  blockui = false;
}


function hideFileDetailsPanel() {
  Dom.get('displayfiledetails').innerHTML = '';
  YAHOO.container.filedetails.hide();
  YAHOO.container.menuBar.cfg.setProperty("visible",false);
}

// Used to close the file details dialog once a download is started from clicking on the filename link or menu.
// Found that IE would only handle this if I used a delay.
function hideFileDetailsPanelDelay() {
  timer = setTimeout( function() {
    Dom.get('displayfiledetails').innerHTML = '';
    YAHOO.container.filedetails.hide();
    YAHOO.container.menuBar.cfg.setProperty("visible",false);
  }, 2000);
}

function showFolderMoveActions(e,obj) {
  Event.preventDefault(e);
  moveDivList = Dom.getElementsByClassName('foldermovelinks','',obj);
  if (moveDivList.length == 1) {
    Dom.setStyle(moveDivList[0],'visibility','visible');
  }
}

function hideFolderMoveActions(e,obj) {
  Event.preventDefault(e);
  moveDivList = Dom.getElementsByClassName('foldermovelinks','',obj);
  if (moveDivList.length == 1) {
    Dom.setStyle(moveDivList[0],'visibility','hidden');
  }
}

function setSearchButtonFocus () {
  Dom.get('searchbutton').focus();
}

function edit_activefolder() {
  Dom.setStyle('activefolder', 'display', 'none');
  Dom.setStyle('edit_activefolder', 'display', 'block');
}

function togglefolderoptions() {
  if (Dom.get('folder_options_container').style.display == 'none') {
    Dom.setStyle('folder_options_container','display','');
    Dom.get('folderoptions_link').title = NEXLANG_click2close;
  } else {
    Dom.setStyle('folder_options_container','display','none');
    Dom.get('folderoptions_link').title = NEXLANG_click2viewfolder;
  }
}


function toggle_filedetails(e) {
  if (Dom.get('editfiledetailslink').innerHTML == 'Display') {
    //YAHOO.log('toggle_filedetails - none ');
    Dom.setStyle('displayfiledetails', 'display', '');
    Dom.setStyle('editfiledetails', 'display', 'none');
    Dom.get('editfiledetailslink').innerHTML = 'Edit';
  } else {
    //YAHOO.log('toggle_filedetails - block');
    Dom.setStyle('displayfiledetails', 'display', 'none');
    Dom.setStyle('editfiledetails', 'display', '');
    Dom.get('editfiledetailslink').innerHTML = 'Display';
  }
}



function expandfolder(id) {
  var el = 'subfolder_icon' + id ;
  var elc = 'subfolder' + id + '_contents';
  Dom.replaceClass(el, 'icon-folderclosed', 'icon-folderopen');
  Dom.setStyle(elc,'display','');

  // If this folder is closed but empty, and in hide details mode, then show placeholder message
  Dom.setStyle('emptyfolder_' + id ,'display','');
  Dom.setStyle('emptyfolder' + id + '_contents','display','');

  // Check if all records are now open
  recordList = Dom.getElementsByClassName('icon-folderclosed');
  if (recordList.length == 0) {
    Dom.get('expandcollapsefolderslink').href='?op=collapse';
    Dom.get('expandcollapsefolderslink').innerHTML='Collapse Folders';
  }

}

// Recursive function called to collapse subfolders
function collapseSubFolders(pid) {
  expandedfolders = arrayRemoveItem(expandedfolders, pid); // Remove this folder from the array we use to track expanded folders
  subfolderList = Dom.getElementsByClassName('parentfolder' + pid);
  for(var i=0; i< subfolderList.length; i++) {
    var elparts = subfolderList[i].id.split('subfolder');
    var el = 'subfolder_icon' + elparts[1] ;
    var elc = 'subfolder' + elparts[1] + '_contents';
    Dom.replaceClass(el, 'icon-folderopen', 'icon-folderclosed');
    Dom.setStyle(elc,'display','none');
    expandedfolders = arrayRemoveItem(expandedfolders, elparts[1]);
  }
}

function togglefolder(id) {
  var el = 'subfolder_icon' + id ;
  var elc = 'subfolder' + id + '_contents';
  if (Dom.hasClass(el, 'icon-folderclosed')) {
    expandedfolders.push(id);      // Add this expanded folder to the array we use to track expanded folders
    expandfolder(id);

  } else {
    Dom.replaceClass(el, 'icon-folderopen', 'icon-folderclosed');
    Dom.setStyle(elc,'display','none');

    collapseSubFolders(id);

    // Check if all records are now closed
    recordList = Dom.getElementsByClassName('icon-folderopen');
    if (recordList.length == 0) {
      Dom.get('expandcollapsefolderslink').href='?op=expand';
      Dom.get('expandcollapsefolderslink').innerHTML = NEXLANG_click2expandfolders;
    }
  }

  YAHOO.filedepot.alternateRows.init('listing_record');

}

function expandCollapseFolders(obj,mode) {

  if (!mode || mode == '') {
    var params = obj.getAttribute('href');
    var mode = parseURL(params,'op');
  }

  if (mode == 'expand') {
    YAHOO.util.Cookie.set("filedepotfolders", "expanded");
    filedepotfolders = 'expanded';
    var elements = Dom.getElementsByClassName('icon-folderclosed','span', 'filelisting_container');
    for (var i = 0; i < elements.length; i++) {
      var folder = elements[i].id.split('subfolder_icon');
      var id = folder[1];
      var el = elements[i];
      var elc = 'subfolder' + folder[1] + '_contents';
      Dom.replaceClass(el, 'icon-folderclosed', 'icon-folderopen');
      Dom.setStyle(elc,'display','');

      // If this folder is closed but empty, and in hide details mode, then show placeholder message
      Dom.setStyle('emptyfolder_' + id ,'display','');
      Dom.setStyle('emptyfolder' + id + '_contents','display','');
    }
    obj.href='?op=collapse';
    obj.innerHTML = NEXLANG_click2collapsefolders;
  } else if (mode == 'collapse') {
    YAHOO.util.Cookie.set("filedepotfolders", "collapsed");
    filedepotfolders = 'collapsed';
    var elements = Dom.getElementsByClassName('icon-folderopen','span', 'filelisting_container');
    for (var i = 0; i < elements.length; i++) {
      var folder = elements[i].id.split('subfolder_icon');
      var id = folder[1];
      var el = elements[i];
      var elc = 'subfolder' + folder[1] + '_contents';
      Dom.replaceClass(el, 'icon-folderopen', 'icon-folderclosed');
      Dom.setStyle(elc,'display','none');
      expandedfolders = arrayRemoveItem(expandedfolders,id); // Remove this folder from the array we use to track expanded folders
    }
    obj.href='?op=expand';
    obj.innerHTML = NEXLANG_click2expandfolders;
  }
  YAHOO.filedepot.alternateRows.init('listing_record');

}

/* If mode (show/hide) is empty, then function will toggle the filedetails */
function showhideFileDetail(mode) {
  var elements = Dom.getElementsByClassName('filedesc','div', 'filelisting_container');
  if (mode != 'hide' && mode != 'show') {
    if (filedepotdetail == 'collapsed') {
      mode = 'show';
    } else {
      mode = 'hide';
    }
  }
  for (var i = 0; i < elements.length; i++) {
    if (mode == 'show') {
      var listingDescChildren = Dom.getElementsByClassName('filedesc_span','span',elements[i]);
      // Should just return the one span tag and we can test if it contains anything to display
      try {
        if (listingDescChildren[0].innerHTML != '') {
          Dom.setStyle(elements[i],'display','');
        }
      } catch (e) {}
    } else if (!Dom.hasClass(elements[i].parentNode,'emptyfolder')) {
      // Check if this node is folder that has no items - don't display the filedesc div
      Dom.setStyle(elements[i],'display','none');
    }
  }

  if (elements.length > 0) {
    if (mode == 'show') {
      YAHOO.util.Cookie.set("filedepotdetail", "expanded");
      filedepotdetail = 'expanded';
      var elm = Dom.getFirstChild('showhidedetail');
      elm.innerHTML = NEXLANG_click2hidedetail;
    } else {
      YAHOO.util.Cookie.set("filedepotdetail", "collapsed");
      filedepotdetail = 'collapsed';
      var elm = Dom.getFirstChild('showhidedetail');
      elm.innerHTML = NEXLANG_click2showdetail;
    }
  }

  var elements = Dom.getElementsByClassName('emptyfolder','div', 'filelisting_container');
  for (var i = 0; i < elements.length; i++) {
    if (Dom.getStyle(elements[i],'display') == 'none') {
      Dom.setStyle(elements[i],'display','');
    } else {
      // If this folder is closed but empty, and in hide details mode, then show placeholder message
      var x = elements[i].id.split('_');
      if (x[1] > 0) {
        if (Dom.hasClass('subfolder'+x[1],'icon-folderopen')) {
          Dom.setStyle('emptyfolder' + x[1] + '_contents','display','');
        }
      }
    }
  }

}


function delete_activefolder(frm) {
  if (confirm(NEXLANG_deletefolderconfirm)) {
    var surl = ajax_post_handler_url + '/deletefolder';
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (oResults.retcode == 200) {
          Dom.get('activefolder_container').innerHTML = oResults.activefolder;
          renderLeftNavigation(oResults);
          renderFileListing(oResults);
          YAHOO.filedepot.alternateRows.init('listing_record');
        } else {
          alert(NEXLANG_errormsg2);
        }
        updateAjaxStatus();
      },
      failure: function(o) {
        YAHOO.log(NEXLANG_ajaxerror + o.status);
      },
      argument: {},
      timeout:55000
    }
    YAHOO.util.Connect.setForm(frm);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback);

  } else {
    Dom.setStyle('edit_activefolder', 'display', 'none');
    Dom.setStyle('activefolder', 'display', 'block');
    return false;

  }
}

function deletefile() {
  var reportmode = document.frmtoolbar.reportmode.value;
  if (reportmode == 'approvals') {
    if (confirm(NEXLANG_deletesubmission)) {
      var fid = document.frmFileDetails.id.value;
      makeAJAXDeleteFile(fid);
    } else {
      return false;
    }
  } else {
    if (confirm(NEXLANG_deletefileconfirm)) {
      var fid = document.frmFileDetails.id.value;
      makeAJAXDeleteFile(fid);
    } else {
      return false;
    }
  }
}


function broadcastnotification() {
  YAHOO.container.broadcastDialog.cfg.setProperty("visible",true);
}


function checkMultiAction(selectoption) {
  if (selectoption == 'delete') {
    if (document.frmtoolbar.reportmode.value == 'notifications') {
      var msg = NEXLANG_deletemultiplenotifications;
    } else {
      var msg = NEXLANG_deletemultiplefiles;
    }
    if (confirm(msg)) {
      var surl = ajax_post_handler_url + '/deletecheckedfiles';
      var ltoken = document.getElementById("flistingltoken").value;
      var postdata = '&ltoken=' + ltoken;
      var callback = {
        success : function(o) {
          var json = o.responseText.substring(o.responseText
          .indexOf('{'), o.responseText.lastIndexOf('}') + 1);
          var oResults = eval('(' + json + ')');
          if (oResults.retcode == 200) {
            if (document.frmtoolbar.reportmode.value == 'notifications') {
              Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
              var myTabs = new YAHOO.widget.TabView('notification_report');
              Dom.setStyle('filelistingheader', 'display', 'none');
              Dom.setStyle('reportlisting_container', 'display', '');
              YAHOO.filedepot.alternateRows.init('listing_record');
              // Setup the Notifications Settings Dialog
              Dom.setStyle('notificationsettingsdialog', 'display', 'block');
              if (!Event.getListeners('clearnotificationhistory')) {   // Check first to see if listener already active
                Event.on('clearnotificationhistory', 'click', doAJAXClearNotificationLog);
              }
            } else {
              if (oResults.errmsg != '') {
                showAlert(oResults.errmsg);
              }
              renderFileListing(oResults);
              try {
                if (oResults.lastrenderedfiles) {
                  YAHOO.filedepot.getmorefiledata(oResults.lastrenderedfiles);
                } else {
                  YAHOO.filedepot.alternateRows.init('listing_record');
                }
              } catch (e) {
                YAHOO.filedepot.alternateRows.init('listing_record');
              }
            }
            Dom.get('headerchkall').checked = false;
            Dom.get('multiaction').selectedIndex = 0;
            Dom.get('multiaction').disabled = true;
          } else {
            alert(NEXLANG_errormsg1);
          }
          updateAjaxStatus();
        },
        failure : function(o) {
          YAHOO.log('AJAX Error: ' + o.status);
        },
        argument : {},
        timeout : 55000
      }
      updateAjaxStatus(NEXLANG_activity);
      var formObject = document.frmtoolbar;
      YAHOO.util.Connect.setForm(formObject);
      YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
      return false;

    } else {
      // Resetting the select element so user can easily re-select same
      // option twice else onChange will not fire
      timer = setTimeout("Dom.get('multiaction').selectedIndex=0", 3000);
      return false;
    }

  } else if (selectoption == 'move') {
    var surl = ajax_post_handler_url + '/rendermoveform';
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        Dom.get('movebatchfilesdialog_form').innerHTML = oResults.displayhtml;
        YAHOO.container.batchfilemovedialog.cfg.setProperty("visible",true);
        if (!Event.getListeners('btnMoveFolderSubmit')) {   // Check first to see if listener already active
          Event.addListener("btnMoveFolderSubmit", "click", moveSelectedFiles);
        }
        if (!Event.getListeners('btnMoveFolderCancel')) {   // Check first to see if listener already active
          Event.addListener("btnMoveFolderCancel", "click",YAHOO.container.batchfilemovedialog.hide, YAHOO.container.batchfilemovedialog, true);
        }
        // Resetting the select element so user can easily re-select same option twice else onChange will not fire
        timer = setTimeout("Dom.get('multiaction').selectedIndex=0", 3000);
      },
      failure: function(o) {
        YAHOO.log('AJAX error loading move folder options : ' + o.status);
      },
      argument: {},
      timeout:55000
    }
    YAHOO.util.Connect.asyncRequest('POST', surl, callback);
    return false;

  } else if (selectoption == 'markfavorite') {
    var surl = ajax_post_handler_url + '/markfavorite';
    var ltoken = document.getElementById("flistingltoken").value;
    var postdata = '&ltoken=' + ltoken;
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (oResults.retcode == 200) {
          renderFileListing(oResults);
          YAHOO.filedepot.alternateRows.init('listing_record');
          Dom.get('headerchkall').checked = false;
          Dom.get('multiaction').selectedIndex=0;
          Dom.get('multiaction').disabled = true;
        } else {
          alert(NEXLANG_errormsg1);
        }
        updateAjaxStatus();
      },
      failure: function(o) {
        YAHOO.log('AJAX Error: ' + o.status);
      },
      argument: {},
      timeout:55000
    }
    updateAjaxStatus(NEXLANG_activity);
    var formObject = document.frmtoolbar;
    YAHOO.util.Connect.setForm(formObject);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
    return false;

  } else if (selectoption == 'clearfavorite') {
    var surl = ajax_post_handler_url + '/clearfavorite';
    var ltoken = document.getElementById("flistingltoken").value;
    var postdata = '&ltoken=' + ltoken;
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (oResults.retcode == 200) {
          renderFileListing(oResults);
          YAHOO.filedepot.alternateRows.init('listing_record');
          Dom.get('headerchkall').checked = false;
          Dom.get('multiaction').selectedIndex=0;
          Dom.get('multiaction').disabled = true;
        } else {
          alert(NEXLANG_errormsg1);
        }
        updateAjaxStatus();
      },
      failure: function(o) {
        YAHOO.log('AJAX Error: ' + o.status);
      },
      argument: {},
      timeout:55000
    }
    updateAjaxStatus(NEXLANG_activity);
    var formObject = document.frmtoolbar;
    YAHOO.util.Connect.setForm(formObject);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
    return false;

  } else if (selectoption == 'approvesubmissions') {
    var surl = ajax_post_handler_url + '/approvesubmissions';
    var ltoken = document.getElementById("flistingltoken").value;
    var postdata = '&ltoken=' + ltoken;
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (oResults.retcode == 200) {
          renderFileListing(oResults);
          renderLeftNavigation(oResults);
          Dom.get('headerchkall').checked = false;
          Dom.get('multiaction').selectedIndex=0;
          Dom.get('multiaction').disabled = true;
        } else {
          alert(NEXLANG_errormsg1);
        }
        updateAjaxStatus();
      },
      failure: function(o) {
        YAHOO.log('AJAX Error: ' + o.status);
      },
      argument: {},
      timeout:55000
    }
    updateAjaxStatus(NEXLANG_activity);
    var formObject = document.frmtoolbar;
    YAHOO.util.Connect.setForm(formObject);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
    return false;

  } else if (selectoption == 'deletesubmissions') {
    var surl = ajax_post_handler_url + '/deletesubmissions';
    var ltoken = document.getElementById("flistingltoken").value;
    var postdata = '&ltoken=' + ltoken;

    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (oResults.retcode == 200) {
          renderFileListing(oResults);
          renderLeftNavigation(oResults);
          Dom.get('headerchkall').checked = false;
          Dom.get('multiaction').selectedIndex=0;
          Dom.get('multiaction').disabled = true;
          YAHOO.filedepot.alternateRows.init('listing_record');
        } else {
          alert(NEXLANG_errormsg1);
        }
        updateAjaxStatus();
      },
      failure: function(o) {
        YAHOO.log('AJAX Error: ' + o.status);
      },
      argument: {},
      timeout:55000
    }
    updateAjaxStatus(NEXLANG_activity);
    var formObject = document.frmtoolbar;
    YAHOO.util.Connect.setForm(formObject);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
    return false;

  } else if (selectoption == 'subscribe') {
    var surl = ajax_post_handler_url + '/multisubscribe';
    var ltoken = document.getElementById("flistingltoken").value;
    var postdata = '&ltoken=' + ltoken;
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (oResults.retcode == 200) {
          renderFileListing(oResults);
          renderLeftNavigation(oResults);
          Dom.get('headerchkall').checked = false;
          document.frmtoolbar.multiaction.selectedIndex=0;
          document.frmtoolbar.multiaction.disabled = true;
        } else {
          alert(NEXLANG_errormsg1);
        }
        updateAjaxStatus();
      },
      failure: function(o) {
        YAHOO.log('AJAX Error: ' + o.status);
      },
      argument: {},
      timeout:55000
    }
    updateAjaxStatus(NEXLANG_activity);
    var formObject = document.frmtoolbar;
    YAHOO.util.Connect.setForm(formObject);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
    return false;

  } else if ((selectoption == "download") || (selectoption == "archive")) {
    document.frmtoolbar.multiaction.selectedIndex=0;
    var ltoken = document.getElementById("flistingltoken").value;
    var urlargs = 'ltoken=' + ltoken + '&checked_files=' + encodeURIComponent(NxFiledepot.checkedItemsManager.exportFiles()) + '&checked_folders=' + encodeURIComponent(NxFiledepot.checkedItemsManager.exportFolders());
    var surl = ajax_post_handler_url + '/archive';
    
    // Are clean URLs enabled?
    if (surl.indexOf('?') != -1) { 
      // not enabled
      surl += '&' + urlargs;
    }
    else {
      // enabled
      surl += '?' + urlargs;
    }
    
    jQuery('input[name=chkfolder]').attr('checked', false);
    resetCheckedItems();
    enable_multiaction('','');
    window.location = surl;
    return false;
  } else if (selectoption == 0) {
    return false;
  }
  else {
    return true;
  }
}

function postSubmitMultiactionResetIfNeed(selectoption) {
  if (selectoption == 'download') {
    jQuery('input[name=chkfolder]').attr('checked', false);
    resetCheckedItems();
    enable_multiaction('','');
    timer = setTimeout('document.frmtoolbar.multiaction.selectedIndex=0', 3000);
  } else if (selectoption == 'delete') {
    if (document.frmtoolbar.reportmode.value == 'notifications') {
      clearCheckedItems();
    }
  }

}

function moveSelectedFiles() {
  var newcid = document.frmBatchMove.movebatchfiles.value;
  var ltoken = document.getElementById("flistingltoken").value;
  document.frmtoolbar.newcid.value = newcid;
  // Since I am resetting the selectbox in checkMultiAction(), I need to now set it to the 'move' option
  Dom.get('multiaction').selectedIndex=2;
  YAHOO.container.batchfilemovedialog.hide();
  var surl = ajax_post_handler_url + '/movecheckedfiles';
  var postdata = '&ltoken=' + ltoken;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        if (document.frmtoolbar.reportmode.value == 'incoming')
          document.frmtoolbar.reportmode.value = '';
        if (oResults.cid > 0) {
          makeAJAXGetFolderListing(oResults.cid);
        } else {
          renderFileListing(oResults);
        }
        if (oResults.message != '') {
          showAlert(oResults.message);
        }
        Dom.get('headerchkall').checked = false;
        Dom.get('multiaction').selectedIndex=0;
        Dom.get('multiaction').disabled = true;
      } else {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log('AJAX Error: ' + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activity);
  var formObject = document.frmtoolbar;
  YAHOO.util.Connect.setForm(formObject);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
}



function makeAJAXCreateFolder() {
  var surl = ajax_post_handler_url + '/createfolder';
  var formObject = document.getElementById('frmNewFolder');

  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        makeAJAXGetFolderListing(oResults.displaycid);
      } else {
        alert(oResults.errmsg);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }

  YAHOO.util.Connect.setForm(formObject, false);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback);
};




var makeAJAXLoadFileDetails = function(id) {
  closeAlert();
  var reportmode = document.frmtoolbar.reportmode.value;
  YAHOO.container.filedetails.focusFirst();
  Dom.get('displayfiledetails').innerHTML = '';
  YAHOO.container.menuBar.cfg.setProperty("visible",true);
  YAHOO.container.filedetails.cfg.setProperty("visible",true);
  YAHOO.container.filedetails.cfg.setProperty("fixedcenter",false);
  document.frmFileDetails.description.value = '';
  document.frmFileDetails.version_note.value = '';
  document.frmFileDetails.editfile_tags.value = '';
  Dom.setStyle('displayfiledetails', 'display', 'block');
  Dom.setStyle('editfiledetails', 'display', 'none');
  try {
    Dom.get('editfiledetailslink').innerHTML = 'Edit';
  } catch (e) {}

  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.error.length == 0) {
        Dom.get('displayfiledetails').innerHTML = oResults.displayhtml;
        document.getElementById("frmFileDetails_ftoken").value = oResults.token;

        if (!oResults.downloadperm) {
          YAHOO.container.menuBar.getItem(0).cfg.setProperty("disabled", true);
          YAHOO.util.Event.removeListener("menubar_downloadlink", "click");
        } else {
          YAHOO.container.menuBar.getItem(0).cfg.setProperty("disabled", false);
          var download_url = siteurl + '/index.php?q=filedepot_download/' + oResults.nid + '/' + oResults.fid;
          if (reportmode == 'incoming') download_url += '/incoming';
          if (reportmode == 'approvals') download_url += '/0/moderator';

          Dom.get('menubar_downloadlink').href = download_url;
          if (!Event.getListeners('menubar_downloadlink')) {   // Check first to see if listener already active
            Event.addListener("menubar_downloadlink", "click", hideFileDetailsPanelDelay);
          }
        }

        if (!oResults.editperm) {
          YAHOO.container.menuBar.getItem(1).cfg.setProperty("disabled", true);
          YAHOO.util.Event.removeListener("editfiledetailslink", "click");
        } else {
          YAHOO.container.menuBar.getItem(1).cfg.setProperty("disabled", false);
          if (!Event.getListeners('editfiledetailslink')) {   // Check first to see if listener already active
            Event.addListener("editfiledetailslink", "click", toggle_filedetails);
          }
        }
        if (!oResults.addperm) {
          YAHOO.container.menuBar.getItem(2).cfg.setProperty("disabled", true);
          YAHOO.util.Event.removeListener("newversionlink", "click");
        } else {
          YAHOO.container.menuBar.getItem(2).cfg.setProperty("disabled", false);
          if (!Event.getListeners('newversionlink')) {   // Check first to see if listener already active
            Event.addListener("newversionlink", "click", showAddNewVersion, YAHOO.container.newfiledialog, true);
          }

        }
        if (!oResults.deleteperm) {
          YAHOO.container.menuBar.getItem(4).cfg.setProperty("disabled", true);
          YAHOO.util.Event.removeListener("deletefiledetailslink", "click");
        } else {
          YAHOO.container.menuBar.getItem(4).cfg.setProperty("disabled", false);
          if (!Event.getListeners('deletefiledetailslink')) {   // Check first to see if listener already active
            Event.addListener("deletefiledetailslink", "click", deletefile);
          }
        }
        if (!oResults.lockperm) {
          YAHOO.container.menuBar.getItem(5).cfg.setProperty("disabled", true);
          YAHOO.util.Event.removeListener("lockfiledetailslink", "click");
        } else {
          YAHOO.container.menuBar.getItem(5).cfg.setProperty("disabled", false);
          if (oResults.locked) {
            Dom.get('lockfiledetailslink').innerHTML = 'UnLock';
          } else {
            Dom.get('lockfiledetailslink').innerHTML = 'Lock';
          }
          if (!Event.getListeners('lockfiledetailslink')) {   // Check first to see if listener already active
            Event.addListener("lockfiledetailslink", "click", adminToggleFilelock);
          }
        }
        if (!oResults.notifyperm) {
          YAHOO.container.menuBar.getItem(6).cfg.setProperty("disabled", true);
          Event.removeListener("notifyfiledetailslink", "click");
        } else {
          YAHOO.container.menuBar.getItem(6).cfg.setProperty("disabled", false);
          if (!Event.getListeners('notifyfiledetailslink')) {   // Check first to see if listener already active
            Event.addListener("notifyfiledetailslink", "click", adminToggleNotification);
          }
        }
        if (!oResults.broadcastperm) {
          YAHOO.container.menuBar.getItem(7).cfg.setProperty("disabled", true);
          Event.removeListener("broadcastnotificationlink", "click");
        } else {
          YAHOO.container.menuBar.getItem(7).cfg.setProperty("disabled", false);
          if (!Event.getListeners('broadcastnotificationlink')) {   // Check first to see if listener already active
            Event.addListener("broadcastnotificationlink", "click", broadcastnotification);
          }
        }

        if (oResults.status == 0) {       // Un-Approved File
          Dom.setStyle('newversionlink', 'display', 'none');
          Dom.setStyle('lockmenubaritem', 'display', 'none');
          Dom.setStyle('notifymenubaritem', 'display', 'none');
          Dom.setStyle('approvefiledetailslink', 'display', '');
          document.frmFileDetails.approved.value = 0;
          if (!Event.getListeners('approvefiledetailslink')) {   // Check first to see if listener already active
            Event.addListener("approvefiledetailslink", "click", adminApproveSubmission);
          }
        } else {
          Dom.setStyle('newversionlink', 'display', '');
          Dom.setStyle('lockmenubaritem', 'display', '');
          Dom.setStyle('notifymenubaritem', 'display', '');
          Dom.setStyle('approvefiledetailslink', 'display', 'none');
          document.frmFileDetails.approved.value = 1;
          Event.removeListener("approvefiledetailslink", "click");
        }



        document.frmBroadcast.fid.value = oResults.fid;
        document.frmBroadcast.message.value = '';
        document.frmFileDetails.id.value = oResults.fid;
        document.frmFileDetails.version.value = oResults.version;
        document.frmFileDetails.filetitle.value = oResults.title;
        document.frmFileDetails.cid.value = oResults.cid;
        Dom.get('filedetails_titlebar').innerHTML = oResults.title + '&nbsp;-&nbsp;' + NEXLANG_details;
        Dom.get('disp_owner').innerHTML = oResults.name;
        Dom.get('disp_date').innerHTML = oResults.date;
        Dom.get('disp_size').innerHTML = oResults.size;
        document.frmFileDetails.description.value = Encoder.htmlDecode(oResults.description.replace('<br />','','g'));
        document.frmFileDetails.version_note.value = oResults.version_note.replace('<br />','','g');
        document.frmFileDetails.editfile_tags.value = oResults.tags.replace('<br />','','g');
        Dom.get('folderoptions').innerHTML = oResults.folderoptions;
        if (reportmode == 'incoming') {
          document.frmFileDetails.cid.value = 'incoming';
          Dom.setStyle('tagswarning','display','none');
          Dom.setStyle('tagsfield','display','none');
        } else {
          if (oResults.tagperms) {
            Dom.setStyle('tagsfield','display','block');
            Dom.setStyle('tagswarning','display','none');
          } else {
            Dom.setStyle('tagsfield','display','none');
            Dom.setStyle('tagswarning','display','block');
          }
        }

        if (oResults.locked) {
          try {
            Dom.get('lockfiledetailslink').innerHTML = 'UnLock';
          } catch(e) {}
        } else {
          try {
            Dom.get('lockfiledetailslink').innerHTML = 'Lock';
          } catch (e) {}
        }
        if (oResults.subscribed) {
          try {
            Dom.get('notifyfiledetailslink').innerHTML = 'UnSubscribe';
          } catch (e) {}
        } else {
          try {
            Dom.get('notifyfiledetailslink').innerHTML = 'Subscribe';
          } catch (e) {}
        }

      } else {
        alert(oResults.error);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000

  }
  updateAjaxStatus(NEXLANG_activitymsg1);
  var surl = ajax_post_handler_url + '/loadfiledetails';
  var postdata = '&id=' + id + '&reportmode=' + reportmode;
  YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
}


function makeAJAXUpdateFileDetails(formObject,fid) {
  var reportmode = document.frmtoolbar.reportmode.value;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        if (oResults.fid > 0) {
          Dom.get('listingDescriptionRec' + oResults.fid ).innerHTML = oResults.description;
          if (oResults.description != '') {
            Dom.setStyle('filedesc_container_' + oResults.fid ,'display','block');
          } else {
            Dom.setStyle('filedesc_container_' + oResults.fid ,'display','none');
          }
          if (document.frmFileDetails.approved.value == 1) {
            Dom.get('listingTagsRec' + oResults.fid ).innerHTML = oResults.tags;
          }
          Dom.get('listingFilenameRec' + oResults.fid ).innerHTML = oResults.filename;

          if (oResults.filemoved && reportmode == 'incoming') {
            renderLeftNavigation(oResults);
            Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
            YAHOO.filedepot.alternateRows.init('listing_record');
            clearCheckedItems();
            hideFileDetailsPanel();
          } else if (oResults.filemoved && oResults.cid > 0) {
            document.location = siteurl + '?q=filedepot&cid=' + oResults.cid;
          } else if (oResults.errmsg.length > 0) {
                showAlert(oResults.errmsg);
          } else {
            Dom.get('tagcloud_words').innerHTML = oResults.tagcloud;
          }
          if (reportmode != 'incoming') {
            if (oResults.tagerror != '') {
              Dom.get('filedetails_statusmsg').innerHTML = oResults.tagerror;
              Dom.setStyle('filedetails_statusmsg', 'display', 'block');
              timer = setTimeout('Dom.setStyle("filedetails_statusmsg", "display", "none")', 3000);
            } else {
              Dom.get('listingTagsRec'+oResults.fid).innerHTML = oResults.tags;
              YAHOO.container.menuBar.cfg.setProperty("visible",false)
              hideFileDetailsPanel();
            }
          } else {
              hideFileDetailsPanel();
          }
        } else {
          YAHOO.container.menuBar.cfg.setProperty("visible",false)
          hideFileDetailsPanel();
        }
      } else {
        alert(oResults.errmsg);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activitymsg2);
  YAHOO.util.Connect.setForm(formObject, false);
  YAHOO.util.Connect.asyncRequest('POST', ajax_post_handler_url + '/updatefile' , callback);
};


// Show Folder Perms - Called to retrieve and show the folder permissions panel for the selected folder
function makeAJAXShowFolderPerms(formObject) {
  var cid=formObject.cid.value;
  var surl = ajax_post_handler_url + '/getfolderperms'
  var postdata = '&cid=' + cid;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        Dom.get('folderperms_content').innerHTML = oResults.html;
        YAHOO.container.folderperms.cfg.setProperty("visible",true);
        Event.addListener("folderperms_cancel", "click", YAHOO.container.folderperms.hide, YAHOO.container.folderperms, true);
      } else {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activitymsg3);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
};


function makeAJAXUpdateFolderPerms(formObject) {
  var surl = ajax_post_handler_url + '/addfolderperm';
  YAHOO.util.Connect.setForm(formObject, false);
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        Dom.get('folderperms_content').innerHTML = oResults.html;
        YAHOO.container.folderperms.cfg.setProperty("visible",true);
        Event.addListener("folderperms_cancel", "click", YAHOO.container.folderperms.hide, YAHOO.container.folderperms, true);
      } else if (oResults.retcode != 204) {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activitymsg4);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback);
};


function makeAJAXUpdateFolderDetails(formObject) {
  if (!blockui)  {
    blockui=true;
    jQuery.blockUI();
  }
  var surl = ajax_post_handler_url + '/updatefolder';
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        makeAJAXGetFolderListing(oResults.cid);
      } else {
        clearBlockUi();
        toggleElements('edit_activefolder','activefolder');
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activitymsg5);
  YAHOO.util.Connect.setForm(formObject, false);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback);
};


// Called from the confirm message displayed by deleteFileHandleSuccess() when user clicks on link in message
function deleteFileSuccessConfirmAction() {
  var cid = document.frmFileDetails.cid.value;
  hideFileDetailsPanel();
}


function makeAJAXDeleteFile(fid) {
  var listingcid = document.frmtoolbar.cid.value;
  var reportmode = document.frmtoolbar.reportmode.value;
  var ftoken = document.getElementById("frmFileDetails_ftoken").value;
  var surl = ajax_post_handler_url + '/deletefile';
  var postdata = '&fid=' + fid + '&listingcid=' + listingcid + '&reportmode=' + reportmode + '&ftoken=' + ftoken;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        if (reportmode == 'incoming') {
            renderLeftNavigation(oResults);
            Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
            YAHOO.filedepot.alternateRows.init('listing_record');
            clearCheckedItems();
            hideFileDetailsPanel();

        } else {
          Dom.get('folder_' + oResults.cid + '_rec_' + oResults.fid).innerHTML = oResults.filemsg;
          YAHOO.container.menuBar.cfg.setProperty("visible",false);
          Dom.get('filedetails_titlebar').innerHTML = oResults.title;
          Dom.get('displayfiledetails').innerHTML = oResults.message;
          timer = setTimeout('deleteFileSuccessConfirmAction()', 3000);
          renderFileListing(oResults);
          try {
            if (oResults.lastrenderedfiles) {
              //YAHOO.log('showfiles: initiate getmorefiledata:' + timeDiff.getDiff() + 'ms');
              YAHOO.filedepot.getmorefiledata(oResults.lastrenderedfiles);
              //YAHOO.log('showfiles: completed getmorefiledata:' + timeDiff.getDiff() + 'ms');
            } else {
              YAHOO.filedepot.alternateRows.init('listing_record');
            }
          } catch(e) {YAHOO.filedepot.alternateRows.init('listing_record');}
        }
      } else {
        alert(NEXLANG_errormsg3);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000

  }
  updateAjaxStatus(NEXLANG_activitymsg6);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback,postdata);
};


function adminToggleFilelock() {
  if (!ajaxactive) {
    ajaxactive = true;
    var fid = document.frmFileDetails.id.value;
    var surl = ajax_post_handler_url + '/togglelock';
    var ftoken = document.getElementById("frmFileDetails_ftoken").value;
    var postdata = '&fid=' + fid + '&ftoken=' + ftoken;
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (oResults.error == '') {
          Dom.get('filedetails_statusmsg').innerHTML = oResults.message;
          Dom.setStyle('filedetails_statusmsg', 'display', 'block');
          if (oResults.locked) {
            Dom.get('lockfiledetailslink').innerHTML = NEXLANG_unlock;
            Dom.get('lockedalertmsg').innerHTML = oResults.locked_message;
            Dom.setStyle("lockedalertmsg", "display", "block");
            Dom.setStyle('listingLockIconRec' + oResults.fid, "display", '');
          } else {
            Dom.get('lockfiledetailslink').innerHTML = NEXLANG_lock;
            Dom.get('lockedalertmsg').innerHTML = '';
            Dom.setStyle("lockedalertmsg", "display", "none");
            Dom.setStyle('listingLockIconRec' + oResults.fid, "display", "none");
          }
          timer = setTimeout('Dom.setStyle("filedetails_statusmsg", "display", "none")', 3000);
        } else {
          alert(NEXLANG_errormsg1);
        }
        setTimeout('updateAjaxStatus()',500)
      },
      failure: function(o) {
        YAHOO.log(NEXLANG_ajaxerror + o.status);
      },
      argument: {},
      timeout:55000
    }
    updateAjaxStatus(NEXLANG_activitymsg7);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback,postdata);
  }
};

function adminApproveSubmission () {
  var id = document.frmFileDetails.id.value;
  var surl = ajax_post_handler_url + '/approvefile';
  var ltoken = document.getElementById("flistingltoken").value;
  var postdata = '&ltoken=' + ltoken + '&id=' + id;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        renderLeftNavigation(oResults);
        renderFileListing(oResults);
        YAHOO.container.menuBar.cfg.setProperty("visible",false)
        hideFileDetailsPanel();
      } else {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000

  }
  updateAjaxStatus(NEXLANG_activitymsg1);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);

}


function adminToggleNotification() {
  if (!ajaxactive) {
    ajaxactive = true;
    var fid = document.frmFileDetails.id.value;
    var surl = ajax_post_handler_url + '/togglesubscribe';
    var ftoken = document.getElementById("frmFileDetails_ftoken").value;
    var postdata = '&fid=' + fid + '&ftoken=' + ftoken;
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (oResults.error == '') {
          if (oResults.message != '') {
            Dom.get('filedetails_statusmsg').innerHTML = oResults.message;
            Dom.setStyle('filedetails_statusmsg', 'display', 'block');
          } else {
            Dom.get('filedetails_statusmsg').innerHTML = '';
            Dom.setStyle('filedetails_statusmsg', 'display', 'none');
          }
          var obj = Dom.get('listingNotifyIconRec' + oResults.fid );
          if (obj) {
            obj.src = oResults.notifyicon;
            obj.title = oResults.notifymsg;
          }
          if (oResults.subscribed) {
            Dom.get('notifyfiledetailslink').innerHTML = NEXLANG_unsubscribe;
          } else {
            Dom.get('notifyfiledetailslink').innerHTML = NEXLANG_subscribe;
          }
          timer = setTimeout('Dom.setStyle("filedetails_statusmsg", "display", "none")', 3000);
          updateAjaxStatus();
        } else {
          alert(NEXLANG_errormsg1);
        }
      },
      failure: function(o) {
        YAHOO.log(NEXLANG_ajaxerror + o.status);
      },
      argument: {},
      timeout:55000
    }
    updateAjaxStatus(NEXLANG_activitymsg9);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback,postdata);
  }
};


function makeAJAXToggleFavorite(id) {
  var surl = ajax_post_handler_url + '/togglefavorite';
  var ltoken = document.getElementById("flistingltoken").value;
  var postdata = '&id=' + id + '&ltoken=' + ltoken;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        var obj = Dom.get('favitem' + id);
        if (obj) obj.src = oResults.favimgsrc;
      } else {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback,postdata);
};


function makeAJAXGetFolderListing(cid) {
  if (!blockui)  {
    blockui=true;
    jQuery.blockUI();
  }
  timeDiff.setStartTime(); // Reset the timer
  document.frmtoolbar.newcid.value = cid;
  var surl = ajax_post_handler_url + '/getfolderlisting';
  var postdata = '&cid=' + cid;
  var callback = {
    success: function(o) {
      //YAHOO.log('getFolderListing Return: ' + timeDiff.getDiff() + 'ms');
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        Dom.get('activefolder_container').innerHTML = oResults.activefolder;
        if (show_upload && !Event.getListeners('newfilelink') && Dom.get('newfilelink')) {
          var oLinkNewFileButton = new YAHOO.widget.Button("newfilelink");
          Event.addListener("newfilelink", "click", showAddFilePanel);
        }
        document.frmtoolbar.cid.value = oResults.cid;
        Dom.get('filelistingheader').innerHTML = oResults.header;
        Dom.setStyle('filelistingheader','display','');
        //YAHOO.log('getFolderlisiting: initiate rendering filelisting:' + timeDiff.getDiff() + 'ms');
        renderFileListing(oResults);
        Dom.setStyle('expandcollapsefolders','display','');
        //YAHOO.log('getFolderlisiting: initiate rendering leftside navigation:' + timeDiff.getDiff() + 'ms');
        YAHOO.filedepot.showLeftNavigation();
        //YAHOO.log('getFolderlisiting Updated page completed in: ' + timeDiff.getDiff() + 'ms');
        updateAjaxStatus(NEXLANG_refreshmsg + timeDiff.getDiff() + 'ms');
        timer = setTimeout("Dom.setStyle('filedepot_ajaxStatus','visibility','hidden')", 3000);
        if (blockui)  {
          setTimeout('jQuery.unblockUI()',200);
          blockui = false;
        }
        try {
          if (oResults.lastrenderedfiles) {
            //YAHOO.log('makeAJAXGetFolderListing: initiate getmorefiledata:' + timeDiff.getDiff() + 'ms');
            YAHOO.filedepot.getmorefiledata(oResults.lastrenderedfiles);
            //YAHOO.log('makeAJAXGetFolderListing: completed getmorefiledata:' + timeDiff.getDiff() + 'ms');
          } else {
            YAHOO.filedepot.alternateRows.init('listing_record');
          }
        } catch(e) {}

      } else {
        alert(NEXLANG_errormsg4);
      }

    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback,postdata);
}


function makeAJAXSetFolderOrder(cid,direction) {
  var listingcid = document.frmtoolbar.cid.value;
  var ltoken = document.getElementById("flistingltoken").value;

  var surl = ajax_post_handler_url + '/setfolderorder';
  var postdata = '&direction=' + direction + '&cid=' + cid + '&listingcid=' + listingcid + '&ltoken=' + ltoken;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        renderFileListing(oResults);
        YAHOO.filedepot.alternateRows.init('listing_record');
      } else {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
}


function makeAJAXSearchTags(searchtags,removetag) {

  timeDiff.setStartTime();    // Reset Timer
  document.fsearch.query.value = '';
  if (searchtags == 'removetag') {
    var surl = ajax_post_handler_url + '/searchtags';
    var postdata = '&tags=' + document.fsearch.tags.value + '&removetag=' + removetag;
  } else {
    var surl = ajax_post_handler_url + '/searchtags';
    var postdata = '&tags=' + searchtags;
  }
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        Dom.get('activefolder_container').innerHTML = oResults.activefolder;
        Dom.get('filelistingheader').innerHTML = oResults.header;
        renderFileListing(oResults);
        YAHOO.filedepot.decorateFileListing();
        if (oResults.searchtags) {
          Dom.setStyle('showactivetags','display','block');
          Dom.get('activesearchtags').innerHTML = oResults.currentsearchtags;
          Dom.get('tagcloud_words').innerHTML = oResults.tagcloud;
          document.fsearch.tags.value=oResults.searchtags;
        } else {
          Dom.setStyle('showactivetags','display','none');
          document.fsearch.tags.value='';
          if (oResults.tagcloud) {
            Dom.get('tagcloud_words').innerHTML = oResults.tagcloud;
          }
        }
        updateAjaxStatus(NEXLANG_refreshmsg + timeDiff.getDiff() + 'ms');

      } else {
        alert(NEXLANG_errormsg5);
        updateAjaxStatus();
      }
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback,postdata);
}



function makeAJAXDeleteIncomingFile(id) {
  var surl = ajax_post_handler_url + '/deleteincomingfile'
  var token = document.getElementById("flistingltoken").value;
  var postdata = '&id=' + id + '&token='+token;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        renderLeftNavigation(oResults);
        Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
        YAHOO.filedepot.alternateRows.init('listing_record');
        clearCheckedItems();
      } else {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
}

function showMoveIncomingFile(id) {

  var surl = ajax_post_handler_url + '/rendermoveincoming';
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      Dom.get('movebatchfiledialog_form').innerHTML = oResults.displayhtml;
      YAHOO.container.moveIncomingFileDialog.cfg.setProperty("visible",true);
      document.frmIncomingFileMove.id.value=id;
      if (!Event.getListeners('btnMoveIncomingFileSubmit')) {   // Check first to see if listener already active
        Event.addListener("btnMoveIncomingFileSubmit", "click", moveIncomingFile);
      }
      if (!Event.getListeners('btnMoveIncomingFileCancel')) {   // Check first to see if listener already active
        Event.addListener("btnMoveIncomingFileCancel", "click",YAHOO.container.moveIncomingFileDialog.hide, YAHOO.container.moveIncomingFileDialog, true);
      }
    },
    failure: function(o) {
      YAHOO.log('AJAX error loading move folder options : ' + o.status);
    },
    argument: {},
    timeout:55000
  }
  YAHOO.util.Connect.asyncRequest('POST', surl, callback);

}



function moveIncomingFile() {
  YAHOO.container.moveIncomingFileDialog.hide();

  var surl = ajax_post_handler_url + '/moveincomingfile';
  var formObject = document.getElementById('frmIncomingFileMove');

  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        renderLeftNavigation(oResults);
        Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
        YAHOO.filedepot.alternateRows.init('listing_record');
        clearCheckedItems();
      } else {
        alert(oResults.errmsg);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }

  YAHOO.util.Connect.setForm(formObject, false);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback);
}


function makeAJAXSearch(form) {
  if (document.fsearch.query.value == '' || document.fsearch.query.value == searchprompt) {
    alert(NEXLANG_errormsg10);
  } else {
    clearAjaxActivity();
    if (!blockui)  {
      blockui=true;
      jQuery.blockUI();
    }
    timeDiff.setStartTime();    // Reset Timer
    Dom.setStyle('showactivetags','display','none');
    YAHOO.container.tagspanel.hide();
    var surl = ajax_post_handler_url + '/search';
    var postdata = '&query=' + document.fsearch.query.value;
    var callback = {
      success: function(o) {
        var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
        var oResults = eval('(' + json + ')');
        if (blockui)  {
          setTimeout('jQuery.unblockUI()',200);
          blockui = false;
        }
        if (oResults.retcode == 200) {
          Dom.get('activefolder_container').innerHTML = oResults.activefolder;
          Dom.get('filelistingheader').innerHTML = oResults.header;
          renderFileListing(oResults);
          updateAjaxStatus(NEXLANG_refreshmsg + timeDiff.getDiff() + 'ms');

        } else {
          alert(NEXLANG_errormsg5);
          updateAjaxStatus();
        }

      },
      failure: function(o) {
        YAHOO.log(NEXLANG_ajaxerror + o.status);
      },
      argument: {},
      timeout:55000
    }
    updateAjaxStatus(NEXLANG_activity);
    YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
  }
}

function makeAJAXBroadcastNotification () {
  timeDiff.setStartTime();    // Reset Timer
  var surl = ajax_post_handler_url + '/broadcastalert';
  var ftoken = document.getElementById("frmFileDetails_ftoken").value;
  var postdata = '&ftoken=' + ftoken;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        updateAjaxStatus();
        alert(Drupal.t('Broadcast message sent to !count users', {'!count': oResults.count}));
        updateAjaxStatus(NEXLANG_broadcastmsg + timeDiff.getDiff() + 'ms');
        YAHOO.container.broadcastDialog.hide();
        timer = setTimeout("Dom.setStyle('filedepot_ajaxStatus','visibility','hidden')", 3000);
      } else if (oResults.retcode == 204) {
        showAlert(NEXLANG_errormsg8);
        YAHOO.container.broadcastDialog.hide();
      } else if (oResults.retcode == 205) {
        showAlert(NEXLANG_errormsg9);
        YAHOO.container.broadcastDialog.hide();
      } else {
        alert(NEXLANG_errormsg1);
        updateAjaxStatus();
        YAHOO.container.broadcastDialog.hide();
      }
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activity);
  var formObject = document.frmBroadcast;
  YAHOO.util.Connect.setForm(formObject);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);

}




function onCategorySelect(elm) {
  if (document.frmNewFile.op.value == 'savefile') {
    if ( fileID != null && elm.options[elm.selectedIndex].value > 0) {
      document.getElementById('btnNewFileSubmit').disabled=false;
    } else {
      document.getElementById('btnNewFileSubmit').disabled=true;
    }
  }
}


function doAJAXEditVersionNote(fobj) {

  var ftoken = document.getElementById("frmFileDetails_ftoken").value;
  var surl = ajax_post_handler_url + '/updatenote';
  var postdata = '&reportmode=' + document.frmtoolbar.reportmode.value + '&ftoken=' + ftoken;
  var callback = {
    success:  function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        if (oResults.fid > 0) {
          Dom.get('displayfiledetails').innerHTML = oResults.displayhtml;
        }
      } else {
        alert(NEXLANG_errormsg6 + oResults.retcode);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  };
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.setForm(fobj, false);
  YAHOO.util.Connect.asyncRequest('POST', surl , callback, postdata);
};

function doAJAXDeleteVersion(fid,version) {

  var ftoken = document.getElementById("frmFileDetails_ftoken").value;
  var surl = ajax_post_handler_url + '/deleteversion';
  var postdata = '&fid=' + fid + '&version=' + version + '&reportmode=' + document.frmtoolbar.reportmode.value + '&ftoken=' + ftoken;
  var callback = {
    success:  function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        if (oResults.fid > 0) {
          Dom.get('displayfiledetails').innerHTML = oResults.displayhtml;
        }
      } else {
        alert(NEXLANG_errormsg6 + oResults.retcode);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  };
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl , callback, postdata);
};


function doAJAXDeleteNotification(type,id) {

  var surl = ajax_post_handler_url + '/deletenotification';
  var postdata = '&id=' + id + '&type=' + type;
  var callback = {
    success:  function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
        var myTabs = new YAHOO.widget.TabView('notification_report');
        if (type == 'file') {
          myTabs.set('activeIndex',0);
        } else {
          myTabs.set('activeIndex',1);
        }
        Dom.setStyle('filelistingheader', 'display', 'none');
        Dom.setStyle('reportlisting_container', 'display', '');
        YAHOO.filedepot.alternateRows.init('listing_record');
        // Setup the Notifications Settings Dialog
        Dom.setStyle('notificationsettingsdialog', 'display', 'block');
      } else {
        alert(NEXLANG_errormsg6 + oResults.retcode);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  };
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl , callback,postdata);
};

function doAJAXUpdateNotificationSettings(formObject) {

  var surl = ajax_post_handler_url + '/updatenotificationsettings';
  var callback = {
    success:  function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
        var myTabs = new YAHOO.widget.TabView('notification_report');
        myTabs.set('activeIndex',3);
        Dom.setStyle('filelistingheader', 'display', 'none');
        Dom.setStyle('reportlisting_container', 'display', '');
        YAHOO.filedepot.alternateRows.init('listing_record');
        // Setup the Notifications Settings Dialog
        Dom.setStyle('notificationsettingsdialog', 'display', 'block');
      } else {
        alert(NEXLANG_errormsg6 + oResults.retcode);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:120000  // 2 min
  };
  updateAjaxStatus(NEXLANG_activitymsg10);
  YAHOO.util.Connect.setForm(formObject);
  YAHOO.util.Connect.asyncRequest('POST', surl , callback);
};

function doAJAXUpdateFolderNotificationSettings(formObject) {
  var surl = ajax_post_handler_url + '/updatefoldersettings';
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        togglefolderoptions();
      } else {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activitymsg10);
  YAHOO.util.Connect.setForm(formObject, false);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback);
};

function doAJAXClearNotificationLog() {
  var surl = ajax_post_handler_url + '/clearnotificationlog';
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        var myTabs = new YAHOO.widget.TabView('notification_report');
        myTabs.set('activeIndex',2);
        Dom.setStyle('notificationlog_report','display','none');
        Dom.setStyle('notificationlog_norecords','display','');
      } else {
        alert(NEXLANG_errormsg1);
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activitymsg11);
  YAHOO.util.Connect.asyncRequest('GET', surl, callback);
};


// Tests if any active files have been selected - if not disable the "More Actions' select element
function enable_multiaction(selected_files,selected_folders) {
  if (selected_files == '' && selected_folders == '') {
    clearCheckedItems();
  } else {
    Dom.get('multiaction').disabled = false;
    Dom.replaceClass('multiaction','disabled_element','enabled_element');
  }
}

function clearCheckedItems() {

  // There will be no chkfile element if there are no listing results
  try {
    if (document.frmfilelisting.chkfile) {
      if (document.frmfilelisting.chkfile.length) {
        for (i=0; i<document.frmfilelisting.chkfile.length; i++) {
          document.frmfilelisting.chkfile[i].checked = false;
        }

        // Check or un-check the folder checkboxes
        if (document.frmfilelisting.chkfolder) {
          for (i=0; i<document.frmfilelisting.chkfolder.length; i++) {
            document.frmfilelisting.chkfolder[i].checked = false;
          }
        }

      } else if (document.frmfilelisting.chkfile.value > 0) {
        try {
          document.frmfilelisting.chkfolder.checked = false;
        } catch (e) {}
        document.frmfilelisting.chkfile.checked = false;
      }
    }
  } catch (e) {}
  Dom.get('multiaction').disabled = true;
  Dom.replaceClass('multiaction','enabled_element','disabled_element');
  document.frmtoolbar.checkeditems.value='';
  document.frmtoolbar.checkedfolders.value='';
  Dom.get('headerchkall').checked = false;

}


function updateCheckedItems(obj,type) {
  if (type == 'folder') {
    var field = document.frmtoolbar.checkedfolders;
  } else {
    var field = document.frmtoolbar.checkeditems;
  }
  if (obj.checked) {
    field.value += ',' + obj.value;
    NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(obj.value, true, 0));
  } else {
    if (field.value == field.value.replace(obj.value + ',', '')) {
      field.value = field.value.replace(obj.value, '');
    } else {
      field.value = field.value.replace(obj.value + ',', '');
    }

    NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(obj.value, false, 0));
  }
  // Remove any leading comma
  field.value = field.value.replace(/^,*/g, '');
  enable_multiaction(field.value,document.frmtoolbar.checkedfolders.value);
}

/**
 * Resets the checked items fields
 */
function resetCheckedItems() {
  document.frmtoolbar.checkeditems.value = "";
  document.frmtoolbar.checkedfolders.value = "";
  NxFiledepot.checkedItemsManager.clearAll();
}

function getPidFromCheckedItemClassName(obj) {
  var pidfid = obj.className;
  var pid = pidfid.split(':')[0];
  return pid;
};


function toggleCheckedItems(obj,files) {

  var selectedFilesField = document.frmtoolbar.checkeditems;
  var selectedFoldersField = document.frmtoolbar.checkedfolders;
  if (obj.value == 'all') {
    // Need to update the hidden fields in the header toolbar form - the 'More Actions' dropdown form
    selectedFilesField.value = '';
    selectedFoldersField.value = '';
    // Need to test if only 1 checkbox exists on page or multiple
    NxFiledepot.checkedItemsManager.clearAll();

    try {
      if (document.frmfilelisting.chkfile.length) {
        for (i=0; i<document.frmfilelisting.chkfile.length; i++) {
          document.frmfilelisting.chkfile[i].checked = obj.checked ? true : false;
          var itemvalue = document.frmfilelisting.chkfile[i].value;
          if (obj.checked) {
            selectedFilesField.value = selectedFilesField.value + itemvalue + ',';
            NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, true, 0));
          }
          else {
            NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, false, 0));
          }
        }

        // Check or un-check the folder checkboxes
        if (document.frmfilelisting.chkfolder) {
          for (i=0; i<document.frmfilelisting.chkfolder.length; i++) {
            document.frmfilelisting.chkfolder[i].checked = obj.checked ? true : false;
            var itemvalue = document.frmfilelisting.chkfolder[i].value;
            if (obj.checked) {
              selectedFoldersField.value = selectedFoldersField.value + itemvalue + ',';
               NxFiledepot.checkedItemsManager.setFolder(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, true, 0));
            }
            else {
               NxFiledepot.checkedItemsManager.setFolder(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, false, 0));
            }
          }
        }

      } else if (document.frmfilelisting.chkfile.value > 0) {
        if (obj.checked)
          field.value = document.frmfilelisting.chkfile.value;
        try {
          document.frmfilelisting.chkfolder.checked = obj.checked ? true : false;
          NxFiledepot.checkedItemsManager.setFolder(NxFiledepot.checkedItemsManager.createCheckedItemObject(document.frmfilelisting.chkfolder.value, obj.checked ? true : false, 0));
        } catch (e) {}
        document.frmfilelisting.chkfile.checked = obj.checked ? true : false;
        NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(document.frmfilelisting.chkfile.value, obj.checked ? true : false, 0));
      }

    } catch(e) {

      try {
        if (document.frmfilelisting.chkfile.value > 0) {
          document.frmfilelisting.chkfile.checked = obj.checked ? true : false;
          var itemvalue = document.frmfilelisting.chkfile.value;
          if (obj.checked) {
            NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, true, 0));
            selectedFilesField.value = itemvalue;
          }
          else {
            NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, false, 0));
          }
        }
        // Check or un-check the folder checkboxes
        if (document.frmfilelisting.chkfolder.length) {
          for (i=0; i<document.frmfilelisting.chkfolder.length; i++) {
            document.frmfilelisting.chkfolder[i].checked = obj.checked ? true : false;
            if (obj.checked) {
              var itemvalue = document.frmfilelisting.chkfolder[i].value;
              selectedFoldersField.value = selectedFoldersField.value + itemvalue + ',';
              NxFiledepot.checkedItemsManager.setFolder(NxFiledepot.checkedItemsManager.createCheckedItemObject(document.frmfilelisting.chkfolder[i].value, obj.checked ? true : false, 0));
            }
          }
        } else {
          try {
            document.frmfilelisting.chkfolder.checked = obj.checked ? true : false;
            if (obj.checked)
              selectedFoldersField.value = document.frmfilelisting.chkfolder.value;
            NxFiledepot.checkedItemsManager.setFolder(NxFiledepot.checkedItemsManager.createCheckedItemObject(document.frmfilelisting.chkfolder.value, obj.checked ? true : false, 0));
          } catch(e) {}
        }
      } catch (e) {}

    }

    // Remove the trailing comma
    selectedFilesField.value = selectedFilesField.value.replace(/,$/g, '');
    selectedFoldersField.value = selectedFoldersField.value.replace(/,$/g, '');

  } else if (obj.value > 0) {
    try {
      // Find any checkboxes (subfolders or files) under this subfolder and toggle the checkboxes
      jQuery("#subfolder"+obj.value+"_contents :input[type=checkbox]").each(function() {
        jQuery(this).attr('checked',!jQuery(this).attr('checked'));
      });
    }
    catch(e) {}

    // Need to update the hidden field in the header form - the 'More Actions' dropdown form
    selectedFilesField.value = '';
    selectedFoldersField.value = '';
    NxFiledepot.checkedItemsManager.clearAll();

    // Need to test if only 1 checkbox exists on page or multiple
    try {
      if (document.frmfilelisting.chkfile.length) {
        for (i=0; i<document.frmfilelisting.chkfile.length; i++) {
          var itemvalue = document.frmfilelisting.chkfile[i].value;
          if (document.frmfilelisting.chkfile[i].checked) {
            NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, true, 0));
            selectedFilesField.value = selectedFilesField.value + itemvalue + ',';
          }
          else {
            NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, false, 0));
          }
        }
      } else if (document.frmfilelisting.chkfile.value > 0) {
        if (document.frmfilelisting.chkfile.checked) {
          selectedFilesField.value = document.frmfilelisting.chkfile.value;
          NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(document.frmfilelisting.chkfile.value, true, 0));
        }
        else {
          NxFiledepot.checkedItemsManager.setFile(NxFiledepot.checkedItemsManager.createCheckedItemObject(document.frmfilelisting.chkfile.value, false, 0));
        }
      }
    } catch (e) {}

    // Check or un-check the folder checkboxes
    if (document.frmfilelisting.chkfolder) {
      var chkobj;
      for (i=0; i<document.frmfilelisting.chkfolder.length; i++) {
        chkobj = document.frmfilelisting.chkfolder[i];
        var itemvalue = chkobj.value;
        if (chkobj.checked) {
          selectedFoldersField.value = selectedFoldersField.value + itemvalue + ',';
          NxFiledepot.checkedItemsManager.setFolder(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, true, 0));
        }
        else {
          NxFiledepot.checkedItemsManager.setFolder(NxFiledepot.checkedItemsManager.createCheckedItemObject(itemvalue, false, 0));
        }
      }
    }

    // Remove the trailing comma and leading comma
    selectedFilesField.value = selectedFilesField.value.replace(/,$/g, '');
    selectedFilesField.value = selectedFilesField.value.replace(/^,*/g, '');

    selectedFoldersField.value = selectedFoldersField.value.replace(/,$/g, '');
    selectedFoldersField.value = selectedFoldersField.value.replace(/^,*/g, '');

  }
  enable_multiaction(selectedFilesField.value,selectedFoldersField.value);

}


function toggleCheckedNotificationItems(obj) {

  var selectedFilesField = document.frmtoolbar.checkeditems;
  var selectedFoldersField = document.frmtoolbar.checkedfolders;
  // Need to update the hidden fields in the header toolbar form - the 'More Actions' dropdown form

  if (obj.id == 'chkallfiles') {
    // Need to test if only 1 checkbox exists on page or multiple
    selectedFilesField.value = '';
    try {
      if (document.frmfilelisting.chkfile.length) {
        for (i=0; i<document.frmfilelisting.chkfile.length; i++) {
          document.frmfilelisting.chkfile[i].checked = obj.checked ? true : false;
          if (obj.checked) {
            var itemvalue = document.frmfilelisting.chkfile[i].value;
            selectedFilesField.value = selectedFilesField.value + itemvalue + ',';
          }
        }

      } else if (document.frmfilelisting.chkfile.value > 0) {
        if (obj.checked)
          field.value = document.frmfilelisting.chkfile.value;
        try {
          document.frmfilelisting.chkfile.checked = obj.checked ? true : false;
        } catch (e) {}
        document.frmfilelisting.chkfile.checked = obj.checked ? true : false;
      }
    } catch(e) { }

    // Remove the trailing comma
    selectedFilesField.value = selectedFilesField.value.replace(/,$/g, '');

  } else if (obj.id == 'chkallfolders') {
    // Need to test if only 1 checkbox exists on page or multiple
    selectedFoldersField.value = '';
    try {
      if (document.frmfilelisting.chkfolder.length) {
        for (i=0; i<document.frmfilelisting.chkfolder.length; i++) {
          document.frmfilelisting.chkfolder[i].checked = obj.checked ? true : false;
          if (obj.checked) {
            var itemvalue = document.frmfilelisting.chkfolder[i].value;
            selectedFoldersField.value = selectedFoldersField.value + itemvalue + ',';
          }
        }

      } else if (document.frmfilelisting.chkfolder.value > 0) {
        if (obj.checked)
          field.value = document.frmfilelisting.chkfolder.value;
        try {
          document.frmfilelisting.chkfolder.checked = obj.checked ? true : false;
        } catch (e) {}
        document.frmfilelisting.chkfolder.checked = obj.checked ? true : false;
      }
    } catch(e) { }

    // Remove the trailing comma
    selectedFoldersField.value = selectedFoldersField.value.replace(/,$/g, '');

  }

  enable_multiaction(selectedFilesField.value,selectedFoldersField.value);

}



// Two Functions used to initialize panels and forms for Add New File 'upload' and Add new Version 'edit'
function showAddFilePanel() {
  closeAlert();
  var activefolder = document.frmtoolbar.cid.value;
  Dom.setStyle('newfiledialog_folderrow', 'display', '');
  Dom.setStyle('newfiledialog_filedesc', 'display', '');
  Dom.setStyle('newfiledialog_filename', 'display', '');
  Dom.get('newfile_displayname').value='';
  Dom.get('newfile_tags').value='';
  Dom.get('newfile_desc').value='';
  Dom.get('newfile_notes').value='';
  Dom.get('filedepot_notify').checked='';
  document.frmNewFile.fid.value = 0;

  // Use this ajax request to get the latest folder options
  var surl = ajax_post_handler_url + '/rendernewfilefolderoptions';
  var postdata = '&cid=' + activefolder;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      Dom.get('newfile_selcategory').innerHTML = oResults.displayhtml;
      Dom.get('newfiledialog_heading').innerHTML='Add a new file';
      YAHOO.container.newfiledialog.cfg.setProperty("visible",true);
      if (!Event.getListeners('btnNewFileCancel')) {   // Check first to see if listener already active
        Event.addListener("btnNewFileCancel", "click",hideNewFilePanel, YAHOO.container.newfiledialog, true);
      }
    },
    failure: function(o) {
      YAHOO.log('AJAX error loading add file form : ' + o.status);
    },
    argument: {},
    timeout:55000
  }
  YAHOO.util.Connect.asyncRequest('POST', surl, callback,postdata);

}

function showAddNewVersion() {
  Dom.get('newfiledialog_heading').innerHTML = NEXLANG_newversion;
  Dom.get('filedepot-newversion-form').filedepot_fid.value = document.frmFileDetails.id.value;
  YAHOO.container.newfiledialog.cfg.setProperty("visible",true);
}

function showAddCategoryPanel() {
  var activefolder = document.frmtoolbar.cid.value;
  var surl = ajax_post_handler_url + '/rendernewfolderform';
  var postdata = '&cid=' + activefolder;
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      Dom.get('newfolderdialog_form').innerHTML = oResults.displayhtml;
      YAHOO.container.newfolderdialog.cfg.setProperty("visible",true);
      if (!Event.getListeners('btnNewFolderSubmit')) {   // Check first to see if listener already active
        Event.addListener("btnNewFolderSubmit", "click", makeAJAXCreateFolder, YAHOO.container.newfolderdialog, true);
      }
      if (!Event.getListeners('btnNewFolderCancel')) {   // Check first to see if listener already active
        Event.addListener("btnNewFolderCancel", "click",YAHOO.container.newfolderdialog.hide, YAHOO.container.newfolderdialog, true);
      }
    },
    failure: function(o) {
      YAHOO.log('AJAX error loading new folder form : ' + o.status);
    },
    argument: {},
    timeout:55000
  }
  YAHOO.util.Connect.asyncRequest('POST', surl, callback,postdata);
};


/**
 * Turn JSON data of reports and folders into YUI TreeView
 *
 * @param oResults string JSON
 */
function renderLeftNavigation(oResults) {

  try {
    if (!Event.getListeners('folderoptions_link')) {   // Check first to see if listener already active
      Event.addListener("folderoptions_link","click",togglefolderoptions);
    }
  } catch (e) {}

  try {
    YAHOO.container.newfolderdialog.hide();
  } catch (e) {}

  // Destroy any current tree view. Is rebuilding nodes cheaper?
  var curTree = YAHOO.widget.TreeView.getTree('filedepotNavTreeDiv');
  if (curTree) {
    curTree.destroy();
    curTree = null;
  }

  var tree = new YAHOO.widget.TreeView("filedepotNavTreeDiv");
  var root = tree.getRoot();

  // Reports
  var reports = new YAHOO.widget.TextNode(NEXLANG_intelfolder1, root, true);
  reports.labelStyle = "icon-files";
  for (var i = 0; i < oResults.reports.length; i++) {
    new YAHOO.widget.TextNode({
      label: oResults.reports[i]['label'],
      labelStyle: oResults.reports[i]['icon'],
      report: oResults.reports[i]['report'],
      cid: 0
    }, reports, false);
  }

  // Recent Folders
  if (oResults.recentfolders != null) {
    var recentfolders = new YAHOO.widget.TextNode(NEXLANG_intelfolder2, root, true);
    recentfolders.labelStyle = "icon-allfolders";
    for (var i = 0; i < oResults.recentfolders.length; i++) {
      new YAHOO.widget.TextNode({
        label: oResults.recentfolders[i]['label'],
        labelStyle: oResults.recentfolders[i]['icon'],
        report: '',
        cid: oResults.recentfolders[i]['cid']
      }, recentfolders, false);
    }
  }

  // Top Folders
  var topfolders = new YAHOO.widget.TextNode(NEXLANG_intelfolder3, root, true);
  topfolders.labelStyle = "icon-allfolders";
  for (var i = 0; i < oResults.topfolders.length; i++) {
    new YAHOO.widget.TextNode({
      label: oResults.topfolders[i]['label'],
      labelStyle: oResults.topfolders[i]['icon'],
      report: '',
      cid: oResults.topfolders[i]['cid']
    }, topfolders, false);
  }

  tree.subscribe('labelClick', prepareAction);
  tree.subscribe('enterKeyPressed', prepareAction);

  function prepareAction(node) {
    if (node.data.report || node.data.cid) {
      var actions = document.frmtoolbar;
      actions.reportmode.value = node.data.report;
      actions.cid.value = node.data.cid;

      if (Dom.get('filedepot_alert').style.display != 'none') {
        closeAlert();
      }

      YAHOO.filedepot.showfiles();
    }
  }
  tree.render();

  // Force focus of tree items when tabbing
  var anchors = tree.getEl().getElementsByTagName("a");
  for (var anchorIndex = 0; anchorIndex < anchors.length; anchorIndex++) {
    var anchor = anchors[anchorIndex];
    Event.on(anchor, 'focus', function (ev) {
        var target = Event.getTarget(ev);
        var node = tree.getNodeByElement(target);
        node.focus();
      },
      tree,
      true
    );
  }

}

// Common function called by AJAX Return functions that need to update the filelisting container
function renderFileListing(oResults) {
  Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
  if (filedepotfolders == "expanded") {
    var linkobj = document.getElementById('expandcollapsefolderslink');
    if (linkobj) expandCollapseFolders(linkobj,'expand')
  }
  if (filedepotdetail == 'expanded')  showhideFileDetail('show');

  folderList = Dom.getElementsByClassName('folder_withhover');
  for(var i=0; i< folderList.length; i++) {
    Event.addListener(folderList[i], 'mouseover', showFolderMoveActions, folderList[i]);
    Event.addListener(folderList[i], 'mouseout', hideFolderMoveActions, folderList[i]);
  }
  clearCheckedItems();
  if(document.frmfilelisting.chkfile) {
    if(document.frmfilelisting.chkfile.length == undefined || document.frmfilelisting.chkfile.length == 1)     // No files in listing - disable the header checkall checkbox
      Dom.get('headerchkall').disabled = true;
  } else {
    Dom.get('headerchkall').disabled = true;
  }

}

YAHOO.filedepot.showLeftNavigation = function() {
  /* Generate Left Side Folder Navigation */
  var surl = ajax_post_handler_url + '/getleftnavigation';
  var navcallback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      renderLeftNavigation(oResults);
    },
    failure: function(o) {
      YAHOO.log('AJAX error loading leftside navigation : ' + o.status);
    },
    argument: {},
    timeout:55000
  }

  YAHOO.util.Connect.asyncRequest('POST', surl, navcallback);
};

YAHOO.filedepot.getmorefiledata = function(data) {
  clear_ajaxactivity = false;
  folderstack = [];
  lastfiledata = eval('(' + data + ')');    // PHP Array that we json_encode within the JSON formatted return data to preserve the array
  YAHOO.filedepot.threadedRequestManager(0);
};

YAHOO.filedepot.threadedRequestManager = function(i) {
  //YAHOO.log('YAHOO.filedepot.threadedRequestManager - i: ' + i);
  if (folderstack.length > numGetFileThreads) {
    timerArray[i] = setTimeout('YAHOO.filedepot.threadedRequestManager(' + i + ')',500);
  } else if (i == 0 || lastfiledata.length > 0) {
    requestdata = lastfiledata.shift();
    YAHOO.filedepot.getmorefiledataRequest(requestdata[0],requestdata[1],requestdata[2],requestdata[3]);
    folderstack.push(requestdata[0]);   // used to track the folders being prcessed.
    i++;
    YAHOO.filedepot.threadedRequestManager(i);
  }
}


YAHOO.filedepot.generateThreadedFileRequest = function(requestdata) {
  folderstack.push(requestdata[0]);   // used to track the folders being prcessed.
  timerArray[i] = setTimeout('YAHOO.filedepot.getmorefiledataRequest('
  + requestdata[0] + ','      //  Folder id   (cid)
  + requestdata[1] + ',"'     //  File id     (fid)
  + requestdata[2] + '",'     //  foldernunber
  + requestdata[3] + ')'      //  level - folder depth
  ,interval);
  //YAHOO.log('Schedule getmoredateRequest for cid: ' + requestdata[0] + ' in ' + interval + ' ms');
}

YAHOO.filedepot.getmorefiledataRequest = function(cid,fid,foldernumber,level) {
  if (!clear_ajaxactivity) {
    var surl = ajax_post_handler_url + '/getmorefiledata';
    var postdata = '&pending=' + folderstack.length;
    postdata += '&cid='+ cid;
    postdata += '&foldernumber=' + foldernumber;
    postdata += '&level=' + level;
    var callback = {
      success: function(o) {
        YAHOO.log('getmorefiledata(' + cid + '): return from AJAX call:' + timeDiff.getDiff() + 'ms');
        var root = o.responseXML.documentElement;
        var oResults = new Object();
        oResults.retcode = parseXML(root,'retcode');
        /* Clear the message area for this folder*/
        try {
          //YAHOO.log("cid:" + cid + " fid:" + fid);
          Dom.get('listingrec' + fid + '_bottom').innerHTML = '';
        } catch(e) {}
        if (oResults.retcode == 200) {
          oResults.displayhtml = parseXML(root,'displayhtml');
          try {
            Dom.get('subfolder' + cid + '_rec' + fid + '_bottom').innerHTML = oResults.displayhtml;
            //Dom.get('subfolder' + cid + '_rec_bottom').innerHTML = oResults.displayhtml;
          } catch(e) {
            try {
              Dom.get('subfolderlisting' + cid + '_bottom').innerHTML = oResults.displayhtml;
            } catch(e) {}
          }
          folderstack = arrayRemoveItem(folderstack,cid); // Remove this folder as it's now been processed
          //YAHOO.log('folderstack length: ' + folderstack.length);

        }
        if (lastfiledata.length == 0 && folderstack.length == 0) {
          clearCheckedItems();
          YAHOO.filedepot.alternateRows.init('listing_record');
          //alert('Completed loading Data in: ' + timeDiff.getDiff() + 'ms');
        }

      },
      failure: function(o) {
        YAHOO.log('AJAX error loading moredata : ' + o.status);
      },
      argument: {},
      timeout:55000
    }

    YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);

  }
}

/* Initiated from an event handler in initapplication.js */
YAHOO.filedepot.getmorefolderdataRequest = function(cid,fid,foldernumber,level,pass2) {
  if (!blockui)  {
    blockui=true;
    jQuery.blockUI();
  }
  timeDiff.setStartTime(); // Reset the timer
  //YAHOO.log('getmorefolderdata: start AJAX call:' + timeDiff.getDiff() + 'ms');
  var surl = ajax_post_handler_url + '/getmorefolderdata';
  var postdata = '&pending=' + folderstack.length;
  postdata += '&cid='+ cid;
  postdata += '&foldernumber=' + foldernumber;
  postdata += '&level=' + level;
  if (pass2 == 1) {
    postdata += '&pass2=1'
  }
  var callback = {
    success: function(o) {
      //YAHOO.log('getmorefiledata(' + cid + '): return from AJAX call:' + timeDiff.getDiff() + 'ms');
      var root = o.responseXML.documentElement;
      var oResults = new Object();
      oResults.retcode = parseXML(root,'retcode');
      if (oResults.retcode == 200) {
        oResults.displayhtml = parseXML(root,'displayhtml');
        try {
          Dom.get('subfolder' + cid + '_rec' + fid + '_bottom').innerHTML = oResults.displayhtml;
        } catch(e) {}
      }
      Dom.setStyle('filedepot_ajaxActivity','visibility','hidden');
      timer = setTimeout("Dom.setStyle('filedepot_ajaxStatus','visibility','hidden')", 3000);
      if (blockui)  {
        setTimeout('jQuery.unblockUI()',200);
        blockui = false;
      }
      YAHOO.filedepot.alternateRows.init('listing_record');
    },
    failure: function(o) {
      YAHOO.log('AJAX error loading more folder(' + cid + ') data : ' + o.status);
    },
    argument: {},
    timeout:55000
  }
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);

}


/* Make the AJAX call to generate an updated file listing and leftside navigation */
YAHOO.filedepot.showfiles = function(cid) {
  //YAHOO.log('showfiles: start AJAX call:' + timeDiff.getDiff() + 'ms');
  clearAjaxActivity();
  if (!blockui)  {
    blockui=true;
    jQuery.blockUI();
  }
  timeDiff.setStartTime(); // Reset the timer
  if (cid == undefined && document.frmtoolbar.cid.value > 0) {
    var cid = document.frmtoolbar.cid.value;
    reportmode = '';
  } else {
    var reportmode = document.frmtoolbar.reportmode.value;
  }
  var reportmode = document.frmtoolbar.reportmode.value;
  var surl = ajax_post_handler_url + '/getfilelisting';
  var postvars = '&cid='+cid + '&reportmode=' + reportmode;
  document.fsearch.query.value = searchprompt;
  YAHOO.container.tagspanel.hide();
  Dom.setStyle('showactivetags','display','none');
  var listingcallback = {
    success: function(o) {
      //YAHOO.log('showfiles: return from AJAX call:' + timeDiff.getDiff() + 'ms');
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {

        Dom.setStyle('activefolder_container','display','');
        Dom.setStyle('filelistingheader','display','');
        document.frmtoolbar.cid.value = oResults.cid;

        document.frmBroadcast.cid.value = oResults.cid;
        document.frmBroadcast.message.value = '';
        Dom.get('filelistingheader').innerHTML = oResults.header;

        //YAHOO.log('showfiles: start rendering filelisting:' + timeDiff.getDiff() + 'ms');

        if (reportmode == 'notifications') {
          Dom.get('filelisting_container').innerHTML = oResults.displayhtml;
          var myTabs = new YAHOO.widget.TabView('notification_report');
          Dom.setStyle('filelistingheader', 'display', 'none');
          Dom.setStyle('reportlisting_container', 'display', '');
          YAHOO.filedepot.alternateRows.init('listing_record');
          // Setup the Notifications Settings Dialog
          Dom.setStyle('notificationsettingsdialog', 'display', 'block');
          if (!Event.getListeners('clearnotificationhistory')) {   // Check first to see if listener already active
            Event.on('clearnotificationhistory', 'click', doAJAXClearNotificationLog);
          }
        } else {
          renderFileListing(oResults);
        }
        //YAHOO.log('showfiles: completed rendering filelisting:' + timeDiff.getDiff() + 'ms');

        Dom.get('activesearchtags').innerHTML = '';     // clear any active search tags
        document.fsearch.tags.value='';
        Dom.setStyle('showactivetags','display','none');
        Dom.get('headerchkall').checked = false;
        if (oResults.activefolder) {
          Dom.get('activefolder_container').innerHTML = oResults.activefolder;
          if (!Event.getListeners('newfilelink') && Dom.get('newfilelink')) {
            var oLinkNewFileButton = new YAHOO.widget.Button("newfilelink");
            Event.addListener("newfilelink", "click", showAddFilePanel);
          }
        } else {
          Dom.get('activefolder_container').innerHTML = '';
        }
        if (oResults.moreactions) {
          var objSelect = Dom.get('multiaction');
          select_innerHTML(objSelect,oResults.moreactions);
        }

        //YAHOO.log('showfiles: initiate rendering leftside navigation:' + timeDiff.getDiff() + 'ms');
        updateAjaxStatus(NEXLANG_refreshmsg + timeDiff.getDiff() + 'ms');
        //YAHOO.log('showfiles: completed function:' + timeDiff.getDiff() + 'ms');

      } else if (oResults.retcode == 401) {   // No permissions to view this category
        Dom.get('filedepot_alert_content').innerHTML = oResults.error;
        Dom.setStyle('filedepot_alert','display','');
      } else {
        alert(NEXLANG_errormsg6 + oResults.retcode);
      }

      Dom.setStyle('filedepot_ajaxActivity','visibility','hidden');
      timer = setTimeout("Dom.setStyle('filedepot_ajaxStatus','visibility','hidden')", 5000);
      if (blockui)  {
        jQuery.unblockUI();
        blockui = false;
        if (initialfid > 0) {
          setTimeout('makeAJAXLoadFileDetails(' + initialfid + ')',500);
          initialfid = 0;
        } else if (initialop == 'newprojectfile' && initialparm > 0) {
          setTimeout('showAddFilePanel()',500);
        }

      }

      // Expand any individual folders user had opened
      for(var i=0; i< expandedfolders.length; i++) {
        expandfolder(expandedfolders[i]);
      }

      try {
        if (oResults['lastrenderedfiles']) {
          //YAHOO.log('showfiles: initiate getmorefiledata:' + timeDiff.getDiff() + 'ms');
          YAHOO.filedepot.getmorefiledata(oResults['lastrenderedfiles']);
          //YAHOO.log('showfiles: completed getmorefiledata:' + timeDiff.getDiff() + 'ms');
        } else {
          YAHOO.filedepot.alternateRows.init('listing_record');
        }
      } catch(e) {YAHOO.filedepot.alternateRows.init('listing_record');}

    },
    failure: function(o) {
      YAHOO.log('AJAX error loading main file listing : ' + o.status);
    },
    argument: {},
    timeout:120000 // 2 min
  }
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl, listingcallback,postvars);
};


YAHOO.filedepot.refreshFileDetails = function(fid) {
  var reportmode = document.frmtoolbar.reportmode.value;
  var surl = ajax_post_handler_url + '/refreshfiledetails';
  var postdata = '&id='+fid+'&reportmode='+reportmode;
  var callback = {
    success:  function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200) {
        if (oResults.fid > 0) {
          Dom.get('displayfiledetails').innerHTML = oResults.displayhtml;
        }
      }
      updateAjaxStatus();
    },
    failure: function(o) {
      YAHOO.log(NEXLANG_ajaxerror + o.status);
    },
    argument: {},
    timeout:55000
  };
  updateAjaxStatus(NEXLANG_activity);
  YAHOO.util.Connect.asyncRequest('POST', surl , callback,postdata);
};


YAHOO.filedepot.alternateRows = {
  /**
  *    Our init function
  *    @params className String, the container class name
  *    Adds the class oddrow to odd rows and class evenrow to even rows
  */
  init: function(className){
    // get all the tables with that particular class name
    recordList = Dom.getElementsByClassName(className);
    var closedfolders = new Array();

    // loop through them
    var setColor;
    var rows = 0;
    for(var i=0; i< recordList.length; i++) {
      setColor = true;
      if (Dom.hasClass(recordList[i],'subfolder')) {   // This record is a folder so we need to see if it's open or closed
        var elparts = recordList[i].id.split('subfolder');  // Extract the folder id from the record id assigned
        var elc = 'subfolder' + elparts[1] + '_contents';
        // Check if this folder is closed and if so - then skip any records under this folder
        if (Dom.getStyle(elc, 'display') == 'none') {
          closedfolders.push(elparts[1]);
          for(var j=0; j < closedfolders.length; j++) {
            if (Dom.hasClass(recordList[i], 'parentfolder' + closedfolders[j]) ) {
              setColor = false;
              break;
            }
          }
        }

      } else if (closedfolders.length > 0) {
        // split out the folder id for this listing record
        var elcparts = recordList[i].id.split('folder_');
        var elparts = elcparts[1].split('_rec');
        for(var j=0; j < closedfolders.length; j++) {
          if (elparts[0] == closedfolders[j]) {
            setColor = false;
            break;
          }
        }
      }

      if (setColor) {
        if (rows % 2 == 0) {
          Dom.removeClass(recordList[i], 'oddrow');
          Dom.addClass(recordList[i], 'evenrow');
        } else {
          Dom.removeClass(recordList[i], 'evenrow');
          Dom.addClass(recordList[i], 'oddrow');
        }
        rows++;
      }

    }

  }
};


YAHOO.filedepot.refreshtagcloud = function() {
  var surl = ajax_post_handler_url + '/refreshtagcloud';
  var callback = {
    success: function(o) {
      var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
      var oResults = eval('(' + json + ')');
      if (oResults.retcode == 200 && oResults.tagcloud) {
        Dom.get('tagcloud_words').innerHTML = oResults.tagcloud;
      }
    },
    failure: function(o) {
      YAHOO.log('AJAX error refreshing tagcloud : ' + o.status);
    },
    argument: {},
    timeout:55000
  }
  YAHOO.util.Connect.asyncRequest('POST', surl, callback);
}

YAHOO.filedepot.decorateFileListing = function() {
  YAHOO.log('YAHOO.filedepot.decorateFileListing');
  YAHOO.filedepot.alternateRows.init('listing_record');
  /*
  folderList = Dom.getElementsByClassName('folder_withhover');
  for(var i=0; i< folderList.length; i++) {
  Event.addListener(folderList[i], 'mouseover', showFolderMoveActions, folderList[i]);
  Event.addListener(folderList[i], 'mouseout', hideFolderMoveActions, folderList[i]);
  }
  */
  clearCheckedItems();
  updateAjaxStatus(NEXLANG_readymsg);
  timer = setTimeout("updateAjaxStatus()", 10000);
  Dom.setStyle('filedepot_ajaxActivity','visibility','hidden');
  timer = setTimeout("Dom.setStyle('filedepot_ajaxStatus','visibility','hidden')", 5000);

  if (blockui)  {
    setTimeout('jQuery.unblockUI()',200);
    blockui = false;
  }

}

