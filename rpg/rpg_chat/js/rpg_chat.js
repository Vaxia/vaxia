/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */
Drupal.behaviors.rpgChat = {
  attach: function(context) { (function($) {

  // Store the rpgChatTimer.
  var rpgChatTimer;

  // Function to inject buttons on page load and init.
  function rpgChatInit() {

    // Set up for the first refresh after load.
    rpgChatTimer = setTimeout(rpgChatRefresh, 60 * 1000);

    // On page load, inject the buttons into place in the DOM.
    $('.rpg-chat-node #edit-actions').prepend('<input type="button" id="edit-pause"' +
      ' class="toggle-rpg-chat-pause form-submit" value="Pause" paused="not-paused">');
    $('.rpg-chat-node #edit-0').before('<input type="button" id="edit-2"' +
      ' class="toggle-rpg-chat-pause form-submit" value="Pause" paused="not-paused">');
    $('.toggle-rpg-chat-pause').css('font-weight', '').css('color', 'graytext');

    // Set the ajax value on init.
    $('body').attr('ajax', 'not-ajax');

    // Make note of the title and messages for updates.
    $('body').attr('chat_title', document.title);
    $('body').attr('chat_mess', $('#rpg-chat [id^="comment-"]').filter(':first').attr('id'));

    // Add the listener for the pause click.
    $('.toggle-rpg-chat-pause').click(function() {
      rpgChatPause();
    });

    // Add listener for when we're in AJAX event.
    $(document).ajaxStart(function(){
      $('body').attr('ajax', 'ajax');
      // Grey out submission buttons when in ajax.
      $('form.node-form #edit-1, form.comment-form #edit-1, ' +
        'form.node-form #edit-submit, form.comment-form #edit-submit').css('color', 'graytext');
    })
    .ajaxStop(function() {
      $('body').attr('ajax', 'not-ajax');
      // Reenable the button and reset the color.
      $('form.node-form #edit-1, form.comment-form #edit-1, ' +
        'form.node-form #edit-submit, form.comment-form #edit-submit').css('color', 'black');
      // On each ajax reload (as triggered by rpgChatRefresh) the ready is re-triggered.
      clearTimeout(rpgChatTimer);
      rpgChatTimer = setTimeout(rpgChatRefresh, 60 * 1000);
      // Check for new messages.
      rpgChatCheckNew();
    });

  }

  // Function to refresh the chatroom by clicking submit.
  function rpgChatRefresh() {
    if ($('.toggle-rpg-chat-pause').attr('paused') == 'not-paused') {
      $('#edit-refresh-chat').triggerHandler('click');
    }
    else {
      clearTimeout(rpgChatTimer);
      rpgChatTimer = setTimeout(rpgChatRefresh, 60 * 1000);
    }
  }

  // Function to toggle pause button.
  function rpgChatPause() {
    if ($('body').attr('ajax') == 'not-ajax') {
      if ($('.toggle-rpg-chat-pause').attr('paused') == 'not-paused') {
        $('.toggle-rpg-chat-pause').attr('paused', 'paused');
        $('.toggle-rpg-chat-pause').css('font-weight', 'bold').css('color', 'red');
      }
      else {
        $('.toggle-rpg-chat-pause').attr('paused', 'not-paused');
        $('.toggle-rpg-chat-pause').css('font-weight', '').css('color', 'graytext');
        $('#edit-refresh-chat').triggerHandler('click');
      }
    }
    // Otherwise do nothing.
    return false;
  }

  // Function to update the title for the page.
  function rpgChatCheckNew() {
    var chat_mess = $('body').attr('chat_mess');
    var chat_first = $('#rpg-chat [id^="comment-"]').filter(':first').attr('id');
    // Check for mismatch.
    if (!document.hasFocus() && chat_first != chat_mess) {
      var numb = 0;
      var found = false;
      // Loop over each comment id.
      $('#rpg-chat [id^="comment-"]').each(function() {
        if (!found && $(this).attr('id') != chat_mess) {
          numb = numb + 1;
        }
        else {
          found = true;
        }
      });
      // Update the title.
      var title = $('body').attr('chat_title');
      if (title.length > 0 ) {
        document.title = title;
      }
      document.title = title + ' (' + numb + ')';
    }
    // Set message for next refresh.
    $('body').attr('chat_mess', chat_first);
  }

  // Once, on inital page load, add the pause button to the interface.
  $('document').ready(function() {

    // If we already have the buttons, don't do anything.
    if (!$('#edit-pause').length) {
      rpgChatInit();
    }

    // Listen for submits, bail out as needed, allow for rebind on ready.
    $('form.node-form, form.comment-form').submit(function() {
      if ($('body').attr('ajax') == 'ajax') {
        return false;
      }
    });

    // Listen for clicks, bail out as needed, allow for rebind on ready.
    $('a').click(function() {
      if ($('body').attr('ajax') == 'ajax') {
        return false;
      }
    });

    // Revert title when we have focus.
    $(window).focus(function() {
      // Revert the title.
      var title = $('body').attr('chat_title');
      if (title.length > 0 ) {
        document.title = title;
      }
      // Update the saved message when refocused.
      rpgChatCheckNew()
    });

  });

  })(jQuery); }
}
