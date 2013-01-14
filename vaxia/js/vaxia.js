/**
 * @file
 * Javascript for Vaxia UI on RPG pages.
 *
 * Allows user to collapse the description as needed.
 */
Drupal.behaviors.vaxiaChat = {
  attach: function(context) { (function($) {

  // Add toggle buttons.
  $('.tabs').once(function() {
    $('.tabs').after('<button class="toggle-rpg-chat-desc button" ' +
      'style="float:right;padding:0.5em 1em;margin:0.25em 4.4em;">' +
      'show/hide' +
      '</button>');
    $('#main #content').after('<button class="toggle-rpg-chat-sidebar button" ' +
      'style="float:left;margin:0.25em -20% 0.25em 0;">' +
      'show/hide' +
      '</button>');
    $('#main .sidebars, #main #content').css('margin-top', '40px');


    // When Chat location toggle clicked, switch.
    $('.toggle-rpg-chat-desc').click(function() {
    if ( $('article.rpg-chat-node .field-name-body').is(':visible') ) {
      $('article.rpg-chat-node .field-name-field-artwork').hide();
      $('article.rpg-chat-node .field-name-body').hide();
      $('article.rpg-chat-node .field-name-field-parent').hide();
      $('#block-views-rpg-chats-rpg-chats-children').hide();
    } else {
      $('article.rpg-chat-node .field-name-field-artwork').show();
      $('article.rpg-chat-node .field-name-body').show();
      $('article.rpg-chat-node .field-name-field-parent').show();
      $('#block-views-rpg-chats-rpg-chats-children').show();
    }
    });
    // When Sidebar toggle clicked, switch.
    $('.toggle-rpg-chat-sidebar').click(function() {
    if ( $('#main .sidebars').is(':visible') ) {
      $('#main .sidebars').hide();
      $('#main #content').css('margin-left', '0%');
      $('#main #content').css('width', '100%');
    } else {
      $('#main .sidebars').show();
      $('#main #content').css('margin-left', '20%');
      $('#main #content').css('width', '80%');
    }
    });
  });

  })(jQuery); }
}
