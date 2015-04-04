<?php
/**
 * @file
 * proposal.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function proposal_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function proposal_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function proposal_node_info() {
  $items = array(
    'proposal' => array(
      'name' => t('Proposal'),
      'base' => 'node_content',
      'description' => t('The proposal system allows you to build a form out of a collection of form pieces to put questions to site votes.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('The form will be open for two weeks, then it will automatically close. If you edit it in that timespan the timer will re-set. This is to make sure everyone can get a chance to weigh in, or update their vote if they change their mind.
</br></br>
Not all site members will be able to vote in this proposal - only those currently active when it was created. This is to avoid ballot stuffing or swoop-ins from long absent players.
</br></br>
And lastly, no one can see the names or IP addresses of the people who do vote. This is so they feel safe in making their wishes known. 
</br></br>
Once the proposal closes, the results will be posted publicly for all to see. Names will still anonymous.
</br></br>
Creating a proposal is a TWO-STEP process. Fill out the information here, and on saving it - the system will take you to part two to set up your form.'),
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
