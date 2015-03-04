/*
 * @file initapplication.js
 * Main Initialization Javascript for the drupal filedepot module
 */

var init_filedepot = function() {

  YAHOO.widget.Logger.enableBrowserConsole()
  // Since we are using AJAX to update display, the cookie is not updated as the page is not refreshing.
  // We can use this variable anyways for the session since it will not be reset until a page refresh
  window.filedepotdetail = YAHOO.util.Cookie.get("filedepotdetail");
  window.filedepotfolders = YAHOO.util.Cookie.get("filedepotfolders");


  var el = document.getElementById('filedepot');
  if (pagewidth == 0) {
    layoutPageWidth = Dom.get('filedepotmodule').offsetWidth;   //Width of the outer element
  } else {
    layoutPageWidth = pagewidth;
  }

  layoutPageHeight = Dom.get('filedepotmodule').offsetHeight - 0;   //Width of the outer element
  var leftcolwidth = Dom.get('filedepot_sidecol').offsetWidth + 42;

  //YAHOO.log('width: ' + layoutPageWidth);
  var layout = new YAHOO.widget.Layout(el,{
    height: layoutPageHeight, //Height of the viewport
    width: layoutPageWidth, //Width of the outer element
    minHeight: 150, //So it doesn't get too small
    units: [
    { position: 'left',  width: leftcolwidth, resize: true, body: 'filedepot_sidecol', scroll: true, animate: true, gutter: '0px 25px 15px 0px' },
    { position: 'center', body: 'filedepot_centercol', gutter: '0px 6px 15px 0px', scroll: true, resize: true }
    ]
  });

  if (pagewidth == 0)
    layout.on('beforeResize', function() {
      if (pagewidth == 0) {
        layoutPageWidth = Dom.get('filedepotmodule').offsetWidth;   //Width of the outer element
      }
      Dom.setStyle('filedepot', 'width', layoutPageWidth + 'px');
  });
  layout.render();
  Dom.setStyle('filedepottoolbar','display','block');
  updateAjaxStatus('Loading Workspace ...')

  if (pagewidth == 0)
    //Handle the resizing of the window
    Event.on(window, 'resize', layout.resize, layout, true);

  // Need to add this class to the body tag for the YUI skin
  var objects = document.getElementsByTagName("body");
  var bodytag = objects[0];
  bodytag.className = bodytag.className + ' yui-skin-sam';

  try {
    Dom.setStyle('header-welcomedate', 'display', 'none');
  } catch(e) {}

  var oSearchButton = new YAHOO.widget.Button("searchbutton");

  // File Tags Panel
  YAHOO.container.tagspanel = new YAHOO.widget.Panel("tagspanel",
  { width:"320px",
    visible:false,
    zindex:2000,
    constraintoviewport:true
  });
  YAHOO.container.tagspanel.render();

  // File Details Dialogs
  Dom.setStyle('filedetails', 'display', 'block');
  YAHOO.container.filedetails = new YAHOO.widget.Panel("filedetails",
  { width : "670px",
    fixedcenter : true,
    visible : false,
    zindex: 2000,
    modal: useModalDialogs,
    effect:{effect:YAHOO.widget.ContainerEffect.FADE, duration: .5},
    constraintoviewport : true
  } );
  YAHOO.container.filedetails.render("filedepotmodule");

  // Add and event on the close icon to hide the menu
  var closeEl = Dom.getElementsByClassName("container-close", null, 'filedetails')[0];
  Event.on(closeEl, 'click', function(e){ YAHOO.container.menuBar.cfg.setProperty("visible",false); });

  // Folder Perms Panel
  Dom.setStyle('folderperms', 'display', 'block');
  YAHOO.container.folderperms = new YAHOO.widget.Panel("folderperms",
  { width : "750px",
    fixedcenter : true,
    visible : false,
    zindex: 2000,
    modal: useModalDialogs,
    constraintoviewport : true
  } );
  YAHOO.container.folderperms.render();


  // Setup the New Folder Dialog
  if (show_newfolder) Dom.setStyle('newfolderdialog', 'display', 'block');
  YAHOO.container.newfolderdialog = new YAHOO.widget.Panel("newfolderdialog",
  { width : "450px",
    visible : false,
    fixedcenter : true,
    modal: useModalDialogs,
    constraintoviewport : true
  } );

  YAHOO.container.newfolderdialog.render();

  // Setup the New File Dialog
  Dom.setStyle('newfiledialog', 'display', 'block');
  YAHOO.container.newfiledialog = new YAHOO.widget.Panel("newfiledialog",
  { width : "520px",
    visible : false,
    fixedcenter : true,
    modal: useModalDialogs,
    constraintoviewport : true
  } );

  YAHOO.container.newfiledialog.render();

  // Setup the Batch File Move Dialog
  Dom.setStyle('movebatchfilesdialog', 'display', 'block');
  YAHOO.container.batchfilemovedialog = new YAHOO.widget.Panel("movebatchfilesdialog",
  { width : "450px",
    visible : false,
    fixedcenter : true,
    modal: useModalDialogs,
    constraintoviewport : true
  } );

  YAHOO.container.batchfilemovedialog.render();

  // Setup the Incoming File Move Dialog
  Dom.setStyle('moveIncomingFileDialog', 'display', 'block');
  YAHOO.container.moveIncomingFileDialog = new YAHOO.widget.Panel("moveIncomingFileDialog",
  { width : "450px",
    visible : false,
    fixedcenter : true,
    modal: useModalDialogs,
    constraintoviewport : true
  } );

  YAHOO.container.moveIncomingFileDialog.render();

  // Setup the Broadcast Notification Dialog
  Dom.setStyle('broadcastDialog', 'display', 'block');
  YAHOO.container.broadcastDialog = new YAHOO.widget.Panel("broadcastDialog",
  { width : "450px",
    visible : false,
    fixedcenter : true,
    modal: useModalDialogs,
    constraintoviewport : true
  } );

  YAHOO.container.broadcastDialog.render();

  try {
    if (filedepotdetail == "expanded") {
      var elements = Dom.getElementsByClassName('filedesc','div', 'filelisting_container');
      for (var i = 0; i < elements.length; i++) {
        Dom.setStyle(elements[i],'display','block');
        var elm = Dom.getFirstChild('showhidedetail');
        elm.innerHTML = NEXLANG_click2hidedetail;
      }
      var elements = Dom.getElementsByClassName('emptyfolder','div', 'filelisting_container');
      for (var i = 0; i < elements.length; i++) {
        Dom.setStyle(elements[i],'display','none');
      }
    }
  } catch(e) {}


  try {
    if (show_newfolder) {
      Dom.setStyle('newfolderlink','display','');
      var oLinkButton1 = new YAHOO.widget.Button("newfolderlink");
      Event.addListener("newfolderlink", "click", showAddCategoryPanel, YAHOO.container.newfolderdialog, true);
    }
  } catch(e) {}

  Event.addListener("filedetails_cancel", "click", hideFileDetailsPanel);
  Event.addListener("showsearchtags", "click", function(e)
  {
    if (YAHOO.container.tagspanel.cfg.getProperty('visible')) {
      YAHOO.container.tagspanel.hide();
    } else {
      YAHOO.container.tagspanel.show();
    }
  });
  Event.addListener("searchbutton", "click", makeAJAXSearch);
  Event.addListener("cancelalert", "click", closeAlert);
  Event.addListener("btnBroadcastSubmit", "click", makeAJAXBroadcastNotification);
  Event.addListener("btnBroadcastCancel", "click", YAHOO.container.broadcastDialog.hide, YAHOO.container.broadcastDialog, true);

  // Setup event handlers for the flag favorite feature - hooked to the Star Image
  Event.addListener('filelisting_container', 'click', function(e){
    var target = Event.getTarget(e);
    if(target.tagName.toUpperCase() === 'IMG' && Dom.hasClass(target,'togglefavorite')) {
      Event.preventDefault(e);
      var link = target.parentNode;
      var params = link.getAttribute('href');
      var id = parseURL(params,'id');
      makeAJAXToggleFavorite(id);
    }
  });

  // Setup event handler on each delete permission link in the folder perms panel
  Event.on('folderperms', 'click', function(e){
    var target = Event.getTarget(e);
    var tagname = target.tagName.toUpperCase();
    if(tagname === 'A' && Dom.hasClass(target,'deleteperm')) {
      Event.preventDefault(e);
      var params = target.getAttribute('href');
      var id = parseURL(params,'accid'); // The data is passed as: "id:tokenvalue"
      var token = document.getElementById("folderpermstoken").value;
      var surl = ajax_post_handler_url + '/delfolderperms';
      var postdata = '&id=' + id + '&token=' + token;
      var callback = {
        success: function(o) {
          var json = o.responseText.substring(o.responseText.indexOf('{'), o.responseText.lastIndexOf('}') + 1);
          var oResults = eval('(' + json + ')');
          if (oResults.retcode == 200) {
            Dom.get('folderperms_content').innerHTML = oResults.html;
            YAHOO.container.folderperms.cfg.setProperty("visible",true);
            Event.addListener("folderperms_cancel", "click", YAHOO.container.folderperms.hide, YAHOO.container.folderperms, true);
          } else {
            alert(NEXLANG_errormsg7);
          }
          updateAjaxStatus();
        },
        failure: function(o) {
          alert(NEXLANG_ajaxerror + o.status);
        },
        argument: {},
        timeout:55000
      }
      updateAjaxStatus(NEXLANG_activitymsg4);
      YAHOO.util.Connect.asyncRequest('POST', surl, callback, postdata);
    }
  });

  // Setup to work with the file details link
  Event.on('filelisting_container', 'click', function(e){
    var target = Event.getTarget(e);
    var tagname = target.tagName.toUpperCase();
    if(tagname === 'A') {
      if (Dom.hasClass(target,'filedetailsdialog')) {
        Event.preventDefault(e);
        var params = target.getAttribute('href');
        var id = parseURL(params,'fid');
        makeAJAXLoadFileDetails(id);
      } else if (Dom.hasClass(target,'deleteincoming')) {
        Event.preventDefault(e);
        var params = target.getAttribute('href');
        var id = parseURL(params,'fid');
        makeAJAXDeleteIncomingFile(id);
      } else if (Dom.hasClass(target,'moveincoming')) {
        Event.preventDefault(e);
        var params = target.getAttribute('href');
        var id = parseURL(params,'fid');
        showMoveIncomingFile(id);
      } else if (Dom.hasClass(target,'morefolderdata')) {
        Event.preventDefault(e);
        var params = target.getAttribute('href');
        var cid   = parseURL(params,'cid');
        var fid   = parseURL(params,'fid');
        var fnum  = parseURL(params,'fnum');
        var level = parseURL(params,'level');
        YAHOO.filedepot.getmorefolderdataRequest(cid,fid,fnum,level);
      }

    } else if (tagname === 'IMG') {
      var link = target.parentNode;
      if (Dom.hasClass(link,'filedetailsdialog')) {
        Event.preventDefault(e);
        var params = link.getAttribute('href');
        var id = parseURL(params,'fid');
        makeAJAXLoadFileDetails(id);
      }
    }
  });

  // Setup event handler on each tag words in the tag cloud
  Event.on('tagspanel', 'click', function(e){
    var target = Event.getTarget(e);
    var tagname = target.tagName.toUpperCase();
    if(tagname === 'A' && Dom.hasClass(target,'tagcloudword')) {
      Event.preventDefault(e);
      var tagword = target.innerHTML;
      if (document.fsearch.tags.value.length > 0)
        tagword = tagword + ',' + document.fsearch.tags.value;
      makeAJAXSearchTags(tagword);
    }
  });

  /*  Instantiate a MenuBar:  */
  YAHOO.container.menuBar = new YAHOO.widget.MenuBar("filedetailsmenubar");
  /*  Call the "render" method with no arguments since the  markup for this MenuBar already exists in the page.   */
  YAHOO.container.menuBar.render();
  YAHOO.container.menuBar.cfg.setProperty("visible",false);

  Dom.setStyle('filedepot', 'visibility', 'visible');
  Dom.setStyle('tagspanel', 'display', 'block');

  if (initialcid == '') {
    document.frmtoolbar.reportmode.value = 'latestfiles';
  }
  YAHOO.filedepot.showLeftNavigation();
  YAHOO.filedepot.showfiles(initialcid);

  // Browser may have cached selected items and checked checkbox'es but verify and reset if need
  enable_multiaction(document.frmtoolbar.checkeditems.value);


};
