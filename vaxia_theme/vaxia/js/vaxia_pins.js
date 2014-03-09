/* jQuery driven theme Behaviors. */
(function ($) {

  Drupal.behaviors.pinDisplay = {
    attach: function (context, settings) {
      // Declare function.
      function addCssToPins() {
        $('.pinterest_display').children('span').addClass('pinterest_embed_grid');
        $('.pinterest_embed_grid').children('span').addClass('pinterest_embed_grid_span');
        $('.pinterest_embed_grid_span').filter(":even").addClass('pinterest_embed_grid_hd');
        $('.pinterest_embed_grid_span').filter(":odd").addClass('pinterest_embed_grid_bd');
        $('.pinterest_display span span span a').addClass('pin_link');
        $('.pinterest_display span span span a img').addClass('pin_image');
        $('.pin_image').attr('height', '').attr('width', '').attr('style', '');
        $('.pin_link').attr('height', '').attr('width', '').attr('style', '');
      }

      // Run it when the page is done.
      $('document').ready(function() {
        $('.pinterest_display').once(function() {
           t = setTimeout(addCssToPins, 3000);
        });
      });

  };
})(jQuery);