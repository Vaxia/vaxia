<?php

namespace Drupal\vaxia_wiki\Plugin\Block;

use Drupal\Core\Block\BlockBase;

/**
 * Wiki page templates a writer can drop into the body.
 *
 * @Block(
 *   id = "site_wiki_templates",
 *   admin_label = @Translation("Site wiki templates")
 * )
 */
class TemplatesBlock extends BlockBase {

  public function build() {
    $items = [];
    foreach ((array) \Drupal::config('vaxia_wiki.settings')->get('site_wiki_templates') as $index => $template) {
      if (!empty($template['template_name'])) {
        $items[] = ['#markup' => '<span class="site_wiki_templates site_wiki_templates_' . (int) $index . '"><span class="site_wiki_template_name"><a href="#">' . htmlspecialchars($template['template_name']) . '</a></span><span class="site_wiki_template" style="display:none;">' . htmlspecialchars((string) $template['template']) . '</span></span>'];
      }
    }
    if (!$items) {
      return [];
    }
    return [
      'list' => ['#theme' => 'item_list', '#title' => $this->t('Wiki templates'), '#items' => $items],
      'help' => ['#markup' => '<div>' . $this->t('These templates are provided to make it easier to quickly load a basic template for wiki articles. If you already have information in the page, or are not using a template feel free to ignore these links.') . '</div>'],
      '#attached' => ['library' => ['vaxia_wiki/templates']],
      '#cache' => ['tags' => ['config:vaxia_wiki.settings']],
    ];
  }

}
