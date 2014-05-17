<?php
/**
  * @file
  * move_incoming_form.tpl.php
  */
?>                      
                     <form id="frmIncomingFileMove" name="frmIncomingFileMove" method="post">
                        <input type="hidden" name="id" value="">
                        <table class="formtable">
                            <tr>
                                <td><label for="parent"><?php print $LANG_newfolder ?>:</label>&nbsp;</td>
                                <td><select id="movesinglefile" name="newcid" style="width:270px">
                                        <?php print $movefolder_options ?>  
                                    </select>
                                </td>
                            </tr>
                            <tr>
                                <td colspan="2" style="text-align:center;padding:15px;">
                                    <input id="btnMoveIncomingFileSubmit" class="form-submit" type="button" value="<?php print $LANG_submit ?>">
                                    <span style="padding-left:10px;">
                                        <input id="btnMoveIncomingFileCancel" class="form-submit" type="button" value="<?php print $LANG_cancel ?>">
                                    </span>
                                </td>
                            </tr>
                        </table>
                        <input type="hidden" name="token" value="<?php print $token ?>" />
                     </form>