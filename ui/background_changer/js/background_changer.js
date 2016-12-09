/**
 * @file
 * Javascript for body background-image.
 *
 */

Drupal.behaviors.backgroundChanger = {
  attach: function(context) { (function($) {

  function setBackground(path) {
    // Get the current time on the user's end and capture the hours digit
    d = new Date();
    n = d.getHours();
    // Converting 24-hour clock to 12-hour, with PM hours working in reverse (1pm = 11am)
    if (n > 12){
      n = n - 12;
    }
    else {
      n = 12 - n;
    }
    // Grab the base path from Drupal.settings and add the specific background image
    path = path + n + '.jpg';
    var target = 'body';
    if ($('.new_vaxia_demo').length > 0) {
        target = '#rpg-chat';
    }
    // Swap the background-image attribute on the body tag
    $(target).css('background-image', 'url(' + path + ')');
  }
  setBackground(Drupal.settings.background_changer.path);

  })(jQuery); }
}
