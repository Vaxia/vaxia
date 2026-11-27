/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */

Drupal.behaviors.diceHelper = {
  attach: function(context) { (function($) {

  // The post form's fields (Drupal 11 names).
  var ARTWORK = 'select[name="field_artwork"]';
  var BKCOLOR = 'select[name="field_background_color"]';
  var COLOR = 'input[name="field_comment_color[0][value]"]';
  var NARRATIVE = 'input[name="field_comment_narrative[value]"]';
  var POST = 'form.comment-form [data-drupal-selector="edit-submit"], [data-drupal-selector="edit-top-post"]';

  var maxSet = 6;
  var lastSet = 0;
  var setsStats = [
    {name: 'attack', desc: 'Attack with melee or ranged weapons.',
      useStats: ['dexterity', 'strength', 'endurance']},
    {name: 'magic', desc: 'Cast a spell or psionic effect.',
      useStats: ['intelligence', 'spirituality']},
    {name: 'tech', desc: 'Work with technology or crafting.',
      useStats: ['intelligence', 'dexterity']},
    {name: 'aware', desc: 'Listen or watch for signifigant things.',
      useStats: ['intelligence', 'spirituality']},
    {name: 'charm', desc: 'Charm a target into cooperation.',
      useStats: ['charisma', 'spirituality']},
    {name: 'threat', desc: 'Intimidate a target into cooperation.',
      useStats: ['charisma', 'spirituality']},
    {name: 'sneak', desc: 'Sneak and hide to avoid detection.',
      useStats: ['intelligence', 'dexterity']}
  ];

  function setPick(number, stat = -1) {
    var selectVal = 1;
    if (stat == -1) {
      selectVal = 0;
    }
    $('.form-item-vaxia-rolls-dice-' + number + '-number select').val(selectVal);
    $('.form-item-vaxia-rolls-dice-' + number + '-size select').val(100);
    $('.form-item-vaxia-rolls-dice-' + number + '-stat select').val(stat);
  }

  // Set quickly picked values.
  function quickPick(type) {
    for (i = 0; i < setsStats.length; i++) {
      if (type == setsStats[i].name) {
        var useStats = setsStats[i].useStats;
        var toSet = useStats.length;
        if ( maxSet < (lastSet + toSet) ) {
          lastSet = 0;
          for (j = 0; j < maxSet; j++) {
            setPick(j, -1);
          }
        }
        for (j = 0; j < toSet; j++) {
          setPick(lastSet + j, useStats[j]);
        }
        lastSet = lastSet + toSet;
      }
    }
  }

  // Set skill if all are to be set.
  function sameSkillCheck(id, newSkill) {
    if ($('#same-skill-for-all').is(':checked')) {
      $('.dice-skill').val(newSkill);
    }
  }

  // The character picked on the post form (Drupal 11 field name; none in
  // Limbo when "- None -" is picked, or for players without characters).
  function pickedCharacter() {
    var $select = $('select[name="field_comment_character"]');
    var value = $select.length ? $select.val() : '';
    return value && value !== '_none' ? value : '';
  }

  // The helper only with a character to roll for, and "Use the same skill
  // for all rolls?" only when that character has skills to pick (user,
  // 2026-11-26). Again after each AJAX request: picking a character
  // rebuilds the dice rows.
  function toggleDiceHelper() {
    var character = pickedCharacter() !== '' && $('#vaxia-dice-character .dice-stat').length > 0;
    $('#dice-helper, #dice-helper-image').toggle(character);
    var skills = $('.dice-skill option').filter(function () {
      return this.value !== '' && this.value !== '-1';
    }).length > 0;
    $('.dice-helper-skill').toggle(character && skills);
    if (!skills) {
      $('#same-skill-for-all').prop('checked', false);
    }
  }

  // Toggle dice helper availablity when character changed.
  function setDiceHelper() {
    var character_value = pickedCharacter();
    toggleDiceHelper();
    // Check the cookie against the helper value to set pic and color.
    if (character_value === '') {
      return;
    }
    var bkcolor = getCookie('vaxia_dice_helper_' + character_value + '_bkcolor');
    if (bkcolor && $(BKCOLOR).find('option[value="' + bkcolor + '"]').length) {
      $(BKCOLOR).val(bkcolor);
    }
    var color = getCookie('vaxia_dice_helper_' + character_value + '_color');
    if (color) {
      $(COLOR).val(color);
    }
    var pic = getCookie('vaxia_dice_helper_' + character_value + '_pic');
    if (pic && $(ARTWORK).find('option[value="' + pic + '"]').length) {
      var dd = $(ARTWORK).data('dd');
      if (dd) {
        dd.setIndexByValue(pic);
      }
      else {
        $(ARTWORK).val(pic);
      }
      setImageAssist();
    }
  }

  // When the image is changed, rotate that in the display.
  // (Thumbnail URLs from vaxia_chat: drupalSettings.vaxiaArtwork.)
  function setImageAssist() {
    var urls = (window.drupalSettings && drupalSettings.vaxiaArtwork) || {};
    var url = urls[$(ARTWORK).val()];
    $('#dice-helper-image').empty();
    if (url) {
      $('#dice-helper-image').append($('<img>').attr({src: url, alt: ''}));
    }
  }

  function makeButton(buttonStat, buttonClasses) {
    return '<input type="button" ' +
      'value="' + buttonStat.name + '" ' +
      'alt="' + buttonStat.name + '" ' +
      'desc="' + buttonStat.desc + '" ' +
      'class="dice-helper-button ' + buttonClasses + ' dice-helper-button-' + buttonStat.name + '">';
  }

  // Add toggle buttons, but only the once.
  function setupButtons() {
    helper = $('#dice-helper');
    if (helper.length == 0) {
      // On page load, inject the buttons into place in the DOM.
      $('#edit-comment-body-wrapper').before('<div id="dice-helper-image"></div>');
      var setsButtons = '';
      for (i = 0; i < setsStats.length; i++) {
        setsButtons = setsButtons + makeButton(setsStats[i], 'dice-helper-icon');
      }
      $('#vaxia-dice-character').before('' +
        '<div id="dice-helper">' +
          '<div class="dice-helper-sets">' + setsButtons + '</div>' +
          '<div class="dice-helper-help dice-helper-icon-help" style="display:none;"></div>' +
          '<div class="dice-helper-skill">' +
            '<input type="checkbox" id="same-skill-for-all" class="dice-helper-select">' +
            'Use the same skill for all rolls?' +
          '</div>' +
        '</div>'
      );
      // Add hovers to the newly injected buttons.
      $('.dice-helper-button').hover(
        function(){
          $('.dice-helper-icon-help').html($(this).attr('desc')).show();
        },
        function(){
          $('.dice-helper-icon-help').hide();
        }
      );
      // Check the cookie against the helper value to set pic and color.
      var narr = getCookie('vaxia_dice_helper_narrative');
      if (narr === 'true') {
        $(NARRATIVE).prop('checked', true);
      }
      // Check the dice helper on load.
      setDiceHelper();
      // Set the image on display.
      setImageAssist();
    }
  }

  // Cookie handling, set cookie.
  function setCookie(c_name, value, exdays) {
    var exdate = new Date();
    exdate.setDate(exdate.getDate() + exdays);
    var c_value = escape(value) + ((exdays == null) ? '' : '; expires=' + exdate.toUTCString());
    // Site-wide, so every room remembers the same settings.
    document.cookie = c_name + '=' + c_value + '; path=/; SameSite=Lax';
  }

  // Cookie handling, get cookie.
  function getCookie(c_name) {
    var i, x, y, ARRcookies = document.cookie.split(';');
    for (i=0; i < ARRcookies.length; i++) {
      x = ARRcookies[i].substr(0, ARRcookies[i].indexOf('='));
      y = ARRcookies[i].substr(ARRcookies[i].indexOf('=') + 1);
      x = x.replace(/^\s+|\s+$/g, '');
      if (x == c_name) {
        return unescape(y);
      }
    }
    return 0;
  }

  // Save the color and pic on post.
  // (Mousedown too: Drupal's AJAX buttons submit on mousedown.)
  $(POST).off('.diceHelperSave').on('mousedown.diceHelperSave click.diceHelperSave', function() {
    var helper = pickedCharacter();
    if (helper !== '') {
      if ($(BKCOLOR).length) {
        setCookie('vaxia_dice_helper_' + helper + '_bkcolor', $(BKCOLOR).val(), 30);
      }
      if ($(COLOR).length) {
        setCookie('vaxia_dice_helper_' + helper + '_color', $(COLOR).val(), 30);
      }
      if ($(ARTWORK).length) {
        setCookie('vaxia_dice_helper_' + helper + '_pic', $(ARTWORK).val(), 30);
      }
    }
    if ($(NARRATIVE).length) {
      setCookie('vaxia_dice_helper_narrative', $(NARRATIVE).is(':checked'), 30);
    }
  });

  // On ready: This is called at the end of ever ajax done.
  // Hence why we include the unbinds and rebinds.

  // Inject buttons.
  setupButtons();

  // Changing the character dropdown.
  $('select[name="field_comment_character"]').off('change.diceHelper').on('change.diceHelper', function() {
    setDiceHelper();
  });
  toggleDiceHelper();

  // Clicking a quick pick button, trigger auto selects.
  $('.dice-helper-button').unbind('click').click(function() {
    quickPick($(this).val());
  });

  // Changing a skill, check for all skill setting.
  $('.dice-skill').unbind('change').change(function() {
    sameSkillCheck($(this).attr('id'), $(this).val());
  });

  //  Changing the character pic dropdown.
  $(ARTWORK).off('change.diceHelper').on('change.diceHelper', function() {
    setImageAssist();
  });

  })(jQuery); }
}
