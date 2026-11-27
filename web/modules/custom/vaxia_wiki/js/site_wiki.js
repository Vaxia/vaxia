/**
 * @file
 * Wiki templates: clicking a template name appends it to the body.
 */
(function (Drupal, once) {
  Drupal.behaviors.siteWikiFeatureTemplates = {
    attach(context) {
      once('site-wiki-template', '.site_wiki_template_name', context).forEach((link) => {
        link.addEventListener('click', (event) => {
          event.preventDefault();
          const template = link.closest('.site_wiki_templates').querySelector('.site_wiki_template').textContent;
          const body = document.querySelector('[name="body[0][value]"]');
          if (body) {
            body.value += template;
          }
        });
      });
    },
  };
})(Drupal, once);
