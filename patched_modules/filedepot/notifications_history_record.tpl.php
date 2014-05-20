<?php
/**
  * @file
  * notifications_history_record.tpl.php
  */
?>  

<tr class="pluginRow{cssid}">
  <td><?php print $notification_date ?></td>
  <td><?php print $notification_type ?></td>
  <td><?php print $submitter_name ?></td>
  <td><?php print ((isset($file_title)) ? $file_title : $file_name); ?></td>
  <td><a href="<?php print $site_url ?>?q=filedepot&cid=<?php print $cid ?>"><?php print $folder_name ?></a></td>
</tr>
