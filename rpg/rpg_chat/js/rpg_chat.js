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
  var in_ajax = false;
  var timer;

  // Function to refresh the chatroom by clicking submit.
  function refreshChat() {
    if (paused == false) {
      $("#edit-refresh-chat").triggerHandler("click");
    }
  }

  // Once, on page load, add the pause button to the interface.
  $('document').ready(function() {

    // If we already have the buttons, don't do anything.
    if (!$('#edit-pause').length) {
      // On page load, inject the buttons into place in the DOM.
      $('.rpg-chat-node #edit-actions').prepend('<input type="button" id="edit-pause" class="toggle-rpg-chat-pause form-submit" value="Pause">');
      $('.rpg-chat-node #edit-0').before('<input type="button" id="edit-2" class="toggle-rpg-chat-pause form-submit" value="Pause">');
      $('.toggle-rpg-chat-pause').css('font-weight', '').css('color', 'graytext');

      // And the listener only after we have the button in place.
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

      // Listen for submits, bail out as needed.
      $('form.node-form, form.comment-form').submit(function() {
        if (in_ajax == true) {
          return false;
        }
      });

      // Listen for clicks, bail out as needed.
      $('a').click(function() {
        if (in_ajax == true) {
          return false;
        }
      });
    }

    // On page load, start the chat running.
    // On each ajax reload (as triggered by refreshChat) the ready is re-triggered.
    timer = setTimeout(refreshChat, refreshRate * 1000);

  });

  // Prevent form submission while in an AJAX event.
  $('form.node-form, form.comment-form').ajaxStart(function(){
    in_ajax = true;
    $('form.node-form #edit-1, form.comment-form #edit-1, form.node-form #edit-submit, form.comment-form #edit-submit').css('color', 'graytext');
  })
  .ajaxStop(function() {
    in_ajax = false;
    $('form.node-form #edit-1, form.comment-form #edit-1, form.node-form #edit-submit, form.comment-form #edit-submit').css('color', 'black');
  });

  // Prevent anchor clicks while in an AJAX event.
  $('a').ajaxStart(function(){
    in_ajax = true;
  })
  .ajaxStop(function() {
    in_ajax = false;
  });

  })(jQuery); }
}
