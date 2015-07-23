/**
 * @file
 * Javascript for Vaxia UI on RPG pages.
 *
 * Allows user to collapse the description as needed.
 */
Drupal.behaviors.setupPullouts = {
  attach: function(context) { (function($) {

    // Only run this if the screen is over 600.
    if ( $( window ).width() > 600) {
      $('document').ready(function() {
        // Add toggle buttons just the once.
        $('.node-type-rpg-chatroom article.node-rpg-chatroom').once(function() {
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

Drupal.behaviors.doPullouts = {
  attach: function(context) { (function($) {

    // Only run this if the screen is over 600.
    if ( $( window ).width() > 600) {

      /* Functions. */
      function make_flap(label, id, txt_target, top) {
        $(txt_target).wrap('<div id="target_' + id + '" class="jsPullout jsPulloutContent isClosed"></div>');
        $('#main').before(
            '<div id="' + id + '" class="jsPullout jsPulloutFlap rotate" style="top:' + top + ';">' +
            '  <a class="jsPulloutFlapLabel" href="#">' + label + '</a>' +
            '</div>'
          );
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

      // Add toggle buttons just the once.
      $('document').ready(function() {

        // Make pullouts for the sidebars.
        if (!$('#target_extruderBarHead').length) {
          make_flap('Site Nav', 'extruderBarHead', '#navigation', '110px');
          make_flap('Sidebar', 'extruderBarFirst', '.sidebar-first .region-sidebar-first', '220px');
          make_flap('Description', 'extruderBarDesc',
            'article.node-rpg-chatroom fieldset.group-description', '330px');

          // Tweak the display width for all moon displays.
          $('img.moon-overlay').each(function() {
            var moon_width = $(this).attr('width');
            moon_width = (moon_width / 26) * 100;
            moon_width = '' + moon_width + '%';
            $(this).attr('width', moon_width).width(moon_width);
          });

          // Copy weather and moon back into display.
          var weather = $('.weather-pic').html();
          var moon = $('.moon-block').html();
          var rpg_weather = '' +
            '<div id="rpg-chat-weather" style="float:right;">' +
            '<div class="rpg-weather rpg-weather-img">' + weather + '</div>' +
            '<div class="rpg-weather rpg-moon-img moon-block">' + moon +'</div>' +
            '</div>';
          $('#rpg-chat-wrapper').before(rpg_weather);

          // Set listener for all flaps now that they've been created.
          $('.jsPulloutFlap a.jsPulloutFlapLabel').click(function() {
            toggle_flap( $(this).parent().attr('id') );
            return false;
          });
        };

      });

    } // End if ( $( window ).width() > 600) {

  })(jQuery); }
}
