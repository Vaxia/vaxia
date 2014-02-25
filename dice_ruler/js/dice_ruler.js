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
    // On page load, inject the button into place in the DOM.
    var rules = $('.dice_rule').length;
    if (rules == 0) {
      var rule_link = '<span class="dice_rule"><a href="#">*</a></span>';
      $('.suggested_rule').before(rule_link);
    }

    // And load it all up when clicked on.
    $('.dice_rule').click(function() {
      // On page load, inject the button into place in the DOM.
      var rolls = $(this).parent('.dice_rolls');
      var suggested = rolls.find('.suggested_rule').html();
      switch(suggested) {
        case 'one-trait':
          $('#edit-roll-type').val('one_trait').trigger('change');
          $('#edit-one-trait-might').val( rolls.find('.dice_roll_0 .might').html() );
          $('#edit-one-trait-rolled').val( rolls.find('.dice_roll_0 .roll').html() );
        break;
        case 'two-trait':
          $('#edit-roll-type').val('two_trait').trigger('change');
          $('#edit-two-trait-might-a').val( rolls.find('.dice_set_0 .might').html() );
          $('#edit-two-trait-rolled-a').val( rolls.find('.dice_roll_0 .roll').html() );
          $('#edit-two-trait-might-b').val( rolls.find('.dice_roll_1 .might').html() );
          $('#edit-two-trait-rolled-b').val( rolls.find('.dice_roll_1 .roll').html() );
        break;
        case 'magic':
          $('#edit-roll-type').val('magic').trigger('change');
          $('#edit-magic-might-a').val( rolls.find('.dice_roll_int .might').html() );
          $('#edit-magic-rolled-a').val( rolls.find('.dice_roll_int .roll').html() );
          $('#edit-magic-might-b').val( rolls.find('.dice_roll_spi .might').html() );
          $('#edit-magic-rolled-b').val( rolls.find('.dice_roll_spi .roll').html() );
        break;
        case 'combat':
          $('#edit-roll-type').val('combat').trigger('change');
          rolls.find('.dice_set').each( function() {
            var index = ($(this).find('.roll_set').html() * 1) + 1; // Convert to number.
            $('#edit-combat-action-count-a').val(index).trigger('change');
            $('#edit-combat-might-a-dex-' + index).val( $(this).find('.dice_roll_dex .might').html() );
            $('#edit-combat-rolled-a-dex-' + index).val( $(this).find('.dice_roll_dex .roll').html() );
            $('#edit-combat-might-a-str-' + index).val( $(this).find('.dice_roll_str .might').html() );
            $('#edit-combat-rolled-a-str-' + index).val( $(this).find('.dice_roll_str .roll').html() );
            $('#edit-combat-might-a-end-' + index).val( $(this).find('.dice_roll_end .might').html() );
            $('#edit-combat-rolled-a-end-' + index).val( $(this).find('.dice_roll_end .roll').html() );
          });
        break;
      }
      return false; // Don't go anywhere.
    });
  });

  })(jQuery); }
}
