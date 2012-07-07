/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */
Drupal.behaviors.vaxiaChat = {
  attach: function(context) { (function($) {

  // Grab the refresh rate from the user settings.
  var refeshRate = Drupal.settings.rpg_chat['refreshRate'];

  // Function to refresh the chatroom by clicking submit.
  function refreshChat() {
    $("#edit-refresh-chat").triggerHandler("click");
  }

  // On page load, start the chat running.
  // Run only the one time to avoid ajax triggering multiple loads.
  // On each ajax reload (as triggered by refreshChat) the ready is re-triggered.
  $('document').ready(function() {
    var timer = setTimeout(refreshChat, refeshRate * 1000);
  });

  // When clicking the comment post, empty the post out.
  $('#edit-submit').ajaxComplete(function() {
    $('#comment-form #edit-comment-body-und-0-value, #comment-form #edit-vaxia-rolls-notes').val();
  });

  })(jQuery); }
}
