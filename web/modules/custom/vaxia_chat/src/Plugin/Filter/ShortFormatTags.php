<?php

namespace Drupal\vaxia_chat\Plugin\Filter;

use Drupal\filter\FilterProcessResult;
use Drupal\filter\Plugin\FilterBase;

/**
 * Production's "Short format tags to BBCode" (D7 flexifilter): *text* is
 * bold, \text\ is italic. Runs before BBCode.
 *
 * @Filter(
 *   id = "vaxia_short_format_tags",
 *   title = @Translation("Short format tags to BBCode"),
 *   description = @Translation("Use *text* to mark bold, \text\ to mark italic text."),
 *   type = Drupal\filter\Plugin\FilterInterface::TYPE_MARKUP_LANGUAGE,
 *   weight = -47
 * )
 */
class ShortFormatTags extends FilterBase {

  public function process($text, $langcode) {
    // The same two regexes production ran (flexifilter parts 35 and 36).
    // BBCode list items ([*]) aren't bold markers: production's regex turned
    // "[*]a [*]b" into "[[b]]a [[/b]]b".
    $text = str_replace('[*]', "\x01", $text);
    $text = preg_replace('/\*(.+?)\*/', '[b]$1[/b]', $text);
    $text = preg_replace('/\\\\(.+?)\\\\/', '[i]$1[/i]', $text);
    $text = str_replace("\x01", '[*]', $text);
    return new FilterProcessResult($text);
  }

  public function tips($long = FALSE) {
    return $this->t('Use *text* to mark bold, \text\ to mark italic text.');
  }

}
