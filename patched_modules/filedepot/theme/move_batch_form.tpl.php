<?php
/**
  * @file
  * move_batch_form.tpl.php
  */
?> 
                    <form name="frmBatchMove" method="post">
                        <table class="formtable">
                            <tr>
                                <td><label for="parent"><?php print $LANG_newfolder ?>:</label>&nbsp;</td>
                                <td><select id="movebatchfiles" name="newcid" style="width:270px">
                                        <?php print $movefolder_options ?>
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2" style="text-align:center;padding:15px;">
                                    <input id="btnMoveFolderSubmit" class="form-submit" type="button" value="<?php print $LANG_submit ?>">
                                    <span style="padding-left:10px;">
                                        <input id="btnMoveFolderCancel" class="form-submit" type="button" value="<?php print $LANG_cancel ?>">
                                    </span>                                    
                                </td>
                            </tr>
                        </table>
                     </form>
