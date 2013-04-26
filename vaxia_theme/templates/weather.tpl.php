<?php
/**
 * @file
 * Template for the weather module.
 */
?>
<div class="weather">
  <style>
    .weather{width:250px;}
    .weather .weather-pic img{width:30px;}
  </style>
  <div class="weather-pic" style="text-align:center;">
    <?php print $weather->image; ?>
  </div>
  <ul>
    <?php if (isset($weather->temperature)): ?>
      <?php if (isset($weather->windchill)): ?>
        <li><?php print t("Temp: !temperature1, windchill !temperature2", array(
          '!temperature1' => $weather->temperature,
          '!temperature2' => $weather->windchill
        )); ?></li>
      <?php else: ?>
        <li><?php print t("Temp: !temperature",
          array('!temperature' => $weather->temperature)); ?></li>
      <?php endif ?>
    <?php endif ?>
    <?php if (isset($weather->rel_humidity)): ?>
      <li><?php print t('Humid: !rel_humidity',
        array('!rel_humidity' => $weather->rel_humidity)); ?></li>
    <?php endif ?>
    <?php if (isset($weather->visibility)): ?>
      <li><?php print t('Vis: !visibility',
        array('!visibility' => $weather->visibility)); ?></li>
    <?php endif ?>
    <?php if (isset($weather->sunrise)): ?>
      <li><?php print $weather->sunrise; ?></li>
    <?php endif ?>
    <?php if (isset($weather->sunset)): ?>
      <li><?php print $weather->sunset; ?></li>
    <?php endif ?>
  </ul>
</div>