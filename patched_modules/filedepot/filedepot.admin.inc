<?php

/**
 * @file
 * filedepot.admin.inc
 * Drupal API functions for the Module Admin Settings
 */

/**
 * Return an array of permission names matched to the machine name for that permission.
 */
function _filedepot_get_permission_name_array() {
  static $s = array(
    'view' => 'view_folder',
    'approval' => 'upload_admin',
    'admin' => 'folder_admin',
    'upload' => 'upload_with_approval',
    'upload_dir' => 'upload_direct',
    'upload_ver' => 'upload_versions'
  );

  return $s;
}

/**
 * Convert a permission string (upload, approval) to the display names and back again
 * @param string  $perm_string
 * @param boolean $convert_to_display            True to convert from machine name to display name, false to convert from display name to machine name
 */
function _filedepot_convert_permission_string($perm_string, $convert_to_display = TRUE) {
  $perm_names = _filedepot_get_permission_name_array();
  $perm_array = explode(',', $perm_string);

  if ($convert_to_display === FALSE) {
    $perm_names = array_flip($perm_names);
  }

  for($i = 0; $i < count($perm_array); $i++) {
    $perm_array[$i] = $perm_names[trim($perm_array[$i])];
  }

  $replaced_perm_string = implode(', ', $perm_array);
  return $replaced_perm_string;
}

/**
 * Convert a multi-line (user: permissions) permission string (upload, approval) to the display names and back again
 * @param string  $perm_string
 * @param boolean $convert_to_display            True to convert from machine name to display name, false to convert from display name to machine name
 */
function _filedepot_convert_permission_string_lines($perm_string, $convert_to_display = TRUE) {
  $lines = preg_split( '/\r\n|\r|\n/', $perm_string);
  for($i = 0; $i < count($lines); $i++) {
    if (!empty($lines[$i])) {
      $split_line = explode(":", $lines[$i]);
      $split_line[1] = _filedepot_convert_permission_string($split_line[1], $convert_to_display);
      $lines[$i] = implode(": ", $split_line);
    }
  }

  return implode("\n", $lines);
}

