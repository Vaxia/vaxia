<?php
/**
  * @file
  * tagsearchlink.tpl.php
  */
?>  

<span class="searchtag"><?php print $label ?></span><span class="removetag"><a href="#" onClick="makeAJAXSearchTags('removetag','<?php print $searchtag ?>');return false;" TITLE="<?php print $LANG_removetag ?>">x</a></span>
