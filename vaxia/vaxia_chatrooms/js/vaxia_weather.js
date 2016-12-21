/**
 * @file
 * Javascript for Vaxia UI on RPG pages.
 *
 * Provides pullout functions.
 */
Drupal.behaviors.vaxia_chatrooms_weather = {
  attach: function(context) { (function($) {

    // Only run this if the screen is over 600.
    if ( $( window ).width() > 600) {
      $('document').ready(function() {
        // Add toggle buttons just the once.
        $('.node-type-rpg-chatroom').once(function() {
/*
// Tweak the display width for all moon displays.
        $('img.moon-overlay').each(function() {
          var moon_width = $(this).attr('width');
          moon_width = (moon_width / 26) * 100;
          moon_width = '' + moon_width + '%';
          $(this).attr('width', moon_width).width(moon_width);
        });
*/
          // Only add the display if we're in the ic room
          var target = $('.node-type-rpg-chatroom .ic-room');
          if (target != null && target.length > 0) {
            // Copy weather and moon back into display.
            var weather = $('.weather-pic').html();
            if (weather == null || weather.length == 0) {
              weather = '';
            }
            var moon = $('#block-moon-phases-moons .moon-block').html();
            if (moon == null || moon.length == 0) {
              moon = '';
            }
            var rpg_weather = '' +
              '<div id="rpg-chat-weather">' +
              '<div class="rpg-weather rpg-weather-img">' + weather + '</div>' +
              '<div class="rpg-weather rpg-moon-img moon-block">' + moon +'</div>' +
              '</div>';
            // Place the new weather widget.
            $(target).before(rpg_weather);
          }
          // Remove the old widget no matter what.
          $('#block-moon-phases-moons').remove();
        });
      });
    }

  })(jQuery); }
}
