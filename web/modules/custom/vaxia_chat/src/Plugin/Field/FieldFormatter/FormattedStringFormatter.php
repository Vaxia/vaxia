<?php

namespace Drupal\vaxia_chat\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\Attribute\FieldFormatter;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\FormatterBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;

/**
 * Plain text fields shown through a text format. D7's chat OOC field (and the
 * ban reason) were formatted text, so BBCode in them rendered; the port keeps
 * them as plain strings, which showed the tags raw. D7 stored nearly all OOC
 * text as plain_text or wiki with no wiki links, so one format fits.
 */
#[FieldFormatter(
  id: 'vaxia_formatted_string',
  label: new TranslatableMarkup('Through a text format'),
  field_types: ['string', 'string_long'],
)]
class FormattedStringFormatter extends FormatterBase {

  public static function defaultSettings() {
    return ['format' => 'plain_text'] + parent::defaultSettings();
  }

  public function settingsForm(array $form, FormStateInterface $form_state) {
    $options = [];
    foreach (filter_formats() as $id => $format) {
      $options[$id] = $format->label();
    }
    return [
      'format' => ['#type' => 'select', '#title' => $this->t('Text format'), '#options' => $options, '#default_value' => $this->getSetting('format')],
    ];
  }

  public function settingsSummary() {
    return [$this->t('Format: @format', ['@format' => $this->getSetting('format')])];
  }

  public function viewElements(FieldItemListInterface $items, $langcode) {
    $elements = [];
    foreach ($items as $delta => $item) {
      $elements[$delta] = ['#type' => 'processed_text', '#text' => (string) $item->value, '#format' => $this->getSetting('format'), '#langcode' => $item->getLangcode()];
    }
    return $elements;
  }

}
