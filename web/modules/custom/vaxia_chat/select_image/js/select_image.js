/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */
Drupal.behaviors.selectImage = {
  attach: function(context) { (function($) {

    // On ready, hook up the image dropdown code to supporting selects.
    try {
      $('body select.form-select-image').msDropDown();
    } catch(e) {
      //alert(e.message);
    }

  })(jQuery); }
}
