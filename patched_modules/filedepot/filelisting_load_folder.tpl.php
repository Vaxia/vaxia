<?php
/**
  * @file
  * filelisting_load_folder.tpl.php
  */
?>  

<div class="loading" style="background-color:#FAFAFA;padding-left:<?php print $message_padding ?>px; ">
    <a href="?cid=<?php print $cid ?>&fid=<?php print $fid ?>&fnum=<?php print $foldernumber ?>&level=<?php print $level ?>" class="morefolderdata" TITLE="<?php echo ('There are more files in this folder - click here to retrieve the rest of files'); ?>"><?php echo t('Click here to return rest of files ....'); ?></a>
</div>