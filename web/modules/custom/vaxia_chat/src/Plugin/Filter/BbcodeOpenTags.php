<?php

namespace Drupal\vaxia_chat\Plugin\Filter;

use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\filter\Attribute\Filter;
use Drupal\filter\FilterProcessResult;
use Drupal\filter\Plugin\FilterBase;
use Drupal\filter\Plugin\FilterInterface;

/**
 * Production's (D7 bbcode 7.x-1.0) unclosed BBCode, rewritten into the closed
 * tags xbbcode parses: [hr] stood alone, [*] ran to the next [*] or [/list],
 * and a post's unclosed [list] and [quote] were closed at its end. Also drops
 * the line breaks D7 kept out of lists and tables. Runs before BBCode.
 */
#[Filter(
  id: 'vaxia_bbcode_open_tags',
  title: new TranslatableMarkup('Production BBCode open tags'),
  description: new TranslatableMarkup('Closes [hr], [*] list items and unclosed [list]/[quote] the way production did. Put it before BBCode.'),
  type: FilterInterface::TYPE_MARKUP_LANGUAGE,
  weight: -48,
)]
class BbcodeOpenTags extends FilterBase {

  public function process($text, $langcode) {
    // D7 replaced exactly "[hr]" (case-sensitive, so wiki links like [[HR]]
    // stay links).
    $text = str_replace('[hr]', '[hr/]', $text);

    // D7 counted openings against closings and appended the missing closings.
    foreach (array('list', 'quote') as $name) {
      $missing = preg_match_all('#\[' . $name . '[=\]]#i', $text) - preg_match_all('#\[/' . $name . '\]#i', $text);
      if ($missing > 0) {
        $text .= str_repeat('[/' . $name . ']', $missing);
      }
    }

    // [*] items: each runs to the next [*] or [/list] of its own list (a
    // nested list stays inside its item). D7 only did this in posts with a
    // [list], where a [*] outside any list still became an item if another
    // [*] or a [list] followed it; any other [*] stayed literal.
    if (stripos($text, '[list') !== FALSE) {
      $parts = preg_split('#(\[list(?:=[^\[\]]*)?\]|\[/list\]|\[\*\])#i', $text, -1, PREG_SPLIT_DELIM_CAPTURE);
      $out = '';
      // Whether each level has an open item; level 0 is outside any list.
      $lists = array(FALSE);
      for ($i = 0; $i < count($parts); $i++) {
        $part = $parts[$i];
        if ($i % 2 === 0) {
          $out .= $part;
        }
        elseif ($part === '[*]') {
          $next = strtolower(substr($parts[$i + 2] ?? '', 0, 5));
          $item = count($lists) > 1 || $next === '[*]' || $next === '[list';
          // Opening tags just before the [*] ("[b][*]Name[/b]") belong to
          // the new item, or they'd cross its [li].
          preg_match('#(?:\[(?!list|li\])[a-z]+(?:=[^\[\]]*)?\]\s*)*$#i', substr($out, -200), $match);
          $moved = $item ? rtrim($match[0]) : '';
          $out = substr($out, 0, strlen($out) - strlen($moved));
          // No line breaks between items (D7 stripped them from list lines).
          if (end($lists)) {
            $out = rtrim($out) . '[/li]';
          }
          elseif ($item) {
            $out = rtrim($out);
          }
          $out .= ($item ? '[li]' : $part) . $moved;
          $lists[count($lists) - 1] = $item;
          if ($item) {
            $parts[$i + 1] = ltrim($parts[$i + 1]);
          }
        }
        elseif (strtolower($part) === '[/list]') {
          if (count($lists) === 1) {
            $out .= $part;
            continue;
          }
          $out = rtrim($out) . (array_pop($lists) ? '[/li]' : '') . $part;
          // A list in an item outside any list ends that item.
          if (count($lists) === 1 && $lists[0]) {
            $out .= '[/li]';
            $lists[0] = FALSE;
          }
          // Nor a line break right after the list.
          $parts[$i + 1] = preg_replace('#^[ \t]*\R#', '', $parts[$i + 1]);
        }
        else {
          // A list nested in an item starts without a line break before it.
          if (end($lists)) {
            $out = rtrim($out);
          }
          $lists[] = FALSE;
          $out .= $part;
        }
      }
      $text = $out;
    }

    // D7 removed the <br /> after table, row and cell tags; without that the
    // line breaks between rows end up as blank lines above the table.
    $text = preg_replace('#(\[(?:table|/?tr|/?th|/?td)(?:[ =][^\[\]]*)?\])\s+#i', '$1', $text);
    // The same for HTML lists and tables written as tags (D7's line breaks
    // skipped block tags; without this each <li> line got a <br />, which
    // doubled the spacing, as in the sidebar's search links block).
    $text = preg_replace('#(<(?:ul|ol|/li|table|/?thead|/?tbody|/?tr|/?th|/?td)\b[^>]*>)\s+#i', '$1', $text);
    $text = preg_replace('#\s+(</(?:ul|ol|table)>|<li\b)#i', '$1', $text);
    $text = preg_replace('#(</(?:ul|ol|table)>)[ \t]*\R#i', '$1', $text);

    return new FilterProcessResult($text);
  }

}
