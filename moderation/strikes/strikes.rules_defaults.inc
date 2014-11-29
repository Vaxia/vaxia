<?php
/**
 * @file
 * strikes.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function strikes_default_rules_configuration() {
  $items = array();
  $items['rules_create_red_strike'] = entity_import('rules_config', '{ "rules_create_red_strike" : {
      "LABEL" : "Strikes - Create red strike",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Strikes" ],
      "REQUIRES" : [ "rules_list_conditions", "rules" ],
      "ON" : { "node_insert--notation" : { "bundle" : "notation" } },
      "IF" : [
        { "ANY" : {
            "USING" : { "list" : [ "node:field-results" ] },
            "ITEM" : { "result" : "Result" },
            "ANY" : [ { "data_is" : { "data" : [ "result" ], "value" : "red" } } ]
          }
        }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-red-strike" ] },
            "ITEM" : { "strike_user" : "Strike user" },
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "node",
                    "param_type" : "strike",
                    "param_title" : "Red Strike - [strike-user:name]",
                    "param_author" : [ "node:author" ]
                  },
                  "PROVIDE" : { "entity_created" : { "strike" : "Strike" } }
                }
              },
              { "data_set" : { "data" : [ "strike:field-severity" ], "value" : "Red" } },
              { "data_set" : { "data" : [ "strike:field-associated-record" ], "value" : [ "node" ] } },
              { "data_set" : {
                  "data" : [ "strike:field-date-of-incident" ],
                  "value" : [ "node:field-date-of-incident" ]
                }
              },
              { "data_set" : { "data" : [ "strike:field-struck-player" ], "value" : [ "strike-user" ] } },
              { "entity_save" : { "data" : [ "strike" ] } },
              { "drupal_message" : { "message" : "A red strike has been issued to [strike-user:name]." } }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_create_yellow_strike'] = entity_import('rules_config', '{ "rules_create_yellow_strike" : {
      "LABEL" : "Strikes - Create yellow strike",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Strikes" ],
      "REQUIRES" : [ "rules_list_conditions", "rules" ],
      "ON" : { "node_insert--notation" : { "bundle" : "notation" } },
      "IF" : [
        { "ANY" : {
            "USING" : { "list" : [ "node:field-results" ] },
            "ITEM" : { "result" : "Result" },
            "ANY" : [ { "data_is" : { "data" : [ "result" ], "value" : "yellow" } } ]
          }
        }
      ],
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-yellow-strike" ] },
            "ITEM" : { "strike_user" : "Strike user" },
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "node",
                    "param_type" : "strike",
                    "param_title" : "Yellow Strike - [strike-user:name] ",
                    "param_author" : [ "node:author" ]
                  },
                  "PROVIDE" : { "entity_created" : { "strike" : "Strike" } }
                }
              },
              { "data_set" : { "data" : [ "strike:field-severity" ], "value" : "Yellow" } },
              { "data_set" : { "data" : [ "strike:field-associated-record" ], "value" : [ "node" ] } },
              { "data_set" : {
                  "data" : [ "strike:field-date-of-incident" ],
                  "value" : [ "node:field-date-of-incident" ]
                }
              },
              { "data_set" : { "data" : [ "strike:field-struck-player" ], "value" : [ "strike-user" ] } },
              { "entity_save" : { "data" : [ "strike" ], "immediate" : 1 } },
              { "drupal_message" : { "message" : "A yellow strike has been issued to [strike-user:name]." } }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_notation_email'] = entity_import('rules_config', '{ "rules_notation_email" : {
      "LABEL" : "Strikes - Notation Email",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Strikes" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert--notation" : { "bundle" : "notation" } },
      "DO" : [
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-players" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "mail" : {
                  "to" : [ "list-item:mail" ],
                  "subject" : "[Vaxia] A Notation has been entered",
                  "message" : "A Notation has been added by a member of the Social department.\\r\\n\\r\\nYour account has been noted as a player who was directly involved in the incident, a player who is affected by it, or as a social member involved in the resolution of the incident.\\r\\n\\r\\nPlease keep in mind that Notations are simply how we track the course of events and behaviors over time. Notations are used to make sure that we don\\u0027t have multiple members of the Social department respond to something and so we don\\u0027t miss any patterns that form over time.\\r\\n\\r\\nThe Notation is as follows:\\r\\n\\r\\nPlayers Involved:\\r\\n[node:field-players]\\r\\n\\r\\nPlayers Affected:\\r\\n\\r\\nSocial Members:\\r\\n[node:field-social-members]\\r\\n\\r\\nCategories:\\r\\n[node:field-category]\\r\\n\\r\\nResults:\\r\\n[node:field-results]\\r\\n\\r\\nDetails:\\r\\n[node:body]\\r\\n\\r\\nAdditional Documentation:\\r\\n[node:field-documentation]\\r\\n\\r\\nURL:\\r\\n[node:url]\\r\\n\\r\\nNotations are ONLY used to provide a neutral record of warnings, training, and positive feedback.\\r\\n\\r\\nSince Notation are recorded on site and have revision control - they may be used in a grievance to provide evidence logs if needed.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
                  "language" : [ "" ]
                }
              }
            ]
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-social-members" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "mail" : {
                  "to" : [ "list-item:mail" ],
                  "subject" : "[Vaxia] A Notation has entered",
                  "message" : "A Notation has been added by a member of the Social department.\\r\\n\\r\\nYour account has been noted as a player who was directly involved in the incident, a player who is affected by it, or as a social member involved in the resolution of the incident.\\r\\n\\r\\n\\r\\nPlease keep in mind that Notations are simply how we track the course of events and behaviors over time. Notations are used to make sure that we don\\u0027t have multiple members of the Social department respond to something and so we don\\u0027t miss any patterns that form over time.\\r\\n\\r\\nThe Notation is as follows:\\r\\n\\r\\nPlayers Involved:\\r\\n[node:field-players]\\r\\n\\r\\nPlayers Affected:\\r\\n\\r\\nSocial Members:\\r\\n[node:field-social-members]\\r\\n\\r\\nCategories:\\r\\n[node:field-category]\\r\\n\\r\\nResults:\\r\\n[node:field-results]\\r\\n\\r\\nDetails:\\r\\n[node:body]\\r\\n\\r\\nAdditional Documentation:\\r\\n[node:field-documentation]\\r\\n\\r\\nURL:\\r\\n[node:url]\\r\\n\\r\\nNotations are ONLY used to provide a neutral record of warnings, training, and positive feedback.\\r\\n\\r\\nSince Notation are recorded on site and have revision control - they may be used in a grievance to provide evidence logs if needed.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
                  "language" : [ "" ]
                }
              }
            ]
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "node:field-witnesses" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "mail" : {
                  "to" : [ "list-item:mail" ],
                  "subject" : "[Vaxia] A Notation has entered",
                  "message" : "A Notation has been added by a member of the Social department.\\r\\n\\r\\nYour account has been noted as a player who was directly involved in the incident, a player who is affected by it, or as a social member involved in the resolution of the incident.\\r\\n\\r\\nPlease keep in mind that Notations are simply how we track the course of events and behaviors over time. Notations are used to make sure that we don\\u0027t have multiple members of the Social department respond to something and so we don\\u0027t miss any patterns that form over time.\\r\\n\\r\\nThe Notation is as follows:\\r\\n\\r\\nPlayers Involved:\\r\\n[node:field-players]\\r\\n\\r\\nPlayers Affected:\\r\\n\\r\\nSocial Members:\\r\\n[node:field-social-members]\\r\\n\\r\\nCategories:\\r\\n[node:field-category]\\r\\n\\r\\nResults:\\r\\n[node:field-results]\\r\\n\\r\\nDetails:\\r\\n[node:body]\\r\\n\\r\\nAdditional Documentation:\\r\\n[node:field-documentation]\\r\\n\\r\\nURL:\\r\\n[node:url]\\r\\n\\r\\nNotations are ONLY used to provide a neutral record of warnings, training, and positive feedback.\\r\\n\\r\\nSince Notation are recorded on site and have revision control - they may be used in a grievance to provide evidence logs if needed.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~",
                  "language" : [ "" ]
                }
              }
            ]
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "21" : "21" } },
            "subject" : "[Vaxia] A Notation has added",
            "message" : "A Notation has been added by a member of the Social department about an incident on site. As a lead you should be aware of Notations as they are entered.\\r\\n\\r\\nPlease keep in mind that Notations are simply how we track the course of events and behaviors over time. A Notation is not a Strike and does not provide any disciplinary action.\\r\\n\\r\\nThe Notation is as follows:\\r\\n\\r\\nPlayers Involved:\\r\\n[node:field-players]\\r\\n\\r\\nPlayers Affected:\\r\\n\\r\\nSocial Members:\\r\\n[node:field-social-members]\\r\\n\\r\\nCategories:\\r\\n[node:field-category]\\r\\n\\r\\nResults:\\r\\n[node:field-results]\\r\\n\\r\\nDetails:\\r\\n[node:body]\\r\\n\\r\\nAdditional Documentation:\\r\\n[node:field-documentation]\\r\\n\\r\\nURL:\\r\\n[node:url]\\r\\n\\r\\nNotations provide a neutral record of warnings, training, and positive feedback. Since Notation are recorded on site and have revision control - they may be used in a grievance to provide evidence logs if needed.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~"
          }
        }
      ]
    }
  }');
  $items['rules_red_strike'] = entity_import('rules_config', '{ "rules_red_strike" : {
      "LABEL" : "Strikes - Red strike",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "10",
      "OWNER" : "rules",
      "TAGS" : [ "Strikes" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "strike" : "strike" } } } },
        { "data_is" : { "data" : [ "node:field-severity" ], "value" : "Red" } }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "9" : "9", "21" : "21", "29" : "29" } },
            "subject" : "[Vaxia] A red strike has been issued [node:field-struck-player:name]",
            "message" : "A red strike has been issued against [node:field-struck-player:name].\\r\\n\\r\\nFurther questions or concerns regarding this strike should be directed to [node:author] .\\r\\n\\r\\nFurther questions, concerns, or challenges to this strike, please direct them here: http:\\/\\/vaxia.org\\/contact\\r\\n\\r\\nAs a red strike, [node:field-struck-player:name] is banned from the site thru [node:field-struck-player:field-banned-until] .\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ "
          }
        },
        { "mail" : {
            "to" : [ "node:field-struck-player:mail" ],
            "subject" : "[Vaxia] A red strike has been issued on your account",
            "message" : "A red strike has been issued to your account.\\r\\n\\r\\nFurther questions, concerns, or challenges to this strike, please direct them here: http:\\/\\/vaxia.org\\/contact\\r\\n\\r\\nAs a red strike, you are banned from the site thru [node:field-struck-player:field-banned-until] in addition to any existing bans.\\r\\n\\r\\nIf this is not your first strike within the last year, you may be banned for longer.\\r\\n\\r\\nIf this is a pattern, you may be banned for life from Vaxia. Please take advice given as part of this ban seriously.\\r\\n\\r\\nWhen you return you may be expected to agree to a contract with Social to avoid further strikes and bans.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ ",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_yellow_strike'] = entity_import('rules_config', '{ "rules_yellow_strike" : {
      "LABEL" : "Strikes - Yellow strike",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Strikes" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "strike" : "strike" } } } },
        { "data_is" : { "data" : [ "node:field-severity" ], "value" : "Yellow" } }
      ],
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "21" : "21", "29" : "29" } },
            "subject" : "[Vaxia] A yellow strike against [node:field-struck-player:name]",
            "message" : "A yellow strike has been issued against [node:field-struck-player:name].\\r\\n\\r\\nIf you have more information to provide to the situation please contact [node:author] to add your documentation. Further questions, concerns, or challenges to this strike should also be directed to [node:author] and to the SH body as a whole.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ ",
            "from" : [ "" ]
          }
        },
        { "mail" : {
            "to" : [ "node:field-struck-player:mail" ],
            "subject" : "[Vaxia] A yellow strike has been issued to your account",
            "message" : "A yellow strike has been issued to your account.\\r\\n\\r\\nFurther questions, concerns, or challenges to this strike should also be directed to [node:author] and to the SH body as a whole.\\r\\n\\r\\nIf this is at least your third yellow strike within the last year, you will also have received a red strike and a temporary ban.\\r\\n\\r\\n~~~~~~\\r\\nThis email was automatically generated by vaxia.org.\\r\\nYou are receiving this email as part of your membership at vaxia.org.\\r\\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\\r\\n~~~~~~ ",
            "from" : [ "" ],
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
