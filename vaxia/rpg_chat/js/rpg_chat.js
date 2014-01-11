/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */
Drupal.behaviors.rpgChat = {
  attach: function(context) { (function($) {

  // Grab the refresh rate from the user settings.
  var refreshRate = Drupal.settings.rpg_chat['refreshRate'];
  var paused = false;
  var timer;

  // Function to refresh the chatroom by clicking submit.
  function refreshChat() {
    if (paused == false) {
      $("#edit-refresh-chat").triggerHandler("click");
    }
  }

  // Once, on page load, add the pause button to the interface.
  $('document').ready(function() {
    if (!$('.rpg-chat-node #edit-pause').length) {
      // On page load, inject the buttons into place in the DOM.
      $('.rpg-chat-node #edit-actions').prepend('<input type="submit" id="edit-pause" class="toggle-rpg-chat-pause form-submit" value="Pause">');
      $('.rpg-chat-node #edit-0').before('<input type="submit" id="edit-2" class="toggle-rpg-chat-pause form-submit" value="Pause">');
      $('.toggle-rpg-chat-pause').css('font-weight', '').css('color', 'graytext');
    }
  });

  // When clicking the comment post, empty the post out.
  $('.toggle-rpg-chat-pause').click(function() {
    if (paused == false) {
      paused = true;
      $('.toggle-rpg-chat-pause').css('font-weight', 'bold').css('color', 'red');
    }
    else {
      paused = false;
      $('.toggle-rpg-chat-pause').css('font-weight', '').css('color', 'graytext');
      $("#edit-refresh-chat").triggerHandler("click");
    }
    // Otherwise do nothing.
    return false;
  });

  // Prevent form submission while in an AJAX event.
  $('form.node-form, form.comment-form').ajaxStart(function(){
    $(this).submit(function() {
      return false;
    });
  })
  .ajaxStop(function() {
    $(this).unbind('submit');
    $(this).submit(function() {
      return true;
    });
  });

  // Prevent anchor clicks while in an AJAX event.
  $('a').ajaxStart(function(){
    $(this).click(function() {
      return false;
    });
  })
  .ajaxStop(function() {
    $(this).unbind('click');
    $(this).click(function() {
      return true;
    });
  });

  // On page load, start the chat running.
  // Run only the one time to avoid ajax triggering multiple loads.
  // On each ajax reload (as triggered by refreshChat) the ready is re-triggered.
  $('document').ready(function() {
    timer = setTimeout(refreshChat, refreshRate * 1000);
  });

  })(jQuery); }
}
