<?php

namespace Drupal\vaxia_core\Plugin\Filter;

use Drupal\filter\Attribute\Filter;
use Drupal\filter\FilterProcessResult;
use Drupal\filter\Plugin\FilterInterface;
use Drupal\filter\Plugin\FilterBase;
use Drupal\Core\StringTranslation\TranslatableMarkup;

/**
 * Joins the lines inside <style> blocks, so a later line-break filter
 * (xbbcode, as D7's bbcode "paragraph breaks") doesn't put <br /> into CSS,
 * which breaks the rules after it (the footer block's terms layout).
 */
#[Filter(
  id: 'vaxia_style_block_lines',
  title: new TranslatableMarkup('Keep CSS in style blocks intact'),
  description: new TranslatableMarkup('Joins the lines inside style tags so line-break conversion leaves CSS alone. Put it before BBCode.'),
  type: FilterInterface::TYPE_TRANSFORM_REVERSIBLE,
  weight: -20,
)]
class StyleBlockLines extends FilterBase {

  public function process($text, $langcode) {
    $text = preg_replace_callback('#<style\b[^>]*>.*?</style>#is', fn($m) => preg_replace('/\R+/', ' ', $m[0]), $text);
    return new FilterProcessResult($text);
  }

}
