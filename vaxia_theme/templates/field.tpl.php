<?php
/**
 * Override vaxian item display to hide zero-th fields for items.
 * @file field.tpl.php
 */
// Hide the field if we have no value.
$field_empty = FALSE;
foreach ($items as $delta => $item) {
  if ($element['#object']->type == 'items') {
    if ( ($item['value'] == '0') || ($item['#markup'] == '0') ) {
      $field_empty = TRUE;
    }
  }
}

if (!$field_empty) {
?>
<div class="<?php print $classes; ?> vaxia-theme-custom-field"<?php print $attributes; ?>>
  <?php if (!$label_hidden): ?>
    <div class="field-label"<?php print $title_attributes; ?>><?php print $label ?>:&nbsp;</div>
  <?php endif; ?>
  <div class="field-items"<?php print $content_attributes; ?>>
    <?php foreach ($items as $delta => $item): ?>
      <div class="field-item <?php print $delta % 2 ? 'odd' : 'even'; ?>"<?php print $item_attributes[$delta]; ?>><?php print render($item); ?></div>
    <?php endforeach; ?>
  </div>
</div>
<?php
}
?>