function filedepot_admin_settings() {

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Base Setup'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['basic']['filedepot_yui_baseurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL to load YUI Javascript Libraries'),
    '#description' => t('Valid URL to load libraries - can be a local location or hosted CDN like Yahoo and Google.<br />Site URL needs to end in a /'),
    '#default_value' => variable_get('filedepot_yui_baseurl', 'http://yui.yahooapis.com/2.7.0/build/'),
    '#size' => 60,
  );

  $form['performance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performance Settings'),
    '#description' => t('With very large document repositories, the file listing logic can be optimized by tuning these options for the 3 pass logic.')
  . t('On initial pass, display a smaller number of files like 2 while all the folders are being generated.')
  . t('Once the initial folder listing is generated, the 2nd pass will execute in the background and populate all folders with more files up the the number set for the pass 2 setting.')
  . t('If there are more files in a folder then the pass 2 setting, the user will have a link to return rest (pass 3). This is so a folder with 1000 files does not hang up the application initial view if this folder is not even expanded.')
  . '<br />' . t('For smaller repositories, you can set Pass 1 to 50 and Pass 2 to 100 for example.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['performance']['filedepot_pass1_recordcount'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of folder records to initially show in pass1'),
    '#description' => t('While displaying the folder, initially return this number of records.'),
    '#default_value' => variable_get('filedepot_pass1_recordcount', 2),
    '#size' => 5,
  );

  $form['performance']['filedepot_pass2_recordcount'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of folder records to display in background during pass2'),
    '#description' => t('After initially displaying folder structure and files from pass1, background AJAX requests will populate display with more records.'),
    '#default_value' => variable_get('filedepot_pass2_recordcount', 10),
    '#size' => 5,
  );

  $form['notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notification Defaults'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );


  $domain = preg_replace('`^www.`', '', $_SERVER['HTTP_HOST']);
  $default_to_email = "noreply@$domain";
  $form['notifications']['filedepot_email_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Default To email address'),
    '#description' => t('Notification emails will be sent to users using Bcc to hide the recipients but we still need a To: email address.'),
    '#default_value' => variable_get('filedepot_email_to', $default_to_email),
    '#size' => 50,
  );

  $form['notifications']['filedepot_notifications_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notifications Enabled'),
    '#default_value' => variable_get('filedepot_notifications_enabled', 1),
    '#description' => t('If enabled, email notifications for new files added or changed will be sent out.'),
  );

  $form['notifications']['filedepot_default_allow_broadcasts'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Admin Broadcasts'),
    '#default_value' => variable_get('filedepot_default_allow_broadcasts', 1),
    '#description' => t('If enabled, members with filedepot admin permission will be able to send out broadcast notifications for a file.'),
  );

  $form['notifications']['filedepot_default_notify_newfile'] = array(
    '#type' => 'checkbox',
    '#title' => t('New Files'),
    '#default_value' => variable_get('filedepot_default_notify_newfile', 0),
    '#description' => t('If enabled, users with view access to a folder will be notified of new files being added.'),
  );

  $form['notifications']['filedepot_default_notify_filechange'] = array(
    '#type' => 'checkbox',
    '#title' => t('File Changes'),
    '#default_value' => variable_get('filedepot_default_notify_filechange', 0),
    '#description' => t('If enabled, users with view access to a folder will be notified of new file versions being added.'),
  );

  if (module_exists('og') AND module_exists('og_access')) {
    $form['og_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Organic Group Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
    );

    $form['og_options']['filedepot_auto_create_group_rootfolder_enabled'] = array('#type' => 'checkbox',
    '#title' => t('Automatically Create Root Level Folder for New Organic Groups'),
    '#default_value' => variable_get('filedepot_auto_create_group_rootfolder_enabled', 0),
    '#description' => t('If enabled, a new top level ROOT folder will be created for the organic group.'));

    $form['og_options']['filedepot_organic_group_mode_enabled'] = array('#type' => 'checkbox',
    '#title' => t('Only Display Organic Group Root Level Folder'),
    '#default_value' => variable_get('filedepot_organic_group_mode_enabled', 0),
    '#description' => t('If enabled and the group context is set then only folders under the Group Root level folder will be displayed.'));
  }

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Misc Options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['options']['filedepot_locked_file_download_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow files to be downloaded if locked'),
    '#default_value' => variable_get('filedepot_locked_file_download_enabled', 1),
    '#description' => t('If enabled, users will be able to download a file when locked.'),
  );

  $form['options']['filedepot_show_index_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the index numbers in the listing of folders and files'),
    '#default_value' => variable_get('filedepot_show_index_enabled', 0),
    '#description' => t('If enabled, the display will show the index numbers before folder and file names.'),
  );

  $form['options']['filedepot_show_recent_folders'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display the recent folders listing'),
    '#default_value' => variable_get('filedepot_show_recent_folders', 1),
    '#description' => t('If enabled, the display will show the recent folders entry in the left navigation panel of Filedepot.'),
  );

  $form['options']['filedepot_displayorder_filesfirst'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display files before folders'),
    '#default_value' => variable_get('filedepot_displayorder_filesfirst', 1),
    '#description' => t('If enabled, the display will show files before folders in Filedepot. If disabled, will show folders first.'),
  );

  $form['options']['filedepot_override_folderorder'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use alphabetic ordering instead of the default settings'),
    '#default_value' => variable_get('filedepot_override_folderorder', 1),
    '#description' => t('If enabled, all folders and files will be sorted alphabetically overriding the default settings. Default is to provide an admin setting for the folder order and files displayed in descending date order.'),
  );

  $form['filetype_filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Type Filtering and Extension Mapping'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $default_filter  = 'jpg png doc docx xls xlsx pdf ppt pptx';
  $form['filetype_filter']['filedepot_filetype_filter'] = array(
    '#type' => 'textarea',
    '#title' => t('File types to allow'),
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' => variable_get('filedepot_filetype_filter', $default_filter),
    '#description' => t('Separate extensions with a space or comma and do not include the leading dot.'),
    '#wysiwyg' => FALSE,
  );

  $default_extension_mapping  = 'jpg=jpg.gif, gif=gif.gif, bmp=bmp.gif, doc=doc.gif, docx=doc.gif, xls=xls.gif, xlsx=xls.gif' . "\n";
  $default_extension_mapping .= 'ppt=ppt.gif, pptx=ppt.gif, mpp=mpp.gif, pdf=pdf.gif, txt=txt.gif, zip=zip.gif' . "\n";

  $form['filetype_filter']['filedepot_extensions'] = array(
    '#type' => 'textarea',
    '#title' => t('Extension Mapping'),
    '#cols' => 60,
    '#rows' => 3,
    '#default_value' => variable_get('filedepot_extensions', $default_extension_mapping),
    '#description' => t('This is a comma sepearated list of file type to icon image files. This is used in the filelisting logic to associate an icon to file type uploaded. We don\'t want to assume that all icons will be in gif format.'),
    '#wysiwyg' => FALSE,
  );

  $perm_names = _filedepot_get_permission_name_array();

  $form['permissions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Permissions'),
    '#description' => t('Folders may have sub-folders which have more restrictive or different permissions. Permissions can be assigned by user and/or by role.')
  . '<br />' . t('Available Permissions are:')
  . '<ul>'
  . '<li><strong>' . $perm_names['view'] . '</strong>: '
  . t('Able to view the folder and files within. If you can view the files, you can download them.')
  . '</li>'
  . '<li><strong>' . $perm_names['admin'] . '</strong>: '
  . t('Full admin permissions to folder and files within folder. Able to change folder permissions.')
  . '</li>'
  . '</li>'
  . '<li><strong>' . $perm_names['upload_dir'] . '</strong>: '
  . t('Able to upload files directly to folder with no moderation.')
  . '</li>'
  . '</li>'
  . '<li><strong>' . $perm_names['upload'] . '</strong>: '
  . t('Able to upload files but they will need to be approved before appearing in folder listing.')
  . '</li>'
  . '</li>'
  . '<li><strong>' . $perm_names['upload_ver'] . '</strong>: '
  . t('Able to upload new versions to existing files listed in the folder.')
  . '</li>'
  . '</li>'
  . '<li><strong>' . $perm_names['approval'] . '</strong>: '
  . t('Able to moderate (approve or reject) new file submissions.')
  . '</li>'
  . '</ul>',
    //li, admin, upload, upload_dir, upload_ver, approval'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $perm_string = variable_get('filedepot_default_owner', 'view, admin, upload_dir, upload_ver, approval');
  $replaced_perm_string = _filedepot_convert_permission_string($perm_string, TRUE);

  $form['permissions']['filedepot_default_owner'] = array(
    '#type' => 'textarea',
    '#title' => t('Default Owner Permissions'),
    '#cols' => 60,
    '#rows' => 1,
    '#default_value' => $replaced_perm_string,
    '#description' => t('When a site member creates a new folder, list the default user based permissions the folder should be setup with.'),
    '#wysiwyg' => FALSE,
  );

  $perm_string = variable_get('filedepot_default_group', 'view, upload_dir');
  $replaced_perm_string = _filedepot_convert_permission_string($perm_string, TRUE);

  $form['permissions']['filedepot_default_group'] = array(
  '#type' => 'textarea',
  '#title' => t('Default OG Permissions'),
  '#cols' => 60,
  '#rows' => 1,
  '#default_value' => $replaced_perm_string,
  '#description' =>
  t('When a new group is created and OG mode is enabled, list the default user based permissions the folder should be setup with.'),
  '#wysiwyg' => FALSE,
  );

  $perm_string = variable_get('filedepot_default_roles', 'authenticated user: view, upload');
  $replaced_perm_string = _filedepot_convert_permission_string_lines($perm_string, TRUE);

  $form['permissions']['filedepot_default_roles'] = array(
    '#type' => 'textarea',
    '#title' => t('Default Role Permissions'),
    '#cols' => 60,
    '#rows' => 3,
    '#default_value' => $replaced_perm_string,
    '#description' => t('Default role based permissions the folder should be setup with. Enter additional mappings of roles to permissions on new lines'),
    '#wysiwyg' => FALSE,
  );

  // Check that private file stream is setup
  $streams = file_get_stream_wrappers();
  if (!array_key_exists('private', $streams)) {
    drupal_set_message(t('Warning: Your private file system path is not setup and is required - admin/config/media/file-system'));
    watchdog('Warning', 'Your private file system path is not setup and is required for the filedepot module');
  }

  return system_settings_form($form);
}


