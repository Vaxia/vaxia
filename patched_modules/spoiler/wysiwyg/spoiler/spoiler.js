(function ($) {
  Drupal.wysiwyg.plugins['spoiler'] = {
    invoke: function(data, settings, instanceId) {
      Drupal.wysiwyg.instances[instanceId].insert('[spoiler]' + data.content + '[/spoiler]');
    }
  };
})(jQuery);
