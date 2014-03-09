/* jQuery driven theme Behaviors. */
(function ($) {

  Drupal.behaviors.pinDisplay = {
    attach: function (context, settings) {
    t = setTimeout( function() {
      $('.pinterest_display').children('span').addClass('pinterest_embed_grid');
      $('.pinterest_embed_grid').children('span').addClass('pinterest_embed_grid_span');
      $('.pinterest_embed_grid_span').filter(":even").addClass('pinterest_embed_grid_hd');
      $('.pinterest_embed_grid_span').filter(":odd").addClass('pinterest_embed_grid_bd');
      $('.pinterest_display span span span a').addClass('pin_link');
      $('.pinterest_display span span span a img').addClass('pin_image');
      $('.pin_image').attr('height', '').attr('width', '').attr('style', '');
      $('.pin_link').attr('height', '').attr('width', '').attr('style', '');
    }, 3000);
  };

})(jQuery);