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
    $('body').css('background-image','url(/sites/all/themes/vaxia_theme/vaxia/images/'+ n +'.jpg';
      }
    }
  }
}
