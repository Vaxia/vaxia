/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */
Drupal.behaviors.diceRuler = {
  attach: function(context) { (function($) {

  // Once, on page load, add the pause button to the interface.
  $('document').ready(function() {
    // A variable to store the last slot we used or set.
    var last_slot = 0;
    var vs_slot = 0;
    // On page load, inject the button into place in the DOM.
    var rules = $('.dice_rule').length;
    var current_value = $('#dice-ruler-form select[name="actions"]').val();
    if (rules == 0) {
      var rule_link = '<ul class="dice_rule dice-ruler-buttons dice-ruler-row-buttons"> ' +
        '<li class="dice-ruler-button dice-ruler-one-trait" type="one_trait" title="Make a One Trait ruling with these rolls."><a href="#">1T</a></li>' +
        '<li class="dice-ruler-button dice-ruler-two-trait" type="two_trait" title="Make a Two Trait ruling with these rolls and the ones on the row below."><a href="#">2T</a></li>' +
        '<li class="dice-ruler-button dice-ruler-trait-vs" type="trait_vs" title="Make a One Trait Versus ruling with these rolls."><a href="#">TvT</a></li>' +
        '<li class="dice-ruler-button dice-ruler-two-trait-vs" type="two_trait_vs" title="Make a Two Trait Versus ruling with these rolls and the ones on the row below."><a href="#">2Tv</a></li>' +
        '</ul>';
      $('.dice_rolls .dice').prepend(rule_link);
      var rule_link = '<ul class="dice_rule dice-ruler-buttons dice-ruler-set-buttons"> ' +
        '<li class="dice-ruler-button dice-ruler-combat" type="combat" title="Make a Combat ruling with all available Str, Dex and End rolls."><a href="#">C</a></li>' +
        '<li class="dice-ruler-button dice-ruler-magic" type="magic" title="Make a Magic ruling with all available Int and Spi rolls."><a href="#">M</a></li>' +
        '<li class="dice-ruler-button dice-ruler-tech" type="tech" title="Make a Tech ruling with all available Int and Dex rolls."><a href="#">T</a></li>' +
        '<li class="dice-ruler-button dice-ruler-combat dice-ruler-combat-vs" type="combat-two" title="Make a Combat ruling with all available Str, Dex and End rolls Second Party."><a href="#">Cv</a></li>' +
         '</ul>';
      $('.dice_sets').after(rule_link);
      // Add a reset button.
      var reset = $('#dice-ruler-reset').length;
      if (reset == 0) {
        var reset_button = '<input type="button" class="form-submit" value="Reset" name="dice-ruler-reset" id="dice-ruler-reset" style="display: none;">';
      }
      $('#edit-rule-dice').after(reset_button);
      // Load up just this row when clicked on.
      $('#dice-ruler-reset').click(function() {
        var this_form = $('#dice-ruler-form');
        $(this_form).find(':input[name*="rolled"]').val('');
        $(this_form).find(':input[name*="might"]').val('');
        $(this_form).find(':input[name*="add_diff"]').val('');
        $(this_form).find(':input[name*="combat_weapon"]').val('unarmed');
        $(this_form).find(':input[name*="combat_weapon_a_add"]').val('');
        $(this_form).find(':input[name*="combat_weapon_b_add"]').val('');
        $(this_form).find(':input[name*="number_actions_b"]').val(1);
        $(this_form).find(':input[name*="magic_target"]').val('');
        $('#dice-ruler-form select[name="actions"]').val('hidden').trigger('change');
      });
      // Show or hide the reset button based on the actions selected.
      $('#dice-ruler-form select[name="actions"]').change(function() {
        if ($(this).val() == 'hidden') {
          $('#dice-ruler-reset').hide();
        }
        else {
          $('#dice-ruler-reset').show();
        }
      });
    }

    // Load up just this row when clicked on.
    $('.dice-ruler-row-buttons a').click(function() {
      var rolls = $(this).parents('.dice_rolls');
      // Start incrementing where-ever we currently are.
      var new_slot = last_slot;
      // Get the type of form.
      var button_type = $(this).parent('.dice-ruler-button').attr('type');
      var this_row = $(this).parents('.dice').attr('dice_row');
      this_row = parseInt(this_row);
      // And load it with any defaults we can.
      switch(button_type) {
        case 'one_trait':
          $(rolls).find('.dice_row_' + this_row).each(function() {
            new_slot = new_slot + 1;
            var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
            $(new_form).find(':input[name^="one_trait_might"]').val( $(this).attr('might') );
            $(new_form).find(':input[name^="one_trait_rolled"]').val( $(this).attr('roll') );
          });
        break;
        case 'two_trait':
          // Get the second trait from the next row.
          next_row = this_row + 1;
          $(rolls).find('.dice_row_' + this_row).each(function(index, value) {
            new_slot = new_slot + 1;
            var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
            $(new_form).find(':input[name^="two_trait_might_a"]').val( $(this).attr('might') );
            $(new_form).find(':input[name^="two_trait_rolled_a"]').val( $(this).attr('roll') );
            var this_set = $(this).parent('.dice_set').attr('set');
            $(rolls).find('.dice_set_' + this_set + ' .dice_row_' + next_row).each(function() {
              $(new_form).find(':input[name^="two_trait_might_b"]').val( $(this).attr('might') );
              $(new_form).find(':input[name^="two_trait_rolled_b"]').val( $(this).attr('roll') );
            });
          });
        break;
        case 'trait_vs':
          $(rolls).find('.dice_row_' + this_row).each(function() {
            new_slot = new_slot + 1;
            var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
            $(new_form).find(':input[name^="trait_vs_might_a"]').val( $(this).attr('might') );
            $(new_form).find(':input[name^="trait_vs_rolled_a"]').val( $(this).attr('roll') );
          });
        break;
        case 'two_trait_vs':
          // Get the second trait from the next row.
          next_row = this_row + 1;
          $(rolls).find('.dice_row_' + this_row).each(function(index, value) {
            new_slot = new_slot + 1;
            var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
            $(new_form).find(':input[name^="two_trait_vs_might_aa"]').val( $(this).attr('might') );
            $(new_form).find(':input[name^="two_trait_vs_rolled_aa"]').val( $(this).attr('roll') );
            var this_set = $(this).parent('.dice_set').attr('set');
            $(rolls).find('.dice_set_' + this_set + ' .dice_row_' + next_row).each(function() {
              $(new_form).find(':input[name^="two_trait_vs_might_ab"]').val( $(this).attr('might') );
              $(new_form).find(':input[name^="two_trait_vs_rolled_ab"]').val( $(this).attr('roll') );
            });
          });
        break;
      }
      // Now set the acount count.
      if (new_slot > 3) {
        new_slot = 4;
      }
      // Set the actions.
      $('#dice-ruler-form select[name="actions"]').val(new_slot).trigger('change');
      // We don't want to change any but the new ones, but set it to the asked for form type.
      for (var form_number = last_slot + 1; form_number <= new_slot; form_number++) {
        $('#dice-ruler-form #edit-action-' + form_number + ' :input[name="roll_type_' + form_number + '"]').val(button_type).trigger('change');
        last_slot = last_slot + 1;
      }      
      // And done.
      if (last_slot > 3) {
        last_slot = 0;
      }
      return false; // Don't go anywhere.
    });

    // Load it all up when clicked on.
    $('.dice-ruler-set-buttons a').click(function() {
      var rolls = $(this).parents('.dice_rolls');
      // Start incrementing where-ever we currently are.
      var new_slot = last_slot;
      // Get the type of form.
      var button_type = $(this).parent('.dice-ruler-button').attr('type');
      // And load it with any defaults we can.
      switch(button_type) {
        case 'combat':
          // Get all str, dex, end combinations.
          $(rolls).find('.dice_set').each(function() {
            if ($(this).find('.dice_roll_str').length > 0 && $(this).find('.dice_roll_dex').length > 0) {
              new_slot = new_slot + 1;
              var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
              $(new_form).find(':input[name^="combat_might_a_dex"]').val( $(this).find('.dice_roll_dex').attr('might') );
              $(new_form).find(':input[name^="combat_might_a_str"]').val( $(this).find('.dice_roll_str').attr('might') );
              $(new_form).find(':input[name^="combat_might_a_end"]').val( $(this).find('.dice_roll_end').attr('might') );
              $(new_form).find(':input[name^="combat_rolled_a_dex"]').val( $(this).find('.dice_roll_dex').attr('roll') );
              $(new_form).find(':input[name^="combat_rolled_a_str"]').val( $(this).find('.dice_roll_str').attr('roll') );
              $(new_form).find(':input[name^="combat_rolled_a_end"]').val( $(this).find('.dice_roll_end').attr('roll') );
            }
          });
        break;
        case 'magic':
          // Get all int, spi combinations.
          $(rolls).find('.dice_set').each(function() {
            if ($(this).find('.dice_roll_int').length > 0 && $(this).find('.dice_roll_spi').length > 0) {
              new_slot = new_slot + 1;
              var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
              $(new_form).find(':input[name^="magic_might_a"]').val( $(this).find('.dice_roll_int').attr('might') );
              $(new_form).find(':input[name^="magic_might_b"]').val( $(this).find('.dice_roll_spi').attr('might') );
              $(new_form).find(':input[name^="magic_rolled_a"]').val( $(this).find('.dice_roll_int').attr('roll') );
              $(new_form).find(':input[name^="magic_rolled_b"]').val( $(this).find('.dice_roll_spi').attr('roll') );
            }
          });
        break;
        case 'tech':
          // Get all int, dex combinations.
          $(rolls).find('.dice_set').each(function() {
            if ($(this).find('.dice_roll_int').length > 0 && $(this).find('.dice_roll_dex').length > 0) {
              new_slot = new_slot + 1;
              var new_form = $('#dice-ruler-form #edit-action-' + new_slot);
              $(new_form).find(':input[name^="two_trait_might_a"]').val( $(this).find('.dice_roll_int').attr('might') );
              $(new_form).find(':input[name^="two_trait_might_b"]').val( $(this).find('.dice_roll_dex').attr('might') );
              $(new_form).find(':input[name^="two_trait_rolled_a"]').val( $(this).find('.dice_roll_int').attr('roll') );
              $(new_form).find(':input[name^="two_trait_rolled_b"]').val( $(this).find('.dice_roll_dex').attr('roll') );
            }
          });
          button_type = 'two_trait';
        case 'combat-two':
          // Get all str, dex, end combinations.
          $(rolls).find('.dice_set').each(function() {
            if ($(this).find('.dice_roll_str').length > 0 && $(this).find('.dice_roll_dex').length > 0) {
              vs_slot = vs_slot + 1;
              var new_form = $('#dice-ruler-form #edit-action-' + vs_slot);
              $(new_form).find(':input[name^="combat_might_b_dex"]').val( $(this).find('.dice_roll_dex').attr('might') );
              $(new_form).find(':input[name^="combat_might_b_str"]').val( $(this).find('.dice_roll_str').attr('might') );
              $(new_form).find(':input[name^="combat_might_b_end"]').val( $(this).find('.dice_roll_end').attr('might') );
              $(new_form).find(':input[name^="combat_rolled_b_dex"]').val( $(this).find('.dice_roll_dex').attr('roll') );
              $(new_form).find(':input[name^="combat_rolled_b_str"]').val( $(this).find('.dice_roll_str').attr('roll') );
              $(new_form).find(':input[name^="combat_rolled_b_end"]').val( $(this).find('.dice_roll_end').attr('roll') );
            }
          });
          button_type = 'combat';
        break;
      }
      // Now set the new count.
      if (new_slot > 3) {
        new_slot = 4;
        vs_slot = 4;
      }
      // Set the actions.
      $('#dice-ruler-form select[name="actions"]').val(new_slot).trigger('change');
      // We don't want to change any but the new ones, but set it to the asked for form type.
      for (var form_number = last_slot + 1; form_number <= new_slot; form_number++) {
        $('#dice-ruler-form #edit-action-' + form_number + ' :input[name="roll_type_' + form_number + '"]').val(button_type).trigger('change');
        last_slot = last_slot + 1;
      }
      // And done.
      if (last_slot > 3) {
        last_slot = 0;
        vs_slot = 0;
      }
      return false; // Don't go anywhere.
    });
  });

  })(jQuery); }
}
