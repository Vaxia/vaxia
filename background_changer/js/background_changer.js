/**
 * @file
 * Javascript for body background-image.
 * 
 */

Drupal.behaviors.backgroundChanger = {
  attach: function(context) { (function($) {
    
  function setBackground (d,n) {
    d = new Date();
    n = d.getHours();

    if (n>12){
      n = 24-n;
    }

    $('body').css('background-image','url(/sites/all/themes/vaxia_theme/vaxia/images/'+ n +'.jpg';

      }
    }
  }
}
