/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */

Drupal.behaviors.diceHelper = {
  attach: function(context) { (function($) {

  // Cookie settings.
  function setCookie(c_name, value, exdays) {
    var exdate=new Date();
    exdate.setDate(exdate.getDate() + exdays);
    var c_value=escape(value) + ((exdays==null) ? "" : "; expires="+exdate.toUTCString());
    document.cookie=c_name + "=" + c_value;
  }

  function getCookie(c_name) {
    var i,x,y,ARRcookies=document.cookie.split(";");
    for (i=0;i<ARRcookies.length;i++) {
      x=ARRcookies[i].substr(0,ARRcookies[i].indexOf("="));
      y=ARRcookies[i].substr(ARRcookies[i].indexOf("=")+1);
      x=x.replace(/^\s+|\s+$/g,"");
      if (x==c_name){
        return unescape(y);
      }
    }
    return 0;
  }

  // Respond to button clicks.
  function quickPick(type) {
  	$('#edit-vaxia-rolls-dice-0-number').val(1);
		$('#edit-vaxia-rolls-dice-0-size').val(100);
		if (type=='melee' || 'ranged') {
			$('#edit-vaxia-rolls-dice-1-number').val(1);
			$('#edit-vaxia-rolls-dice-1-size').val(100);
			$('#edit-vaxia-rolls-dice-2-number').val(1);
			$('#edit-vaxia-rolls-dice-2-size').val(100);
			$('#edit-vaxia-rolls-dice-0-stat').val('field_dexterity');
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

  // Set the value in the form based on the given character index.
  function setCharacterColorNPic(character_value) {
    // Check the cookie against the helper value to set pic and color.
    var color = getCookie('vaxia_dice_helper_' + character_value + '_color');
    if (typeof color !== 'undefined' && color.length > 0) {
      $('#edit-field-comment-color-und-0-value').val(color);
    }
    var pic = getCookie('vaxia_dice_helper_' + character_value + '_pic');
    if (typeof pic !== 'undefined' && pic.length > 0) {
      $('body select.form-select-image').msDropDown().data('dd').setIndexByValue(pic);
    }
  }

  // Add toggle buttons, but only the once.
  $('#vaxia-dice-roller').once(function() {
    helper = $('#dice-helper');
    if (helper.length == 0) {
      // On page load, inject the buttons into place in the DOM.
      $('#vaxia-dice-roller').before('<div id="dice-helper" style="">' +
        '<input type="button" value="Melee" class="dice-helper-button">' +
        '<input type="button" value="Ranged" class="dice-helper-button">' +
        '<input type="button" value="Magic" class="dice-helper-button">' +
        '<input type="button" value="Tech" class="dice-helper-button">' +
        '<input type="button" value="Aware" class="dice-helper-button">' +
        '<input type="checkbox" id="same-skill-for-all" class="dice-helper-select"> Same skill?' +
        '</div>'
      );
      helper = $('#edit-field-comment-character-und').val();
      if (helper == '_none') {
        $('#dice-helper').hide();
      }
      else {
        $('#dice-helper').show();
      }
      setCharacterColorNPic(helper);
    }
  });

  // Add the listeners.
  $('#edit-field-comment-character-und').change(function() {
    helper = $(this).val();
    if (helper == '_none') {
      $('#dice-helper').hide();
    }
    else {
      $('#dice-helper').show();
    }
    setCharacterColorNPic(helper);
  });

  // Save the color and pic on post.
  $('#edit-submit, #edit-1').click(function() {
    var helper = $('#edit-field-comment-character-und').val();
    var color = $('#edit-field-comment-color-und-0-value').val();
    setCookie('vaxia_dice_helper_' + helper + '_color', color, 30);
    var pic = $('#edit-field-artwork-und').val();
    setCookie('vaxia_dice_helper_' + helper + '_pic', pic, 30);
  });

  $('.dice-helper-button').click(function() {
    quickPick($(this).val().toLowerCase());
  });

  $('.dice-skill').change(function() {
    sameSkillCheck($(this).attr('id'), $(this).val());
  });

  })(jQuery); }
}
