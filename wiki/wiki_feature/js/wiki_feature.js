/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */
Drupal.behaviors.wikiFeatureTemplates = {
  attach: function(context) { (function($) {

  // When clicking on a template link in the UI.
  // Load the template.
  $('.wiki_feature_template_name').click(function() {
    var template = $(this).parent('.wiki_feature_templates').find('.wiki_feature_template').html();
    // Append.
    $('#edit-body-und-0-value').val($('#edit-body-und-0-value').val() + template);
    // Don't go anywhere.
    return false;
  });

  })(jQuery); }
}
