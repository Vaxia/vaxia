/**
 * @file
 * Javascript for body background-image.
 * 
 */

Drupal.behaviors.backgroundChanger = {
  attach: function(context) { (function($) {
  
  
  function setBackground (d,n,path) {
    //Get the current time on the user's end and capture the hours digit
    d = new Date();
    n = d.getHours();
    
    //Converting 24-hour clock to 12-hour, with PM hours working in reverse (1pm = 11am)
    if (n>12){
      n = 24-n;
    }
    
    //Find the image subfolder within the background_changer module
    path = DRUPAL_ROOT + '/' + drupal_get_path('module', 'background_changer') + "/images/" + n + ".jpg";
  
    //Swap the background-image attribute on the body tag
    $('body').css('background-image',path);

      }
    }
  }
}
