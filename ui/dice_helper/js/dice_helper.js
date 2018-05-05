/**
 * @file
 * Javascript for RPG chat UI.
 *
 * Refreshes the AJAX comments on a regular refresh.
 */

Drupal.behaviors.diceHelper = {
  attach: function(context) { (function($) {

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
    all = $('#same-skill-for-all').attr('checked');
    if (all == true) {
      $('.dice-skill').val(newSkill);
    }
  }

  // Toggle dice helper availablity when character changed.
  function setDiceHelper() {
    character_value = $('#edit-field-comment-character-und').val();
    if (character_value == '_none') {
      $('#dice-helper').hide();
      $('#dice-helper-image').hide();
    }
    else {
      $('#dice-helper').show();
      $('#dice-helper-image').show();
    }
    // Check the cookie against the helper value to set pic and color.
    var bkcolor = getCookie('vaxia_dice_helper_' + character_value + '_bkcolor');
    if (typeof bkcolor !== 'undefined' && bkcolor.length > 0) {
      $('#edit-field-background-color-und').val(bkcolor);
    }
    var color = getCookie('vaxia_dice_helper_' + character_value + '_color');
    if (typeof color !== 'undefined' && color.length > 0) {
      $('#edit-field-comment-color-und-0-value').val(color);
    }
    var pic = getCookie('vaxia_dice_helper_' + character_value + '_pic');
    if (typeof pic !== 'undefined' && pic.length > 0) {
      $('body select.form-select-image').msDropDown().data('dd').setIndexByValue(pic);
    }
  }

  // When the image is changed, rotate that in the display.
  function setImageAssist() {
    var image_html = $('#edit-field-artwork-und option:selected').html();
    if (image_html !== null && image_html !== undefined && image_html.length > 0) {
      image_html = image_html.replace(/&lt;/g, '<');
      image_html = image_html.replace(/&gt;/g, '>');
      $('#dice-helper-image').html(image_html);
      image_html = $('#dice-helper-image .views-field-field-artwork-image .field-content').html();
      $('#dice-helper-image').html(image_html);
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
      $('#edit-comment-body').before('<div id="dice-helper-image" style=""></div>');
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
      if (typeof narr !== 'undefined' && narr.length > 0 && narr == 'true') {
        $('#edit-field-comment-narrative-und').attr('checked','checked');
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
    document.cookie = c_name + '=' + c_value;
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
  $('#edit-submit, #edit-1').click(function() {
    var helper = $('#edit-field-comment-character-und').val();
    var bkcolor = $('#edit-field-background-color-und').val();
    setCookie('vaxia_dice_helper_' + helper + '_bkcolor', bkcolor, 30);
    var color = $('#edit-field-comment-color-und-0-value').val();
    setCookie('vaxia_dice_helper_' + helper + '_color', color, 30);
    var pic = $('#edit-field-artwork-und').val();
    setCookie('vaxia_dice_helper_' + helper + '_pic', pic, 30);
    var narr = $('#edit-field-comment-narrative-und').is(':checked');
    setCookie('vaxia_dice_helper_narrative', narr, 30);
  });

  // On ready: This is called at the end of ever ajax done.
  // Hence why we include the unbinds and rebinds.

  // Inject buttons.
  setupButtons();

  // Changing the character dropdown.
  $('#edit-field-comment-character-und').change(function() {
    setDiceHelper();
  });

  // Clicking a quick pick button, trigger auto selects.
  $('.dice-helper-button').unbind('click').click(function() {
    quickPick($(this).val());
  });

  // Changing a skill, check for all skill setting.
  $('.dice-skill').unbind('change').change(function() {
    sameSkillCheck($(this).attr('id'), $(this).val());
  });

  //  Changing the character pic dropdown.
  $('#edit-field-artwork-und').change(function() {
    setImageAssist();
  });

  })(jQuery); }
}
