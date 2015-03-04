<?php
/**
  * @file
  * toolbar_form.tpl.php
  */
?>

<?php
  // Initialize variable id unknown to solve any PHP Notice level error messages
  if (!isset($current_category)) $current_category = 0;
  if (!isset($base_url)) $base_url = '';
  if (!isset($report_option)) $report_option = '';
?>
<div style="float:left;padding-left:0px;margin-top:-2px">
  <form name="frmtoolbar" action="#" method="post" style="margin:0px;">
    <input type="hidden" name="checkeditems" value="">
    <input type="hidden" name="checkedfolders" value="">
    <input type="hidden" name="cid" value="<?php print $current_category ?>">
    <input type="hidden" name="newcid" value="">
    <input type="hidden" name="reportmode" value="<?php print $report_option ?>">
    <div class="floatleft" style="padding-top:5px;width:160px;">
      <select id="multiaction" class="form-select disabled_element" name="multiaction" style="padding:0px;width:100%;" onChange="if (checkMultiAction(this.value)) submit(); postSubmitMultiactionResetIfNeed(this.value);" disabled="disabled"><option value="0">test</option></select>
    </div>
  </form>
</div>
