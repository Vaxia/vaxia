<?php
/**
 * @file
 * Theme implementation to display information about the post/profile author.
 *
 * See author-pane.tpl.php in Author Pane module for a full list of variables.
 */
?>

<?php
  // This bit of debugging info will show the full path to and name of this
  // template file to make it easier to figure out which template is
  // controlling which author pane.
  if (!empty($show_template_location)) {
    print __FILE__;
  }
?>

<div class="author-pane">
 <div class="author-pane-inner">
    <?php /* General section */ ?>
    <div class="author-pane-section author-pane-general">
      <?php /* Account name */ ?>
      <div class="author-pane-line author-name" style="font-weight:bold;font-size:110%;">
        <?php print $account_name; ?>
      </div>

      <?php /* User picture / avatar (has div in variable) */ ?>
        <?php
          $artwork_image = file_load($account->field_artwork_image['und'][0]['fid']);
          $default_filepath = drupal_get_path('module', 'vaxia') . '/images/vaxialogo.gif';
          $filepath = isset($artwork_image->uri) ? $artwork_image->uri : $default_filepath;
          $alt = t("@user's picture.", array('@user' => $account->name));
          // If the image does not have a valid Drupal scheme (for eg. HTTP) don't load image styles.
          if (module_exists('image') && file_valid_uri($filepath) && $style = variable_get('user_picture_style', '')) {
            $picture = theme('image_style', array('style_name' => $style, 'path' => $filepath, 'alt' => $alt, 'title' => $alt));
          }
          else {
            $picture = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt));
          }
          if (!empty($picture)) {
            echo $picture;
          }
        ?>

      <?php /* User badges */ ?>
      <?php if (!empty($user_badges)): ?>
        <div class="author-pane-line author-badges">
          <?php print $user_badges; ?>
        </div>
      <?php endif; ?>

      <?php /* Last active */ ?>
      <?php if (!empty($last_active)): ?>
        <div class="author-pane-line">
           <div class="author-pane-label"><b><?php print t('Last online'); ?></b></div>
           <?php print t('!time ago', array('!time' => $last_active)); ?>
        </div>
      <?php endif; ?>

    </div>

    <?php /* Contact section */ ?>
    <div class="author-pane-section author-pane-contact">

      <?php /* Private message */ ?>
      <?php if (!empty($privatemsg)): ?>
        <div class="author-pane-line author-pane-link-line author-privatemsg">
          <?php print $privatemsg; ?>
        </div>
      <?php endif; ?>
    </div>

    <?php /* Admin section */ ?>
    <div class="author-pane-section author-pane-admin">
      <?php /* IP */ ?>
      <?php if (!empty($user_stats_ip)): ?>
        <div class="author-pane-line author-ip">
          <span class="author-pane-label"><?php print t('IP'); ?></br></span> <?php print $user_stats_ip; ?>
        </div>
      <?php endif; ?>

     <?php /* Fasttoggle block */ ?>
     <?php if (!empty($fasttoggle_block_author)): ?>
        <div class="author-fasttoggle-block"><?php print $fasttoggle_block_author; ?></div>
      <?php endif; ?>
    </div>
  </div>
</div>
