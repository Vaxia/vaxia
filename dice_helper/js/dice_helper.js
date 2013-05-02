/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */

Drupal.behaviors.diceHelper = {
  attach: function(context) { (function($) {

  // Respond to button clicks.
  function quickPick(type) {
  	$('#edit-vaxia-rolls-dice-0-number').val(1);
		$('#edit-vaxia-rolls-dice-0-size').val(100);
		if (type=='melee') {
			$('#edit-vaxia-rolls-dice-1-number').val(1);
			$('#edit-vaxia-rolls-dice-1-size').val(100);
			$('#edit-vaxia-rolls-dice-2-number').val(1);
			$('#edit-vaxia-rolls-dice-2-size').val(100);
			$('#edit-vaxia-rolls-dice-0-stat').val('field_dexterity');
			$('#edit-vaxia-rolls-dice-1-stat').val('field_strength');
			$('#edit-vaxia-rolls-dice-2-stat').val('field_endurance');
		} 
		if (type=='ranged') {
			 $('#edit-vaxia-rolls-dice-1-number').val(1);
			 $('#edit-vaxia-rolls-dice-1-size').val(100);
			 $('#edit-vaxia-rolls-dice-2-number').val(1);
			 $('#edit-vaxia-rolls-dice-2-size').val(100);
			 $('#edit-vaxia-rolls-dice-0-stat').val('field_reflexes');
			 $('#edit-vaxia-rolls-dice-1-stat').val('field_strength');
			 $('#edit-vaxia-rolls-dice-2-stat').val('field_endurance');  
		}
		if (type=='magic') {
			 $('#edit-vaxia-rolls-dice-1-number').val(1);
			 $('#edit-vaxia-rolls-dice-1-size').val(100);
			 $('#edit-vaxia-rolls-dice-2-number').val(0);
			 $('#edit-vaxia-rolls-dice-0-stat').val('field_intelligence');
			 $('#edit-vaxia-rolls-dice-1-stat').val('field_spirituality');
			 $('#edit-vaxia-rolls-dice-2-stat').val(-1);
		}
		if (type=='tech') {
			 $('#edit-vaxia-rolls-dice-1-number').val(1);
			 $('#edit-vaxia-rolls-dice-1-size').val(100);
			 $('#edit-vaxia-rolls-dice-2-number').val(0);
			 $('#edit-vaxia-rolls-dice-0-stat').val('field_intelligence');
			 $('#edit-vaxia-rolls-dice-1-stat').val('field_dexterity');
			 $('#edit-vaxia-rolls-dice-2-stat').val(-1);
		}
		if (type=='aware') {
			 $('#edit-vaxia-rolls-dice-1-number').val(0);
			 $('#edit-vaxia-rolls-dice-2-number').val(0);
			 $('#edit-vaxia-rolls-dice-0-stat').val('field_awareness');
			 $('#edit-vaxia-rolls-dice-1-stat').val(-1);
			 $('#edit-vaxia-rolls-dice-2-stat').val(-1);
		}
	}

	// Respond to skill change
	function sameSkillCheck(id, newSkill) {
		all = $('#same-skill-for-all').attr('checked');
		if (all == true) {
		  $('.dice-skill').val(newSkill);
		}
	}

  // Add toggle buttons, but only the once.
  $('#vaxia-dice-roller').once(function() {
    helper = $('#dice-helper');
    if (helper.length == 0) {
      // On page load, inject the buttons into place in the DOM.
      $('#vaxia-dice-roller').before('<div id="dice-helper" style="display:none;">' +
        '<input type="button" value="Melee" class="dice-helper-button">' +
        '<input type="button" value="Ranged" class="dice-helper-button">' +
        '<input type="button" value="Magic" class="dice-helper-button">' +
        '<input type="button" value="Tech" class="dice-helper-button">' +
        '<input type="button" value="Aware" class="dice-helper-button">' +
        '<input type="checkbox" id="same-skill-for-all" class="dice-helper-select"> Same skill?' +
        '</div>'
      );
    }

    // After creating it, check for the dice-roller values.
    $('#edit-field-comment-character-und').change(function() {
      helper = $(this).val();
      if (helper == '_none') {
        $('#dice-helper').hide();
      }
      else {
        $('#dice-helper').show();
      }
    });

    // Add the listeners.
    $('.dice-helper-button').click(function() {
      quickPick($(this).val().toLowerCase());
    });

    $('.dice-skill').change(function() {
      sameSkillCheck($(this).attr('id'), $(this).val());
    });

    // Once ready, also trigger to display or not display as needed.
    $('#edit-field-comment-character-und').change();

  });

  })(jQuery); }
}
