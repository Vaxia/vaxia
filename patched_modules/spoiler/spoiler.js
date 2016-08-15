(function ($) {

Drupal.behaviors.spoiler = {
  attach: function() {
    $('.spoiler').each(function(index) {
      var spoiler_label = $(this).children('.spoiler-warning').attr('show');
      $(this)
        .addClass('spoiler-js')
        .removeClass('spoiler')
        .children('.spoiler-content')
        .hide()
        .siblings('.spoiler-warning')
        .html(Drupal.t('<span class="spoiler-button" title="Click to view"><span>Show</span> ' +
          '<span class="spoiler-button-hide">Hide</span> ' +
          spoiler_label  +
          ' </span>'))
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
    });
  }
};
}(jQuery));