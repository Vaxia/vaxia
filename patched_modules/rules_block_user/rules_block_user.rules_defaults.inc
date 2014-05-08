<?php

/**
 * @file
 * Default rule configurations for rules_block_user.
 */

function rules_block_user_default_rules_configuration() {
  $user_is_blocked = '{
    "user_has_been_blocked" : {
      "LABEL" : "User has been blocked",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "REQUIRES" : [ "rules", "rules_block_user" ],
      "ON" : [ "user_was_blocked" ],
      "DO" : [
        { "drupal_message" : { "message" : "[account:name] has been blocked.", "repeat" : 0 } }
      ]
    }
  }';
  $configs['user_is_blocked'] = rules_import($user_is_blocked);

  $user_is_unblocked = '{
    "user_has_been_unblocked" : {
      "LABEL" : "User has been unblocked",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "REQUIRES" : [ "rules", "rules_block_user" ],
      "ON" : [ "user_was_unblocked" ],
      "DO" : [
        { "drupal_message" : { "message" : "[account:name] has been unblocked.", "repeat" : 0 } }
      ]
    }
  }';
  $configs['user_is_unblocked'] = rules_import($user_is_unblocked);

  return $configs;
}
