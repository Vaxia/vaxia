/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */
Drupal.behaviors.siteWikiFeatureTemplates = {
  attach: function(context) { (function($) {

  // When clicking on a template link in the UI.
  // Load the template.
  $('.site_wiki_template_name').click(function() {
    var template = $(this).parent('.site_wiki_templates').find('.site_wiki_template').html();
    // Append.
    $('#edit-body-und-0-value').val($('#edit-body-und-0-value').val() + template);
    // Don't go anywhere.
    return false;
  });

  })(jQuery); }
}
