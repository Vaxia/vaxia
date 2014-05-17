<?php
/**
  * @file
  * folderperm_record.tpl.php
  */
?>  

<tr align="center" style="border-top:1px solid #CCC;">
  <td width="20%" class="alignleft"><?php print $name ?></td>
  <td><?php print $view_perm ?></td>
  <td><?php print $upload_perm ?></td>
  <td><?php print $uploaddir_perm ?></td>
  <td><?php print $uploadver_perm ?></td>
  <td><?php print $approve_perm ?></td>
  <td><?php print $admin_perm ?></td>
  <td><a class="deleteperm" href="?accid=<?php print $accid ?>"><?php print $LANG_delete ?></a></td>
</tr>