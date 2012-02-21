/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */
Drupal.behaviors.vaxiaChat = {
  attach: function(context) { (function($) {

  var refeshRate = Drupal.settings.rpg_chat['refreshRate'];
  var timer;
  var chatRunning = 0;

  // On page load, start the chat running.
  // Run only the one time to avoid ajax triggering multiple loads. 
  $('document').once().ready(function() {
    if (!chatRunning) {
      chatRunning = 1;
      refreshChat();
    }
  });

  // Function to refresh the chatroom by clicking submit.
  function refreshChat() {
    $("#edit-refresh").triggerHandler("click");
    clearTimeout(timer);
    // TESTING.
    // Since the refresh is driven by AJAX, the doc gets reloaded.
    // When it's reloaded, the refresh hits up again.
    // So we actually don't need to loop this.
    //timer = setTimeout(refreshChat, refeshRate * 1000);
  }

  })(jQuery); }
}
