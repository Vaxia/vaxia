<?php

/**
 * @file
 * D7 permissions whose D11 module calls them something else: D7 name => D11
 * name. The test harness grants permissions by their D7 names.
 */

return [
  // privatemsg 2.x has one permission for reading and writing your own
  // messages.
  'read privatemsg' => 'privatemsg write messages',
  'write privatemsg' => 'privatemsg write messages',
  'delete privatemsg' => 'privatemsg delete own messages',
  'administer privatemsg settings' => 'administer privatemsg',
  // Webform 3.x results/submissions permissions.
  'access all webform results' => 'view webform submissions any node',
  'access own webform results' => 'view webform submissions own node',
  'access own webform submissions' => 'view own webform submission',
  'delete all webform submissions' => 'delete any webform submission',
  'delete own webform submissions' => 'delete own webform submission',
  'edit all webform submissions' => 'edit any webform submission',
  'edit own webform submissions' => 'edit own webform submission',
];
