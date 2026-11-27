/**
 * @file
 * Drag-to-reorder for the favorites block (D7 favorites.js used jQuery UI
 * sortable and saved each item's new weight).
 */
(function (Drupal, drupalSettings, once, Sortable) {

  Drupal.behaviors.favoritesLinks = {
    attach: function (context) {
      once('favorites-sortable', '#favorites-list ul', context).forEach(function (list) {
        Sortable.create(list, {
          onEnd: function () {
            var body = new FormData();
            list.querySelectorAll('a.favorites-remove').forEach(function (link) {
              body.append('fids[]', link.getAttribute('fid'));
            });
            fetch(drupalSettings.favorites.orderPath, {method: 'POST', body: body, credentials: 'same-origin'});
          }
        });
      });
    }
  };

})(Drupal, drupalSettings, once, Sortable);
