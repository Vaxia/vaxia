/* jQuery driven theme Behaviors. */
(function ($) {

  Drupal.behaviors.themeVaxia = {
    attach: function (context, settings) {
      /* Add css tag even/odd to each image in an article body. */
      $('article .field-name-body img', context).each(function (index) {
        if (index % 2 == 0 || index == 0) {
          $(this).addClass('even');
        }
        else {
          $(this).addClass('odd');
        }
        var this_id = $(this).attr('id');
        if (this_id == undefined || this_id.length == 0) {
          this_id = 'img_' + index;
          $(this).attr('id', this_id);
        }
        $(this).wrap('<a class="colorbox-load" href="' + $(this).attr('src') + '?width=500&height=500&inline=true#' + this_id + '"></a>');
      });

      /* Add css tag even/odd to each paragraph in an article  body. */
      $('article .field-name-body p', context).each(function (index) {
        if (index % 2 == 0 || index == 0) {
          $(this).addClass('even');
        }
        else {
          $(this).addClass('odd');
        }
      });

      /* Add css tag to service links so we can treat them individually. */
      $('.links li[class^="service-links"]', context).each(function (index) {
        $(this).addClass('service-links');
      });

      /* Add css tag to buttons. */
      $('.af-button-small, #content .links a', context).each(function (index) {
        $(this).addClass('button');
        $(this).addClass('button-small');
      });
      $('.af-button-large', context).each(function (index) {
        $(this).addClass('button');
        $(this).addClass('button-large');
      });

      /* Change the text of an html button in quote for consistancy. */
      $('.links li.quote a', context).each(function (index) {
        $(this).html('quote');
      });
    }
  };

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