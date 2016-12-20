/**
 * @file
 * Javascript for Vaxia UI on RPG pages.
 *
 * Allows user to collapse the description as needed.
 */
Drupal.behaviors.vaxia_chatrooms = {
  attach: function(context) { (function($) {

    // Only run this if the screen is over 600.
    if ( $( window ).width() > 600) {
      $('document').ready(function() {
        // Add toggle buttons just the once.
        $('.node-type-rpg-chatroom').once(function() {
          // Hide and adjust widths on the sidebar vs content.
          $('.sidebar-first #content').css('margin-left', '0%');
          $('.sidebar-first #content').css('width', '100%');
          $('.sidebar-first .region-sidebar-first').css('width', '100%');
          // And hide the header.
          $('#header').hide();
        });
      });
    }
    else {
      // Only run this if the screen is equal or under 480.
      // If it can be collapsed, collapse it.
      $('document').ready(function() {
        $('fieldset.collapsible').addClass('collapsed');
      });
    }

  })(jQuery); }
}
