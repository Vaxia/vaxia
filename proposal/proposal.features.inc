<?php
/**
 * @file
 * proposal.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function proposal_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function proposal_views_api() {
  return array("version" => "3.0");
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
      'help' => t('The proposal system allows you to build a form out of a collection of form pieces to put questions to site votes.
</br></br>
We use proposals because while a forum thread and five-star voting may give you a rough feel for the mood of the site on a topic, it doesn\'t account for players who - for one reason or another - do not speak out on a forum thread. Silence does not at all indicate agreement, so we need to explicitly ask for a site vote when the topic is important.
</br></br>
Proposals are meant to be a way to formally make a request for the input of site members, so there are a few restrictions here to keep in mind. The form will be open for two weeks, after which it will automatically close. If you edit it in that timespan the timer will re-set. This is to make sure everyone can get a chance to weigh in. Not all site members will be able to vote in this proposal - only those currently active when you create it. This is to avoid ballot stuffing or swoop-ins from long absent players. And lastly, you won\'t be able to see the names or IP addresses of the people who do vote. This is so we can make sure they feel safe and secure in making their wishes known. 
</br></br>
You should know that once the proposal closes, the results will be posted publicly for all to see. Names will still anonymous.'),
    ),
  );
  return $items;
}
