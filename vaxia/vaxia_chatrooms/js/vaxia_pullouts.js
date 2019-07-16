/**
 * @file
 * Javascript for Vaxia UI on RPG pages.
 *
 * Provides pullout functions.
 */
Drupal.behaviors.vaxia_chatrooms_pullouts = {
  attach: function(context) { (function($) {

    // Only run this if the screen is over 600.
    if ( $( window ).width() > 600) {
      $('document').ready(function() {
        // Add toggle buttons just the once.
        $('.node-type-rpg-chatroom').once(function() {

          function make_flap(label, id, txt_target, top) {
            var target = $(txt_target);
            if (target != null && target.length > 0) {
              $(target).wrap('<div id="target_' + id + '" class="jsPullout jsPulloutContent isClosed"></div>');
              $('#main-content').before(
                '<div id="' + id + '" class="jsPullout jsPulloutFlap rotate" style="top:' + top + ';">' +
                '  <a class="jsPulloutFlapLabel" href="#">' + label + '</a>' +
                '</div>'
              );
            }
          }

          function toggle_flap(flap_name) {
            var flap = $('#' + flap_name);
            var target = $('#target_' + flap_name);
            if (target.hasClass('isClosed')) {
              $('.jsPulloutContent').addClass('isClosed').fadeTo(200, 0, 'swing').hide();
              $('.jsPulloutFlap').removeClass('isActive').fadeTo(200, 0.8, 'swing');
              target.removeClass('isClosed').fadeTo(400, 1, 'swing').show();
              flap.addClass('isActive').fadeTo(400, 1, 'swing');
            }
            else {
              target.addClass('isClosed').fadeTo(400, 0, 'swing').hide();
              flap.removeClass('isActive').fadeTo(400, 0.8, 'swing');
            }
          }
          // Set marking JS
          $('body').addClass('jsPulloutActive');

          // Make pullouts for the sidebars.
          make_flap('Site Nav', 'extruderBarHead', '#primary-menu-bar .primary-menu', '110px');
          make_flap('Sidebar', 'extruderBarFirst', '.sidebar-first .region-sidebar-first', '220px');
          make_flap('Description', 'extruderBarDesc',
            'article.node-rpg-chatroom fieldset.group-description', '330px');
          make_flap('Search', 'extruderBarSearch',
            '#block-views-rpg-chats-inroom-rpg-chats', '440px');

          // Set listener for all flaps now that they've been created.
          $('.jsPulloutFlap a.jsPulloutFlapLabel').click(function() {
            toggle_flap( $(this).parent().attr('id') );
            return false;
          });
        });
      });
    }

  })(jQuery); }
}
