<?php
/**
 * @file
 * strikes.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function strikes_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'body'
  $field_bases['body'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(
      0 => 'node',
    ),
    'field_name' => 'body',
    'field_permissions' => array(
      'type' => 0,
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_with_summary',
  );

  // Exported field_base: 'field_associated_record'
  $field_bases['field_associated_record'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_associated_record',
    'indexes' => array(
      'nid' => array(
        0 => 'nid',
      ),
    ),
    'locked' => 0,
    'module' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'article' => 0,
        'artwork' => 0,
        'character_sheet' => 0,
        'event' => 0,
        'filedepot_folder' => 0,
        'forum' => 0,
        'items' => 0,
        'long_answer' => 0,
        'mass_contact' => 0,
        'multichoice' => 0,
        'notation' => 'notation',
        'page' => 0,
        'proposal' => 0,
        'quiz' => 0,
        'rpg_chatroom' => 0,
        'scene' => 0,
        'session_report' => 0,
        'storyhost_application' => 0,
        'strike' => 0,
        'webform' => 0,
        'wiki_page' => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'node_reference',
  );

  // Exported field_base: 'field_category'
  $field_bases['field_category'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_category',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'language' => 'Attacking other players (verbal abuse)',
        'slander' => 'Attacking other players (slander)',
        'threats' => 'Attacking other players (threats)',
        'sexual' => 'Harassing other players (sexual)',
        'hounding' => 'Harassing other players (hounding)',
        'stalking' => 'Harassing other players (IC stalking / lurking after asked not to)',
        'disruption' => 'OOC Session disruption',
        'consent' => 'Ignoring consent of other player (IC / OOC)',
        'griefing' => 'Player disturbance (griefing)',
        'extortion' => 'Player disturbance (extortion)',
        'manipulation' => 'Emotional manipulation (overt aggression)',
        'guilt-trip' => 'Emotional manipulation (guilt-tripping)',
        'passive-aggro' => 'Emotional manipulation (covert / passive aggression)',
        'baiting' => 'Emotional manipulation (baiting / goading)',
        'chaseme' => 'Emotional manipulation (chase me behavior)',
        'play-victim' => 'Emotional manipulation (projecting blame)',
        'gaslighting' => 'Emotional manipulation (gaslighting)',
        'xxxpost' => 'Posting in public room (sexual)',
        'bigotpost' => 'Posting in public room (racist or bigoted)',
        'violentpost' => 'Posting in public room (extremely violent)',
        'noncompliance' => 'Not following directions of site staff',
        'contract' => 'Breaking terms of social contract',
        'cheating' => 'Deliberate violation of System rules or Setting limits',
        'impersonation' => 'Impersonation of site staff / moderation staff',
        'hacking' => 'Unauthorized access of server or account',
        'multiaccounts' => 'Operating multiple accounts without permission',
        'guiding_principles' => 'Violating site guiding principles',
        'policy-violation' => 'Site policy: General player policy',
        'policy-social' => 'Site policy: Social dept. policy',
        'policy-setting' => 'Site policy: Setting dept. policy',
        'policy-system' => 'Site policy: System dept. policy',
        'positive' => 'Positive feedback / Check-in',
        'misc' => 'Misc',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_date_of_incident'
  $field_bases['field_date_of_incident'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_date_of_incident',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'hour' => 0,
        'minute' => 0,
        'month' => 'month',
        'second' => 0,
        'year' => 'year',
      ),
      'repeat' => 0,
      'timezone_db' => '',
      'todate' => '',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'datetime',
  );

  // Exported field_base: 'field_details'
  $field_bases['field_details'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_details',
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );

  // Exported field_base: 'field_documentation'
  $field_bases['field_documentation'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_documentation',
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'file',
    'settings' => array(
      'display_default' => 0,
      'display_field' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'file',
  );

  // Exported field_base: 'field_players'
  $field_bases['field_players'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_players',
    'indexes' => array(
      'uid' => array(
        0 => 'uid',
      ),
    ),
    'locked' => 0,
    'module' => 'user_reference',
    'settings' => array(
      'referenceable_roles' => array(
        2 => 2,
        3 => 0,
        8 => 0,
        9 => 0,
        10 => 0,
        12 => 0,
        13 => 0,
        14 => 0,
        15 => 0,
        17 => 0,
        19 => 0,
        20 => 0,
        21 => 0,
        22 => 0,
        23 => 0,
        24 => 0,
        25 => 0,
        26 => 0,
        27 => 0,
        28 => 0,
        29 => 0,
        30 => 0,
        31 => 0,
        32 => 0,
      ),
      'referenceable_status' => array(
        0 => 0,
        1 => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'user_reference',
  );

  // Exported field_base: 'field_results'
  $field_bases['field_results'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_results',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'na' => 'No additional actions taken',
        'chill' => 'Player requested to disengage in situation',
        'training' => 'Training given',
        'mediation' => 'Mediation between Player',
        'contract' => 'A contract with Social was agreed on by Player',
        'apology' => 'Private apology requested',
        'apology-public' => 'Public apology requested',
        'custody' => 'Alternate RP schedules arranged between Player',
        'break-1d' => 'Player requested to take a break from the site (1 day)',
        'break-3d' => 'Player requested to take a break from the site (3 days)',
        'break-1w' => 'Player requested to take a break from the site (1 week)',
        'yellow' => 'Yellow Strike recommended',
        'red-upgrade' => 'Yellow upgraded to Red, 3rd Yellow w/in year',
        'red' => 'Red Strike recommended',
        'site-ban' => 'Permanent site-ban recommended',
        'other' => 'Other action taken',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_severity'
  $field_bases['field_severity'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_severity',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Yellow' => 'Yellow',
        'Red' => 'Red (suspension)',
      ),
      'allowed_values_function' => '',
      'allowed_values_php' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_social_members'
  $field_bases['field_social_members'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_social_members',
    'indexes' => array(
      'uid' => array(
        0 => 'uid',
      ),
    ),
    'locked' => 0,
    'module' => 'user_reference',
    'settings' => array(
      'referenceable_roles' => array(
        2 => 2,
        3 => 0,
        8 => 0,
        9 => 0,
        10 => 0,
        12 => 0,
        13 => 0,
        14 => 0,
        15 => 0,
        17 => 0,
        19 => 0,
        20 => 0,
        21 => 0,
        22 => 0,
        23 => 0,
        24 => 0,
        25 => 0,
        26 => 0,
        27 => 0,
        28 => 0,
        29 => 0,
        30 => 0,
        31 => 0,
        1000 => 0,
      ),
      'referenceable_status' => array(
        0 => 0,
        1 => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'user_reference',
  );

  // Exported field_base: 'field_struck_player'
  $field_bases['field_struck_player'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_struck_player',
    'indexes' => array(
      'uid' => array(
        0 => 'uid',
      ),
    ),
    'locked' => 0,
    'module' => 'user_reference',
    'settings' => array(
      'referenceable_roles' => array(
        2 => 2,
        3 => 0,
        8 => 0,
        9 => 0,
        10 => 0,
        12 => 0,
        13 => 0,
        14 => 0,
        15 => 0,
        17 => 0,
        19 => 0,
        20 => 0,
        21 => 0,
        22 => 0,
        23 => 0,
        24 => 0,
        25 => 0,
        26 => 0,
        27 => 0,
        28 => 0,
        29 => 0,
        30 => 0,
        31 => 0,
        1000 => 0,
      ),
      'referenceable_status' => array(
        0 => 0,
        1 => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'user_reference',
  );

  // Exported field_base: 'field_witnesses'
  $field_bases['field_witnesses'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_witnesses',
    'indexes' => array(
      'uid' => array(
        0 => 'uid',
      ),
    ),
    'locked' => 0,
    'module' => 'user_reference',
    'settings' => array(
      'referenceable_roles' => array(
        2 => 2,
        3 => 0,
        8 => 0,
        9 => 0,
        10 => 0,
        12 => 0,
        13 => 0,
        14 => 0,
        15 => 0,
        17 => 0,
        19 => 0,
        20 => 0,
        21 => 0,
        22 => 0,
        23 => 0,
        24 => 0,
        25 => 0,
        26 => 0,
        27 => 0,
        28 => 0,
        29 => 0,
        30 => 0,
        31 => 0,
        32 => 0,
        33 => 0,
      ),
      'referenceable_status' => array(
        0 => 0,
        1 => 0,
      ),
      'view' => array(
        'args' => array(),
        'display_name' => '',
        'view_name' => '',
      ),
    ),
    'translatable' => 0,
    'type' => 'user_reference',
  );

  return $field_bases;
}
