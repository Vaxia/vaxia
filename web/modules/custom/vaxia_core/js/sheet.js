/**
 * @file
 * Character form: the realm drives parts of the form.
 *
 * - Realm details and Skills stay closed until a realm is picked, and open
 *   when one is.
 * - Rows added to the skills ("Add another skill") get the realm's #states
 *   too. Drupal announces a trigger's value to its dependents only once, so
 *   rows that arrive by AJAX (Tech contexts, shown for science fiction only)
 *   start visible whatever the realm. The realm is announced again after each
 *   AJAX request.
 */
(function ($, Drupal, once) {
  'use strict';

  function realm() {
    return $('select[name="field_tag_realm"]');
  }

  function picked() {
    var value = realm().val();
    return !!value && value !== '_none';
  }

  function groups() {
    return $('#edit-group-realm, #edit-group-skills');
  }

  Drupal.behaviors.vaxiaSheet = {
    attach: function (context) {
      once('vaxia-sheet', 'html').forEach(function () {
        if (realm().length && !picked()) {
          groups().prop('open', false);
        }
        $(document).on('ajaxComplete', function () {
          var $realm = realm();
          if ($realm.length) {
            $realm.trigger({type: 'state:value', value: $realm.val(), oldValue: null});
          }
        });
      });
      // On the select itself: its AJAX handler stops the event bubbling.
      once('vaxia-sheet-realm', 'select[name="field_tag_realm"]', context).forEach(function (select) {
        $(select).on('change', function () {
          groups().prop('open', picked());
        });
      });
    }
  };
})(jQuery, Drupal, once);
