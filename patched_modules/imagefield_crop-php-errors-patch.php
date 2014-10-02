diff --git a/patched_modules/imagefield_crop/imagefield_crop.module b/patched_modules/imagefield_crop/imagefield_crop.module
index 9e92d76..407473c 100644
--- a/patched_modules/imagefield_crop/imagefield_crop.module
+++ b/patched_modules/imagefield_crop/imagefield_crop.module
@@ -236,7 +236,10 @@ function imagefield_crop_widget_process($element, &$form_state, $form) {
     $file_to_crop = _imagefield_crop_file_to_crop($element['#file']->fid);
 
     $element['cropinfo'] = _imagefield_add_cropinfo_fields($element['#file']->fid);
-    list($res_w, $res_h) = explode('x', $widget_settings['resolution']);
+    $res_w = $res_h = 0;
+    if (!empty($widget_settings['resolution'])) {
+      list($res_w, $res_h) = explode('x', $widget_settings['resolution']);
+    }
     list($crop_w, $crop_h) = explode('x', $widget_settings['croparea']);
 
     $element['preview'] = array(
@@ -328,8 +331,8 @@ function imagefield_crop_widget_preview_process($element, &$form_state, $form) {
       'preview' => array(
         'orig_width' => $image_info['width'],
         'orig_height' => $image_info['height'],
-        'width' => (integer)$width,
-        'height' => (integer)$height,
+        'width' => (integer) isset($width) ? $width : 0,
+        'height' => (integer) isset($height) ? $height : 0,
       ),
     ),
   );
@@ -341,8 +344,8 @@ function imagefield_crop_widget_preview_process($element, &$form_state, $form) {
   );
   $element['#imagefield_crop'] = array(
     '#file' => $element['#file'],
-    '#width' => $width,
-    '#height' => $height,
+    '#width' => isset($width) ? $width : 0,
+    '#height' => isset($height) ? $height : 0,
     '#path' => file_create_url($file->uri),
     );
   return $element;
@@ -359,6 +362,7 @@ function imagefield_crop_widget_value(&$element, &$input, $form_state) {
 
     // get crop and scale info
     $crop = $input['cropinfo'];
+    $scale = NULL
     $instance = field_widget_instance($element, $form_state);
     if ($instance['widget']['settings']['resolution']) {
       list($scale['width'], $scale['height']) = explode('x', $instance['widget']['settings']['resolution']);