function filedepot_admin_settings_submit($form, &$form_state) {
  $f = 10;
}

function filedepot_admin_settings_validate($form, &$form_state) {

  if (!valid_email_address($form_state['values']['filedepot_email_to'])) {
    form_set_error('filedepot_email_to', t('The email address appears to be invalid.'));
  }
  $filterdata = array();
  $rawfilefilter = $form_state['values']['filedepot_filetype_filter'];
  $filterlines = preg_split( '/\r\n|\r|\n/', $rawfilefilter );
  variable_set('filedepot_filetype_filterdata', $filterlines);

  $extensiondata = array();
  $rawextensions = $form_state['values']['filedepot_extensions'];
  $extensionlines = preg_split( '/\r\n|\r|\n/', $rawextensions );
  foreach ($extensionlines as $line) {
    if (!empty($line)) {
      $records = explode(',', $line);
      foreach ($records as $mapping) {
        $data = explode('=', $mapping);
        $ext = trim($data[0]);
        $icon = trim($data[1]);
        $extensiondata[$ext] = $icon;
      }
    }
  }
  variable_set('filedepot_extension_data', serialize($extensiondata));

  /* Parse the default perms settings and create a single associative array the filedepot class will use */
  $validperms = array('view', 'admin', 'upload', 'upload_dir', 'upload_ver', 'approval');
  $defaultperms = array();
  $cleanperms = array();
  $ownerperms = _filedepot_convert_permission_string($form_state['values']['filedepot_default_owner'], FALSE);
  $form_state['values']['filedepot_default_owner'] = $ownerperms;
  $aperms = explode(',', $ownerperms);
  foreach ($aperms as $perm) {
    $perm = trim($perm);
    if (in_array($perm, $validperms)) {
      $cleanperms[] = $perm;
    }
  }
  $defaultperms['owner'] = $cleanperms;

  $cleanperms = array();
  $groupperms = _filedepot_convert_permission_string($form_state['values']['filedepot_default_group'], FALSE);
  $form_state['values']['filedepot_default_group'] = $groupperms;
  $aperms = explode(',', $groupperms);
  foreach ($aperms as $perm) {
    $perm = trim($perm);
    if (in_array($perm, $validperms)) {
      $cleanperms[] = $perm;
    }
  }
  $defaultperms['group'] = $cleanperms;

  $rawroleperms = _filedepot_convert_permission_string_lines($form_state['values']['filedepot_default_roles'], FALSE);
  $form_state['values']['filedepot_default_roles'] = $rawroleperms;
  $permlines = preg_split( '/\r\n|\r|\n/', $rawroleperms );
  foreach ($permlines as $line) {
    if (!empty($line)) {
      $parts = explode(':', $line);
      $role = trim($parts[0]);
      $perms = explode(',', $parts[1]);
      foreach ($perms as $perm) {
        $perm = trim($perm);
        if (in_array($perm, $validperms)) {
          $defaultperms[$role][] = trim($perm);
        }
      }
    }
  }
  variable_set('filedepot_default_perms_data', serialize($defaultperms));

}
