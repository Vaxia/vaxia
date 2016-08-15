(function ($) {

Drupal.behaviors.spoiler = {
  attach: function() {
    $('.spoiler')
      .addClass('spoiler-js')
      .removeClass('spoiler')
      .children('.spoiler-content')
      .hide()
      .siblings('.spoiler-warning')
      .html(Drupal.t('<span class="spoiler-button" title="Click to view"><span>Show</span><span class="spoiler-button-hide">Hide</span> spoiler</span>'))
      .children('.spoiler-button')
      .click(function() {
        $(this)
        .parent()
        .siblings('.spoiler-content')
        .toggle('normal');
        $('span', this).toggle();
      })
      .children('span.spoiler-button-hide')
      .hide();
  }
};
}(jQuery));