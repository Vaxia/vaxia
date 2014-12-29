/* jQuery driven theme Behaviors. */
Drupal.behaviors.themeVaxia = {
  attach: function(context) { (function($) {

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

  /* Hide standard display messags in chatrooms after they've posted. */
  $('.node-type-rpg-chatroom .messages--status').delay('fast').fadeOut();

  })(jQuery); }
}
