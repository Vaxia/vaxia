/* jQuery driven theme Behaviors. */
Drupal.behaviors.pinsDisplay = {
  attach: function(context) { (function($) {

  // Only run this if the screen is over 600.
  if ( $( window ).width() > 600) {
      
    // Declare function.
    function addCssToPins() {
      // Add css.
      $('.pinterest_display').children('span').addClass('pinterest_embed_grid');
      $('.pinterest_embed_grid').children('span').addClass('pinterest_embed_grid_span');
      $('.pinterest_embed_grid_span').filter(":even").addClass('pinterest_embed_grid_hd');
      $('.pinterest_embed_grid_span').filter(":odd").addClass('pinterest_embed_grid_bd');
      $('.pinterest_display span span span a').addClass('pin_link');
      $('.pinterest_display span span span a img').addClass('pin_image');
      // Add listener events.
      $('.pin_link').mouseover(function(event) {
        $(this).addClass('pinHover');
        $('.pinDisplay').html( $(this).html() );
        $('.pinDisplay img').css('height', 'auto').css('width', 'auto')
          .css('margin', '5px').css('border-radius', '10px');
        $('.pinDisplay').css('width', 'auto').css('height', 'auto').css('');
        var imageHeight = $('.pinDisplay img').height();
        var boxTop = event.pageY - (imageHeight / 2);
        var boxLeft = event.pageX + 120;
        $('.pinDisplay').css('top',boxTop).css('left', boxLeft);
        $('.pinDisplay').fadeIn('200');
      });
      $('.pin_link').mouseout(function() {
        $(this).removeClass('pinHover');
        $('.pinDisplay').html('').hide();
      });
    }
  
    // Run it when the page is done.
    $('document').ready(function() {
      // Add display window.
      $('body').once(function() {
        $(this).append('<div class="pinDisplay" ' + 
          'style="display:none;position:absolute;z-index:20;margin:5px;border-radius:10px;">' +
          '</div>'); 
      });
      // Set up CSS and listeners.
      $('.pinterest_display').once(function() {
         t = setTimeout(addCssToPins, 3000);
      });
    });

  } // End if.

  })(jQuery); }
}
