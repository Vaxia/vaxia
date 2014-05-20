<?php
/**
  * @file
  * tagcloud.tpl.php
  */
?>  

<?php
  // Initialize variable id unknown to solve any PHP Notice level error messages 
  if (!isset($tagwords)) $tagwords = 0;
?> 

<div id="tagcloud_words">
    <?php print $tagwords ?>
</div>
