<?php

namespace Drupal\vaxia_core;

use Drupal\Core\Session\AccountInterface;

/**
 * Display preferences: production's per-user header blocks (D7 block
 * visibility "users may choose"), each a few lines of CSS. Keys are the D7
 * custom block ids, which the account form keeps as element names
 * (block[14]) and the data migration carries over from users.data.
 */
class DisplayPrefs {

  /**
   * Block id => [label, CSS, on unless the user turns it off].
   */
  public const OPTIONS = [
    14 => ['Display chat posts in scrollable box', '#rpg-chat{height:900px;overflow-y:scroll;}', TRUE],
    20 => ['Hide dice assistant buttons', '.dice_rule{display:none;}', FALSE],
    4 => ['Display using larger font (10% larger)', 'body{font-size:110%;} .dice_rule{font-size:90%;}', FALSE],
    18 => ['Display using larger font (20% larger)', 'body{font-size:120%;} .dice_rule{font-size:80%;}', FALSE],
    37 => ['Use smaller character images in chat', '.rpg-chat-node article.comment header .submitted{width:100px;}', FALSE],
    16 => ['Override text colors in chatrooms w/ grey - high contrast', '#rpg-chat article.comment{background-color:#555555 !important;} #rpg-chat article.comment span{color:#FFFFFF !important;}', FALSE],
    29 => ['Override text colors in chatrooms w/ white - high contrast', '#rpg-chat article.comment{background-color:#FFF !important;} #rpg-chat article.comment.odd{background-color:#EEE !important;} #rpg-chat article.comment span{color:#111 !important;}', FALSE],
    15 => ['Use black background - high contrast', "body{background-image:url('');background:#111111;}", FALSE],
    17 => ['Use blue background - high contrast', "body{background-image:url('');background:#111166;}", FALSE],
    28 => ['Use white background - high contrast', "body{background-image:url('');color:#111;background:#FFF;} #header{background:#EEE;} a{color:#4444EE !important;} a:visited{color:#4444AA !important;} #forum-comments div.forum-post.odd{background:#EEE;} .author-badges{background:#CCC;} .jsPullout, .jsPulloutFlap{background:#CCC;} .jsPulloutContent{color:#111;background:#FFF;border:1px solid #111;}", FALSE],
  ];

  /**
   * The block ids switched on for an account, in display order.
   */
  public static function enabled(AccountInterface $account): array {
    $saved = $account->isAuthenticated() ? (\Drupal::service('user.data')->get('vaxia_core', $account->id(), 'display_prefs') ?: []) : [];
    $enabled = [];
    foreach (self::OPTIONS as $bid => [, , $default]) {
      if (isset($saved[$bid]) ? (bool) $saved[$bid] : $default) {
        $enabled[] = $bid;
      }
    }
    return $enabled;
  }

}
