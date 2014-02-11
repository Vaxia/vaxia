<?php
/**
 * @file
 * rpg_site_staff.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function rpg_site_staff_default_rules_configuration() {
  $items = array();
  $items['rules_add_character_and_become_a_player'] = entity_import('rules_config', '{ "rules_add_character_and_become_a_player" : {
      "LABEL" : "Add character and become a player",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Vaxia" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert--character_sheet" : { "bundle" : "character_sheet" } },
      "IF" : [
        { "NOT user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "8" : "8" } } } }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "8" : "8" } } } }
      ]
    }
  }');
  $items['rules_add_voting_player'] = entity_import('rules_config', '{ "rules_add_voting_player" : {
      "LABEL" : "Add Voting Player Comment",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "comment" ],
      "ON" : { "comment_insert" : [] },
      "IF" : [
        { "NOT user_is_blocked" : { "account" : [ "comment:author" ] } },
        { "user_has_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "8" : "8" } }
          }
        },
        { "NOT user_has_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "19" : "19" } }
          }
        }
      ],
      "DO" : [
        { "user_add_role" : {
            "account" : [ "comment:author" ],
            "roles" : { "value" : { "19" : "19" } }
          }
        },
        { "data_set" : { "data" : [ "comment:author:field-became-voter" ], "value" : "now" } }
      ]
    }
  }');
  $items['rules_make_user_eligible_for_sh_on_xp_gain'] = entity_import('rules_config', '{ "rules_make_user_eligible_for_sh_on_xp_gain" : {
      "LABEL" : "Make user eligible for SH on XP gain",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "SH Test" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "NOT user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "12" : "12" } } } },
        { "data_is" : {
            "data" : [ "user:field-xp-over-time" ],
            "op" : "\\u003E",
            "value" : "40"
          }
        }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "12" : "12" } } } },
        { "mail" : {
            "to" : "[user:mail]",
            "subject" : "[Vaxia] You are eligible to become a Storyhost",
            "message" : "Congratulations!\\r\\n\\r\\nYou have been with us for 40 XP worth of posting and RP, and you are now eligible to become a Storyhost. If you\\u0027re interested in joining our Storyhost team please check out the following link:\\r\\n\\r\\nhttp:\\/\\/vaxia.org\\/wiki\\/sh-course\\r\\n\\r\\nWe\\u0027re happy you\\u0027ve stayed with us, and we hope to share adventures for years to come!\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_rem_priv_roles_if_no_run_6mo'] = entity_import('rules_config', '{ "rules_rem_priv_roles_if_no_run_6mo" : {
      "LABEL" : "Rem priv roles if no-run 6mo",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "php", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "9" : "9", "14" : "14" } },
            "operation" : "OR"
          }
        },
        { "data_is" : {
            "data" : [ "user:created" ],
            "op" : "\\u003C",
            "value" : {
              "select" : "site:current-date",
              "date_offset" : { "value" : -15552000 }
            }
          }
        },
        { "php_eval" : { "code" : "return sh_test_check_last_run($user-\\u003Euid);" } }
      ],
      "DO" : [
        { "user_remove_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "9" : "9", "14" : "14" } }
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "4" : "4", "3" : "3" } },
            "subject" : "[Vaxia] Administrative privileges lost ",
            "message" : "[user:name] has been absent from the site for 6 months, and has lost all permissions above player as a result.\\r\\n\\r\\nYou are receiving this email because you share some of the same roles, and need to be aware of the changes. If this is a position that needs to be critically filled, please consult with other leadership on the site to recruit or nominate new members to this position.\\r\\n\\r\\nWhile we understand the need to prioritize life over game, we do need members in leadership positions to uphold their responsibilities. [user:name]\\u0027s loss of position due to absence does not preclude re-gaining the position upon return to the site - on the condition that the return is intended to be a long-term return.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ "
          }
        },
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Storyhost privileges lost ",
            "message" : "[user:name],\\r\\n\\r\\nDespite being listed as active, you have not run a session in six month\\u0027s time.  As a result, you have automatically lost your privileges as a story host, and been reverted to the role of player.\\r\\n\\r\\nWhile we understand the need for everyone to prioritize life over game, we also need those in the role of SH to uphold their responsibilities, including but not limited to running sessions for the site. Your loss of position due to absence does not prevent you from re-gaining the position if and when you return to the site, provided you retake and again complete the ASH course, including all tests and session requirements involved therein.\\r\\n\\r\\nSHs aware of sustained absences from the site have the power to set themselves on \\u0022Vacation Mode\\u0022 via their account settings.  Doing so will temporarily suspend your permissions, but you can be reactivated at any time in the future through the same settings pane.\\r\\n\\r\\nIf you have any questions about the policies on inactive SHs, use the site\\u0027s Contact form or post to the Policies forum to get clarification.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ ",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_rem_sh_role_if_absentee_6mo'] = entity_import('rules_config', '{ "rules_rem_sh_role_if_absentee_6mo" : {
      "LABEL" : "Rem priv roles if absentee 6mo",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : {
                "13" : "13",
                "10" : "10",
                "17" : "17",
                "15" : "15",
                "9" : "9",
                "20" : "20",
                "14" : "14"
              }
            },
            "operation" : "OR"
          }
        },
        { "data_is" : {
            "data" : [ "user:last-access" ],
            "op" : "\\u003C",
            "value" : {
              "select" : "site:current-date",
              "date_offset" : { "value" : -15552000 }
            }
          }
        }
      ],
      "DO" : [
        { "user_remove_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : {
                "13" : "13",
                "10" : "10",
                "17" : "17",
                "15" : "15",
                "9" : "9",
                "20" : "20",
                "14" : "14"
              }
            }
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "4" : "4", "3" : "3" } },
            "subject" : "[Vaxia] Administrative privileges lost ",
            "message" : "[user:name] has been absent from the site for 6 months, and has lost all permissions above player as a result.\\r\\n\\r\\nYou are receiving this email because you share some of the same roles, and need to be aware of the changes. If this is a position that needs to be critically filled, please consult with other leadership on the site to recruit or nominate new members to this position.\\r\\n\\r\\nWhile we understand the need to prioritize life over game, we do need members in leadership positions to uphold their responsibilities. [user:name]\\u0027s loss of position due to absence does not preclude re-gaining the position upon return to the site - on the condition that the return is intended to be a long-term return.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ "
          }
        },
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Storyhost privileges lost ",
            "message" : "[user:name],\\r\\n\\r\\nDespite being listed as active, you have been completely absent from the site for six months without logging in.  As a result, you have automatically lost your privileges as a story host, and been reverted to the role of player.\\r\\n\\r\\nWhile we understand the need for everyone to prioritize life over game, we also need those in the role of SH to uphold their responsibilities, including but not limited to running sessions for the site. Your loss of position due to absence does not prevent you from re-gaining the position if and when you return to the site, provided you retake and again complete the ASH course, including all tests and session requirements involved therein.\\r\\n\\r\\nSHs aware of sustained absences from the site have the power to set themselves on \\u0022Vacation Mode\\u0022 via their account settings.  Doing so will temporarily suspend your permissions, but you can be reactivated at any time in the future through the same settings pane.\\r\\n\\r\\nIf you have any questions about the policies on inactive SHs, use the site\\u0027s Contact form or post to the Policies forum to get clarification.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ ",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_remove_non_voting_player'] = entity_import('rules_config', '{ "rules_remove_non_voting_player" : {
      "LABEL" : "Remove Non-Voting Player",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "php", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "19" : "19" } } } },
        { "NOT php_eval" : { "code" : "return proposal_check_posted_in_last_timeframe($uid);" } }
      ],
      "DO" : [
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "19" : "19" } } } },
        { "data_set" : { "data" : [ "user:field-became-voter" ], "value" : [ "" ] } }
      ]
    }
  }');
  $items['rules_retire_inactive_user_account'] = entity_import('rules_config', '{ "rules_retire_inactive_user_account" : {
      "LABEL" : "Retire inactive user account",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Retire" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "NOT user_is_blocked" : { "account" : [ "user" ] } },
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "8" : "8" } } } },
        { "data_is" : {
            "data" : [ "user:created" ],
            "op" : "\\u003C",
            "value" : {
              "select" : "site:current-date",
              "date_offset" : { "value" : -31104000 }
            }
          }
        },
        { "data_is" : {
            "data" : [ "user:last-access" ],
            "op" : "\\u003C",
            "value" : {
              "select" : "site:current-date",
              "date_offset" : { "value" : -31536000 }
            }
          }
        }
      ],
      "DO" : [
        { "user_block" : { "account" : [ "user" ] } },
        { "mail" : {
            "to" : "bastlynn@gmail.com",
            "subject" : "[Vaxia] User blocked for inactivity [user:name]",
            "message" : "User blocked for inactivity for 1 yr on: [user:name]  - last access: [user:last-access]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_retire_unused_user_account'] = entity_import('rules_config', '{ "rules_retire_unused_user_account" : {
      "LABEL" : "Delete unused user account",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "1",
      "OWNER" : "rules",
      "TAGS" : [ "Retire" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "NOT user_is_blocked" : { "account" : [ "user" ] } },
        { "NOT user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "8" : "8" } },
            "operation" : "OR"
          }
        },
        { "data_is" : {
            "data" : [ "user:created" ],
            "op" : "\\u003C",
            "value" : { "select" : "site:current-date", "date_offset" : { "value" : -259200 } }
          }
        },
        { "data_is_empty" : { "data" : [ "user:last-login" ] } }
      ],
      "DO" : [
        { "entity_delete" : { "data" : [ "user" ] } },
        { "mail" : {
            "to" : "bastlynn@gmail.com",
            "subject" : "[Vaxia] User deleted [user:name]",
            "message" : "[Vaxia] User deleted [user:name]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_sh_general_expire'] = entity_import('rules_config', '{ "rules_sh_general_expire" : {
      "LABEL" : "SH General Expire",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "9" : "9" } } } },
        { "OR" : [
            { "data_is" : {
                "data" : [ "user:field-took-ash1" ],
                "op" : "\\u003C",
                "value" : {
                  "select" : "site:current-date",
                  "date_offset" : { "value" : -31104000 }
                }
              }
            },
            { "data_is" : {
                "data" : [ "user:field-took-ash2" ],
                "op" : "\\u003C",
                "value" : {
                  "select" : "site:current-date",
                  "date_offset" : { "value" : -31104000 }
                }
              }
            }
          ]
        }
      ],
      "DO" : [
        { "user_remove_role" : { "account" : [ "user" ], "roles" : { "value" : { "9" : "9" } } } },
        { "user_add_role" : { "account" : [ "user" ], "roles" : { "value" : { "15" : "15" } } } },
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Your SH privileges have expired",
            "message" : "You are receiving this email because it has been at least a year since you passed ASH Test One and ASH Test Two.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep your skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate and the longer the time the more so, hence the refresher.\\r\\n\\r\\nAs soon as you retake both ASH Test One and ASH Test Two you will be re-enabled automatically as a SH.\\r\\n\\r\\nASH Test 1: http:\\/\\/vaxia.org\\/content\\/ash-test-1-story-hosting-basics\\r\\nASH Test 2: http:\\/\\/vaxia.org\\/content\\/ash-test-2-renewing-shs\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "4" : "4" } },
            "subject" : "[Vaxia] A SH privileges have expired",
            "message" : "[user:name] is up for SH renewal because it has been at least a year since they passed ASH Test One and ASH Test Two.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate, and the longer the time the more so, hence the refresher.\\r\\n\\r\\nAs soon as [user:name] retakes both ASH Test One and ASH Test Two [user:name] will be re-enabled automatically as a SH.\\r\\n\\r\\nASH Test 1: http:\\/\\/vaxia.org\\/content\\/ash-test-1-story-hosting-basics\\r\\nASH Test 2: http:\\/\\/vaxia.org\\/content\\/ash-test-2-renewing-shs\\r\\n\\r\\nThis email is just a courtesy to let you know about the temporary change in status.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  $items['rules_sh_general_expire_warning'] = entity_import('rules_config', '{ "rules_sh_general_expire_warning" : {
      "LABEL" : "SH General Expire Warning",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "php", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : { "account" : [ "user" ], "roles" : { "value" : { "9" : "9" } } } },
        { "OR" : [
            { "data_is" : {
                "data" : [ "user:field-took-ash1" ],
                "value" : {
                  "select" : "site:current-date",
                  "date_offset" : { "value" : -28512000 }
                }
              }
            },
            { "data_is" : {
                "data" : [ "user:field-took-ash2" ],
                "value" : {
                  "select" : "site:current-date",
                  "date_offset" : { "value" : -28512000 }
                }
              }
            }
          ]
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Your SH privileges are up for renewal",
            "message" : "You are receiving this email because it has almost been a year since you passed ASH Test One and ASH Test Two.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep your skills sharp and consistent with the rest of the team. Over time personal style in ruling can deviate, and the longer the time the more so. We ask that you take a refresher every year.\\r\\n\\r\\nPlease be sure to retake these tests before \\u003C?php print date(\\u0027m\\/d\\/Y\\u0027, strtotime(\\u0027+1 month\\u0027)); ?\\u003E to avoid losing your permissions.\\r\\n\\r\\nASH Test 1: http:\\/\\/vaxia.org\\/content\\/ash-test-1-story-hosting-basics\\r\\nASH Test 2: http:\\/\\/vaxia.org\\/content\\/ash-test-2-renewing-shs\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
            "language" : [ "" ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "4" : "4" } },
            "subject" : "[Vaxia] A SH privileges are up for renewal",
            "message" : "[user:name] is up for SH renewal because it has almost been a year since they passed ASH Test One and ASH Test Two.\\r\\n\\r\\nJust like getting a driver\\u0027s license, we ask that SH\\u0027s retake these tests regularly so to keep skills sharp and consistent with the rest of the team.\\r\\n\\r\\n[user:name] needs to retake the ASH tests before \\u003C?php print date(\\u0027m\\/d\\/Y\\u0027, strtotime(\\u0027+1 month\\u0027)); ?\\u003E to avoid losing their permissions.\\r\\n\\r\\nASH Test 1: http:\\/\\/vaxia.org\\/content\\/ash-test-1-story-hosting-basics\\r\\nASH Test 2: http:\\/\\/vaxia.org\\/content\\/ash-test-2-renewing-shs\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  $items['rules_vacation_end'] = entity_import('rules_config', '{ "rules_vacation_end" : {
      "LABEL" : "Vacation end",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "vacation" ],
      "ON" : { "vacation_end" : [] },
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "4" : "4" } },
            "subject" : "[Vaxia] Storyhost or ASH has returned from vacation",
            "message" : "[user:name] has returned from a long-term vacation using the Vaxian vacation mode system. This email is to notify you to welcome this player back.\\r\\n\\r\\n[user:name]\\u0027s permissions have been re-enabled as they were before they left, and they are ready to fulfil those roles.\\r\\n\\r\\nIf you have any questions about [user:name] \\u0027s absence or the policies on inactive SHs, use the site\\u0027s Contact form or post to the Policies forum to get clarification.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ "
          }
        },
        { "redirect" : { "url" : "\\u003Cfront\\u003E" } },
        { "drupal_message" : { "message" : "Welcome back! Take a moment to check in with the rest of the Storyhosts so you know what you missed. If you\\u0027ve been gone for a long time - expect to have to do some catching up." } }
      ]
    }
  }');
  $items['rules_vacation_start'] = entity_import('rules_config', '{ "rules_vacation_start" : {
      "LABEL" : "Vacation start",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "vacation" ],
      "ON" : { "vacation_start" : [] },
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "4" : "4" } },
            "subject" : "[Vaxia] Storyhost or ASH has gone on vacation",
            "message" : "[user:name] has gone on a long-term vacation using the Vaxian vacation mode system. This email is to notify you to expect a long term vacation or absence on behalf of this player.\\r\\n\\r\\nWhile on vacation, [user:name] will only have permissions and access as per a player. They will not have any SH or ASH powers and will not be receiving any notices relevant to those roles.\\r\\n\\r\\nOnce [user:name] declares themselves off of vacation, the permissions will be re-enabled and they will be available to continue in the previous roles.\\r\\n\\r\\nIf you have any questions about [user:name]\\u0027s absence or the policies on inactive SHs, use the site\\u0027s Contact form or post to the Policies forum to get clarification.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ "
          }
        },
        { "redirect" : { "url" : "\\u003Cfront\\u003E" } },
        { "drupal_message" : { "message" : "Your vacation notice has gone out, roles have been removed and you are ready to hit the beach and\\/or deal with what you need to. Good luck, and we\\u0027ll see you again when you\\u0027re ready to return." } }
      ]
    }
  }');
  $items['vaxia_accounts_add_voting_player_node'] = entity_import('rules_config', '{ "vaxia_accounts_add_voting_player_node" : {
      "LABEL" : "Add Voting Player Node",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "NOT user_is_blocked" : { "account" : [ "node:author" ] } },
        { "user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "8" : "8" } } } },
        { "NOT user_has_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "19" : "19" } } } }
      ],
      "DO" : [
        { "user_add_role" : { "account" : [ "node:author" ], "roles" : { "value" : { "19" : "19" } } } },
        { "data_set" : { "data" : [ "node:author:field-became-voter" ], "value" : "now" } }
      ]
    }
  }');
  $items['vaxia_accounts_delete_non_player_user_account'] = entity_import('rules_config', '{ "vaxia_accounts_delete_non_player_user_account" : {
      "LABEL" : "Delete non-player user account",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "1",
      "OWNER" : "rules",
      "TAGS" : [ "Retire" ],
      "REQUIRES" : [ "rules", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "NOT user_is_blocked" : { "account" : [ "user" ] } },
        { "NOT user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "8" : "8" } },
            "operation" : "OR"
          }
        },
        { "data_is" : {
            "data" : [ "user:created" ],
            "op" : "\\u003C",
            "value" : { "select" : "site:current-date", "date_offset" : { "value" : -259200 } }
          }
        },
        { "data_is" : {
            "data" : [ "user:last-login" ],
            "op" : "\\u003C",
            "value" : { "select" : "site:current-date", "date_offset" : { "value" : -1209600 } }
          }
        }
      ],
      "DO" : [
        { "entity_delete" : { "data" : [ "user" ] } },
        { "mail" : {
            "to" : "bastlynn@gmail.com",
            "subject" : "[Vaxia] User deleted [user:name]",
            "message" : "[Vaxia] User deleted [user:name]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['vaxia_accounts_rem_priv_roles_if_no_run_6mo_warning'] = entity_import('rules_config', '{ "vaxia_accounts_rem_priv_roles_if_no_run_6mo_warning" : {
      "LABEL" : "Rem priv roles if no-run 6mo warning",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "php", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "user_has_role" : {
            "account" : [ "user" ],
            "roles" : { "value" : { "9" : "9", "14" : "14" } }
          }
        },
        { "data_is" : {
            "data" : [ "user:created" ],
            "value" : {
              "select" : "site:current-date",
              "date_offset" : { "value" : -12960000 }
            }
          }
        },
        { "php_eval" : { "code" : "return sh_test_check_last_run($user-\\u003Euid, \\u0027-5 months\\u0027);" } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "user:mail" ],
            "subject" : "[Vaxia] Storyhost privileges to expire in one month ",
            "message" : "[user:name],\\r\\n\\r\\nDespite being listed as active, you have not run a session in fives month\\u0027s time. As an evaluator we ask that you run a sessions once every six months. You have a month to get a session in before your permissions will expire.\\r\\n\\r\\nSHs aware of sustained absences from the site have the power to set themselves on \\u0022Vacation Mode\\u0022 via their account settings.  Doing so will temporarily suspend your permissions, but you can be reactivated at any time in the future through the same settings pane.\\r\\n\\r\\nIf you have any questions about the policies on inactive SHs, use the site\\u0027s Contact form or post to the Policies forum to get clarification.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ ",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['vaxia_sh_test_yearly_election'] = entity_import('rules_config', '{ "vaxia_sh_test_yearly_election" : {
      "LABEL" : "Yearly Election",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Positions" ],
      "REQUIRES" : [ "rules", "php", "user_stats" ],
      "ON" : { "user_stats_day_older" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "user:uid" ], "value" : "1" } },
        { "php_eval" : { "code" : "return vaxia_check_election_day();" } }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "proposal",
              "param_title" : "\\u003C?php print date(\\u0027Y\\u0027); ?\\u003E Vaxia Site Election",
              "param_author" : "1"
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Created election poll" } }
          }
        },
        { "entity_save" : { "data" : [ "entity-created" ], "immediate" : 1 } },
        { "php_eval" : { "code" : "return vaxia_setup_election_proposal($entity_created);" } }
      ]
    }
  }');
  return $items;
}