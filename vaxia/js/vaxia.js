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
      'style="float:right;padding:0.5em 1em;margin:0.25em;">' +
      'show/hide' +
      '</button>');
  });

  // When toggle clicked, switch.
  $('.toggle-rpg-chat-desc').click(function() {
    $('article.rpg-chat-node .field-name-field-artwork').toggle();
    $('article.rpg-chat-node .field-name-body').toggle();
    $('article.rpg-chat-node .field-name-field-parent').toggle();
    $('#block-views-rpg-chats-rpg-chats-children').toggle();
  });

  })(jQuery); }
}
