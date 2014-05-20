<?php
/**
  * @file
  * folder_onhover_move.tpl.php
  */
?>  

<div class="foldermovelinks floatright" style="visibility:hidden;padding-right:65px;">
  <span class="foldermovelink"><a href="#" TITLE="<?php print $LANG_moveup ?>" onClick="makeAJAXSetFolderOrder(<?php print $folderid ?>,'up');return false;" style="display:<?php print $hide_moveup ?>;"><img src="<?php print $layout_url ?>/css/images/up.png"></a></span>
  <span class="foldermovelink"><a href="#" TITLE="<?php print $LANG_movedown ?>" onClick="makeAJAXSetFolderOrder(<?php print $folderid ?>,'down');return false;" style="display:<?php print $hide_movedown ?>;"><img src="<?php print $layout_url ?>/css/images/down.png"></a></span>
</div>
<div class="clearboth"></div>