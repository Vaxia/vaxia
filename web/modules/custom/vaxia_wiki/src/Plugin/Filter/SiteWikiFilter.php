<?php

namespace Drupal\vaxia_wiki\Plugin\Filter;

use Drupal\Core\Url;
use Drupal\filter\FilterProcessResult;
use Drupal\filter\Plugin\FilterBase;

/**
 * Links the first mention of every other published wiki page's title (D7
 * site_wiki filter). D7 swallowed the character after the title ("to
 * Rivermoot." lost its period).
 *
 * @Filter(
 *   id = "site_wiki",
 *   title = @Translation("Site wiki format"),
 *   description = @Translation("Auto adds links to text for wiki content."),
 *   type = Drupal\filter\Plugin\FilterInterface::TYPE_MARKUP_LANGUAGE,
 *   weight = 10
 * )
 */
class SiteWikiFilter extends FilterBase {

  public function process($text, $langcode) {
    $titles = \Drupal::database()->query("SELECT nid, title FROM {node_field_data} WHERE type = 'wiki_page' AND status = 1 ORDER BY CHAR_LENGTH(title) DESC")->fetchAllKeyed();
    foreach ($titles as $nid => $title) {
      $title = trim(str_replace('|', '', $title));
      if ($title === '') {
        continue;
      }
      $link = '<a href="' . Url::fromRoute('entity.node.canonical', ['node' => $nid])->toString() . '">' . htmlspecialchars($title) . '</a>';
      // A whole word or phrase: after a space, newline, tag end or "]"; before a
      // space, punctuation, newline, tag start or "[". Never inside a tag.
      $pattern = '/(?<=[\s>\]]|^)' . preg_quote(htmlspecialchars($title), '/') . '(?=[\s.,?!<\[]|$)(?![^<]*>)/i';
      $new = preg_replace($pattern, $link, $text, 1);
      if ($new !== NULL) {
        $text = $new;
      }
    }
    $result = new FilterProcessResult($text);
    $result->addCacheTags(['node_list:wiki_page']);
    return $result;
  }

}
