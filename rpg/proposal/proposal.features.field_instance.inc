<?php
/**
 * @file
 * proposal.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function proposal_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-proposal-body'.
  $field_instances['node-proposal-body'] = array(
    'bundle' => 'proposal',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 2,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-proposal-field_associated_files'.
  $field_instances['node-proposal-field_associated_files'] = array(
    'bundle' => 'proposal',
    'deleted' => 0,
    'description' => 'Upload any files, spreadsheets or images detailing system numbers, or other useful information to give context to this proposal.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 4,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_associated_files',
    'label' => 'Associated Files',
    'required' => 0,
    'settings' => array(
      'description_field' => 1,
      'file_directory' => 'proposal_files',
      'file_extensions' => 'txt gif jpg bmp png pdf',
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-proposal-field_associated_forum_threads'.
  $field_instances['node-proposal-field_associated_forum_threads'] = array(
    'bundle' => 'proposal',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Add any related forum threads that will provide useful context to this proposal.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'node_reference',
        'settings' => array(),
        'type' => 'node_reference_default',
        'weight' => 3,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_associated_forum_threads',
    'label' => 'Associated Forum Threads',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'node_reference',
      'settings' => array(
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'node_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'node_reference_autocomplete',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-proposal-field_grievance_members'.
  $field_instances['node-proposal-field_grievance_members'] = array(
    'bundle' => 'proposal',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Please list all members whose behavior has been noted as problematic for the associated grievance. A question regarding each site member\'s fitness for the roles they hold will be automatically created for the grievance.

After you click save, this proposal will be automatically opened to begin the grievance closure.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_grievance_members',
    'label' => 'Site members under question',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'user_reference',
      'settings' => array(
        'autocomplete_match' => 'contains',
        'autocomplete_path' => 'user_reference/autocomplete',
        'size' => 60,
      ),
      'type' => 'user_reference_autocomplete',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-proposal-field_proposal_type'.
  $field_instances['node-proposal-field_proposal_type'] = array(
    'bundle' => 'proposal',
    'default_value' => array(
      0 => array(
        'value' => 'proposal',
      ),
    ),
    'default_value_function' => '',
    'default_value_php' => '',
    'deleted' => 0,
    'description' => 'Select the type of proposal this is. Most proposals are going to be standard proposals.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_proposal_type',
    'label' => 'Type',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-proposal-taxonomy_forums'.
  $field_instances['node-proposal-taxonomy_forums'] = array(
    'bundle' => 'proposal',
    'default_value' => array(
      0 => array(
        'tid' => 16,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'question' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'taxonomy_forums',
    'label' => 'Forums',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Add any related forum threads that will provide useful context to this proposal.');
  t('Associated Files');
  t('Associated Forum Threads');
  t('Body');
  t('Forums');
  t('Please list all members whose behavior has been noted as problematic for the associated grievance. A question regarding each site member\'s fitness for the roles they hold will be automatically created for the grievance.

After you click save, this proposal will be automatically opened to begin the grievance closure.');
  t('Select the type of proposal this is. Most proposals are going to be standard proposals.');
  t('Site members under question');
  t('Type');
  t('Upload any files, spreadsheets or images detailing system numbers, or other useful information to give context to this proposal.');

  return $field_instances;
}
