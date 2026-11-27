/**
 * @file
 * Common skills on the character form: pick a skill preset of the chosen
 * realm and "Add skill" fills the next empty skill row (name, description,
 * aspects), adding a row first when none is empty. The value is left for the
 * player to set.
 */
(function ($, Drupal, drupalSettings, once) {
  'use strict';

  // The preset waiting for a new row (the add-more request replaces the
  // skills markup, and with it the behavior's own variables).
  var pending = null;

  function emptyRow() {
    var found = null;
    $('input[name^="field_skill["][name$="[subform][field_skill_name][0][value]"]').each(function () {
      if (found === null && !$(this).val()) {
        found = this.name.match(/^field_skill\[(\d+)\]/)[1];
      }
    });
    return found;
  }

  function fill(delta, preset) {
    var base = 'field_skill[' + delta + '][subform]';
    $('[name="' + base + '[field_skill_name][0][value]"]').val(preset.name).trigger('change');
    $('[name="' + base + '[field_skill_desc][0][value]"]').val(preset.description).trigger('change');
    ['one', 'two', 'three'].forEach(function (n, i) {
      $('[name="' + base + '[field_aspect_' + n + ']"]').val(preset.aspects[i] || '').trigger('change');
    });
    $('[name="' + base + '[field_skill_value][0][value]"]').trigger('focus');
  }

  Drupal.behaviors.vaxiaSkillPresets = {
    attach: function (context) {
      once('vaxia-skill-presets-ajax', 'html').forEach(function () {
        $(document).on('ajaxComplete', function () {
          var delta = pending ? emptyRow() : null;
          if (delta !== null) {
            fill(delta, pending);
            pending = null;
          }
        });
      });
      once('vaxia-skill-presets', '.vaxia-skill-presets', context).forEach(function (wrapper) {
        var presets = drupalSettings.vaxiaSkillPresets || {};
        var $select = $(wrapper).find('select.vaxia-skill-preset');
        var $realm = $('select[name="field_tag_realm"]');

        // Only the chosen realm's presets; hidden until a realm is picked.
        function options() {
          var realm = $realm.val();
          $select.find('option').not(':first').remove();
          Object.keys(presets).forEach(function (id) {
            if (realm && presets[id].realm === realm) {
              $select.append($('<option>').val(id).text(presets[id].name));
            }
          });
          $select.val('');
          $(wrapper).toggle($select.find('option').length > 1);
        }
        $realm.off('change.vaxiaSkillPresets').on('change.vaxiaSkillPresets', options);
        options();

        $(wrapper).find('.vaxia-skill-preset-add').on('click', function (e) {
          e.preventDefault();
          var preset = presets[$select.val()];
          if (!preset) {
            return;
          }
          var delta = emptyRow();
          if (delta !== null) {
            fill(delta, preset);
            $select.val('');
            return;
          }
          pending = preset;
          $('input[name="field_skill_skill_add_more"]').trigger('mousedown');
        });
      });
    }
  };
})(jQuery, Drupal, drupalSettings, once);
