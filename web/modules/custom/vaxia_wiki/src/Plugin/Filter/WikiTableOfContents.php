<?php

namespace Drupal\vaxia_wiki\Plugin\Filter;

use Drupal\Component\Utility\Html;
use Drupal\filter\FilterProcessResult;
use Drupal\filter\Plugin\FilterBase;

/**
 * A table of contents on wiki pages (D7 simplewiki's "autotoc"): headings
 * (h2-h4) get anchors, and pages with three or more headings get a contents
 * list at the top.
 *
 * @Filter(
 *   id = "vaxia_wiki_toc",
 *   title = @Translation("Wiki table of contents"),
 *   description = @Translation("Adds a table of contents to wiki pages with three or more headings."),
 *   type = Drupal\filter\Plugin\FilterInterface::TYPE_MARKUP_LANGUAGE,
 *   weight = 20
 * )
 */
class WikiTableOfContents extends FilterBase {

  public function process($text, $langcode) {
    if (!preg_match_all('#<h([2-4])([^>]*)>(.*?)</h\1>#is', $text, $matches, PREG_SET_ORDER) || count($matches) < 3) {
      return new FilterProcessResult($text);
    }
    $items = '';
    $used = [];
    foreach ($matches as $match) {
      $label = trim(strip_tags($match[3]));
      $id = Html::getClass($label) ?: 'section';
      $base = $id;
      for ($n = 2; isset($used[$id]); $n++) {
        $id = $base . '-' . $n;
      }
      $used[$id] = TRUE;
      $heading = '<h' . $match[1] . $match[2] . ' id="toc-' . $id . '">' . $match[3] . '</h' . $match[1] . '>';
      $text = preg_replace('#' . preg_quote($match[0], '#') . '#', $heading, $text, 1);
      $items .= '<li class="toc-level-' . $match[1] . '"><a href="#toc-' . $id . '">' . Html::escape($label) . '</a></li>';
    }
    $toc = '<nav class="wiki-toc"><div class="wiki-toc-title">' . $this->t('Contents') . '</div><ul>' . $items . '</ul></nav>';
    return new FilterProcessResult($toc . $text);
  }

}
