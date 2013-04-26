/**
 * @file
 * Javascript for Vaxia UI on RPG pages.
 *
 * Allows user to collapse the description as needed.
 */
Drupal.behaviors.vaxiaChat = {
  attach: function(context) { (function($) {
  
  function setCookie(c_name, value, exdays) {
    var exdate=new Date();
    exdate.setDate(exdate.getDate() + exdays);
    var c_value=escape(value) + ((exdays==null) ? "" : "; expires="+exdate.toUTCString());
    document.cookie=c_name + "=" + c_value;
  }
  
  function getCookie(c_name) {
    var i,x,y,ARRcookies=document.cookie.split(";");
    for (i=0;i<ARRcookies.length;i++) {
      x=ARRcookies[i].substr(0,ARRcookies[i].indexOf("="));
      y=ARRcookies[i].substr(ARRcookies[i].indexOf("=")+1);
      x=x.replace(/^\s+|\s+$/g,"");
      if (x==c_name){
        return unescape(y);
      }
    }
    return 0;
  }

  // Add toggle buttons.
  $('.tabs').once(function() {
    $('.tabs').after('<button class="toggle-rpg-chat-desc button" ' +
      'style="float:right;padding:0.5em 1em;margin:0.25em 4.4em;">' +
      'hide description' +
      '</button>');
    $('#main #content').after('<button class="toggle-rpg-chat-sidebar button" ' +
      'style="float:left;margin:0.25em -20% 0.25em 0;">' +
      'hide sidebar' +
      '</button>');
    $('#main .sidebars, #main #content').css('margin-top', '40px');

    // When Chat location toggle clicked, switch.
    $('.toggle-rpg-chat-desc').click(function() {
      if ( $('article.rpg-chat-node .field-name-body').is(':visible') ) {
        $('article.rpg-chat-node article.node-artwork .field-name-field-artwork').hide();
        $('article.rpg-chat-node .field-name-body').hide();
        $('article.rpg-chat-node .field-name-field-parent').hide();
        $('#block-views-rpg-chats-rpg-chats-children').hide();
        $('.toggle-rpg-chat-desc').html('show description');
        setCookie('toggle_description', 'hide', 365);
      } else {
        $('article.rpg-chat-node article.node-artwork .field-name-field-artwork').show();
        $('article.rpg-chat-node .field-name-body').show();
        $('article.rpg-chat-node .field-name-field-parent').show();
        $('#block-views-rpg-chats-rpg-chats-children').show();
        $('.toggle-rpg-chat-desc').html('hide description');
        setCookie('toggle_description', 'show', 365);
      }
    });

    // When Sidebar toggle clicked, switch.
    $('.toggle-rpg-chat-sidebar').click(function() {
      if ( $('#main .sidebars').is(':visible') ) {
        $('#main .sidebars').hide();
        $('#main #content').css('margin-left', '0%');
        $('#main #content').css('width', '100%');
        $('.toggle-rpg-chat-sidebar').html('show sidebar');
        setCookie('toggle_sidebar', 'hide', 365);
      } else {
        $('#main .sidebars').show();
        $('#main #content').css('margin-left', '20%');
        $('#main #content').css('width', '80%');
        $('.toggle-rpg-chat-sidebar').html('hide sidebar');
        setCookie('toggle_sidebar', 'show', 365);
      }
    });

    var toggle_description = getCookie('toggle_description');
    if (toggle_description == 'hide') {
      $('.toggle-rpg-chat-desc').click();
    }

    var toggle_sidebar = getCookie('toggle_sidebar');
    if (toggle_sidebar == 'hide') {
      $('.toggle-rpg-chat-sidebar').click();
    }
  });

  })(jQuery); }
}
